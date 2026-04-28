#include "Zebra.h"
#include "zDatabaseManager.h"

template <class data,class datafile>
zDataBM<data, datafile> *zDataBM<data, datafile>::me(NULL);

zDataBM<zObjectB,ObjectBase> &objectbm=zDataBM<zObjectB,ObjectBase>::getMe();
zDataBM<zBlueObjectB,BlueObjectBase> &blueobjectbm=zDataBM<zBlueObjectB,BlueObjectBase>::getMe();
zDataBM<zGoldObjectB,GoldObjectBase> &goldobjectbm=zDataBM<zGoldObjectB,GoldObjectBase>::getMe();
zDataBM<zDropGoldObjectB,DropGoldObjectBase> &dropgoldobjectbm=zDataBM<zDropGoldObjectB,DropGoldObjectBase>::getMe();
zDataBM<zZhuanGoldObjectB,ZhuanGoldObjectBase> &zhuangoldobjectbm=zDataBM<zZhuanGoldObjectB,ZhuanGoldObjectBase>::getMe();
zDataBM<zSetObjectB,SetObjectBase> &setobjectbm=zDataBM<zSetObjectB,SetObjectBase>::getMe();
zDataBM<zFiveSetB, FiveSetBase> &fivesetbm=zDataBM<zFiveSetB, FiveSetBase>::getMe();
zDataBM<zHolyObjectB,HolyObjectBase> &holyobjectbm=zDataBM<zHolyObjectB,HolyObjectBase>::getMe();
zDataBM<zUpgradeObjectB,UpgradeObjectBase> &upgradeobjectbm=zDataBM<zUpgradeObjectB,UpgradeObjectBase>::getMe();
zDataBM<zUpgradeObject2B,UpgradeObject2Base> &upgradeobject2bm=zDataBM<zUpgradeObject2B,UpgradeObject2Base>::getMe();
zDataBM<zUpgradePurpleB,UpgradePurpleBase> &upgradepurplebm=zDataBM<zUpgradePurpleB,UpgradePurpleBase>::getMe();
zDataBM<zAmuletObjectB,AmuletObjectBase> &amuletobjectbm=zDataBM<zAmuletObjectB,AmuletObjectBase>::getMe();
zDataBM<zUdouhunObjectB,UdouhunObjectBase> &udouhunobjectbm=zDataBM<zUdouhunObjectB,UdouhunObjectBase>::getMe();
zDataBM<zUbutianObjectB,UbutianObjectBase> &ubutianobjectbm=zDataBM<zUbutianObjectB,UbutianObjectBase>::getMe();
zDataBM<zUdtroughObjectB, UdtroughObjectBase> &udtroughobjectbm = zDataBM<zUdtroughObjectB, UdtroughObjectBase>::getMe();
zDataBM<zUdrastarObjectB,UdrastarObjectBase> &udrastarobjectbm=zDataBM<zUdrastarObjectB,UdrastarObjectBase>::getMe();
zDataBM<zMedaltObjectB,MedaltObjectBase> &medaltobjectbm=zDataBM<zMedaltObjectB,MedaltObjectBase>::getMe();
zDataBM<zUhbqlObjectB,UhbqlObjectBase> &uhbqlobjectbm=zDataBM<zUhbqlObjectB,UhbqlObjectBase>::getMe();
zDataBM<zUshenqiObjectB,UshenqiObjectBase> &ushenqiobjectbm=zDataBM<zUshenqiObjectB,UshenqiObjectBase>::getMe();
zDataBM<zUfabaoObjectB,UfabaoObjectBase> &ufabaoobjectbm=zDataBM<zUfabaoObjectB,UfabaoObjectBase>::getMe();
zDataBM<zUYUANSHENObjectB,UYUANSHENObjectBase> &uYUANSHENobjectbm=zDataBM<zUYUANSHENObjectB,UYUANSHENObjectBase>::getMe();
zDataBM<zUYSBSObjectB,UYSBSObjectBase> &uYSBSobjectbm=zDataBM<zUYSBSObjectB,UYSBSObjectBase>::getMe();
zDataBM<zUSEXXObjectB,USEXXObjectBase> &usexxobjectbm=zDataBM<zUSEXXObjectB,USEXXObjectBase>::getMe();
zDataBM<zUrefineObjectB,UrefineObjectBase> &urefineobjectbm=zDataBM<zUrefineObjectB,UrefineObjectBase>::getMe();
zDataBM<zUbrightObjectB,UbrightObjectBase> &ubrightobjectbm=zDataBM<zUbrightObjectB,UbrightObjectBase>::getMe();
zDataBM<zUangelObjectB,UangelObjectBase> &uangelobjectbm=zDataBM<zUangelObjectB,UangelObjectBase>::getMe();
zDataBM<zMythicalObjectB,MythicalObjectBase> &mythicalobjectbm=zDataBM<zMythicalObjectB,MythicalObjectBase>::getMe();
zDataBM<zEquipObjectB,EquipObjectBase> &equipobjectbm=zDataBM<zEquipObjectB,EquipObjectBase>::getMe();
zDataBM<zMagicalObjectB,MagicalObjectBase> &magicalobjectbm=zDataBM<zMagicalObjectB,MagicalObjectBase>::getMe();
zDataBM<zNpcB,NpcBase> &npcbm=zDataBM<zNpcB,NpcBase>::getMe();
//zDataBM<zCharacterB , CharacterBase> &characterbm = zDataBM<zCharacterB , CharacterBase>::getMe();
zDataBM<zExperienceB , ExperienceBase> &experiencebm = zDataBM<zExperienceB , ExperienceBase>::getMe();
zDataBM<zZhuanExperienceB , ZhuanExperienceBase> &zhuanexperiencebm = zDataBM<zZhuanExperienceB , ZhuanExperienceBase>::getMe();
zDataBM<zHonorB , HonorBase> &honorbm = zDataBM<zHonorB , HonorBase>::getMe();
zDataBM<zSkillB , SkillBase> &skillbm = zDataBM<zSkillB , SkillBase>::getMe();
zDataBM<zLiveSkillB , LiveSkillBase> &liveskillbm = zDataBM<zLiveSkillB , LiveSkillBase>::getMe();
zDataBM<zSoulStoneB , SoulStoneBase> &soulstonebm = zDataBM<zSoulStoneB , SoulStoneBase>::getMe();
zDataBM<zHairStyleB , HairStyle> &hairstylebm = zDataBM<zHairStyleB , HairStyle>::getMe();
zDataBM<zHairColourB , HairColour> &haircolourbm = zDataBM<zHairColourB , HairColour>::getMe();
zDataBM<zCountryMaterialB, CountryMaterial> &countrymaterialbm = zDataBM<zCountryMaterialB, CountryMaterial>::getMe();
zDataBM<zHeadListB , HeadList> &headlistbm = zDataBM<zHeadListB , HeadList>::getMe();
zDataBM<zPetB , PetBase> &petbm = zDataBM<zPetB , PetBase>::getMe();
//百兽图鉴进阶
zDataBM<zUhandbookObjectB,UhandbookObjectBase> &uhandbookobjectbm=zDataBM<zUhandbookObjectB,UhandbookObjectBase>::getMe();

zDataBM<zUlongwanglvObjectB,UlongwanglvObjectBase> &ulongwanglvobjectbm=zDataBM<zUlongwanglvObjectB,UlongwanglvObjectBase>::getMe();
zDataBM<zUnitianlvObjectB,UnitianlvObjectBase> &unitianlvobjectbm=zDataBM<zUnitianlvObjectB,UnitianlvObjectBase>::getMe();
zDataBM<zUdouzhansflvObjectB,UdouzhansflvObjectBase> &udouzhansflvobjectbm=zDataBM<zUdouzhansflvObjectB,UdouzhansflvObjectBase>::getMe();
zDataBM<zUaystonelvObjectB,UaystonelvObjectBase> &uaystonelvobjectbm=zDataBM<zUaystonelvObjectB,UaystonelvObjectBase>::getMe();
zDataBM<zUshangfanglvObjectB,UshangfanglvObjectBase> &ushangfanglvobjectbm=zDataBM<zUshangfanglvObjectB,UshangfanglvObjectBase>::getMe();
zDataBM<zUzijinlplvObjectB,UzijinlplvObjectBase> &uzijinlplvobjectbm=zDataBM<zUzijinlplvObjectB,UzijinlplvObjectBase>::getMe();
//龙凤吟
zDataBM<zUlongfengyingObjectB, UlongfengyingObjectBase> &ulongfengyingobjectbm = zDataBM<zUlongfengyingObjectB, UlongfengyingObjectBase>::getMe();
//神斧
zDataBM<zUxuanhuaObjectB, UxuanhuaObjectBase> &uxuanhuaobjectbm = zDataBM<zUxuanhuaObjectB, UxuanhuaObjectBase>::getMe();
//黑曜
zDataBM<zUheiyaoObjectB, UheiyaoObjectBase> &uheiyaoobjectbm = zDataBM<zUheiyaoObjectB, UheiyaoObjectBase>::getMe();
//精金
zDataBM<zUjingjinObjectB, UjingjinObjectBase> &ujingjinobjectbm = zDataBM<zUjingjinObjectB, UjingjinObjectBase>::getMe();
zDataBM<zUjgzqhObjectB,UjgzqhObjectBase> &ujgzqhobjectbm=zDataBM<zUjgzqhObjectB,UjgzqhObjectBase>::getMe();  //金箍咒强化
zDataBM<zUjgzzwObjectB,UjgzzwObjectBase> &ujgzzwobjectbm=zDataBM<zUjgzzwObjectB,UjgzzwObjectBase>::getMe();  //金箍咒字纹
zDataBM<zUjgzclObjectB,UjgzclObjectBase> &ujgzclobjectbm=zDataBM<zUjgzclObjectB,UjgzclObjectBase>::getMe();  //金箍咒淬炼
zDataBM<zUhbslObjectB,UhbslObjectBase> &uhbslobjectbm=zDataBM<zUhbslObjectB,UhbslObjectBase>::getMe();  //寒冰神炼
zDataBM<zUhbzwObjectB,UhbzwObjectBase> &uhbzwobjectbm=zDataBM<zUhbzwObjectB,UhbzwObjectBase>::getMe();  //寒冰字纹
zDataBM<zUhbwzObjectB,UhbwzObjectBase> &uhbwzobjectbm=zDataBM<zUhbwzObjectB,UhbwzObjectBase>::getMe();  //寒冰纹章
bool loadAllBM()
{
	//Zebra::logger->debug("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

	//基本表
	if(!npcbm.refresh((Zebra::global["tbldir"] + "/NpcBase_New.tbl").c_str())) return false;
	if(!objectbm.refresh((Zebra::global["tbldir"] + "/ObjectBase.tbl").c_str())) return false;
	if(!fivesetbm.refresh((Zebra::global["tbldir"] + "/FiveSetBase.tbl").c_str())) return false; 
	if(!setobjectbm.refresh((Zebra::global["tbldir"] + "/SetObjectBase.tbl").c_str())) return false;	
	if(!holyobjectbm.refresh((Zebra::global["tbldir"] + "/HolyObjectBase.tbl").c_str())) return false;
	if(!experiencebm.refresh((Zebra::global["tbldir"] + "/ExperienceBase.tbl").c_str())) return false;      //非转身经验
	if(!zhuanexperiencebm.refresh((Zebra::global["tbldir"] + "/ZhuanExperienceBase.tbl").c_str())) return false;   //转生经验

	if(!honorbm.refresh((Zebra::global["tbldir"] + "/HonorBase.tbl").c_str())) return false;
    if(!skillbm.refresh((Zebra::global["tbldir"] + "/SkillBase.tbl").c_str())) return false;
    if(!liveskillbm.refresh((Zebra::global["tbldir"] + "/WorkSkillBase.tbl").c_str())) return false;
    if(!soulstonebm.refresh((Zebra::global["tbldir"] + "/SoulStoneBase.tbl").c_str())) return false;
	if(!hairstylebm.refresh((Zebra::global["tbldir"] + "/HairStyle.tbl").c_str())) return false;
	if(!haircolourbm.refresh((Zebra::global["tbldir"] + "/HairColour.tbl").c_str())) return false;
	if(!countrymaterialbm.refresh((Zebra::global["tbldir"] + "/CountryMaterial.tbl").c_str())) return false;
	if(!headlistbm.refresh((Zebra::global["tbldir"] + "/HeadListBase.tbl").c_str())) return false;
	if(!petbm.refresh((Zebra::global["tbldir"] + "/PetBase.tbl").c_str())) return false;
 
	//新的升级装备
    if(!equipobjectbm.refresh((Zebra::global["tbldir"] + "/EquipObjectBase.tbl").c_str())) return false; //装备升级


	//神圣标识基本表
    if(!blueobjectbm.refresh((Zebra::global["tbldir"] + "/BlueObjectBase.tbl").c_str())) return false;
	if(!goldobjectbm.refresh((Zebra::global["tbldir"] + "/GoldObjectBase.tbl").c_str())) return false;
	if(!dropgoldobjectbm.refresh((Zebra::global["tbldir"] + "/DropGoldObjectBase.tbl").c_str())) return false;
	if(!zhuangoldobjectbm.refresh((Zebra::global["tbldir"] + "/ZhuanGoldObjectBase.tbl").c_str())) return false;


	//装备升星基本表
	if(!upgradeobjectbm.refresh((Zebra::global["tbldir"] + "/UpgradeObjectBase.tbl").c_str())) return false;
	if(!upgradeobject2bm.refresh((Zebra::global["tbldir"] + "/UpgradeObjectBase.tbl").c_str())) return false;
	if(!upgradepurplebm.refresh((Zebra::global["tbldir"] + "/UpgradePurpleBase.tbl").c_str())) return false;
	if(!udtroughobjectbm.refresh((Zebra::global["tbldir"] + "/DragonjxObjectBase.tbl").c_str())) return false; //龙星激活 DtroughObjectBase.tbl
	if(!udrastarobjectbm.refresh((Zebra::global["tbldir"] + "/LonestarObjectBase.tbl").c_str())) return false; //龙星升级	


	//附件道具基本表
    if(!amuletobjectbm.refresh((Zebra::global["tbldir"] + "/AmuletObjectBase.tbl").c_str())) return false; //护身符
	if(!medaltobjectbm.refresh((Zebra::global["tbldir"] + "/MedalObjectBase.tbl").c_str())) return false;  //荣耀勋章
	if(!uhbqlobjectbm.refresh((Zebra::global["tbldir"] + "/HbqlObjectBase.tbl").c_str())) return false;    //寒冰麒麟
	if(!uangelobjectbm.refresh((Zebra::global["tbldir"] + "/AngelObjectBase.tbl").c_str())) return false;  //翅膀
	if(!magicalobjectbm.refresh((Zebra::global["tbldir"] + "/MagicalObjectBase.tbl").c_str())) return false;   //神兵升级	
	if(!mythicalobjectbm.refresh((Zebra::global["tbldir"] + "/MythicalObjectBase.tbl").c_str())) return false; //神兽
	if(!ushangfanglvobjectbm.refresh((Zebra::global["tbldir"] + "/ShangfanglvObjectBase.tbl").c_str())) return false;  //尚方宝剑升级
	if(!uzijinlplvobjectbm.refresh((Zebra::global["tbldir"] + "/ZijinlplvObjectBase.tbl").c_str())) return false;  //紫金龙袍升级
	if(!ufabaoobjectbm.refresh((Zebra::global["tbldir"] + "/FabaoObjectBase.tbl").c_str())) return false;//法宝
	if(!uYUANSHENobjectbm.refresh((Zebra::global["tbldir"] + "/YuanshenObjectBase.tbl").c_str())) return false;//元神
	if(!uYSBSobjectbm.refresh((Zebra::global["tbldir"] + "/YsbsObjectBase.tbl").c_str())) return false;//元神宝石
	if(!usexxobjectbm.refresh((Zebra::global["tbldir"] + "/SEXXObjectBase.tbl").c_str())) return false;//十二星宿
	
    //装备附加属性基本表
	if(!udouhunobjectbm.refresh((Zebra::global["tbldir"] + "/FightsoulObjectBase.tbl").c_str())) return false; //斗魂
    if(!ubutianobjectbm.refresh((Zebra::global["tbldir"] + "/StonefillObjectBase.tbl").c_str())) return false; //补天石
	if(!ushenqiobjectbm.refresh((Zebra::global["tbldir"] + "/ShenQiObjectBase.tbl").c_str())) return false;    //神器
	if(!urefineobjectbm.refresh((Zebra::global["tbldir"] + "/RefineObjectBase.tbl").c_str())) return false;  //马牌精炼升级
    if(!ubrightobjectbm.refresh((Zebra::global["tbldir"] + "/BrightObjectBase.tbl").c_str())) return false;  //璀璨宝石镶嵌
	if(!uhandbookobjectbm.refresh((Zebra::global["tbldir"] + "/HandbookObjectBase.tbl").c_str())) return false;  //图鉴进阶
	if(!ulongwanglvobjectbm.refresh((Zebra::global["tbldir"] + "/LongwanglvObjectBase.tbl").c_str())) return false;  //神龙镶嵌
	if(!unitianlvobjectbm.refresh((Zebra::global["tbldir"] + "/NitianlvObjectBase.tbl").c_str())) return false;  //逆天进阶
	if(!udouzhansflvobjectbm.refresh((Zebra::global["tbldir"] + "/DouzhansflvObjectBase.tbl").c_str())) return false;  //斗战圣佛镶嵌进阶
	if(!uaystonelvobjectbm.refresh((Zebra::global["tbldir"] + "/AystonelvObjectBase.tbl").c_str())) return false;  //暗影宝石镶嵌

	//龙凤吟
    if(!ulongfengyingobjectbm.refresh((Zebra::global["tbldir"] + "/lfyObjectBase.tbl").c_str())) return false; 
	//神斧
 	if(!uxuanhuaobjectbm.refresh((Zebra::global["tbldir"] + "/XuanhuaObjectBase.tbl").c_str())) return false; 
	//黑曜
 	if(!uheiyaoobjectbm.refresh((Zebra::global["tbldir"] + "/HeiyaoObjectBase.tbl").c_str())) return false; 
	//精金
 	if(!ujingjinobjectbm.refresh((Zebra::global["tbldir"] + "/JingjinObjectBase.tbl").c_str())) return false; 	

	 if(!ujgzqhobjectbm.refresh((Zebra::global["tbldir"] + "/JgzqhObjectBase.tbl").c_str())) return false;  //金箍咒强化
	 if(!ujgzzwobjectbm.refresh((Zebra::global["tbldir"] + "/JgzzwObjectBase.tbl").c_str())) return false;  //金箍咒字纹
	 if(!ujgzclobjectbm.refresh((Zebra::global["tbldir"] + "/JgzclObjectBase.tbl").c_str())) return false;  //金箍咒淬炼
	 if(!uhbslobjectbm.refresh((Zebra::global["tbldir"] + "/HbslObjectBase.tbl").c_str())) return false;  //寒冰神炼
	 if(!uhbzwobjectbm.refresh((Zebra::global["tbldir"] + "/HbzqObjectBase.tbl").c_str())) return false;  //寒冰字纹
	 if(!uhbwzobjectbm.refresh((Zebra::global["tbldir"] + "/HbwzObjectBase.tbl").c_str())) return false;  //寒冰纹章
	//npcbm.listAll();
	
	//if(!characterbm.refresh((Zebra::global["tbldir"] + "/CharacterBase.tbl").c_str())) return false;
	//characterbm.listAll();
	


		
	return true;

}


bool loadAllBM2()
{
	//Zebra::logger->debug("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

	//基本表
	if(!npcbm.refresh((Zebra::global["tbldir"] + "/NpcBase_New.tbl").c_str())) return false;
	if(!objectbm.refresh((Zebra::global["tbldir"] + "/ObjectBase.tbl").c_str())) return false;
	if(!fivesetbm.refresh((Zebra::global["tbldir"] + "/FiveSetBase.tbl").c_str())) return false; 
	if(!setobjectbm.refresh((Zebra::global["tbldir"] + "/SetObjectBase.tbl").c_str())) return false;	
	
	if(!skillbm.refresh((Zebra::global["tbldir"] + "/SkillBase.tbl").c_str())) return false;
    if(!soulstonebm.refresh((Zebra::global["tbldir"] + "/SoulStoneBase.tbl").c_str())) return false;
	
	

	//神圣标识基本表
    if(!blueobjectbm.refresh((Zebra::global["tbldir"] + "/BlueObjectBase.tbl").c_str())) return false;
	if(!goldobjectbm.refresh((Zebra::global["tbldir"] + "/GoldObjectBase.tbl").c_str())) return false;
	if(!dropgoldobjectbm.refresh((Zebra::global["tbldir"] + "/DropGoldObjectBase.tbl").c_str())) return false;
	if(!zhuangoldobjectbm.refresh((Zebra::global["tbldir"] + "/ZhuanGoldObjectBase.tbl").c_str())) return false;


	//装备升星基本表
	if(!upgradeobjectbm.refresh((Zebra::global["tbldir"] + "/UpgradeObjectBase.tbl").c_str())) return false;
	if(!upgradeobject2bm.refresh((Zebra::global["tbldir"] + "/UpgradeObjectBase.tbl").c_str())) return false;
	if(!upgradepurplebm.refresh((Zebra::global["tbldir"] + "/UpgradePurpleBase.tbl").c_str())) return false;
	
	
		
	return true;

}

void unloadAllBM()
{
}

