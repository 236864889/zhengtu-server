/**
 *\file		
 *\version	$ID$
 *\author	$$$04公司
 *			$$$04公司@###.com
 *\date		2010-05-15 21:35
 *\brief	自动挂机对话框
 *\name     star
 * 任务对话框：显示任务、任务描述、任务难度
 *
 */
/////////////////////star100515     自动挂机对话框
#include "public.h"
#include "./table.h"
#include "./roleskill.h"
#include "./Game.h"
#include "./GameGuiManager.h"
#include "./GameScene.h"
#include "./GameGuiManager.h"
#include "./MainCharacter.h"
#include "./GameAppation.h"
#include "./GuiMain.h"
#include "./GuiSystem.h"
#include "./GuiAutoAttackDlg.h" 
// 需要在GuiAutoAttackDlg.cpp文件顶部添加以下头文件
#include "./RoleItem.h"
#include "./AutoGame.h"
#include "./MainCharacter.h"
#include "../magic/objectbase.h"


#define SendMailDlg_ButtonSetEnable(id,bEnable)	{CGuiButton* pBtn = GetButton(id);if( NULL != pBtn ){ pBtn->SetEnabled(bEnable);}}
#define SendMailDlg_ButtonSetVisible(id,bShow)	{CGuiButton* pBtn = GetButton(id);if( NULL != pBtn ){ pBtn->SetVisible(bShow);}}
#define SendMailDlg_ButtonGetCheck(id,bCheck)	{CGuiButton* pBtn = GetButton(id);if( NULL != pBtn ){ bCheck = pBtn->GetChecked();}}
#define SendMailDlg_ButtonSetCheck(id,bCheck)   {CGuiButton* pBtn = GetButton(id);if( NULL != pBtn ){ pBtn->SetChecked(bCheck);}}
#define SendMailDlg_EditSetEnable(id,bEnable)	{CGuiEditBox* pEdit = GetEditBox(id);if(NULL != pEdit){ pEdit->SetEnabled(bEnable);}}	
#define SendMailDlg_EditSetVisible(id,bShow)	{CGuiEditBox* pEdit = GetEditBox(id);if(NULL != pEdit){ pEdit->SetVisible(bShow);}}	
#define SendMailDlg_EditClear(id)				{CGuiEditBox* pEdit = GetEditBox(id);if(NULL != pEdit){ pEdit->ClearText();}}	
#define SendMailDlg_SetCheck(id,bCheck)         {CGuiCheckBox* Check = GetCheckBox(id);if( NULL != Check ){ Check->SetChecked(bCheck);}} //新加
//#define SendMailDlg_Edit_ClearAndHide(id) SendMailDlg_EditClear(id) SendMailDlg_EditSetVisible(id,false)

static const int BUTTEN_STATE					= 25; // 开启挂机
static const int BUTTEN_ATTACK					= 26; // 自动打怪
static const int BUTTEN_ORIGIN					= 27; // 定点打怪
static const int BUTTEN_RANGE					= 28; // 范围打怪
static const int EDITBO_X						= 30; // 范围 X
static const int EDITBO_Y						= 31; // 范围 Y
static const int EDITBO_RANGE				    = 33; // 半径

static const int BUTTEN_AUTOMAGIC				= 305; // 自动释放技能
static const int BUTTEN_AUTOTEAM				= 36;  // 自动组队
static const int BUTTEN_AUTOPICKUP				= 34;  // 远程拾取
static const int BUTTEN_PICKUPSETUP				= 45;  // 物品过滤设置
static const int COMBOBOX_PUT_SKILL	    		= 301; // 技能框 左右长度
static const int LIST_SHOW_SKILL                = 304; // 技能框 上下高度
static const int BUTTEN_ADDLIST_SHILL			= 302; // 技能增加按钮
static const int BUTTEN_DELLIST_SHILL			= 303; // 技能减少按钮

static const int BUTTEN_PROTECTION_HP			= 200; // 红药使用
static const int BUTTEN_PROTECTION_MP			= 201; // 蓝药使用
static const int BUTTEN_PROTECTION_HP_20HAVE    = 202; // 红药购买
static const int BUTTEN_PROTECTION_MP_20HAVE    = 203; // 蓝药购买
static const int EDITBO_PUT_HP					= 208; // 生命值%
static const int EDITBO_PUT_MP	    			= 209; // 法术值%
static const int COMBOBOX_PUT_HP	    		= 51;  // 红药列表
static const int COMBOBOX_PUT_MP	    		= 52;  // 蓝药列表

//////////////////star100604
static const int BUTTEN_DS_AID					= 60; // 开启辅助
static const int BUTTEN_DS_RESUME				= 61; // 自动打坐
static const int BUTTEN_DS_TEAMERRESURRECTION	= 62; // 复活队友
static const int BUTTEN_DS_SLEALTH				= 80; // 自动隐身
static const int EDITBO_X_SIT					= 63; // 打坐 x
static const int EDITBO_Y_SIT					= 64; // 打坐 y
static const int BUTTEN_DS_THISPOS				= 65; // 当前坐标

//////////////////end
static const int BUTTEN_SAVE        			= 23; // 保存设置

//const char* TXT_MAGICLIST_SETUP = "技能列表设置：";
//const char* TXT_TEAM_SETUP = "队员列表设置：";
//const char* TXT_PICKUP_SETUP = "关键字设置,输入则不拾取：";
//const stRectI g_sRect1(18 +219,97 +92,105 +219,109 +92);
//const stRectI g_sRect2(179+219,97 +92,266 +219,109 +92);
//const stRectI g_sRect3(370+219,97 +92,515 +219,109 +92);
//
//const char* TXT_HP = "生命值低于";
//const char* TXT_MP = "魔法值低于";
//const char* TXT_ADD_HP = "O%自动使用红药";
//const char* TXT_ADD_MP = "O%自动使用蓝药";
//const char* TXT_HAVE20 = "保证包内有20个";
//
//const stRectI g_sRectHP    ( 66 +219,260 +92,128 +219,274 +92);
//const stRectI g_sRectMP    ( 66 +219,286 +92,128 +219,299 +92);
//const stRectI g_sRectADD_HP(156 +219,260 +92,242 +219,274 +92);
//const stRectI g_sRectADD_MP(156 +219,286 +92,242 +219,299 +92);
//
//const stRectI g_sRectHAVE20_up(  356+219,260 +92,442 +219,274 +92);
//const stRectI g_sRectHAVE20_down(356+219,286 +92,442 +219,299 +92);


static const int TIME_GOAWAY_ATTRAGIN  = 30;
////////////////////star100604
extern bool g_bSitDownHotKeyActive;
extern bool g_bAutoFight;
extern void OnSitDown();

CGuiAutoAttack::CGuiAutoAttack()
{
	FUNCTION_BEGIN;

	m_bCloseIsHide			= true;

	m_fBegin_State			= false;
	m_fBegin_Attack			= false;
	m_fBegin_Origin			= false;
	m_fBegin_Range			= false;
	m_fAuto_UseMagic		= false;
	m_fAuto_InTeam			= false;
	m_fAuto_PicUp			= false;
	m_fAuto_PicUp_Setup		= false;
	m_fProtection_HP        = false;;
	m_fProtection_MP        = false;;
	m_fProtection_HP_20have = false;;
	m_fProtection_MP_20have = false;;

	m_pEditBox_PutX = NULL;		
	m_pEditBox_PutY = NULL;		
	m_pEditBox_Range= NULL;			
	m_pSkillList    = NULL;
//////////star100604
	m_fBegin_DS_Aid					= false;
	m_fAuto_DS_Resume				= false;
	m_fAuto_DS_TeamerResurrection	= false;
	m_fAuto_DS_Stealth				= false;

	m_iSitdown_PosX					= 0;
	m_iSitdown_PosY					= 0;

	m_fAniToWait					= false;
//////////end

	// 初始化装备颜色过滤变量
	m_fFilter_WhiteEquip = false;
	m_fFilter_BlueEquip = false;
	m_fFilter_GoldEquip = false;
	m_fFilter_GreenEquip = false;
	//by=>friday

	FUNCTION_END;

}
CGuiAutoAttack::~CGuiAutoAttack()
{
}
void CGuiAutoAttack::OnCreate()
{
	FUNCTION_BEGIN;
	CGuiDialog::OnCreate();

	m_pImage[0]=GetImage(2); 
	m_pImage[1]=GetImage(3);
	
	GetButton(11)->SetButtonGroup(1);
	GetButton(12)->SetButtonGroup(1);	
	
	m_tab = GetTab(10);

	m_tab->AddItem("",NULL);
	m_tab->AddItem("",NULL);
	
	m_tab->AddControl(0,(CGuiControl*)GetImage(0));
	m_tab->AddControl(0,(CGuiControl*)GetButton(11));
	m_tab->AddControl(0,(CGuiControl*)GetButton(12));
	m_tab->AddControl(0,(CGuiControl*)GetButton(14));
	m_tab->AddControl(0,(CGuiControl*)GetButton(15));
	m_tab->AddControl(0,(CGuiControl*)GetButton(22));
	m_tab->AddControl(0,(CGuiControl*)GetButton(23));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(25));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(26));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(27));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(28));
	m_tab->AddControl(0,GetControl(29));
	m_tab->AddControl(0,(CGuiControl*)GetEditBox(30));
	m_tab->AddControl(0,(CGuiControl*)GetEditBox(31));
	m_tab->AddControl(0,GetControl(32));
	m_tab->AddControl(0,(CGuiControl*)GetEditBox(33));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(34));
	m_tab->AddControl(0,(CGuiControl*)GetComboBox(35));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(36));
	m_tab->AddControl(0,(CGuiControl*)GetButton(37));
	m_tab->AddControl(0,(CGuiControl*)GetButton(38));
	m_tab->AddControl(0,(CGuiControl*)GetListBox(41));
	m_tab->AddControl(0,(CGuiControl*)GetMLEditBox(43));
	m_tab->AddControl(0,(CGuiControl*)GetButton(45));
	m_tab->AddControl(0,GetControl(47));
	m_tab->AddControl(0,GetControl(48));
	m_tab->AddControl(0,(CGuiControl*)GetComboBox(51));
	m_tab->AddControl(0,(CGuiControl*)GetComboBox(52));
	m_tab->AddControl(0,GetControl(199));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(200));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(201));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(202));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(203));
	m_tab->AddControl(0,GetControl(204));
	m_tab->AddControl(0,GetControl(205));
	m_tab->AddControl(0,GetControl(206));
	m_tab->AddControl(0,GetControl(207));
	m_tab->AddControl(0,(CGuiControl*)GetEditBox(208));
	m_tab->AddControl(0,(CGuiControl*)GetEditBox(209));
	m_tab->AddControl(0,GetControl(220));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(221));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(222));
	m_tab->AddControl(0,GetControl(223));
	m_tab->AddControl(0,GetControl(224));
	m_tab->AddControl(0,(CGuiControl*)GetEditBox(225));
	m_tab->AddControl(0,(CGuiControl*)GetComboBox(226));
	m_tab->AddControl(0,(CGuiControl*)GetComboBox(227));
	m_tab->AddControl(0,GetControl(300));
	m_tab->AddControl(0,(CGuiControl*)GetComboBox(301));
	m_tab->AddControl(0,(CGuiControl*)GetButton(302));
	m_tab->AddControl(0,(CGuiControl*)GetButton(303));
	m_tab->AddControl(0,(CGuiControl*)GetListBox(304));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(305));
	
	// 添加新增的装备颜色过滤控件
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(90));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(91));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(92));
	m_tab->AddControl(0,(CGuiControl*)GetCheckBox(93));
	//by=>friday
	
	m_tab->AddControl(1,(CGuiControl*)GetImage(1));
	m_tab->AddControl(1,(CGuiControl*)GetButton(11));
	m_tab->AddControl(1,(CGuiControl*)GetButton(12));
	m_tab->AddControl(1,(CGuiControl*)GetButton(14));
	m_tab->AddControl(1,(CGuiControl*)GetButton(15));
	m_tab->AddControl(1,(CGuiControl*)GetButton(22));
	m_tab->AddControl(1,(CGuiControl*)GetButton(23));
	m_tab->AddControl(1,(CGuiControl*)GetCheckBox(60));
	m_tab->AddControl(1,(CGuiControl*)GetCheckBox(61));
	m_tab->AddControl(1,(CGuiControl*)GetCheckBox(62));
	m_tab->AddControl(1,(CGuiControl*)GetEditBox(63));
	m_tab->AddControl(1,(CGuiControl*)GetEditBox(64));
	m_tab->AddControl(1,(CGuiControl*)GetCheckBox(80));
	m_tab->AddControl(1,GetControl(73));
	m_tab->AddControl(1,(CGuiControl*)GetButton(65));
	
	m_tab->SetVisible(false);
	
	OnGuiEvent(EVENT_BUTTON_CLICKED, 11, GetButton(11));
	
	m_pEditBox_PutX  = GetEditBox(EDITBO_X);
	m_pEditBox_PutY  = GetEditBox(EDITBO_Y);
	m_pEditBox_Range = GetEditBox(EDITBO_RANGE);
/////////////////////star100604
	m_pEditBox_Sitdown_PutX  = GetEditBox(EDITBO_X_SIT);	
	m_pEditBox_Sitdown_PutY  = GetEditBox(EDITBO_Y_SIT);		
/////////////////////end
	///////////star100719
	m_iNeedToProtect_HP = 50 ;
	m_iNeedToProtect_MP = 50;

	m_pEditBox_NeedToProtect_HP = GetEditBox(EDITBO_PUT_HP);
	m_pEditBox_NeedToProtect_MP = GetEditBox(EDITBO_PUT_MP);
	m_pEditBox_NeedToProtect_HP->SetText( "50" );
	m_pEditBox_NeedToProtect_MP->SetText( "50" );
	///////////

	m_iRange_PosX = 0;
	m_iRange_PosY = 0;
	m_iRange_Range= 0;
	//m_iNeedToProtect_HP = 0 ;
	m_iComboBox_HP = 0;
	m_iComboBox_MP = 0;
	//m_iNeedToProtect_MP = 0 ;
	memset(m_dwNeedMedicine_HP, 0, sizeof(m_dwNeedMedicine_HP));
	memset(m_dwNeedMedicine_MP, 0, sizeof(m_dwNeedMedicine_MP));

	m_dwNeedMedicine_HP[0] = 578;
	m_dwNeedMedicine_HP[1] = 579;
	m_dwNeedMedicine_HP[2] = 580;
	m_dwNeedMedicine_MP[0] = 585;
	m_dwNeedMedicine_MP[1] = 586;
	m_dwNeedMedicine_MP[2] = 587;

//strtoul(m_pDingEditBox->GetText(),NULL,10)
	stGameConfig config = *((CGameAppation*)Engine_GetAppation())->GetConfig();
	SetConfig(config);

 	m_pSkillList = GetListBox(LIST_SHOW_SKILL);
	if( NULL == m_pSkillList  )
	{
		GetGameGuiManager()->AddMessageBox("内挂脚本出错");
		Engine_WriteLog("内挂脚本出错");
		Close();
	}
	FUNCTION_END;
}


bool CGuiAutoAttack::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	FUNCTION_BEGIN;


	if(nEvent == EVENT_CHECKBOX_CHANGED)
	{
		switch(nID)
		{
		case BUTTEN_STATE:
			{
				OnBegin_State();
				return true;
			}
			break;
		case BUTTEN_ATTACK:
			{
				OnBegin_Attack();
				return true;
			}
			break;
		case BUTTEN_ORIGIN:
			{
				OnBegin_AttOrigin();
				return true;
			}
			break;
		case BUTTEN_RANGE:
			{
				OnBegin_AttRange();
				return true;
			}
			break;
		case BUTTEN_AUTOMAGIC:
			{
				OnAutoUseMagic();
				return true;
			}
			break;
		case BUTTEN_AUTOTEAM:
			{
				OnAutoInTeam();
				return true;
			}
			break;
		case BUTTEN_AUTOPICKUP:
			{
				OnAutoPicUp();
				return true;
			}
			break;
		case BUTTEN_PROTECTION_HP:
			{
				OnfProtection_HP();
				return true;
			}
			break;
		case BUTTEN_PROTECTION_MP:
			{
				OnfProtection_MP();
				return true;
			}
			break;
		case BUTTEN_PROTECTION_HP_20HAVE:
			{
				OnfProtection_HP_20have();
				return true;
			}
			break;
		case BUTTEN_PROTECTION_MP_20HAVE:
			{
				OnfProtection_MP_20have();
				return true;
			}
			break;
		case BUTTEN_DS_AID:
			{
				OnBegin_DS_Aid();
				return true;
			}
			break;
		case BUTTEN_DS_RESUME:
			{
				OnAuto_DS_Resume();
				return true;
			}
			break;
		case BUTTEN_DS_TEAMERRESURRECTION:
			{
				OnAuto_DS_TeamerResurrection();
				return true;
			}
			break;
		case BUTTEN_DS_SLEALTH:
			{
				OnAuto_DS_Stealth();
				return true;
			}
			break;
		case 90: // 过滤白色装备
			{
				OnFilterWhiteEquip();
				return true;
			}
			break;
		case 91: // 过滤蓝色装备
			{
				OnFilterBlueEquip();
				return true;
			}
			break;
		case 92: // 过滤金色装备
			{
				OnFilterGoldEquip();
				return true;
			}
			break;
		case 93: // 过滤绿色装备
			{
				OnFilterGreenEquip();
				return true;
			}
			break;
		}
	}

	if( nEvent == EVENT_BUTTON_CLICKED )
	{
		switch( nID )
		{
		//case BUTTEN_STATE:				return OnBegin_State();
		//case BUTTEN_ATTACK:				return OnBegin_Attack();
		//case BUTTEN_ORIGIN:					return OnBegin_AttOrigin();
		//case BUTTEN_RANGE:					return OnBegin_AttRange();

		//case BUTTEN_AUTOMAGIC:				return OnAutoUseMagic();
		//case BUTTEN_AUTOTEAM:				return OnAutoInTeam();
		//case BUTTEN_AUTOPICKUP:				return OnAutoPicUp();
		case BUTTEN_PICKUPSETUP:			return OnAutoPicUp_Setup();

		//case BUTTEN_PROTECTION_HP:			return OnfProtection_HP();
		//case BUTTEN_PROTECTION_MP:			return OnfProtection_MP();
		//case BUTTEN_PROTECTION_HP_20HAVE:	return OnfProtection_HP_20have();
		//case BUTTEN_PROTECTION_MP_20HAVE:	return OnfProtection_MP_20have();
		case BUTTEN_SAVE:					return OnSaveButtenCleck();

		case BUTTEN_ADDLIST_SHILL:			return OnAddListSkill();
		case BUTTEN_DELLIST_SHILL:			return OnDelListSkill();
//////////////////////////star100604
		//case BUTTEN_DS_AID:					return OnBegin_DS_Aid();
		//case BUTTEN_DS_RESUME:				return OnAuto_DS_Resume();
		//case BUTTEN_DS_TEAMERRESURRECTION:	return OnAuto_DS_TeamerResurrection();
		//case BUTTEN_DS_SLEALTH:				return OnAuto_DS_Stealth();
		case BUTTEN_DS_THISPOS:             return OnAuto_SetPosSit();
/////////////////////////end
        case 11:
			{
				m_tab->SetCurItem(0);
				GetButton(11)->SetChecked(true);
				SetBackImage(stResourceLocation("data\\interfaces.gl",75,0));
			}
			break;
		case 12:
			{
				m_tab->SetCurItem(1);
				GetButton(12)->SetChecked(true);
				SetBackImage(stResourceLocation("data\\interfaces.gl",75,1));
			}
			break;
		 case 14:
			{
				SetVisible(false);;
			}
			break;	
		}
	}
	if( nEvent == EVENT_DIALOG_SHOW )
	{
		stGameConfig config = *((CGameAppation*)Engine_GetAppation())->GetConfig();
		SetConfig_Skill(config);//SetConfig(config);
		return true;
	}

	return CGuiDialog::OnGuiEvent(nEvent,nID,pControl);
	FUNCTION_END;
}
HRESULT CGuiAutoAttack::OnRender(float fElapsedTime)
{
	FUNCTION_BEGIN;
	HRESULT hr = CGuiDialog::OnRender(fElapsedTime);
///////star100719
    stRectI g_sRect_top(310, 30 ,567,310);

	int iOldFont = GetDevice()->SetFont(1);
	g_sRect_top.OffsetRect(m_x,m_y);

	DWORD color = COLOR_ARGB(255,0,255,0);//soke 名字显示绿色
	GetDevice()->DrawString( "内挂 ", g_sRect_top,color );
	GetDevice()->SetFont( iOldFont );

///////end

	//GetDevice()->DrawString(TXT_MAGICLIST_SETUP,g_sRect1);
	//GetDevice()->DrawString(TXT_TEAM_SETUP,g_sRect2);
	//GetDevice()->DrawString(TXT_PICKUP_SETUP,g_sRect3);

	//GetDevice()->DrawString(TXT_HP,g_sRectHP);
	//GetDevice()->DrawString(TXT_MP,g_sRectMP);
	//GetDevice()->DrawString(TXT_ADD_HP,g_sRectADD_HP);
	//GetDevice()->DrawString(TXT_ADD_MP,g_sRectADD_MP);

	//GetDevice()->DrawString(TXT_HAVE20,g_sRectHAVE20_up);
	//GetDevice()->DrawString(TXT_HAVE20,g_sRectHAVE20_down);

	return hr;
	FUNCTION_END;
}
VOID CGuiAutoAttack::OnRender_AutoAtt(float fElapsedTime)
{
	static float f_tempTime = 0;
	OnProtection_Life();

	if( m_fBegin_State && GetGameApplication() && GetGameApplication()->bClientSetted(enumAuto_Kill_Summon) )
	{
		CGameScene* pScene = GetScene();
		CMainCharacter* pMainCharacter = pScene ? pScene->GetMainCharacter() : NULL;
		if( pMainCharacter )
		{
			if( m_fAuto_UseMagic )
				pMainCharacter->RunAutoWorkSkill();

			if( m_fAuto_PicUp )
				pMainCharacter->RunAutoPickup();
		}
	}

	f_tempTime += fElapsedTime;
	if( f_tempTime > TIME_GOAWAY_ATTRAGIN)
	{
		OnIsGoaway();
		//OnAuto_GotoSitdown();
		f_tempTime = 0;
	}

	static float f_tempTime_stand = 0;
	if( m_iAni == 1)
		f_tempTime_stand += fElapsedTime;
	else
		f_tempTime_stand = 0;

	
	static float f_tempTime_2 = 0;
	f_tempTime_2 += fElapsedTime;
	if( f_tempTime_2 > TIME_GOAWAY_ATTRAGIN -15 )
	{
		if( !m_fAniToWait)// && ) 
			return;
		if(f_tempTime_stand < TIME_GOAWAY_ATTRAGIN - 15)
			return;
		OnAuto_GotoSitdown();
		f_tempTime = 0;
	}

}
bool CGuiAutoAttack::OnBegin(bool f)
{
	if(f)
	{
		m_fBegin_State = true;
		m_fBegin_Attack = true;
	}
	else
	{
		m_fBegin_State = false;
		m_fBegin_Attack = false;
	
	}
	SendMailDlg_ButtonSetCheck(BUTTEN_STATE,m_fBegin_State);
	SendMailDlg_ButtonSetCheck(BUTTEN_ATTACK,m_fBegin_Attack);
	return true;
}

bool CGuiAutoAttack::OnBegin_State()
{
	m_fBegin_State = !m_fBegin_State;
	SendMailDlg_ButtonSetCheck(BUTTEN_STATE,m_fBegin_State);	
	if(m_fBegin_State && m_fBegin_Attack)
	{
		OnOffAutoAttack(true);
		OnBegin_AttRange_Setup();
	}
	else
	{
		if(m_fBegin_State && !m_fBegin_Attack)
			OnOffAutoAttack(false,1);
		else
			OnOffAutoAttack(false,2);
	}
	return true;
}
bool CGuiAutoAttack::OnBegin_Attack()
{
	m_fBegin_Attack = !m_fBegin_Attack;
	SendMailDlg_ButtonSetCheck(BUTTEN_ATTACK,m_fBegin_Attack);	

	if(m_fBegin_State && m_fBegin_Attack)
	{
		OnOffAutoAttack(true);
		OnBegin_AttRange_Setup();
	}
	else
	{
		if(m_fBegin_State && !m_fBegin_Attack)
			OnOffAutoAttack(false,3);
		else
			OnOffAutoAttack(false,4);
	}

	return true;
}
bool CGuiAutoAttack::OnBegin_AttOrigin()
{
	m_fBegin_Origin = !m_fBegin_Origin;
	SendMailDlg_ButtonSetCheck(BUTTEN_ORIGIN,m_fBegin_Origin);	

	return true;
}
bool CGuiAutoAttack::OnBegin_AttRange()
{
	m_fBegin_Range = !m_fBegin_Range;
	SendMailDlg_ButtonSetCheck(BUTTEN_RANGE,m_fBegin_Range);	
	
	OnBegin_AttRange_Setup();
	return true;
}
		
bool CGuiAutoAttack::OnAutoUseMagic()
{
	m_fAuto_UseMagic = !m_fAuto_UseMagic;
	SendMailDlg_ButtonSetCheck(BUTTEN_AUTOMAGIC,m_fAuto_UseMagic);	

	//stGameConfig config = *((CGameAppation*)Engine_GetAppation())->GetConfig();
	//SetConfig_Skill(config);

	return true;
}
bool CGuiAutoAttack::OnAutoInTeam()
{
	m_fAuto_InTeam = !m_fAuto_InTeam;
	SendMailDlg_ButtonSetCheck(BUTTEN_AUTOTEAM,m_fAuto_InTeam);	

	return true;
}
bool CGuiAutoAttack::OnAutoPicUp()
{
	m_fAuto_PicUp = !m_fAuto_PicUp;
	SendMailDlg_ButtonSetCheck(BUTTEN_AUTOPICKUP,m_fAuto_PicUp);	

	return true;
}
bool CGuiAutoAttack::OnAutoPicUp_Setup()
{
	gAutoGameConfig.setCullObject.clear();

	CGuiMLEditBox* pMLEditBox = GetMLEditBox(43);
	if( pMLEditBox == NULL || pMLEditBox->GetText() == NULL )
		return true;

	const char* pszText = pMLEditBox->GetText();
	if( *pszText == '\0' )
		return true;

	char szKeywords[1024] = {0};
	strncpy(szKeywords, pszText, sizeof(szKeywords) - 1);

	for( int i = 0; szKeywords[i] != '\0'; ++i )
	{
		if( szKeywords[i] == '\r' || szKeywords[i] == '\n' )
			szKeywords[i] = ';';
	}

	char* tempStr = _strdup(szKeywords);
	if( tempStr == NULL )
		return true;

	char* token = strtok(tempStr, ";");
	while( token != NULL )
	{
		while( *token == ' ' || *token == '\t' )
			++token;

		char* tokenEnd = token + strlen(token);
		while( tokenEnd > token && (tokenEnd[-1] == ' ' || tokenEnd[-1] == '\t') )
		{
			--tokenEnd;
			*tokenEnd = '\0';
		}

		if( *token != '\0' )
		{
			for( size_t i = 0; i < g_tableObjectBase.size(); ++i )
			{
				if( g_tableObjectBase[i].strName && *g_tableObjectBase[i].strName )
				{
					if( strstr(g_tableObjectBase[i].strName, token) )
						gAutoGameConfig.setCullObject.insert(g_tableObjectBase[i].dwID);
				}
			}
		}

		token = strtok(NULL, ";");
	}

	free(tempStr);
	return true;
}

bool CGuiAutoAttack::OnfProtection_HP()
{
	m_fProtection_HP = !m_fProtection_HP;
	SendMailDlg_ButtonSetCheck(BUTTEN_PROTECTION_HP,m_fProtection_HP);	

	m_pEditBox_NeedToProtect_HP  = GetEditBox(EDITBO_PUT_HP);
	m_iNeedToProtect_HP  = strtoul(m_pEditBox_NeedToProtect_HP->GetText(), NULL,10);
	m_iComboBox_HP = GetComboBox(COMBOBOX_PUT_HP)->GetCurItem();

	return true;
}

bool CGuiAutoAttack::OnfProtection_MP()
{
	m_fProtection_MP = !m_fProtection_MP;
	SendMailDlg_ButtonSetCheck(BUTTEN_PROTECTION_MP,m_fProtection_MP);	

	m_pEditBox_NeedToProtect_MP  = GetEditBox(EDITBO_PUT_MP);
	m_iNeedToProtect_MP  = strtoul(m_pEditBox_NeedToProtect_MP->GetText(), NULL,10);
	m_iComboBox_MP = GetComboBox(COMBOBOX_PUT_MP)->GetCurItem();

	return true;
}

bool CGuiAutoAttack::OnfProtection_HP_20have()
{
	m_fProtection_HP_20have = !m_fProtection_HP_20have;
	SendMailDlg_ButtonSetCheck(BUTTEN_PROTECTION_HP_20HAVE,m_fProtection_HP_20have);	

	return true;
}

bool CGuiAutoAttack::OnfProtection_MP_20have()
{
	m_fProtection_MP_20have = !m_fProtection_MP_20have;
	SendMailDlg_ButtonSetCheck(BUTTEN_PROTECTION_MP_20HAVE,m_fProtection_MP_20have);	

	return true;
}
//////////////////star100604
bool CGuiAutoAttack::OnBegin_DS_Aid()
{
	m_fBegin_DS_Aid = !m_fBegin_DS_Aid;
	SendMailDlg_ButtonSetCheck(BUTTEN_DS_AID,m_fBegin_DS_Aid);	

	return true;
}
bool CGuiAutoAttack::OnAuto_DS_Resume()
{
	m_fAuto_DS_Resume = !m_fAuto_DS_Resume;
	SendMailDlg_ButtonSetCheck(BUTTEN_DS_RESUME,m_fAuto_DS_Resume);	

	return true;
}
bool CGuiAutoAttack::OnAuto_DS_TeamerResurrection()
{	
	m_fAuto_DS_TeamerResurrection = !m_fAuto_DS_TeamerResurrection;
	SendMailDlg_ButtonSetCheck(BUTTEN_DS_TEAMERRESURRECTION,m_fAuto_DS_TeamerResurrection);	

	return true;
}
bool CGuiAutoAttack::OnAuto_DS_Stealth()
{	
	m_fAuto_DS_Stealth = !m_fAuto_DS_Stealth;
	SendMailDlg_ButtonSetCheck(BUTTEN_DS_SLEALTH,m_fAuto_DS_Stealth);	

	return true;
}

//////////////////end
bool CGuiAutoAttack::OnAddListSkill()
{
	int i_tempSkill = -1;
	CGuiComboBox* pCtrl_temp_Skill = GetComboBox(COMBOBOX_PUT_SKILL);
	if(pCtrl_temp_Skill == NULL) return false;
	i_tempSkill = pCtrl_temp_Skill->GetCurItem();
	if(i_tempSkill == -1) return false;
	sAutoSkill  *tempSkill = (sAutoSkill*)pCtrl_temp_Skill->GetItemData(i_tempSkill);
	if(tempSkill == NULL) return false;
	
	if( !tempSkill->bUse )
	{
		tempSkill->bUse = true;
		m_pSkillList->AddItem( tempSkill->cName, tempSkill);
	}

	return true;
}
bool CGuiAutoAttack::OnDelListSkill()
{

	int curIndex = -1;
	curIndex = m_pSkillList->GetCurItem();
	if(m_pSkillList == NULL || curIndex == -1) 
		return false;
	sAutoSkill  *tempSkill = (sAutoSkill*)m_pSkillList->GetItemData( curIndex );
	if(tempSkill == NULL)
		return false;
	m_pSkillList->RemoveItem(curIndex);
	tempSkill->bUse = false;

	return true;
}

bool CGuiAutoAttack::OnIsGoaway()
{
	if( !GetGameApplication() || !GetGameApplication()->bClientSetted( enumAuto_Kill_Summon ) )
		return false;

	CGameGuiManager* pGuiManager = GetGameGuiManager();
	if( pGuiManager == NULL || pGuiManager->m_guiAutoAttackDlg == NULL )
		return false;

	if( !pGuiManager->m_guiAutoAttackDlg->m_fBegin_Range )
		return false;

	CGameScene* pScene = GetScene();
	CMainCharacter* pMainCharacter = pScene ? pScene->GetMainCharacter() : NULL;
	if( pMainCharacter == NULL )
		return false;

	POINT pos = pMainCharacter->GetGridPos();//GetScene()->PixelPosToGridPos(m_pCharacter->GetPos());
	POINT posDst;
	posDst.x = pGuiManager->m_guiAutoAttackDlg->m_iRange_PosX;//m_iRange_PosX
	posDst.y = pGuiManager->m_guiAutoAttackDlg->m_iRange_PosY;
	int dis = Scene_GetDis(pos,posDst);
	if( dis > pGuiManager->m_guiAutoAttackDlg->m_iRange_Range)//m_iRange_Range
	{
		pMainCharacter->Goto( posDst );
		return true;
	}

	return false;
}
bool CGuiAutoAttack::OnBegin_AttRange_Setup()
{
	if(m_fBegin_Range)
	{
		m_pEditBox_PutX  = GetEditBox(EDITBO_X);
		m_pEditBox_PutY  = GetEditBox(EDITBO_Y);
		m_pEditBox_Range = GetEditBox(EDITBO_RANGE);

		m_iRange_PosX  = strtoul(m_pEditBox_PutX->GetText(), NULL,10);
		m_iRange_PosY  = strtoul(m_pEditBox_PutY->GetText(), NULL,10);
		m_iRange_Range = strtoul(m_pEditBox_Range->GetText(),NULL,10);
		return true;
	}
	return false;
}

void CGuiAutoAttack::OnSetCloseLocation()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnSetCloseLocation();

	if(m_pBtnClose)
	{
		stPointI pt(m_pBtnClose->GetLocation());

		m_pBtnClose->SetLocation(pt.x-9 ,3);
	}

	FUNCTION_END;

}
void CGuiAutoAttack::SetConfig(stGameConfig config,int index )
{
	if(index == -1 || index == 0)
	{
		CGuiComboBox* pCtrl_HP = GetComboBox(COMBOBOX_PUT_HP);
		pCtrl_HP->RemoveAllItems();
		CGuiComboBox* pCtrl_MP = GetComboBox(COMBOBOX_PUT_MP);
		pCtrl_MP->RemoveAllItems();

		char szStr[32] = {0};
		_snprintf(szStr, sizeof(szStr),"补血丹");
		pCtrl_HP->AddItem(szStr,NULL);
		szStr[0]='\0';
		_snprintf(szStr, sizeof(szStr),"还神丹");
		pCtrl_HP->AddItem(szStr,NULL);
		szStr[0]='\0';
		_snprintf(szStr, sizeof(szStr),"仙宿丹");
		pCtrl_HP->AddItem(szStr,NULL);
		szStr[0]='\0';
		_snprintf(szStr, sizeof(szStr),"九华丹");
		pCtrl_HP->AddItem(szStr,NULL);
		szStr[0]='\0';
		_snprintf(szStr, sizeof(szStr),"还魂丹");
		pCtrl_HP->AddItem(szStr,NULL);
		szStr[0]='\0';
		_snprintf(szStr, sizeof(szStr),"大还丹");
		pCtrl_HP->AddItem(szStr,NULL);
		szStr[0]='\0';
		_snprintf(szStr, sizeof(szStr),"小还丹");
		pCtrl_HP->AddItem(szStr,NULL);
		szStr[0]='\0';
		_snprintf(szStr, sizeof(szStr),"清心露");
		pCtrl_MP->AddItem(szStr,NULL);
		szStr[0]='\0';
		_snprintf(szStr, sizeof(szStr),"丹凤露");
		pCtrl_MP->AddItem(szStr,NULL);
		szStr[0]='\0';
		_snprintf(szStr, sizeof(szStr),"百花露");
		pCtrl_MP->AddItem(szStr,NULL);
		szStr[0]='\0';
		_snprintf(szStr, sizeof(szStr),"天星露");
		pCtrl_MP->AddItem(szStr,NULL);
		szStr[0]='\0';
		_snprintf(szStr, sizeof(szStr),"灵玉露");
		pCtrl_MP->AddItem(szStr,NULL);
		szStr[0]='\0';
		_snprintf(szStr, sizeof(szStr),"定心露");
		pCtrl_MP->AddItem(szStr,NULL);
		szStr[0]='\0';

	}
}
void CGuiAutoAttack::SetConfig_Skill(stGameConfig config,int index )
{
	if(index == -1 || index == 0)
	{
		CGuiComboBox* pCtrl_Skill = GetComboBox(COMBOBOX_PUT_SKILL);
		pCtrl_Skill->RemoveAllItems();

		CMainCharacter* pMain = GetScene()->GetMainCharacter();
		if(pMain)
		{
			for(CMainCharacter::tListSkill::iterator it = pMain->m_listSkill.begin(); it != pMain->m_listSkill.end(); ++it)
			{
				SkillBase_t * p = (*it)->GetSkillBase();
				if( (p->dwType == SkillType_StateAttack || p->dwType == SkillType_State) && (p->dwTarget & SkillTarget_Self) )
				{////////////////star100707
					bool ifinto = false;
					for(std::vector<sAutoSkill>::iterator iPter = m_PassiveSkill.begin(); iPter != m_PassiveSkill.end(); ++iPter)
					{
						if( p->dwID == iPter->dID) ifinto = true;
					}
					if( !ifinto )
					{
						sAutoSkill  tempSkill;
						tempSkill.bUse = false;
						tempSkill.dID  = p->dwID;
						_snprintf( tempSkill.cName, sizeof(tempSkill.cName), p->pstrName );

						m_PassiveSkill.push_back(tempSkill);
					}
				}
			}
			for(std::vector<sAutoSkill>::iterator iPter = m_PassiveSkill.begin(); iPter != m_PassiveSkill.end(); ++iPter)
			{
				pCtrl_Skill->AddItem( iPter->cName, &(*iPter) );
			}
				
		}

	
	}
}

void CGuiAutoAttack::OnProtection_Life()
{
	//if(!GetGameApplication()->bClientSetted( enumAuto_Kill_Summon ))
	//	return;
	if(!m_fBegin_State) 
		return;

	CGameScene* pScene = GetScene();
	CMainCharacter* pMainCharacter = pScene ? pScene->GetMainCharacter() : NULL;
	if( pMainCharacter == NULL )
		return;

	CGuiEditBox* pEditBox_HP = GetEditBox(EDITBO_PUT_HP);
	CGuiComboBox* pComboBox_HP = GetComboBox(COMBOBOX_PUT_HP);
	if( pEditBox_HP && pEditBox_HP->GetText() )
		m_iNeedToProtect_HP = strtoul(pEditBox_HP->GetText(), NULL,10);
	if( pComboBox_HP )
		m_iComboBox_HP = pComboBox_HP->GetCurItem();

	CGuiEditBox* pEditBox_MP = GetEditBox(EDITBO_PUT_MP);
	CGuiComboBox* pComboBox_MP = GetComboBox(COMBOBOX_PUT_MP);
	if( pEditBox_MP && pEditBox_MP->GetText() )
		m_iNeedToProtect_MP = strtoul(pEditBox_MP->GetText(), NULL,10);
	if( pComboBox_MP )
		m_iComboBox_MP = pComboBox_MP->GetCurItem();

	if(m_fProtection_HP && (m_iNeedToProtect_HP != 0) )
	{
		if( m_iComboBox_HP >= 0 && m_iComboBox_HP < (int)(sizeof(m_dwNeedMedicine_HP) / sizeof(m_dwNeedMedicine_HP[0])) && m_dwNeedMedicine_HP[m_iComboBox_HP] != 0 )
		{
			if( (pMainCharacter->GetMaxHP() * m_iNeedToProtect_HP / 100) > pMainCharacter->GetHP())
			{
				pMainCharacter->OnUseItem_forAutoAttack(m_dwNeedMedicine_HP[m_iComboBox_HP/* -1*/]);
			}
		}
	}
	if(m_fProtection_MP && (m_iNeedToProtect_MP != 0) )
	{
		if( m_iComboBox_MP >= 0 && m_iComboBox_MP < (int)(sizeof(m_dwNeedMedicine_MP) / sizeof(m_dwNeedMedicine_MP[0])) && m_dwNeedMedicine_MP[m_iComboBox_MP] != 0 )
		{
			if( (pMainCharacter->GetMaxMP() * m_iNeedToProtect_MP / 100) > pMainCharacter->GetMP())
			{
				pMainCharacter->OnUseItem_forAutoAttack(m_dwNeedMedicine_MP[m_iComboBox_MP/* -1*/]);
			}
		}
	}
}

bool CGuiAutoAttack::OnSaveButtenCleck()
{
	if(m_fProtection_HP)
	{
		m_pEditBox_NeedToProtect_HP  = GetEditBox(EDITBO_PUT_HP);
		m_iNeedToProtect_HP  = strtoul(m_pEditBox_NeedToProtect_HP->GetText(), NULL,10);
		m_iComboBox_HP = GetComboBox(COMBOBOX_PUT_HP)->GetCurItem();
	}
	if(m_fProtection_MP)
	{
		m_pEditBox_NeedToProtect_MP  = GetEditBox(EDITBO_PUT_MP);
		m_iNeedToProtect_MP  = strtoul(m_pEditBox_NeedToProtect_MP->GetText(), NULL,10);
		m_iComboBox_MP = GetComboBox(COMBOBOX_PUT_MP)->GetCurItem();
	}
	if(m_fBegin_Range)
	{
		m_pEditBox_PutX  = GetEditBox(EDITBO_X);
		m_pEditBox_PutY  = GetEditBox(EDITBO_Y);
		m_pEditBox_Range = GetEditBox(EDITBO_RANGE);

		m_iRange_PosX  = strtoul(m_pEditBox_PutX->GetText(), NULL,10);
		m_iRange_PosY  = strtoul(m_pEditBox_PutY->GetText(), NULL,10);
		m_iRange_Range = strtoul(m_pEditBox_Range->GetText(),NULL,10);
	}
	
	if(m_fAuto_PicUp)
		OnAutoPicUp_Setup();

	//by=>friday
	
	// 提示保存成功
	GetGameGuiManager()->AddMessageBox("设置保存成功");
	//by=>friday
	
	return true;
}

bool CGuiAutoAttack::OnAuto_Sitdown()
{
	if( m_iSitdown_PosX == 0 && m_iSitdown_PosY == 0 )
		return false;
	if( !GetScene()->GetMainCharacter()->IsSitDown() )
	{
		//GetScene()->GetMainCharacter()->OnSit(true);
				OnSitDown();

				g_bSitDownHotKeyActive = false;

		return true;
	}
	return false;
}
bool CGuiAutoAttack::OnAuto_GotoSitdown()
{
	if(m_fBegin_State && m_fBegin_DS_Aid)
	{
		if( m_fAuto_DS_Resume )
		{
			int dis = 0;
			POINT pos = GetScene()->GetMainCharacter()->GetGridPos();
            POINT posDst;
			posDst.x = m_iSitdown_PosX;
			posDst.y = m_iSitdown_PosY;
			dis = Scene_GetDis(pos,posDst);
			if( dis > 2 )
			{
				GetScene()->GetMainCharacter()->Goto( posDst );
				return false;
			}
			else
			{
				return OnAuto_Sitdown();
				
			}
		}
	}
	return false;
}

bool CGuiAutoAttack::OnAuto_SetPosSit()
{
	//m_pEditBox_PutX  = GetEditBox(EDITBO_X);
	//m_pEditBox_PutY  = GetEditBox(EDITBO_Y);

	POINT pos = GetScene()->GetMainCharacter()->GetGridPos();
	m_iSitdown_PosX  = pos.x;
	m_iSitdown_PosY  = pos.y;
//SetWindowText
	char t1[20], t2[20];
	_snprintf(t1, sizeof(t1), "%d",m_iSitdown_PosX);
	_snprintf(t2, sizeof(t2), "%d",m_iSitdown_PosY);

	CGuiEditBox* m_pEditBox_Sitdown_PutX  = GetEditBox(EDITBO_X_SIT);	
	CGuiEditBox* m_pEditBox_Sitdown_PutY  = GetEditBox(EDITBO_Y_SIT);
	m_pEditBox_Sitdown_PutX->SetText( t1 );
	m_pEditBox_Sitdown_PutY->SetText( t2 );
	//m_pEditBox_Sitdown_PutX->SetWindowText( t1 );
	//m_pEditBox_Sitdown_PutY->SetWindowText( t2 );

	return true;

}

// 新增装备颜色过滤函数
bool CGuiAutoAttack::OnFilterWhiteEquip()
{
	m_fFilter_WhiteEquip = !m_fFilter_WhiteEquip;
	SendMailDlg_ButtonSetCheck(90, m_fFilter_WhiteEquip);
	//by=>friday
	
	return true;
}

bool CGuiAutoAttack::OnFilterBlueEquip()
{
	m_fFilter_BlueEquip = !m_fFilter_BlueEquip;
	SendMailDlg_ButtonSetCheck(91, m_fFilter_BlueEquip);
	//by=>friday
	
	return true;
}

bool CGuiAutoAttack::OnFilterGoldEquip()
{
	m_fFilter_GoldEquip = !m_fFilter_GoldEquip;
	SendMailDlg_ButtonSetCheck(92, m_fFilter_GoldEquip);
	//by=>friday
	
	return true;
}

bool CGuiAutoAttack::OnFilterGreenEquip()
{
	m_fFilter_GreenEquip = !m_fFilter_GreenEquip;
	SendMailDlg_ButtonSetCheck(93, m_fFilter_GreenEquip);
	//by=>friday
	
	return true;
}
//by=>friday

// 添加OnOffAutoAttack函数实现
void CGuiAutoAttack::OnOffAutoAttack(bool flag, int index)
{
	if(index == -1)
	{
		m_fBegin_State = flag;
		m_fBegin_Attack = flag;
	}
	else if(index == 0)
	{
		m_fBegin_State = flag;
	}
	else if(index == 1)
	{
		m_fBegin_Attack = flag;
	}

	SendMailDlg_ButtonSetCheck(BUTTEN_STATE, m_fBegin_State);
	SendMailDlg_ButtonSetCheck(BUTTEN_ATTACK, m_fBegin_Attack);

	bool bAutoAttack = (m_fBegin_State && m_fBegin_Attack);
	if(GetGameApplication())
		GetGameApplication()->SetClientSet(enumAuto_Kill_Summon, bAutoAttack);
	g_bAutoFight = bAutoAttack;
	if(!bAutoAttack)
	{
		CGameScene* pScene = GetScene();
		CMainCharacter* pMainCharacter = pScene ? pScene->GetMainCharacter() : NULL;
		if( pMainCharacter )
		{
			pMainCharacter->ClearPath();
			pMainCharacter->ClearRunEvent();
			pMainCharacter->m_AStar.FreePath();
			pMainCharacter->m_AStarZone.FreePath();
		}
	}
	if(GetGameGuiManager() && GetGameGuiManager()->m_guiSystem)
		GetGameGuiManager()->m_guiSystem->UpdateAutoPkSwitch();

	//by=>friday
}



