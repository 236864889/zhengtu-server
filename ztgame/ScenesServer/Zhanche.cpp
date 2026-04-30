#include "SceneUser.h"
#include "SceneNpcManager.h"
#include "NpcTrade.h"
#include "RebuildObject.h"
#include "ScenesServer.h"
#include "CharBase.h"
#include "Command.h"
#include <string>
#include "fjconfig.h"
#include "SessionClient.h"

// 战车系统 by:醉梦
bool SceneUser::doZhancheCmd(const Cmd::stZhancheUserCmd *ptCmd, unsigned int cmdLen)
{
	using namespace Cmd;
	switch (ptCmd->byParam) //这里就是判断子命令了byParam
	{
		case ZHANCHE_LIST:
		{
			if (this->charbase.septid !=0)
			{
				Cmd::Session::t_ZhancheList_SceneSession send;
				send.dwUserID = this->id;
				send.septid = this->charbase.septid;
				sessionClient->sendCmd(&send, sizeof(send));
			}
			else{
				Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "你还没有家族不能领用战车！");
			}
		}
		break;
		case ZHANCHE_LINGYONG:
		{
			if (this->charbase.zhancheid !=0)
			{
				Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "你已经领用一台战车");
			}
			else{
				stZhancheLingyongCmd *cmd = (stZhancheLingyongCmd *)(ptCmd);
				Cmd::Session::t_ZhancheLingYong_SceneSession send;
				send.dwUserID = this->id;
				send.zhancheid = cmd->id;
				sessionClient->sendCmd(&send, sizeof(send));
			}
		}
		break;
		case ZHANCHE_ZHAOHUAN://召唤战车
		{
			if(this->issetUState(1410) )
			{
				Channel::sendSys(this, Cmd::INFO_TYPE_FAIL,"你已经召唤了战车");	
			}
			else{
				//by=>friday 简化战车载具模式实现
				if (this->charbase.zhancheid != 0)
				{
					// 进入战车载具模式
					// this->showCurrentEffect(1409, true); 
					// this->showCurrentEffect(Cmd::USTATE_USER_ZC, true);
					this->showCurrentEffect(1410, true);  //by=>friday 修复状态同步问题
					this->showCurrentEffect(Cmd::USTATE_USER_ZC, true);
					
					//by=>friday 设置战车载具状态标记
					this->zhanche_vehicle_mode = true;
					
					// 设置对应等级的战车外观ID(用于攻击判断)
					switch(this->charbase.zhancheid)
					{
						case 1: this->zhanche_npc_id = 50001; break; // 1级战车
						case 2: this->zhanche_npc_id = 50002; break; // 2级战车
						case 3: this->zhanche_npc_id = 50003; break; // 3级战车
						case 4: this->zhanche_npc_id = 50004; break; // 4级战车
						case 5: this->zhanche_npc_id = 50005; break; // 5级战车
						default: this->zhanche_npc_id = 50001; break;
					}
					
					Channel::sendSys(this, Cmd::INFO_TYPE_GAME, "战车召唤成功！你现在处于战车模式");
					Zebra::logger->debug("[战车系统] 玩家%s进入战车载具模式，战车ID:%u", this->name, this->zhanche_npc_id);
				}
				else
				{
					Channel::sendSys(this, Cmd::INFO_TYPE_FAIL,"你没有战车");
			}
			}
		}
		break;
		case ZHANCHE_GUIHUAN://归还战车
		{
			if (this->charbase.zhancheid == 0)
			{
				Channel::sendSys(this, Cmd::INFO_TYPE_FAIL, "你没有要归还的战车");
			}
			else{
				Cmd::Session::t_ZhancheGuiHuan_SceneSession send;
				send.dwUserID = this->id;
				send.zhancheid = this->charbase.zhancheid;
				sessionClient->sendCmd(&send, sizeof(send));
			}
		}
		break;
		case ZHANCHE_TUOLI://脱离战车
		{
			if(this->issetUState(1410) )
			{
				this->showCurrentEffect(1410, false ); 
				//by=>friday 清理战车载具模式状态
				this->zhanche_vehicle_mode = false;
				this->zhanche_npc_id = 0;
				Channel::sendSys(this, Cmd::INFO_TYPE_GAME,"脱离战车成功");	
				Zebra::logger->debug("[战车系统] 玩家%s脱离战车载具模式", this->name);
			}
			else{
				Channel::sendSys(this, Cmd::INFO_TYPE_FAIL,"你已经脱离了战车");	
			}
		}
		break;
		case ZHANCHE_SKILL://战车加成
		{
			stZhancheSkillCmd *cmd = (stZhancheSkillCmd *)(ptCmd);
			if(cmd->type==1)//生命
			{
				this->showCurrentEffect(USTATE_USER_ZC_TYPE1, true ); 
				this->charbase.zhanchetime1 = 0;      //soke 清空时间
				this->charbase.zhanchetime1 += 3000; //soke 战车1时间
				this->sendtoZhancheSelectedOfficialtate1();
				for(int i=0;i<100;i++)
				{
					SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(cmd->playerid[i]); // 玩家B
					if(pUser)
					{
						if(pUser->charbase.septid == this->charbase.septid )
						{
							pUser->showCurrentEffect(USTATE_USER_ZC_TYPE1, true ); 
							pUser->charbase.zhanchetime1 = 0;      //soke 清空时间
							pUser->charbase.zhanchetime1 += 3000; //soke 战车1时间
							pUser->sendtoZhancheSelectedOfficialtate1();
						}
						
					}

				}
			}
			else if(cmd->type==2)//攻击
			{
				this->showCurrentEffect(USTATE_USER_ZC_TYPE2, true ); 
				this->charbase.zhanchetime2 = 0;      //soke 清空时间
				this->charbase.zhanchetime2 += 3000; //soke 战车1时间
				this->sendtoZhancheSelectedOfficialtate2();
				for(int i=0;i<100;i++)
				{
					SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(cmd->playerid[i]); // 玩家B
					if(pUser)
					{
						if(pUser->charbase.septid == this->charbase.septid )
						{
							pUser->showCurrentEffect(USTATE_USER_ZC_TYPE2, true ); 
							pUser->charbase.zhanchetime2 = 0;      //soke 清空时间
							pUser->charbase.zhanchetime2 += 3000; //soke 战车1时间
							pUser->sendtoZhancheSelectedOfficialtate2();
						}
					}

				}
			}
			else if(cmd->type==3)//防御
			{
				this->showCurrentEffect(USTATE_USER_ZC_TYPE3, true ); 
				this->charbase.zhanchetime3 = 0;      //soke 清空时间
				this->charbase.zhanchetime3 += 3000; //soke 战车1时间
				this->sendtoZhancheSelectedOfficialtate3();
				for(int i=0;i<100;i++)
				{
					SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(cmd->playerid[i]); // 玩家B
					if(pUser)
					{
						if(pUser->charbase.septid == this->charbase.septid )
						{
							pUser->showCurrentEffect(USTATE_USER_ZC_TYPE3, true ); 
							pUser->charbase.zhanchetime3 = 0;      //soke 清空时间
							pUser->charbase.zhanchetime3 += 3000; //soke 战车1时间
							pUser->sendtoZhancheSelectedOfficialtate3();
						}
					}

				}
			}

		}
		break;


	}
	return true;
}



