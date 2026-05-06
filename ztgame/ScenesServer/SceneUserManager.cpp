
#include "SceneUserManager.h"
#include "Scene.h"
#include "QuestEvent.h"
#include "QuestTable.h"
#include "TimeTick.h"
#include <algorithm>
#include <list>
#include <sstream>
#include <vector>

#include "RecordCommand.h"
#include "RecordClient.h"
#include "SessionClient.h"
#include "SessionCommand.h"
#include "SceneManager.h"
#include "script.h"
#include "zXMLParser.h"

SceneUserManager *SceneUserManager::sum(NULL);
SceneRecycleUserManager *SceneRecycleUserManager::instance(NULL);

namespace
{
	struct GuoJiaBiaoCheRewardItem
	{
		DWORD id;
		DWORD num;
		DWORD bind;
	};

	struct GuoJiaBiaoCheRewardConfig
	{
		std::vector<GuoJiaBiaoCheRewardItem> items;
		char sysMsg[256];
		char worldMsg[256];
	};

	GuoJiaBiaoCheRewardConfig g_guoJiaBiaoCheRewards[5];

	void setRewardMsg(GuoJiaBiaoCheRewardConfig &reward, const char *sysMsg, const char *worldMsg)
	{
		bzero(reward.sysMsg, sizeof(reward.sysMsg));
		bzero(reward.worldMsg, sizeof(reward.worldMsg));
		snprintf(reward.sysMsg, sizeof(reward.sysMsg), "%s", sysMsg ? sysMsg : "");
		snprintf(reward.worldMsg, sizeof(reward.worldMsg), "%s", worldMsg ? worldMsg : "");
	}

	void addRewardItem(GuoJiaBiaoCheRewardConfig &reward, DWORD id, DWORD num, DWORD bind)
	{
		GuoJiaBiaoCheRewardItem item;
		item.id = id;
		item.num = num;
		item.bind = bind;
		reward.items.push_back(item);
	}

	void initDefaultGuoJiaBiaoCheRewards()
	{
		for (int i = 0; i < 5; ++i)
		{
			g_guoJiaBiaoCheRewards[i].items.clear();
			setRewardMsg(g_guoJiaBiaoCheRewards[i], "Escort reward received", "Escort reward: %s%s protected the escort");
		}

		addRewardItem(g_guoJiaBiaoCheRewards[1], 795, 5000, 1);
		addRewardItem(g_guoJiaBiaoCheRewards[1], 799, 500, 1);
		setRewardMsg(g_guoJiaBiaoCheRewards[1], "Escort reward received: 5000/500", "Escort reward: %s%s protected an intact escort");

		setRewardMsg(g_guoJiaBiaoCheRewards[2], "", "");

		addRewardItem(g_guoJiaBiaoCheRewards[3], 799, 5000, 1);
		addRewardItem(g_guoJiaBiaoCheRewards[3], 795, 500, 1);
		setRewardMsg(g_guoJiaBiaoCheRewards[3], "Escort reward received: 5000/500", "Escort reward: %s%s protected an intact escort");

		setRewardMsg(g_guoJiaBiaoCheRewards[4], "", "");
	}

	void grantGuoJiaBiaoCheReward(SceneUser *user, DWORD rewardID)
	{
		if (!user || rewardID >= 5)
			return;

		GuoJiaBiaoCheRewardConfig &reward = g_guoJiaBiaoCheRewards[rewardID];
		for (std::vector<GuoJiaBiaoCheRewardItem>::const_iterator it = reward.items.begin(); it != reward.items.end(); ++it)
		{
			char fetchCmd[128];
			snprintf(fetchCmd, sizeof(fetchCmd), "id=%u num=%u bind=%u", it->id, it->num, it->bind);
			Gm::fetch(user, fetchCmd);
		}

		if (reward.sysMsg[0])
			Channel::sendSys(user, Cmd::INFO_TYPE_GAME, "%s", reward.sysMsg);
		if (reward.worldMsg[0])
		{
			Channel::sendAllInfo(Cmd::INFO_TYPE_EXP5, reward.worldMsg,
					SceneManager::getInstance().getCountryNameByCountryID(user->charbase.country), user->charbase.name);
		}
	}
}

bool SceneUserManager::getUniqeID(DWORD& tempid)
{
	return true;
}

void SceneUserManager::putUniqeID(const DWORD& tempid)
{
}

SceneUserManager &SceneUserManager::getMe()
{
	if(sum==NULL)
	{
		sum=new SceneUserManager();
		//Zebra::logger->debug("д:(=%u , д=%u)" , sum->rwlock.rd_count , sum->rwlock.wr_count);
	}
	return *sum;
}

void SceneUserManager::destroyMe()
{
	if(sum!=NULL)
	{
		//Zebra::logger->debug("д:(=%u , д=%u)" , sum->rwlock.rd_count , sum->rwlock.wr_count);
		SAFE_DELETE(sum);
	}
}

SceneUserManager::SceneUserManager() 
{
}

SceneUserManager::~SceneUserManager()
{
	clear();
}

SceneUser * SceneUserManager::getUserByName( const char * name)
{
	SceneUser *ret;
	ret=(SceneUser *)zUserManager::getUserByName(name);
	if(ret && !ret->hasInScene())
		return NULL;
	else
		return ret;
}

SceneUser * SceneUserManager::getUserByID( DWORD id)
{
	SceneUser *ret;
	ret=(SceneUser *)zUserManager::getUserByID(id);
	if(ret && !ret->hasInScene())
		return NULL;
	else
		return ret;
}

SceneUser * SceneUserManager::getUserByTempID( DWORD tempid)
{
	SceneUser *ret;
	ret=(SceneUser *)zUserManager::getUserByTempID(tempid);
	if(ret && !ret->hasInScene())
		return NULL;
	else
		return ret;
}

SceneUser * SceneUserManager::getUserByNameOut( const char * name)
{
	SceneUser *ret;
	ret=(SceneUser *)zUserManager::getUserByName(name);
	if(ret && ret->hasInScene())
		return NULL;
	else
		return ret;
}

SceneUser * SceneUserManager::getUserByIDOut( DWORD id)
{
	SceneUser *ret;
	ret=(SceneUser *)zUserManager::getUserByID(id);
	if(ret && ret->hasInScene())
		return NULL;
	else
		return ret;
}

SceneUser * SceneUserManager::getUserByTempIDOut( DWORD tempid)
{
	SceneUser *ret;
		ret=(SceneUser *)zUserManager::getUserByTempID(tempid);
	if(ret && ret->hasInScene())
		return NULL;
	else
		return ret;
}

bool SceneUserManager::loadGuoJiaBiaoCheRewardConfig()
{
	initDefaultGuoJiaBiaoCheRewards();

	std::string configFile = Zebra::global["guoJiaBiaoCheRewardConfig"];
	if (configFile.empty())
		configFile = "Config/GuoJiaBiaoCheReward.xml";

	zXMLParser xml;
	if (!xml.initFile(configFile))
	{
		Zebra::logger->debug("load escort reward config failed, use defaults file=%s", configFile.c_str());
		return false;
	}

	xmlNodePtr root = xml.getRootNode("guojiaBiaoCheRewards");
	if (!root)
	{
		Zebra::logger->debug("escort reward config root missing, use defaults file=%s", configFile.c_str());
		return false;
	}

	xmlNodePtr rewardNode = xml.getChildNode(root, "reward");
	while (rewardNode)
	{
		DWORD rewardID = 0;
		if (xml.getNodePropNum(rewardNode, "id", &rewardID, sizeof(rewardID)) && rewardID > 0 && rewardID < 5)
		{
			GuoJiaBiaoCheRewardConfig parsed;
			parsed.items.clear();
			setRewardMsg(parsed, g_guoJiaBiaoCheRewards[rewardID].sysMsg, g_guoJiaBiaoCheRewards[rewardID].worldMsg);

			char msg[256];
			if (xml.getNodePropStr(rewardNode, "sysMsg", msg, sizeof(msg)))
				snprintf(parsed.sysMsg, sizeof(parsed.sysMsg), "%s", msg);
			if (xml.getNodePropStr(rewardNode, "worldMsg", msg, sizeof(msg)))
				snprintf(parsed.worldMsg, sizeof(parsed.worldMsg), "%s", msg);

			xmlNodePtr itemNode = xml.getChildNode(rewardNode, "item");
			while (itemNode)
			{
				DWORD itemID = 0;
				DWORD num = 0;
				DWORD bind = 1;
				xml.getNodePropNum(itemNode, "id", &itemID, sizeof(itemID));
				xml.getNodePropNum(itemNode, "num", &num, sizeof(num));
				xml.getNodePropNum(itemNode, "bind", &bind, sizeof(bind));
				if (itemID && num)
					addRewardItem(parsed, itemID, num, bind);
				itemNode = xml.getNextNode(itemNode, "item");
			}

			if (!parsed.items.empty())
				g_guoJiaBiaoCheRewards[rewardID] = parsed;
		}
		rewardNode = xml.getNextNode(rewardNode, "reward");
	}

	Zebra::logger->info("load escort reward config success file=%s", configFile.c_str());
	return true;
}

bool SceneUserManager::GuoJiaBiaoCheJiangLi1(Scene *scene, DWORD countryID)
{
	struct GuoJiaBiaoCheJiangLi : public execEntry<SceneUser>
	{
		Scene *scene;
		DWORD countryID;
		GuoJiaBiaoCheJiangLi(Scene *s, DWORD c) : scene(s), countryID(c)
		{
		}
		bool exec(SceneUser *u)
		{
			if (u->scene && u->scene->id == scene->id && u->charbase.country == countryID)
				grantGuoJiaBiaoCheReward(u, 1);
			return true;
		}
	};
	GuoJiaBiaoCheJiangLi exec(scene, countryID);
	SceneUserManager::getMe().execEveryUser(exec);
	return true;
}

bool SceneUserManager::GuoJiaBiaoCheJiangLi2(Scene *scene, DWORD countryID)
{
	(void)scene;
	(void)countryID;
	// Damaged escorts are allowed to finish and be recycled, but do not send rewards.
	return true;
}

bool SceneUserManager::GuoJiaBiaoCheJiangLi3(Scene *scene, DWORD countryID)
{
	struct GuoJiaBiaoCheJiangLi : public execEntry<SceneUser>
	{
		Scene *scene;
		DWORD countryID;
		GuoJiaBiaoCheJiangLi(Scene *s, DWORD c) : scene(s), countryID(c)
		{
		}
		bool exec(SceneUser *u)
		{
			if (u->scene && u->scene->id == scene->id && u->charbase.country == countryID)
				grantGuoJiaBiaoCheReward(u, 3);
			return true;
		}
	};
	GuoJiaBiaoCheJiangLi exec(scene, countryID);
	SceneUserManager::getMe().execEveryUser(exec);
	return true;
}

bool SceneUserManager::GuoJiaBiaoCheJiangLi4(Scene *scene, DWORD countryID)
{
	(void)scene;
	(void)countryID;
	// Damaged escorts are allowed to finish and be recycled, but do not send rewards.
	return true;
}

DWORD SceneUserManager::countUserInOneScene(Scene *scene)
{
	struct CountSceneExec :public execEntry<SceneUser>
	{
		Scene *scene;
		DWORD count;
		CountSceneExec(Scene *s):scene(s),count(0)
		{
		}
		bool exec(SceneUser *u)
		{
			if(u->scene && u->scene->id == scene->id)
			{
				count++;
			}
			return true;
		}
	};
	CountSceneExec exec(scene);
	SceneUserManager::getMe().execEveryUser(exec);
	return exec.count;
}
DWORD SceneUserManager::countUserByTask(SceneTask *task)
{
	struct CountUserBySceneTask :public execEntry<SceneUser>
	{
		SceneTask *task;
		DWORD count;
		CountUserBySceneTask(SceneTask *t):task(t),count(0)
		{
		}
		bool exec(SceneUser *us)
		{
			if(us && us->gatetask->getID()==task->getID())
			{
				count++;
			}
			return true;
		}
	};
	CountUserBySceneTask exec(task);	
	SceneUserManager::getMe().execEveryUser(exec);
	return exec.count;
}
void SceneUserManager::removeUserByTask(SceneTask *task)
{
	struct removeAllUserBySceneTask :public execEntry<SceneUser>
	{
		SceneTask *task;
		std::vector<DWORD> del_vec;
		removeAllUserBySceneTask(SceneTask *t):task(t)
		{
		}
		bool exec(SceneUser *us)
		{
			if(us->scene && us->gatetask==task)
			{
				del_vec.push_back(us->id);
			}
			return true;
		}
	};
	removeAllUserBySceneTask exec(task);	
	SceneUserManager::getMe().execEveryUser(exec);
	for(std::vector<DWORD>::iterator iter = exec.del_vec.begin() ; iter != exec.del_vec.end() ; iter ++)
	{
		SceneUser *pUser=SceneUserManager::getMe().getUserByID(*iter);
		if(pUser)
		{
			OnQuit event(1);
			EventTable::instance().execute(*pUser, event);
			execute_script_event(pUser,"quit");

			pUser->save(Cmd::Record::LOGOUT_WRITEBACK);
			//pUser->killAllPets();
			pUser->unreg();
			Zebra::logger->trace("û%s(%ld)жע",pUser->name,pUser->id);
		}
		else
		{
			SceneUser *pUser=SceneUserManager::getMe().getUserByIDOut(*iter);
			if(pUser)
			{
				Zebra::logger->trace("û%s(%ld)жע,ڶȡ",pUser->name,pUser->id);
				Cmd::Record::t_RemoveUser_SceneRecord rec_ret;
				rec_ret.accid = pUser->accid;
				rec_ret.id = pUser->id;
				recordClient->sendCmd(&rec_ret, sizeof(rec_ret));
				if(pUser)
				{
					pUser->destroy();
					SAFE_DELETE(pUser);
					//						static_cast<SceneUserVisitor *>(pUser)->destroy();
				}
			}
		}
	}
}

void SceneUserManager::removeAllUser()
{
	struct UnloadAllExec :public execEntry<SceneUser>
	{
		std::vector<DWORD> del_vec;
		UnloadAllExec()
		{
		}
		bool exec(SceneUser *u)
		{
			if(u->scene)
				del_vec.push_back(u->id);
			return true;
		}
	};
	UnloadAllExec exec;
	SceneUserManager::getMe().execEveryUser(exec);
	for(std::vector<DWORD>::iterator iter = exec.del_vec.begin() ; iter != exec.del_vec.end() ; iter ++)
	{
		SceneUser *pUser=SceneUserManager::getMe().getUserByID(*iter);
		if(pUser)
		{
			//pUser->save(Cmd::Record::LOGOUT_WRITEBACK);
			//SceneUserManager::getMe().removeUser(pUser);
			Zebra::logger->trace("û%s(%ld)رж",pUser->name,pUser->id);
			OnQuit event(1);
			EventTable::instance().execute(*pUser, event);
			execute_script_event(pUser,"quit");

			pUser->save(Cmd::Record::LOGOUT_WRITEBACK);
			//pUser->killAllPets();
			pUser->unreg();
			//֪ͨط
			Cmd::Scene::t_Unreg_LoginScene retgate;
			retgate.dwUserID = pUser->id;
			retgate.dwSceneTempID = pUser->scene->tempid;
			retgate.retcode = Cmd::Scene::UNREGUSER_RET_UNLOAD_SCENE;
			pUser->gatetask->sendCmd(&retgate,sizeof(retgate));

			//SAFE_DELETE(pUser);
		}
	}
}
void SceneUserManager::removeUserInOneScene(Scene *scene)
{
	struct UnloadSceneExec :public execEntry<SceneUser>
	{
		Scene *scene;
		std::vector<DWORD> del_vec;
		UnloadSceneExec(Scene *s):scene(s)
		{
		}
		bool exec(SceneUser *u)
		{
			if(u->scene && u->scene->id == scene->id)
			{
				del_vec.push_back(u->id);
			}
			return true;
		}
	};
	UnloadSceneExec exec(scene);
	SceneUserManager::getMe().execEveryUser(exec);
	for(std::vector<DWORD>::iterator iter = exec.del_vec.begin() ; iter != exec.del_vec.end() ; iter ++)
	{
		SceneUser *pUser=SceneUserManager::getMe().getUserByID(*iter);
		if(pUser)
		{
			//pUser->save(Cmd::Record::LOGOUT_WRITEBACK);
			//SceneUserManager::getMe().removeUser(pUser);
			Zebra::logger->trace("û%s(%ld)жسע",pUser->name,pUser->id);
			OnQuit event(1);
			EventTable::instance().execute(*pUser, event);
			execute_script_event(pUser,"quit");

			pUser->save(Cmd::Record::LOGOUT_WRITEBACK);
			//pUser->killAllPets();
			pUser->unreg();
			//֪ͨط
			Cmd::Scene::t_Unreg_LoginScene retgate;
			retgate.dwUserID = pUser->id;
			retgate.dwSceneTempID = pUser->scene->tempid;
			retgate.retcode = Cmd::Scene::UNREGUSER_RET_UNLOAD_SCENE;
			pUser->gatetask->sendCmd(&retgate,sizeof(retgate));

			//SAFE_DELETE(pUser);
		}
	}
}
void SceneUserManager::removeUser(SceneUser *user)
{
	zUserManager::removeUser(user);
}

bool SceneUserManager::addUser(SceneUser *user)
{
	return zUserManager::addUser(user);
}

void SceneUserManager::setAntiAtt(DWORD dwType, DWORD dwFromRelationID, DWORD dwToRelationID)
{
	struct EverySceneUserAction : public execEntry<SceneUser>
	{
		BYTE _byType;
		DWORD _dwFromRelationID;
		DWORD _dwToRelationID;

		EverySceneUserAction(BYTE byType, DWORD dwFromRelationID, DWORD dwToRelationID)
		{
			_byType = byType;
			_dwFromRelationID = dwFromRelationID;
			_dwToRelationID = dwToRelationID;
		}

		bool exec(SceneUser *su)
		{
			if (su->scene && su->charbase.country == _dwFromRelationID)
			{
				su->setAntiAttState(_byType, _dwToRelationID);
			}
			
			return true;
		}
	};      

	EverySceneUserAction esua(dwType, dwFromRelationID, dwToRelationID);
	SceneUserManager::getMe().execEveryUser(esua);
}

void SceneUserManager::enterWar(Cmd::Session::t_enterWar_SceneSession* cmd)
{
	struct EverySceneUserAction : public execEntry<SceneUser>
	{
		Cmd::Session::t_enterWar_SceneSession* ptCmd;
		EverySceneUserAction(Cmd::Session::t_enterWar_SceneSession* cmd)
		{
			ptCmd = cmd;
		}

		bool exec(SceneUser *su)
		{
			if (su->scene && su->charbase.country == ptCmd->dwFromRelationID)
			{
				if (ptCmd->dwStatus == 1)
				{
#ifdef _ALL_SUPER_GM					
					Zebra::logger->debug("ǰս¼: %u", su->warSize());
					Zebra::logger->debug("ս: toRelation:%u, isAtt:%u", 
							ptCmd->dwToRelationID, ptCmd->isAtt);
#endif
					su->addWarRecord(ptCmd->dwWarType, ptCmd->dwToRelationID, ptCmd->isAtt);

#ifdef _ALL_SUPER_GM					
					Zebra::logger->debug("ǰս¼: %u", su->warSize());
#endif					

					if (ptCmd->isAntiAtt)
					{
						su->setAntiAttState(ptCmd->dwWarType, ptCmd->dwToRelationID);
					}
						
					if (su->scene->getRealMapID() == 139 
							&& ptCmd->dwToRelationID==su->scene->getCountryID()
							&& su->scene->getCountryDareBackToMapID())
					{
						su->deathBackToMapID =  (ptCmd->dwToRelationID << 16) + 
							su->scene->getCountryDareBackToMapID();
					}
				}
				else
				{
#ifdef _ALL_SUPER_GM					
					Zebra::logger->debug("ǰս¼: %u", su->warSize());
					Zebra::logger->debug("ɾս: toRelation:%u, isAtt:%u", 
							ptCmd->dwToRelationID, ptCmd->isAtt);
#endif					

					su->removeWarRecord(ptCmd->dwWarType, ptCmd->dwToRelationID);
					
#ifdef _ALL_SUPER_GM					
					Zebra::logger->debug("ǰս¼: %u", su->warSize());
#endif					

					//if (!su->isSpecWar(Cmd::COUNTRY_FORMAL_DARE))
					//{// ڹս״̬
						su->setDeathBackToMapID(su->scene);
					//}
				}

				//su->sendNineToMe(); // ʱ¶ս״̬
				//FunctionTimes times(40,__FUNCTION__);
				su->setStateToNine(Cmd::USTATE_WAR);
			}

			return true;
		}
	};      

	EverySceneUserAction esua(cmd);
	SceneUserManager::getMe().execEveryUser(esua);
}
void SceneUserManager::countryTrans(DWORD dwCountryID, DWORD dwLevel)
{
	struct EverySceneUserAction : public execEntry<SceneUser>
	{
		DWORD _dwCountryID;
		DWORD _dwLevel;
		std::vector<SceneUser*> _vWaitTrans;

		EverySceneUserAction(DWORD dwCountryID, DWORD dwLevel) : _dwCountryID(dwCountryID),
									 _dwLevel(dwLevel)
		{
		}

		void trans()
		{
			for (std::vector<SceneUser*>::iterator vIter=_vWaitTrans.begin();
					vIter!=_vWaitTrans.end(); vIter++)
			{
				SceneUser* pUser = (SceneUser*)*vIter;
				Cmd::stAnswerCountryDareUserCmd send;
				pUser->sendCmdToMe(&send, sizeof(send));

				if (pUser->charbase.exploit > (1*exploit_arg) )
				{
					pUser->charbase.exploit = pUser->charbase.exploit - (1*exploit_arg);
				}
				else
				{
					pUser->charbase.exploit = 0;
				}

				Channel::sendSys(pUser, Cmd::INFO_TYPE_EXP, "ȼ˷̨Ӣµ㱻ѡս");
			}
		}

		bool exec(SceneUser *su)
		{
			if (su->scene)
			{
				if (su->charbase.country == _dwCountryID 
						&& su->charbase.level >= _dwLevel
						&& su->scene->getRealMapID() != 139
						&& su->scene->getRealMapID() != 137)
				{
					_vWaitTrans.push_back(su);
				}
				else
				{
					su->charbase.exploit += (1*exploit_arg);
				}
			}
			return true;
		}
	};      

	EverySceneUserAction esua(dwCountryID, dwLevel);
	SceneUserManager::getMe().execEveryUser(esua);
	esua.trans();
}

SceneRecycleUserManager &SceneRecycleUserManager::getInstance()
{
	if(instance==NULL)
	{
		instance=new SceneRecycleUserManager();
	}
	return *instance;
}
void SceneRecycleUserManager::destroyInstance()
{
	SAFE_DELETE(instance);
}
bool SceneRecycleUserManager::addUser(zSceneEntry *user)
{
	rwlock.wrlock();
	bool ret =addEntry((zEntry *)user);
	rwlock.unlock();
	return ret;
}
bool SceneRecycleUserManager::canReg(DWORD id)
{
	rwlock.rdlock();
	SceneUser *ret =(SceneUser *)getEntryByID(id);
	rwlock.unlock();
	if(!ret)
	{
		return true;
	}
	else
	{
		Zebra::logger->debug("ȴʱٴε½:%s",ret->name);
		rwlock.wrlock();
		SceneRecycleUserManager::getInstance().removeUser(ret);
		ret->gatetask=NULL;
		SAFE_DELETE(ret);
		rwlock.unlock();
		return true;
	}
}
SceneUser* SceneRecycleUserManager::getUserByID(DWORD id)
{
	SceneUser *user =(SceneUser *)SceneRecycleUserManager::getInstance().getEntryByID(id);
	return user;
}

void SceneRecycleUserManager::removeUser(SceneUser *user)
{
	SceneRecycleUserManager::getInstance().removeEntry((zEntry *)user);
}
struct Del 
{
	template<typename T>
		void operator()(T id)
		{
			SceneUser *user =(SceneUser *)SceneRecycleUserManager::getInstance().getUserByID(id);
			if(user)
			{
				SceneRecycleUserManager::getInstance().removeUser(user);
				user->gatetask=NULL;
				SAFE_DELETE(user);
			}
		}
};
void SceneRecycleUserManager::refresh()
{
	struct RecycleUserExec:public execEntry<SceneUser>
	{
		std::list<DWORD> wait_del;
		bool exec(SceneUser *su)
		{
			if(su->canRecycle(SceneTimeTick::currentTime))
			{
				wait_del.push_back(su->id);
			}
			return true;
		}
	};
	RecycleUserExec exec;
	rwlock.rdlock();
	execEveryUser(exec);
	rwlock.unlock();
	if(!exec.wait_del.empty())
	{
		rwlock.wrlock();
		std::for_each(exec.wait_del.begin(),exec.wait_del.end(),Del());
		rwlock.unlock();
	}
}
void SceneUserManager::removeUserToHuangcheng(Scene *scene)
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	if(!scene) return;
	struct GotoSceneExec :public execEntry<SceneUser>
	{
		Scene *scene;
		std::vector<DWORD> del_vec;
		GotoSceneExec(Scene *s):scene(s)
		{
		}
		bool exec(SceneUser *u)
		{
			if(u->scene && u->scene->getRealMapID() == scene->getRealMapID())
			{
				del_vec.push_back(u->id);
			}
			return true;
		}
	};
	GotoSceneExec exec(scene);
	SceneUserManager::getMe().execEveryUser(exec);
	for(std::vector<DWORD>::iterator iter = exec.del_vec.begin() ; iter != exec.del_vec.end() ; iter ++)
	{
		SceneUser *pUser=SceneUserManager::getMe().getUserByID(*iter);
		if(pUser)
		{
			Gm::gomap(pUser, "name=ʳ type=4");
		}
	}
}
///////////////////////////////////////////////////////////
void SceneUserManager::removeUserToWhere( Scene *fromScn, const char* toWhere )
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	if( !fromScn || !toWhere ) return;
	struct GotoSceneExec :public execEntry<SceneUser>
	{
		Scene *scene;
		std::vector<DWORD> del_vec;
		GotoSceneExec(Scene *s):scene(s)
		{
		}
		bool exec(SceneUser *u)
		{
			if(u->scene && u->scene->getRealMapID() == scene->getRealMapID())
			{
				del_vec.push_back(u->id);
			}
			return true;
		}
	};

    std::ostringstream os;
    os << "name=" << toWhere;
    GotoSceneExec exec(fromScn);
    SceneUserManager::getMe().execEveryUser(exec);
    for(std::vector<DWORD>::iterator iter = exec.del_vec.begin() ; iter != exec.del_vec.end() ; iter ++)
    {
        SceneUser *pUser=SceneUserManager::getMe().getUserByID(*iter);
        if(pUser)
        {
            Gm::gomap(pUser, os.str().c_str());
        }
    }
}
//////////////////////////////////////////////////////////////
void SceneUserManager::clearEmperorDare(Scene *scene)
{
	if(!scene) return;
	struct ClearEmperorDareSceneExec : public execEntry<SceneUser>
	{
		Scene *scene;
		ClearEmperorDareSceneExec(Scene *s):scene(s)
		{
		}
		bool exec(SceneUser *u)
		{
			if(u->scene && u->scene->getRealMapID() == scene->getRealMapID())
			{
				Cmd::stEnterEmperorDareZone send;
				send.state = 0;
				send.dwDefCountryID = 0;
				u->sendCmdToMe(&send, sizeof(send));
			}
			
			return true;
		}
	};
	ClearEmperorDareSceneExec exec(scene);
	SceneUserManager::getMe().execEveryUser(exec);
}

void SceneUserManager::setEmperorDare(Scene *scene)
{
	if(!scene) return;
	struct SetEmperorDareSceneExec : public execEntry<SceneUser>
	{
		Scene *scene;
		SetEmperorDareSceneExec(Scene *s):scene(s)
		{
		}
		bool exec(SceneUser *u)
		{
			if(u->scene && u->scene->getRealMapID() == scene->getRealMapID())
			{
				Cmd::stEnterEmperorDareZone send;
				send.state = 1;
				send.dwDefCountryID = scene->getEmperorDareDef();
				u->sendCmdToMe(&send, sizeof(send));
			}
			
			return true;
		}
	};

	SetEmperorDareSceneExec exec(scene);
	SceneUserManager::getMe().execEveryUser(exec);
}

