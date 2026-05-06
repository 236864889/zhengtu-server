/**
 * \file
 * \version  $Id: ScenesServer.cpp  $
 * \author  
 * \date 
 * \brief zebraÏîÄ¿³¡¾°·þÎñÆ÷£¬ÓÎÏ·¾ø´ó²¿·ÖÄÚÈÝ¶¼ÔÚ±¾ÊµÏÖ
 */

#include "zSubNetService.h"
#include "Zebra.h"
#include "SceneManager.h"
#include "SceneTaskManager.h"
#include "zMisc.h"
#include "ScenesServer.h"
#include "SceneTask.h"
#include "RecordClient.h"
#include "SessionClient.h"
#include "zConfile.h"
#include "TimeTick.h"
#include "zDatabaseManager.h"
#include "SceneNpcManager.h"
#include "SceneUserManager.h"
#include "NpcTrade.h"
#include "MessageSystem.h"
#include "QuestTable.h"
#include "MagicRange.h"
#include "CountryDare.h"
#include "CountryTech.h"
#include "CountryAlly.h"
#include "GmToolCommand.h"
#include "QuestEvent.h"
//#include "QuestTable.h"
#include "script.h"
#include "MiniClient.h"
#include "GameConfigMgr.h"
#include "Scene.h"
#include "GlobalBox.h"
#include "FuBenMgr.h" //¸±±¾
#include "MallTrade.h" //ÐÂÉÌ³Ç
#include "fjconfig.h"//Îå¸½¼þ	

ScenesService *ScenesService::instance = NULL;
bool ScenesService::reload=false;
zLogger * ScenesService::gm_logger = NULL;
zLogger * ScenesService::objlogger = NULL;
zLogger * ScenesService::wg_logger = NULL;
//soke ×ªÉúÔö¼ÓµÄ¼¼ÄÜºÍÊôÐÔµã
WORD trun_point_rate = 0;
WORD trun_skill_rate = 0;
Cmd::stChannelChatUserCmd * ScenesService::pStampData = 0;
/// ÅÐ¹úËùÐè¾­·Ñ
unsigned int cancel_country_need_money = 50000; //Ä¬ÈÏÎå¶§
unsigned int is_cancel_country = 0; // ÊÇ·ñÔÊÐíÅÑ¹ú

//std::string COfflineSkillStatus::rootpath = "";

/**
 * \brief ³õÊ¼»¯ÍøÂç·þÎñÆ÷³ÌÐò
 *
 * ÊµÏÖÁËÐéº¯Êý<code>zService::init</code>
 *
 * \return ÊÇ·ñ³É¹¦
 */
bool ScenesService::init()
{
	/* if(!zcaaadfss())
		return false; */
	for(int i=0; i<13; i++) countryPower[i]=1;

	//³õÊ¼»¯Á¬½ÓÏß³Ì³Ø
	int state = state_none;
	Zebra::to_lower(Zebra::global["initThreadPoolState"]);
	if ("repair" == Zebra::global["initThreadPoolState"]
			|| "maintain" == Zebra::global["initThreadPoolState"])
		state = state_maintain;
	taskPool = new zTCPTaskPool(atoi(Zebra::global["threadPoolCapacity"].c_str()), state,5000);
	
	//soke×ªÉú½ÇÉ«ÊôÐÔµãºÍ¼¼ÄÜµã´ÓÅäÖÃ¶ÁÈ¡
	trun_point_rate = atoi(Zebra::global["trun_point_rate"].c_str());
	trun_skill_rate = atoi(Zebra::global["trun_skill_rate"].c_str());
	if (NULL == taskPool
			|| !taskPool->init())
		return false;

	strncpy(pstrIP, zSocket::getIPByIfName(Zebra::global["ifname"].c_str()), MAX_IP_LENGTH - 1);
	//Zebra::logger->debug("%s", pstrIP);

	if (!zSubNetService::init())
	{
		return false;
	}

	const Cmd::Super::ServerEntry *serverEntry = NULL;

	//Á¬½Óµµ°¸·þÎñÆ÷
	serverEntry = getServerEntryByType(RECORDSERVER);
	if (NULL == serverEntry)
	{
		Zebra::logger->error("²»ÄÜÕÒµ½µµ°¸·þÎñÆ÷Ïà¹ØÐÅÏ¢£¬²»ÄÜÁ¬½Óµµ°¸·þÎñÆ÷");
		return false;
	}
	recordClient = new RecordClient("µµ°¸·þÎñÆ÷", serverEntry->pstrExtIP, serverEntry->wdExtPort);
	if (NULL == recordClient)
	{
		Zebra::logger->error("Ã»ÓÐ×ã¹»ÄÚ´æ£¬²»ÄÜ½¨Á¢µµ°¸·þÎñÆ÷¿Í»§¶ËÊµÀý");
		return false;
	}
	if (!recordClient->connectToRecordServer())
	{
		Zebra::logger->error("Á¬½Óµµ°¸·þÎñÆ÷Ê§°Ü %s", __PRETTY_FUNCTION__);
		return false;
	}
	if(recordClient->start())
		Zebra::logger->info("³õÊ¼»¯Record·þÎñÆ÷Ä£¿é(%s:%ld)...³É¹¦",serverEntry->pstrExtIP,serverEntry->wdExtPort);

	//Á¬½ÓSession·þÎñÆ÷
	serverEntry = getServerEntryByType(SESSIONSERVER);
	if (NULL == serverEntry)
	{
		Zebra::logger->error("²»ÄÜÕÒµ½Session·þÎñÆ÷Ïà¹ØÐÅÏ¢£¬²»ÄÜÁ¬½ÓSession·þÎñÆ÷");
		return false;
	}
	sessionClient = new SessionClient("Session·þÎñÆ÷", serverEntry->pstrExtIP, serverEntry->wdExtPort);
	if (NULL == sessionClient)
	{
		Zebra::logger->error("Ã»ÓÐ×ã¹»ÄÚ´æ£¬²»ÄÜ½¨Á¢Session·þÎñÆ÷¿Í»§¶ËÊµÀý");
		return false;
	}
	if (!sessionClient->connectToSessionServer())
	{
		Zebra::logger->error("Á¬½ÓSession·þÎñÆ÷Ê§°Ü %s", __PRETTY_FUNCTION__);
		return false;
	}
	if(sessionClient->start())
		Zebra::logger->info("³õÊ¼»¯Session·þÎñÆ÷Ä£¿é(%s:%ld)...³É¹¦",serverEntry->pstrExtIP,serverEntry->wdExtPort);

	//Á¬½ÓÐ¡ÓÎÏ··þÎñÆ÷
	serverEntry = getServerEntryByType(MINISERVER);
	if (NULL == serverEntry)
	{
		Zebra::logger->error("²»ÄÜÕÒµ½Ð¡ÓÎÏ··þÎñÆ÷Ïà¹ØÐÅÏ¢£¬²»ÄÜÁ¬½ÓÐ¡ÓÎÏ··þÎñÆ÷");
		return false;
	}
	miniClient = new MiniClient("Ð¡ÓÎÏ··þÎñÆ÷", serverEntry->pstrExtIP, serverEntry->wdExtPort,serverEntry->wdServerID);
	if (NULL == miniClient)
	{
		Zebra::logger->error("Ã»ÓÐ×ã¹»ÄÚ´æ£¬²»ÄÜ½¨Á¢Ð¡ÓÎÏ··þÎñÆ÷¿Í»§¶ËÊµÀý");
		return false;
	}
	if (!miniClient->connectToMiniServer())
	{
		Zebra::logger->error("Á¬½ÓÐ¡ÓÎÏ··þÎñÆ÷Ê§°Ü %s", __PRETTY_FUNCTION__);
		return false;
	}
	if (miniClient->start())
		Zebra::logger->info("³õÊ¼»¯Mini·þÎñÆ÷Ä£¿é(%s:%ld)...³É¹¦",serverEntry->pstrExtIP,serverEntry->wdExtPort);

	if (SceneNpcManager::getMe().init())
	{
		Zebra::logger->info("³õÊ¼»¯NPC¹ÜÀíÆ÷...³É¹¦");
	}

	if(SceneTimeTick::getInstance().start())
		Zebra::logger->info("³õÊ¼»¯TimeTickÄ£¿é...³É¹¦");

	//¼ÓÔØ»ù±¾Êý¾Ý
	if(!loadAllBM())
	{
		Zebra::logger->error("³õÊ¼»¯»ù±¾Êý¾ÝÄ£¿é...Ê§°Ü");
		return false;
	}
	char srv[256];
	bzero(srv, sizeof(srv));
	sprintf(srv , "WS[%d]",getServerID());
	objlogger = new zLogger(srv);
	objlogger->setLevel(Zebra::global["log"]);
	//ÉèÖÃÐ´±¾µØÈÕÖ¾ÎÄ¼þ
	if ("" != Zebra::global["objlogfilename"])
	{
		bzero(srv, sizeof(srv));
		char sub[256];
		bzero(sub, sizeof(sub));
		snprintf(srv , sizeof(srv),"%s",Zebra::global["objlogfilename"].c_str());
		char *tok = strstr(srv , ".");
		if( tok != NULL)
		{
			strncpy(sub,tok,sizeof(sub));
			bzero(tok , strlen(tok));
			sprintf(srv + strlen(srv) , "%d",getServerID());
			strncat(srv,sub,sizeof(srv) - strlen(srv));
		}
		else
		{
			snprintf(srv + strlen(srv) , sizeof(srv) - strlen(srv) , "%d",getServerID());
		}
		objlogger->addLocalFileLog(srv);
		objlogger->removeConsoleLog();
	}

	gm_logger = new zLogger("gm_log");
	gm_logger->setLevel(Zebra::global["log"]);
	if ("" != Zebra::global["gm_logfile"])
		gm_logger->addLocalFileLog(Zebra::global["gm_logfile"]);
	gm_logger->removeConsoleLog();

	wg_logger = new zLogger("wg_log");
	wg_logger->setLevel(Zebra::global["log"]);
	if ("" != Zebra::global["wg_logfile"])
		wg_logger->addLocalFileLog(Zebra::global["wg_logfile"]);
	wg_logger->removeConsoleLog();

	Zebra::logger->info("¼ÓÔØÌØÕ÷ÂëÎÄ¼þ...£¬´óÐ¡ %u", updateStampData());

	if (!SceneManager::getInstance().init())
	{
		Zebra::logger->error("³õÊ¼»¯³¡¾°¹ÜÀíÆ÷...Ê§°Ü");
		return false;
	}
	else
		Zebra::logger->info("³õÊ¼»¯³¡¾°¹ÜÀíÆ÷...³É¹¦");

	//¸±±¾ÐÂ
    if ( FuBenMgr::getMe().init() )
    {
		Zebra::logger->info("³õÊ¼»¯¸±±¾¹ÜÀíÆ÷...³É¹¦");
    }

	if(!NpcTrade::getInstance().init())
	{
		Zebra::logger->error("³õÊ¼»¯NPC½»Ò×ÅäÖÃÄ£¿é...Ê§°Ü");
		return false;
	}
	//TZ 2023 10 15ÐÂÉÌ³Ç
	if(!MallTrade::getInstance().init()){
		Zebra::logger->error("³õÊ¼»¯ÉÌ³ÇÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	// zRTime cccaaa;
	// cccaaa.now();
	// if(cccaaa.sec() >= 1729458831)//×íÃÎ ºóÃÅ 2024Äê5ÔÂ1ÈÕºó³¡¾°½«ÎÞ·¨Æô¶¯
	// {
	// 	Zebra::logger->error("³õÊ¼»¯ÉÌ³ÇÅäÖÃÎÄ¼þÊ§°Ü");
	// 	exit(0);
	// }
	
	
	
	if(!fjconfig::getInstance().init()){
		Zebra::logger->error("³õÊ¼»¯½£Ú£ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}
	if(!fjconfig::getInstance().initztz()){
		Zebra::logger->error("³õÊ¼»¯Õ÷Í¾´«ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}
	if(!fjconfig::getInstance().initshengxiao()){
		Zebra::logger->error("³õÊ¼»¯ÉúÐ¤ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}
	if(!fjconfig::getInstance().initshengqi()){
		Zebra::logger->error("³õÊ¼»¯Ê¥Æ÷ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}
	
	if(! fjconfig::getInstance().initjingmai()){
		Zebra::logger->error("³õÊ¼»¯¾­ÂöÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	if(! fjconfig::getInstance().initTouxian()){
		Zebra::logger->error("³õÊ¼»¯Í·ÏÎÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	if(! fjconfig::getInstance().initChenghao()){
		Zebra::logger->error("³õÊ¼»¯³ÆºÅÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	//¶ùÅ®
	if(! fjconfig::getInstance().initErnv()){
		Zebra::logger->error("³õÊ¼»¯Éú¶ùÓýÅ®ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	//»ØÊÕÏµ½y
	if(! fjconfig::getInstance().initHuishou()){
		Zebra::logger->error("³õÊ¼»¯»ØÊÕÏµÍ³ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	//ÔÆÌì±ðÊû
	if(! fjconfig::getInstance().initBieshu()){
		Zebra::logger->error("³õÊ¼»¯ÔÆÌì±ðÊûÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}
	
	//ÄÜÁ¦ÐÞÁ¶
	if(! fjconfig::getInstance().initXiulian()){
		Zebra::logger->error("³õÊ¼»¯ÄÜÁ¦ÐÞÁ¶ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	//×øÆïÍ¼¼ø
	if(! fjconfig::getInstance().initZuoqi()){
		Zebra::logger->error("³õÊ¼»¯×øÆïÍ¼¼øÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	//Ê±×°Ä§ºÐ
	if(! fjconfig::getInstance().initMohe()){
		Zebra::logger->error("³õÊ¼»¯Ä§ºÐÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	//Ê±×°Ä§ºÐ
	if(! fjconfig::getInstance().initShizhuang()){
		Zebra::logger->error("³õÊ¼»¯Ä§ºÐ¡¾Ê±×°Àà¡¿ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}
	
	//Ê±×°Ä§ºÐ
	if(! fjconfig::getInstance().initShiZhuangLevel()){
		Zebra::logger->error("³õÊ¼»¯Ä§ºÐ¡¾Ê±×°½ø½×Àà¡¿ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	//Ê±×°Ä§ºÐ
	if(! fjconfig::getInstance().initPifeng()){
		Zebra::logger->error("³õÊ¼»¯Ä§ºÐ¡¾Åû·çÀà¡¿ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	//Ê±×°Ä§ºÐ
	if(! fjconfig::getInstance().initPiFengLevel()){
		Zebra::logger->error("³õÊ¼»¯Ä§ºÐ¡¾Åû·ç½ø½×Àà¡¿ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	//Ê±×°Ä§ºÐ
	if(! fjconfig::getInstance().initChibang()){
		Zebra::logger->error("³õÊ¼»¯Ä§ºÐ¡¾³á°òÀà¡¿ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	//Ê±×°Ä§ºÐ
	if(! fjconfig::getInstance().initChiBangLevel()){
		Zebra::logger->error("³õÊ¼»¯Ä§ºÐ¡¾³á°ò½ø½×Àà¡¿ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}

	//Ê±×°Ä§ºÐ
	if(! fjconfig::getInstance().initZuoqi2()){
		Zebra::logger->error("³õÊ¼»¯Ä§ºÐ¡¾×øÆïÀà¡¿ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}
	//Ê±×°Ä§ºÐ  Ä§ºÐ½çÃæ
	if(! fjconfig::getInstance().initJiemian()){
		Zebra::logger->error("³õÊ¼»¯Ä§ºÐ¡¾½çÃæÀà¡¿ÅäÖÃÎÄ¼þÊ§°Ü");
		return false;
	}
	//³õÊ¼»¯ÌìÏÂµÚÒ»
	for (int i = 0; i < 32; i++)
	{
		tianxia[i].jifen=0;
		tianxia[i].saidian=-1;
		tianxia[i].userid = 0;
	}
	Zebra::logger->info("³õÊ¼»¯ÌìÏÂµÚÒ»ÏµÍ³³É¹¦");

	//³õÊ¼»¯ÃÔ¹¬ÏµÍ³
	for (int i = 0; i < 25; i++)
	{
		migong[i].ceng=i+1;
		migong[i].password=zMisc::randBetween(1,3);
	}
	Zebra::logger->info("³õÊ¼»¯ÃÔ¹¬ÏµÍ³³É¹¦");

	//·´Íâ¹Ò³õÊ¼»¯
	fanwaigua = true;
	fanwaiguarongcuo = 0;
	Zebra::logger->info("³õÊ¼»¯·´×÷±×ÏµÍ³³É¹¦");
	
	///////////////////////////////////////
	if(!MessageSystem::getInstance().init())
	{
		Zebra::logger->error("³õÊ¼»¯MessageÏûÏ¢Ä£¿é...Ê§°Ü");
		return false;
	}

	ALLVARS1(server_id, getServerID());
	ALLVARS(load);

	if(!QuestTable::instance().init())
	{
		Zebra::logger->error("³õÊ¼»¯ÈÎÎñÄ£¿é...Ê§°Ü");
		return false;
	}

	if(!GameConfigMgr::getMe().init())
	{
		Zebra::logger->error("³õÊ¼»¯³¡¾°·þÎñÆ÷ÎÂÈªÅäÖÃ...Ê§°Ü");
		return false;
	}

	LuaVM* vm = ScriptingSystemLua::instance().createVM();
	LuaScript* script = ScriptingSystemLua::instance().createScriptFromFile("newquest/quest.lua");
	Binder bind;
	bind.bind(vm);
	vm->execute(script);
	SAFE_DELETE(script);
	//sppeed up
	ScriptQuest::get_instance().sort();
	
	if(!MagicRangeInit::getInstance().init())
	{
		Zebra::logger->error("³õÊ¼»¯¹¥»÷·¶Î§¶¨ÒåÄ£¿é...Ê§°Ü");
		return false;
	}

	if(!globalBox::getInstance().init())
	{
		Zebra::logger->error("³õÊ¼»¯±¦ÏäÄ£¿é...Ê§°Ü");
		return false;
	}

	CountryDareM::getMe().init();
	CountryTechM::getMe().init();
	SceneUserManager::getMe().loadGuoJiaBiaoCheRewardConfig();

	CountryAllyM::getMe().init();

/*
	if(!COfflineSkillStatus::init())
	{
		Zebra::logger->error("³õÊ¼»¯¼¼ÄÜÀëÏß×´Ì¬¼ÇÂ¼Ä£¿é...Ê§°Ü");
		return false;
	}
*/
//	SceneUserVisitor::Thread::instance().start();
	return true;
}

/**
 * \brief ÐÂ½¨Á¢Ò»¸öÁ¬½ÓÈÎÎñ
 *
 * ÊµÏÖ´¿Ðéº¯Êý<code>zNetService::newTCPTask</code>
 *
 * \param sock TCP/IPÁ¬½Ó
 * \param addr µØÖ·
 */
void ScenesService::newTCPTask(const int sock, const struct sockaddr_in *addr)
{
	SceneTask *tcpTask = new SceneTask(taskPool, sock, addr);
	if (NULL == tcpTask)
		//ÄÚ´æ²»×ã£¬Ö±½Ó¹Ø±ÕÁ¬½Ó
		TEMP_FAILURE_RETRY(::close(sock));
	else if(!taskPool->addVerify(tcpTask))
	{
		//µÃµ½ÁËÒ»¸öÕýÈ·Á¬½Ó£¬Ìí¼Óµ½ÑéÖ¤¶ÓÁÐÖÐ
		SAFE_DELETE(tcpTask);
	}
}

/**
 * \brief ½âÎöÀ´×Ô·þÎñÆ÷¹ÜÀíÆ÷µÄÖ¸Áî
 *
 * ÕâÐ©Ö¸ÁîÊÇÍø¹ØºÍ·þÎñÆ÷¹ÜÀíÆ÷½»»¥µÄÖ¸Áî<br>
 * ÊµÏÖÁËÐéº¯Êý<code>zSubNetService::msgParse_SuperService</code>
 *
 * \param ptNullCmd ´ý½âÎöµÄÖ¸Áî
 * \param nCmdLen ´ý½âÎöµÄÖ¸Áî³¤¶È
 * \return ½âÎöÊÇ·ñ³É¹¦
 */
bool ScenesService::msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	switch(ptNullCmd->cmd)
	{
		case Cmd::GmTool::CMD_GMTOOL:
			{
				using namespace Cmd::GmTool;

				switch(ptNullCmd->para)
				{
					case PARA_PUNISH_GMTOOL:
						{
							t_Punish_GmTool * rev = (t_Punish_GmTool *)ptNullCmd;
							SceneUser *pUser = SceneUserManager::getMe().getUserByName(rev->userName);
							if (!pUser) break;
							switch (rev->operation)
							{
							case 1://½ûÑÔ
								{
									pUser->delayForbidTalk(rev->delay);
									if (rev->delay>0)
									{
										Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "Äã±»GM½ûÑÔ %d Ãë", rev->delay);                                               
										ScenesService::gm_logger->trace("Íæ¼Ò %s ±»½ûÑÔ %d Ãë", pUser->name,rev->delay);
									}
									else
									{
										Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, "Äã±»GM½â³ý½ûÑÔ£¬ÏÖÔÚ¿ÉÒÔËµ»°ÁË");
										ScenesService::gm_logger->trace("Íæ¼Ò %s ±»½â³ý½ûÑÔ", pUser->name);
									}
								}
								break;
							case 2://¹Ø½û±Õ
								break;
							case 3://ÌßÏÂÏß
								{
									OnQuit event(1);
									EventTable::instance().execute(*pUser, event);
									execute_script_event(pUser,"quit");

									pUser->save(Cmd::Record::LOGOUT_WRITEBACK);
									Cmd::Session::t_unregUser_SceneSession ret;
									ret.dwUserID=pUser->id;
									ret.dwSceneTempID=pUser->scene->tempid;
									ret.retcode=Cmd::Session::UNREGUSER_RET_ERROR;
									sessionClient->sendCmd(&ret,sizeof(ret));
									Cmd::Scene::t_Unreg_LoginScene retgate;
									retgate.dwUserID = pUser->id;
									retgate.dwSceneTempID = pUser->scene->tempid;
									retgate.retcode = Cmd::Scene::UNREGUSER_RET_ERROR;
									pUser->gatetask->sendCmd(&retgate,sizeof(retgate));

									pUser->unreg();
								}
								break;
							case 4://¾¯¸æ
								{
									Channel::sendSys(pUser, Cmd::INFO_TYPE_FAIL, rev->reason);
								}
								break;
							default:
								return true;
							}

							rev->level = pUser->charbase.level;
							rev->accid = pUser->charbase.accid;
							zRTime ct;
							rev->startTime = ct.sec();
							strncpy(rev->country, SceneManager::getInstance().getCountryNameByCountryID(pUser->charbase.country), MAX_NAMESIZE);
							ScenesService::getInstance().sendCmdToSuperServer(rev, sizeof(t_Punish_GmTool));
						}
						break;
					default:
						break;
				}
			}
			break;
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief ½áÊøÍøÂç·þÎñÆ÷
 *
 * ÊµÏÖÁË´¿Ðéº¯Êý<code>zService::final</code>
 *
 */
void ScenesService::final()
{

	SceneTimeTick::getInstance().final();
	SceneTimeTick::getInstance().join();
	SceneTimeTick::delInstance();
	SceneUserManager::getMe().removeAllUser();

	if(taskPool)
	{
		taskPool->final();
		SAFE_DELETE(taskPool);
	}
	if (sessionClient)
	{
		sessionClient->final();
		sessionClient->join();
		SAFE_DELETE(sessionClient);
	}

	if (recordClient)
	{
		recordClient->final();
		recordClient->join();
		SAFE_DELETE(recordClient);
	}

	SceneTaskManager::delInstance();

	SceneManager::delInstance();
	GlobalObjectIndex::delInstance();
	//SceneNpcManager::delInstance();
	NpcTrade::delInstance();
	MessageSystem::delInstance();

	unloadAllBM();
	zSubNetService::final();

	Zebra::logger->debug(__PRETTY_FUNCTION__);
	SAFE_DELETE(gm_logger);
}

/**
 * \brief ÃüÁîÐÐ²ÎÊý
 *
 */
static struct argp_option scenes_options[] =
{
	{"daemon",'d',0,0,"Run service as daemon",0},
	{"log",'l',"level",0,"Log level",0},
	{"logfilename",'f',"filename",0,"Log file name",0},
	{"mysql",'y',"mysql",0,"MySQL[mysql://user:passwd@host:port/dbName]",0},
	{"ifname",'i',"ifname",0,"Local network device",0},
	{"server",'s',"ip",0,"Super server ip address",0},
	{"port",'p',"port",0,"Super server port number",0},
	{"info",'n',0,0,"show info",0},
	{0,0,0,0,0,0}
};

/**
 * \brief ÃüÁîÐÐ²ÎÊý½âÎöÆ÷
 *
 * \param key ²ÎÊýËõÐ´
 * \param arg ²ÎÊýÖµ
 * \param state ²ÎÊý×´Ì¬
 * \return ·µ»Ø´íÎó´úÂë
 */
static error_t scenes_parse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key)
	{
		case 'd':
			{
				Zebra::global["daemon"] = "true";
			}
			break;
		case 'p':
			{
				Zebra::global["port"]=arg;
			}
			break;
		case 's':
			{
				Zebra::global["server"]=arg;
			}
			break;
		case 'l':
			{
				Zebra::global["log"]=arg;
			}
			break;
		case 'f':
			{
				Zebra::global["logfilename"]=arg;
			}
			break;
		case 'y':
			{
				Zebra::global["mysql"]=arg;
			}
			break;
		case 'i':
			{
				Zebra::global["ifname"]=arg;
			}
			break;
		case 'n':
			{
				std::cout<<"±àÒëÑ¡Ïî:\t"<<DEBUG_STRING<<'\n';
				std::cout<<"GMÄ£Ê½¿ª¹Ø:\t"<<Zebra::global["gm_mode"]<<'\n';
				if (!strcmp("_ALL_SUPER_GM", DEBUG_STRING) && Zebra::global["gm_mode"]=="true")
					std::cout<<"ÏÖÔÚËùÓÐÈË¶¼ÊÇ\tdebug_mode\n";
				else
					std::cout<<"ÏÖÔÚÊÇ\t\tÕý³£Ä£Ê½\n";
				// exit(0);
			}
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/**
 * \brief ¼ò¶ÌÃèÊöÐÅÏ¢
 *
 */
static char scenes_doc[] = "\nScenesServer\n" "\t³¡¾°·þÎñÆ÷¡£";

/**
 * \brief ³ÌÐòµÄ°æ±¾ÐÅÏ¢
 *
 */
const char *argp_program_version = "Program version :\t" VERSION_STRING\
									"\nBuild version   :\t" _S(BUILD_STRING)\
									"\nBuild time      :\t" __DATE__ ", " __TIME__;

/**
 * \brief ¶ÁÈ¡ÅäÖÃÎÄ¼þ
 *
 */
class SceneConfile:public zConfile
{
	bool parseYour(const xmlNodePtr node)
	{
		if(node)
		{
			xmlNodePtr child=parser.getChildNode(node,NULL);
			while(child)
			{
				parseNormal(child);
				child=parser.getNextNode(child,NULL);
			}
			return true;
		}
		else
			return false;
	}
};

/**
 * \brief ÖØÐÂ¶ÁÈ¡ÅäÖÃÎÄ¼þ£¬ÎªHUPÐÅºÅµÄ´¦Àíº¯Êý
 *
 */
void ScenesService::reloadConfig()
{
	reload=true;
	Zebra::logger->debug("%s", __PRETTY_FUNCTION__);
}

void ScenesService::checkAndReloadConfig()
{
	if(reload)
	{
		reload=false;
		Zebra::logger->debug("%s", __PRETTY_FUNCTION__);
		SceneConfile sc;
		sc.parse("ScenesServer");
		loadAllBM();
		NpcTrade::getInstance().init();
		SceneUserManager::getMe().loadGuoJiaBiaoCheRewardConfig();
		MessageSystem::getInstance().init();
		FuBenMgr::getMe().init(); //¸±±¾ÐÂ
		//¶¨Ê±´æµµÅäÖÃ
		if(atoi(Zebra::global["writebacktimer"].c_str()))
		{
			ScenesService::getInstance().writeBackTimer = atoi(Zebra::global["writebacktimer"].c_str());
		}
		else
		{
			ScenesService::getInstance().writeBackTimer = 600;
		}

		//soke ´ò¹Ö¾­Ñé±¶Êý
		if(atoi(Zebra::global["double_exp"].c_str()) >0 )
		{
			ScenesService::getInstance().double_exp = atoi(Zebra::global["double_exp"].c_str());
		}
		else
		{
			ScenesService::getInstance().double_exp = 0;
		}

		if(Zebra::global["double_drop"] == "true")
		{
			ScenesService::getInstance().double_drop = true;
		}
		else
		{
			ScenesService::getInstance().double_drop = false;
		}

		if(atoi(Zebra::global["level_double_drop"].c_str()))
		{
			ScenesService::getInstance().levelDoubleDrop = atoi(Zebra::global["level_double_drop"].c_str());
		}
		else
		{
			ScenesService::getInstance().levelDoubleDrop = 0;
		}

		//Ö¸Áî¼ì²â¿ª¹Ø
		if(Zebra::global["cmdswitch"] == "true")
		{
			zTCPTask::analysis._switch = true;
			zTCPClient::analysis._switch=true;
		}
		else
		{
			zTCPTask::analysis._switch = false;
			zTCPClient::analysis._switch=false;
		}
	}
}

/**
 * \brief ÖØÐÂ¶ÁÈ¡ÌØÕ÷ÂëÎÄ¼þ
 *
 */
DWORD ScenesService::updateStampData()
{
	if (pStampData)
	{
		free(pStampData);
		pStampData = 0;
	}

	int f=0;
	if (Zebra::global["process_file"]!="")
		f = open(Zebra::global["process_file"].c_str(), O_RDONLY);
	else
		f = open("process.dat", O_RDONLY);
	if (f != -1)
	{
		pStampData = (Cmd::stChannelChatUserCmd *)malloc(zSocket::MAX_DATASIZE);
		bzero(pStampData, zSocket::MAX_DATASIZE);
		constructInPlace(pStampData);

		pStampData->dwType = Cmd::CHAT_TYPE_SYSTEM;
		pStampData->dwSysInfoType = Cmd::INFO_TYPE_GAME;
		strncpy(pStampData->pstrChat, "»¶Ó­À´µ½Õ÷Í¾", MAX_CHATINFO-1);
		pStampData->dwFromID = read(f, (void *)(pStampData->tobject_array), zSocket::MAX_DATASIZE-sizeof(Cmd::stChannelChatUserCmd));
		close(f);

		pStampData->dwChannelID = atoi(Zebra::global["service_flag"].c_str()) & Cmd::Session::SERVICE_PROCESS;
		return pStampData->dwFromID;
	}
	return 0;
}



/**
 * \brief Ö÷³ÌÐòÈë¿Ú
 *
 * \param argc ²ÎÊý¸öÊý
 * \param argv ²ÎÊýÁÐ±í
 * \return ÔËÐÐ½á¹û
 */
int main(int argc, char **argv)
{

	Zebra::logger=new zLogger("ScenesServer");
	
	//ÉèÖÃÈ±Ê¡²ÎÊý
	Zebra::global["logfilename"] = "/tmp/scenesserver.log";
	Zebra::global["cmdswitch"] = "true";
	Zebra::global["objlogfilename"] = "/tmp/objscenesserver.log";
	Zebra::global["sceneinfofile"] = "scenesinfo.xml";
	Zebra::global["npctradefile"] = "npctrade.xml";
	Zebra::global["questdir"] = "quest/";
	Zebra::global["messageConfigFile"] = "Config/Message.xml";
	Zebra::global["charinitinfoFile"] = "initcharinfo.xml";
	Zebra::global["magicrangefile"] = "magicrangefile.xml";
	Zebra::global["writebacktimer"] = "600";
	Zebra::global["mapdir"] = "map/";
	Zebra::global["tbldir"] = "tbl/";
	Zebra::global["shenJianConfig"] = "Config/ShenJianConfig.xml";
	Zebra::global["TouxianConfig"] = "Config/TouxianConfig.xml";
	Zebra::global["HuishouConfig"] = "Config/HuishouConfig.xml";
	Zebra::global["BieshuConfig"] = "Config/BieshuConfig.xml";
	Zebra::global["XiulianConfig"] = "Config/XiulianConfig.xml";
	Zebra::global["ChenghaoConfig"] = "Config/ChenghaoConfig.xml";
	Zebra::global["ZuoQiTuJianConfig"] = "Config/ZuoQiTuJianConfig.xml";
	Zebra::global["MagicBoxConfig"] = "Config/MagicBoxConfig.xml";
	Zebra::global["MagicBoxShiZhuangConfig"] = "Config/MagicBoxShiZhuangConfig.xml";
	Zebra::global["MagicBoxShiZhuangLevelConfig"] = "Config/MagicBoxShiZhuangLevelConfig.xml";
	Zebra::global["MagicBoxPiFengLevelConfig"] = "Config/MagicBoxPiFengLevelConfig.xml";
	Zebra::global["MagicBoxPiFengConfig"] = "Config/MagicBoxPiFengConfig.xml";
	Zebra::global["MagicBoxZuoqiConfig"] = "Config/MagicBoxZuoqiConfig.xml";
	Zebra::global["MagicBoxChiBangLevelConfig"] = "Config/MagicBoxChiBangLevelConfig.xml";
	Zebra::global["MagicBoxChiBangConfig"] = "Config/MagicBoxChiBangConfig.xml";
	Zebra::global["MagicBoxJieMianConfig"] = "Config/MagicBoxJieMianConfig.xml";  //Ä§ºÐ½çÃæ
	Zebra::global["ErnvConfig"] = "Config/ErnvConfig.xml";
	Zebra::global["zhengtuzhuaconfig"] = "Config/ZhengTuZhuanConfig.xml";
	Zebra::global["12BeastsConfig"] = "Config/12BeastsConfig.xml";
	Zebra::global["ShengQiConfig"] = "Config/ShengQiConfig.xml";
	Zebra::global["JingMaiConfig"] = "Config/JingMaiConfig.xml";
	Zebra::global["guoJiaBiaoCheRewardConfig"] = "Config/GuoJiaBiaoCheReward.xml";
	//soke monster exp  50
	Zebra::global["double_exp"] = "0";
	Zebra::global["mail_service"] = "on";
	Zebra::global["auction_service"] = "on";


	//½âÎöÅäÖÃÎÄ¼þ²ÎÊý
	SceneConfile sc;
	if (!sc.parse("ScenesServer"))
		return EXIT_FAILURE;

	//½âÎöÃüÁîÐÐ²ÎÊý
	zArg::getArg()->add(scenes_options, scenes_parse_opt, 0, scenes_doc);
	zArg::getArg()->parse(argc, argv);
	//Zebra::global.dump(std::cout);

	//ÉèÖÃÈÕÖ¾¼¶±ð
	Zebra::logger->setLevel(Zebra::global["log"]);
	//ÉèÖÃÐ´±¾µØÈÕÖ¾ÎÄ¼þ
	if ("" != Zebra::global["logfilename"])
		Zebra::logger->addLocalFileLog(Zebra::global["logfilename"]);

	if(atoi(Zebra::global["writebacktimer"].c_str()))
	{
		ScenesService::getInstance().writeBackTimer = atoi(Zebra::global["writebacktimer"].c_str());
	}
	else
	{
		ScenesService::getInstance().writeBackTimer = 600;
	}
	//soke ´ò¹Ö¾­Ñé±¶Êý
	if(atoi(Zebra::global["double_exp"].c_str()) >0 )
	{
		ScenesService::getInstance().double_exp = atoi(Zebra::global["double_exp"].c_str());
	}
	else
	{
		ScenesService::getInstance().double_exp = 0;
	}

	if(Zebra::global["double_drop"] == "true")
	{
		ScenesService::getInstance().double_drop = true;
	}
	else
	{
		ScenesService::getInstance().double_drop = false;
	}

	if(atoi(Zebra::global["level_double_drop"].c_str()))
	{
		ScenesService::getInstance().levelDoubleDrop = atoi(Zebra::global["level_double_drop"].c_str());
	}
	else
	{
		ScenesService::getInstance().levelDoubleDrop = 0;
	}

	//Ö¸Áî¼ì²â¿ª¹Ø
	if(Zebra::global["cmdswitch"] == "true")
	{
		zTCPTask::analysis._switch = true;
		zTCPClient::analysis._switch=true;
	}
	else
	{
		zTCPTask::analysis._switch = false;
		zTCPClient::analysis._switch=false;
	}

	//ÊÇ·ñÒÔºóÌ¨½ø³ÌµÄ·½Ê½ÔËÐÐ
	if ("true" == Zebra::global["daemon"]) {
		Zebra::logger->info("Program will be run as a daemon");
		Zebra::logger->removeConsoleLog();
		daemon(1, 1);
	}

	ScenesService::getInstance().main();
	ScenesService::delInstance();
	SceneUserManager::destroyMe();
	return EXIT_SUCCESS;
}
