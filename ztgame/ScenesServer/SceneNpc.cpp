/**
 * \file
 * \version  $Id: SceneNpc.cpp $
 * \author  
 * \date 
 * \brief Npc
 *
 * 
 */
#include <list>
#include <vector>
#include <algorithm>

#include "zSceneEntry.h"
#include "SceneNpc.h"
#include "Zebra.h"
#include "Chat.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TeamManager.h"
#include "TimeTick.h"
#include "SkillStatusManager.h"
#include "QuestEvent.h"
#include "QuestTable.h"
#include "Rush.h"
#include "CountryDare.h"
#include "SessionClient.h"
#include "TimeTick.h"
#include "ScenesServer.h"
#include "Command.h"
#include "script.h"
#include "CartoonPet.h"

DWORD SceneNpc::serialID = SceneNpc::maxUniqueID;
zUniqueDWORDID SceneNpc::uniqueID(1, SceneNpc::maxUniqueID);

/**
 * \brief жϸnpcǷԱ
 * falseȫܱ
 *
 */
bool SceneNpc::canBeAttack()
{
	if (getPetType()==Cmd::PET_TYPE_RIDE//ƥҲܱ
			|| getPetType()==Cmd::PET_TYPE_CARTOON)
		return false;

	switch(npc->kind)
	{
		case NPC_TYPE_STORAGE:
		case NPC_TYPE_ROADSIGN:
		case NPC_TYPE_TREASURE:
		case NPC_TYPE_MOBILETRADE:
		case NPC_TYPE_LIVENPC:
		case NPC_TYPE_MAILBOX:
		case NPC_TYPE_AUCTION:
		case NPC_TYPE_SURFACE:
		case NPC_TYPE_CARTOONPET:
			{
				return false;
			}
			break;

		default:
			break;
	}
	return true;
}

					

/**
 * \brief жϸnpcǷԱ(Ƿǹ)
 * NPC_TYPE_TRADE NPC_TYPE_TASK ͿԱ˹
 */
bool SceneNpc::isBugbear()
{
	if (getPetType()==Cmd::PET_TYPE_RIDE//ƥҲܱ
			|| getPetType()==Cmd::PET_TYPE_CARTOON)
		return false;

	switch(npc->kind)
	{
		case NPC_TYPE_TRADE:
		case NPC_TYPE_TASK:
		case NPC_TYPE_STORAGE:
		case NPC_TYPE_ROADSIGN:
		case NPC_TYPE_TREASURE:
		case NPC_TYPE_MOBILETRADE:
		case NPC_TYPE_LIVENPC:
		case NPC_TYPE_MAILBOX:
		case NPC_TYPE_AUCTION:
		case NPC_TYPE_SURFACE:
		case NPC_TYPE_CARTOONPET:
			{
				return false;
			}
			break;

		default:
			break;
	}
	return true;
}
/**
 * \brief Npc
 *
 */
void SceneNpc::death(const zRTime &ct)
{
	this->skillStatusM.processDeath();

	killAllPets();
	petDeath();
	//赲Ϣ
	scene->clearBlock(getPos());

	setState(SceneEntry_Death);
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("npc %s(%u) ", name, tempid);
#endif
	summonByNpcMap(define->deathSummonList);

	//Ʒӳ
	setMoveTime(ct, 1000);
	setAttackTime(ct, 1000);

	if (Cmd::PET_TYPE_NOTPET==getPetType())//ﲻ
	{
		lostObject = true;
		mayRush = true;
	}

	if (ALLY_GUARDNPC==npc->id)
		//Zebra::logger->trace("%s ˹ڳɱ",SceneManager::getInstance().getCountryNameByCountryID(scene->getCountryID()));
		Zebra::logger->debug("%s ˹ڳɱ",SceneManager::getInstance().getCountryNameByCountryID(scene->getCountryID()));
}

/**
 * \brief 
 * Գ֤߶Գ
 * \param direct ˷
 * \param step ˲
 */
void SceneNpc::backoff(const int direct, const int step)
{
	const int walk_adjust[9][2]= { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, 0} };
	int i = 0;

	zPosI oldPosI = getPosI();
	zPos oldPos = getPos(),  newPos = getPos();
	for(i = 1; i <= step; i++)
	{
		newPos.x += walk_adjust[direct][0];
		newPos.y += walk_adjust[direct][1];
		if(scene->checkBlock(newPos))
			break;
	}
	if (i > 1)
	{
		newPos.x -= walk_adjust[direct][0];
		newPos.y -= walk_adjust[direct][1];

		if (scene->refresh(this, newPos))
		{
			scene->setBlock(newPos);
			scene->clearBlock(oldPos);
			setDir(scene->getReverseDirect(direct));

			Cmd::stBackOffMagicUserCmd cmd;
			cmd.dwTempID = tempid;
			cmd.byType = Cmd::MAPDATATYPE_NPC;
			cmd.byDirect = direct;
			cmd.x = newPos.x;
			cmd.y = newPos.y;
			scene->sendCmdToNine(oldPosI, &cmd, sizeof(cmd));

			if (oldPosI != getPosI())
			{
				Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
				removeNpc.dwMapNpcDataPosition = tempid;
				scene->sendCmdToReverseDirect(oldPosI,
						scene->getScreenDirect(oldPosI,getPosI()), &removeNpc, sizeof(removeNpc));
				Cmd::stAddMapNpcMapScreenUserCmd addNpc;
				full_t_MapNpcData(addNpc.data);
				scene->sendCmdToDirect(getPosI(), scene->getScreenDirect(oldPosI,getPosI()),
						&addNpc, sizeof(addNpc));
				Cmd::stRTMagicPosUserCmd ret;
				full_stRTMagicPosUserCmd(ret);
				scene->sendCmdToDirect(getPosI(), scene->getScreenDirect(oldPosI,getPosI()), &ret, sizeof(ret));
				attackRTHpAndMp();
			}
		}
	}
	backOffing = 1000;
}

/**
 * \brief 龭бҵĹǷʱ
 * \param pAtt 
 */
void SceneNpc::refreshExpmapAttackTime(SceneUser* pAtt)
{
	NpcHpHashmap_iterator iter;
	DWORD leader = pAtt->team.getLeader();

	if (!leader)
	{
		leader = pAtt->tempid;
	}

	iter = expmap.find(leader);

	if (iter != expmap.end())
	{
		iter->second.attack_time.now();
	}
}

/**
 * \brief  userhp
 *
 * \param pAtt 
 * \param wdHP ٵhp
 * \return 
 */
void SceneNpc::reduceHP(SceneUser *pAtt , uint64_t wdHP) //by=>friday ޸Ϊ֧64λ޷˺
{
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::reduceHP %s hp=%u wdHP=%llu", name, hp, wdHP); //by=>friday ޸64λʾʽ
#endif
	if(hp + wdHP == this->getMaxHP())
	{
		first_time.now();
		//Zebra::logger->debug("npc(%ld , %s)һ⵽ , ʱ:%ld", this->tempid , this->name , ctv.sec());
	}
	NpcHpHashmap_iterator iter;
	DWORD leader = 0; 
	if(pAtt->team.IsTeamed() && pAtt->team.isNormalExp())
	{
		leader = pAtt->team.getLeader();
	}


	if (!leader)
	{
		leader = pAtt->tempid;
	}

	if (this->id == COUNTRY_MAIN_FLAG || this->id == COUNTRY_SEC_FLAG)
	{
		CountryDareM::getMe().updateAttackTime(this->scene->getCountryID(), SceneTimeTick::currentTime.sec());
	}

	if (this->id == COUNTRY_KING_MAIN_FLAG || this->id == COUNTRY_KING_SEC_FLAG)
	{
		CountryDareM::getMe().updateKingAttackTime(this->scene->getCountryID(), SceneTimeTick::currentTime.sec());
	}

	if ((this->isMainGeneral() || this->id == COUNTRY_SEC_GEN))
	{
		if (pAtt->isAtt(Cmd::COUNTRY_FORMAL_DARE))
		{
			CountryDareM::getMe().updateAttGenAttackTime(this->scene->getCountryID(), 
					SceneTimeTick::currentTime.sec());
		}
		else
		{
			CountryDareM::getMe().updateGenAttackTime(this->scene->getCountryID(), 
					SceneTimeTick::currentTime.sec());
		}
	}


	iter = expmap.find(leader);

	if (iter == expmap.end())
	{
		if(this->getState() != zSceneEntry::SceneEntry_Death)
		{
			t_expRec expRec;
			showHP(pAtt, hp+wdHP);
			expRec.wdHP = wdHP;
			expmap.insert(NpcHpHashmap_pair(leader, expRec));
#ifdef _XWL_DEBUG
			Zebra::logger->debug("%s wdHP=%llu hp=%u", name, expRec.wdHP, wdHP); //by=>friday ޸64λʾʽ
#endif			
		}
	}
	else
	{
		//		if (iter->second.attack_time(SceneTimeTick::currentTime))
		if (abs(SceneTimeTick::currentTime.sec() - iter->second.attack_time.sec()) <= 10)
		{
			iter->second.wdHP += wdHP;
#ifdef _XWL_DEBUG
			Zebra::logger->debug("Сʮ룬ۼHPֵ:%llu hp=%u", iter->second.wdHP, hp); //by=>friday ޸64λʾʽ
#endif			

		}
		else
		{
			iter->second.wdHP = wdHP;
#ifdef _XWL_DEBUG
			Zebra::logger->debug("ʮ룬 HPֵ:%llu hp=%u", iter->second.wdHP, hp); //by=>friday ޸64λʾʽ
#endif			
		}
		iter->second.attack_time.now();
	}
}

/**
 * \brief ǮĻص
 *
 */
struct MoneyTeamExecExceptMe : public TeamMemExec
{
	///ӳ
	SceneUser *leader;
	///Լ
	SceneUser *me;
	///Ǯ
	WORD money;

	/**
	 * \brief 캯
	 *
	 * \param pMe Լ
	 * \param u ӳ
	 * \param mon Ǯ
	 */
	MoneyTeamExecExceptMe(SceneUser *pMe, SceneUser *u , WORD mon)
	{
		me = pMe;
		leader = u;
		money = mon;
	}
	/**
	 * \brief Ǯ
	 *
	 * \param member 
	 * \return Ƿص
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
		if(pUser && pUser != me)
		{
			if(pUser->scene->checkTwoPosIInNine(pUser->getPosI(),me->getPosI()))
			{
				if(pUser->getState() != SceneUser::SceneEntry_Death)
				{
					pUser->packs.addMoney((DWORD)(money),"Ǯ");
					pUser->team.putMoneyPlus(money);
				}
			}
		}
		return true;
	}

};
//ӷ
/**
 * \brief Ǯ
 *
 * \param money Ǯ
 */
void SceneNpc::distributeMoney(DWORD money)
{
	SceneUser *pUser = SceneUserManager::getMe().getUserByID(dwNpcLockedUser);
	if(pUser)
	{
		if(pUser->team.getLeader())
		{
			SceneUser *leader = SceneUserManager::getMe().getUserByTempID(pUser->team.getLeader());
			if(leader)
			{
				WORD mon = 0;
				WORD myMon = (WORD)(money*0.2f + 0.9f);
				if(leader->team.getExpSize(pUser->getPosI(),this->scene->id))
				{
					mon = (WORD)((money - myMon) / leader->team.getExpSize(pUser->getPosI(),this->scene->id) + 0.9f);
				}
				myMon +=mon;
				pUser->packs.addMoney(myMon,"Ǯ");
				pUser->team.putMoneyPlus(myMon);

				MoneyTeamExecExceptMe exec(pUser, leader , mon);
				leader->team.execEveryOne(exec);
			}
		}
		else
		{
			pUser->packs.addMoney(money,"Ǯ");
		}
	}
}

struct CountSeptTeamExec : public TeamMemExec
{
	///
	int count;
	///־ɫļID
	DWORD septid;
	///ɱnpc
	SceneNpc *npc;

	CountSeptTeamExec(DWORD dwSeptid, SceneNpc *pnpc)
	{
		septid = dwSeptid;
		count=0;
		npc = pnpc;
	}

	/**
	 * \brief ͬһͳƺ
	 *
	 *
	 * \param member 
	 * \return Ƿص
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
		if(pUser)
		{
			if (pUser->scene->tempid == npc->scene->tempid &&
					pUser->scene->checkTwoPosIInNine(pUser->getPosI(),npc->getPosI()))
			{
				if (pUser->charbase.septid == septid)
				{
					count++;
					if (count >1) return false;
				}
			}
		}
		return true;
	}
};

/**
 * \brief ־Ļص
 *
 */
struct ExpTeamExec : public TeamMemExec
{
	///ӳ
	SceneUser *leader;
	///ɱnpc
	SceneNpc *npc;
	///ֵ
	DWORD exp;
	///ID

	/**
	 * \brief 캯
	 *
	 * \param u ӳ
	 * \param n ɱnpc
	 * \param wdExp 
	 */
	ExpTeamExec(SceneUser *u , SceneNpc *n , DWORD wdExp)
	{
		leader = u;
		npc = n;
		exp = wdExp;
	}
	/**
	 * \brief Ǯĺ
	 *
	 *
	 * \param member 
	 * \return Ƿص
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
		if(pUser)
		{
			// ģʽӳֵ
			if((pUser == leader && pUser->team_mode == Cmd::TEAM_HONOR) || (leader->team_mode == Cmd::TEAM_HONOR &&pUser->charbase.level >= TEAM_HONOR_MEMBER_LEVEL))
			{
				return true;
			}
			if(pUser->getState() != SceneUser::SceneEntry_Death)
			{
				//bool checkUserInNine(SceneUser *pMe, SceneUser *pOhter, const bool locker = true);
				if(leader->team.getAverageExp() > 0 
						&& pUser->scene->tempid == npc->scene->tempid
						&& pUser->scene->checkTwoPosIInNine(pUser->getPosI(),npc->getPosI()))
				{
					//Zebra::logger->debug("ƽ=%d,=%d",leader->team.getAverageExp() , leader->team.getExpSize(npc->getPosI()));
					//DWORD wdExp=(DWORD)(exp * (((pUser->charbase.level / 30)+1.0f) / leader->team.getAverageExp()) + 0.9); 
					DWORD exp_level=0;
					if(leader->team_mode == Cmd::TEAM_HONOR)
					{
						if(leader->scene->id == pUser->scene->id 
								&& pUser->scene->checkTwoPosIInNine(pUser->getPosI(),leader->getPosI())
								&& (int)(leader->team.getSize() - 1 > 0))
						{
							exp_level = (DWORD)(leader->charbase.level / (leader->team.getSize() - 1));
						}
					}
					exp_level += pUser->charbase.level; 
					DWORD early_level=pUser->charbase.level;
					DWORD wdExp = (DWORD)(exp *((float)(exp_level) / (float)leader->team.getAverageExp()) + 0.9); 
					wdExp += pUser->team.getExpPlus(wdExp);
					if (pUser->pet) pUser->pet->addExp(wdExp);//ﾭ鲻
					for (SceneUser::adopt_it it=pUser->adoptList.begin(); it!=pUser->adoptList.end(); it++)
						it->second->releaseExp(wdExp*15/100);
					wdExp = npc->levelExp(wdExp , pUser->charbase.level);
					wdExp = npc->addOtherExp(wdExp , pUser);
					if (wdExp >0)
					{
					//	if (pUser->scene&&(pUser->charbase.country == pUser->scene->getCountryID())) wdExp = (DWORD)(wdExp*(pUser->wdTirePer/100.0f));
						if (pUser->isSpecWar(Cmd::COUNTRY_FORMAL_DARE))
						{//սڣ־50%
							wdExp = wdExp/2;
						}
						//TODO 
						if (wdExp==0) wdExp=1;

						pUser->sendExpToSept(wdExp);
						if (pUser->charbase.septid>0)
						{
							CountSeptTeamExec execCount(pUser->charbase.septid, npc);
							leader->team.execEveryOne(execCount);
							if (execCount.count>1)	wdExp= (DWORD)(wdExp*1.2f);
						}
						DWORD oldExp = wdExp;
						wdExp += pUser->scene->sceneExp(wdExp);
						if(pUser->charbase.country == pUser->scene->getCountryID())
						{
							wdExp += pUser->scene->winnerExp(oldExp);
						}
						wdExp = wdExp*(100+5*pUser->adoptList.size())/100;	//һ5%
						pUser->addExp(wdExp, false, npc->tempid, Cmd::MAPDATATYPE_NPC, true);
						pUser->packs.equip.obtain_exp(pUser, wdExp);
					}
					//Ϊӳ
					if(leader->team_mode == Cmd::TEAM_HONOR && early_level < pUser->charbase.level 
							&& pUser->charbase.level <= TEAM_HONOR_MEMBER_LEVEL
							&& leader->scene->id == pUser->scene->id
							&& pUser->scene->checkTwoPosIInNine(pUser->getPosI(),leader->getPosI()))
					{
						DWORD get_honor=0;
						for(;early_level < pUser->charbase.level ; early_level ++)
						{
							zHonorB *hb = honorbm.get(early_level);
							if(hb)
							{
								int honor_temp=0;
								honor_temp = leader->packs.equip.reduceDur(leader , Cmd::EQUIPCELLTYPE_ADORN , ItemType_Honor , hb->value , true , false);
								if(!honor_temp)
								{
									honor_temp = leader->packs.equip.reduceDur(leader , Cmd::EQUIPCELLTYPE_ADORN + 1 , ItemType_Honor , hb->value , true , false);
								}
								get_honor += honor_temp; 
							}
						}
						if(get_honor)
						{
							leader->charbase.honor += get_honor;
							leader->charbase.maxhonor += get_honor;
							zObject::logger(0,0,"ֵ",leader->charbase.honor,get_honor,1,pUser->id , pUser->name ,leader->id,leader->name,"ֵ",NULL,0,0);
							Channel::sendSys(leader, Cmd::INFO_TYPE_GAME,"%d",get_honor);
							Cmd::stMainUserDataUserCmd  userinfo;
							leader->full_t_MainUserData(userinfo.data);
							leader->sendCmdToMe(&userinfo,sizeof(userinfo));
						}
					}
				}
			}
		}
		return true;
	}

};
/**
 * \brief ־Ļص
 *
 */
struct SpecialExpTeamExec : public TeamMemExec
{
	///Ա
	SceneUser *user;
	///ֵ
	SceneNpc *npc;
	DWORD exp;
	///ID

	/**
	 * \brief 캯
	 *
	 * \param u õû
	 * \param wdExp 
	 */
	SpecialExpTeamExec(SceneUser *u , SceneNpc *n , DWORD wdExp)
	{
		user = u;
		npc=n;
		exp = wdExp/10;
		exp = exp?exp:1;
	}
	/**
	 * \brief Ǯĺ
	 *
	 *
	 * \param member 
	 * \return Ƿص
	 */
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
		if(pUser)
		{
			// ־鲿ֲԼ
			if(pUser == user)
			{
				return true;
			}
			if(pUser->getState() != SceneUser::SceneEntry_Death)
			{
				if(user->scene->id == pUser->scene->id
						&& pUser->scene->checkTwoPosIInNine(pUser->getPosI(),user->getPosI()))
				{
					pUser->addExp(exp, false, npc->tempid, Cmd::MAPDATATYPE_NPC);
				}
			}
		}
		return true;
	}

};
/**
 * \brief ־
 *
 */
void SceneNpc::distributeExp()
{
	NpcHpHashmap_iterator iter;
	for(iter = expmap.begin() ; iter != expmap.end() ; iter ++)
	{
		if (abs(iter->second.attack_time.sec() - SceneTimeTick::currentTime.sec()) >=10)
		{
#ifdef _ZJW_DEBUG
			Zebra::logger->debug("%d һιʮ", iter->first);
#endif			
			continue;
		}

		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(iter->first);
		if(pUser)
		{
			if(pUser->team.getLeader() && pUser->team.isNormalExp())
			{
				SceneUser *leader = SceneUserManager::getMe().getUserByTempID(pUser->team.getLeader());
				if(leader)
				{
					if(this->getMaxHP() > 0)
					{
						//float per = (((pUser->team.getExpSize(this->getPosI()) - 1.0f)/2.0f) * 5.0f + 5.0f)/100.0f;
						float per = ((leader->team.getExpSize(this->getPosI() , this->scene->id) - 1.0f) *20.0f)/100.0f;
						leader->team.calAverageExp(this->getPosI(),this->scene->id);
						DWORD exp = (DWORD)(((float)npc->exp * ((float)iter->second.wdHP / (float)(this->getMaxHP()))) * (1.0f + per)  + 0.9f);

						ExpTeamExec exec(leader , this , exp);
						leader->team.execEveryOne(exec);
					}
				}
			}
			else
			{
				DWORD wdExp = (DWORD)(npc->exp * ((float)iter->second.wdHP / (this->getMaxHP())) + 0.9f);

				//Ӿֵ
				wdExp +=pUser->pkValue.exp;
				//
				if (pUser->pet) pUser->pet->addExp(wdExp);
				//
				for (SceneUser::adopt_it it=pUser->adoptList.begin(); it!=pUser->adoptList.end(); it++)
					it->second->releaseExp(wdExp*15/100);
				wdExp = levelExp(wdExp , pUser->charbase.level);
				wdExp = addOtherExp(wdExp , pUser);
				if (wdExp >0)
				{
				   //if (pUser->scene &&(pUser->charbase.country == pUser->scene->getCountryID())) wdExp = (DWORD)(wdExp*(pUser->wdTirePer/100.0f));

					if (pUser->isSpecWar(Cmd::COUNTRY_FORMAL_DARE))
					{//սڣ־50%
						wdExp = wdExp/2;
					}
					
					//soke жǷǾ鱶״̬
					if (pUser->issetUState(Cmd::USTATE_EXP_125))
					{
					    //Zebra::logger->debug("ԭֵΪ:%u",wdExp);
						wdExp = (DWORD)(wdExp*1.25);
						//Zebra::logger->debug("1.25ֵΪ:%u",wdExp);
					}

					if (pUser->issetUState(Cmd::USTATE_EXP_150))
					{
					    //Zebra::logger->debug("ԭֵΪ:%u",wdExp);
						wdExp = (DWORD)(wdExp*1.5);
						//Zebra::logger->debug("1.50ֵΪ:%u",wdExp);
					}

					if (pUser->issetUState(Cmd::USTATE_EXP_175))
					{
					    //Zebra::logger->debug("ԭֵΪ:%u",wdExp);
					    wdExp = (DWORD)(wdExp*1.75);
					    //Zebra::logger->debug("1.75ֵΪ:%u",wdExp);
					}

					if (wdExp==0) wdExp=1;
					//pUser->sendExpToSept(wdExp);
					pUser->packs.equip.obtain_exp(pUser,wdExp);
					
					//־鷽ʽ
					if(pUser->team.IsTeamed() && pUser->team.isSpecialExp())
					{
						SceneUser *leader = SceneUserManager::getMe().getUserByTempID(pUser->team.getLeader());
						if(leader)
						{
							SpecialExpTeamExec exec(pUser, this,wdExp);
							leader->team.execEveryOne(exec);
						}
					}
					DWORD oldExp = wdExp;
					wdExp += pUser->scene->sceneExp(wdExp);
					if(pUser->charbase.country == pUser->scene->getCountryID())
					{
						wdExp += pUser->scene->winnerExp(oldExp);
					}
					wdExp = wdExp*(100+5*pUser->adoptList.size())/100;	//һ5%
					pUser->addExp(wdExp, false, this->tempid, Cmd::MAPDATATYPE_NPC, true);
				}
			}
		}
	}
	expmap.clear();
}

DWORD SceneNpc::addOtherExp(DWORD wdExp , SceneUser *pUser)
{
	if(pUser)
	{
		if(pUser->issetUState(Cmd::USTATE_GUARD))
		{
			wdExp = (DWORD)(wdExp * 1.5f);
		}
		BYTE per = pUser->packs.equip.getEquips().get_doublexp();
		if ((per>20)&&(per<100)) per=20;
		else if (per>=100) per=100;
		///////////////////////////////////////////////
        //soke ־鱶
		if(ScenesService::getInstance().double_exp > 0)
		{
			wdExp = wdExp*ScenesService::getInstance().double_exp ;
		}

		if(zMisc::selectByPercent(per))
		///////////////////////////////////////////////
		{
			wdExp <<= 1;
		}
	}
	return wdExp;
}

/*
DWORD SceneNpc::levelExp(DWORD wdExp , DWORD char_level)
{
	int diff = (int)(char_level - this->npc->level);
	if(diff >= 6 && diff < 10)
	{
		wdExp = (DWORD)((float)wdExp * 0.5f);
	}
	else if(diff >= 10)
	{
		wdExp = 1;
	}
	return wdExp;
}*/

//soke ޸Ĵֵĵȼ
DWORD SceneNpc::levelExp(DWORD wdExp , DWORD char_level)
{
	int diff = (int)(char_level - this->npc->level);
	if(diff >= 6 && diff <= 100)
	{
		wdExp = (DWORD)((float)wdExp * 0.5f);
	}
	else if(diff > 100 && diff <= 150)
	{
		DWORD Exp = (DWORD)((float)wdExp * 0.5f);
		wdExp = Exp/2;
	}
	else if(diff > 150)
	{
		wdExp = 1;
	}
	return wdExp;
}

void SceneNpc::changeHP(const SDWORD &curHp)
{
	SDWORD changeValue = 0;

	if (((int)this->hp)+(int)curHp>=0)
	{
		changeValue = this->hp;
		this->hp += curHp;
		if (this->hp > this->getMaxHP()) this->hp = this->getMaxHP();
		changeValue = (int)this->hp-changeValue;
	}
	else
	{
		changeValue= this->hp;
		this->hp=0;
	}

	notifyHMS = true;

	if (changeValue !=0)
	{
		Cmd::stObjectHpMpPopUserCmd ret;
		ret.dwUserTempID = this->tempid;
		ret.byTarget = Cmd::MAPDATATYPE_NPC;
		ret.vChange = (int)changeValue;
		ret.type = Cmd::POP_HP;
		this->scene->sendCmdToNine(getPosI(), &ret , sizeof(ret));
	}
}


void SceneNpc::changeMP(const SDWORD &mp)
{
    notifyHMS = true;
}

void SceneNpc::changeSP(const SDWORD &sp)
{
    notifyHMS = true;
}


bool SceneNpc::preAttackMe(SceneEntryPk *pEntry, const Cmd::stAttackMagicUserCmd *rev, bool physics, const bool good)
{
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::preAttackMe %s hp=%u", name, hp);
#endif
	if(getState() != zSceneEntry::SceneEntry_Normal
			|| !canBeAttack())  
	{
		if (rev)
			ScenePk::attackFailToMe(rev, pEntry, true);

		return false;
	}

	if (this->npc->kind == NPC_TYPE_RESOURCE)
	{
		if (pEntry->getType() != zSceneEntry::SceneEntry_Player)
		{
			if (rev)
				ScenePk::attackFailToMe(rev, pEntry, true);
			return false;
		}
		else
		{
			if (rev)
			{
				if (rev->wdMagicType != SKILLNORMAL)
				{
					return false;
				}
			}
	//soke ȡɼ	ȡԶ	
	////////////////////////////////////////////////////////		
	//		SceneUser *pUser = (SceneUser *)pEntry;
	//		zObject *temp=NULL;
			bool ret=false;
	//		if(pUser->packs.equip.getObjectByZone(&temp , 0 , Cmd::EQUIPCELLTYPE_HANDR))
			{
	//			if(temp)
				{
	//				if (0 != temp->data.dur)
					{
	//					if (temp->base->id == 876) //ɼ
	////////////////////////////////////////////////////////					
						{
							ret=true;
						}
					}
				}
			}
			if (!ret)
			{
				if (rev)
					ScenePk::attackFailToMe(rev, pEntry, true);
				return false;
			}
		}
	}

	if (!good)
	{
		DWORD attackRating = 0; // 
		DWORD attLevel = 0;
		DWORD attCountryID = 0;
		bool  isUnionCityWar = false;
		bool  isCountryFormal = false; // жϴ󽫾빥Ƿڵж
		bool  isAtt = true; // ĬΪսеĹ
		bool  isAttCountry = true; // սеĹ
		bool  isAntiCountry = false;

		SceneEntryPk * m = ((SceneEntryPk *)pEntry)->getMaster();
		while (m && m->getType()!=zSceneEntry::SceneEntry_Player)
			m = m->getMaster();

		SceneUser * master = NULL;

		if (m)
		{
			master = (SceneUser*)m;
			attackRating = master->charstate.attackrating;
			attLevel = master->charbase.level;
			attCountryID = master->charbase.country;
			isUnionCityWar = master->isSpecWar(Cmd::UNION_CITY_DARE);

			isCountryFormal = master->isWarRecord(Cmd::COUNTRY_FORMAL_DARE, this->scene->getCountryID());
			isAtt	= master->isAtt(Cmd::UNION_CITY_DARE);
			isAttCountry = master->isAtt(Cmd::COUNTRY_FORMAL_DARE);
			isAntiCountry = master->isAntiAtt(Cmd::COUNTRY_FORMAL_DARE, this->scene->getCountryID());

			refreshExpmapAttackTime(master);
		}
		else
		{
			attackRating = ((SceneNpc*)pEntry)->npc->rating;
			attLevel = ((SceneNpc *)pEntry)->getLevel();
			attCountryID = ((SceneNpc *)pEntry)->scene->getCountryID();
		}

		if (this->id == 58101) // ID
		{
//			if (!isUnionCityWar || 	(this->scene->getCountryID() != attCountryID && this->scene->getCountryID()!=6) || !isAtt)
			if (!isUnionCityWar || !isAtt)

			{
#ifdef _ZJW_DEBUG
				Channel::sendNine(this, "ڶսǹ,ܹ58101");
#endif			

				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		//soke ǿʱ䲻ܹNPC
		if (this->id > 0 && (atoi(Zebra::global["service_flag"].c_str())==9))
		{
            Channel::sendNine(this, "ûʱأ۸˼");
			return false;
		}
		
		//ѫɢ
		if (this->id==140003)
		{
			zRTime cur_time;
			cur_time.now();
			if(cur_time.sec() - atoi(Zebra::global["pigtime"].c_str()) >= 120)
			{
				char a[32];
				sprintf(a, "%d", cur_time.sec());
				Zebra::global["pigtime"]=a;
				//ͻʱ俪ʼʱ
				Cmd::stJingcaiReturnPigTimeCmd ret;
				ret.pigtime = atoi(Zebra::global["pigtime"].c_str());
				Scene * s = SceneManager::getInstance().getSceneByName("ѫͼ");
				s->sendCmdToScene(&ret, sizeof(ret));

				Cmd::stJingcaiReturnKillPigTimeCmd ret2;
				ret2.killpigtime = 0;
				ret2.killpigtime_hm = 0;
				s->sendCmdToScene(&ret2, sizeof(ret2));	
			}
		}	
		//BOSS
		if (this->id==144006)
		{
			if (master->charbase.country==12)
			{
				if(atoi(Zebra::global["BOSS_CHU"].c_str())==0)
				{
					Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "BOSSδɱܽռBOSS"); 
					return false;	
				}
			}

			if (master->charbase.country==8)
			{
				if(atoi(Zebra::global["BOSS_HAN"].c_str())==0)
				{
					Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "BOSSδɱܽռBOSS"); 
					return false;	
				}
				

			}
		}



		if (this->id == COUNTRY_MAIN_FLAG)
		{
			if (!CountryDareM::getMe().isAttackMainFlag(this->scene, COUNTRY_SEC_FLAG))
			{
				if(master)  
				{
					Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "ڸ죬ܹ"); 
				}
#ifdef _ZJW_DEBUG
				Channel::sendNine(this, "ʧܣڸ");
#endif			

				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if (this->id==COUNTRY_MAIN_FLAG || this->id==COUNTRY_SEC_FLAG)
		{

			if (this->scene->getCountryID() ==attCountryID)
			{
#ifdef _ZJW_DEBUG
				Channel::sendNine(this, "˲");
#endif			
				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);
				return false;
			}

			if (attLevel>50)
			{
#ifdef _ZJW_DEBUG
				Channel::sendNine(this, "50˳ڵĹ");
#endif			

				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if (this->id==COUNTRY_KING_MAIN_FLAG || this->id==COUNTRY_KING_SEC_FLAG)
		{
			if (this->scene->getCountryID() ==attCountryID)
			{
#ifdef _ZJW_DEBUG
				Channel::sendNine(this, "˲");
#endif			
				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;

			}

			if (isCountryFormal)
			{
				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if (this->id == COUNTRY_KING_MAIN_FLAG)
		{
			if (!CountryDareM::getMe().isAttackMainFlag(this->scene, COUNTRY_KING_SEC_FLAG))
			{
				if(master)  
				{
					Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "ڸ죬ܹ"); 
				}
#ifdef _ZJW_DEBUG
				Channel::sendNine(this, "ʧܣڸ");
#endif			
				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if (this->isMainGeneral() || this->id==COUNTRY_SEC_GEN)
		{//˲󽫾齫
			if (this->scene->getCountryID() ==attCountryID)
			{
				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);
				return false;
			}
		}
		if (this->isMainGeneral())
		{//жǷ󽫾
			if (!isCountryFormal || !CountryDareM::getMe().isAttackMainGen(this->scene) ||
					(!isAttCountry && !isAntiCountry))
			{
				if (master)
				{
					Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "ڹսڼǽܹ󽫾");
				}

				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if (this->id==COUNTRY_EMPEROR_MAIN_GEN || this->id==COUNTRY_EMPEROR_SEC_GEN)
		{//ռ˲ʳǴ󽫾ͻʳ齫
			if (this->scene->getEmperorDare())
			{
				if (this->scene->getEmperorDareDef() == attCountryID)
				{// Լ˲Լ
					if (rev)
						ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);
					return false;
				}
			}
			else
			{//ڻʳսڼ,˭ܴʳǴ󽫾齫
				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);
				return false;
			}
		}

		if (this->id==COUNTRY_EMPEROR_MAIN_GEN)
		{//жǷʳǴ󽫾
			if (!this->scene->getEmperorDare() 
					|| !CountryDareM::getMe().isAttackMainFlag(this->scene, COUNTRY_EMPEROR_SEC_GEN))
			{
				if (master) 
				{
					//Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "ڽӳǲսܹ󽫾"); 
					Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "ڻʳսڼнӳܹ󽫾");
				}

				if (rev)
					ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

				return false;
			}
		}

		if ((this->id == COUNTRY_SEC_GEN) && !this->scene->getCountryDare())
		{//жǷ齫
			if (master) 
			{
				Channel::sendSys(master, Cmd::INFO_TYPE_FAIL, "ڹսڼ䣬ܹӳ"); 
			}

			if (rev)
				ScenePk::attackFailToMe(rev, (SceneUser*)pEntry, true);

			return false;
		}
	}

	//ʼpk
	this->pkValue.init();
	this->skillValue.init();

	this->skillStatusM.processPassiveness();	// ҵı״̬Ӱ

	if (pEntry->getType() == SceneEntry_Player)
	{
		SceneUser *pAtt = (SceneUser *)pEntry;
		ScenePk::calpdamU2N(rev , pAtt , this);
	}
	else
	{
		SceneNpc *pAtt = (SceneNpc *)pEntry;
		ScenePk::calpdamN2N(rev, pAtt , this);
	}
	if (pEntry->getType() == SceneEntry_Player)
	{
		SceneUser *pAtt = (SceneUser *)pEntry;
		ScenePk::calmdamU2N(rev , pAtt , this);
	}
	else
	{
		SceneNpc *pAtt = (SceneNpc *)pEntry;
		ScenePk::calmdamN2N(rev, pAtt , this);
	}

	SceneEntryPk *pMaster = this->getTopMaster();
	if (pMaster&&pMaster->getType() == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = (SceneUser *)pMaster;
		pUser->packs.equip.costDefenceDurByPet(pUser);
	}
	return true;
}

bool SceneNpc::AttackMe(SceneEntryPk *pAtt, const Cmd::stAttackMagicUserCmd *rev, bool physics, SWORD rangDamageBonus)
{
	//by=>friday ޸ڳɱ - ֹκι߹ڳ
	if (npc && npc->id == 54099) //õڳ
	{
		//ڳܱκηʽٻ
		SceneEntryPk * am = pAtt->getTopMaster();
		if (am->getType()==zSceneEntry::SceneEntry_Player)
		{
			SceneUser * attacker = (SceneUser *)am;
			Channel::sendSys(attacker, Cmd::INFO_TYPE_FAIL, "˹ڳѱӣ޷");
		}
		return false;
	}
	
	using namespace Cmd;

	SceneEntryPk * am = pAtt->getTopMaster();
	if (am->getType()==zSceneEntry::SceneEntry_Player)
	{
		SceneUser * a = (SceneUser *)am;
		//˴һʱ֪ͨ
		if (hp==getMaxHp() && scene->getCountryID()!=a->charbase.country)
		{
			char buf[MAX_CHATINFO];
			bzero(buf, sizeof(buf));
			snprintf(buf, MAX_CHATINFO-1, "%s(%s)  %s(%u,%u)  %s,֧Ԯ"
					, a->name, SceneManager::getInstance().getCountryNameByCountryID(a->charbase.country)
					, scene->name, getPos().x, getPos().y, name);

			//NPCȫ֪ͨ
			if (!isBugbear() && canBeAttack())
				Channel::sendCountryInfo(scene->getCountryID(), Cmd::INFO_TYPE_EXP, buf);

			//󽫾ͽӳǡ߾Ͻ֪ͨ
			if (isMainGeneral() || id==COUNTRY_SEC_GEN)
			{
				DWORD high = (scene->getCountryID()<<16);
				Channel::sendMapInfo(high+WANGCHENG_MAP_ID, Cmd::INFO_TYPE_EXP, buf);
				Channel::sendMapInfo(high+DONGJIAO_MAP_ID, Cmd::INFO_TYPE_EXP, buf);
				Channel::sendMapInfo(high+NANJIAO_MAP_ID, Cmd::INFO_TYPE_EXP, buf);
				Channel::sendMapInfo(high+BIANJING_MAP_ID, Cmd::INFO_TYPE_EXP, buf);
			}
		}
			
		if (hp==getMaxHp() && scene->getCountryID()!=a->charbase.country)
		{
			//NPC󳼣ȫ֪ͨ
			if (npc->id==227)
			{
				Channel::sendCountryInfo(scene->getCountryID(), Cmd::INFO_TYPE_GAME1,"%s%sڹ󳼣ʿٻ!Ͻɨ!", SceneManager::getInstance().getCountryNameByCountryID(a->charbase.country), a->name);
			}
		}
		else if (hp<=getMaxHp()*0.4 && scene->getCountryID()!=a->charbase.country)
		{
			//NPC󳼣ȫ֪ͨ
			if (npc->id==227)
			{
				Channel::sendCountryInfo(scene->getCountryID(), Cmd::INFO_TYPE_GAME4,"%s%sڹ󳼣ѪΪ30%ʿٻ!Ͻɨ!", SceneManager::getInstance().getCountryNameByCountryID(a->charbase.country), a->name);
			}
		}
		else if (hp<=getMaxHp()*0.7 && scene->getCountryID()!=a->charbase.country)
		{
			//NPC󳼣ȫ֪ͨ
			if (npc->id==227)
			{
				Channel::sendCountryInfo(scene->getCountryID(), Cmd::INFO_TYPE_GAME2,"%s%sڹ󳼣ѪΪ60%ʿٻ!Ͻɨ!", SceneManager::getInstance().getCountryNameByCountryID(a->charbase.country), a->name);
			}
		}
	}

	if (pAtt->getMaster())
	{
		this->setLockUser(pAtt->getMaster()->id);
	}
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::AttackMe %s hp=%u", name, hp);
#endif
	SceneEntryPk::AttackMe(pAtt,rev,physics,rangDamageBonus);

	if (Cmd::PET_TYPE_NOTPET!=getPetType())
	{
		((ScenePet *)this)->sendHpExp();
	}
	//AI
	if (AIC)
		AIC->on_hit(pAtt);
	if (aif&AIF_GIVEUP_6_SEC)
		setEndBattleTime(SceneTimeTick::currentTime, 6*1000);
	if (aif&AIF_GIVEUP_3_SEC)
		setEndBattleTime(SceneTimeTick::currentTime, 3*1000);
	//setAttackTime(SceneTimeTick::currentTime,1200);
	return true;
}


struct HpTeamExec : public TeamMemExec
{
	const Cmd::stNPCHPMapScreenUserCmd &ret;
	HpTeamExec(const Cmd::stNPCHPMapScreenUserCmd &cmd) : ret(cmd)
	{
	}
	bool exec(TeamMember &member)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(member.tempid);
		if(pUser)
		{
			pUser->sendCmdToMe(&ret , sizeof(ret));
			return true;
		}
		else
		{
			return true;
		}
	}
};

/**
 * \brief жһǷڹԼ
 * ݾбж
 * ֻжҺͳ
 *
 * \param entry ҪжϵĶ
 * \return ǷڹԼ
 */
bool SceneNpc::isAttackMe(SceneEntryPk *entry)
{
	if(!entry) return false;

	if (zSceneEntry::SceneEntry_NPC==entry->getType())
	{
		if (Cmd::PET_TYPE_NOTPET!=((SceneNpc *)entry)->getPetType())
			return isAttackMe(((SceneNpc *)entry)->getMaster());
		else
			return false;
	}
	else if (zSceneEntry::SceneEntry_Player==entry->getType())
	{
		SceneUser * pUser = (SceneUser *)entry;
		if(pUser->team.getLeader())
		{
			NpcHpHashmap_iterator iter;
			for(iter = expmap.begin() ; iter != expmap.end() ; iter ++)
			{
				SceneUser *u = SceneUserManager::getMe().getUserByTempID(iter->first);
				if(u)
				{
					if (pUser->team.getLeader() == u->team.getLeader())
					{
						return true;
					}
				}
			}
		}
		else
		{
			NpcHpHashmap_iterator iter;
			iter = expmap.find(pUser->tempid);
			if(iter != expmap.end())
			{
				return true;
			}
		}
	}
	return false;
}
void SceneNpc::showHP(SceneUser *pUser, QWORD npchp)
{
	if(!pUser)
	{
		return ;
	}
	Cmd::stNPCHPMapScreenUserCmd ret;
	ret.dwMapNpcDataPosition = tempid;
	ret.dwHP = npchp;//this->hp;				
	ret.dwMaxHP = getMaxHP();
	pUser->sendCmdToMe(&ret , sizeof(ret));
}
/**
 * \brief ֪ͨͻֵı仯
 * \author fqnewman
 */
void SceneNpc::attackRTHpAndMp()
{
    notifyHMS = false;
}


/**
 * \brief 캯
 * \param scene npcڵĳ
 * \param npc 
 * \param define 
 * \param type 
 * \param entrytype (ҡnpc)
 * \param anpc ǿnpc base
 */
SceneNpc::SceneNpc(Scene *scene, zNpcB *npc, const t_NpcDefine *define, const SceneNpcType type, const SceneEntryType entrytype, zNpcB *anpc) : SceneEntryPk(entrytype, SceneEntry_Hide),npc(npc), anpc(anpc), define(define), _half_sec(0.3f), _one_sec(1), _3_sec(3), lockedUserTime(), nextMoveTime(npc->distance), nextAttackTime(npc->adistance), type(type), isUse(false)
{
	id = npc->id;
	this->scene = scene;
	if (type == STATIC)
	{
		tempid = ++serialID;
	}
	else if (type == GANG)
	{
		tempid = uniqueID.get();
		if (tempid==uniqueID.invalid())
			Zebra::logger->fatal("%s ʱŷ", __PRETTY_FUNCTION__);
	}
	if ((char)define->name[0]) {
		strncpy(name, define->name, MAX_NAMESIZE);
	}else {
		strncpy(name, npc->name, MAX_NAMESIZE);
	}

	catchme = 0; //˹־
	boostupPet =0; // ǿ
	boostupPetMDef = 0; //ǿķ
	boostupSummon = 0; //ٻ޹ǿ
	dwReduceDam = 0; //ٻ˺ۼ
	giddy =0; //ŭѵʹԷѣεļ
	boostupHpMaxP =0; // ǿֵٷֱ

	hp = this->getMaxHP();//npc->hp+(anpc?anpc->hp:0);
	chaseMode = CHASE_NONE;
	curTargetType = 0;
	curTargetID = 0;
	defTargetType = 0;
	defTargetID = 0;
	dwNpcLockedUser = 0;
	lostObject = false;
	notifyHMS = false;
	clearMe = false;
	backOffing = 0;
	dwStandTime =SceneTimeTick::currentTime.sec();
	dwStandTimeCount=0;
	skillStatusM.initMe(this);

	speedRate = 1.0;
	aspeedRate = 1.0;
	speedUpUnder20 = false;
	aspeedUpUnder50 = false;
	recoverUnder30 = false;
	//master = NULL;

	appendMinDamage = 0;
	appendMaxDamage = 0;

	AIC = new NpcAIController(this);

	AIC->setActRegion(define->region.c, define->width/2, define->height/2);
	AIC->loadScript(define->scriptID);

	isRushNpc = false;
	lockTarget = false;

	needRecover = false;

	notifystep=0; // boss ֪ͨ

	//petAI = 0;
	aif = npc->ai;
#ifdef _XWL_DEBUG
	//aif = npc->ai | AIF_LIMIT_REGION;
#endif
	//
	pet = 0;
	summon = 0;
	totems.clear();

	summoned = false;

	switch(this->npc->kind)
	{
		case NPC_TYPE_TOTEM:
			{
				switch(this->npc->id)
				{
					case 300:
						{
							this->setUState(39);
						}
						break;
					default:
						{
							this->setUState(39);
						}
						break;
				}
			}
			break;
		default:
			break;
	}

	///npc
	if (!npc->skillMap.empty())
	{
		std::map<int, std::vector<npcSkill> >::iterator map_it;
		for (map_it=npc->skillMap.begin();map_it!=npc->skillMap.end();map_it++)
		{
			std::vector<npcSkill>::iterator skill_it;
			for (skill_it=map_it->second.begin();skill_it!=map_it->second.end();skill_it++)
			{
				//zSkill::create(this , skill_it->id , 1);

				zSkill *skill = zSkill::create(this , skill_it->id , 1);
				if(!skill)
					Zebra::logger->error("SceneNpc::SnceneNpc():޷ؼ name=%s skill=%u level=%u",npc->name, skill_it->id, npc->level);
#ifdef _DEBUGLOG
				else
					Zebra::logger->error("NPC[%s]ؼskill=[%u] level=[%u]",npc->name, skill_it->id, npc->level);
#endif
			}
		}
	}
	if (npc->skill>0)
	{
		zSkill::create(this , npc->skill , npc->level) ;
	}
}

/**
 * \brief 
 *
 */
SceneNpc::~SceneNpc()
{
	if (type == GANG)
	{
		uniqueID.put(tempid);
		SAFE_DELETE(define);
	}
	SAFE_DELETE(AIC);
}

/**
 * \brief Npc״̬
 * \return ״̬
 */
SceneNpc::SceneNpcChase SceneNpc::getChaseMode() const
{
    return chaseMode;
}

/**
 * \brief ȡû
 * \return û
 */
SceneEntryPk* SceneNpc::getChaseSceneEntry() const
{
	switch (curTargetType)
	{
		case zSceneEntry::SceneEntry_Player:
			{
				return SceneUserManager::getMe().getUserByTempID(curTargetID);
			}
			break;
		case zSceneEntry::SceneEntry_NPC:
			{
				return SceneNpcManager::getMe().getNpcByTempID(curTargetID);
			}
			break;
		default:
			{
				return NULL;
			}
			break;
	}
	return NULL;
}

/**
 * \brief øû
 * \param type ٶ
 * \param entryid Ҫûı
 * \return Ƿٳɹ
 */
bool SceneNpc::chaseSceneEntry(const DWORD type, const DWORD entryid)
{
    //Լ
    if (type==(DWORD)getType()&&entryid==tempid) return false;

	if (curTargetID!=0)
			return false;
	else
	{
			chaseMode = CHASE_ATTACK;
			//curTargetType = type;
			//curTargetID = entryid;
			setCurTarget(entryid, type);
			if (aif&AIF_GIVEUP_10_SEC)
					setEndBattleTime(SceneTimeTick::currentTime, 10*1000);
			return true;
	}
}

/**
 * \brief ǿƸûѾڸûô45%ļʽĿתĿǰû
 * \param userid Ҫûı
 * \return Ƿٳɹ
 */
bool SceneNpc::forceChaseUser(SceneEntryPk *pAtt)
{
	//if (chaseMode != CHASE_NONE)
	if (curTargetID!=0)
	{
		if (zMisc::selectByPercent(30))
		{
			chaseMode = CHASE_ATTACK;
			setCurTarget(pAtt);
			//Channel::sendNine(this, "Ŀ%s", pAtt->name);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		chaseMode = CHASE_ATTACK;
		setCurTarget(pAtt);
		AIC->on_find_enemy(pAtt);
		if (aif&AIF_GIVEUP_10_SEC)
			setEndBattleTime(SceneTimeTick::currentTime, 10*1000);

		return true;
	}
}

/**
 * \brief ȡûи
 *
 */
void SceneNpc::unChaseUser()
{
	if (0!=curTargetID)
	{
		chaseMode = CHASE_NONE;
		leaveBattle();
		lockTarget = false;
#ifdef _XWL_DEBUG
		//Zebra::logger->debug("%s Ŀ", name);
#endif
	}
}

/**
 * \brief ûƷʱʹ
 * \param ct ǰʱ
 */
bool SceneNpc::checkLockUserOverdue(const zRTime &ct)
{
	if (getState() == zSceneEntry::SceneEntry_Normal
			&& dwNpcLockedUser
			&& ct > lockedUserTime)
	{
		//ֵ0֮ǰȡ״̬
		SceneUser *pUser = SceneUserManager::getMe().getUserByID(dwNpcLockedUser);
		if (pUser)
		{
			
			if(pUser->issetUState(Cmd::USTATE_BOSSGUISHU))
			{
				pUser->showCurrentEffect(Cmd::USTATE_BOSSGUISHU, false ); 
			}		
		
		}

		dwNpcLockedUser = 0;
		lockedUserTime = ct;
		return true;
	}
	return false;
}

/**
 * \brief û
 * \param dwID û
 */
void SceneNpc::setLockUser(const DWORD dwID)
{
    checkLockUserOverdue(SceneTimeTick::currentTime);
    if (dwID == dwNpcLockedUser && dwID != 0)
    {
		SceneUser *pUser = SceneUserManager::getMe().getUserByID(dwNpcLockedUser);
		if(pUser)
		{
			
			pUser->BossGsStateTime = 10;	
		}
		lockedUserTime = SceneTimeTick::currentTime;
		lockedUserTime.addDelay(10000);
    }
    else if(dwNpcLockedUser == 0)
    {
	dwNpcLockedUser = dwID;
	SceneUser *pUser = SceneUserManager::getMe().getUserByID(dwNpcLockedUser);
	if(pUser)
	{
		if(!pUser->issetUState(Cmd::USTATE_BOSSGUISHU))
		{
			pUser->BossGsStateTime = 10;
			pUser->showCurrentEffect(Cmd::USTATE_BOSSGUISHU, true ); 
		}		
	}

	//״̬
	lockedUserTime = SceneTimeTick::currentTime;
	lockedUserTime.addDelay(10000);
    }
}

/**
 * \brief NpcǷѾƶʱ
 * \param ct ǰʱ
 * \return жǷɹ
 */
bool SceneNpc::checkMoveTime(const zRTime &ct)
{
    return ct >= nextMoveTime;
}

/**
 * \brief Npcһƶʱ
 * \param ct ǰʱ
 */
void SceneNpc::setMoveTime(const zRTime &ct)
{
    nextMoveTime = ct;
    nextMoveTime.addDelay((int)(npc->distance/speedRate));
    //Channel::sendNine(this, "ƶӳ1 %u ", (int)(npc->distance/speedRate));
}

/**
 * \brief Npcһƶʱ
 * \param ct ǰʱ
 * \param delay ӳʱ䣬
 */
void SceneNpc::setMoveTime(const zRTime &ct, const int delay)
{
    nextMoveTime = ct;
    nextMoveTime.addDelay((int)(delay/speedRate));
    //Channel::sendNine(this, "ƶӳ2 %u ", (int)(delay/speedRate));
}

/**
 * \brief ӳ´ƶʱ
 *
 *
 * \return 
 */
void SceneNpc::delayMoveTime(const int delay)
{
    nextMoveTime.addDelay(delay);
    if (getState()==zSceneEntry::SceneEntry_Death)
	Zebra::logger->debug("%s ӳٸʱ %u", name, delay);
    //Channel::sendNine(this, "ƶӳ3 %u ", delay);
}

/**
 * \brief NpcǷѾԹʱ
 * \param ct ǰʱ
 * \return жǷɹ
 */
bool SceneNpc::checkAttackTime(const zRTime &ct) const
{
    return ct >= nextAttackTime;
}

/**
 * \brief Npcһιʱ
 * \param ct ǰʱ
 */
void SceneNpc::setAttackTime(const zRTime &ct)
{
    //960벥Ŷʱ
    nextAttackTime = ct;
    nextAttackTime.addDelay((int)(npc->adistance/aspeedRate));
#ifdef _XWL_DEBUG
    //if (!isSpecialNpc())
    //	Zebra::logger->debug("%s ´ι %d ", name, (int)(npc->adistance/aspeedRate));
#endif
    //nextAttackTime.addDelay(npc->distance + 960);
}

/**
 * \brief Npcһιʱ
 * \param ct ǰʱ
 * \param delay ӳʱ䣬
 */
void SceneNpc::setAttackTime(const zRTime &ct, const int delay)
{
    nextAttackTime = ct;
    nextAttackTime.addDelay((int)(delay/aspeedRate));
}

/**
 * \brief жǷԵƷ
 * \param ct ǰʱ
 * \return ǷԵƷ
 */
bool SceneNpc::canLostObject(const zRTime &ct)
{
	if (lostObject)
	{
		reliveTime = SceneTimeTick::currentTime;
		reliveTime.addDelay(define->interval * 1000); 
		if (2==npc->kind || 4==npc->kind || 5==npc->kind || 31==npc->kind)	//2 4 5 31BOSSɱ		
		{
			Zebra::logger->debug("[BOSS]%s(%u) ", name, tempid);
		//soke BOSS´θʾ	
		Channel::sendAllInfo(Cmd::INFO_TYPE_XTYSCROLL, "¹ĻŻţʿĥţ%d %s  %s ط˽磬ٶ", define->interval /60,name, scene->getRealName());
		Channel::sendAllInfo(Cmd::INFO_TYPE_XG2, "¹ĻŻţʿĥţ%d %s  %s ط˽磬ٶ", define->interval /60,name, scene->getRealName());
		}
		//setMoveTime(ct, define->interval * 1000);//ʱ
		//setAttackTime(ct, define->interval * 1000);
		lostObject = false;
		return true;
	}
	else
		return false;
}

/**
 * \brief жǷ
 * \param ct ǰʱ
 * \return Ƿ
 */
// ޸BOSSˢʾ 20 10 1ӵʾ
bool SceneNpc::canRelive(const zRTime &ct)
{
	if (!lostObject
			&& ct > reliveTime)
	{
		//if (npc->kind==NPC_TYPE_BBOSS || npc->kind==NPC_TYPE_LBOSS || npc->kind==NPC_TYPE_PBOSS)
		//	Zebra::logger->debug("boss %s(%u) ", name, tempid);
		setMoveTime(ct);
		setAttackTime(ct);
		notifystep=0;
		if (this->npc->kind == 2 || this->npc->kind == 4 || this->npc->kind == 5 || this->npc->kind == 31 || this->npc->kind == 33)
		{
			Cmd::stRefreshBossUserCmd send;			
			strncpy(send.mapName,this->scene->getName(),sizeof(send.mapName));
			//send.x = this->getPos().x;
			//send.y = this->getPos().y;
			send.time = 0;
			send.npcid = this->npc->id;
			send.country = (BYTE)this->scene->getCountryID();
			this->forwardSession(&send, sizeof(send));
		}
		return true;
	}
	else
	{
		if (this->npc->kind == 2 || this->npc->kind == 4 || this->npc->kind == 5 || this->npc->kind == 31)
		{
			int timegap=500;
			switch(notifystep)
			{
				case 0:
					timegap = 20;
					break;
				case 1:
					timegap = 10;
					break;
				case 3:
					timegap = 1;
					break;
				default:
					break;
			}
			if ((int)((reliveTime.sec()-ct.sec())/60) == timegap)
			{
				Cmd::stRefreshBossUserCmd send;
		        Channel::sendAllInfo(Cmd::INFO_TYPE_XTYSCROLL, "¹ĻŻţʿĥţ%d %s  %s ط˽磬ٶ", ((reliveTime.sec()-ct.sec())/60),name, scene->getRealName());
		        Channel::sendAllInfo(Cmd::INFO_TYPE_XG2, "¹ĻŻţʿĥţ%d %s  %s ط˽磬ٶ", ((reliveTime.sec()-ct.sec())/60),name, scene->getRealName());				
				strncpy(send.mapName,this->scene->getName(),sizeof(send.mapName));
				//send.x = this->getPos().x;
				//send.y = this->getPos().y;
				send.time = timegap;
				send.npcid = this->npc->id;
				send.country = (BYTE)this->scene->getCountryID();
				this->forwardSession(&send, sizeof(send));
				notifystep++;
			}
		}
		/* switch(this->npc->kind)
		{
			case NPC_TYPE_BBOSS:
				{
					int timegap=500;
					switch(notifystep)
					{
						case 0:
							timegap = 20;
							break;
						case 1:
							timegap = 10;
							break;
						case 3:
							timegap = 1;
							break;
						default:
							break;
					}
					if ((int)((reliveTime.sec()-ct.sec())/60) == timegap)
					{
						Cmd::stRefreshBossUserCmd send;
						strncpy(send.mapName,this->scene->getName(),sizeof(send.mapName));
						//send.x = this->getPos().x;
						//send.y = this->getPos().y;
						send.time = timegap;
						send.npcid = this->npc->id;
						send.country = (BYTE)this->scene->getCountryID();
						this->forwardSession(&send, sizeof(send));
						notifystep++;
					}
				}
				break;
			default:
				break;
		} */
		return false;
	}
}

/**
 * \brief תSession
 *
 * \param ptNullCmd ת
 * \param nCmdLen 
 */
bool SceneNpc::forwardSession(const Cmd::stNullUserCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if(nCmdLen > zSocket::MAX_USERDATASIZE)
	{
		Zebra::logger->debug("ϢԽ(%d,%d)",ptNullCmd->byCmd,ptNullCmd->byParam);
	}
	BYTE buf[zSocket::MAX_DATASIZE];
	Cmd::Session::t_Session_ForwardUser *sendCmd=(Cmd::Session::t_Session_ForwardUser *)buf;
	constructInPlace(sendCmd);
	sendCmd->dwID=0;
	sendCmd->size=nCmdLen;
	bcopy(ptNullCmd,sendCmd->data,nCmdLen);
	return sessionClient->sendCmd(buf,sizeof(Cmd::Session::t_Session_ForwardUser)+nCmdLen);
}

/**
 * \brief ȡNpc
 * ̬ĻǶ̬
 * \return Npc
 */
const SceneNpc::SceneNpcType &SceneNpc::getSceneNpcType() const
{
    return type;
}

/**
 * \brief NPCԼϢṹ
 *
 * \param data ҪĽṹ
 */
void SceneNpc::full_t_NpcData(Cmd::t_NpcData &data)
{
    data.dwMapNpcDataPosition=tempid;
    data.dwNpcDataID=id;
    data.byDirect=dir;
    data.movespeed = (DWORD)(npc->distance/speedRate);
    //data.movespeed = (DWORD)(640/speedRate);
    //if (skillValue.movespeed!=0)
    //setSpeedRate(getSpeedRate()*640.0f/((float)data.movespeed));
    //bcopy(byState, data.byState, sizeof(byState));
    data.level = getLevel();
}
/**
 * \brief NPCԼϢṹ
 *
 * \param data ҪĽṹ
 */
void SceneNpc::full_t_MapNpcData(Cmd::t_MapNpcData &data)
{
	full_t_NpcData(*((Cmd::t_NpcData*)&data));
	full_all_UState(data.byState);
}
/**
 * \brief NPCԼϢṹ
 *
 * \param data ҪĽṹ
 */
void SceneNpc::full_t_MapNpcDataAndPos(Cmd::t_MapNpcDataPos &data)
{
	full_t_MapNpcData(*(Cmd::t_MapNpcData*)&data);
	//
	data.x=getPos().x;
	data.y=getPos().y;
	data.byDir=getDir();
}
/**
 * \brief NPCԼϢṹ
 *
 * \param data ҪĽṹ
 */
void SceneNpc::full_t_MapNpcDataState(Cmd::t_MapNpcDataState &data)
{
	full_t_NpcData(*((Cmd::t_NpcData*)&data));
	data.num = full_UState(data.state);
}
/**
 * \brief NPCԼϢṹ
 *
 * \param data ҪĽṹ
 */
void SceneNpc::full_t_MapNpcDataAndPosState(Cmd::t_MapNpcDataPosState &data)
{
	full_t_NpcData(*((Cmd::t_NpcData*)&data));
	//
	data.x=getPos().x;
	data.y=getPos().y;
	data.byDir=getDir();
	data.num = full_UState(data.state);
}
/**
 * \brief ħλϢṹ
 *
 */
void SceneNpc::full_stRTMagicPosUserCmd(Cmd::stRTMagicPosUserCmd &ret) const
{
    ret.pos.dwTempID=tempid;
    ret.pos.byDir=getDir();
    ret.pos.x=getPos().x;
    ret.pos.y=getPos().y;
    ret.byTarget=Cmd::MAPDATATYPE_NPC;
}

/**
 * \brief Npc
 * \return ִǷɹ
 */
void SceneNpc::action(const zRTime& ctv)
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	if(backOffing > 0)
	{
		backOffing -= 50;
	}

	if (_one_sec(ctv)) 
	{
		this->skillStatusM.timer();
		//TODOֻڷ仯ʱŷ
		if(hp != this->getMaxHP() && getState() == zSceneEntry::SceneEntry_Normal)
		{
			sendtoSelectedHpAndMp();
		}
	}

	if (AIC) AIC->processPhase();

	switch(getState())
	{
		case zSceneEntry::SceneEntry_Normal:
			//ͨNpc
			normalAction();
			break;
		case zSceneEntry::SceneEntry_Death:
			//Npc
			{
				deathAction();
			}
			break;
		case zSceneEntry::SceneEntry_Hide:
			hideAction();
			//صNpc
			//do nothing
			break;
	}
}

/**
 * \brief NpcAI
 * \param ctv ǰʱ
 * \param affectNpc Ҫnpcб
 */
void SceneNpc::AI(const zRTime& ctv, MonkeyNpcs &affectNpc,const DWORD group , const bool every)
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	MonkeyNpcs::iterator oldit;
	for(MonkeyNpcs::iterator it=affectNpc.begin();it!=affectNpc.end();)
	{
		/// ڵǰ鴦ҲҪȫʱ,ͷѭԸnpcĴ
		if (every || ((*it)->tempid % MAX_NPC_GROUP == group))
		{
			if ((*it)->needClear())
			{
				oldit = it;
				it++;
				SceneNpc * delNpc = *oldit;
				affectNpc.erase(oldit);
				Scene *mScene = delNpc->scene;

				//֪ͨͻɾNPC
				Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
				removeNpc.dwMapNpcDataPosition = delNpc->tempid;
				mScene->sendCmdToNine(delNpc->getPosI(), &removeNpc, sizeof(removeNpc));
				//ӵͼ͹ɾNPC
				mScene->removeNpc(delNpc);
				SceneNpcManager::getMe().removeSceneNpc(delNpc);
				SceneNpcManager::getMe().removeSpecialNpc(delNpc); //޸̬ͼ
				//SceneNpcManager::getMe().removeSpecialNpc(delNpc); //ѾɾˣûбҪظ
				//Zebra::logger->trace("%s, ɾnpc %s(%u)", __FUNCTION__, delNpc->name, delNpc->tempid);
				SAFE_DELETE(delNpc);
			}
			else
			{
				(*it)->action(ctv);
				it++;
			}
		}
		else
		{
			it++;
		}
	}
}

/**
 * \brief ڸٵĵ
 *
 * \param entry ҵĶ
 * \return Ƿҳɹ
 */
bool SceneNpc::checkChaseAttackTarget(SceneEntryPk *&entry)
{
	if (getPetType()!=Cmd::PET_TYPE_NOTPET && getPetType()!=Cmd::PET_TYPE_SEMI)
		((ScenePet *)this)->checkMasterTarget(entry);
	if (entry) return true;

	if (curTargetID!=0 || defTargetID!=0)
	{
		entry = getChaseSceneEntry();
		SceneEntryPk * dt = 0;
		bool switchTarget = false;
		if (getPetType()==Cmd::PET_TYPE_NOTPET || getPetType()==Cmd::PET_TYPE_SEMI)
		{
			dt = getDefTarget();
			if (!entry || (dt && zMisc::selectByPercent(1)))
			{
				entry = dt;
				switchTarget = true;
			}
		}
		if (NULL == entry//ҲĿ꣬߻߻ͼ
				|| entry->getState() != zSceneEntry::SceneEntry_Normal//
				|| (!scene->zPosShortRange(getPos(), entry->getPos(), npc_lost_target_region) && !lockTarget)
				|| !canReach(entry) //ʹǰҲ
				//|| ((master==NULL) ?false:!scene->zPosShortRange(getPos(), master->getPos(), npc_chase_region+1))//ٷΧ
				|| (NPC_AI_PATROL==AIDefine.type&&!entry->isRedNamed())
				|| (!(canFight()))
				|| entry->hideme)	//ս
		{
#ifdef _XWL_DEBUG
			//if (entry && !scene->zPosShortRange(getPos(), entry->getPos(), npc_lost_target_region))
			//Zebra::logger->debug("%s->%s 볬 %u ", name, entry->name, npc_lost_target_region);
#endif
			if (!chaseSecondTarget()&&!chaseItsMaster())
				unChaseUser();//׷
		}
		else
		{
			if (1==isEnemy(entry))
			{
#ifdef _XWL_DEBUG
				//Channel::sendNine(this, "%s ", entry->name);
#endif
				if (switchTarget)
				{
					setCurTarget(entry, true);
					clearDefTarget();
#ifdef _XWL_DEBUG
					//Zebra::logger->debug("%s лĿ %s", name, entry->name);
#endif
				}
				return true;
			}
			else
			{
				unChaseUser();//׷
			}
		}
	}

	entry = NULL;
	return false;
}


/**
 * \brief жϵǷڹΧ
 *
 *
 * \param entry ָ
 * \return ǷڹΧ
 */
bool SceneNpc::inRange(SceneEntryPk * entry)
{
	BYTE atype = 0;
	BYTE action = 0;
	npc->getATypeAndAction(atype,action);

	switch(atype)
	{
		case NPC_ATYPE_NEAR:	/// 빥
		case NPC_ATYPE_MNEAR:	/// 뷨
			if (!scene->zPosShortRange(getPos(), entry->getPos(), 1))
			{
				//Zebra::logger->debug("빥ɹ");
				return false;
			}
			break;
		case NPC_ATYPE_MFAR:	/// Զ빥
		case NPC_ATYPE_FAR:		/// Զ빥
			if (!scene->zPosShortRange(getPos(), entry->getPos(), 6))
			{
				if (getPetType()!=Cmd::PET_TYPE_NOTPET && scene->zPosShortRange(getPos(), entry->getPos(), 10))
					return true;

				return false;
			}
			else
			{
				if ((Cmd::PET_TYPE_NOTPET==getPetType())&&
						(!scene->zPosShortRange(getPos(), entry->getPos(), 1))&&
						zMisc::selectByPercent(1)) return false;
			}
			break;
		default:
			return false;
	}
	return true;
}

/**
 * \brief жƶǷڹΧ
 *
 *
 * \param entry ָ
 * \return ǷڹΧ
 */
bool SceneNpc::canReach(SceneEntryPk *entry)
{
	//ֻгжΧ
	switch (getPetType())
	{
		case Cmd::PET_TYPE_NOTPET:
		case Cmd::PET_TYPE_SEMI:
		case Cmd::PET_TYPE_TOTEM:
			{
				switch (this->npc->kind)
				{
					case NPC_TYPE_TRAP:
						break;
					default:
						{
							return true;
						}
						break;
				}
			}
			break;
		default:
			break;
	}
	if (!entry) return false;

	zPos c(0,0);
	int x=0, y=0;
	AIC->getActRegion(c, x, y);
	/*
	   if (getPetType()!=Cmd::PET_TYPE_NOTPET)
	   {
	   x += 10;
	   y += 10;
	   }
	   else
	   */
	{
		x = 7;//7
		y = 7;
	}

	//Χ޷߶
	if (!canMove() || isSurrounded())
	{
		c = getPos();
		x = 0;
		y = 0;
	}

	int atkRange = 0;
	BYTE atype=0, action=0;
	npc->getATypeAndAction(atype,action);
	switch(atype)
	{
		case NPC_ATYPE_NEAR:	/// 빥
		case NPC_ATYPE_MNEAR:	/// 빥
			atkRange = 1;
			break;
		case NPC_ATYPE_MFAR:	/// Զ빥
		case NPC_ATYPE_FAR:	/// Զ빥
			atkRange = 6;
			break;
		default:
			return false;
	}

	x += atkRange;
	y += atkRange;

	const zPos &pos = entry->getPos();
	return (pos.x>=c.x-x)
		&&(pos.x<=c.x+x)
		&&(pos.y>=c.y-y)
		&&(pos.y<=c.y+y);
}

/**
 * \brief жǷԹö
 *
 *
 * \param entry 
 * \return ǷԹ
 */
bool SceneNpc::canAttack(SceneEntryPk *entry)
{
	//NpcǷԹ
	if (!checkAttackTime(SceneTimeTick::currentTime))// || !zMisc::selectByPercent(50))
		return false;

	if (!attackAction) return false; //׷
	return true;
}

/**
 * \brief Npc
 * \param entry 
 * \return Ƿɹ
 */
bool SceneNpc::attackTarget(SceneEntryPk *entry)
{
	if (this->assault)
	{
		this->skillStatusM.clearRecoveryElement(121);
	}
	if (!canAttack(entry)) return false;

	//by=>friday սؾģʽ - NPCܹս
	if (entry && entry->getType() == zSceneEntry::SceneEntry_Player)
	{
		SceneUser* targetUser = (SceneUser*)entry;
		if (targetUser->zhanche_vehicle_mode)
		{
			// Zebra::logger->debug("[ս] NPC %s Թսؾģʽ %sֹ", this->name, targetUser->name);
			return false;
		}
	}

	if (entry->getTopMaster() &&
			entry->getTopMaster()->getType() == zSceneEntry::SceneEntry_Player &&
			this->getTopMaster() &&
			this->getTopMaster()->getType() == zSceneEntry::SceneEntry_Player)
	{
		if (!(this->isPkZone(entry) && entry->isPkZone(this))) // ¼&&this->isPkZone(pDef)
		{
			return false;
		}
	}

	BYTE atype = 0;
	BYTE action = 0;
	npc->getATypeAndAction(atype,action);

	//ɹû
	if (entry && this->npc->kind != NPC_TYPE_SURFACE) 
	{
		setDir(getPos().getDirect(entry->getPos()));
#ifdef _XWL_DEBUG
		//if (getPetType()!=Cmd::PET_TYPE_NOTPET)
		//	Zebra::logger->debug("%s ÷ %s", name, entry->name);
#endif
	}

	using namespace Cmd;
	stAttackMagicUserCmd att;
	att.dwUserTempID = tempid;
	if (entry) att.dwDefenceTempID = entry->tempid;

	switch(atype)
	{
		case NPC_ATYPE_NEAR:	/// 빥
			{
				if (npc->skill)
					att.wdMagicType = npc->skill;
				else
					att.wdMagicType = SKILLNORMAL;
				att.byAction = Ani_Attack;
			}
			break;
		case NPC_ATYPE_FAR:		/// Զ빥
			{
				if (npc->skill)
					att.wdMagicType = npc->skill;
				else
					att.wdMagicType = NORMALBOW;
				//att.wdMagicType = SKILLNORMAL;
				att.byAction = Ani_Attack;
				//att.byAction = Ani_Attack;
			}
			break;
		case NPC_ATYPE_MFAR:
		case NPC_ATYPE_MNEAR:
			{
				if (!this->skillAction) return false;
				att.wdMagicType = npc->skill;
				att.byAction = 0;
			}
			break;
		default:
			break;
	}
	//att.byAction = action;
	/*
	*/

	if (entry)
	{
		att.xDes = (WORD)entry->getPos().x;
		att.yDes = (WORD)entry->getPos().y;
		att.byDirect = getDir();
		switch (entry->getType())
		{
			case zSceneEntry::SceneEntry_Player:
				{
					att.byAttackType = ATTACKTYPE_N2U;
				}
				break;
			case zSceneEntry::SceneEntry_NPC:
				{
					att.byAttackType = ATTACKTYPE_N2N;
				}
				break;
			default:
				{
					att.byAttackType = ATTACKTYPE_N2U;
				}
				break;
		}

	}
	else
	{
		att.xDes = (WORD)getPos().x;
		att.yDes = (WORD)getPos().y;
		att.byDirect = getDir();
	}
	//scene->sendCmdToNine(getPosI(), &att, sizeof(att), false);

	this->skillValue.init();
	this->skillStatusM.processPassiveness();// ҵı״̬Ӱ

	//if (zMisc::selectByPercent(this->npc->rating))
	{
		//ScenePk::physicalMagicN2U(this, sceneUser);

		switch(atype)
		{
			case NPC_ATYPE_NEAR:	/// 빥
			case NPC_ATYPE_FAR:		/// Զ빥
				{
					if (entry)
					{
						if (this->checkMagicFlyRoute(entry,(atype==2||atype==3)?AttackFly:AttackNear))
						{
							if (entry->preAttackMe(this,&att))
							{
								scene->sendCmdToNine(getPosI(), &att, sizeof(att));
								entry->AttackMe(this,&att);
							}
						}
					}
				}
				break;
			case NPC_ATYPE_MFAR:
			case NPC_ATYPE_MNEAR:
				{
					//ħ
					if (entry)
					{
						if (this->checkMagicFlyRoute(entry,(atype==2||atype==3)?AttackFly:AttackNear))
						{
							if (entry->preAttackMe(this,&att,false))
							{
								scene->sendCmdToNine(getPosI(), &att, sizeof(att));
								entry->AttackMe(this,&att,false);
							}
						}
						/*
						   else
						   {
						   if (getPetType()==Cmd::PET_TYPE_NOTPET)
						   scene->sendCmdToNine(getPosI(), &att, sizeof(att), false);
						   }
						   */
					}

					/*
					   zSkill *s = this->usm.findSkill(att.wdMagicType);
					   if(s)
					   {
					   if (skillAction)
					   {
					   if (s->action(&att))
					   {
					   break;
					   }
					   }
					   }
					   if (entry)
					   {
					   entry->preAttackMe(this,NULL);
					   entry->AttackMe(this,NULL);
					   }
					   */
					//sceneUser->preAttackMe(this,NULL,false,npc->five);
					//sceneUser->AttackMe(this,NULL,false);
				}
				break;
			default:
				break;
		}

		if (entry)
		{
			if (this->giddy>0) //ŭѵ
			{
				if (zMisc::selectByPercent(giddy))
				{
					Cmd::stAttackMagicUserCmd cmd;

					if (entry->getType() == zSceneEntry::SceneEntry_Player)
					{
						cmd.dwDefenceTempID = entry->tempid;
						cmd.byAttackType = Cmd::ATTACKTYPE_N2U;
						cmd.byAction = Cmd::Ani_Null;
					}
					else
					{
						cmd.dwDefenceTempID = entry->tempid;
						cmd.byAttackType = Cmd::ATTACKTYPE_N2N;
						cmd.byAction = Cmd::Ani_Null;
					}

					cmd.dwUserTempID = this->tempid;
					cmd.wdMagicType = 386;
					cmd.byDirect = this->getDir();

					zSkill *s = NULL;

					s = zSkill::createTempSkill(this , 386 , 1);
					if(s)
					{
						s->action(&cmd, sizeof(cmd));
						SAFE_DELETE(s);
					}
				}
			}



			if (entry->getType() == zSceneEntry::SceneEntry_Player) ((SceneUser *)entry)->mask.on_defence();
		}
	}

	SceneEntryPk *pMaster = this->getTopMaster();
	if (pMaster&&pMaster->getType() == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = (SceneUser *)pMaster;
		pUser->packs.equip.costAttackDurByPet(pUser);
	}

	//һιʱ
	setAttackTime(SceneTimeTick::currentTime);
	if (nextMoveTime>SceneTimeTick::currentTime)
		delayMoveTime(720);
	else
		setMoveTime(SceneTimeTick::currentTime, 720);
	setEndBattleTime(SceneTimeTick::currentTime, 10*1000);

	return true;
}

#include "RebuildObject.h"

bool is_boss(int type)
{
	switch (type)
	{
		case NPC_TYPE_BACKBONE:
		case NPC_TYPE_BBOSS:
		case NPC_TYPE_LBOSS:
		case NPC_TYPE_GOLD:
		case NPC_TYPE_PBOSS:
		case NPC_TYPE_REDBOSS:
		case NPC_TYPE_DUCKHIT:
		case NPC_TYPE_RESOURCE:
			return true;
			break;	
		default:
			break;
	}
	return false;
}

int drop_odds(WORD player_level, WORD npc_level, DWORD npc_kind)
{
	int odds = 100;

	if (is_boss(npc_kind))
	{
		if (player_level>npc_level+170)
		{
			DWORD sub = (player_level-(npc_level+170))*10;
			odds = (sub>=100)?0:(100-sub);
		}
	}
	else
	{
		int diff = player_level - npc_level;
		if(diff >= 6 && diff <= 100)
		{
			odds = 50;
		}
		else if(diff > 100 && diff <= 120)
		{
			odds = 40;
		}
		else if(diff > 120 && diff <= 140)
		{
			odds = 30;
		}
		else if(diff > 140 && diff <= 160)
		{
			odds = 70;
		}
		else if(diff > 160 && diff <= 170)
		{
			odds = 10;
		}
		else if(diff > 170)
		{
			odds = 0;
		}
	}
	if (odds>100) odds=100;
	if (npc_kind==NPC_TYPE_DUCKHIT || npc_kind==NPC_TYPE_RESOURCE) odds = 100;
#ifdef _XWL_DEBUG
	Zebra::logger->debug(" %u", 100-odds);
#endif
	return odds;
}

/*
int drop_odds(WORD player_level, WORD npc_level, DWORD npc_kind)
{
	int odds = 100;

	if (is_boss(npc_kind))
	{
		if (player_level>npc_level+20)
		{
			DWORD sub = (player_level-(npc_level+20))*10;
			odds = (sub>=100)?0:(100-sub);
		}
	}
	else
	{
		int diff = player_level - npc_level;
		if(diff >= 6 && diff < 10)
		{
			odds = 50;
		}
		else if(diff >= 10)
		{
			odds = 0;
		}
	}
	if (odds>100) odds=100;
	if (npc_kind==NPC_TYPE_DUCKHIT || npc_kind==NPC_TYPE_RESOURCE) odds = 100;
#ifdef _XWL_DEBUG
	Zebra::logger->debug(" %u", 100-odds);
#endif
	return odds;
}*/

/**
 * \brief NpcĶ
 * \return ִǷɹ
 */
bool SceneNpc::deathAction()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
	if (canLostObject(SceneTimeTick::currentTime))
	{
		//Zebra::logger->debug("%s ", name);
		uint64_t value=1; //by=>friday
		uint64_t value1=0; //by=>friday
		uint64_t value2=0; //by=>friday
		uint64_t player_level = 0; //by=>friday
		uint64_t vcharm = 0; //by=>friday
		uint64_t vlucky = 0; //by=>friday
		SceneUser *pUser = SceneUserManager::getMe().getUserByID(dwNpcLockedUser);
		if (pUser)
		{
			if(pUser->issetUState(Cmd::USTATE_BOSSGUISHU))
			{
				pUser->showCurrentEffect(Cmd::USTATE_BOSSGUISHU, false ); 
			}	
			DWORD leader_id = pUser->team.getLeader();
			SceneUser* leader = SceneUserManager::getMe().getUserByTempID(leader_id);
			if (leader) {
				Team& team = const_cast<Team&>(leader->team.getTeam());

				team.rwlock.rdlock();
				std::vector<TeamMember>::iterator it = team.member.begin();
				for(; it!=team.member.end(); ++it) {
					SceneUser* member = SceneUserManager::getMe().getUserByTempID(it->tempid);
					if (member && (member->charbase.level < 20 || //soke ȼƷ
								scene->checkTwoPosIInNine(pUser->getPosI() , member->getPosI()) ||
								scene->checkTwoPosIInNine(this->getPosI() , member->getPosI()) )) {
						//moved from attackDeathNpc by lqy, avoid unfair bonus
						OnKill event(id) ;
						EventTable::instance().execute(*member, event);
						OnKillByLevel e(npc->level);
						EventTable::instance().execute(*member, e);
						if (dwNpcLockedUser == member->id)
						{
							OnKillBySelf es(id);
							EventTable::instance().execute(*member, es);
						}

						if (ScriptQuest::get_instance().has(ScriptQuest::NPC_KILL, npc->id)) { 
							char func_name[32];
							sprintf(func_name, "%s_%d", "kill", npc->id);
							execute_script_event(member, func_name, this);
						}
					}
				}
				team.rwlock.unlock();
			}else {
				OnKill event(id) ;
				EventTable::instance().execute(*pUser, event);
				OnKillByLevel e(npc->level);
				EventTable::instance().execute(*pUser, e);
				OnKillBySelf es(id);
				EventTable::instance().execute(*pUser, es);

				if (ScriptQuest::get_instance().has(ScriptQuest::NPC_KILL, npc->id)) { 
					char func_name[32];
					sprintf(func_name, "%s_%d", "kill", npc->id);
					execute_script_event(pUser,func_name, this);
				}
			}

			value1=pUser->packs.equip.getEquips().get_mf();
			value2=pUser->packs.equip.getEquips().get_incgold();
			vcharm = pUser->charstate.charm;
			vlucky = pUser->charstate.lucky;
			player_level = pUser->charbase.level;
		}

		NpcLostObject nlo;
		if (npc->kind == NPC_TYPE_BBOSS && zMisc::selectByPercent(50))
		{
			npc->nco.lostGreen(nlo, value, value1, value2, vcharm, vlucky);
		}
		else if (zMisc::selectByOneHM((unsigned int)100)) //by=>friday
		{
			npc->nco.lostAll(nlo);
		}
		else
		{
			npc->nco.lost(nlo, value, value1, value2, vcharm, vlucky, player_level, ScenesService::getInstance().double_drop, ScenesService::getInstance().levelDoubleDrop);
		}

		for(NpcLostObject::const_iterator it = nlo.begin(); it != nlo.end(); it++)
		{
		//soke ƷĹID
			if (!((npc->id>=21501 && npc->id<=21511) ||
				  (npc->id>=26501 && npc->id<=26511) ||
				  (npc->id>=30051 && npc->id<=30102)))
			{
				if (!zMisc::selectByPercent(drop_odds(player_level, npc->level, npc->kind))) continue;
			}

			zObjectB *ob = objectbm.get((*it).id);
			if (ob)
			{
				uint64_t num = (*it).minnum; //by=>friday
				if ((*it).minnum != (*it).maxnum)
					num = zMisc::randBetween((int)(*it).minnum, (int)(*it).maxnum); //by=>friday
				if (num > 0)
				{
					if(ob->id == 665)
					{
						//
						distributeMoney(num);
					}
					else
					{
						SceneUser *pUser = SceneUserManager::getMe().getUserByID(dwNpcLockedUser);
						if(pUser && pUser->team.getLeader()&& this->npc->kind != NPC_TYPE_RESOURCE)
						{
						    SceneUser *leader = SceneUserManager::getMe().getUserByTempID(pUser->team.getLeader());
							//бBOSSǷƷʱ
							//ɫbossӹ
							// if (pUser && this->npc->kind == NPC_TYPE_REDBOSS)
							// {
                            //     if(leader && leader->team.isSpecialObj())
						    // 	{
							//         scene->addObject(ob, num, getPos(), leader->team.getNextObjOwnerID(), id ,false);
							//     }
							//     else 
							//     {
							// 	    scene->addObject(ob, num, getPos(), dwNpcLockedUser, id , false);
							//     }
							// }
							// else
							// {
								if(leader && leader->team.isSpecialObj())
						    	{
									// if (this->npc->id == 144001)
							    	// {	
										scene->addObject2(ob, num, getPos(), leader->team.getNextObjOwnerID(), id ,true);
										// }
										// else 
										// {
										// 	scene->addObject(ob, num, getPos(), leader->team.getNextObjOwnerID(), id ,true);
										// }
							        
							    }
							    else 
							    {
									// if (this->npc->id == 144001)
							    	// {	
										scene->addObject2(ob, num, getPos(), dwNpcLockedUser, id , true);
									// }
								    // else
									// {
									// 	scene->addObject(ob, num, getPos(), dwNpcLockedUser, id , true);
									// }
							    }
							// }
							
						}
						else
						{
							if (this->npc->kind == NPC_TYPE_RESOURCE && pUser)
							{
								zObject *o = zObject::create(ob,num);
								if (o)
								{
									DWORD addnum=o->data.dwNum;
									Combination callback(pUser, o);
									pUser->packs.main.execEvery(callback);
									if (pUser->packs.equip.pack(EquipPack::L_PACK) && pUser->packs.equip.pack(EquipPack::L_PACK)->can_input()) pUser->packs.equip.pack(EquipPack::L_PACK)->execEvery(callback);
									if (pUser->packs.equip.pack(EquipPack::R_PACK) && pUser->packs.equip.pack(EquipPack::R_PACK)->can_input()) pUser->packs.equip.pack(EquipPack::R_PACK)->execEvery(callback);

									bool added = false;
									if (o->data.dwNum)
									{
										if (pUser->packs.uom.space(pUser) >= 1 && pUser->packs.addObject(o, true, AUTO_PACK))
										{
				                            //soke ضҪ
                                            o->checkXinBind();
											added = true;
											Cmd::stAddObjectPropertyUserCmd status;
											status.byActionType = Cmd::EQUIPACTION_OBTAIN;
											bcopy(&o->data, &status.object, sizeof(t_Object));
											pUser->sendCmdToMe(&status, sizeof(status));
										}
										else
										{
											addnum -= o->data.dwNum;
											Channel::sendSys(pUser , Cmd::INFO_TYPE_FAIL, "İ");
											scene->addObject(ob, o->data.dwNum, getPos(), dwNpcLockedUser, id);



											if (callback.num() || added)
											{
												zObject::logger(o->createid,o->data.qwThisID,o->data.strName,addnum,addnum,1,this->scene->id,pUser->scene->name,pUser->id,pUser->name,"ɼõ",o->base,o->data.kind,o->data.upgrade);
												OnGet event(o->data.dwObjectID);
												EventTable::instance().execute(*pUser, event);
												if (ScriptQuest::get_instance().has(ScriptQuest::OBJ_GET, o->data.dwObjectID))
												{ 
													char func_name[32];
													sprintf(func_name, "%s_%d", "get", o->data.dwObjectID);
													execute_script_event(pUser,func_name, o);
												}								
											}
										}
									}
									if (!o->data.dwNum || !added)
									{
										zObject::destroy(o);
									}
								}
							}
							else
							{
								//ɫbossӹ
								// //бBOSSǷƷʱ
								//bossҹ
								// if (pUser && this->npc->id == 144001)
							    // {
								    scene->addObject2(ob, num, getPos(), dwNpcLockedUser, id , true);
							    // }
								// else if (pUser && this->npc->kind == NPC_TYPE_REDBOSS)
							    // {
								//     scene->addObject(ob, num, getPos(), dwNpcLockedUser, id , false);
							    // }
							    // else 
							    // {
								//     scene->addObject(ob, num, getPos(), dwNpcLockedUser, id , true);
							    // }
							}
						}
					}
				}
			}
		}
		//ǵ
		if (zMisc::/*selectByPercent*//*Ҫĳ֮*/selectByTenTh((unsigned int)npc->soulrate) && zMisc::selectByPercent(drop_odds(player_level, npc->level, npc->kind)) ) { //by=>friday

			//zObjectB *base = objectbm.get(SoulStone::id());
			zObjectB *base = objectbm.get(SoulStone::id(npc->trait));
			if (!base) return false;

			zObject *ob = zObject::create(base, 1);
			if (ob) {
				SoulStone::assign(ob, npc->trait);
				scene->addObject(ob, getPos(), false , dwNpcLockedUser);
				zObject::logger(ob->createid,ob->data.qwThisID,ob->data.strName,ob->data.dwNum,ob->data.dwNum,1,0,NULL,scene->id,scene->name,"npc_lost",ob->base,ob->data.kind,ob->data.upgrade);
			}
		}

		//
		if (canRush())
			createRush();

	}
	else if (canRelive(SceneTimeTick::currentTime))
	{
		zPos pos;

		if (getSceneNpcType() == SceneNpc::GANG)
		{
			if (!isRushNpc)
				setClearState();
			//Zebra::logger->debug("Ƕ̬npc %s", name);
			//̬NpcҪԴ
			//Zebra::logger->debug("TODO ̬NpcҪɾ");
			/*TODO ̬NpcҪɾ
			  scene->removeNpc(sceneNpc, false);
			  SceneNpcManager::getMe().removeSceneNpc(sceneNpc);
			  SAFE_DELETE(sceneNpc);
			// */
			//СģڴѾͷˣ֮ʹ
		}
		else if (scene->randPosByRegion(define->region.index, pos))
		{
			//
			isUse = false; // ʬʹ״̬ϴ
			clearUState(Cmd::USTATE_DEATH);
			Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
			removeNpc.dwMapNpcDataPosition = tempid;
			scene->sendCmdToNine(getPosI(), &removeNpc, sizeof(removeNpc));

			hp = this->getMaxHP();
			recoverUnder30 = false;
			//ͷʼ·ƶ
			if (AIC) AIC->on_relive();

			if (define->initstate == zSceneEntry::SceneEntry_Normal)
			{
				//ҷ赲ɹ
				scene->setBlock(pos);
				setState(zSceneEntry::SceneEntry_Normal);
			}
			else
				setState(zSceneEntry::SceneEntry_Hide);

			if (scene->refresh(this, pos))
			{
				if (getState() == zSceneEntry::SceneEntry_Normal)
				{
					this->sendMeToNine();
				}
				//ٻnpcĳ
				if (!define->petList.empty())
				{
					for (std::map<DWORD, std::pair<DWORD, DWORD> >::const_iterator it=define->petList.begin(); it!=define->petList.end(); it++)
					{
						for (DWORD i=0; i<it->second.first; i++)
						{
							if (zMisc::selectByPercent(it->second.second))
							{
								ScenePet * pet = summonPet(it->first, Cmd::PET_TYPE_TOTEM, 0, 0, 0, 0);
								if (pet)
									pet->setPetAI(Cmd::PETAI_ATK_ACTIVE);
							}
						}
					}
				}
				//
				if (!define->dieList.empty())
				{
					for (std::list< std::pair<DWORD,zPos> >::const_iterator it=define->dieList.begin(); it!=define->dieList.end(); it++)
					{
						ScenePet * pet = summonPet(it->first, Cmd::PET_TYPE_SEMI, 0, 0, 0, 0, it->second);
						if (pet)
						{
							pet->setPetAI(Cmd::PETAI_ATK_ACTIVE);
							Zebra::logger->debug("%s ٻ %s", name, pet->name);
						}
					}
				}

				if (npc->kind==2 || npc->kind==4 || npc->kind==5 || npc->kind==31) //޸BOSSѾˢʾ
				{
					Zebra::logger->debug("[BOSS]%s(%u)  pos=(%u,%u)", name, tempid, pos.x, pos.y);
				  //soke BOSS  2 3 31
		//		   Channel::sendAllInfo(Cmd::INFO_TYPE_XTYSCROLL, "%s Ѿ %s ", name, scene->getRealName());
				   Channel::sendAllInfo(Cmd::INFO_TYPE_XTYSCROLL, "%s Ѿ %s (%u,%u) ", name, scene->getRealName(), pos.x, pos.y);
				   Channel::sendAllInfo(Cmd::INFO_TYPE_XG2, "%s Ѿ %s (%u,%u) ", name, scene->getRealName(), pos.x, pos.y);	
				   //֪ͨSession ıBOSSĸ
					Cmd::Session::t_BossDieInfo_SceneSession BossSend;
					BossSend.NpcID = npc->id;
					BossSend.CountryID = scene->getCountryID();
					bcopy(scene->getRealName(),BossSend.MapName, MAX_NAMESIZE );
					BossSend.KillState = 0; //Ĵ״̬
					BossSend.ReviveTime = 0; //ĸʱ
					sessionClient->sendCmd(&BossSend, sizeof(BossSend)); 
				  }								   
				   
				}
				/*
				if (npc->kind==31) //޸BOSSѾˢʾ--
				{

					Zebra::logger->debug("[BOSS]%s(%u)  pos=(%u,%u)", name, tempid, pos.x, pos.y);
				   //soke BOSS  2 3 31 33 34 35
				   Channel::sendAllInfo(Cmd::INFO_TYPE_XTYSCROLL, "%s Ѿ %s (%u,%u) ", name, scene->getRealName(), pos.x, pos.y);
				   Channel::sendAllInfo(Cmd::INFO_TYPE_XG2, "%s Ѿ %s (%u,%u) ", name, scene->getRealName(), pos.x, pos.y);

				  	//֪ͨSession ıBOSSĸ
					Cmd::Session::t_BossDieInfo_SceneSession BossSend;
					BossSend.NpcID = npc->id;
					BossSend.CountryID = scene->getCountryID();
					bcopy(scene->getRealName(),BossSend.MapName, MAX_NAMESIZE );
					BossSend.KillState = 0; //Ĵ״̬
					BossSend.ReviveTime = 0; //ĸʱ
					sessionClient->sendCmd(&BossSend, sizeof(BossSend)); 
				}	
				*/				
			}
			else
			{
				setState(zSceneEntry::SceneEntry_Death);
				Zebra::logger->debug("[BOSS]%s(%u) ʧ pos=(%u,%u)", name, tempid, pos.x, pos.y);
			}

			if (this->id == COUNTRY_MAIN_FLAG)
			{
				CountryDareM::getMe().reliveSecondFlag(this->scene, COUNTRY_SEC_FLAG);
			}

			if (this->id == COUNTRY_KING_MAIN_FLAG)
			{
				CountryDareM::getMe().reliveSecondFlag(this->scene, COUNTRY_KING_SEC_FLAG);
			}


		}
		return true;
	}



/*
 * \brief npc
 *
 * \showDelay صʱ
 */
void SceneNpc::hideMe(int showDelay)
{
    Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
    removeNpc.dwMapNpcDataPosition = tempid;
    scene->sendCmdToNine(getPosI(), &removeNpc, sizeof(removeNpc));

    setState(zSceneEntry::SceneEntry_Hide);
    showTime = SceneTimeTick::currentTime;
    showTime.addDelay(showDelay);
#ifdef _XWL_DEBUG
    Zebra::logger->debug("npc %s %d", npc->name, showDelay);
#endif
}

/*
 * \brief npc״̬Ϊ
 *
 * \return Ƿִгɹ
 */
bool SceneNpc::hideAction()
{
	My_FunctionTime_wrapper(__PRETTY_FUNCTION__);
    if (showTime<=SceneTimeTick::currentTime)
	{
		setState(zSceneEntry::SceneEntry_Normal);
		scene->setBlock(getPos());
		sendMeToNine();
#ifdef _XWL_DEBUG
		Zebra::logger->debug("npc %s ", name);
#endif
	}
    return true;
}

/**
 * \brief Ѱ·жмǷɴĿĵ
 * \param tempPos Ѱ·̵м
 * \param destPos Ŀĵ
 * \param radius Ѱ·ΧΧΪĿĵزɴ
 * \return ǷɵĿĵ
 */
bool SceneNpc::moveable(const zPos &tempPos, const zPos &destPos, const int radius)
{
	return (scene->zPosShortRange(tempPos, destPos, radius)
			&& (!scene->checkBlock(tempPos) //ĿɴĿ
				|| tempPos == destPos));
}

/**
 * \brief Npcĳһƶ
 * \param direct 
 * \param step ƶĲ
 * \return ƶǷɹ
 */
bool SceneNpc::move(const int direct, const int step)
{
	if (!canMove()) return false;
	//if ((aif&AIF_NO_MOVE)||(petAI&Cmd::PETAI_MOVE_STAND)) return false;

	const int walk_adjust[9][2]= { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, 0} };

	if(backOffing)
	{
		return true;
	}
	//Zebra::logger->debug("ƶ%u", step);
	lastPos2 = lastPos1;
	lastPos1 = pos;
	zPosI oldPosI = getPosI();
	zPos oldPos = getPos(),  newPos = getPos();
	newPos.x += (step*walk_adjust[direct][0]);
	newPos.y += (step*walk_adjust[direct][1]);
	//Ŀ赲ԼǷϷ
	if (scene->checkBlock(newPos))
	{
		//Zebra::logger->debug("%u, %u, %s, %u, %u, %u", id, tempid, name, direct, getPos().x, getPos().y);
		return false;
	}
	if (scene->refresh(this, newPos))
	{
		if (getPetType()!=Cmd::PET_TYPE_CARTOON && getPetType()!=Cmd::PET_TYPE_RIDE)
			scene->setBlock(newPos);
		scene->clearBlock(oldPos);
		setDir(direct);

		Cmd::stNpcMoveMoveUserCmd cmd;
		cmd.dwNpcTempID = tempid;
		cmd.byDirect = direct;
		cmd.bySpeed = step;
		if (Cmd::PET_TYPE_RIDE==getPetType()&&speedRate>1.5)
		{
			cmd.bySpeed = 3;
#ifdef _XWL_DEBUG
			//Zebra::logger->debug("%s ƶ 2", name);
#endif
		}
		cmd.x = newPos.x;
		cmd.y = newPos.y;
		scene->sendCmdToNine(oldPosI, &cmd, sizeof(cmd));

		if (oldPosI != getPosI())
		{
			Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
			removeNpc.dwMapNpcDataPosition = tempid;
			scene->sendCmdToReverseDirect(oldPosI,
					scene->getScreenDirect(oldPosI,getPosI()), &removeNpc, sizeof(removeNpc));
			BUFFER_CMD(Cmd::stAddMapNpcMapScreenStateUserCmd ,send , zSocket::MAX_USERDATASIZE);
			this->full_t_MapNpcDataState(send->data);
			scene->sendCmdToDirect(getPosI(), scene->getScreenDirect(oldPosI,getPosI()), send,send->size());
			Cmd::stRTMagicPosUserCmd ret;
			full_stRTMagicPosUserCmd(ret);
			scene->sendCmdToDirect(getPosI(), scene->getScreenDirect(oldPosI,getPosI()), &ret, sizeof(ret));
			attackRTHpAndMp();
		}
	}

	//һƶʱ
	setMoveTime(SceneTimeTick::currentTime);
	//´ιʱɵӰ죺ͨƶʱ䣻 ƶʱһ
	if (getPetType()==Cmd::PET_TYPE_NOTPET || getPetType()==Cmd::PET_TYPE_SEMI)
	{
		if (nextAttackTime>SceneTimeTick::currentTime)
			nextAttackTime.addDelay((int)(npc->distance/speedRate));
		else
			setAttackTime(SceneTimeTick::currentTime);
	}
	else
	{
		if (nextAttackTime>SceneTimeTick::currentTime)
			nextAttackTime.addDelay((int)(npc->distance/speedRate/2));
		else
			setAttackTime(SceneTimeTick::currentTime, (int)(npc->distance/speedRate/2));
	}

	return true;
}

/**
 * \brief ˲ƶ
 *
 *
 * \param newPos Ŀλ
 * \return Ƿɹ
 */
bool SceneNpc::warp(const zPos &newPos,bool ignore)
{
    if (!ignore)
	if (!canMove()) return false;

    zPosI oldPosI = getPosI();
    zPos oldPos = getPos();

    zPos findedPos;
    bool founded = scene->findPosForUser(newPos, findedPos);
    if(scene->refresh(this,founded ? findedPos : newPos))
	{
		if (SceneEntry_Normal==getState() && getPetType()!=Cmd::PET_TYPE_CARTOON && getPetType()!=Cmd::PET_TYPE_RIDE)
			scene->setBlock(newPos);
		scene->clearBlock(oldPos);


		Cmd::stRTMagicPosUserCmd ret;
		full_stRTMagicPosUserCmd(ret);
		if (oldPosI != getPosI())
		{
			Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
			removeNpc.dwMapNpcDataPosition = tempid;
			Cmd::stAddMapNpcMapScreenUserCmd addNpc;
			full_t_MapNpcData(addNpc.data);
			if(scene->checkTwoPosIInNine(oldPosI,getPosI()))
			{
				scene->sendCmdToReverseDirect(oldPosI,scene->getScreenDirect(oldPosI,getPosI()),&removeNpc,sizeof(removeNpc));
				scene->sendCmdToDirect(getPosI(), scene->getScreenDirect(oldPosI,getPosI()), &addNpc, sizeof(addNpc));
				//scene->sendCmdToDirect(getPosI(), scene->getScreenDirect(oldPosI,getPosI()), &ret, sizeof(ret));
			}
			else
			{
				scene->sendCmdToNine(oldPosI,&removeNpc, sizeof(removeNpc));
				sendMeToNine();
			}
		}
		scene->sendCmdToNine(getPosI(), &ret, sizeof(ret));
		//setMoveTime(SceneTimeTick::currentTime);
		return true;
	}
    else return false;
}

/**
 * \brief ʹĳһƶ
 * Ѱ·㷨ƶֻǷװһ£ڲͬnpcͬòͬA*㷨
 * \param srcPos 
 * \param destPos Ŀĵ
 * \return ƶǷɹ
 */
bool SceneNpc::gotoFindPath(const zPos &srcPos, const zPos &destPos)
{
    if(id==27 || id==28 || id==29)
	//һԲΪ2
	return zAStar<2>::gotoFindPath(srcPos, destPos);
    else
	//ͨһֻһԲΪ1
	return zAStar<>::gotoFindPath(srcPos, destPos);
}

/**
 * \brief Npcĳһƶ
 * Ѱ·㷨ƶֻǷװһ£ڲͬnpcͬòͬA*㷨
 * \param pos Ŀĵ
 * \return ƶǷɹ
 */
bool SceneNpc::goTo(const zPos &pos)
{
    if(id==27 || id==28 || id==29)
	//һԲΪ2
	return zAStar<2>::goTo(getPos(), pos);
    else
	//ͨһֻһԲΪ1
	return zAStar<>::goTo(getPos(), pos);
}

/**
 * \brief Npcĳһƶ
 * ֻǷװһ£ڲͬnpcͬòͬA*㷨
 * \param direct 
 * \return ƶǷɹ
 */
bool SceneNpc::shiftMove(const int direct)
{
    if(id==27 || id==28 || id==29)
	//һԲΪ2
	return zAStar<2>::shiftMove(direct);
    else
	//ͨһֻһԲΪ1
	return zAStar<>::shiftMove(direct);
}

void SceneNpc::clearStateToNine(WORD state)
{
	if(!scene) return;
	Cmd::stClearStateMapScreenUserCmd send;
	send.type=Cmd::MAPDATATYPE_NPC;
	send.dwTempID = this->tempid;
	send.wdState =state;
	if (npc->kind == NPC_TYPE_TRAP)
	{
		SceneEntryPk *entry = getMaster();
		if (entry && entry->getType() == zSceneEntry::SceneEntry_Player)
		{
			((SceneUser*)entry)->sendCmdToMe(&send, sizeof(send));
		}		
	}
	else
	{
		this->scene->sendCmdToNine(getPosI(),&send,sizeof(send));
	}
}
/**
 * \brief ĳ״̬û
 *
 * \author zjw
 */

void SceneNpc::setStateToNine(WORD state)
{
	Cmd::stSetStateMapScreenUserCmd send;
	send.type=Cmd::MAPDATATYPE_NPC;
	send.dwTempID = this->tempid;
	send.wdState =state;
	if (npc->kind == NPC_TYPE_TRAP)
	{
		SceneEntryPk *entry = getMaster();
		if (entry && entry->getType() == zSceneEntry::SceneEntry_Player)
		{
			((SceneUser*)entry)->sendCmdToMe(&send, sizeof(send));
		}		
	}
	else
	{
		this->scene->sendCmdToNine(getPosI(),&send,sizeof(send));
	}
}
/**
 * \brief ֪ͨͻʾ״̬Ч
 * \param state ״̬Ч
 * \param isShow ֻʧ
 */
void SceneNpc::showCurrentEffect(const WORD &state, bool isShow,bool notify)
{
	if (isShow)
	{
		if(this->setUState(state) && notify)
			this->setStateToNine(state);
	}
	else
	{
		if(this->clearUState(state) && notify)
			this->clearStateToNine(state);
	}
}

/**
 * \brief ɫN
 * \param dwAttTempID ߵʱID
 * \param grids
 * \author fqnewman
 */
void SceneNpc::standBack(const DWORD dwAttTempID, DWORD grids)
{
	SceneUser *att = SceneUserManager::getMe().getUserByTempID(dwAttTempID);
	if (att)
	{
		backoff(Scene::getCompDir(att->getPos(), this->pos), grids);
	}
	else
	{
		backoff(getDir(), grids);
	}
}

/**
 * \brief Ŀ껻dwTempIDָĽɫ
 * \param dwTempID ĿɫʱID
 * \author fqnewman
 */
void SceneNpc::changeAttackTarget(const DWORD &dwTempID)
{
	SceneUser *targetUser = SceneUserManager::getMe().getUserByTempID(dwTempID);
	if (targetUser) 
	{
		if (canChaseTarget(targetUser))
		{
			unChaseUser();
			chaseSceneEntry(targetUser->getType(), targetUser->tempid);
		}
	}
}

/**
 * \brief ýɫ
 * \author fqnewman
 */
void SceneNpc::toDie(const DWORD &dwTempID)
{
	if (0==dwTempID)
		ScenePk::attackDeathNpc(0, this);
	else
	{
		SceneUser *att = SceneUserManager::getMe().getUserByTempID(dwTempID);
		if (att)
		{
			hp = 0;
			processDeath(att);
			//ScenePk::attackDeathNpc(att, this);
		}
	}
}

/**
 * \brief жϽɫǷ
 * \author fqnewman
 * \return trueΪ
 */
bool SceneNpc::isDie()
{
    if (this->getState() == SceneEntry_Death) return true;
    return false;
}


/**
 * \brief õnpcĵȼ
 *
 *
 * \return ȼ
 */
DWORD SceneNpc::getLevel() const
{
    return npc->level;
}

/**
 * \brief Ҫְҵͣʹõļ
 * \author fqnewman
 */
bool SceneNpc::needType(const DWORD &needtype)
{
    return true;
}

/**
 * \brief Ҫְҵͣʹõļ
 * \author fqnewman
 */
bool SceneNpc::addSkillToMe(zSkill *skill)
{
    return usm.addSkill(skill);
}

/**
 * \brief ǷиüҪ
 * \author fqnewman
 * \return true  false û
 */
bool SceneNpc::needWeapon(DWORD skillid)
{
    return true;
}

/**
 * \brief ǷPk
 * \param other PK
 * \author fqnewman
 * \return true  false 
 */
bool SceneNpc::isPkZone(SceneEntryPk *other)
{
    return true;
}

/**
 * \brief Ʒͷ
 * \param object Ʒ
 * \param num Ʒ
 * \author fqnewman
 * \return true ĳɹ false ʧ
 */
bool SceneNpc::reduce(const DWORD &object, const BYTE num)
{
    return true;
}

/**
 * \brief ƷǷ㹻
 * \param object Ʒ
 * \param num Ʒ
 * \author fqnewman
 * \return true 㹻 false 
 */
bool SceneNpc::checkReduce(const DWORD &object, const BYTE num)
{
    return true;
}

/**
 * \brief ʩżµMP,HP,SP
 * \param base ܻԶ
 * \author fqnewman
 * \return true ĳɹ false ʧ
 */
bool SceneNpc::doSkillCost(const zSkillB *base)
{
    return true;
}

/**
 * \brief ʩżµMP,HP,SPǷ㹻
 * \param base ܻԶ
 * \author fqnewman
 * \return true ĳɹ false ʧ
 */
bool SceneNpc::checkSkillCost(const zSkillB *base)
{
    return true;
}

/**
 * \brief ʩųɹʣμǷʩ
 * \author fqnewman
 * \return true ɹ false ʧ
 */
bool SceneNpc::checkPercent()
{
    return true;
}

/**
 * \brief ıɫhp
 * \param hp HP
 * \author fqnewman
 */
//soke ͻ ˺Խ磨ֵѪ
uint64_t SceneNpc::directDamage(SceneEntryPk *pAtt, const uint64_t &dam, bool notify) //by=>friday ޸Ϊ֧64λ޷˺
{

	SceneEntryPk::directDamage(pAtt, dam, notify);

	uint64_t reduceHP=0; //by=>friday ޸Ͳƥ䣬Ϊ޷
	if (hp >= dam) //by=>friday ޸޷
	{
		hp -= dam;
		reduceHP = dam;
	}
	else
	{
		reduceHP = hp;
		hp=0;
	}

	if (reduceHP !=0 && notify)
	{
		Cmd::stObjectHpMpPopUserCmd ret;
		ret.dwUserTempID = this->tempid;
		ret.byTarget = Cmd::MAPDATATYPE_NPC;
		ret.vChange = 0-(int)reduceHP;
		ret.type = Cmd::POP_HP;
		this->scene->sendCmdToNine(getPosI(), &ret , sizeof(ret));
	}


	SceneEntryPk *pEntry = pAtt->getMaster();
	if (pEntry&&(pEntry->getType() == zSceneEntry::SceneEntry_Player))
	{
		SceneUser * master = (SceneUser *)pEntry;
		if (master)
		{
			this->reduceHP(master , reduceHP);
			this->setLockUser(master->id);
		}
	}
	return reduceHP;
}

/**
 * \brief npc
 *
 *
 * \param pAtt 
 * \return Ƿ
 */
bool SceneNpc::processDeath(SceneEntryPk *pAtt)
{
	SceneEntryPk *mymaster=NULL;
	if (hp ==0 && getState()!=zSceneEntry::SceneEntry_Death)
	{
		SceneEntryPk *temp = NULL;
		DWORD attCountryID = 0;
		DWORD attUnionID   = 0;
		DWORD dwAttUserID = 0;
		bool isAntiAtt = false;

		// ٻ
		mymaster = this->getTopMaster();
		if (mymaster->summon != this) mymaster = NULL;
		switch(pAtt->getType())
		{
			case zSceneEntry::SceneEntry_NPC:
				{
					temp = pAtt->getTopMaster();
					if (!temp) temp = pAtt;

					if (((SceneUser*)temp)->getType() == zSceneEntry::SceneEntry_Player)
					{
						attUnionID   = ((SceneUser*)temp)->charbase.unionid;
						attCountryID = ((SceneUser*)temp)->charbase.country;
						dwAttUserID = ((SceneUser*)temp)->id;
						isAntiAtt = ((SceneUser*)temp)->isWarRecord(Cmd::COUNTRY_FORMAL_ANTI_DARE, 
								this->scene->getCountryID());
					}
				}
				break;
			case zSceneEntry::SceneEntry_Player:
				{
					temp = pAtt;
					attCountryID = ((SceneUser*)temp)->charbase.country;
					attUnionID = ((SceneUser*)temp)->charbase.unionid;
					dwAttUserID = ((SceneUser*)temp)->id;

					isAntiAtt = ((SceneUser*)temp)->isWarRecord(Cmd::COUNTRY_FORMAL_ANTI_DARE, 
								this->scene->getCountryID());

					switch ( npc->kind)
					{
						case NPC_TYPE_TRADE:
						case NPC_TYPE_TASK:
						case NPC_TYPE_LIVENPC:
						case NPC_TYPE_MAILBOX:
						case NPC_TYPE_AUCTION:
							if (attCountryID!=scene->getCountryID())
							{
					//			Zebra::logger->debug("----------عNPCɱҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "%sNPC %s(%u,%u)  %s(%s) ɱ", scene->getRealName(), name, pos.x, pos.y, temp->name, SceneManager::getInstance().getCountryNameByCountryID(attCountryID));
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
						default:
							break;
					}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				    //soke ӹɱNPCҹ ʼ
		          /*switch(npc->kind)
			        {
				      //case NPC_TYPE_NORMAL:              /// ͨ   1    ɫͨͲϹ
				      //case NPC_TYPE_BBOSS:              /// Boss  2   
				      //case NPC_TYPE_LBOSS:              /// СBoss  3          Ҳ
				      //case NPC_TYPE_PBOSS:              /// Boss  31  
				      //case NPC_TYPE_BACKBONE:		      /// Ӣ  4   
				      //case NPC_TYPE_GOLD:		          /// ƽ  5  
				      //case NPC_TYPE_SUMMONS:		      /// ٻ  11   ٻҲ
				      //case NPC_TYPE_AGGRANDIZEMENT:	  /// ǿ  13  ȡ
				      //case NPC_TYPE_ABERRANCE:		  ///   14  ȡ
				      //case NPC_TYPE_BACKBONEBUG:		  ///   10  ȡ
				      //case NPC_TYPE_PET:	              ///   9
				      //case NPC_TYPE_TOTEM:			  /// ͼ  12  ֮
				      //case NPC_TYPE_HUMAN:	          ///        0
				      //case NPC_TYPE_WILDHORSE:          /// Ұ  18
				      //case NPC_TYPE_DUCKHIT:	          /// ²ܴnpc  ֮  21
				      //case NPC_TYPE_BANNER:	          ///   22
				      //case NPC_TYPE_TRAP:               ///   23
							if (attCountryID!=scene->getCountryID())
							{
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "治ң ҹBOSS %s %s(%u,%u)ʱ%s%sɱ", name , scene->getRealName(), pos.x, pos.y, SceneManager::getInstance().getCountryNameByCountryID(attCountryID), temp->name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
						default:
							break;
					}*/

					//sky ȥɱ
					switch(npc->kind)
					{
					case NPC_TYPE_BBOSS:              /// Boss  2   
						if (attCountryID!=scene->getCountryID())
						{
							if (scene->getCountryID() == 6)
								Channel::sendAllInfo(Cmd::INFO_TYPE_EXP2,"ţ%s%s%s(%u,%u)ɱBOSS %s()", SceneManager::getInstance().getCountryNameByCountryID(attCountryID), temp->name,scene->getRealName(), pos.x, pos.y, name);	
						}
						break;
					case NPC_TYPE_PBOSS: //
						{
							if (scene->getCountryID() == 6)
								Channel::sendAllInfo(Cmd::INFO_TYPE_EXP3,"%s%s%s(%u,%u)ɱBOSS %s()", SceneManager::getInstance().getCountryNameByCountryID(attCountryID), temp->name,scene->getRealName(), pos.x, pos.y, name);	
						}
						break;
					case NPC_TYPE_BACKBONE: //
						{
							if (scene->getCountryID() == 6)
								Channel::sendAllInfo(Cmd::INFO_TYPE_EXP4,"%s%s%s(%u,%u)ɱBOSS %s()", SceneManager::getInstance().getCountryNameByCountryID(attCountryID), temp->name,scene->getRealName(), pos.x, pos.y, name);	
						}
						break;
					case NPC_TYPE_GOLD: //
						{
							if (scene->getCountryID() == 6)
								Channel::sendAllInfo(Cmd::INFO_TYPE_EXP5,"%s%s%s(%u,%u)ɱBOSS %s()", SceneManager::getInstance().getCountryNameByCountryID(attCountryID), temp->name,scene->getRealName(), pos.x, pos.y, name);	
						}
						break;			
					case NPC_TYPE_REDBOSS: //
						{
							if (scene->getCountryID() == 6)
								Channel::sendAllInfo(Cmd::INFO_TYPE_EXP5,"%s%s%s(%u,%u)ɱBOSS %s()", SceneManager::getInstance().getCountryNameByCountryID(attCountryID), temp->name,scene->getRealName(), pos.x, pos.y, name);	
						}
						break;						
					default:
						break;
					}

			        switch(npc->kind)
			        {
                        case NPC_TYPE_DUCKHIT:	          /// ²ܴnpc  ֮  21
							if (attCountryID!=scene->getCountryID())
							{
					//		Zebra::logger->debug("----------npc 䡢֮ģ21˲ɼҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "ź ҹ%s %s(%u,%u)%s%sѹһˣ",  scene->getRealName(), name, pos.x, pos.y, SceneManager::getInstance().getCountryNameByCountryID(attCountryID), temp->name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
						default:
							break;
					}

			        switch(npc->kind)
			       {
				        case NPC_TYPE_BACKBONE:		      /// Ӣ  4   
							if (attCountryID!=scene->getCountryID())
							{
					//			Zebra::logger->debug("----------ɫBOSS4˻ɱҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "治ң ҹBOSS %s()%s(%u,%u)%s%sѣ", name , scene->getRealName(), pos.x, pos.y, SceneManager::getInstance().getCountryNameByCountryID(attCountryID), temp->name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
				        case NPC_TYPE_GOLD:		          /// ƽ  5  
							if (attCountryID!=scene->getCountryID())
							{
					//			Zebra::logger->debug("----------ػɫBOSS5˻ɱҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "治ң ҹBOSS %s()%s(%u,%u)%s%sɱˣ", name , scene->getRealName(), pos.x, pos.y, SceneManager::getInstance().getCountryNameByCountryID(attCountryID), temp->name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
				        case NPC_TYPE_BBOSS:              /// Boss  2   
							if (attCountryID!=scene->getCountryID())
							{
							//	Zebra::logger->debug("----------ɫBOSS2˻ɱҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "治ң ҹBOSS %s()%s(%u,%u)%s%s̵ɱ", name , scene->getRealName(), pos.x, pos.y, SceneManager::getInstance().getCountryNameByCountryID(attCountryID), temp->name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
				        case NPC_TYPE_PBOSS:              /// Boss  31  
							if (attCountryID!=scene->getCountryID())
							{
						//		Zebra::logger->debug("----------ɫBOSS31˻ɱҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "治ң ҹBOSS %s()%s(%u,%u)%s%s̵ɱ", name , scene->getRealName(), pos.x, pos.y, SceneManager::getInstance().getCountryNameByCountryID(attCountryID), temp->name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
				        case NPC_TYPE_REDBOSS:		          /// ƽ  33  
							if (attCountryID!=scene->getCountryID())
							{
					//			Zebra::logger->debug("----------ػɫBOSS5˻ɱҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "治ң ҹBOSS %s()%s(%u,%u)%s%sɱˣ", name , scene->getRealName(), pos.x, pos.y, SceneManager::getInstance().getCountryNameByCountryID(attCountryID), temp->name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
						default:
							break;
					}
					//soke ӹɱNPCҹ 

			        switch(npc->kind)
			        {
                        case NPC_TYPE_DUCKHIT:	          /// ²ܴnpc  ֮  21
							if (attCountryID=scene->getCountryID())//soke  
							{
						//	Zebra::logger->debug("----------npc 䡢֮ģ21˲ɼҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "ź %s%s(%u,%u)%sѹһˣ",  scene->getRealName(), name, pos.x, pos.y,/* SceneManager::getInstance().getCountryNameByCountryID(attCountryID),*/ temp->name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
						default:
							break;
					}

			        switch(npc->kind)
			        {
				        case NPC_TYPE_BACKBONE:		      /// Ӣ  4   
							if (attCountryID=scene->getCountryID())
							{
						//		Zebra::logger->debug("----------ɫBOSS4˻ɱҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "ҹ%s%s(%u,%u)ɹɱBOSS %s()",/*SceneManager::getInstance().getCountryNameByCountryID(attCountryID),*/ temp->name ,scene->getRealName(), pos.x, pos.y, name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
				        case NPC_TYPE_GOLD:		          /// ƽ  5  
							if (attCountryID=scene->getCountryID())
							{
						//		Zebra::logger->debug("----------ػɫBOSS5˻ɱҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "ҹ%s%s(%u,%u)ɹɱBOSS %s()",/*SceneManager::getInstance().getCountryNameByCountryID(attCountryID),*/ temp->name ,scene->getRealName(), pos.x, pos.y, name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
				        case NPC_TYPE_BBOSS:              /// Boss  2   
							if (attCountryID=scene->getCountryID())
							{
						//		Zebra::logger->debug("----------ɫBOSS2˻ɱҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "ҹ%s%s(%u,%u)ɹɱBOSS %s()",/*SceneManager::getInstance().getCountryNameByCountryID(attCountryID),*/ temp->name ,scene->getRealName(), pos.x, pos.y, name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
						case NPC_TYPE_PBOSS:              /// Boss  31  
							if (attCountryID=scene->getCountryID())
							{
						//		Zebra::logger->debug("----------ɫBOSS31˻ɱҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "찡 ҹ%s%s(%u,%u)ɹɱBOSS %s()",/*SceneManager::getInstance().getCountryNameByCountryID(attCountryID),*/ temp->name ,scene->getRealName(), pos.x, pos.y, name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
				        case NPC_TYPE_REDBOSS:		          /// ƽ  33  
							if (attCountryID=scene->getCountryID())
							{
						//		Zebra::logger->debug("----------ػɫBOSS5˻ɱҹ----------");
								Cmd::Session::t_countryNotify_SceneSession send;
								send.infoType = Cmd::INFO_TYPE_EXP;
								send.dwCountryID = scene->getCountryID();
								snprintf(send.info, MAX_CHATINFO, "ҹ%s%s(%u,%u)ɹɱBOSS %s()",/*SceneManager::getInstance().getCountryNameByCountryID(attCountryID),*/ temp->name ,scene->getRealName(), pos.x, pos.y, name);
								sessionClient->sendCmd(&send, sizeof(send));
							}
							break;
						default:
							break;
					}


					if (npc->id==30034)//ɱ
					{
						SceneUser * u = (SceneUser *)pAtt;
						if (u->isRedNamed())
						{
							u->charbase.goodness = (DWORD)Cmd::GOODNESS_2_1;
							while (!u->pkState.cancelProtect(u));

							Channel::sendSys(u, Cmd::INFO_TYPE_EXP, "ɱ٣Լеķ¼");
							Zebra::logger->debug("[]%s ɱ", u->name);
						}
					}

					if (npc->id==144001)//BOSS
					{
						// SceneUser * u = (SceneUser *)pAtt;
						// if(u)
						// {
						// 	if (u->charbase.country == 8)
						// 	{
								
		
						// 	}
						// 	else{
						// 		Channel::sendAllInfo(Cmd::INFO_TYPE_EXP5,"ϲҳɹɱBOSSùȨ" );	
						// 		Channel::sendAllInfo(Cmd::INFO_TYPE_ZISEYROLL,"ϲҳɹɱBOSSùȨ");
						
						// 	}
						// }
						Channel::sendAllInfo(Cmd::INFO_TYPE_EXP5,"BOSSѱɱ" );	
						Channel::sendAllInfo(Cmd::INFO_TYPE_ZISEYROLL,"BOSSѱɱ");
						
					}

					if (npc->id==140003)//ɱɢ
					{
						zRTime cur_time;
						cur_time.now();
						long long  time = cur_time.msecs();
						long long  time2 = time%1000;
						char a[64];
						sprintf(a, "%ld", time);
						Zebra::global["killpigtime"]=a;
						sprintf(a, "%ld", time2);
						Zebra::global["killpigtime_hm"]=a;
						//ͻʱ俪ʼʱ
						Cmd::stJingcaiReturnKillPigTimeCmd ret;
						ret.killpigtime = atol(Zebra::global["killpigtime"].c_str());
						ret.killpigtime_hm = atol(Zebra::global["killpigtime_hm"].c_str());
						Scene * s = SceneManager::getInstance().getSceneByName("ѫͼ");
						s->sendCmdToScene(&ret, sizeof(ret));	
						int kaijiang =0;
						if(time2<10)
						{
							kaijiang = time2;
						}
						else{
							kaijiang = time2%10;
						}
						Channel::sendAllInfo(Cmd::INFO_TYPE_EXP,"־½ǣ%d", kaijiang);	
						Cmd::Session::t_Gxjckj_SceneSession send;
						send.index = kaijiang;
						sessionClient->sendCmd(&send, sizeof(send));

						//ѫ¿
						
					}
					if (npc->id==54100)//ɱڳ
					{
						zNpcB *base = npcbm.get(54099);
						t_NpcDefine define;
						define.id = base->id;
						strcpy(define.name, base->name);
						define.pos = getPos();
						define.num = 1;
						define.interval = 0; 
						define.initstate = zSceneEntry::SceneEntry_Normal;
						define.width = 2;
						define.height = 2;
						define.scriptID = 0;
						scene->initRegion(define.region, define.pos, define.width, define.height);
						ScenesService::getInstance().han_biaoche2 = scene->summonOneNpc<SceneNpc>(define, define.pos, base);
						SceneUser * u = (SceneUser *)pAtt;
						zObjectB *ob = objectbm.get(123123);
						//zObjectB *ob2 = objectbm.get(20800);
						for(int i=0;i<100;i++)
						{
							if (ob)
							{
								scene->addObject2(ob,100,getPos(),u->id);
							}
						}
						/*
						for(int i=0;i<16;i++)//ƽɳ
						{
							if (ob2)
							{
								scene->addObject2(ob2,50,getPos(),u->id);
							}
						}
						*/
						for (SceneManager::CountryMap_iter iter = SceneManager::getInstance().country_info.begin(); iter != SceneManager::getInstance().country_info.end();
						 iter++)
						{
							char a[MAX_NAMESIZE];
							sprintf(a, "%s damaged escort", u->name);
							Cmd::Session::t_countryNotify_SceneSession send;
							bzero(send.info, sizeof(send.info));
							sprintf(send.info, a);
							send.dwCountryID = iter->second.id;
							sessionClient->sendCmd(&send, sizeof(send));

							// Cmd::Session::t_countryNotify_SceneSession send;
							bzero(send.info, sizeof(send.info));
							sprintf(send.info, "Escort damaged, keep escorting");
							send.dwCountryID = iter->second.id;
							sessionClient->sendCmd(&send, sizeof(send));
						}
						Channel::sendAllInfo(Cmd::INFO_TYPE_EXP2,"Escort damaged, keep escorting");
						ScenesService::getInstance().han_biaoche->setClearState();
						ScenesService::getInstance().han_biaoche = NULL;
					}
					if (npc->id==54098)//ɱڳ
					{
						zNpcB *base = npcbm.get(54099);
						t_NpcDefine define;
						define.id = base->id;
						strcpy(define.name, base->name);
						define.pos = getPos();
						define.num = 1;
						define.interval = 0; 
						define.initstate = zSceneEntry::SceneEntry_Normal;
						define.width = 2;
						define.height = 2;
						define.scriptID = 0;
						scene->initRegion(define.region, define.pos, define.width, define.height);
						ScenesService::getInstance().chu_biaoche2 = scene->summonOneNpc<SceneNpc>(define, define.pos, base);
						SceneUser * u = (SceneUser *)pAtt;
						zObjectB *ob = objectbm.get(123123);
						//zObjectB *ob2 = objectbm.get(20800);
						for(int i=0;i<100;i++)
						{
							if (ob)
							{
								scene->addObject2(ob,100,getPos(),u->id);
							}
						}
						/*
						for(int i=0;i<16;i++)//ƽɳ
						{
							if (ob2)
							{
								scene->addObject2(ob2,50,getPos(),u->id);
							}
						}
						*/
						for (SceneManager::CountryMap_iter iter = SceneManager::getInstance().country_info.begin(); iter != SceneManager::getInstance().country_info.end();
						 iter++)
						{
							char a[MAX_NAMESIZE];
							sprintf(a, "%s damaged escort", u->name);
							Cmd::Session::t_countryNotify_SceneSession send;
							bzero(send.info, sizeof(send.info));
							sprintf(send.info, a);
							send.dwCountryID = iter->second.id;
							sessionClient->sendCmd(&send, sizeof(send));
							
							// Cmd::Session::t_countryNotify_SceneSession send;
							bzero(send.info, sizeof(send.info));
							sprintf(send.info, "Escort damaged, keep escorting");
							send.dwCountryID = iter->second.id;
							sessionClient->sendCmd(&send, sizeof(send));
						}
						Channel::sendAllInfo(Cmd::INFO_TYPE_EXP2,"Escort damaged, keep escorting");
						ScenesService::getInstance().chu_biaoche->setClearState();
						ScenesService::getInstance().chu_biaoche = NULL;
					}

					if (npc->id==54376)//ɱڳ
					{
						zNpcB *base = npcbm.get(54099);
						t_NpcDefine define;
						define.id = base->id;
						strcpy(define.name, base->name);
						define.pos = getPos();
						define.num = 1;
						define.interval = 0; 
						define.initstate = zSceneEntry::SceneEntry_Normal;
						define.width = 2;
						define.height = 2;
						define.scriptID = 0;
						scene->initRegion(define.region, define.pos, define.width, define.height);
						ScenesService::getInstance().wei_biaoche2 = scene->summonOneNpc<SceneNpc>(define, define.pos, base);
						SceneUser * u = (SceneUser *)pAtt;
						zObjectB *ob = objectbm.get(123123);
						//zObjectB *ob2 = objectbm.get(20800);
						for(int i=0;i<100;i++)
						{
							if (ob)
							{
								scene->addObject2(ob,100,getPos(),u->id);
							}
						}
						/*
						for(int i=0;i<16;i++)//ƽɳ
						{
							if (ob2)
							{
								scene->addObject2(ob2,50,getPos(),u->id);
							}
						}
						*/
						for (SceneManager::CountryMap_iter iter = SceneManager::getInstance().country_info.begin(); iter != SceneManager::getInstance().country_info.end();
						 iter++)
						{
							char a[MAX_NAMESIZE];
							sprintf(a, "%s damaged escort", u->name);
							Cmd::Session::t_countryNotify_SceneSession send;
							bzero(send.info, sizeof(send.info));
							sprintf(send.info, a);
							send.dwCountryID = iter->second.id;
							sessionClient->sendCmd(&send, sizeof(send));
							
							// Cmd::Session::t_countryNotify_SceneSession send;
							bzero(send.info, sizeof(send.info));
							sprintf(send.info, "Ĺڳѱӣֻʣ¹ͺͺƳˣ̫ˣ");
							send.dwCountryID = iter->second.id;
							sessionClient->sendCmd(&send, sizeof(send));
						}
						Channel::sendAllInfo(Cmd::INFO_TYPE_EXP2,"Ĺڳѱӣֻʣ¹ͺͺƳˣ̫ˣ");
						ScenesService::getInstance().wei_biaoche->setClearState();
						ScenesService::getInstance().wei_biaoche = NULL;
					}



					/*˽ڳ
					if ((npc->id>=54035 && npc->id<=54038) || (npc->id>=54060 && npc->id<=54063)|| (npc->id>=54081 && npc->id<=54087))
					{
						SceneUser * u = (SceneUser *)pAtt;
						zObjectB *ob2 = objectbm.get(20801);
						for(int i=0;i<10;i++)//ƽɲ
						{
							if (ob2)
							{
								scene->addObject2(ob2,10,getPos(),u->id);
							}
						}
					}
					*/
					if (npc->id==144007)//ɱBOSS
					{
						char a[16];
						sprintf(a, "%d", 1);
						Zebra::global["BOSS_CHU"]=a;
					}
					if (npc->id==144008)//ɱBOSS
					{
						char a[16];
						sprintf(a, "%d", 1);
						Zebra::global["BOSS_HAN"]=a;
					}

					if (npc->id==40001)//1
					{
						SceneUser * u = (SceneUser *)pAtt;
						if(u->charbase.fba1 <=0 )
						{
							Gm::gomap(u, "name=ʳ pos=832,699");
							return true;
						}

						u->charbase.fba1--;	
					}
					if (npc->id==40002)//2
					{
						SceneUser * u = (SceneUser *)pAtt;
						if(u->charbase.fba2 <=0 )
						{
							Gm::gomap(u, "name=ʳ pos=832,699");
							return true;
						}

						u->charbase.fba2--;	
					}
					if (npc->id==40003)//3
					{
						SceneUser * u = (SceneUser *)pAtt;
						if(u->charbase.fba3 <=0 )
						{
							Gm::gomap(u, "name=ʳ pos=832,699");
							return true;
						}

						u->charbase.fba3--;	
					}
					if (npc->id==40004)//4
					{
						SceneUser * u = (SceneUser *)pAtt;
						if(u->charbase.fba4 <=0 )
						{
							Gm::gomap(u, "name=ʳ pos=832,699");
							return true;
						}

						u->charbase.fba4--;	
					}
				}
				break;
			default:
				{
					temp = pAtt;
				}
				break;
		}

		resetSpeedRate();
		resetAspeedRate();

		if (AIC) AIC->on_die();

		if (this->id == COUNTRY_MAIN_FLAG || this->isMainGeneral() || this->id == COUNTRY_KING_MAIN_FLAG || this->id == COUNTRY_EMPEROR_MAIN_GEN)
		{
			Cmd::Session::t_countryDareResult_SceneSession send;
			send.dwAttCountryID = attCountryID;
			send.dwDefCountryID = this->scene->getCountryID();

			if (attCountryID>0)
			{
				strncpy(send.attCountryName, 
						SceneManager::getInstance().getCountryNameByCountryID(attCountryID), 
						sizeof(send.attCountryName));
			}
			else
			{
				bzero(send.attCountryName, sizeof(send.attCountryName));
			}

			strncpy(send.defCountryName, 
					SceneManager::getInstance().getCountryNameByCountryID(this->scene->getCountryID()), 
					sizeof(send.defCountryName));

			if (this->id == COUNTRY_MAIN_FLAG || this->id == COUNTRY_KING_MAIN_FLAG)
			{
				send.byType = Cmd::Session::COUNTRY_ANNOY_DARE;
				send.dwAttUserID = dwAttUserID;
			}
			else if (this->isMainGeneral())
			{
				if (isAntiAtt)
				{
					send.byType = Cmd::Session::COUNTRY_FORMAL_ANTI_DARE;
				}
				else
				{
					send.byType = Cmd::Session::COUNTRY_FORMAL_DARE;
				}

				send.dwAttUserID = dwAttUserID;
			}
			else if (this->id == COUNTRY_EMPEROR_MAIN_GEN)
			{
				send.byType = Cmd::Session::EMPEROR_DARE;
				send.dwAttUserID = dwAttUserID;
			}

			sessionClient->sendCmd(&send, sizeof(send));
		}
	    //31͵Npc
		if (npc->kind==2 || npc->kind==4 || npc->kind==5 || npc->kind==31)
		{	
			Cmd::Session::t_BossDieInfo_SceneSession BossSend;
			BossSend.NpcID = npc->id;
			BossSend.CountryID = scene->getCountryID();
			bcopy(scene->getRealName(),BossSend.MapName, MAX_NAMESIZE);
			BossSend.KillState = 1; //Ĵ״̬
			BossDieTime = SceneTimeTick::currentTime;
	        QWORD DieTime = BossDieTime.msecs();
			BossSend.ReviveTime = DieTime+(define->interval*1000); 
			sessionClient->sendCmd(&BossSend, sizeof(BossSend));
		}	
		if (this->id == COUNTRY_SEC_GEN)
		{// ֪ͨط󽫾ܵ
			Cmd::Session::t_countryNotify_SceneSession send;
			bzero(send.info, sizeof(send.info));
			sprintf(send.info, "%s", "ӳ󽫾ܵ!");
			send.dwCountryID = this->scene->getCountryID();
			sessionClient->sendCmd(&send, sizeof(send));
		}

		if (this->id == COUNTRY_SEC_FLAG)
		{// ֪ͨطܵ
			Cmd::Session::t_countryNotify_SceneSession send;
			bzero(send.info, sizeof(send.info));
			sprintf(send.info, " %s (%u,%u)", this->scene->getRealName(), this->getPos().x, this->getPos().y);
			send.dwCountryID = this->scene->getCountryID();
			sessionClient->sendCmd(&send, sizeof(send));
		}

		if (this->id == 58101 && attUnionID>0 && attCountryID > 0) //ǳ죬֪ͨỰг任
		{
			Cmd::Session::t_UnionCity_DareResult_SceneSession send;
			send.dwUserID = pAtt->id;
			send.dwCountryID = this->scene->getCountryID();
			send.dwCityID = pAtt->scene->getRealMapID();
			send.dwUnionID = attUnionID;
			sessionClient->sendCmd(&send, sizeof(send));
		}

		ScenePk::attackDeathNpc(temp, this);
		if (this->summonsplit)
		{
			if (mymaster&&zMisc::selectByPercent(this->summonsplit))
			{
				mymaster->summonPet(this->npc->id, Cmd::PET_TYPE_TOTEM, 30, 0, 0, 0);
				mymaster->summonPet(this->npc->id, Cmd::PET_TYPE_SUMMON, 0, 0, 0, 0);
			}
		}
		return true;
	}
	return false;
}

#include "Chat.h"

#include "script.h"
/**
 * \brief ״̬
 *
 *
 * \param user 
 */
void SceneNpc::set_quest_status(SceneUser* user)
{
	clearUState(Cmd::USTATE_START_QUEST);
	clearUState(Cmd::USTATE_DOING_QUEST);	
	clearUState(Cmd::USTATE_FINISH_QUEST);	

	if (ScriptQuest::get_instance().has(ScriptQuest::NPC_VISIT, id)) { 
		char func_name[32];
		sprintf(func_name, "%s_%d", "state", id);
		int state = execute_script_event(user,func_name, this);
		if (state) {
			setUState(state);
			return;
		}
	}

	OnVisit event(id);
	int state = EventManager<OnVisit>::instance().state(*user, event) ;
	if (state != -1) {
		setUState(state);
		//Zebra::logger->debug("NPC(%s), (%d, %d) ", name, id, state);

	}
}

/**
 * \brief 9ԼϢ
 */
void SceneNpc::reSendMyMapData()
{
    this->sendMeToNine();
}

/**
 * \brief npcлͼ
 * ֻͬһ
 *
 *
 * \param newScene Ҫȥĵͼ
 * \param pos Ҫȥλ
 * \return Ƿлɹ
 */
bool SceneNpc::changeMap(Scene * newScene, const zPos &pos)
{
	if (!newScene)
	{
		Zebra::logger->error("SceneNpc::changeMap(): target scene is null");
		return false;
	}

	Scene *oldScene = scene;
	if (!oldScene)
	{
		Zebra::logger->error("SceneNpc::changeMap(): old scene is null npc=%s(%u)", name, tempid);
		return false;
	}

	const zPos oldPos = getPos();
	const zPosI oldPosI = getPosI();
	const uint64_t oldHP = hp;
	const uint64_t oldLastHP = lasthp;

	oldScene->removeNpc(this);
	if (!newScene->refreshNpc(this, pos))
	{
		Zebra::logger->debug("SceneNpc::changeMap(): npc=%s(%u) target=%s pos=(%u,%u) failed, restoring old scene=%s pos=(%u,%u)",
				name, tempid, newScene->name, pos.x, pos.y, oldScene->name, oldPos.x, oldPos.y);
		scene = oldScene;
		hp = oldHP;
		lasthp = oldLastHP;
		oldScene->refreshNpc(this, oldPos);
		this->sendMeToNine();
		return false;
	}

	scene = newScene;
	hp = oldHP;
	lasthp = oldLastHP;

	Cmd::stRemoveMapNpcMapScreenUserCmd removeNpc;
	removeNpc.dwMapNpcDataPosition = tempid;
	oldScene->sendCmdToNine(oldPosI, &removeNpc, sizeof(removeNpc));
	this->sendMeToNine();

	Zebra::logger->debug("SceneNpc::changeMap(): npc=%s(%u) target=%s pos=(%u,%u) hp=%llu/%llu",
			name, tempid, newScene->name, getPos().x, getPos().y,
			(unsigned long long)hp, (unsigned long long)getMaxHP());
	return true;
}


/**
 * \brief ѡиnpcҷ͸npc״̬Ϣ
 *
 *
 * \param state npc״̬
 * \param value ֵ
 * \param time ʱ
 * \return 
 */
void SceneNpc::sendtoSelectedState(DWORD state , WORD value , WORD time)
{
	//Zebra::logger->debug("NPC(%s), ״̬(%d, %d , %d)", name, state , value , time);
	using namespace Cmd;
	char Buf[200]; 
	bzero(Buf , sizeof(Buf));
	stSelectReturnStatesPropertyUserCmd *srs=(stSelectReturnStatesPropertyUserCmd*)Buf;
	constructInPlace(srs);
	srs->byType = MAPDATATYPE_NPC;
	srs->dwTempID = this->tempid;
	srs->states[0].state = state;
	srs->states[0].result = value;
	srs->states[0].time = time;
	srs->size=1;
	//selected_lock.lock();
	SelectedSet_iterator iter = selected.begin();
	for(; iter != selected.end() ;)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(*iter);
		if(pUser)
		{
			if(this->scene->checkTwoPosIInNine(this->getPosI() , pUser->getPosI()))
			{
				pUser->sendCmdToMe(srs , sizeof(stSelectReturnStatesPropertyUserCmd) + sizeof(srs->states[0]));
				iter ++ ;
				continue;
			}
		}
		SelectedSet_iterator iter_del = iter;
		iter_del ++;
		selected.erase(iter);
		iter = iter_del;
	}
	//selected_lock.unlock();
}

/**
 * \brief ѡָnpcҷ͸npchpnp
 */
void SceneNpc::sendtoSelectedHpAndMp()
{
	Cmd::stRTSelectedHpMpPropertyUserCmd ret;
	ret.byType = Cmd::MAPDATATYPE_NPC;
	ret.dwTempID = this->tempid;//ʱ
	ret.dwHP = this->hp;//ǰѪ
	ret.dwMaxHp = getMaxHP();//hp
	ret.dwMP = 0;//this->charbase.mp;//ǰmp
	ret.dwMaxMp = 0;//this->charstate.maxmp;//mp
	//selected_lock.lock();
	SelectedSet_iterator iter = selected.begin();
	for(; iter != selected.end() ;)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(*iter);
		if(pUser)
		{
			if(this->scene->checkTwoPosIInNine(this->getPosI() , pUser->getPosI()))
			{
				pUser->sendCmdToMe(&ret ,sizeof(ret));
				//Zebra::logger->debug("sendtoSelectedHpAndMp(): hpmp %s", pUser->name);
				iter ++ ;
				continue;
			}
		}
		SelectedSet_iterator iter_del = iter;
		iter_del ++;
		selected.erase(iter);
		iter = iter_del;
	}
	//selected_lock.unlock();
}

/**
 * \brief ͼϵĳʱ
 *
 * \param standTime ӳʱ
 */
void SceneNpc::setStandingTime(DWORD standTime)
{
    dwStandTime += standTime;
    dwStandTimeCount=standTime;
#ifdef _DEBUGLOG
    if (standTime>0) Zebra::logger->debug("ͼϵĳʱ%u ǰʱ[%u]", standTime, dwStandTime);
#endif
}


/**
 * \brief жnpc
 *
 *
 * \param five Ҫжϵֵ
 * \return 1 2 0޹
 */
int SceneNpc::IsOppose(DWORD five)
{
	if(this->npc->five == (five + 4)%5)
	{
		return 1;
	}
	else
	{
		if(this->npc->five == (five + 1)%5)
		{
			return 2;
		}

	}
	if ((this->npc->five != 5) &&(five == 5))
		return 1;
	if ((this->npc->five == 5) &&(five != 5))
		return 2;
	return 0;
}

/**
 * \brief npcƶٶȱ
 *
 * \param rate 
 */
void SceneNpc::setSpeedRate(float rate)
{
	speedRate = rate;

	//hp20%ٶȼӱ
	if (speedUpUnder20)
		speedRate *= 2.0;

	//this->sendMeToNine();
	//Channel::sendNine(this, "ƶٶ %f", speedRate);
}

/**
 * \brief npcԭʼٶ
 */
void SceneNpc::resetSpeedRate()
{
	DWORD value = (DWORD)((640-skillValue.movespeed)*(1+this->skillValue.array_dmvspeed/100.0f));
	if (value == 0)
	{
		speedRate = 6;
	}
	else
	{
		speedRate = 640.0f/((float)value);
	}

	if (this->assault) speedRate =4;

	//hp20%ٶȼӱ
	if (speedUpUnder20)
		speedRate *= 2.0;

	//this->sendMeToNine();
	//Channel::sendNine(this, "ƶٶ %f", speedRate);
}

/**
 * \brief npcĹٶȱ
 *
 * \param rate 
 */
void SceneNpc::setAspeedRate(float rate)
{
    aspeedRate = rate;

    //hp50%¹ٶȼӱ
    if (aspeedUpUnder50)
		aspeedRate *= 2.0;

    //Channel::sendNine(this, "ٶ %f", aspeedRate);
    //Zebra::logger->debug("%s ùٶȱ %f", name, rate);
}

/**
 * \brief npcԭʼٶ
 */
void SceneNpc::resetAspeedRate()
{
    aspeedRate = 1.0;

    //hp50%¹ٶȼӱ
    if (aspeedUpUnder50)
		aspeedRate *= 2.0;

    //Channel::sendNine(this, "ٶ %f", aspeedRate);
}

/**
 * \brief ȡnpc
 */
void SceneNpc::clearMaster()
{
    //master = NULL;
}

//Ѱʱж
/**
 * \brief жǷĿ
 *
 *
 * \param entry ҪжϵĿ
 * \return Ƿ
 */
bool SceneNpc::canChaseTarget(const SceneEntryPk * entry)
{
	switch (entry->getType())
	{
		case zSceneEntry::SceneEntry_Player:
			{
				if (NPC_AI_PATROL==AIDefine.type)
				{
					if (((SceneUser *)entry)->isRedNamed())
						return true;
					else
						return false;
				}
				else
					return true;
			}
		case zSceneEntry::SceneEntry_NPC:
			{
				//ʱ
				if (NPC_TYPE_PET!=/* ((SceneNpc *)entry)-> */npc->kind) return false;
				if (NPC_AI_PATROL==AIDefine.type)
				{
					if (((SceneNpc *)entry)->isRedNamed()) return true;

					SceneEntryPk * pk = ((SceneNpc *)entry)->getChaseSceneEntry();
					if ((pk) && (zSceneEntry::SceneEntry_Player==pk->getType())
							&&(NPC_TYPE_PET!=((SceneNpc *)entry)->npc->kind))
						return true;
				}
				else
					if (NPC_TYPE_PET==((SceneNpc *)entry)->npc->kind)
						return true;
			}
		default:
			return false;
	}
	return false;
}

/**
 * \brief npcǷ
 *
 * \return Ƿ
 */
bool SceneNpc::isRedNamed(bool allRedMode)
{
    return false;
}

/**
 * \brief npcű
 *
 * \param id űid
 * \return Ƿóɹ
 */
bool SceneNpc::setScript(int id)
{
    if (!AIC) return false;

    AIC->loadScript(id);
    return AIC->isActive();
}

/**
 * \brief npcű
 *
 */
void SceneNpc::clearScript()
{
    if (!AIC) return;
    AIC->unloadScript();
}

/**
 * \brief ߳
 * \param attacktype ߵ
 * \param tempid ߵʱid
 * \author fqnewman
 */
void SceneNpc::assaultMe(BYTE attacktype, DWORD tempid)
{
	if (attacktype == zSceneEntry::SceneEntry_Player)
	{
		SceneUser *pUser = SceneUserManager::getMe().getUserByTempID(tempid);
		if (pUser)
		{
			zPos tmppos = pos;
			const zPos &pos1 = pUser->getPos();
			if (pos1.x>pos.x)
			{
				tmppos.x++;
			}
			else if (tmppos.x>0)
			{
				tmppos.x--;
			}

			if (pos1.y>pos.y)
			{
				tmppos.y++;
			}
			else if (tmppos.y>0)
			{
				tmppos.y--;
			}
			pUser->goTo(tmppos);
		}
	}
}

/**
 * \brief Ŀʹü
 *
 *
 * \param target Ŀ
 * \param id id
 * \return Ƿʹóɹ
 */
bool SceneNpc::useSkill(SceneEntryPk * target, DWORD id)
{
	//return false;
	if (!target) return false;
	if (!canAttack(target)) return false;

	zSkill *s = usm.findSkill(id);
	if (!s) return false;
	if (!s->canUse()) return false;

	BYTE atype = 0;
	BYTE action = 0;
	npc->getATypeAndAction(atype, action);

	if (this->npc->kind != NPC_TYPE_SURFACE) setDir(getPos().getDirect(target->getPos()));

	using namespace Cmd;
	stAttackMagicUserCmd att;
	att.dwUserTempID = tempid;
	att.dwDefenceTempID = target->tempid;

	att.wdMagicType = id;
	switch(atype)
	{
		case NPC_ATYPE_NEAR:	/// 빥
			{
				att.byAction = Ani_Attack;
			}
			break;
		case NPC_ATYPE_FAR:		/// Զ빥
			{
				att.byAction = Ani_Attack;
			}
			break;
		case NPC_ATYPE_MFAR:
		case NPC_ATYPE_MNEAR:
			{
				att.byAction = Ani_Null;
			}
			break;
		case NPC_ATYPE_NOACTION:
			{
				att.byAction = Ani_Num;
			}
			break;
		default:
			break;
	}
	//att.byAction = action;

	att.xDes = (WORD)target->getPos().x;
	att.yDes = (WORD)target->getPos().y;
	att.byDirect = getDir();
	switch (target->getType())
	{
		case zSceneEntry::SceneEntry_Player:
			{
				att.byAttackType = ATTACKTYPE_N2U;
			}
			break;
		case zSceneEntry::SceneEntry_NPC:
			{
				att.byAttackType = ATTACKTYPE_N2N;
			}
			break;
		default:
			{
				att.byAttackType = ATTACKTYPE_N2U;
			}
			break;
	}

	s->action(&att, sizeof(att));
#ifdef _XWL_DEBUG
	//Channel::sendNine(this, "%s!", s->name);
#endif
	setAttackTime(SceneTimeTick::currentTime);
	setMoveTime(SceneTimeTick::currentTime);
	setEndBattleTime(SceneTimeTick::currentTime, 10*1000);

	return true;
}

/**
 * \brief npc˵
 *
 * \param type ˵
 */
void SceneNpc::randomChat(NpcChatType type)
{
	//soke ˵NPC
	if ((npc->kind!=0)
			&&(npc->kind!=1)
			&&(npc->kind!=2)
			&&(npc->kind!=3)
			&&(npc->kind!=4)
			&&(npc->kind!=5)
			&&(npc->kind!=6)
			&&(npc->kind!=7)
			&&(npc->kind!=8)
			&&(npc->kind!=9)
			&&(npc->kind!=10)
			&&(npc->kind!=11)
			&&(npc->kind!=19)
			&&(npc->kind!=20)
			&&(npc->kind!=24)
			&&(npc->kind!=25)
			&&(npc->kind!=26)
			&&(npc->kind!=27)
			&&(npc->kind!=28)
			&&(npc->kind!=30)
			&&(npc->kind!=31))
		return;

	char str[MAX_CHATINFO];
	if (SceneNpcManager::getMe().getNpcCommonChat(type, str))
		Channel::sendNine(this, str);
	//else
	//	Zebra::logger->debug("ȡ˵ʧ type=%d", type);
}

/**
 * \brief ɫ˿־Ч
 */
bool SceneNpc::dreadProcess()
{
	if (dread)
	{
		int count=10;
		int curDir=0;
		do {
			if (count==10)
				curDir = getDir()+zMisc::randBetween(-1,1);
			else
				curDir++;
			if (curDir <0) curDir = 7;
			count--;
		}while(!move(curDir%8, 1)&&count>0);
		return true;
	}
	return false;
}

/**
 * \brief õnpcĹ
 *
 * \return 
 */
BYTE SceneNpc::getAType()
{
    BYTE atype = 0;
    BYTE action = 0;
    npc->getATypeAndAction(atype,action);
    return atype;
}

/**
 * \brief жǷ
 *
 * \return Ƿ
 */
bool SceneNpc::isRedNamed(bool allRedMode) const
{
    return false;
}
/**
 * \brief ϢĽṹ
 *
 *
 * \param data ṹַ
 */     
void SceneNpc::full_PetDataStruct(Cmd::t_PetData & data)
{
}

/**
 * \brief Ƿ
 *
 * \return Ƿ
 */
bool SceneNpc::isActive()
{
    return aif&AIF_ACTIVE_MODE;
}

/**
 * \brief Ƿս
 *
 * \return Ƿս
 */
bool SceneNpc::canFight()
{
    return !(aif&AIF_NO_BATTLE);
}

/**
 * \brief Ƿƶ
 *
 * \return Ƿƶ
 */
bool SceneNpc::canMove()
{
    return ( !(aif&AIF_NO_MOVE) && moveAction);
}

/**
 * \brief õ
 *
 * \return 
 */
Cmd::petType SceneNpc::getPetType()
{
    return Cmd::PET_TYPE_NOTPET;
}

/**
 * \brief npcΪ״̬
 * ´ѭʱ
 */
void SceneNpc::setClearState()
{
    //if (isSpecialNpc())
    //	SceneNpcManager::getMe().removeSpecialNpc(this);
    clearMe = true;

	if (isMainGeneral())
		scene->bossMap.erase(COUNTRY_MAIN_GEN);
	if (scene->bossMap[id]==this)
		scene->bossMap.erase(id);
    //Zebra::logger->trace("npc %s(%u)", name, tempid);
}

/**
 * \brief npcǷҪ
 *
 * \return ǷҪ
 */
bool SceneNpc::needClear()
{
    return clearMe;
}

/**
 * \brief жǷnpc
 *
 * \return Ƿnpc
 */
bool SceneNpc::isTaskNpc()
{
    if(npc->kind == NPC_TYPE_TRADE 
	    || npc->kind == NPC_TYPE_TASK 
	    || npc->kind == NPC_TYPE_LIVENPC)
    {
	return true;
    }
    return false;
}
/**
 * \brief жǷǹnpc
 *
 * \return Ƿǹnpc
 */
bool SceneNpc::isFunctionNpc()
{
    if(npc->kind == NPC_TYPE_ROADSIGN 
	    || npc->kind == NPC_TYPE_TRADE 
	    || npc->kind == NPC_TYPE_TASK 
	    || npc->kind == NPC_TYPE_MOBILETRADE
	    || npc->kind == NPC_TYPE_MAILBOX
	    || npc->kind == NPC_TYPE_AUCTION)
    {
	return true;
    }
    return false;
}
/**
 * \brief жǷnpc
 *
 * npcbossй̶űnpc
 * \return Ƿnpc
 */
bool SceneNpc::isSpecialNpc()
{
	if (Cmd::PET_TYPE_NOTPET!=getPetType())
	{
		SceneEntryPk *p = getTopMaster();
		if (0==p) return false;
		if (p==this) return false;
		if (p->getType() == zSceneEntry::SceneEntry_Player) return true;
		if (p->getType() == zSceneEntry::SceneEntry_NPC) return ((SceneNpc *)p)->isSpecialNpc();
	}
	if (AIC->isActive()) return true;
	if (NPC_TYPE_BBOSS==npc->kind || NPC_TYPE_PBOSS==npc->kind || NPC_TYPE_REDBOSS==npc->kind) return true;
	return false;
}

/**
 * \brief õٶȱ
 *
 * \return ٶȱ
 */
float SceneNpc::getSpeedRate()
{
    return speedRate;
}

/**
 * \brief ı䲢ˢ½ɫ
 * \author fqnewman
 */
void  SceneNpc::changeAndRefreshHMS(bool lock, bool sendData)
{
    this->resetSpeedRate();
    this->reSendData = false;
}

/**
 * \brief ùĿ
 * 
 *
 * \param target Ŀ
 * \param force ǿĿ
 * \return Ƿɹ
 */
bool SceneNpc::setCurTarget(SceneEntryPk * target, bool force)
{
	if (!target) return false;
	if (!canFight()) return false;
	if (!canReach(target)) return false;
	if (curTargetID!=0 && !force)
	{
		if (lockTarget||aif&AIF_LOCK_TARGET) return false;
		return forceChaseUser(target);
	}

	if (aif&AIF_CALL_FELLOW_7 || aif&AIF_CALL_FELLOW_9)
		AIC->on_find_enemy(target);

	if (SceneEntryPk::setCurTarget(target))
	{
		closeCount = 0;
		return true;
	}
	return false;
}

/**
 * \brief ùĿ
 *
 *
 * \param type Ŀ
 * \param id ĿʱID
 * \param force ǿĿ
 * \return Ƿɹ
 */
bool SceneNpc::setCurTarget(DWORD id, DWORD type, bool force)
{
	//if (!canFight()) return false;
	SceneEntryPk * target = 0;
	switch (type)
	{
		case zSceneEntry::SceneEntry_Player:
			{
				target = scene->getUserByTempID(id);
				break;
			}
		case zSceneEntry::SceneEntry_NPC:
			{
				target = SceneNpcManager::getMe().getNpcByTempID(id);
				break;
			}
		default:
			return false;
	} 
	return setCurTarget(target);
}

/**
 * \brief л
 *
 * \param id ID
 * \param type 
 * \param standTime npcĳʱ
 * \param sid űID
 * \param petName ָ
 * \param anpcid ǿĸnpcID
 *
 * \return óڣسָ룬ΪNULL
 */
ScenePet * SceneNpc::summonPet(DWORD id, Cmd::petType type, DWORD standTime, DWORD sid, const char * petName, DWORD anpcid, zPos pos, BYTE vdir)
{
	if ((Cmd::PET_TYPE_PET>type)||(Cmd::PET_TYPE_SEMI<type))
	{
		Zebra::logger->trace("SceneNpc::summonPet(): %s ٻδ֪͵ĳ type=%d", name, type);
		return 0;
	}

	zNpcB *base = npcbm.get(id);
	zNpcB *abase = NULL;
	if (anpcid>0) abase = npcbm.get(anpcid);
	if (NULL == base) return 0;

	t_NpcDefine define;
	//	zPos pos = getPos();
	define.id = base->id;
	strncpy(define.name, base->name, MAX_NAMESIZE-1);
	define.pos = getPos();
	define.num = 1;
	define.interval = 5;
	define.initstate = zSceneEntry::SceneEntry_Normal;
	define.width = SceneUser::CALL_PET_REGION;
	define.height = SceneUser::CALL_PET_REGION;
	define.pos -= zPos(SceneUser::CALL_PET_REGION/2,SceneUser::CALL_PET_REGION/2);
	define.scriptID = sid;
	scene->initRegion(define.region, define.pos, define.width, define.height);

	ScenePet * newPet = scene->summonOneNpc<ScenePet>(define, pos, base, standTime, abase, vdir);

	if (newPet)
	{
		/*
		   char n[MAX_NAMESIZE];
		   bzero(n, MAX_NAMESIZE);
		   if (0==strncmp(petName, "", MAX_NAMESIZE))
		   snprintf(n, MAX_NAMESIZE-1, "(%s)%s", name, newPet->name);
		   else
		   snprintf(n, MAX_NAMESIZE-1, "(%s)%s", name, petName);
		   strncpy(newPet->name, , MAX_NAMESIZE-1);
		   */

		newPet->setMaster(this);
		newPet->setPetType(type);
		newPet->setDir(vdir);
		if (isSpecialNpc())
			SceneNpcManager::getMe().addSpecialNpc(newPet);

		using namespace Cmd;
		switch (type)
		{
			case PET_TYPE_PET:
				{
					if (pet)
					{
						pet->death(SceneTimeTick::currentTime);
						killOnePet(pet);
					}
					if (summon)
					{
						summon->death(SceneTimeTick::currentTime);
						killOnePet(summon);
					}
					pet = newPet;
				}
				break;
			case PET_TYPE_SUMMON:
				{
					if (pet)
					{
						pet->death(SceneTimeTick::currentTime);
						killOnePet(pet);
					}
					if (summon)
					{
						summon->death(SceneTimeTick::currentTime);
						killOnePet(summon);
					}
					summon = newPet;
				}
				break;
			case PET_TYPE_TOTEM:
#ifdef _XWL_DEBUG
				//Zebra::logger->debug("SceneNpc::summonPet(): lock %s", name);
#endif
				totems.push_back(newPet);
#ifdef _XWL_DEBUG
				//Zebra::logger->debug("SceneNpc::summonPet(): unlock %s", name);
#endif
				break;
			case PET_TYPE_SEMI:
				semipetList.push_back(newPet);
				break;
			default:
				Zebra::logger->trace("SceneNpc::summonPet(): δ֪ĳ %d", type);
				break;
		}

		Zebra::logger->debug("%s ӳ %s  %d", name, newPet->name, type);
	}

	return newPet;
}


/**
 * \brief ɾһ
 *
 */
bool SceneNpc::killOnePet(ScenePet * kill)
{
	if (!kill) return false;
	if (kill->getMaster()!=this)
	{
		Zebra::logger->error("[]%s(%u) NPC %s(%u) ĳkill->getMaster()=%u", kill->name, kill->tempid, name, tempid, kill->getMaster());
		return false;
	}

	using namespace Cmd;

#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::killOnePet(): lock %s", name);
#endif
	switch (kill->getPetType())
	{
		case PET_TYPE_PET:
			{
				pet = 0;
			}
			break;
		case PET_TYPE_SUMMON:
			{
				summon = 0;
			}
			break;
		case PET_TYPE_TOTEM:
			{
				totems.remove(kill);
			}
			break;
		case PET_TYPE_SEMI:
			{
				semipetList.remove(kill);
			}
			break;
		default:
			break;
	}

	kill->clearMaster();

	return true;
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::killOnePet(): unlock %s", name);
#endif
}

/**
 * \brief ɾг
 *
 */
void SceneNpc::killAllPets()
{
	//ɾг
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::killAllPets(): lock %s type=%u", name, getPetType());
#endif
	std::list<ScenePet *> copy(totems);
	for (std::list<ScenePet *>::iterator it=copy.begin(); it!=copy.end(); it++)
	{
		(*it)->skillStatusM.clearActiveSkillStatus();

		(*it)->killAllPets();
		//petDeath();
		(*it)->leaveBattle();
		(*it)->scene->clearBlock((*it)->getPos());
		(*it)->setState(SceneEntry_Death);

		Cmd::stNpcDeathUserCmd death;
		death.dwNpcTempID = (*it)->tempid;
		(*it)->scene->sendCmdToNine((*it)->getPosI() , &death , sizeof(death));

		(*it)->clearMaster();
	}
	totems.clear();
	copy.clear();
	copy = semipetList;
	for (std::list<ScenePet *>::iterator it=copy.begin(); it!=copy.end(); it++)
	{
		(*it)->skillStatusM.clearActiveSkillStatus();

		(*it)->killAllPets();
		(*it)->leaveBattle();
		(*it)->scene->clearBlock((*it)->getPos());
		(*it)->setState(SceneEntry_Death);

		Cmd::stNpcDeathUserCmd death;
		death.dwNpcTempID = (*it)->tempid;
		(*it)->scene->sendCmdToNine((*it)->getPosI() , &death , sizeof(death));

		(*it)->clearMaster();
	}
	semipetList.clear();
#ifdef _XWL_DEBUG
	//Zebra::logger->debug("SceneNpc::killAllPets(): unlock %s type=%u", name, getPetType());
#endif

	if (pet)
	{
		pet->skillStatusM.clearActiveSkillStatus();

		pet->killAllPets();
		//petDeath();
		pet->leaveBattle();
		pet->scene->clearBlock(pet->getPos());
		pet->setState(SceneEntry_Death);

		Cmd::stNpcDeathUserCmd death;
		death.dwNpcTempID = pet->tempid;
		pet->scene->sendCmdToNine(pet->getPosI() , &death , sizeof(death));

		pet->clearMaster();
		pet = 0;

	}
	if (summon)
	{
		summon->skillStatusM.clearActiveSkillStatus();

		summon->killAllPets();
		//petDeath();
		summon->leaveBattle();
		summon->scene->clearBlock(summon->getPos());
		summon->setState(SceneEntry_Death);

		Cmd::stNpcDeathUserCmd death;
		death.dwNpcTempID = summon->tempid;
		summon->scene->sendCmdToNine(summon->getPosI() , &death , sizeof(death));

		summon->clearMaster();
		summon = 0;

	}
}

/**
 * \brief Ƿ˻Ѫʱ
 *
 *
 * \param ct ڱȽϵʱ
 * \return Ƿʱ
 */
bool SceneNpc::checkRecoverTime(const zRTime& ct)
{
    return ct>=rcvTimePet;
}

/**
 * \brief ´λѪʱ
 *
 * \param ct ӳٿʼʱ
 * \param delay ӳ
 */
void SceneNpc::setRecoverTime(const zRTime& ct, int delay)
{
    rcvTimePet = ct;
    rcvTimePet.addDelay(delay);
}

/**
 * \brief ˹฽ӹ
 * \param mindamage С
 * \param maxdamage 󹥻
 */
void SceneNpc::setAppendDamage(DWORD mindamage, DWORD maxdamage)
{
    appendMinDamage = mindamage;
    appendMaxDamage = maxdamage;
}

/**
 * \brief ȡС
 * \return С
 */
// uint64_t SceneNpc::getMinMDamage()
// {
//     uint64_t value = (uint64_t)(this->npc->mdamage+appendMinDamage+skillValue.uppetdamage+skillValue.theurgy_umdam-skillValue.dmdam-skillValue.theurgy_dmdam);
//     if (value <0) value =0;
//     return value;

// }

// /**
//  * \brief ȡ
//  * \return 
//  */
// uint64_t SceneNpc::getMaxMDamage() 
// {
//     uint64_t value = (uint64_t)(this->npc->maxmdamage+appendMaxDamage+skillValue.uppetdamage+skillValue.theurgy_umdam-skillValue.dmdam-skillValue.theurgy_dmdam);
//     if (value <0) value =0;

//     return value;
// }

// /**
//  * \brief ȡС
//  * \return С
//  */
// uint64_t SceneNpc::getMinPDamage()
// {
//     uint64_t value = (uint64_t)(this->npc->damage+appendMinDamage+skillValue.uppetdamage+skillValue.theurgy_updam-skillValue.dpdam-skillValue.theurgy_dpdam);
//     if (value <0) value =0;
//     return value;
// }

// /**
//  * \brief ȡ
//  * \return 
//  */
// uint64_t SceneNpc::getMaxPDamage()
// {
//     uint64_t value = (uint64_t)(this->npc->maxdamage+appendMaxDamage+skillValue.uppetdamage+skillValue.theurgy_updam-skillValue.dpdam-skillValue.theurgy_dpdam);
//     if (value <0) value =0;
//     return value;
// }
/**
 * \brief ȡС
 * \return С
 */
uint64_t SceneNpc::getMinMDamage()
{
    uint64_t positive = this->npc->mdamage+appendMinDamage+skillValue.uppetdamage+skillValue.theurgy_umdam;
    uint64_t negative = skillValue.dmdam+skillValue.theurgy_dmdam;
    uint64_t value = (positive >= negative) ? (positive - negative) : 0;  //by=>friday uint64_t
    return value;
}

/**
 * \brief ȡ
 * \return 
 */
uint64_t SceneNpc::getMaxMDamage() 
{
    uint64_t positive = this->npc->maxmdamage+appendMaxDamage+skillValue.uppetdamage+skillValue.theurgy_umdam;
    uint64_t negative = skillValue.dmdam+skillValue.theurgy_dmdam;
    uint64_t value = (positive >= negative) ? (positive - negative) : 0;  //by=>friday uint64_t
    return value;
}

/**
 * \brief ȡС
 * \return С
 */
uint64_t SceneNpc::getMinPDamage()
{
    uint64_t positive = this->npc->damage+appendMinDamage+skillValue.uppetdamage+skillValue.theurgy_updam;
    uint64_t negative = skillValue.dpdam+skillValue.theurgy_dpdam;
    uint64_t value = (positive >= negative) ? (positive - negative) : 0;  //by=>friday uint64_t
    return value;
}

/**
 * \brief ȡ
 * \return 
 */
uint64_t SceneNpc::getMaxPDamage()
{
    uint64_t positive = this->npc->maxdamage+appendMaxDamage+skillValue.uppetdamage+skillValue.theurgy_updam;
    uint64_t negative = skillValue.dpdam+skillValue.theurgy_dpdam;
    uint64_t value = (positive >= negative) ? (positive - negative) : 0;  //by=>friday uint64_t
    return value;
}
/**
 * \brief ȡС
 * \return С
 */
// uint64_t SceneNpc::getMinMDefence()
// {
//     uint64_t value = this->npc->mdefence+skillValue.uppetdefence+skillValue.umdef+skillValue.theurgy_umdef - skillValue.theurgy_dmdef;
//     value = (uint64_t)(value *( 1 - skillValue.dmdefp/100.0f));
//     if (value < 0) value = 0;
//     return value;
// }
uint64_t SceneNpc::getMinMDefence()
{
    uint64_t base = this->npc->mdefence+skillValue.uppetdefence+skillValue.umdef+skillValue.theurgy_umdef;
    uint64_t reduce = skillValue.theurgy_dmdef;
    uint64_t value = (base >= reduce) ? (base - reduce) : 0;  //by=>friday uint64_t
    value = (uint64_t)(value * (1 - skillValue.dmdefp/100.0f));
    return value;
}
/**
 * \brief ȡ
 * \return 
 */
// uint64_t SceneNpc::getMaxMDefence() 
// {
//     uint64_t value = this->npc->maxmdefence+skillValue.uppetdefence+skillValue.umdef+skillValue.theurgy_umdef - skillValue.theurgy_dmdef;
//     value = (uint64_t)(value *( 1 - skillValue.dmdefp/100.0f));
//     if (value < 0) value = 0;
//     return value;
// }
uint64_t SceneNpc::getMaxMDefence() 
{
    uint64_t base = this->npc->maxmdefence+skillValue.uppetdefence+skillValue.umdef+skillValue.theurgy_umdef;
    uint64_t reduce = skillValue.theurgy_dmdef;
    uint64_t value = (base >= reduce) ? (base - reduce) : 0;  //by=>friday uint64_t
    value = (uint64_t)(value * (1 - skillValue.dmdefp/100.0f));
    return value;
}
/**
 * \brief ȡС
 * \return С
 */
// uint64_t SceneNpc::getMinPDefence()
// {
//     uint64_t value = this->npc->pdefence+skillValue.uppetdefence+skillValue.updef+skillValue.theurgy_updef - skillValue.theurgy_dpdef;
//     if (value < 0) value = 0;
//     return value;
// }
uint64_t SceneNpc::getMinPDefence()
{
    uint64_t base = this->npc->pdefence+skillValue.uppetdefence+skillValue.updef+skillValue.theurgy_updef;
    uint64_t reduce = skillValue.theurgy_dpdef;
    uint64_t value = (base >= reduce) ? (base - reduce) : 0;  //by=>friday uint64_t
    return value;
}

/**
 * \brief ȡ
 * \return 
 */
// uint64_t SceneNpc::getMaxPDefence()
// {
//     uint64_t value = this->npc->maxpdefence+skillValue.uppetdefence+skillValue.updef+skillValue.theurgy_updef - skillValue.theurgy_dpdef;
//     if (value < 0) value = 0;
//     return value;
// }
uint64_t SceneNpc::getMaxPDefence()
{
    uint64_t base = this->npc->maxpdefence+skillValue.uppetdefence+skillValue.updef+skillValue.theurgy_updef;
    uint64_t reduce = skillValue.theurgy_dpdef;
    uint64_t value = (base >= reduce) ? (base - reduce) : 0;  //by=>friday uint64_t
    return value;
}
/**
 * \brief hp
 * \author fqnewman
 * \return ֵ
 */
uint64_t SceneNpc::getMaxHP()
{
#ifdef _XWL_DEBUG
	if (boostupHpMaxP)
		Zebra::logger->debug("boostupHpMaxP=%u", boostupHpMaxP);
#endif
    return (uint64_t)((npc->hp+(anpc?anpc->hp:0)+skillValue.maxhp)*(1.0f+(float)(boostupHpMaxP/100.0f)));;
}

/**
 * \brief hp
 * \author fqnewman
 * \return ֵ
 */
uint64_t SceneNpc::getBaseMaxHP()
{
    return npc->hp+(anpc?anpc->hp:0);
}

/**
 * \brief 9Լ
 *
 */
void SceneNpc::sendMeToNine()
{
	if (npc->kind == NPC_TYPE_TRAP)
	{
		SceneEntryPk *entry = getMaster();
		if (entry && entry->getType() == zSceneEntry::SceneEntry_Player)
		{
			SceneUser *pUser = (SceneUser *)entry;
		BUFFER_CMD(Cmd::stAddMapNpcAndPosMapScreenStateUserCmd ,send , zSocket::MAX_USERDATASIZE);
		this->full_t_MapNpcDataAndPosState(send->data);
			pUser->sendCmdToMe(send, send->size());
		}		
	}
	else
	{
		BUFFER_CMD(Cmd::stAddMapNpcAndPosMapScreenStateUserCmd ,send , zSocket::MAX_USERDATASIZE);
		this->full_t_MapNpcDataAndPosState(send->data);
		this->scene->sendCmdToNine(getPosI(),send,send->size());
	}
}

/**
 * \brief npcѪ
 *
 */
bool SceneNpc::recover()
{
	if (!npc->recover.num) return 0;

	bool ret = false;
	uint64_t tMaxHP = getMaxHP();

	if (!needRecover && (npc->recover.start*tMaxHP/100 > hp))
		needRecover = true;
	if (!needRecover) return 0;

	if (_3_sec(SceneTimeTick::currentTime))
	{
		if (npc->recover.type==1)//ٷֱȻ
			hp += tMaxHP*npc->recover.num/100;
		else if (npc->recover.type==2)//
	//		hp += npc->recover.num;
	        hp += npc->recover.num/10; //soke npcѪ/10

		ret = true;
	}

	if (hp>tMaxHP) hp = tMaxHP;
	if (hp==tMaxHP)
		needRecover = false;

	return ret;
}

/**
 * \brief ս 
 *
 */
void SceneNpc::leaveBattle()
{
	if (aif&AIF_RCV_REST)//սѪ
	{
		rcvTimeRest.now();
		rcvTimeRest.addDelay(30*1000);
	}

	clearDefTarget();
	lockTarget = false;
	SceneEntryPk::leaveBattle();
//#ifdef _XWL_DEBUG
//	Zebra::logger->debug("%s ս", name);
//#endif
}

/**
 * \brief õڶĿ
 * \param target Ŀָ
 * \return Ƿɹ
 */
bool SceneNpc::setSecondTarget(SceneEntryPk * target)
{
	if (!target) return false;
	if (!canFight()) return false;
	if (curTargetType==(DWORD)target->getType() && curTargetID==target->tempid) return false;
	if (!canReach(target)) return false;

	if (target==this) return false;//Լ
	if (!isEnemy(target)) return false;//

	secondTargetType = target->getType();
	secondTargetID = target->tempid;
#ifdef _XWL_DEBUG
	//	Zebra::logger->debug("%s õڶĿ id=%u type=%u", name, secondTargetID, secondTargetType);
#endif
	return true;
}

/**
 * \brief õڶĿ
 * \param id ʱid
 * \param type 
 * \return Ƿɹ
 */
bool SceneNpc::setSecondTarget(DWORD id, DWORD type)
{
	SceneEntryPk * def = NULL;
	if (zSceneEntry::SceneEntry_Player==type)
		def = scene->getUserByTempID(tempid);
	else if (zSceneEntry::SceneEntry_NPC==type)
		def = SceneNpcManager::getMe().getNpcByTempID(tempid);

	if (def)
		return setSecondTarget(def);
	else
		return false;
}

/**
 * \brief õιĿ
 *
 * \return ιĿ
 * 
 */
SceneEntryPk * SceneNpc::getSecondTarget()
{
	switch (secondTargetType)
	{
		case zSceneEntry::SceneEntry_Player:
			return scene->getUserByTempID(secondTargetID);
		case zSceneEntry::SceneEntry_NPC:
			return SceneNpcManager::getMe().getNpcByTempID(secondTargetID);
		default:
			return 0;
	}     
}

/**
 * \brief õڶĿΪǰĿ
 *
 * \return Ƿɹ
 * 
 */
bool SceneNpc::chaseSecondTarget()
{
    if (secondTargetID==0) return false;
    //setCurTarget(secondTargetID, secondTargetType);
    curTargetID = secondTargetID;
    curTargetType = secondTargetType;
#ifdef _XWL_DEBUG
    //Zebra::logger->debug("%s ڶĿ id=%u type=%u", name, secondTargetID, secondTargetType);
#endif
    secondTargetID = 0;
    secondTargetType = 0;
    return true;
}

/**
 * \brief õǰĿ˵ǰĿ
 *
 * \return Ƿɹ
 * 
 */
bool SceneNpc::chaseItsMaster()
{
    SceneEntryPk * c = getCurTarget();
    if (!c) return false;

    SceneEntryPk * m = c->getTopMaster();
    if (!m || m==c) return false;

#ifdef _XWL_DEBUG
    //Zebra::logger->debug("%s  id=%u type=%u", name, m->tempid, m->getType());
#endif
    return setCurTarget(m, true);
}

void SceneNpc::goToRandomScreen()
{
	zPos randPos;
	if(this->scene->randzPosOnRect(getPos() , randPos , SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		warp(randPos);
	}
}

//soke Ǵ󽫾ȱ10%Ѫ
bool SceneNpc::isMainGeneral()
{
	return (id>=58200 && id<58300 && 0==id%10);
}

/**
 * \brief npcMapٻnpcHP60%¡ٻnpc
 *
 * \return ٻ
 */
int SceneNpc::summonByNpcMap(std::map<DWORD, std::pair<DWORD, DWORD> > map)
{
	unsigned int count = 0;
	for (std::map<DWORD, std::pair<DWORD, DWORD> >::iterator it=map.begin(); it!=map.end(); it++)
	{
		zNpcB *base = npcbm.get(it->first);

		if (base)
		{
			t_NpcDefine define;
			//const zPos &pos = getPos();
			define.id = base->id;
			strncpy(define.name, base->name, MAX_NAMESIZE-1);
			define.pos = getPos();
			define.num = 1;
			define.interval = 5;
			define.initstate = zSceneEntry::SceneEntry_Normal;
			define.width = 10;
			define.height = 10;
			define.pos -= zPos(5,5);
			define.scriptID = 0;
			scene->initRegion(define.region, define.pos, define.width, define.height);

			for (unsigned int i=0; i<it->second.first; i++)
			{
				if (zMisc::selectByPercent(it->second.second))
				{
					SceneNpc * newPet = scene->summonOneNpc<SceneNpc>(define, zPos(0, 0), base, 0, 0);    
					if (newPet)
						count++;
				}
			}
		}
	}
	return count;
}

	/**
	 * \brief Ļڶƶ
	 * \author fqnewman
	 */
	void SceneNpc::jumpTo(zPos &newPos)
	{
		warp(newPos);
	}
