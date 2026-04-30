/**
 * \file
 * \version  $Id: BillClient.cpp  $
 * \author  
 * \date 
 * \brief 实现计费客户端
 */

#include <cstdio>
#include <cstdlib>

#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "BillCache.h"
#include "BillCallback.h"
#include "BillClient.h"
#include "BillClientManager.h"
#include "UserCommand.h"

/**
 * \brief 构造函数
 */
BillClient::BillClient(
		const std::string &ip,
		const unsigned short port,
		BillCallback &bc,
		const unsigned int my_id) : zTCPClientTask(ip, port), bc(bc), netType(NetType_near), my_id(my_id), gameZone()
{
	//Zebra::logger->debug("%s, my_id = %u", __PRETTY_FUNCTION__, my_id);
	bzero(gameZone_str, sizeof(gameZone_str));
}

/**
 * \brief 析构函数
 */
BillClient::~BillClient() {}

int BillClient::checkRebound()
{
	int retcode = pSocket->recvToBuf_NoPoll();
	if (retcode > 0)
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = pSocket->recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
		if (nCmdLen <= 0)
			//这里只是从缓冲取数据包，所以不会出错，没有数据直接返回
			return 0;
		else
		{
			using namespace Cmd::UserServer;
			t_logon_OK *ptCmd = (t_logon_OK *)pstrCmd;
			if (CMD_USER_LOGON == ptCmd->cmd)
			{
				Zebra::logger->debug("登陆平台计费服务器成功，收到区的编号：%u(%u, %u), %s, %u",
						ptCmd->gameZone.id,
						ptCmd->gameZone.game,
						ptCmd->gameZone.zone,
						ptCmd->name,
						ptCmd->netType); 
				netType = (ptCmd->netType == 0 ? NetType_near : NetType_far);
				gameZone = ptCmd->gameZone;
				bzero(gameZone_str, sizeof(gameZone_str));
				sprintf(gameZone_str, "%02u%03u", gameZone.game, gameZone.zone);
				Zebra::logger->debug("%s", gameZone_str);
				return 1;
			}
			else
			{
				Zebra::logger->debug("登陆平台计费服务器失败");
				return -1;
			}
		}
	}
	else
		return retcode;
}

void BillClient::addToContainer()
{
	BillClientManager::getInstance().add(this);
}

void BillClient::removeFromContainer()
{
	BillClientManager::getInstance().remove(this);
}

bool BillClient::connect()
{
	if (!zTCPClientTask::connect())
		return false;

	using namespace Cmd::UserServer;
	t_logon cmd;
	return sendCmd(&cmd, sizeof(cmd));
}

bool BillClient::action(BillData *bd)
{
	using namespace Cmd::UserServer;
	bool ret = false;
	switch(bd->at)
	{
		case AT_CONSUME://消费请求
			{
				BillData *new_bd = BillCache::getInstance().add(bd, gameZone_str);
				if (new_bd)
				{
					t_cmd_consume cmd;
					cmd.para = CMD_USER_CONSUME;
					cmd.uid = new_bd->uid;
					strncpy(cmd.tid, new_bd->tid, SEQ_MAX_LENGTH);
					cmd.tid[SEQ_MAX_LENGTH] = '\0';
					cmd.point = new_bd->point;
					cmd.source = BillClientManager::getInstance().source;
					strncpy(cmd.remark, new_bd->remark, REMARK_LENGTH);
					cmd.remark[REMARK_LENGTH] = '\0';
					strncpy(cmd.ip , new_bd->ip , MAX_IP_LENGTH);
					cmd.ip[MAX_IP_LENGTH] = '\0';
					ret = true;
					if (!sendCmd(&cmd, sizeof(cmd)))
					{
						BillCache::getInstance().remove(bd->tid);
						ret = false;
					}
				}
			}
			break;
			case AT_FILLIN://充值
			break;
			case AT_MCARD://sky 5倍保险
			{
				BillData *new_bd = BillCache::getInstance().add(bd, gameZone_str);
				if (new_bd)
				{
					t_cmd_consume cmd;
					//cmd.para = CMD_USER_MCARD;
					cmd.para = CMD_USER_CONSUME;
					cmd.uid = new_bd->uid;
					strncpy(cmd.tid, new_bd->tid, SEQ_MAX_LENGTH);
					cmd.tid[SEQ_MAX_LENGTH] = '\0';
					cmd.point = new_bd->point;
					cmd.source = BillClientManager::getInstance().source;
					strncpy(cmd.remark, new_bd->remark, REMARK_LENGTH);
					cmd.remark[REMARK_LENGTH] = '\0';
					strncpy(cmd.ip , new_bd->ip , MAX_IP_LENGTH);
					cmd.ip[MAX_IP_LENGTH] = '\0';
					ret = true;
					if (!sendCmd(&cmd, sizeof(cmd)))
					{
						BillCache::getInstance().remove(bd->tid);
						ret = false;
					}
				}
			}
			break;
			case AT_SCARD://专区卡
			{
			   BillData *new_bd = BillCache::getInstance().add(bd, gameZone_str);
			   if (new_bd)
			   {
						t_cmd_pcard cmd;
						cmd.para = CMD_USER_SCARD;
						cmd.uid = new_bd->uid;
						strncpy(cmd.pcardid, new_bd->cardid, CARDID_LENGTH);
						cmd.source = BillClientManager::getInstance().source;
						strncpy(cmd.tid, new_bd->tid, SEQ_MAX_LENGTH);
						strncpy(cmd.ip , new_bd->ip , MAX_IP_LENGTH);
						cmd.ip[MAX_IP_LENGTH] = '\0';
						ret = true;
						if (!sendCmd(&cmd, sizeof(cmd)))
						{
							BillCache::getInstance().remove(bd->tid);
							ret = false;
						}
					}
			}
			break;
			case AT_PCARD://道具卡
			{
			   BillData *new_bd = BillCache::getInstance().add(bd, gameZone_str);
			   if (new_bd)
			   {
						t_cmd_pcard cmd;
						cmd.para = CMD_USER_PCARD;
						cmd.uid = new_bd->uid;
						strncpy(cmd.pcardid, new_bd->cardid, CARDID_LENGTH);
						cmd.source = BillClientManager::getInstance().source;
						strncpy(cmd.tid, new_bd->tid, SEQ_MAX_LENGTH);
						strncpy(cmd.ip , new_bd->ip , MAX_IP_LENGTH);
						cmd.ip[MAX_IP_LENGTH] = '\0';
						ret = true;
						if (!sendCmd(&cmd, sizeof(cmd)))
						{
							BillCache::getInstance().remove(bd->tid);
							ret = false;
						}
					}
			}
			break;
			case AT_QBALANCE://查询
			{
				BillData *new_bd = BillCache::getInstance().add(bd, gameZone_str);
				if (new_bd)
				{
					t_cmd_qbalance cmd;
					cmd.para = CMD_USER_QBALANCE;
					cmd.uid = new_bd->uid;
					strncpy(cmd.tid, new_bd->tid, SEQ_MAX_LENGTH);
					cmd.tid[SEQ_MAX_LENGTH] = '\0';
					ret = true;
					if (!sendCmd(&cmd, sizeof(cmd)))
					{
						BillCache::getInstance().remove(bd->tid);
						ret = false;
					}
				}
			}
			break;
			default:
				Zebra::logger->debug("无效参数");
	}
	return ret;
}

bool BillClient::msgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	return MessageQueue::msgParse(ptNullCmd , nCmdLen);
}

bool BillClient::cmdMsgParse(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace Cmd::UserServer;
	bool retval = true;
	switch(ptNullCmd->cmd)
	{
		case CMD_USER_RET:
			{
				t_cmd_ret *pCmd = (t_cmd_ret *)ptNullCmd;
				BillData *bd = BillCache::getInstance().get(pCmd->tid);
				if (bd)
				{
					switch (bd->at)
					{
						case AT_CONSUME://消费
								bd->result = pCmd->ret;
								bd->balance = pCmd->balance;
								bd->bonus = pCmd->bonus;
								bd->hadfilled = pCmd->hadfilled ;
								retval = bc.action(bd);
								BillClientManager::tlogger->info("%s,%d,%u,%d,%d,,,%d,%s", 
								pCmd->tid, retval ? RET_OK : RET_FAIL, bd->uid, AT_CONSUME,
								BillClientManager::getInstance().source, bd->point, bd->remark);
						break;
						case AT_SCARD://专区卡
								bd->result = pCmd->ret;
								bd->subat = pCmd->subat;
								bd->balance = pCmd->balance;
								retval = bc.action(bd);
								BillClientManager::tlogger->info("%s,%d,%u,%d,%d,,,,%s", 
								pCmd->tid, retval ? RET_OK : RET_FAIL, bd->uid, AT_SCARD,
								BillClientManager::getInstance().source, bd->remark);
						break;
						case AT_MCARD:
							bd->result = pCmd->ret;
							bd->balance = pCmd->balance;
							bd->bonus = pCmd->bonus;
							bd->hadfilled = pCmd->hadfilled ;
							retval = bc.action(bd);
							BillClientManager::tlogger->info("%s,%d,%u,%d,%d,,,%d,%s", 
								pCmd->tid, retval ? RET_OK : RET_FAIL, bd->uid, AT_MCARD,
								BillClientManager::getInstance().source, bd->point, bd->remark);
							break;
						break;
						case AT_PCARD://道具卡
								bd->result = pCmd->ret;
								bd->subat = pCmd->subat;
								retval = bc.action(bd);
								BillClientManager::tlogger->info("%s,%d,%u,%d,%d,,,,%s", 
								pCmd->tid, retval ? RET_OK : RET_FAIL, bd->uid, AT_PCARD, 
								BillClientManager::getInstance().source, bd->remark);
						break;
						case AT_FILLIN://充值
						break;
						case AT_QBALANCE://查询
						{
								bd->result = pCmd->ret;
								bd->balance = pCmd->balance;
								bd->bonus = pCmd->bonus;
								bd->hadfilled = pCmd->hadfilled ;
								retval = bc.action(bd);
						}
						break;
						default:	break;
					}
					SAFE_DELETE(bd);
				}
				else
				{
					//TODO FIXME
					Zebra::logger->debug("tradeSN not found, %s", pCmd->tid);
				}
				return retval;
			}
			break;
	}

	Zebra::logger->debug("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

