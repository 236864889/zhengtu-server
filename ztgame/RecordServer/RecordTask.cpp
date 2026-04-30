/**
 * \file
 * \version  $Id: RecordTask.cpp w $
 * \author  
 * \date 
 * \brief 实现读档连接类
 *
 * 
 */


#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "RecordTask.h"
#include "Zebra.h"
#include "zDBConnPool.h"
#include "zString.h"
#include "RecordCommand.h"
#include "RecordServer.h"
#include "RecordSessionManager.h"
#include <string>
/**
 * \brief 验证登陆档案服务器的连接指令
 *
 * 如果验证不通过直接断开连接
 *
 * \param ptCmd 登陆指令
 * \return 验证是否成功
 */
bool RecordTask::verifyLogin(const Cmd::Record::t_LoginRecord *ptCmd)
{
	Zebra::logger->trace("RecordTask::verifyLogin(%s:%d)%u",getIP(),ptCmd->wdServerID,ptCmd->wdServerType);
	using namespace Cmd::Record;

	if (CMD_LOGIN == ptCmd->cmd	&& PARA_LOGIN == ptCmd->para)
	{
		const Cmd::Super::ServerEntry *entry = RecordService::getInstance().getServerEntry(ptCmd->wdServerID);
		char strIP[32];
		strcpy(strIP, getIP());		
		if (entry
				&& ptCmd->wdServerType == entry->wdServerType
				&& 0 == strcmp(strIP, entry->pstrIP))
		{
			wdServerID = ptCmd->wdServerID;
			wdServerType = ptCmd->wdServerType;
			return true;
		}
	}

	return false;
}

/**
 * \brief 等待接受验证指令并进行验证
 *
 * 实现虚函数<code>zTCPTask::verifyConn</code>
 *
 * \return 验证是否成功，或者超时
 */
int RecordTask::verifyConn()
{
	Zebra::logger->trace("RecordTask::verifyConn");
	int retcode = mSocket.recvToBuf_NoPoll();
	if (retcode > 0)
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = mSocket.recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
		if (nCmdLen <= 0)
			//这里只是从缓冲取数据包，所以不会出错，没有数据直接返回
			return 0;
		else
		{
			using namespace Cmd::Record;
			if (verifyLogin((t_LoginRecord *)pstrCmd))
			{
				Zebra::logger->debug("客户端连接通过验证");
				return 1;
			}
			else
			{
				Zebra::logger->error("客户端连接验证失败");
				return -1;
			}
		}
	}
	else
		return retcode;
}

/**
 * \brief 确认一个服务器连接的状态是可以回收的
 *
 * 当一个连接状态是可以回收的状态，那么意味着这个连接的整个生命周期结束，可以从内存中安全的删除了：）<br>
 * 实现了虚函数<code>zTCPTask::recycleConn</code>
 *
 * \return 是否可以回收
 */
int RecordTask::recycleConn()
{
	Zebra::logger->trace("RecordTask::recycleConn");
	RecordSessionManager::getInstance().removeAllByServerID(getID());
	//TODO 需要保证存档指令处理完成了
	return 1;
}
bool RecordTask::msgParse_Session(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("RecordTask::msgParse_Session");

	using namespace Cmd::Record;
	
	switch(ptNullCmd->para)
	{
		case PARA_CHK_USER_EXIST:
			{
				t_chkUserExist_SessionRecord *rev = (t_chkUserExist_SessionRecord *)ptNullCmd;
				connHandleID handle = RecordService::dbConnPool->getHandle();
				if ((connHandleID)-1 == handle)
				{
					Zebra::logger->error("不能获取数据库句柄");
					return false;
				}

				struct exist_struct
				{
					DWORD id;
					DWORD level;
					char name[MAX_NAMESIZE+1];
				} __attribute__ ((packed));

				static const dbCol exist_define[] = {
					{ "`CHARID`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
					{ "`LEVEL`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
					{ "`NAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) }, 
					{ NULL, 0, 0}
				};
				exist_struct * es;
				char where[128];
				bzero(where, sizeof(where));

				std::string escapeName;
				snprintf(where, sizeof(where) - 1, "NAME='%s'", 
						RecordService::dbConnPool->escapeString(handle,rev->name,escapeName).c_str());

				unsigned int retcode = RecordService::dbConnPool->exeSelect(handle, "`CHARBASE`", exist_define, where, "CHARID DESC", (BYTE **)&es);
				RecordService::dbConnPool->putHandle(handle);

				if (es)
				{
					for (unsigned int i=0; i< retcode; i++)
					{
						if (strcmp(es[i].name, rev->name) == 0)
						{
							rev->user_id = es[i].id;
							rev->user_level = es[i].level;
							break;
						}
					}
					SAFE_DELETE_VEC(es);
					sendCmd(rev, sizeof(t_userExist_SceneRecord));
				}
				return true;
			}
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool RecordTask::msgParse_Gateway(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("RecordTask::msgParse_Gateway");
	using namespace Cmd::Record;

	switch(ptNullCmd->para)
	{
		case PARA_GATE_DELCHAR:
			{
				t_DelChar_GateRecord *rev = (t_DelChar_GateRecord *)ptNullCmd;
				t_DelChar_Return_GateRecord cmd;
				char where[128];

				cmd.accid = rev->accid;
				cmd.id = rev->id;
				strcpy(cmd.name, rev->name);
				cmd.retcode = 0;

				connHandleID handle = RecordService::dbConnPool->getHandle();
				if ((connHandleID)-1 == handle)
				{
					Zebra::logger->error("不能获取数据库句柄");
					sendCmd(&cmd, sizeof(cmd));
					return false;
				}

				//首先删除原来已经作废的角色
				bzero(where, sizeof(where));
				snprintf(where, sizeof(where) - 1, "ACCID = %u AND CHARID = %u", rev->accid, rev->id);
				if ((unsigned int)-1 == RecordService::dbConnPool->exeDelete(handle, "`CHARBASE`", where))
				{
					RecordService::dbConnPool->putHandle(handle);
					sendCmd(&cmd, sizeof(cmd));
					Zebra::logger->trace("删除角色时失败:%u, %u", rev->accid,rev->id);
					return false;
				}
				RecordService::dbConnPool->putHandle(handle);
				Zebra::logger->trace("删除角色:%u, %u", rev->accid,rev->id);

				cmd.retcode = 1;
				sendCmd(&cmd, sizeof(cmd));
				//删除角色后重新得到角色列表
				return getSelectInfo(rev->accid); 

				return true;
			}
			break;
			//请求国家档案排序
		case REQUEST_GATE_COUNTRY_ORDER:
			{
				connHandleID handle = RecordService::dbConnPool->getHandle();
				if ((connHandleID)-1 == handle)
				{
					Zebra::logger->error("不能获取数据库句柄");
					return false;
				}

				static const dbCol countryid_define[] = {
					{ "`COUNTRY`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
					{ "`A`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
					{ NULL, 0, 0}
				};
				char Buf[200];
				bzero(Buf , sizeof(Buf));
				t_order_Country_GateRecord *ret_gate = 
					(t_order_Country_GateRecord*)Buf;
				constructInPlace(ret_gate);
				unsigned int retcode=0; 
				char where[128];
				bzero(where ,sizeof(where));
				strcpy(where ,"SELECT `COUNTRY` `A`, count(`COUNTRY`)  AS `A` FROM `CHARBASE` GROUP BY `COUNTRY` ORDER BY `A`");
				retcode = RecordService::dbConnPool->execSelectSql(handle, 
						where ,strlen(where), countryid_define,  (DWORD)10, (BYTE *)(ret_gate->order.order));
						
				RecordService::dbConnPool->putHandle(handle);
				if(retcode != (DWORD)-1)
				{
					ret_gate->order.size = retcode;
				}
				for(int i = 0 ; i < (int)ret_gate->order.size ; i ++)
				{
					Zebra::logger->debug("国家:%d,注册人数:%d",ret_gate->order.order[i].country , ret_gate->order.order[i].count);
				}
				sendCmd(ret_gate , sizeof(t_order_Country_GateRecord) 
						+ sizeof(ret_gate->order.order[0]) * ret_gate->order.size); 

				return true;
			}
			break;
		case PARA_GATE_CHECKNAME:
			{
				t_CheckName_GateRecord * rev = (t_CheckName_GateRecord *)ptNullCmd;
				t_CheckName_Return_GateRecord ret;
				ret.accid = rev->accid;
				strncpy(ret.name, rev->name, MAX_NAMESIZE-1);
				//首先验证名称是否重复

				connHandleID handle = RecordService::dbConnPool->getHandle();
				if ((connHandleID)-1 == handle)
				{
					Zebra::logger->error("不能获取数据库句柄");
					return false;
				}

				static const dbCol verifyname_define[] = {
					{ "`NAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) }, 
					{ NULL, 0, 0}
				};
				char strName[MAX_NAMESIZE+1];
				char where[128];
				bzero(where, sizeof(where));
				
				std::string upName;
				RecordService::dbConnPool->escapeString(handle,rev->name,upName);

				snprintf(where, sizeof(where) - 1, "NAME = '%s'", upName.c_str());
				unsigned int retcode = RecordService::dbConnPool->exeSelectLimit(handle, "`CHARBASE`", verifyname_define, where, "CHARID DESC", 1, (BYTE *)(strName));

				RecordService::dbConnPool->putHandle(handle);
				Zebra::logger->debug("角色名检查:%s have %d", upName.c_str(), retcode);

				ret.err_code = retcode;
				sendCmd(&ret, sizeof(ret));
				return true;
			}
			break;
		case PARA_GATE_CREATECHAR:
			{
				t_CreateChar_GateRecord *rev = (t_CreateChar_GateRecord *)ptNullCmd;
				t_CreateChar_Return_GateRecord ret;
				static const dbCol createchar_define[] = {
					{ "`ACCID`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
					{ "`NAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) }, 
					{ "`TYPE`", zDBConnPool::DB_WORD, sizeof(WORD) }, 
					{ "`LEVEL`", zDBConnPool::DB_WORD, sizeof(WORD) }, 
					{ "`HAIR`", zDBConnPool::DB_DWORD, sizeof(DWORD) },
					{ "`MAPID`", zDBConnPool::DB_DWORD, sizeof(DWORD) },
					{ "`MAPNAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) },
					{ "`COUNTRY`", zDBConnPool::DB_WORD, sizeof(WORD) },
					{ "`ACCPRIV`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
					{ "`CREATEIP`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
					{ NULL, 0, 0}
				};
				struct {
					DWORD accid;
					char name[MAX_NAMESIZE+1];
					WORD type;
					WORD level;
					DWORD hair;
					DWORD mapid;
					char  mapName[MAX_NAMESIZE+1];
					WORD country;
					DWORD accPriv;
					DWORD createip;
				} __attribute__ ((packed))
				createchar_data;
				connHandleID handle = RecordService::dbConnPool->getHandle();
				if ((connHandleID)-1 == handle)
				{
					Zebra::logger->error("不能获取数据库句柄");
					return false;
				}

				//检查帐号权限
				static const dbCol priv_define[] = {
					{ "`PRIV`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
					{ NULL, 0, 0}
				};
				/*
				//首先验证名称是否重复
				char strName[MAX_NAMESIZE+1];
				char where[128];
				static const dbCol verifyname_define[] = {
					{ "`NAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) }, 
					{ NULL, 0, 0}
				};

				bzero(where, sizeof(where));
				std::string escapeName;
				RecordService::dbConnPool->escapeString(handle,rev->name,escapeName);
				//snprintf(where, sizeof(where) - 1, "NAME = '%s' AND AVAILABLE = 1", escapeName.c_str());
				snprintf(where, sizeof(where) - 1, "NAME = '%s'", escapeName.c_str());
				unsigned int retcode = RecordService::dbConnPool->exeSelectLimit(handle, "`CHARBASE`", verifyname_define, where, "CHARID DESC", 1, (BYTE *)(strName));
				if (retcode == 1)
				{
					Zebra::logger->error("角色名称重复 %u, %s", rev->accid, rev->name);
					RecordService::dbConnPool->putHandle(handle);

					ret.accid = rev->accid;
					ret.retcode = 0;
					bzero(&ret.charinfo, sizeof(ret.charinfo));
					sendCmd(&ret, sizeof(ret));

					return false;
				}
				*/

				//插入数据库角色信息
				bzero(&createchar_data, sizeof(createchar_data));
				createchar_data.accid = rev->accid;
				strncpy(createchar_data.name, rev->name, MAX_NAMESIZE);
				createchar_data.type = rev->type;
				createchar_data.country = rev->country;
				createchar_data.level = 1;
				createchar_data.hair = rev->hair;
				createchar_data.mapid = 0;
				strncpy(createchar_data.mapName, rev->mapName, MAX_NAMESIZE);
				createchar_data.createip = rev->createip;
				//Zebra::logger->debug("创建角色IP %s(%u)", inet_ntoa(*(struct in_addr*)&createchar_data.createip),createchar_data.createip);
				char where[64];
				bzero(where, sizeof(where));
				snprintf(where, sizeof(where)-1, "ACCID=%u", rev->accid);
				unsigned int retcode = RecordService::dbConnPool->exeSelectLimit(handle, "`ACCPRIV`", priv_define, where, NULL, 1, (BYTE *)(&createchar_data.accPriv));
				retcode = RecordService::dbConnPool->exeInsert(handle, "`CHARBASE`", createchar_define, (const unsigned char *)(&createchar_data));
				RecordService::dbConnPool->putHandle(handle);
				if ((unsigned int)-1 == retcode)
				{
					Zebra::logger->error("创建角色插入数据库出错 %u, %s", rev->accid, rev->name);

					ret.accid = rev->accid;
					ret.retcode = 0;
					bzero(&ret.charinfo, sizeof(ret.charinfo));
					sendCmd(&ret, sizeof(ret));

					return false;
				}

				//返回新创建角色信息到网关
				ret.accid = rev->accid;
				ret.retcode = 1;
				bzero(&ret.charinfo, sizeof(ret.charinfo));
				ret.charinfo.id = retcode;
				strncpy(ret.charinfo.name, createchar_data.name, MAX_NAMESIZE);
				ret.charinfo.type = createchar_data.type;
				ret.charinfo.level = createchar_data.level;
				ret.charinfo.mapid = createchar_data.mapid;
				ret.charinfo.country = createchar_data.country;
				strncpy(ret.charinfo.mapName, createchar_data.mapName, MAX_NAMESIZE);

				return sendCmd(&ret, sizeof(ret));
			}
			break;
		case PARA_GATE_GET_SELECTINFO:
			{
				t_Get_SelectInfo_GateRecord *rev= (t_Get_SelectInfo_GateRecord *)ptNullCmd;
				return getSelectInfo(rev->accid); 
			}
			break;
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

//soke 取消突破属性限制
const dbCol RecordTask::charbase_define[] = {
	{ "`ACCID`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`CHARID`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`NAME`",					zDBConnPool::DB_STR,	sizeof(char[MAX_NAMESIZE+1]) }, 
	{ "`TYPE`",					zDBConnPool::DB_WORD,	sizeof(WORD) }, 
	{ "`LEVEL`",				zDBConnPool::DB_WORD,	sizeof(WORD) }, 
	{ "`FACE`",					zDBConnPool::DB_WORD,	sizeof(WORD) }, 
	{ "`HAIR`",					zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`BODYCOLOR`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`GOODNESS`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`MAPID`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`MAPNAME`",				zDBConnPool::DB_STR,	sizeof(char[MAX_NAMESIZE+1]) },
	{ "`X`",					zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`Y`",					zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`UNIONID`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`SCHOOLID`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`SEPTID`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`HP`",					zDBConnPool::DB_QWORD,	sizeof(QWORD) },
//	{ "`MAXHP`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
//	{ "`RESUMEHP`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`MP`",					zDBConnPool::DB_QWORD,	sizeof(QWORD) },
//	{ "`MAXMP`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
//	{ "`RESUMEMP`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`SP`",					zDBConnPool::DB_DWORD,	sizeof(DWORD) },
//	{ "`MAXSP`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
//	{ "`RESUMESP`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
//	{ "`PDAMAGE`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
//	{ "`MDAMAGE`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
//	{ "`PDEFENCE`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
//	{ "`MDEFENCE`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`EXP`",					zDBConnPool::DB_QWORD,	sizeof(QWORD) },
//	{ "`ATTACKSPEED`",			zDBConnPool::DB_WORD,	sizeof(WORD) }, 
//	{ "`MOVESPEED`",			zDBConnPool::DB_WORD,	sizeof(WORD) }, 
//	{ "`ATTACKRATING`",			zDBConnPool::DB_WORD,	sizeof(WORD) }, 
//	{ "`ATTACKDODGE`",			zDBConnPool::DB_WORD,	sizeof(WORD) }, 
//	{ "`BANG`",					zDBConnPool::DB_WORD,	sizeof(WORD) }, 
	{ "`LUCKY`",				zDBConnPool::DB_WORD,	sizeof(WORD) }, 
	{"`SKILLPOINTS`",			zDBConnPool::DB_WORD,	sizeof(WORD) },
	{ "`POINTS`",				zDBConnPool::DB_WORD,	sizeof(WORD) }, 
	{ "`COUNTRY`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`CONSORT`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`FORBIDTALK`",			zDBConnPool::DB_QWORD,	sizeof(QWORD) },
	{ "`BITMASK`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`ONLINETIME`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`CON`",					zDBConnPool::DB_DWORD,	sizeof(DWORD) }, //by=>friday
	{ "`STR`",					zDBConnPool::DB_DWORD,	sizeof(DWORD) }, //by=>friday
	{ "`DEX`",					zDBConnPool::DB_DWORD,	sizeof(DWORD) }, //by=>friday
	{ "`INT`",					zDBConnPool::DB_DWORD,	sizeof(DWORD) }, //by=>friday
	{ "`MEN`",					zDBConnPool::DB_DWORD,	sizeof(DWORD) }, //by=>friday
	{ "`RELIVEWEAKTIME`",		zDBConnPool::DB_WORD,	sizeof(WORD) }, 
	{ "`GRACE`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`EXPLOIT`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`TIRETIME`",				zDBConnPool::DB_BIN,	sizeof(char[36+1])}, 
	{ "`OFFLINETIME`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`FIVETYPE`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`FIVELEVEL`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
//	{ "`PASS`",				zDBConnPool::DB_STR,	sizeof(char[8]) }, 
	{ "`PKADDITION`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`MONEY`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`ANSWERCOUNT`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`HONOR`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`MAXHONOR`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`GOMAPTYPE`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`MSGTIME`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`ACCPRIV`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`GOLD`",					zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`TICKET`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`SALARY`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`CREATETIME`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`GOLDGIVE`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`PETPACK`",				zDBConnPool::DB_BYTE,	sizeof(BYTE) }, 
	{ "`PETPOINT`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`LEVELSEPT`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`PUNISHTIME`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`TRAINTIME`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`DOUBLEATIME`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`DOUBLEBTIME`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`DOUBLECTIME`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`FLOWER1TIME`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`FLOWER2TIME`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`FLOWER3TIME`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`FLOWER4TIME`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`FLOWER5TIME`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`FLOWER6TIME`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`LUCKYTIME`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`LUCKYCOUNT`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`LOGINCOUNT`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`SVIPCOUNT`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`TAIMCOUNT`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`KILLERONE`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`KILLERTWO`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`KILLERTHE`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`KILLERTIME`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`SAFETIME`",	            zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`CHANGEJOB`",            zDBConnPool::DB_DWORD,  sizeof(DWORD) },
	{ "`CHARVIP`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, //VIP点
 	{ "`CHONGZHI`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, //充值点
	{ "`FBCOUNT`",			    zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`FUBENTIME`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`FOLWERS`",              zDBConnPool::DB_DWORD,  sizeof(DWORD) },
	{ "`MOODS`",                zDBConnPool::DB_DWORD,  sizeof(DWORD) },
	{ "`FLLOVES`",              zDBConnPool::DB_DWORD,  sizeof(DWORD) },
	{ "`ROUND`",                zDBConnPool::DB_DWORD,  sizeof(DWORD) },
	{ "`ANGEL`",                zDBConnPool::DB_DWORD,  sizeof(DWORD) },
	{ "`HANBING`",              zDBConnPool::DB_DWORD,  sizeof(DWORD) },
	{ "`GOLDSUM`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },  //5倍保险
	{ "`GIVENUM`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },  //5倍保险
	{ "`ZHANLI`",				zDBConnPool::DB_QWORD,	sizeof(QWORD) },
	{ "`XHBBCOUNT`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) }, //鲜花榜特效
	{ "`HEROCOUNT`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) }, //护国榜特效
	{ "`ZHANLICOUNT`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) }, //护国榜特效

	{ "`JIAN1`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JIAN2`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JIAN3`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JIAN4`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JIAN5`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JIAN6`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	//接下来  我们要 在这个地方 加上 刚刚加的字段 记住 加字段 一定要在这个KEYEQUIP 的前面加 否则不会存数据 
	{ "`ZHUAN1`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHUAN2`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHUAN3`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHUAN4`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHUAN5`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHUAN6`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHUAN7`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHUAN8`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHUAN9`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHUAN10`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGQ1`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGX1`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGX2`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGX3`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGX4`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGX5`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGX6`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGX7`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGX8`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGX9`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGX10`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGX11`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHENGX12`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGM1`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGM2`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGM3`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`LIANZHANNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`LIANZHANMAXNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`LIANZHANTIME`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI0_OBJECTID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI0_OBJECTLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI0_OBJECTNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI1_OBJECTID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI1_OBJECTLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI1_OBJECTNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI2_OBJECTID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI2_OBJECTLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI2_OBJECTNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI3_OBJECTID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI3_OBJECTLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI3_OBJECTNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI4_OBJECTID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI4_OBJECTLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI4_OBJECTNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI5_OBJECTID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI5_OBJECTLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI5_OBJECTNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI6_OBJECTID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI6_OBJECTLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI6_OBJECTNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI7_OBJECTID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI7_OBJECTLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI7_OBJECTNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI8_OBJECTID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI8_OBJECTLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI8_OBJECTNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI9_OBJECTID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI9_OBJECTLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI9_OBJECTNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI10_OBJECTID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI10_OBJECTLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI10_OBJECTNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI11_OBJECTID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI11_OBJECTLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JINGCAI11_OBJECTNUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`TOUXIANLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`TOUXIANEXP`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`CHENGHAO_SELECT`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`FBA1`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`FBA2`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`FBA3`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`FBA4`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`FBA5`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`VIPFB`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BABYTYPE`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BABYLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BABYEXP`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BABYSKILL1`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BABYSKILL2`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BABYSKILL3`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BABYSKILL4`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BABYSKILL5`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BABYSKILLPOINT`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BIKECS`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BIKESTATE`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BIKENUM`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BIESHULEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BIESHUEXP`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`BIESHUSTATE`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`XIULIANEXP`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`XIULIANLEVEL1`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`XIULIANLEVEL2`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`XIULIANLEVEL3`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`XIULIANLEVEL4`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`XIULIANLEVEL5`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHANCHEID`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHANCHETYPE`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHANCHETIME1`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHANCHETIME2`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZHANCHETIME3`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZUOQITUJIANLEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZUOQITUJIANEXP`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`MOHELEVEL`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`MOHEEXP`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`SHIZHUANG_SELECT`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`PIFENG_SELECT`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`CHIBANG_SELECT`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`ZUOQI_SELECT`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`JIEMIAN_SELECT`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },  //魔盒界面
	{ "`TIANJIANGTIME`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`MIGONGCS`",                zDBConnPool::DB_DWORD,sizeof(DWORD) },
	{ "`TIANXIACOUNT`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) }, //护国榜特效 这里是 sql查询语句 也要加 OK到这里 我们就成功加了一个角色字段 只需要在需要的时候调用就行了
	{ "`DUANWEIEXP`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`DUANWEILINGQU`",	        zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`JINGCAI`",			    zDBConnPool::DB_BIN,	sizeof(char[1564+1]) }, 
	{ "`CHENGHAO`",			    zDBConnPool::DB_BIN,	sizeof(char[1564+1]) }, 
	{ "`SHIZHUANG`",			    zDBConnPool::DB_BIN,	sizeof(char[5120+1]) }, 
	{ "`PIFENG`",			    zDBConnPool::DB_BIN,	sizeof(char[5120+1]) }, 
	{ "`CHIBANG`",			    zDBConnPool::DB_BIN,	sizeof(char[5120+1]) }, 
	{ "`ZUOQI`",			    zDBConnPool::DB_BIN,	sizeof(char[5120+1]) }, 
	{ "`JIEMIAN`",			    zDBConnPool::DB_BIN,	sizeof(char[5120+1]) },   //魔盒界面
	{ "`KEYEQUIP`",			    zDBConnPool::DB_BIN,	sizeof(char[5120+1]) }, 
	{ "`M13AXHP`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`P13DAMAGE`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`M13AXPDAMAGE`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`M13DAMAGE`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`M13AXMDAMAGE`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`P13DEFENCE`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`M13DEFENCE`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) },	
	{ "`ALLBINARY`",			zDBConnPool::DB_BIN2,	0}, 
	{ NULL, 0, 0}
};

bool RecordTask::readCharBase(const Cmd::Record::t_ReadUser_SceneRecord *rev)
{
	Zebra::logger->trace("RecordTask::readCharBase");
	char readBuf[zSocket::MAX_DATASIZE];
	using namespace Cmd::Record;
	t_UserInfo_SceneRecord *ret;
	ret = (t_UserInfo_SceneRecord *)readBuf;
	constructInPlace(ret);

	char where[128];

	ret->id=rev->id;
	ret->dwMapTempID=rev->dwMapTempID;

	connHandleID handle = RecordService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("不能获取数据库句柄");
		return false;
	}
	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "CHARID = %u AND AVAILABLE = 1", rev->id);
	unsigned int retcode = RecordService::dbConnPool->exeSelectLimit(
			handle, "`CHARBASE`", charbase_define, where, "CHARID DESC", 1, (BYTE *)(&ret->charbase));//TODO 等待修改?
	RecordService::dbConnPool->putHandle(handle);
	if (1 == retcode)
	{
		Zebra::logger->trace("找到合格的角色记录：%u, %s,wdCon=%u,wdStr=%u,wdDex=%u,====,%u,%u,%u,%u,%u)", ret->charbase.id, ret->charbase.name,ret->charbase.wdCon,ret->charbase.wdStr,ret->charbase.wdDex,ret->charbase.wdProperty[0],ret->charbase.wdProperty[1],ret->charbase.wdProperty[2],ret->charbase.wdProperty[3],ret->charbase.wdProperty[4]);
	}
	else
	{
		Zebra::logger->error("读取档案失败，没有找到记录");
		bzero(&ret->charbase , sizeof(ret->charbase));
		ret->dataSize = (DWORD)PARA_SCENE_USER_READ_ERROR;
		sendCmd(ret,sizeof(t_UserInfo_SceneRecord));
		return false;
	}
	Zebra::logger->debug("读取档案服务器数据,压缩数据大小(size = %ld)" , ret->dataSize);
	return sendCmd(ret,sizeof(t_UserInfo_SceneRecord) + ret->dataSize);
}

bool RecordTask::saveCharBase(const Cmd::Record::t_WriteUser_SceneRecord *rev)
{
	Zebra::logger->trace("RecordTask::saveCharBase");
	char where[128];

	connHandleID handle = RecordService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("不能获取数据库句柄");
		return false;
	}
	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "`CHARID` = %u AND `AVAILABLE` = 1", rev->id);
	unsigned int retcode = RecordService::dbConnPool->exeUpdate(handle, "`CHARBASE`", charbase_define, (BYTE *)(&rev->charbase), where);
	RecordService::dbConnPool->putHandle(handle);

	//Zebra::logger->debug(where);
	if (1 == retcode)
	{
		Zebra::logger->trace("保存档案成功：%u, %u", rev->id, retcode);
	}
	else
	{
		Zebra::logger->error("保存档案失败：%u, %u", rev->id, retcode);
	}

	return true;
}

bool RecordTask::msgParse_Scene(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("RecordTask::msgParse_Scene");
	using namespace Cmd::Record;

	switch(ptNullCmd->para)
	{
		case PARA_SCENE_USER_WRITE:
			{
				t_WriteUser_SceneRecord *rev = (t_WriteUser_SceneRecord *)ptNullCmd;
				if (((TIMETICK_WRITEBACK == rev->writeback_type || OPERATION_WRITEBACK == rev->writeback_type)
							&& RecordSessionManager::getInstance().verify(rev->accid, rev->id, getID()))
						|| (LOGOUT_WRITEBACK == rev->writeback_type
							&& RecordSessionManager::getInstance().remove(rev->accid, rev->id, getID()))
						|| (CHANGE_SCENE_WRITEBACK == rev->writeback_type
							&& RecordSessionManager::getInstance().remove(rev->accid, rev->id, getID())))
				{
					if (saveCharBase(rev))
					{
						if(CHANGE_SCENE_WRITEBACK == rev->writeback_type || LOGOUT_WRITEBACK == rev->writeback_type)
						{
							using namespace Cmd::Record;
							t_WriteUser_SceneRecord_Ok ok; 
							ok.type=rev->writeback_type;
							ok.id=rev->id;
							ok.accid=rev->accid;
							sendCmd(&ok,sizeof(ok));
						}
						return true;
					}
				}
				else
				{
					Zebra::logger->error("回写档案验证失败，不能回写档案：%lu, %lu", rev->accid, rev->id);
				}
			}
			break;
		case PARA_SCENE_USER_READ:
			{
				t_ReadUser_SceneRecord *rev = (t_ReadUser_SceneRecord *)ptNullCmd;
				if (RecordSessionManager::getInstance().add(rev->accid, rev->id, getID()))
				{
					if (readCharBase(rev)) 
					{
						return true;
					}
					else
					{
						RecordSessionManager::getInstance().remove(rev->accid, rev->id, getID());
						return true;
					}
				}
				else
				{
					using namespace Cmd::Record;
					t_UserInfo_SceneRecord ret;
					ret.id=rev->id;
					ret.dwMapTempID=rev->dwMapTempID;
					ret.dataSize = (DWORD)PARA_SCENE_USER_READ_ERROR;
					sendCmd(&ret,sizeof(t_UserInfo_SceneRecord));
					Zebra::logger->error("添加读取记录失败，不能读取档案信息：%lu, %lu", rev->accid, rev->id);
					return true;
				}
			}
			break;
		case PARA_SCENE_USER_REMOVE:
			{
				t_RemoveUser_SceneRecord *rev = (t_RemoveUser_SceneRecord *)ptNullCmd;
				RecordSessionManager::getInstance().remove(rev->accid, rev->id, getID());
				Zebra::logger->iffy("用户在读取档案过程中退出(accid=%u,id=%u",rev->accid , rev->id);
				return true;
			}
			break;
		case PARA_SCENE_USER_EXIST:
			{
				t_userExist_SceneRecord *rev = (t_userExist_SceneRecord *)ptNullCmd;

				connHandleID handle = RecordService::dbConnPool->getHandle();
				if ((connHandleID)-1 == handle)
				{
					Zebra::logger->error("不能获取数据库句柄");
					return false;
				}

				struct exist_struct
				{
					DWORD id;
					char name[MAX_NAMESIZE+1];
				} __attribute__ ((packed));

				static const dbCol exist_define[] = {
					{ "`CHARID`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
					{ "`NAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) }, 
					{ NULL, 0, 0}
				};
				exist_struct * es;
				char where[128];
				bzero(where, sizeof(where));

				std::string escapeName;
				snprintf(where, sizeof(where) - 1, "NAME='%s'", RecordService::dbConnPool->escapeString(handle,rev->sm.toName,escapeName).c_str());
				unsigned int retcode = RecordService::dbConnPool->exeSelect(handle, "`CHARBASE`", exist_define, where, "CHARID DESC", (BYTE **)&es);
				RecordService::dbConnPool->putHandle(handle);

				if (es)
				{
					for (unsigned int i=0; i< retcode; i++)
					{
						if (strcmp(es[i].name, rev->sm.toName))
							continue;

						rev->toID = es[i].id;
					}
					SAFE_DELETE_VEC(es);
				}
				sendCmd(rev, sizeof(t_userExist_SceneRecord));
				return true;
			}
			break;
#ifdef _TEST_DATA_LOG
		case PARA_SCENE_INSERT_CHARTEST:
			{
				insertCharTest((t_Insert_CharTest_SceneRecord *)ptNullCmd);
				return true;
			}
			break;
		case PARA_SCENE_UPDATE_CHARTEST:
			{
				updateCharTest((t_Update_CharTest_SceneRecord *)ptNullCmd);
				return true;
			}
			break;
		case PARA_SCENE_DELETE_CHARTEST:
			{
				deleteCharTest((t_Delete_CharTest_SceneRecord *)ptNullCmd);
				return true;
			}
			break;
		case PARA_SCENE_READ_CHARTEST:
			{
				readCharTest((t_Read_CharTest_SceneRecord *)ptNullCmd);
				return true;
			}
			break;
#endif
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief 解析来自各个服务器连接的指令
 *
 * \param ptNullCmd 待处理的指令
 * \param nCmdLen 指令长度
 * \return 处理是否成功
 */
bool RecordTask::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Zebra::logger->trace("RecordTask::msgParse");
	using namespace Cmd::Record;

	switch(ptNullCmd->cmd)
	{
		case CMD_GATE:
			if (msgParse_Gateway(ptNullCmd, nCmdLen))
			{
				return true;
			}
			break;
		case CMD_SCENE:
			if (msgParse_Scene(ptNullCmd, nCmdLen))
			{
				return true;
			}
			break;
		case CMD_SESSION:
			if (msgParse_Session(ptNullCmd, nCmdLen))
			{
				return true;
			}
			break;
		default:
			break;
	}

	Zebra::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}
bool RecordTask::getSelectInfo(DWORD accid)
{
	Zebra::logger->trace("RecordTask::getSelectInfo");
	static const dbCol charinfo_define[] = {
		{ "`CHARID`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
		{ "`NAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) }, 
		{ "`TYPE`", zDBConnPool::DB_WORD, sizeof(WORD) }, 
		{ "`LEVEL`", zDBConnPool::DB_WORD, sizeof(WORD) }, 
		{ "`MAPID`", zDBConnPool::DB_DWORD, sizeof(DWORD) },
		{ "`MAPNAME`", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) },
		{ "`COUNTRY`", zDBConnPool::DB_WORD, sizeof(WORD) },
		{ "`FACE`", zDBConnPool::DB_WORD,   sizeof(WORD) },
		{ "", zDBConnPool::DB_STR, sizeof(char[MAX_NAMESIZE+1]) },
		{ "`BITMASK`", zDBConnPool::DB_DWORD, sizeof(DWORD) },
		//{ "`UNIONID`", zDBConnPool::DB_DWORD, sizeof(DWORD) }, 
		{ NULL, 0, 0}
	};
	Cmd::Record::t_Ret_SelectInfo_GateRecord ret;
	char where[128];

	connHandleID handle = RecordService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("不能获取数据库句柄");
		return false;
	}
	bzero(where, sizeof(where));
	snprintf(where, sizeof(where) - 1, "ACCID = %u AND AVAILABLE = 1", accid);

	unsigned int retcode = RecordService::dbConnPool->exeSelectLimit(handle, "`CHARBASE`", charinfo_define, where, "LEVEL DESC"/* , CHARID DESC" */, Cmd::MAX_CHARINFO, (BYTE *)(ret.info));

	RecordService::dbConnPool->putHandle(handle);
	if ((unsigned int)-1 == retcode
			|| 0 == retcode)
	{
		//Zebra::logger->debug("没有找到记录");
		ret.num = 0;
		bzero(ret.info, sizeof(ret.info));
	}
	else
	{
		//Zebra::logger->debug("找到 %u 条合格的角色记录", retcode);
		ret.num = retcode;
	}
	ret.accid=accid;
#if 0
	ret.num=1;
	std::string s;
	ret.info[0].id=rev->accid*2;
	char name[32];
	snprintf(name,32,"测试用户%d",ret.info[0].id);
	s=name;
	strcpy((char *)ret.info[0].name,s.c_str());
	ret.info[0].level=1;
	ret.info[0].mapid=1;
	s="测试地图";
	strcpy((char *)ret.info[0].mapName,s.c_str());
#endif

	return sendCmd(&ret,sizeof(ret));
}
#ifdef _TEST_DATA_LOG
const dbCol RecordTask::chartest_define[] = {
	{ "`NAME`",					zDBConnPool::DB_STR,	sizeof(char[MAX_NAMESIZE]) }, 
	{ "`LEVEL`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`UPDATETIME`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`UPDATEUSETIME`",		zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`DEATHTIMES`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`HPLEECHDOM`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`MPLEECHDOM`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`SPLEECHDOM`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`GETMONEY`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`GETHEIGH`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`GETSOCKET`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`GETMATERIAL`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`GETSTONE`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) }, 
	{ "`GETSCROLL`",			zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ "`MONEY`",				zDBConnPool::DB_DWORD,	sizeof(DWORD) },
	{ NULL, 0, 0}
};
/**
 * \brief 读取测试数据档案
 *
 *
 * \param rev: 收到的读取指令
 * \return 读取是否成功
 */
bool RecordTask::readCharTest(Cmd::Record::t_Read_CharTest_SceneRecord *rev)
{
	Zebra::logger->trace("RecordTask::readCharTest");
	
	using namespace Cmd::Record;
	t_Read_CharTest_SceneRecord ret;
	strncpy(ret.name , rev->name , MAX_NAMESIZE);
	ret.level = rev->level;

	char where[128];

	connHandleID handle = RecordService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("不能获取数据库句柄");
		return false;
	}
	bzero(where, sizeof(where));
	std::string escapeName;
	snprintf(where, sizeof(where) - 1, "`NAME` = '%s' AND `LEVEL` = %u", RecordService::dbConnPool->escapeString(handle,ret.name,escapeName).c_str(), ret.level);
	unsigned int retcode = RecordService::dbConnPool->exeSelectLimit(
			handle, "`CHARTEST`", chartest_define, where, "`NAME` DESC", 1, (BYTE *)(ret.name));//TODO 等待修改?
	RecordService::dbConnPool->putHandle(handle);
	if (1 == retcode)
	{
		Zebra::logger->debug("读取测试统计数据成功(%s)", ret.name);
	}
	else
	{
		Zebra::logger->debug("未找到统计数据(%s)", ret.name);
		return false;
	}
	return sendCmd(&ret,sizeof(ret));
}
/**
 * \brief 插入测试数据
 *
 *
 * \param rev: 收到的插入指令
 * \return 插入是否成功
 */
bool RecordTask::insertCharTest(Cmd::Record::t_Insert_CharTest_SceneRecord *rev)
{
	Zebra::logger->trace("RecordTask::insertCharTest");
	unsigned int retcode = 0;
	connHandleID handle = RecordService::dbConnPool->getHandle();
	retcode=RecordService::dbConnPool->exeInsert(handle,"`CHARTEST`",chartest_define,(const unsigned char *)(rev->name));
	RecordService::dbConnPool->putHandle(handle);
	if ((unsigned int)-1 == retcode)
	{
		Zebra::logger->error("角色插入测试数据库出错 %s", rev->name);
		return false;
	}
	return true;
}
/**
 * \brief 更新测试数据
 *
 *
 * \param rev: 收到的更新请求指令
 * \return 更新是否成功
 */
bool RecordTask::updateCharTest(Cmd::Record::t_Update_CharTest_SceneRecord *rev)
{
	Zebra::logger->trace("RecordTask::updateCharTest");
	char where[128];

	connHandleID handle = RecordService::dbConnPool->getHandle();
	if ((connHandleID)-1 == handle)
	{
		Zebra::logger->error("不能获取数据库句柄");
		return false;
	}
	bzero(where, sizeof(where));
	std::string escapeName;
	snprintf(where, sizeof(where) - 1, "`NAME` = '%s' AND `LEVEL` = %u", RecordService::dbConnPool->escapeString(handle,rev->name,escapeName).c_str(), rev->level);
	unsigned int retcode=RecordService::dbConnPool->exeUpdate(handle,"`CHARTEST`",chartest_define,(BYTE *)(rev->name),where);
	RecordService::dbConnPool->putHandle(handle);

	Zebra::logger->debug(where);
	if (1 == retcode)
	{
		Zebra::logger->debug("保存测试数据成功%s, %u", rev->name, retcode);
	}
	else
	{
		Zebra::logger->error("保存测试数据失败：%s, %u", rev->name, retcode);
	}
	return true;
}
/**
 * \brief 删除测试数据
 *
 *
 * \param rev: 收到的删除请求指令
 * \return 删除是否成功
 */
bool RecordTask::deleteCharTest(Cmd::Record::t_Delete_CharTest_SceneRecord *rev)
{
	Zebra::logger->trace("RecordTask::deleteCharTest");
	char where[128];
	bzero(where, sizeof(where));
	connHandleID handle = RecordService::dbConnPool->getHandle();
	std::string escapeName;
	snprintf(where, sizeof(where) - 1, "`NAME` = '%s' AND `LEVEL` = %u", RecordService::dbConnPool->escapeString(handle,rev->name,escapeName).c_str(), rev->level);
	RecordService::dbConnPool->exeDelete(handle, "`CHARBASE`", where);
	RecordService::dbConnPool->putHandle(handle);
	return true;
}
#endif
