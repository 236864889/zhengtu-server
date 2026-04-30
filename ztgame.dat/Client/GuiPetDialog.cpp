/**
*\file		GuiPetDialog.cpp
*\version	$ID$
*\author	$$$04公司
*			$$$04公司@###.com
*\date		2005-07-06 19:58:00
*\brief	//todo
*
*\详细描述
*
*/


#include "public.h"
#include "./GameGuiManager.h"
#include "./GameScene.h"
#include ".\GuiPetDialog.h"
#include "./RoleItem.h"
#include "./MainCharacter.h"
#include "./guiuserinfo.h"
#include "./GuiZuoqi.h"
#include ".\GuiCartoonAddCentDialog.h"

#define scTabid					1
#define scHorseButtonid			2
#define scPetButtonid			3
#define scSummon				4
#define scCartoon				5

#define scFollowMasterid		20
#define scRemoveHorseid			21

#define sxRenamePetEditid		112
#define sxRenameSummonEditid	212

#define scRenamePetBtnid		113
#define scRenameSummonBtnid		213

#define scHorseImageid			14
#define scPetImageid			114
#define scSummonImageid			214
#define scCartoonImageid		310


#define scCartoonAddCentID		300
#define scCartoonPutoutID		301
#define scCartoonPreID			302
#define scCartoonStopTrainID	303
#define scCartoonNextID			304
#define scCartoonRenameID		305
#define scCartoonFeedID			306
#define scCartoonGetExpID		307
#define scCartoonPutinID		308
#define scCartoonStartTrainID	309

#ifdef _DEBUG
#define Pet_Trace(fun,id,b)		{TRACE("%s(%d,%d) line:%d\n",fun,id,b,__LINE__); }
#else
#define Pet_Trace(fun,id,b)		__noop
#endif

#define Pet_SetStaticText(id,txt)			{CGuiStatic *p##id = GetStatic(id);if(p##id) p##id->SetText(txt);}
#define Pet_SetStaticTextColor(id,color)	{CGuiStatic *p##id = GetStatic(id);if(p##id) p##id->SetTextColor(color);}
#define Pet_AddTabButton(id)				{CGuiButton *p##id = GetButton(id);if(p##id) p##id->SetButtonGroup(1);}
#define Pet_EnableCtrl(id,b)				{bool bRes = b; CGuiControl *p = GetControl(id);if(p) {p->SetEnabled(bRes);if(bRes){ p->SetVisible(bRes);}Pet_Trace("Pet_EnableCtrl",id,bRes);} }
#define Pet_ShowCtrl(id,b)					{bool bRes = b; CGuiControl *p = GetControl(id);if(p) {p->SetVisible(bRes);Pet_Trace("Pet_ShowCtrl",id,bRes);} }
#define Pet_ShowAndEnable(id,b)				{bool bRes = b; CGuiControl *p = GetControl(id);if(p) {p->SetVisible(bRes);p->SetEnabled(bRes);Pet_Trace("Pet_ShowAndEnable",id,bRes);}}
#define Pet_ShowAndEnableGrouped(id1,id2,b1,b2)	{	CGuiButton* p1 = GetButton( id1 );	CGuiButton* p2 = GetButton( id2 );			\
													bool bRes1 = b1;bool bRes2 = b2;												\
													if( p1 && p2 ){																	\
														if( !bRes1 && !bRes2 ){														\
															p1->SetEnabled(false);p1->SetVisible(true);								\
															p2->SetEnabled(false);p2->SetVisible(false);}							\
														else if( bRes1 ){															\
								p1->SetEnabled(false);p1->SetVisible(true);p2->SetVisible(false);p1->SetEnableDelay(true,1000);}	\
														else if( bRes2 ){															\
								p2->SetEnabled(false);p2->SetVisible(true);p1->SetVisible(false);p2->SetEnableDelay(true,1000);}}	\
													TRACE("Pet_ShowAndEnableGrouped(%d,%d,%d,%d) line:%d\n",id1,bRes1,id2,bRes2,__LINE__);}

#define Pet_DelayEnable(id,b)	{bool bRes = b; CGuiControl *p = GetControl(id);if(p) {p->SetEnabled(false);if(bRes){p->SetEnableDelay(true,1000);}Pet_Trace("Pet_DelayEnable",id,bRes);}}

const stRectI CGuiPetDialog::sNameRect(113,101,351,117);
const stRectI CGuiPetDialog::sLeveRect(113,127,351,141);
//const stRectI g_CartoonTimeRect(63,173,360,192);

/**
* \brief 构造函数
* 
*/
CGuiPetDialog::CGuiPetDialog()
{
	FUNCTION_BEGIN;
	//Engine_WriteLogF("CGuiPetDialog构造 \n");
	m_bCloseIsHide = true;

	m_bCaption = true;

	m_pCurCartoonPet = NULL;
	m_pBmpExp = NULL;
	m_pCurHorse = NULL;

	FUNCTION_END;
}

/**
* \brief 析构函数
* 
*/
CGuiPetDialog::~CGuiPetDialog()
{
	FUNCTION_BEGIN;
	//Engine_WriteLogF("CGuiPetDialog析构 \n");
	m_pCurCartoonPet = NULL;

	FUNCTION_END;
}

/**
 * \brief 设置玩家角色
 * 
 * 设置玩家角色
 * 
 * \param type : 角色类型
 * \return 无
 */
void CGuiPetDialog::SetCharType(int type)
{
	FUNCTION_BEGIN;

	if (GetGameGuiManager()->m_guiPetDlg)
		GetGameGuiManager()->m_guiPetDlg->SetCharType(type);

	FUNCTION_END;
}

/**
 * \brief 得到网格
 * 
 * 得到网格
 * 
 * \param equip_type : 网格类型
 * \param x : 网格位置
 * \return 得到网格
 */
CGuiTable* & CGuiPetDialog::GetEquipTable(int equip_type,int x )
{
	FUNCTION_BEGIN;

	return GetGameGuiManager()->m_guiUserInfo->GetEquipTable(equip_type,x);

	FUNCTION_END;
}

CRoleItem* CGuiPetDialog::GetEquipItem( int equip_type,int x /* = 0 */ )
{
	CRoleItem* pRoleItem = NULL;

	CGuiTable* pTable = GetEquipTable(equip_type, x);
	if (pTable)
	{
		if (pTable->m_listItem.size()>0)
		{
			pRoleItem =(CRoleItem*) pTable->m_listItem[0]->m_pItemData;
		}
	}

	return pRoleItem;
}

/**
* \brief "创建消息"响应函数
* 
* \return void
*/
void CGuiPetDialog::OnCreate(void)
{
	FUNCTION_BEGIN;

	CGuiDialog::OnCreate();

	byte nTableType = OBJECTCELLTYPE_EQUIP;

	GetEquipTable(EQUIPCELLTYPE_HORSESHOE) = GetTable(215);
	GetEquipTable(EQUIPCELLTYPE_HORSESHOE)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_HORSESHOE)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_HORSESHOE);

	GetEquipTable(EQUIPCELLTYPE_HORSEROPE) = GetTable(216);
	GetEquipTable(EQUIPCELLTYPE_HORSEROPE)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_HORSEROPE)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_HORSEROPE);

	GetEquipTable(EQUIPCELLTYPE_HORSESADDLE) = GetTable(217);
	GetEquipTable(EQUIPCELLTYPE_HORSESADDLE)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_HORSESADDLE)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_HORSESADDLE);

	GetEquipTable(EQUIPCELLTYPE_HORSESAFE) = GetTable(218);
	GetEquipTable(EQUIPCELLTYPE_HORSESAFE)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_HORSESAFE)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_HORSESAFE);

	GetEquipTable(EQUIPCELLTYPE_HORSEIRON) = GetTable(219);
	GetEquipTable(EQUIPCELLTYPE_HORSEIRON)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_HORSEIRON)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_HORSEIRON);

	GetEquipTable(EQUIPCELLTYPE_HORSEFASHION) = GetTable(220);
	GetEquipTable(EQUIPCELLTYPE_HORSEFASHION)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_HORSEFASHION)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_HORSEFASHION);	
	m_ptIcon.x = 33;
	m_ptIcon.y = 90;

	CGuiTab *pTab = GetTab(scTabid);
	AssertCreateCtrl(pTab,"宠物对话框脚本错误");

	//添加跟随弃马按钮到页面0
	AddControl2Tab(pTab,0,scFollowMasterid);
	AddControl2Tab(pTab,0,scRemoveHorseid);	
	AddControl2Tab(pTab,0,scHorseImageid);	

#define Pet_AddControlToTab(frame,id)	{CGuiControl* pCtrl = GetControl(id);			\
	if(pCtrl) {pTab->AddControl(frame,pCtrl);			\
	stToolTipRect ttr;									\
	ttr.key = id;										\
	ttr.index = frame;									\
	ttr.rect = pCtrl->GetBoundingBox();					\
	m_toolTipRects.push_back(ttr);}}

	//添加马属性到pTab,页面0
	Pet_AddControlToTab(0,30);
	Pet_AddControlToTab(0,31);
	Pet_AddControlToTab(0,32);
	Pet_AddControlToTab(0,33);
	Pet_AddControlToTab(0,34);
	Pet_AddControlToTab(0,35);
	Pet_AddControlToTab(0,36);
	Pet_AddControlToTab(0,37);
	Pet_AddControlToTab(0,38);
	Pet_AddControlToTab(0,39);
	Pet_AddControlToTab(0,48);
	Pet_AddControlToTab(0,215);
	Pet_AddControlToTab(0,216);
	Pet_AddControlToTab(0,217);
	Pet_AddControlToTab(0,218);
	Pet_AddControlToTab(0,219);
	Pet_AddControlToTab(0,220);
	Pet_AddControlToTab(0,601);

	//添加宠物属性到pTab,页面1
	Pet_AddControlToTab(1,100);
	Pet_AddControlToTab(1,101);
	Pet_AddControlToTab(1,102);
	Pet_AddControlToTab(1,103);
	Pet_AddControlToTab(1,104);
	Pet_AddControlToTab(1,105);
	Pet_AddControlToTab(1,106);
	Pet_AddControlToTab(1,107);
	Pet_AddControlToTab(1,108);
	Pet_AddControlToTab(1,109);
	Pet_AddControlToTab(1,110);
	Pet_AddControlToTab(1,111);

	AddControl2Tab(pTab,1,112);//编辑名字编辑框
	AddControl2Tab(pTab,1,113);//编辑名字修改
	Pet_EnableCtrl(112,false);
	Pet_EnableCtrl(113,false);
	AddControl2Tab(pTab,1,scPetImageid);

	//添加宠物属性到pTab,页面2
	Pet_AddControlToTab(2,200);
	Pet_AddControlToTab(2,201);
	Pet_AddControlToTab(2,202);
	Pet_AddControlToTab(2,203);
	Pet_AddControlToTab(2,204);
	Pet_AddControlToTab(2,205);
	Pet_AddControlToTab(2,206);
	Pet_AddControlToTab(2,207);
	Pet_AddControlToTab(2,208);
	Pet_AddControlToTab(2,209);
	Pet_AddControlToTab(2,210);
	Pet_AddControlToTab(2,211);

	AddControl2Tab(pTab,2,212);//编辑名字编辑框
	AddControl2Tab(pTab,2,213);//编辑名字修改
	Pet_EnableCtrl(212,false);
	Pet_EnableCtrl(213,false);
	AddControl2Tab(pTab,2,scSummonImageid);

	//添加控件到第3个页面
	AddControl2Tab(pTab,3,300);
	AddControl2Tab(pTab,3,301);
	AddControl2Tab(pTab,3,302);
	AddControl2Tab(pTab,3,303);
	AddControl2Tab(pTab,3,304);
	AddControl2Tab(pTab,3,305);
	AddControl2Tab(pTab,3,306);
	AddControl2Tab(pTab,3,307);
	AddControl2Tab(pTab,3,308);
	AddControl2Tab(pTab,3,309);
	AddControl2Tab(pTab,3,310);
	AddControl2Tab(pTab,3,311);
	Pet_AddControlToTab(3,312);
	Pet_AddControlToTab(3,313);
	Pet_AddControlToTab(3,314);
	Pet_AddControlToTab(3,315);
	Pet_AddControlToTab(3,316);
	Pet_AddControlToTab(3,317);
	Pet_AddControlToTab(3,318);
	Pet_AddControlToTab(3,319);

	Pet_AddTabButton(scHorseButtonid);
	Pet_AddTabButton(scPetButtonid);
	Pet_AddTabButton(scSummon);
	Pet_AddTabButton(scCartoon);	

	{   //经验条显示
		stToolTipRect ttr;									
		ttr.key = 1000;										
		ttr.index = 3;									
		ttr.rect = stRectI(50,393,377,398);
		m_toolTipRects.push_back(ttr);
	}
	
	pTab->SetVisible(false);

	OnGuiEvent(EVENT_BUTTON_CLICKED,scHorseButtonid,GetButton(scHorseButtonid));

	stResourceLocation rl;
	rl.SetFileName( GetGuiGraphicPackName() );
	rl.group = 60;
	rl.frame = 34;
	m_pBmpExp = GetDevice()->FindBitmaps( &rl );	

	UpdateContents();

	FUNCTION_END;
}


/**
* \brief GUI消息处理函数
* 
* \param nEvent : 消息句柄
* \param nID : 控件id
* \param pControl : 控件指针
* \return true if succeed else false
*/
bool CGuiPetDialog::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	FUNCTION_BEGIN;

	switch(nEvent)
	{
	case EVENT_BUTTON_CLICKED:	
		{
#define Pet_Button_Clicked_Event(id,fun) case (id):{if( fun() ) return true;}break
			switch(nID)
			{
				Pet_Button_Clicked_Event(scFollowMasterid,OnHorseFollowMaster);
				Pet_Button_Clicked_Event(scRemoveHorseid,OnHorseRemove);
				Pet_Button_Clicked_Event(scHorseButtonid,OnHorseTab);
				Pet_Button_Clicked_Event(scPetButtonid,OnPetTab);
				Pet_Button_Clicked_Event(scSummon,OnSummonTab);
				Pet_Button_Clicked_Event(scCartoon,OnCartoonTab);
				Pet_Button_Clicked_Event(scRenamePetBtnid,OnPetRename);
				Pet_Button_Clicked_Event(scRenameSummonBtnid,OnSummonRename);

				Pet_Button_Clicked_Event(scCartoonAddCentID,OnCartoonAddCent);
				Pet_Button_Clicked_Event(scCartoonPutoutID,OnCartoonPutout);
				Pet_Button_Clicked_Event(scCartoonPreID,OnCartoonPre);
				Pet_Button_Clicked_Event(scCartoonStopTrainID,OnCartoonStopTraining);
				Pet_Button_Clicked_Event(scCartoonNextID,OnCartoonNext);
				Pet_Button_Clicked_Event(scCartoonRenameID,OnCartoonRename);
				Pet_Button_Clicked_Event(scCartoonFeedID,OnCartoonFeed);
				Pet_Button_Clicked_Event(scCartoonGetExpID,OnCartoonGetExp);
				Pet_Button_Clicked_Event(scCartoonPutinID,OnCartoonPutin);
				Pet_Button_Clicked_Event(scCartoonStartTrainID,OnCartoonStartTraining);
				Pet_Button_Clicked_Event(318,OnStartAutoRepair);
				Pet_Button_Clicked_Event(319,OnEndAutoRepair);
				Pet_Button_Clicked_Event(601,OnAddZuoqi);//坐骑图鉴
			}
		}
		break;
	}


	return CGuiDialog::OnGuiEvent(nEvent,nID,pControl);

	FUNCTION_END;
}


/**
* \brief 
* 
* 
*
* \return void
*/
void CGuiPetDialog::SetPetImage(int id,DWORD dwImage)
{
	CGuiImage *pImage = GetImage(id);
	if(pImage)
	{	
		pImage->SetImage(stResourceLocation("data\\icons.gl",5,dwImage));
	}
}

/**
* \brief 
* 
* 
*
* \return void
*/
void CGuiPetDialog::AfterOnUpdateTab(int id,petType type)
{
	CPet* pPet = GetScene()->FindPetByType( type );
	if( pPet )
	{			
		NpcBase_t* pNpcBase = pPet->GetNpcBase();
		if(pNpcBase)
		{
			//set dialog icon
			SetIcon(&(stResourceLocation("data\\icons.gl",4,pNpcBase->dwPic)));
			//set pet image
			SetPetImage(id,pNpcBase->dwImage);
			return ;
		}
	}
	SetPetImage(id,-1);
}

/**
* \brief 
* 
* 
*
* \return void
*/
bool CGuiPetDialog::OnHorseTab(void)
{
	FUNCTION_BEGIN;

	UpdateHorseTab();
	CGuiTab *pTab = GetTab(scTabid);
	if(pTab)
	{
		pTab->SetCurItem(0);
		GetButton(scHorseButtonid)->SetChecked(true);
		SetIcon(&stResourceLocation("data\\interfaces.gl",22,0));
		SetBackImage(stResourceLocation("data\\interfaces.gl",25,0));
		AfterOnUpdateTab(scHorseImageid,PET_TYPE_RIDE);		
	}
	return true;

	FUNCTION_END;
}


bool CGuiPetDialog::OnPetTab(void)
{
	FUNCTION_BEGIN;

	UpdatePetTab();
	CGuiTab *pTab = GetTab(scTabid);
	if(pTab)
	{
		pTab->SetCurItem(1);
		GetButton(scPetButtonid)->SetChecked(true);
		SetBackImage(stResourceLocation("data\\interfaces.gl",25,16));
		SetIcon(&stResourceLocation("data\\interfaces.gl",22,31));
		AfterOnUpdateTab(scPetImageid,PET_TYPE_PET);
		GetEditBox(112)->SetText("");
	}
	return true;

	FUNCTION_END;
}

/**
* \brief 
* 
* 
*
* \return void
*/
bool CGuiPetDialog::OnSummonTab(void)
{
	FUNCTION_BEGIN;

	UpdateSummonTab();
	CGuiTab *pTab = GetTab(scTabid);
	if(pTab)
	{
		pTab->SetCurItem(2);
		GetButton(scSummon)->SetChecked(true);
		SetBackImage(stResourceLocation("data\\interfaces.gl",25,16));
		SetIcon(&stResourceLocation("data\\interfaces.gl",22,31));
		AfterOnUpdateTab(scSummonImageid,PET_TYPE_SUMMON);
		GetEditBox(212)->SetText("");
	}
	return true;

	FUNCTION_END;
}

bool CGuiPetDialog::OnCartoonTab()
{
	FUNCTION_BEGIN;

	CGuiTab *pTab = GetTab(scTabid);
	if(pTab)
	{
		pTab->SetCurItem(3);
		GetButton(scCartoon)->SetChecked(true);
		SetBackImage(stResourceLocation("data\\interfaces.gl",60,0)); //soke 使用征途原版
		SetIcon(&stResourceLocation("data\\interfaces.gl",22,31));
	}
	UpdateCartoonTab();
	return true;

	FUNCTION_END;
}

void CGuiPetDialog::ClearEditText()
{
	FUNCTION_BEGIN;

	GetEditBox(112)->SetText("");
	GetEditBox(212)->SetText("");

	FUNCTION_END;
}

/**
* \brief 对话框关闭消息处理
* 
* 退出对话框、更新管理器对应的指针
*
* \return void
*/
void CGuiPetDialog::OnClose(void)
{
	FUNCTION_BEGIN;

	GetGameGuiManager()->m_guiPetDlg = NULL;

	FUNCTION_END;
}

/**
* \brief 刷性控件显示
* 
* 根据角色拥有马的情况显示
*
* \return void
*/
void CGuiPetDialog::UpdateContents()
{
	FUNCTION_BEGIN;

	int iTab = GetTab(scTabid)->GetCurItem();
	switch(iTab)
	{
	case 0:	{UpdateHorseTab();	AfterOnUpdateTab(scHorseImageid,PET_TYPE_RIDE);	}	break;
	case 1: {UpdatePetTab();	AfterOnUpdateTab(scPetImageid,PET_TYPE_PET);}		break;
	case 2: {UpdateSummonTab();	AfterOnUpdateTab(scSummonImageid,PET_TYPE_SUMMON);}	break;
	//case 3: {UpdateCartoonTab();AfterOnUpdateTab(scCartoonImageid,PET_TYPE_SUMMON);}	break;
	default: break;
	}	

	FUNCTION_END;
}

void CGuiPetDialog::SetCartoonPetImage(int id,CCartoonPet* pCartoon)
{
	if( pCartoon )
	{			
		NpcBase_t* pNpcBase = pCartoon->GetNpcBase();
		if(pNpcBase)
		{
			//set dialog icon
			SetIcon(&(stResourceLocation("data\\icons.gl",4,pNpcBase->dwPic)));
			//set pet image
			SetPetImage(id,pNpcBase->dwImage);
			return ;
		}
	}
	else
	{
		SetIcon(&stResourceLocation("data\\interfaces.gl",22,31));
		SetPetImage(id,-1);
	}	
}

void CGuiPetDialog::UpdateCartoonTab()
{
	if( NULL == m_pCurCartoonPet )
	{
		CCartoonPetList* pList = GetScene()->GetCartoonPetList();
		if( pList )
		{
			SetCurCartoonPet( pList->GetAt( 0 ) );
		}
	}
	else
	{
		SetCurCartoonPet(m_pCurCartoonPet);
	}
}

const char* GetHorseSkillText(DWORD dwSkill)
{
	static char szSkillText[MAX_PATH];
	switch(dwSkill>>16)
	{
	case HORSE_SKILL_DEF_UP:
		_snprintf(szSkillText,MAX_PATH,"提高物防魔防:%d",dwSkill & 0xffff);szSkillText[MAX_PATH-1] = 0;return szSkillText;
	case HORSE_SKILL_PATK_UP:
		_snprintf(szSkillText,MAX_PATH,"提高物攻:%d",dwSkill & 0xffff);szSkillText[MAX_PATH-1] = 0;return szSkillText;
	case HORSE_SKILL_MATK_UP:
		_snprintf(szSkillText,MAX_PATH,"提高魔攻:%d",dwSkill & 0xffff);szSkillText[MAX_PATH-1] = 0;return szSkillText;
	case HORSE_SKILL_HP_UP:
		_snprintf(szSkillText,MAX_PATH,"提高生命上限:%d",dwSkill & 0xffff);szSkillText[MAX_PATH-1] = 0;return szSkillText;
	case HORSE_SKILL_MP_UP:
		_snprintf(szSkillText,MAX_PATH,"提高魔法上限:%d",dwSkill & 0xffff);szSkillText[MAX_PATH-1] = 0;return szSkillText;
	case HORSE_SKILL_DMG_UP:
		_snprintf(szSkillText,MAX_PATH,"附加伤害:%d",dwSkill & 0xffff);szSkillText[MAX_PATH-1] = 0;return szSkillText;
	case HORSE_SKILL_DMG_DOWN:
		_snprintf(szSkillText,MAX_PATH,"减少自身伤害:%d",dwSkill & 0xffff);szSkillText[MAX_PATH-1] = 0;return szSkillText;
	default:
		return NULL;
	}
	return NULL;
}
/**
* \brief 刷性坐骑页面控件显示
* 
* 根据角色拥有马的情况显示
*
* \return void
*/
void CGuiPetDialog::UpdateHorseTab()
{
	FUNCTION_BEGIN;

	m_pCurHorse = NULL;
	CPet* pPet = GetScene()->FindPetByType( PET_TYPE_RIDE );
	if( pPet )
	{
		m_pCurHorse = static_cast<CHorse*>( pPet );
	}
	if(m_pCurHorse)
	{
		const t_HorseData *pData = m_pCurHorse->GetHorseProperty();
		if( pData )
		{
			Pet_EnableCtrl(scFollowMasterid,true);
			Pet_EnableCtrl(scRemoveHorseid,true);

			char szText[32];
			int nID = 30;

#define PetDlg_SetHorseText(id,txt,d)			{sprintf(szText,"%s%u",txt,pData->d);Pet_SetStaticText(id,szText);}			
#define PetDlg_SetHorseExt(txt,mem)				{if( pData->mem > 0 )	{PetDlg_SetHorseText(nID,txt,mem); nID++;}}
#define PetDlg_SetHorseTextPercent(id,txt,d)	{sprintf(szText,"%s%u%%",txt,pData->d);Pet_SetStaticText(id,szText);}			
#define PetDlg_SetHorseExtPercent(txt,mem)		{if( pData->mem > 0 )	{PetDlg_SetHorseTextPercent(nID,txt,mem); nID++;}}

			PetDlg_SetHorseExt("增加主人力量:",str);
			PetDlg_SetHorseExt("增加主人智力:",intel);
			PetDlg_SetHorseExt("增加主人敏捷:",dex);
			PetDlg_SetHorseExt("增加主人精神:",men);
			PetDlg_SetHorseExt("增加主人体质:",con);
			PetDlg_SetHorseExt("增加主人速度:",speed);
			PetDlg_SetHorseExtPercent("增加主人物攻:",pdam);
			PetDlg_SetHorseExtPercent("增加主人物防:",pdef);
			PetDlg_SetHorseExtPercent("增加主人魔攻:",mdam);
			PetDlg_SetHorseExtPercent("增加主人魔防:",mdef);
			PetDlg_SetHorseExt("增加主人最大生命值:",maxhp);
			PetDlg_SetHorseExt("增加主人最大魔法值:",maxmp);
			PetDlg_SetHorseExt("增加主人抗减速",slowRes);
			PetDlg_SetHorseExt("增加主人抗失明",blindRes);
			PetDlg_SetHorseExt("增加主人抗石化",petrifyRes);
			PetDlg_SetHorseExt("增加主人抗冰冻",freezeRes);
			PetDlg_SetHorseExt("增加主人抗混乱",chaosRes);
			PetDlg_SetHorseExt("增加主人抗晕眩",faintRes);
			PetDlg_SetHorseExt("增加主人抗麻痹",lullRes);
			PetDlg_SetHorseExt("增加主人抗毒",poisonRes);
		
			return ;
		}
	}

	Pet_EnableCtrl(scFollowMasterid,false);
	Pet_EnableCtrl(scRemoveHorseid,false);

	Pet_SetStaticText(30,"");
	Pet_SetStaticText(31,"");
	Pet_SetStaticText(32,"");
	Pet_SetStaticText(33,"");
	Pet_SetStaticText(34,"");
	Pet_SetStaticText(35,"");
	Pet_SetStaticText(36,"");
	Pet_SetStaticText(37,"");
	Pet_SetStaticText(38,"");
	Pet_SetStaticText(39,"");

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiPetDialog::UpdatePetTab()
{
	FUNCTION_BEGIN;

	CPet* pPet = GetScene()->FindPetByType( PET_TYPE_PET );
	if( pPet )
	{
		const t_PetData *pData = pPet->GetProperty();
		if( pData ) 
		{
			char buf[32];
			DWORD color = -1;
#define _SetPetText(id,d) { std::string s = FormatLargeNumber(static_cast<uint64_t>(pData->d)); strncpy(buf, s.c_str(), sizeof(buf)-1); buf[sizeof(buf)-1] = '\0'; Pet_SetStaticText(id,buf); }
#define _SetPetTextEx(id,d,dplus) { uint64_t val = pData->d + pData->dplus; std::string s = FormatLargeNumber(val); strncpy(buf, s.c_str(), sizeof(buf)-1); buf[sizeof(buf)-1] = '\0'; Pet_SetStaticText(id,buf); }
#define _SetPetTextRange(id,d1,d2) { std::string s1 = FormatLargeNumber(static_cast<uint64_t>(pData->d1)); std::string s2 = FormatLargeNumber(static_cast<uint64_t>(pData->d2)); _snprintf(buf, sizeof(buf), "%s/%s", s1.c_str(), s2.c_str()); Pet_SetStaticText(id,buf); }
#define _SetPetTextRangeEx(id,d1,d2,dplus) { std::string s1 = FormatLargeNumber(pData->d1); std::string s2 = FormatLargeNumber(pData->d2 + pData->dplus); _snprintf(buf, sizeof(buf), "%s/%s", s1.c_str(), s2.c_str()); Pet_SetStaticText(id,buf); }
#define _SetPetTextRange_(id,d1,d2) { std::string s1 = FormatLargeNumber(pData->d1); std::string s2 = FormatLargeNumber(pData->d2); _snprintf(buf, sizeof(buf), "%s-%s", s1.c_str(), s2.c_str()); Pet_SetStaticText(id,buf); }
#define _SetPetTextRange_Ex(id,d1,d1plus,d2,d2plus) { uint64_t v1 = pData->d1 + pData->d1plus; uint64_t v2 = pData->d2 + pData->d2plus; std::string s1 = FormatLargeNumber(v1); std::string s2 = FormatLargeNumber(v2); _snprintf(buf, sizeof(buf), "%s-%s", s1.c_str(), s2.c_str()); Pet_SetStaticText(id,buf); }

			//_SetPetTextRange(100,hp,maxhp);
			_SetPetTextRangeEx(100,hp,maxhp,maxhp_plus);
			_SetPetText(101,str);
			_SetPetText(102,intel);
			_SetPetText(103,agi);
			_SetPetText(104,men);
			_SetPetText(105,vit);
			_SetPetTextRange(106,exp,maxexp);
			_SetPetTextRange_Ex(107,atk,atk_plus,maxatk,maxatk_plus);
			_SetPetTextRange_Ex(108,matk,matk_plus,maxmatk,maxmatk_plus);
			_SetPetTextEx(109,def,pdef_plus);
			_SetPetTextEx(110,mdef,mdef_plus);
			_SetPetText(111,cri);

			if(pData->maxhp_plus>0)
			{
				Pet_SetStaticTextColor(100,plusColor);
			}
			else
			{
				Pet_SetStaticTextColor(100,-1);
			}
			if(pData->atk_plus>0 || pData->maxatk_plus>0)
			{
				Pet_SetStaticTextColor(107,plusColor);
			}
			else
			{
				Pet_SetStaticTextColor(107,-1);
			}
			if(pData->matk_plus>0 || pData->maxmatk_plus>0)
			{
				Pet_SetStaticTextColor(108,plusColor);
			}
			else
			{
				Pet_SetStaticTextColor(108,-1);
			}


			if(pData->pdef_plus>0)
			{
				Pet_SetStaticTextColor(109,plusColor);
			}
			else
			{
				Pet_SetStaticTextColor(109,-1);
			}
			if( pData->mdef_plus>0)
			{
				Pet_SetStaticTextColor(110,plusColor);
			}
			else
			{
				Pet_SetStaticTextColor(110,-1);
			}

			Pet_EnableCtrl(113,true);
			return ;
		}
	}

	Pet_SetStaticText(100,"");
	Pet_SetStaticText(101,"");
	Pet_SetStaticText(102,"");
	Pet_SetStaticText(103,"");
	Pet_SetStaticText(104,"");
	Pet_SetStaticText(105,"");
	Pet_SetStaticText(106,"");
	Pet_SetStaticText(107,"");
	Pet_SetStaticText(108,"");
	Pet_SetStaticText(109,"");
	Pet_SetStaticText(110,"");
	Pet_SetStaticText(111,"");
	Pet_EnableCtrl(112,false);
	Pet_EnableCtrl(113,false);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiPetDialog::UpdateSummonTab()
{
	FUNCTION_BEGIN;

	CPet* pPet = GetScene()->FindPetByType( PET_TYPE_SUMMON );
	if( pPet )
	{
		const t_PetData *pData = pPet->GetProperty();
		if( pData )
		{
			char buf[32];
// 设置单个数值的文本显示 - 保持不变
#define _SetSummonText(id,d) { \
    std::string s = FormatLargeNumber(static_cast<uint64_t>(pData->d)); \
    strncpy(buf, s.c_str(), sizeof(buf)-1); \
    buf[sizeof(buf)-1] = '\0'; \
    Pet_SetStaticText(id,buf); \
}

// 设置单个数值（基础值+加成值）的文本显示 - 保持不变
#define _SetSummonTextEx(id,d,dplus) { \
    uint64_t val = pData->d + pData->dplus; \
    std::string s = FormatLargeNumber(val); \
    strncpy(buf, s.c_str(), sizeof(buf)-1); \
    buf[sizeof(buf)-1] = '\0'; \
    Pet_SetStaticText(id,buf); \
}

// 设置范围显示（当前值/最大值），最大值有加成 - 修改为只显示最大值（含加成）
#define _SetSummonTextRangeEx(id,d1,d2,dplus) { \
    std::string s2 = FormatLargeNumber(pData->d2 + pData->dplus); \
    strncpy(buf, s2.c_str(), sizeof(buf)-1); \
    buf[sizeof(buf)-1] = '\0'; \
    Pet_SetStaticText(id,buf); \
}

// 设置范围显示（当前值/最大值） - 修改为只显示最大值
#define _SetSummonTextRange(id,d1,d2) { \
    std::string s2 = FormatLargeNumber(static_cast<uint64_t>(pData->d2)); \
    strncpy(buf, s2.c_str(), sizeof(buf)-1); \
    buf[sizeof(buf)-1] = '\0'; \
    Pet_SetStaticText(id,buf); \
}

// 设置范围显示（最小值-最大值） - 修改为只显示最大值
#define _SetSummonTextRange_(id,d1,d2) { \
    std::string s2 = FormatLargeNumber(pData->d2); \
    strncpy(buf, s2.c_str(), sizeof(buf)-1); \
    buf[sizeof(buf)-1] = '\0'; \
    Pet_SetStaticText(id,buf); \
}

// 设置范围显示（基础值+加成值 到 最大值+加成值） - 修改为只显示最大值（含加成）
#define _SetSummonTextRange_Ex(id,d1,d1plus,d2,d2plus) { \
    uint64_t v2 = pData->d2 + pData->d2plus; \
    std::string s2 = FormatLargeNumber(v2); \
    strncpy(buf, s2.c_str(), sizeof(buf)-1); \
    buf[sizeof(buf)-1] = '\0'; \
    Pet_SetStaticText(id,buf); \
}
			//_SetSummonTextRange(200,hp,maxhp);
			// 这些调用代码完全保持不变
_SetSummonTextRangeEx(200,hp,maxhp,maxhp_plus);          // 只显示最大HP（含加成）
_SetSummonText(201,str);                                // 显示力量
_SetSummonText(202,intel);                              // 显示智力  
_SetSummonText(203,agi);                                // 显示敏捷
_SetSummonText(204,men);                                // 显示精神
_SetSummonText(205,vit);                                // 显示体力
_SetSummonTextRange(206,exp,maxexp);                    // 只显示最大经验
_SetSummonTextRange_Ex(207,atk,atk_plus,maxatk,maxatk_plus); // 只显示最大物理攻击（含加成）
_SetSummonTextRange_Ex(208,matk,matk_plus,maxmatk,maxmatk_plus); // 只显示最大魔法攻击（含加成）
_SetSummonTextEx(209,def,pdef_plus);                    // 显示防御（含加成）
_SetSummonTextEx(210,mdef,mdef_plus);                   // 显示魔法防御（含加成）
_SetSummonText(211,cri);                                // 显示暴击率

			if(pData->maxhp_plus>0)	
			{
				Pet_SetStaticTextColor(200,plusColor);
			}
			else
			{
				Pet_SetStaticTextColor(200,-1);
			}
			if(pData->atk_plus>0 || pData->maxatk_plus>0)
			{
				Pet_SetStaticTextColor(207,plusColor);
				Pet_SetStaticTextColor(208,plusColor);
			}
			else
			{
				Pet_SetStaticTextColor(207,-1);
				Pet_SetStaticTextColor(208,-1);
			}
			if(pData->pdef_plus>0)
			{
				Pet_SetStaticTextColor(209,plusColor);
			}
			else
			{
				Pet_SetStaticTextColor(209,-1)
			}
			if ( pData->mdef_plus>0 )
			{
				Pet_SetStaticTextColor(210,plusColor);
			}
			else
			{
				Pet_SetStaticTextColor(210,-1);
			}

			Pet_EnableCtrl(213,true);
			return ;
		}
	}

	Pet_SetStaticText(200,"");
	Pet_SetStaticText(201,"");
	Pet_SetStaticText(202,"");
	Pet_SetStaticText(203,"");
	Pet_SetStaticText(204,"");
	Pet_SetStaticText(205,"");
	Pet_SetStaticText(206,"");
	Pet_SetStaticText(207,"");
	Pet_SetStaticText(208,"");
	Pet_SetStaticText(209,"");
	Pet_SetStaticText(210,"");
	Pet_SetStaticText(211,"");

	Pet_EnableCtrl(212,false);
	Pet_EnableCtrl(213,false);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiPetDialog::OnSetCloseLocation()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnSetCloseLocation();

	if(m_pBtnClose)
	{
		stPointI pt(m_pBtnClose->GetLocation());

		m_pBtnClose->SetLocation(pt.x,pt.y + 4);
	}

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
HRESULT CGuiPetDialog::OnRender(float fElapsedTime)
{
	FUNCTION_BEGIN;

	HRESULT hr = CGuiDialog::OnRender(fElapsedTime);

	int iTab = GetTab(scTabid)->GetCurItem();
	RenderTab(fElapsedTime,iTab);

	return hr;
}


void CGuiPetDialog::RenderCartoonPetExp(CCartoonPet* pCartoon)
{
	if( pCartoon )
	{
		if( m_pBmpExp && pCartoon->GetData().exp <=  pCartoon->GetData().maxExp )
		{
			stRectI rc(50,393,377,398);
			rc.OffsetRect(m_x,m_y);
			stPointF scale((float) pCartoon->GetData().exp / (pCartoon->GetData().maxExp),1.0f);
			m_pBmpExp->Render(0,rc.left,rc.top,NULL,&scale);
		}
		RenderToolTip(NULL,3);
	}
}

void CGuiPetDialog::RenderTab(float fElapsedTime,int cur)
{
	bool bRes = false;
	int iOldFont = GetDevice()->SetFont(1);
	switch(cur)
	{
	case 0: bRes = RenderText(fElapsedTime,PET_TYPE_RIDE);	break;
	case 1: bRes = RenderText1(fElapsedTime,PET_TYPE_PET);	break;
	case 2: bRes = RenderText1(fElapsedTime,PET_TYPE_SUMMON);break;
	case 3: RenderCartoonPetExp(m_pCurCartoonPet); bRes = true; break;
	default:break;
	}
	if(!bRes)
	{
		char name[32];
		char level[32];
		memset(name,0,32);
		memset(level,0,32);
		switch(cur)
		{
		case 0: sprintf(name,"坐骑");	sprintf(level,"你还没有坐骑");		break;
		case 1: sprintf(name,"宠物");	sprintf(level,"你还没有宠物");		break;
		case 2: sprintf(name,"召唤兽");	sprintf(level,"你还没有召唤兽");	break;
		default:break;
		}
		{
			stRectI rect(sNameRect);
			rect.OffsetRect(m_x,m_y);
			GetDevice()->DrawString(name,rect,ColorBlend(-1,0xffffffff),DT_VCENTER|DT_CENTER);
		}
		{
			stRectI rect(sLeveRect);
			rect.OffsetRect(m_x,m_y);
			GetDevice()->DrawString(level,rect,ColorBlend(-1,0xffffffff),DT_VCENTER|DT_CENTER);
		}
	}
	GetDevice()->SetFont(iOldFont);
}

bool CGuiPetDialog::RenderText(float fElapsedTime,petType type)
{
	CPet* pPet = GetScene()->FindPetByType( type );
	if( pPet )
	{
		const t_PetData *pData = pPet->GetProperty();
		if( pData ) 
		{
			char buf[32];
			{
				sprintf(buf,"%s",pData->name);
				stRectI rect(sNameRect);
				rect.OffsetRect(m_x,m_y);
				GetDevice()->DrawString(buf,rect,pPet->GetNameTextColor(),DT_VCENTER|DT_CENTER);
			}
			{
				sprintf(buf,"(等级  %u)",pData->lv);
				stRectI rect(sLeveRect);
				rect.OffsetRect(m_x,m_y);
				GetDevice()->DrawString(buf,rect,ColorBlend(-1,0xffffffff),DT_VCENTER|DT_CENTER);
			}

			
			//马匹训练
			const t_HorseData *hData = m_pCurHorse->GetHorseProperty();
	
			if( hData )
			{
				int iOldFont = GetDevice()->SetFont(0);
				stPointI ptText ;
				int dy = 0;
				ptText = stPointI(70,167) + GetLocation();

				if( hData->horseXLlevel > 0 && hData->horseXLlevel < 5 )
				{
					if( hData->horseXLlevel == 4 )
				    {
						sprintf(buf,"训练等级：四级");
						GetDevice()->DrawString(buf ,ptText ,0xffffff00);
					}
					else if( hData->horseXLlevel == 3 )
				    {
						sprintf(buf,"训练等级：三级");
						GetDevice()->DrawString(buf ,ptText ,0xffffff00);
					}
					else if( hData->horseXLlevel == 2 )
				    {
						sprintf(buf,"训练等级：二级");
						GetDevice()->DrawString(buf ,ptText ,0xffffff00);
					}
					else if( hData->horseXLlevel == 1 )
				    {
						sprintf(buf,"训练等级：一级");
						GetDevice()->DrawString(buf ,ptText ,0xffffff00);
					}
					GetButton(48)->SetVisible(true);
				}					
				else
				{
					GetDevice()->DrawString("训练等级：0",ptText,0xffffff00);
					GetButton(48)->SetVisible(false);
				}

				dy=24;
				ptText.y += dy;
				if( hData->horseXLlevel == 1)
				{
					GetDevice()->DrawString("增加移动速度：10" ,ptText ,0xffffff00);
				}
				else if( hData->horseXLlevel == 2 )
				{
					GetDevice()->DrawString("增加移动速度：20" ,ptText ,0xffffff00);
				}
				else if( hData->horseXLlevel == 3 )
				{
					GetDevice()->DrawString("增加移动速度：30" ,ptText ,0xffffff00);
				}
				else if( hData->horseXLlevel == 4 )
				{
					GetDevice()->DrawString("增加移动速度：50" ,ptText ,0xffffff00);
				}
				else
					GetDevice()->DrawString("增加移动速度：0",ptText,0xffffff00);

				ptText = stPointI(170,167) + GetLocation();
				if( hData->horseXLtime > 0)
				{
					int day = hData->horseXLtime/86400;
					int hour = ( hData->horseXLtime/3600 )%24;
					int fenz = ( hData->horseXLtime%3600 )/60;
					sprintf(buf,"剩余时间：%d天%d小时%d分钟", day, hour, fenz );
					GetDevice()->DrawString(buf ,ptText ,0xffffff00);
				}
				else
					GetDevice()->DrawString("剩余时间：0", ptText, 0xffffff00);

				stToolTipRect tr;
				tr.index = 0;
				tr.key = 40;
				tr.rect.SetRect(70,167,165,170);
				m_toolTipRects.push_back(tr);

				tr.key = 41;
				tr.rect.SetRect(70,191,165,195);
				m_toolTipRects.push_back(tr);

				tr.key = 42;
				tr.rect.SetRect(170,167,250,170);
				m_toolTipRects.push_back(tr);
				GetDevice()->SetFont(iOldFont);
			}

		    //render tooltip
			{
				int iTab = GetTab(scTabid)->GetCurItem();
				RenderToolTip(pData,iTab);
			}
			// 调整坐骑纸娃娃位置
			if( hData->id == 3000 ) //枣红马
			{
				GetImage(scHorseImageid)->SetLocation( 0, -10 );
			}
			else if( hData->id == 3200 ) //战马
			{
				GetImage(scHorseImageid)->SetLocation( -10, -5 );
			}
			else if( hData->id == 3203 ) //的卢马
			{
				GetImage(scHorseImageid)->SetLocation( 0, -30 );
			}
			else if( hData->id == 3201 ) //赤兔马
			{
				GetImage(scHorseImageid)->SetLocation( 0, -10 );
			}
			else if( hData->id == 3205 ) //玄冥圣虎
			{
				GetImage(scHorseImageid)->SetLocation( -20, 30 );
			}
			else if( hData->id == 3206 ) //紫金麒麟
			{
				GetImage(scHorseImageid)->SetLocation( 0, 0 );
			}
			else if( hData->id == 3207 ) //天煞狂狼
			{
				GetImage(scHorseImageid)->SetLocation( 0, 20 );
			}
			else if( hData->id == 3208 ) //金甲神牛
			{
				GetImage(scHorseImageid)->SetLocation( 0, 20 );
			}
			else if( hData->id == 3209 ) //赤焰魔狮
			{
				GetImage(scHorseImageid)->SetLocation( -10, 20 );
			}
			else if( hData->id == 3210 ) //龙麟圣兽
			{
				GetImage(scHorseImageid)->SetLocation( -10, 30 );
			}
			else if( hData->id == 3336 ) //九天神驹
			{
				GetImage(scHorseImageid)->SetLocation( 0, -10 );
			}
			else if( hData->id == 3337 ) //九天神龙
			{
				GetImage(scHorseImageid)->SetLocation( -10, 30 );
			}
			else if( hData->id == 3338 ) //帝王御马
			{
				GetImage(scHorseImageid)->SetLocation( 0, -10 );
			}
			else
				GetImage(scHorseImageid)->SetLocation( 0, 0 );

			return true;
		}		
	}		
	return false;
}
//宠物 召唤兽
bool CGuiPetDialog::RenderText1(float fElapsedTime,petType type)
{
	CPet* pPet = GetScene()->FindPetByType( type );
	if( pPet )
	{
		const t_PetData *pData = pPet->GetProperty();
		if( pData ) 
		{
			char buf[32];
			{
				sprintf(buf,"%s",pData->name);
				stRectI rect(sNameRect);
				rect.OffsetRect(m_x,m_y);
				GetDevice()->DrawString(buf,rect,pPet->GetNameTextColor(),DT_VCENTER|DT_CENTER);
			}
			{
				sprintf(buf,"(等级  %u)",pData->lv);
				stRectI rect(sLeveRect);
				rect.OffsetRect(m_x,m_y);
				GetDevice()->DrawString(buf,rect,ColorBlend(-1,0xffffffff),DT_VCENTER|DT_CENTER);
			}

			// const t_HorseData *hData = m_pCurHorse->GetHorseProperty();

            // if( hData->horseXLlevel > 0 && hData->horseXLlevel < 5 )
			// 	{
			// 		GetButton(48)->SetVisible(true);
			// 	}					
			// 	else
			// 	{
			// 		GetButton(48)->SetVisible(false);
			// 	}

			//by=>friday 添加对m_pCurHorse的NULL检查
            if(m_pCurHorse != NULL)  // 修改添加这一行
            {
                const t_HorseData *hData = m_pCurHorse->GetHorseProperty();

                if(hData->horseXLlevel > 0 && hData->horseXLlevel < 5)
                {
                    GetButton(48)->SetVisible(true);
                }					
                else
                {
                    GetButton(48)->SetVisible(false);
                }
            }
            else  // 如果没有坐骑，默认隐藏按钮
            {
                GetButton(48)->SetVisible(false);
            }

             //render tooltip
			{
				int iTab = GetTab(scTabid)->GetCurItem();
				RenderToolTip(pData,iTab);
			}
			return true;
		}		
	}		
	return false;
}

void CGuiPetDialog::RenderHorse(float fElapsedTime)
{
	int iOldFont = GetDevice()->SetFont(1);

	CMainCharacter *pMainChar = GetScene()->GetMainCharacter();

	NpcBase_t *pHorse = GetNpcBase(pMainChar->GetRideID());
	if(pHorse)
	{
		//名字
		if(pMainChar->IsHasHorse())
		{
			stRectI rect(sNameRect);
			rect.OffsetRect(m_x,m_y);
			GetDevice()->DrawString(pHorse->strName,rect,ColorBlend(-1,0xffffffff),DT_VCENTER|DT_CENTER);
		}
		else
		{
			stRectI rect(sNameRect);
			rect.OffsetRect(m_x,m_y);
			GetDevice()->DrawString("坐骑",rect,ColorBlend(-1,0xffffffff),DT_VCENTER|DT_CENTER);
		}

		// 等级 
		if(pMainChar->IsHasHorse())
		{			
			stRectI rect(sLeveRect);
			rect.OffsetRect(m_x,m_y);
			GetDevice()->DrawString(avar("%u级",pHorse->dwLevel ),rect,0xffffffff,DT_VCENTER|DT_CENTER);
		}
		else
		{
			stRectI rect(sLeveRect);
			rect.OffsetRect(m_x,m_y);
			GetDevice()->DrawString("你还没有马匹",rect,0xffffffff,DT_VCENTER|DT_CENTER);
		}
	}
	GetDevice()->SetFont(iOldFont);
}

#define RenderPetToolTips(title,num)	{							\
	m_ToolTips.SetCurColor(-1);													\
	m_ToolTips.AddText(title);													\
	m_ToolTips.AddText(FormatLargeNumber(num).c_str());} //by=>friday 使用FormatLargeNumber格式化

void CGuiPetDialog::RenderHorseToolTips(UINT nID)
{	
	if(m_pCurHorse)
	{
		const t_HorseData *pData = m_pCurHorse->GetHorseProperty();
		switch(nID)
		{
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
		case 38:
		case 39:
		default:
			break;
		}
	}
}

void CGuiPetDialog::RenderToolTip(const t_PetData* pData,int index)
{
	if (GetGuiManager()->GetMouseOverDlg()!=this)
		return ;	

	stPointI pt(Engine_GetCursor()->GetPosition());
	pt -= GetLocation();
	for(ToolTipRectIt it = m_toolTipRects.begin();it != m_toolTipRects.end();++it)
	{
		stToolTipRect& tip = *it;
		if(index == tip.index && tip.rect.PtInRect(pt))
		{
			m_ToolTips.Clear();
			m_ToolTips.SetBorderColor(0);
			m_ToolTips.SetBkColor(COLOR_ARGB(128,0,0,0));
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
			m_ToolTips.SetAlignType( GUI_ALIGN_LEFT );
			switch(tip.key)
			{
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			case 38:
			case 39:
				{
					RenderHorseToolTips(tip.key);
				}
				break;
			case 40:
				{
					m_ToolTips.AddText("坐骑目前的训练等级。可在王城的战马训练师处对坐骑进行训练，训练的等级越高，坐骑的能力提升越高");
				}
				break;
			case 41:
				{
					m_ToolTips.AddText("被训练后坐骑增加的移动速度。同等级多次训练中，增加的移动速度将为训练的最高值");
				}
				break;
			case 42:
				{
					m_ToolTips.AddText("训练效果的剩余时间，同等级的训练效果时间可以叠加");
				}
				break;
			case 100:
			case 200:
				{
					m_ToolTips.SetCurColor(-1);
					m_ToolTips.AddText("当前生命值 = ");
					m_ToolTips.AddText(FormatLargeNumber(pData->hp).c_str()); //by=>friday 使用FormatLargeNumber格式化
					m_ToolTips.AddText("\n最大生命值 = ");
					m_ToolTips.AddText(FormatLargeNumber(pData->maxhp).c_str()); //by=>friday 使用FormatLargeNumber格式化
					if(pData->maxhp_plus > 0)
					{
						m_ToolTips.SetCurColor(plusColor);
						m_ToolTips.AddText(" + ");
						m_ToolTips.AddText(FormatLargeNumber(pData->maxhp_plus).c_str()); //by=>friday 使用FormatLargeNumber格式化
						m_ToolTips.SetCurColor(-1);
					}
				}
				break;
			case 101:
			case 201://str
				{
					RenderPetToolTips("当前力量 = ",pData->str);
				}
				break;
			case 102:
			case 202://intel
				{
					RenderPetToolTips("当前智力 = ",pData->intel);
				}
				break;
			case 103:
			case 203://agi
				{
					RenderPetToolTips("当前敏捷 = ",pData->agi);
				}
				break;
			case 104:
			case 204://men
				{
					RenderPetToolTips("当前精神 = ",pData->men);
				}
				break;
			case 105:
			case 205://vit
				{
					RenderPetToolTips("当前体质 = ",pData->vit);
				}
				break;
			case 106:
			case 206://exp-maxexp
				{
					RenderPetToolTips("当前经验值 = ",pData->exp);
					RenderPetToolTips("\n最大经验值 = ",pData->maxexp);
				}
				break;
			case 107:
			case 207:
				{
					m_ToolTips.SetCurColor(-1);
					m_ToolTips.AddText("当前攻击力 = ");
					m_ToolTips.AddText(FormatLargeNumber(pData->atk).c_str()); //by=>friday 使用FormatLargeNumber格式化
					if(pData->atk_plus>0)
					{
						m_ToolTips.SetCurColor(plusColor);
						m_ToolTips.AddText(" + ");
						m_ToolTips.AddText(FormatLargeNumber(pData->atk_plus).c_str()); //by=>friday 使用FormatLargeNumber格式化
						m_ToolTips.SetCurColor(-1);
					}
					m_ToolTips.AddText("\n最大攻击力 = ");
					m_ToolTips.AddText(FormatLargeNumber(pData->maxatk).c_str()); //by=>friday 使用FormatLargeNumber格式化
					if(pData->maxatk_plus > 0)
					{
						m_ToolTips.SetCurColor(plusColor);
						m_ToolTips.AddText(" + ");
						m_ToolTips.AddText(FormatLargeNumber(pData->maxatk_plus).c_str()); //by=>friday 使用FormatLargeNumber格式化
						m_ToolTips.SetCurColor(-1);
					}
				}
				break;
			case 108:
			case 208:
				{
					m_ToolTips.SetCurColor(-1);
					m_ToolTips.AddText("当前魔法攻击力 = ");
					m_ToolTips.AddText(FormatLargeNumber(pData->matk).c_str()); //by=>friday 使用FormatLargeNumber格式化
					if(pData->matk_plus>0)
					{
						m_ToolTips.SetCurColor(plusColor);
						m_ToolTips.AddText(" + ");
						m_ToolTips.AddText(FormatLargeNumber(pData->matk_plus).c_str()); //by=>friday 使用FormatLargeNumber格式化
						m_ToolTips.SetCurColor(-1);
					}
					m_ToolTips.AddText("\n最大魔法攻击力 = ");
					m_ToolTips.AddText(FormatLargeNumber(pData->maxmatk).c_str()); //by=>friday 使用FormatLargeNumber格式化
					if(pData->maxmatk_plus > 0)
					{
						m_ToolTips.SetCurColor(plusColor);
						m_ToolTips.AddText(" + ");
						m_ToolTips.AddText(FormatLargeNumber(pData->maxmatk_plus).c_str()); //by=>friday 使用FormatLargeNumber格式化
						m_ToolTips.SetCurColor(-1);
					}
				}
				break;
			case 109:
			case 209:
				{
					m_ToolTips.SetCurColor(-1);
					m_ToolTips.AddText("当前物理防御 = ");
					m_ToolTips.AddText(FormatLargeNumber(pData->def).c_str()); //by=>friday 使用FormatLargeNumber格式化
					if(pData->pdef_plus > 0)
					{
						m_ToolTips.SetCurColor(plusColor);
						m_ToolTips.AddText(" + ");
						m_ToolTips.AddText(FormatLargeNumber(pData->pdef_plus).c_str()); //by=>friday 使用FormatLargeNumber格式化
						m_ToolTips.SetCurColor(-1);
					}
				}
				break;
			case 110:
			case 210:
				{
					m_ToolTips.SetCurColor(-1);
					m_ToolTips.AddText("当前魔法防御 = ");
					m_ToolTips.AddText(FormatLargeNumber(pData->mdef).c_str()); //by=>friday 使用FormatLargeNumber格式化
					if(pData->mdef_plus > 0)
					{
						m_ToolTips.SetCurColor(plusColor);
						m_ToolTips.AddText(" + ");
						m_ToolTips.AddText(FormatLargeNumber(pData->mdef_plus).c_str()); //by=>friday 使用FormatLargeNumber格式化
						m_ToolTips.SetCurColor(-1);
					}
				}
				break;
			case 111:
			case 211:
				{
					RenderPetToolTips("当前爆击率 = ",pData->cri);
				}
				break;
			case 312:
				{
					if( m_pCurCartoonPet )
					{
						RenderPetToolTips("当前生命值 = ",m_pCurCartoonPet->GetData().sp);
						RenderPetToolTips("\n最大生命值 = ",m_pCurCartoonPet->GetData().maxSp);
					}
				}
				break;
			case 313:
				{
					if( m_pCurCartoonPet )
					{
						RenderPetToolTips("等级 = ",m_pCurCartoonPet->GetData().lv);
					}
				}
				break;
			case 315:
				{
					if( m_pCurCartoonPet )
					{
						char szText[32];
						m_ToolTips.SetCurColor(-1);
						_snprintf(szText,32,"状态:%s",CCartoonPet::CartoonState2String(m_pCurCartoonPet));
						szText[31] = 0;
						m_ToolTips.AddText(szText);
					}
				}
				break;
			case 1000:
				{		
					if( m_pCurCartoonPet )
					{
						RenderPetToolTips("当前经验值 = ",m_pCurCartoonPet->GetData().exp);
						RenderPetToolTips("\n最大经验值 = ",m_pCurCartoonPet->GetData().maxExp);
					}
				}
				break;
			default:
				{
					m_ToolTips.Clear();
					m_ToolTips.Resize();
					return ;
				}
				break;
			}

			const char* psz = m_ToolTips.GetText();
			if( psz && strlen(psz)>0 )
			{
				m_ToolTips.Resize();
				stRectI rect = tip.rect;
				rect.OffsetRect(GetX(),GetY());
				m_ToolTips.RenderTopLeftPrefer(rect,GetDevice()->GetWidth(),GetDevice()->GetHeight());
			}
		}		
	}
}

bool CGuiPetDialog::OnHorseFollowMaster()
{
	CHorse* pHorse = NULL;
	CPet* pPet = GetScene()->FindPetByType( PET_TYPE_RIDE );
	if( pPet )
	{
		pHorse = static_cast<CHorse*>( pPet );
	}
	if(pHorse)
	{
		horseState state = pHorse->GetHorseState();
		switch(state)
		{
		case HORSE_STATE_PUTUP: pHorse->Follow();break;
		case HORSE_STATE_FOLLOW: pHorse->Putup();break;
		case HORSE_STATE_RIDE: //pHorse->Follow();break;
		default:break;
		}
	}
	return true;
}

bool CGuiPetDialog::OnHorseRemove()
{
	GetGameGuiManager()->AddMessageBox("你确定要弃马吗？", eAction_RemoveHorseConfirm, NULL,"确定|1|Enter|取消|2|Esc");
	return true;
}

bool CGuiPetDialog::OnPetRename()
{
	if(GetEditBox(112)->IsEnabled())
	{
		CPet* pPet = GetScene()->FindPetByType( PET_TYPE_PET );
		char name[MAX_NAMESIZE];
		strcpy(name,GetEditBox(112)->GetText());
		str_trim_right(name);
		str_trim_left(name);
		if( pPet && strlen(name) > 0) 
		{
			pPet->Rename(name);
			GetEditBox(112)->SetText("");
			Pet_EnableCtrl(112,false);
		}
		else
		{
			GetGameGuiManager()->AddMessageBox("宠物名字不能为空");
		}
	}
	else
	{
		Pet_EnableCtrl(112,true);
		SetFocus(GetEditBox(112));
	}

	return true;
}

bool CGuiPetDialog::OnSummonRename()
{
	if(GetEditBox(212)->IsEnabled())
	{
		CPet* pPet = GetScene()->FindPetByType( PET_TYPE_SUMMON );
		char name[MAX_NAMESIZE];
		strcpy(name,GetEditBox(212)->GetText());
		str_trim_right(name);
		str_trim_left(name);
		if( pPet && strlen(name) > 0) 
		{
			pPet->Rename(name);
			GetEditBox(212)->SetText("");
			Pet_EnableCtrl(212,false);
		}
	}
	else
	{
		Pet_EnableCtrl(212,true);
		SetFocus(GetEditBox(212));
	}
	return true;
}

bool CGuiPetDialog::OnCartoonRename()
{
	if( m_pCurCartoonPet )
	{
		CGuiEditBox* pEdit = GetEditBox(311);
		if( pEdit )
		{
			if( pEdit->IsEnabled() )
			{
				const char* psz = pEdit->GetText();
				m_pCurCartoonPet->Rename(psz);
				pEdit->SetEnabled( false );
			}
			else
			{
				pEdit->SetEnabled( true );
			}
		}

//#ifdef _DEBUG
//		if(m_pCurCartoonPet->GetData().exp < m_pCurCartoonPet->GetData().maxExp)
//			m_pCurCartoonPet->GetData().exp += 10;
//#endif
	}
	return true;
}

bool CGuiPetDialog::OnCartoonAddCent()
{
	if( m_pCurCartoonPet )
	{
		GetGameGuiManager()->AddCartoonAddCentDialog(m_pCurCartoonPet->GetCartoonID());
		Pet_DelayEnable(scCartoonAddCentID,true);
	}
	
	return true;
}

bool CGuiPetDialog::OnCartoonPutout()	//跟随
{
	if( m_pCurCartoonPet )
	{
		CCartoonPet::CartoonFollow(m_pCurCartoonPet->GetCartoonID());
		Pet_EnableCtrl(301,false);
	}
	return true;
}

bool CGuiPetDialog::OnCartoonPutin()	//取消跟随
{
	if( m_pCurCartoonPet )
	{
		CCartoonPet::CartoonFollow(0);
		Pet_EnableCtrl(308,false);
	}
	return true;
}

bool CGuiPetDialog::OnCartoonNext()
{
	if( m_pCurCartoonPet )
	{
		CCartoonPet* pCartoon = m_pCurCartoonPet->GetNextCartoon();
		SetCurCartoonPet( pCartoon );
	}
	return true;
}

bool CGuiPetDialog::OnCartoonPre()
{
	if( m_pCurCartoonPet )
	{
		CCartoonPet* pCartoon = m_pCurCartoonPet->GetPreCartoon();
		SetCurCartoonPet( pCartoon );
	}
	
	return true;
}

bool CGuiPetDialog::OnCartoonFeed()		//喂养
{
	Pet_DelayEnable(scCartoonFeedID,true);
	return true;
}

bool CGuiPetDialog::OnCartoonStartTraining()	//开始修炼
{
	if( m_pCurCartoonPet )
	{
		CCartoonPet::CartoonLetOut(m_pCurCartoonPet->GetCartoonID());
	}
	return true;
}

bool CGuiPetDialog::OnCartoonStopTraining()		//终止修炼
{
	CCartoonPet::GetBackCartoon();
	return true;
}

bool CGuiPetDialog::OnCartoonGetExp()
{
	if( m_pCurCartoonPet )
	{
		CCartoonPet::DrawExpCartoon(m_pCurCartoonPet->GetCartoonID());
		Pet_DelayEnable(scCartoonGetExpID,true);
	}
	return true;
}

bool CGuiPetDialog::OnStartAutoRepair()
{
	if( m_pCurCartoonPet )
	{
		CCartoonPet::AutoRepair(m_pCurCartoonPet->GetCartoonID(),(BYTE)1);
		Pet_DelayEnable(318,true);

	}
	return true;
}

bool CGuiPetDialog::OnEndAutoRepair()
{
	if( m_pCurCartoonPet )
	{
		CCartoonPet::AutoRepair(m_pCurCartoonPet->GetCartoonID(),(BYTE)0);
		Pet_DelayEnable(319,true);
	}
	return true;
}


bool CGuiPetDialog::OnAddZuoqi()
{
	if(GetGameGuiManager()->m_guiZuoqi && ((CGuiDialog*)GetGameGuiManager()->m_guiZuoqi)->IsVisible())
	{
		((CGuiDialog*)GetGameGuiManager()->m_guiZuoqi)->SetVisible(false);
	}
	else
	{
				
		GetGameGuiManager()->AddZuoqi();
	}
	return true;
}

void CGuiPetDialog::SetCurCartoonPet( CCartoonPet* pCartoon )
{
	m_pCurCartoonPet = pCartoon;
	if( m_pCurCartoonPet == NULL )
	{
		//clear text
		//...
		SetPetImage(310,-1);
		Pet_SetStaticText(312,"");
		Pet_SetStaticText(313,"");
		Pet_SetStaticText(315,"");
		Pet_SetStaticText(316,"");
		Pet_SetStaticText(317,"");
		Pet_ShowCtrl(308,false);	//收回
		Pet_EnableCtrl(300,false);	//充值
		Pet_EnableCtrl(301,false);	//放出
		Pet_EnableCtrl(302,false);	//上一个
		Pet_EnableCtrl(303,false);	//终止修炼
		Pet_EnableCtrl(304,false);	//下一个
		Pet_EnableCtrl(305,false);	//修改名字
		Pet_EnableCtrl(306,false);	//喂食
		Pet_EnableCtrl(307,false);	//提取经验值
		Pet_EnableCtrl(309,false);	//开始修炼
		Pet_EnableCtrl(318,false);	//开始自动修理
		Pet_EnableCtrl(319,false);	//关闭自动修理
		//Pet_EnableCtrl(311,false);	//名字输入
		Pet_ShowCtrl(303,false);	//终止修炼

		//名字
		{
			CGuiEditBox* pEdit = GetEditBox( 311 );
			if( pEdit )
			{
				pEdit->SetText( "" );
				pEdit->SetEnabled( false );
			}
		}

		SetCartoonPetImage(310,NULL);
	}
	else
	{
		//set cartoon text
		//...	
		Pet_EnableCtrl( 302,m_pCurCartoonPet->IsHavePre() );			//上一个
		Pet_EnableCtrl( 304,m_pCurCartoonPet->IsHaveNext() );			//下一个

		Pet_DelayEnable( 300,m_pCurCartoonPet->IsCanAddCent() );		//充值
		Pet_DelayEnable( 305,true );									//修改名字
		Pet_DelayEnable( 306,m_pCurCartoonPet->IsCanFeed());			//喂养
		Pet_DelayEnable( 307,m_pCurCartoonPet->IsCanFetchExp());		//提取经验值

		Pet_ShowAndEnableGrouped( 301,308,m_pCurCartoonPet->IsCanFollow(),m_pCurCartoonPet->IsCanPickup() );					//放出/收回
		Pet_ShowAndEnableGrouped( 303,309,m_pCurCartoonPet->IsCanEndTraining(),m_pCurCartoonPet->IsCanStartTraining() );		//终止修炼/开始修炼
		Pet_ShowAndEnableGrouped( 318,319,m_pCurCartoonPet->IsCanStartAutoRepair(),m_pCurCartoonPet->IsCanEndAutoRepari() );	//自动修理开启/关闭

		Debug_Bool(m_pCurCartoonPet->IsCanFollow());
		Debug_Bool(m_pCurCartoonPet->IsCanPickup());
		Debug_Bool(m_pCurCartoonPet->IsCanEndTraining());
		Debug_Bool(m_pCurCartoonPet->IsCanStartTraining());
		Debug_Bool(m_pCurCartoonPet->IsCanStartAutoRepair());
		Debug_Bool(m_pCurCartoonPet->IsCanEndAutoRepari());

		t_CartoonData& data = m_pCurCartoonPet->GetData();
		char szTemp[64] = {0};

		//精气值
		_snprintf(szTemp,64,"%d-%d",data.sp,data.maxSp);
		szTemp[63] = 0;
		Pet_SetStaticText(312,szTemp);

		//等级
		_snprintf(szTemp,64,"%d",data.lv);
		szTemp[63] = 0;
		Pet_SetStaticText(313,szTemp);

		//状态
		const char* state = CCartoonPet::CartoonState2String(m_pCurCartoonPet);
		Pet_SetStaticText(315,state);

		//剩余时间
		_snprintf( szTemp,64,"剩余修炼时间:%s",Second2String(data.time) );
		szTemp[63] = 0;
		Pet_SetStaticText(316,szTemp);

		//图片
		SetCartoonPetImage(310,m_pCurCartoonPet);

		//可以提取的经验值
		_snprintf( szTemp,64,"可以提取的经验值:%d",data.addExp );
		szTemp[63] = 0;
		Pet_SetStaticText(317,szTemp);
		
		//名字
		{
			CGuiEditBox* pEdit = GetEditBox( 311 );
			if( pEdit )
			{
				pEdit->SetText( data.name );
				pEdit->SetEnabled( false );
			}
		}
	}
}

void CGuiPetDialog::OnRemoveCartoonPet( DWORD dwCartoonID )
{
	if( 3 != GetTab(scTabid)->GetCurItem() )
	{
		return ;
	}
	if( m_pCurCartoonPet && m_pCurCartoonPet->GetCartoonID() == dwCartoonID )
	{
		if( m_pCurCartoonPet->IsHavePre() )
		{
			CCartoonPet* pCartoon = m_pCurCartoonPet->GetPreCartoon();
			SetCurCartoonPet( pCartoon );
			return ;
		}
		else if( m_pCurCartoonPet->IsHaveNext() )
		{
			CCartoonPet* pCartoon = m_pCurCartoonPet->GetNextCartoon();
			SetCurCartoonPet( pCartoon );
			return ;
		}
		else 
		{
			m_pCurCartoonPet = NULL;
		}
	}
	SetCurCartoonPet( m_pCurCartoonPet );
}

void CGuiPetDialog::OnUpdateCartoonPet( DWORD dwCartoonID )
{
	if( 3 != GetTab(scTabid)->GetCurItem() )
	{
		return ;
	}
	if( m_pCurCartoonPet && m_pCurCartoonPet->GetCartoonID() == dwCartoonID )
	{
		CCartoonPet* pCartoon = GetScene()->GetCartoonPetList()->GetCartoon( dwCartoonID );
		SetCurCartoonPet( pCartoon );
		return ;
	}
	if( NULL == m_pCurCartoonPet )
	{
		m_pCurCartoonPet = GetScene()->GetCartoonPetList()->GetCartoon( dwCartoonID );
		SetCurCartoonPet( m_pCurCartoonPet );
		return ;
	}
	else
	{
		SetCurCartoonPet( m_pCurCartoonPet );
	}
}

void CGuiPetDialog::ConsignAdoptCartoon(const char* pszName)	//指定pszName领养当前页面孩子
{
	CCartoonPet* pCartoon = NULL;
	if( GetGameGuiManager()->m_guiPetDlg )
	{
		pCartoon = GetGameGuiManager()->m_guiPetDlg->m_pCurCartoonPet;
	}
	if( pCartoon == NULL )
	{
		pCartoon = GetScene()->GetCartoonPetList()->GetAt(0);
	}
	if( pCartoon )
	{
		if( pCartoon->GetState() != CARTOON_STATE_WAITING &&
			pCartoon->GetState() != CARTOON_STATE_PUTAWAY )
		{
			GetGameGuiManager()->AddClientSystemMessage("你的当前替身宝贝未处于\"等待领养\"或\"收起\"状态下操作无效");
			return ;
		}
		if( pCartoon->GetData().time <= 0 )
		{
			GetGameGuiManager()->AddClientSystemMessage("你的当前替身宝贝余额不足操作无效");
			return ;
		}
		if( GetScene()->GetCartoonPetList()->IsHaveAdoptedCartoonPet() )
		{
			GetGameGuiManager()->AddClientSystemMessage("你已有一个替身宝贝\"被领养\"操作无效");
			return ;
		}
		stConsignCartoonCmd cmd;
		cmd.cartoonID = pCartoon->GetCartoonID();
		_snprintf(cmd.name,MAX_NAMESIZE,pszName);
		cmd.name[MAX_NAMESIZE - 1] = 0;
		SEND_USER_CMD(cmd);
	}
	else
	{
		GetGameGuiManager()->AddClientSystemMessage("你当前没有替身宝贝可供领养");
	}
}