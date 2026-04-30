#include "public.h"
#include "../RenderD3D/include/D3DDevice.h"
#include "../RenderD3D/include/RenderD3D.h"
#include ".\npc.h"
#include "./Game.h"
#include "GameScene.h"
#include "MainCharacter.h"
#include "Magic.h"
#include "./RoleItem.h"
#include "./GameCursor.h"
#include "../engine/include/BmpTextout.h"
#include "./GameGuiManager.h"
#include "./GuiSkill.h"
#include "./RoleSkill.h"
#include "./GuiMain.h"
#include "./GuiItem.h"
#include "./GuiNpcTrade.h"
#include "./GuiSalaryTrade.h"  //工资
#include "./GuiTicketTrade.h"  //积分
//#include "./GuiChongzhiTrade.h" //充值点
#include "./GuiReliveDlg.h"
#include "./CharacterProperty.h"
#include "./GameState.h"
#include "./GuiShopDialog.h"
#include "./Country.h"
#include "./GuiTeam.h"
#include "./GameAppation.h"
#include "Effect.h"

/************************************ 摊位名字显示 begin **************************************/
CNpc::CShopNameTip::CShopNameTip()
{
	stResourceLocation rl( GetGuiGraphicPackName(),47,7 );	
	m_pBmpShopName = GetDevice()->FindBitmaps( &rl );
	m_dwColor = -1;
	m_strText = "";
	m_iBaseWidth = 0;
	m_iWidth = 0;
	m_rcBmpClip = stRectI(0,0,0,0);
	m_rcShopNameRect = stRectI(0,0,0,0);
	if ( NULL != m_pBmpShopName )
	{
		IBitmap* pBitmap = m_pBmpShopName->GetBitmap(0);
		if ( pBitmap )
		{
			m_iBaseWidth = pBitmap->GetClipWidth();
			m_rcBmpClip = pBitmap->GetClipRect();
			CuttRect3(m_rcBmpClip,15,m_rcBmps);			
		}		
	}	
}

DWORD scShopNameTipFormat[3]={DT_LEFT,DT_CENTER,DT_RIGHT};

void CNpc::CShopNameTip::Render(POINT pt)
{
	if( m_strText.length() < 1 ) return ;
	if( NULL == m_pBmpShopName ) return ;
	stPointI pt0( pt.x - m_iWidth/2,pt.y - m_rcBmpDst.Height() );// = pt - stPointI(m_iWidth/2,0);
	for(size_t i = 0;i < 3; ++i)
	{
		stRectI rc = m_rc[i];
		rc.OffsetRect(pt0);
		PatchRenderImage(rc,m_pBmpShopName,&m_rcBmps[i],-1,Blend_Null,scShopNameTipFormat[i]);
	}
	stRectI rcTemp = m_rcBmpDst;
	rcTemp.OffsetRect( pt0 );
	m_rcShopNameRect = rcTemp;
	rcTemp.OffsetRect(0,3);
	GetDevice()->DrawString( m_strText.c_str(),rcTemp,m_dwColor,DT_CENTER,FontEffect_Border,Blend_Null);
}	

void CNpc::CShopNameTip::SetShopName(const char* cText,DWORD dwColor)
{
	if( NULL == cText || strlen(cText) < 1) return ;
	m_strText = cText;
	m_dwColor = dwColor;

	stRectI rc(0,0,400,100);
	GetDevice()->GetStringRect(m_strText.c_str(),m_strText.length(),rc,DT_CENTER);
	m_iWidth = rc.Width() + 80;

	m_rcBmpDst = stRectI(0,0,m_iWidth,m_rcBmpClip.Height());
	CuttRect3(m_rcBmpDst,15,m_rc);	
}

inline RECT CNpc::CShopNameTip::GetClipRect() const
{	
	return m_rcShopNameRect;
}

/************************************ 摊位名字显示 end **************************************/

/***************CNpc::CChatTip******************/
const DWORD CNpc::CChatTip::m_scTotalShowTime = 8500;
///横边切割宽度
const int CNpc::CChatTip::m_scWidthIncise = 10;
///纵边切割宽度
const int CNpc::CChatTip::m_scHeightIncise = 10;

/***************CNpc::CChatTip******************/
/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CNpc::CChatTip::CChatTip()
{
	FUNCTION_BEGIN;

	m_dwTotalShowTime = m_scTotalShowTime;
	stResourceLocation rl;
	rl.SetFileName( GetGuiGraphicPackName() );
	rl.group = 5;
	
	//soke 修改人物、NPC说话
	//rl.frame = 28;
	rl.SetFileName( "Data\\Interfaces1.pak" );
	rl.frame = 1;

	m_pBmpRect = GetDevice()->FindBitmaps( &rl );
	AssertFatal(m_pBmpRect,avar("文件%s [%d][%d] error or not exist",rl.szFileName,rl.group,rl.frame));
	//Assert(m_pBmpRect);

	// 2010
//	rl.frame = 29;
	rl.frame = 2;

	m_pBmpAngle = GetDevice()->FindBitmaps( &rl );
	AssertFatal(m_pBmpAngle,avar("文件%s [%d][%d] error or not exist",rl.szFileName,rl.group,rl.frame));
	//Assert(m_pBmpAngle);

	stPointI s_ptBorderIncise(m_scWidthIncise,m_scHeightIncise);
	stRectI rcTexture;
	stRectI rects[9];
	rcTexture.SetRect(0,0,114,32);
	CuttRect9(rcTexture,s_ptBorderIncise,s_ptBorderIncise,rects);
	for(size_t i=0;i < 9;++i)
	{
		m_bmpRect[i] = rects[i];
	}

	int step = (114 - s_ptBorderIncise.x -  s_ptBorderIncise.x - 14) / 2;

	stRectI rc[3];
	CuttRect3(m_bmpRect[7],step,rc);
	m_bmpRect[7] = rc[0];
	m_bmpRect[9] = rc[1];
	m_bmpRect[10] = rc[2];

	m_width = 180;
	m_height = 100;
	m_startTime = 0;
	m_cellHeight = 20;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CNpc::CChatTip::Render(POINT pt)
{
	FUNCTION_BEGIN;

	if(m_strTextVec.empty()) return ;

	DWORD curTime = xtimeGetTime();	
	DWORD subTime = curTime - m_startTime;
	if( subTime > m_dwTotalShowTime )
	{
		if(!m_strTextVec.empty()) m_strTextVec.clear();
		return ;
	}

	DWORD dwStopTime = m_dwTotalShowTime * 9/10;
	int alpha = 255;
	if(subTime > dwStopTime) alpha = (m_dwTotalShowTime - subTime )*255 /( m_dwTotalShowTime - dwStopTime);

	DWORD dwBackColor =  COLOR_ARGB(alpha,255,255,255);
	DWORD dwTextColor =  m_dwColor ;

	dwTextColor &= 0x00ffffff;
	dwTextColor |= alpha << 24;

	if( NULL != m_pBmpRect )
	{
		for(size_t i = 0;i < 11; ++i)
		{
			DWORD dwFormat = (i < 9)? s_PatchAlignFormat[i]:(DT_LEFT | DT_BOTTOM);
			stRectI rc = m_renderRect[i];
			rc.OffsetRect(pt.x - m_width / 2,pt.y - m_height - 14);
			if( i != 7)	PatchRenderImage(rc,m_pBmpRect,&m_bmpRect[i],dwBackColor,Blend_Null,dwFormat);
		}
	}
	if( NULL != m_pBmpAngle )
	{
		m_pBmpAngle->Render(0,pt.x - 7 ,pt.y - 24,NULL,NULL,dwBackColor,Blend_Null);
	}

	int iFont = GetDevice()->SetFont(1);

	stRectI rc = m_renderRect[1];
	rc.OffsetRect(pt.x - m_width / 2,pt.y - m_height - 14);

	for(size_t j = 0;j < m_strTextVec.size();++j)
	{			
		stRectI cel( pt.x - m_width / 2 - 5,
			rc.top + 10 + j * m_cellHeight, 
			pt.x + m_width / 2 + 5,
			rc.top + 10 + ( j  + 1 ) * m_cellHeight );

		GetDevice()->DrawString(m_strTextVec[j].c_str(),cel,dwTextColor,DT_CENTER,
			FontEffect_Border,ColorBlend(dwBackColor,c_dwClrDefaultShadow));	
	}
	GetDevice()->SetFont(iFont);

	FUNCTION_END;
}
/**
* \brief 添加文字
* 
* 添加文字
* 
* \return 
*/
void CNpc::CChatTip::AddText(const char *cText,DWORD dwColor,DWORD dwShowTime ) 
{
	FUNCTION_BEGIN;

	m_dwTotalShowTime = dwShowTime;
	stPointI s_ptBorderIncise(m_scWidthIncise,m_scHeightIncise);

	m_dwColor  = dwColor;
	m_startTime = xtimeGetTime();
	if(!m_strTextVec.empty()) m_strTextVec.clear();
	if(!cText || strlen(cText) < 1) 
	{
		return ;
	}		
	str_split<strVec>(cText,m_strTextVec,20);
	if(!m_strTextVec.empty()) 
	{
		stRectI rc(0,0,200,100);
		GetDevice()->GetStringRect(m_strTextVec[0].c_str(),m_strTextVec[0].length(),rc,DT_CENTER);
		//TRACE("AddText:%d,%d,%d,%d\n",rc.left,rc.top,rc.right,rc.bottom);
		if(m_strTextVec.size() > 1) 
			m_width = 200;
		else 
		{
			m_width = rc.Width() + 50;
			m_width = (m_width > 70) ? m_width : 70;
		}
		m_cellHeight = rc.Height() + 5;
		m_height = m_strTextVec.size() * m_cellHeight + 20;
	}

	stRectI renderBack(0,0,m_width,m_height);
	CuttRect9(renderBack,s_ptBorderIncise,s_ptBorderIncise,m_renderRect);
	int step = (m_width - s_ptBorderIncise.x -  s_ptBorderIncise.x - 14) / 2 + 2;

	stRectI rc[3];
	CuttRect3(m_renderRect[7],step,rc);
	m_renderRect[7] = rc[0];
	m_renderRect[9] = rc[1];
	m_renderRect[10] = rc[2];

	FUNCTION_END;
}



/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CNpc::CNpc(void)
: m_pCharacter(NULL)
, m_pChangeFace(NULL)
, m_bDieWaitingMagic(false)
, m_bFillObstacle(false)
, m_nRecieveAttack(0)
, m_timeCommand(0)
, m_nDieWaitingTime(0)
, m_nDirect(_DIR_DOWN)
, m_nSendAttack(0)
, m_nRecieveWalk(0)
, m_nSendWalk(0)
, m_bDied(false)
, m_pNpcBase(NULL)
, m_bDieEnd(false)
, m_bIsSitting(false)
, m_bClientRideState(false)
, m_bClientSitting(false)
, m_bAshesFadeOut(false)
, m_bIsCombination(false)
, m_dwCombinationID(-1)
, m_pPetData(NULL)
//, m_dwUseSkillTime(0)
//, m_dwUseSkillIntervalTime(0)
{
	FUNCTION_BEGIN;

	m_dwSceneObjectType |= OBJECT_NPC;
	m_pCharacter = Character_CreateCharacter();
	m_pCharacter->SetData(this);
	m_data.dwNpcDataID = -1;
	m_data.dwMapNpcDataPosition = -1;
	ResetImage();
	m_queueCmd.Init(1024);

	m_aSkillUseTime.clear();

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CNpc::~CNpc(void)
{
	FUNCTION_BEGIN;

	Release();

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CNpc::Release()
{
	FUNCTION_BEGIN;

	while(!m_queueHPCmd.empty())
	{
		delete m_queueHPCmd.front();
		m_queueHPCmd.pop_front();
	}

	//If this our interest npc
	if ( GetScene()->m_pInterestNpc && GetScene()->m_pInterestNpc->IsNpc() )
	{
		if ( GetScene()->m_pInterestNpc == this )
		{
			GetScene()->UnSelectInterestNpc();
			if ( GetGameGuiManager()->m_guiCharacterProperty )
				GetGameGuiManager()->m_guiCharacterProperty->SetVisible( false );
		}
	}


	if ((GetScene()->GetMainCharacter()->m_pAttackNpc)&&(GetScene()->GetMainCharacter()->m_pAttackNpc==this))
	{
		GetScene()->GetMainCharacter()->m_pAttackNpc = NULL;
	}

	if( GetScene()->m_pMouseOverObject == this){
		GetScene()->m_pMouseOverObject = NULL;
	}

	RemoveDynamicObstacle();
	if(m_pCharacter)
	{
		//GetScene()->RemvoeMagicByCharacter(this);
		m_pCharacter->SetData(NULL);
		m_pCharacter->FinalRelease();
		m_pCharacter->Release();
		m_pCharacter = NULL;
	}
	memset(&m_data,0,sizeof(m_data));
	m_nSendAttack =  0;
	m_nRecieveAttack = 0;
	m_nRecieveWalk = 0;
	m_nSendWalk = 0;
	CSceneObject::Release();

	//宠物数据
	delete m_pPetData;
	m_pPetData = NULL;

	FUNCTION_END;
}

bool CNpc::IsCanChangeFaceTo()
{
	if ( NULL == m_pNpcBase )
		return false;
	
	//NpcTypeItem_None = -1,
	//NpcTypeItem_Human = 0,		//人形类
	//NpcTypeItem_Normal = 1,		//普通类型
	//NpcTypeItem_Boss = 2,		//大BOSS类型
	//NpcTypeItem_SmBoss = 3,		//小BOSS类型
	//NpcTypeItem_Extract	 = 4,	//精英类
	//NpcTypeItem_Gold	 = 5,	//黄金
	//NpcTypeItem_Trade = 6,		//买卖类的
	//NpcTypeItem_Task = 7,		//任务类的
	//NpcTypeItem_Enlisted = 8,	//守卫的士兵类
	//NpcTypeItem_Pet=9,			//宠物
	//NpcTypeItem_ExtractBugbear=10,	//精怪类
	//NpcTypeItem_Beckon = 11,		//召唤怪物
	//NpcTypeItem_Totem=12,			//图腾NPC
	//NpcTypeItem_Guide = 16,			//路标类型
	//NpcTypeItem_Box = 17,			//宝箱
	/*return ( (m_pNpcBase->dwType == NpcTypeItem_Human )
			|| ( m_pNpcBase->dwType == NpcTypeItem_Normal )	
			|| ( m_pNpcBase->dwType == NpcTypeItem_Boss )
			|| ( m_pNpcBase->dwType == NpcTypeItem_SmBoss )
			|| ( m_pNpcBase->dwType == NpcTypeItem_Extract )
			|| ( m_pNpcBase->dwType == NpcTypeItem_Pet )
			|| ( m_pNpcBase->dwType == NpcTypeItem_ExtractBugbear )
			|| ( m_pNpcBase->dwType == NpcTypeItem_Beckon ) );*/

	return m_pNpcBase->dwType == NpcTypeItem_Normal;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \return 返回值的描述
*/
bool CNpc::SetAttrib(t_MapNpcData* pCmd)
{
	FUNCTION_BEGIN;

	bool bUpdateImage = (m_data.dwNpcDataID != pCmd->dwNpcDataID) && !IsChangeFace() && !IsCombinationState();
	bool bUpdateState = (memcmp(m_data.byState,pCmd->byState,sizeof(m_data.byState)) != 0);


	m_data = *pCmd;
	m_dwID = pCmd->dwMapNpcDataPosition;
	m_pNpcBase = GetNpcBase(m_data.dwNpcDataID);
	//	TRACE("%s----SetAttrib---%d-----begin\n",pCmd->name,m_data.dwNpcDataID);
	if(!m_pNpcBase) return false;
	//	TRACE("----SetAttrib---%d-----end\n",m_data.dwNpcDataID);

	if( m_pNpcBase && !IsPet() )
	{
		

	SetName(m_pNpcBase->strName);
		
		
	}

	AfterSetAttrib(bUpdateImage,bUpdateState);

	return true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param bUpdateImage : 描述
* \param bUpdateState : 描述
* \return 返回值的描述
*/
void CNpc::AfterSetAttrib(bool bUpdateImage,bool bUpdateState)
{
	FUNCTION_BEGIN;

	if (bUpdateImage)
		ResetImage();	// 得到所有的人物部件位置...

	if(bUpdateState)
		ResetState();

	m_pCharacter->ChangeColor(Ani_Part_BodySystemColor,GetBodySystemColor());
	
	GetInterface()->SetMoveSpeed( (float)m_data.movespeed/640.0f );
    	
	FUNCTION_END;
}

void CNpc::RunRide()
{
	//ride horse state
	if( isset_state(GetState(),USTATE_RIDE) )
	{
		//TRACE( "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&骑马\n" );
		if(!IsRide() && m_pCharacter->CanChangeRideState())
			OnRide(true);
	}
	else
	{
		//TRACE( "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&下马\n" );
		if(IsRide() && m_pCharacter->CanChangeRideState())
			OnRide(false);
	}
}

void CNpc::RunSitDown()
{
	//Sit dowm state
	if ( isset_state(GetState(), USTATE_SITDOWN) )		//
	{
		if ( !IsSitDown() )
			OnSit( true );
	}
	else
	{
		if ( IsSitDown() )
			OnSit( false );
	}
}

DWORD CNpc::GetTeamAttackState()
{
	if ( !IsCharacter() )
		return -1;
	
	BYTE* byState = GetState();
	for(tTableCharStateBase::iterator it = g_tableCharStateBase.begin(); it != g_tableCharStateBase.end();
		++it)
	{
		if((*it).dwID >= MAX_STATE)
			break;

		if( isset_state(byState,(*it).dwID) && ::IsTeamAttackState((*it).dwID) )
			return (*it).dwID;
	}

	return -1;
}

//struct stSkillState{
//	int iState;		// 状态
//	int iSkillID;	// 技能ID
//};
//
//static stSkillState g_aSkillState[] = 
//{
//	{USTATE_PLANTING,	194},
//	{USTATE_DIGGING,	193},
//	{USTATE_PROSPECTING,192},
//	{USTATE_CUTTING,	191},
//	{USTATE_FISHING,	190},
//	{USTATE_HUNTING,	189},
//	{USTATE_GATHERING,	188},
//	//{USTATE_GUARD,		-1},
//};

#define CHANGE_COLOR(color) \
{\
	if ( IsCharacter() ){ \
		D3DXCOLOR outColor; \
		GetInterface()->ChangeColor( Ani_Part_Body, color ); \
		GetInterface()->ChangeColor( Ani_Part_BodySystemColor, (DWORD)*(D3DXColorModulate( &outColor, &(D3DXCOLOR)(color), &(D3DXCOLOR)GetBodySystemColor() )) ); \
		GetInterface()->ChangeColor( Ani_Part_BodyCustomColor, (DWORD)*(D3DXColorModulate( &outColor, &(D3DXCOLOR)(color), &(D3DXCOLOR)GetBodyCustomColor() )) ); \
		GetInterface()->ChangeColor( Ani_Part_Hair, (DWORD)*(D3DXColorModulate( &outColor, &(D3DXCOLOR)(color), &(D3DXCOLOR)GetHairColor() )) ); \
	} \
	else \
	{ \
		GetInterface()->ChangeColor( color ); \
	} \
}\
	
/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CNpc::ResetState()
{
	FUNCTION_BEGIN;


	BYTE* byState = GetState();
	//for(size_t i=0;i<count_of(g_aSkillState);++i)

	//This is just a test
	/*if ( IsMainRole() )
		set_state( byState, USTATE_HIDE );*/
	//End test
#ifdef _DEBUG
	if ( isset_state(byState,USTATE_BOMB) )
	{
		int ii = 10;
	}

	if	( isset_state(byState,USTATE_WAR) )
	{
		int iiiii = 0;
	}
	else
	{
		int iiii = 0;
	}

	if ( !isset_state(byState, 235) && IsMainRole() )
	{
		int iiii = 0;
	}
#endif

	for(tTableCharStateBase::iterator it = g_tableCharStateBase.begin(); it != g_tableCharStateBase.end();
		++it)
	{
		if((*it).dwID >= MAX_STATE)
			break;

		if(isset_state(byState,(*it).dwID))
		{
#ifdef _DEBUG
			if( (*it).dwID == USTATE_BOMB)
			{
				TRACE("诅咒炸弹状态设置开\n");
			}
#endif 

			//if ( (*it).dwID == USTATE_ULTRA_EQUIPMENT )
			//{
			//	int iIndex = 0;
			//}
			//GetInterface()->SetMagicRing((*it).dwPicID,60000 * 60);
			if( (*it).dwPicID1 != 0xffffffff)
			{
				GetInterface()->SetMagicRing((*it).dwPicID1,60000 * 60);
			}
			if( (*it).dwPicID2 != 0xffffffff)
			{
				GetInterface()->SetMagicRing((*it).dwPicID2,60000 * 60);
			}
			if( (*it).dwPicID3 != 0xffffffff)
			{
				GetInterface()->SetMagicRing((*it).dwPicID3,60000 * 60);
			}
			if( (*it).dwPicID4 != 0xffffffff)
			{
				GetInterface()->SetMagicRing((*it).dwPicID4,60000 * 60);
			}

			//We add state icon when receive data
			if ( IsMainRole() && !(isset_state(byState,USTATE_DEATH)) )
			{
				/*if ( GetGameGuiManager()->m_guiMain )
					GetGameGuiManager()->m_guiMain->AddOneState( true, false, (*it).dwID, 0xffff, 0 );*/
			}
		}
		else
		{
			//if ( (*it).dwID == 6 )
			//	TRACE("------------------------------------------------删除状态 %d\n",(*it).dwID);
			if ( (*it).dwID == USTATE_ULTRA_EQUIPMENT  && IsMainRole() )
				int temp = 10;

#ifdef _DEBUG
			if( (*it).dwID == USTATE_BOMB)
			{
				TRACE("诅咒炸弹状态设置关\n");
			}
#endif 

			//GetInterface()->RemoveAMagic((*it).dwPicID);
			if( (*it).dwPicID1 != 0xffffffff)
			{
				GetInterface()->RemoveAMagic((*it).dwPicID1);
			}
			if( (*it).dwPicID2 != 0xffffffff)
			{
				GetInterface()->RemoveAMagic((*it).dwPicID2);
			}
			if( (*it).dwPicID3 != 0xffffffff)
			{
				GetInterface()->RemoveAMagic((*it).dwPicID3);
			}
			if( (*it).dwPicID4 != 0xffffffff)
			{
				GetInterface()->RemoveAMagic((*it).dwPicID4);
			}
			//Delete this state icon
			//if ( IsMainRole() )		//When npc state clear we clear ref state buffer
			if ( IsMainRole() || (this == GetScene()->m_pInterestNpc) )
			{
				if ( GetGameGuiManager()->m_guiMain )
					GetGameGuiManager()->m_guiMain->DeleteOneState( IsMainRole(), false, (*it).dwID );
			}
		}
	}		

	if(isset_state(byState,USTATE_DEATH))
	{
		if ( IsMainRole() )
		{
			GetGameGuiManager()->EndNpcDialog();
			GetGameState()->SetStateItem(eGameStateItem_Die);

			if ( GetScene() )
				GetScene()->UnSelectInterestNpc();

			GetScene()->GetMainCharacter()->SetHPAndMP( 0, 0, 0 );
		}

		if(!m_bDied)
		{
			//If is changing face state
			//We first change back
			if ( IsChangeFace() )
				ChangeFace( -1 );
			/*if ( IsCombinationState() )
				CombinationWithPet( -1 );*/

			// Do anything die show do
			if ( !IsCharacter() )
			{
				GetInterface()->DieEnd();
				m_bDieEnd = true;
			}
			if ( !IsMainRole() )
				m_bDied = true;

			//delete all state in GuiMain dlg
			if ( GetGameGuiManager()->m_guiMain )
			{
				while ( GetGameGuiManager()->m_guiMain->DeleteAllState( IsMainRole() ) );
			}

			//Delete our interest npc
			if ( this == GetScene()->m_pInterestNpc )
				GetScene()->UnSelectInterestNpc();			
		}
	}
	else
	{
		if(m_bDied)
		{
			GetInterface()->Init(true);
			m_bDied = false;
			m_bDieEnd = false;
		}
	}

	//Combination with pet
	for( int k=0;k<COMBINATION_STATE_NUM;k++ )
	{
		if ( isset_state( byState, cmbState[k] ) )
		//if ( IsMainRole() )
		{
			if ( !IsCombinationState() )
				CombinationWithPet( cmbNpcID[k] );

			break;
		}
	}

	if ( k == COMBINATION_STATE_NUM )
	{
		if ( IsCombinationState() )
		{
			CombinationWithPet( -1 );

			if ( IsMainRole() && !IsChangeFace() )
			{
				GetScene()->GetMainCharacter()->SetMoveSpeed( 2 );
			}
		}		
	}

	if ( isset_state( byState, USTATE_COMMON_TEMP_USE ) )
	{
		//We send Christ change face cmd to server
		if ( IsMainRole() && !IsChangeFace() )
		{
			stChangeFaceMapScreenUserCmd cmd;
			cmd.dwUserTempID = GetScene()->GetMainCharacter()->GetID();
			if ( GetScene()->GetMainCharacter()->GetSex() == eSex_Male )
				cmd.dwChangeFaceID = 44031;
			else
				cmd.dwChangeFaceID = 44032;
			SEND_USER_CMD( cmd );
		}
	}
	
	
	//change face state
	if ( isset_state( byState, USTATE_CHANGEFACE ) )
	{
		//We just send change face cmd to server
		if ( IsMainRole() && !IsChangeFace() )
		{
			//Find one nearby NPC as my face
			DWORD id = GetScene()->FindOneCanMoveNpcRandom();
			if ( id != -1 )
			{
				stChangeFaceMapScreenUserCmd cmd;
				cmd.dwUserTempID = GetID();
				cmd.dwChangeFaceID = id;
				SEND_USER_CMD( cmd );
			}	
		}		
	}
	//change face state
	else if ( isset_state( byState, USTATE_ZHUTOU ) )
	{
		//We just send change face cmd to server
		if ( IsMainRole() && !IsChangeFace() )
		{
			//Find one nearby NPC as my fac
			
				stChangeFaceMapScreenUserCmd cmd;
				cmd.dwUserTempID = GetID();
				cmd.dwChangeFaceID = 10011;
				SEND_USER_CMD( cmd );
				
		}		
	}
	else
	{
		if ( IsChangeFace() )
		{
			if ( !isset_state( byState, USTATE_COMMON_TEMP_USE ) )
				ChangeFace( -1 );
		}

		if ( IsMainRole() && !IsCombinationState() )
		{
			GetScene()->GetMainCharacter()->SetMoveSpeed( 2 );
		}
	}


	
	if(!m_bDied)
	{
		//ride horse state
		if(isset_state(byState,USTATE_RIDE))
		{
			//TRACE( "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&骑马\n" );
			if(!IsRide())
				OnRide(true);
		}
		else
		{
			//TRACE( "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&下马\n" );
			if(IsRide())
				OnRide(false);
		}
	}

	//Sit dowm state
	if ( isset_state(byState, USTATE_SITDOWN) )		//
	{
		if ( !IsSitDown() )
			OnSit( true );
	}
	else
	{
		if ( IsSitDown() )
			OnSit( false );
	}

	//For main character
	//When on crazy action state
	//we first clear all run event
	if ( IsMainRole() )
	{
		/*if ( isset_state( byState, USTATE_CRAZY_ACTION ) )
		{
			GetScene()->GetMainCharacter()->ClearRunEvent();
			if ( GetScene()->GetMainCharacter()->GetPKMode() != PKMODE_ENTIRE )
				ChangeAttackMode( PKMODE_ENTIRE );
		}*/
		if ( isset_state( byState, USTATE_CRAZY) )
		{
			GetScene()->GetMainCharacter()->ClearRunEvent();
		}
	}

	//For main character
	//When on liquid move state
	//We first remove all dynamic obstacle
	if ( IsMainRole() )
	{
		if ( isset_state( byState, USTATE_LIQUID_MOVE ) )
		{
			if ( !IsLiquidMoveState() )
			{
				//Remove all NPC & Character that is dynamic obstacle
				GetScene()->AddOrRemoveAllDynamicObstacle( false );
				m_bLiquidMoveState = true;
			}			
		}
		else
		{
			if ( IsLiquidMoveState() )
			{
				//Add all NPC & Character as dynamic obstacle
				GetScene()->AddOrRemoveAllDynamicObstacle( true );
				m_bLiquidMoveState = false;
			}
		}
	}

	//Set state color
	bool bChangedColor = false;
	if ( GetInterface() )
	{
		//Backup old color
		if ( isset_state(GetState(),USTATE_FREEZE) )
		{
			CHANGE_COLOR( COLOR_ARGB(255, 0, 0, 220) );
			bChangedColor = true;
		}

		if ( isset_state(GetState(),USTATE_POISON) )
		{
			CHANGE_COLOR( 0xff00ff00 );
			bChangedColor = true;
		}

		if ( isset_state(GetState(),USTATE_ROCK) )
		{
			CHANGE_COLOR( 0xff808080 );
			bChangedColor = true;
		}

		if ( isset_state(GetState(),USTATE_PALSY) )
		{
			CHANGE_COLOR( 0xff800080 );
			bChangedColor = true;
		}

		if ( !bChangedColor )
		{
			if ( IsCharacter() )
			{
				GetInterface()->ChangeColor( Ani_Part_Body, -1 );
				GetInterface()->ChangeColor( Ani_Part_BodySystemColor, GetBodySystemColor() );
				GetInterface()->ChangeColor( Ani_Part_BodyCustomColor, GetBodyCustomColor() );
				GetInterface()->ChangeColor( Ani_Part_Hair, GetHairColor() );
			}
			else
			{
				GetInterface()->ChangeColor( -1 );
			}
			
		}
	}

	return true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
DWORD CNpc::GetNpcPic()
{
	return GetNpcImageID(m_data.dwNpcDataID);
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CNpc::ResetImage()
{
	FUNCTION_BEGIN;

	NpcBase_t* pNpcBase;

	pNpcBase = GetNpcBase(m_data.dwNpcDataID);
	int iImageID = 1028;
	if(pNpcBase) iImageID = pNpcBase->dwPic;
	static std::vector<int> aID;
	aID.resize(1);
	aID[0] = iImageID;

	if (IsCanAttack())
		m_pCharacter->CreateCharacter(-1,aID,TRUE);
	else
		m_pCharacter->CreateCharacter(-1,aID,FALSE);

	if(m_pCharacter->IsWait())
	{
		m_pCharacter->TurnTo(m_data.byDirect);
	}

	if ( IsFireWall() && (iImageID != 1028) )
	{
		DWORD dwID = GetTypeID();
		if ( (dwID >= 42360) && (dwID <=42369) )			//Here we made some special treatment cause the img is some defferent from common npc img
		{
			stPointI ptCenter = m_pCharacter->GetDefaultCenter();
			ptCenter.y += 120;
			m_pCharacter->SetDefaultCenter( ptCenter );
		}		
	}
	
	//m_pCharacter->Action( Ani_Wait );
	//if ( (iImageID != 1028) && !IsCharacter() && !IsDie() /*&& m_pCharacter->IsExistAction(Ani_Appear)*/ )
	//	m_pCharacter->Appear();

	return true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CNpc::RemoveDynamicObstacle()
{
	FUNCTION_BEGIN;

	if(IsMainRole()) return;
	if(m_bFillObstacle)
	{
		GetScene()->RemoveDynamicObstacle(m_ptBlock);
		m_bFillObstacle = false;
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
void CNpc::AddDynamicObstacle(POINT pt)
{
	FUNCTION_BEGIN;
	
	if (IsDie()) return;
	if(IsMainRole()) return;

	if ( GetScene()->GetMainCharacter() )
	{
		if ( isset_state( GetScene()->GetMainCharacter()->GetState(), USTATE_LIQUID_MOVE ) )
		{
			m_bFillObstacle = false;
			return;
		}
	}

	if(m_bFillObstacle) return;

	//If this is a Catch Npc we don't use it as a obstacle
	if ( (GetNpcTypeItem() == NpcTypeItem_CatchItem)
		|| (GetNpcTypeItem() == NpcTypeItem_Cartoon)
		|| IsSurfaceType() )
	{
		m_bFillObstacle = false;
		return;
	}


	m_ptBlock = pt;
	GetScene()->AddDynamicObstacle(pt);
	m_bFillObstacle = true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CNpc::UpdatePosition(void)
{
	FUNCTION_BEGIN;

	m_nDirect = m_pCharacter->GetDir();
	m_ptPixelOffset = m_pCharacter->GetPos();
	POINT ptGrid = GetScene()->PixelPosToGridPos(m_ptPixelOffset);
	POINT ptDes = m_pCharacter->GetDes();
	POINT ptGridDes = GetScene()->PixelPosToGridPos(ptDes);

	if (ptGrid != m_ptGrid && (!m_pCharacter->IsMoving() || ptGrid == ptGridDes))
	{
		RemoveDynamicObstacle();
		m_ptGrid = ptGrid;
		AdjustAxis();
		AddDynamicObstacle(ptGrid);

		if(IsMainRole())
		{
			GetScene()->UpdateFindPathRect();
		}
	}
	
	FUNCTION_END;
}

bool CNpc::IsCanAutoAttackNpc()
{
	FUNCTION_BEGIN;

	if ( GetScene()->GetMainCharacter() )
	{
		if (!GetScene()->GetMainCharacter()->IsEquipedPickResourceTool() )//改成求反
			return GetNpcTypeItem() == NpcTypeItem_Resource;
	}

	if ( IsPet() )
		return false;

       //ZM 0 1 32类型怪物可自动打怪或采集
	return ((GetNpcTypeItem() == NpcTypeItem_Normal) || (GetNpcTypeItem() == NpcTypeItem_Human) || (GetNpcTypeItem() == NpcTypeItem_Resource) || (GetNpcTypeItem() == NpcTypeItem_Boss) || (GetNpcTypeItem() == NpcTypeItem_SmBoss) || (GetNpcTypeItem() == NpcTypeItem_ExGold) || (GetNpcTypeItem() == NpcTypeItem_Extract) || (GetNpcTypeItem() == NpcTypeItem_Gold) || (GetNpcTypeItem() == NpcTypeItem_RedBoss))

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param ptGrid : 描述
* \return 返回值的描述
*/
bool CNpc::UpdateGridPosition(POINT ptGrid)
{
	FUNCTION_BEGIN;

	if(IsMainRole())
	{
		GetScene()->m_pMainCharacter->UnPauseMove();
	}

	stPointI ptPixel = GetScene()->GridCenterToPixelPos(ptGrid);
	stPointI ptPos = m_pCharacter->GetPos();
	if (m_pCharacter->IsMoving() || m_pCharacter->IsMovingBreak())
	{
		ptPos = m_pCharacter->GetDes();

		stPointI ptGridDst = GetScene()->PixelPosToGridPos(ptPos);
		if (ptGridDst == ptGrid)
			return true;
	}
	else 
	{
		stPointI ptGridDst = GetScene()->PixelPosToGridPos(ptPos);
		if (ptGridDst == ptGrid)
			return true;
	}

	if(IsDie())
	{
#if defined _DEBUG || defined RELEASE_TEST
		Engine_WriteLogF("NPC(%d:%s),死亡后收到刷新坐标指令\n",m_dwID,GetName());
#endif
	}
	m_pCharacter->Stop(ptPixel);

	UpdatePosition();

	if(IsMainRole())
	{
		GetScene()->m_pMainCharacter->ClearPath();
	}

	return true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
bool CNpc::PushCommand(stNullUserCmd* pCmd, size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byCmdType) 
	{
	case MAGIC_USERCMD:
		switch(pCmd->byParameterType)
		{
		case MAGIC_USERCMD_PARA:	// 攻击指令
			m_nRecieveAttack ++;
			//TRACE("收到攻击指令,sendNum:%d ,recvNum:%d\n",m_nSendAttack,m_nRecieveAttack);
			break;
		case RTMAGIC_USERCMD_PARA:	// 攻击返回
			{
				stRTMagicUserCmd * cmd = new stRTMagicUserCmd;
				*cmd = *((stRTMagicUserCmd*) pCmd);
				if (IsMainRole()) TRACE("RTMAGIC_USERCMD_PARAMETER 主角收到受伤消息\n");
				//第一次被击
				if (IsMainRole()) OnTipHelp(USER_SETTING_HELP_ATTACK);

				cmd->dwTimestamp = xtimeGetTime();
				m_queueHPCmd.push_back(cmd);
				return true;
			}
			break;
		case RTMAGIC_POS_USERCMD_PARA:
			{
				stRTMagicPosUserCmd *p = (stRTMagicPosUserCmd*)pCmd;
				int iiiii=0;
			}
			break;
		}
		break;
	case DATA_USERCMD:
		switch(pCmd->byParam)
		{
		case SETHPANDMP_DATA_USERCMD_PARA:	// 设置用户生命和魔法
			{
				stSetHPAndMPDataUserCmd* p = new stSetHPAndMPDataUserCmd;
				*p = *(stSetHPAndMPDataUserCmd*)pCmd;
				m_queueHPCmd.push_back(p);
				return true;
			}
			break;
		}
		break;
	case MAPSCREEN_USERCMD:
		switch(pCmd->byParam)
		{
		case REMOVEMAPNPC_MAPSCREEN_USERCMD_PARA:	// 在地图上删除NPC
			m_dwID = 0;	
			break;
		case REMOVEUSER_MAPSCREEN_USERCMD_PARA:	// 地图上面删除人物
			m_dwID = 0;
			break;
		case NPCHP_MAPSCREEN_USERCMD_PARA:	// Npc血槽
			{
				stNPCHPMapScreenUserCmd* p = new stNPCHPMapScreenUserCmd;
				*p = *(stNPCHPMapScreenUserCmd*)pCmd;
				m_queueHPCmd.push_back(p);
				return true;
			}
			break;
		case SELECTED_HPMP_PROPERTY_USERCMD_PARA:	// 通知选择自己的玩家hp和mp的变化
			{
				stRTSelectedHpMpPropertyUserCmd* p = new stRTSelectedHpMpPropertyUserCmd;
				*p = *(stRTSelectedHpMpPropertyUserCmd*)pCmd;
				m_queueHPCmd.push_back(p);
				return true;
			}
			break;
		}
		break;
	}

	if (size != 0)
	{
		m_queueCmd.Push(pCmd,size);
		m_timeCommand = xtimeGetTime();
		return true;
	}

	return false;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CNpc::OnCommandNotCome()
{
	FUNCTION_BEGIN;


	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \return 返回值的描述
*/
bool CNpc::HandleCommand(stNullUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	bool bHandle = true;
	switch(pCmd->byCmdType) 
	{
	case MAPSCREEN_USERCMD:								// 地图指令
		switch(pCmd->byParameterType)
		{
		case ADDMAPNPC_MAPSCREEN_USERCMD_PARA:		// 在地图上增加NPC
			if(IsDie())
			{
#if defined _DEBUG || defined RELEASE_TEST
				Engine_WriteLogF("NPC(%d:%s),死亡后刷新属性\n",m_dwID,GetName());
#endif
			}
			SetAttrib(&((stAddMapNpcMapScreenUserCmd*)pCmd)->data);
			break;
		case ADDUSER_MAPSCREEN_USERCMD_PARA:		// 地图上增加人物
			Assert(IsCharacter());
			if(IsCharacter())
			{
				((CCharacter*)this)->SetAttrib(&((stAddUserMapScreenUserCmd*)pCmd)->data);				
			}
			break;
		case REMOVEUSER_MAPSCREEN_USERCMD_PARA:	// 地图上面删除人物
			m_bDeleted = true;
			break;
		case REMOVEMAPNPC_MAPSCREEN_USERCMD_PARA:	// 在地图上删除NPC
			if(IsDie())
			{

#if defined _DEBUG || defined RELEASE_TEST
				Engine_WriteLogF("NPC(%d:%s),死亡后收到删除指令，开始尸体淡出\n",m_dwID,GetName());
#endif
				m_bAshesFadeOut = true;
				m_nAshesFadeOutTime = 1000;
			}
			else
			{
				m_bDeleted = true;
			}
			break;
		case NPCHP_MAPSCREEN_USERCMD_PARA:
			{
				stNPCHPMapScreenUserCmd* cmd = (stNPCHPMapScreenUserCmd*)pCmd;
				SetHP(cmd->dwHP);
				SetMaxHP(cmd->dwMaxHP);
				//TRACE("-------------hp = %d,maxHp=%d\n",cmd->dwHP,cmd->dwMaxHP);
				TRACE("-------------hp = %I64u,maxHp=%I64u\n",cmd->dwHP,cmd->dwMaxHP); //by=>friday
				bHandle =true;
			}
			break;
		case SELECTED_HPMP_PROPERTY_USERCMD_PARA: // 通知选择自己的玩家hp和mp的变化
			{
				//if ( GetScene()->m_pInterestNpc && GetScene()->m_pInterestNpc == this)
				{
					stRTSelectedHpMpPropertyUserCmd* cmd = (stRTSelectedHpMpPropertyUserCmd*)pCmd;
					//To see if the same NPC
					//Set interest npc property
					SetMP( cmd->dwMP );
					SetMaxMP( cmd->dwMaxMp );
					SetHP( cmd->dwHP );
					SetMaxHP( cmd->dwMaxHp );
					TRACE("hp = %d,maxHp=%d\n",cmd->dwHP,cmd->dwMaxHp);
				}
			}
			break;
		case SETSTATE_MAPSCREEN_USERCMD_PARA:
			{
				stSetStateMapScreenUserCmd* cmd = (stSetStateMapScreenUserCmd*)pCmd;
				set_state( GetState(), cmd->wdState );
				ResetState();
				bHandle = true;
			}
			break;
		}
		break;
	case MOVE_USERCMD:
		switch (pCmd->byParameterType)
		{
		case NPCMOVE_MOVE_USERCMD_PARA:	// Npc移动指令
			{
				if(IsDie())
				{
#if defined _DEBUG || defined RELEASE_TEST
					Engine_WriteLogF("NPC(%d:%s),死亡后收到移动指令\n",m_dwID,GetName());
#endif
				}

				if ( IsPet() )
					int temp = 10;

				stNpcMoveMoveUserCmd* _pCmd = (stNpcMoveMoveUserCmd* )pCmd;				
				stUserMoveMoveUserCmd cmd;				
				memset(&cmd,0,sizeof(cmd));
				cmd.x = _pCmd->x;
				cmd.y = _pCmd->y;
				cmd.byDirect = _pCmd->byDirect;
				cmd.bySpeed = _pCmd->bySpeed;
				bHandle = OnLoopMove(&cmd);

				if (bHandle)
				{
					m_nRecieveWalk ++;
				}
			}
			break;
		case USERMOVE_MOVE_USERCMD_PARA:	// 用户移动
			bHandle = OnLoopMove((stUserMoveMoveUserCmd*) pCmd);
			if (bHandle)
			{
				m_nRecieveWalk ++;
			}
			break;
		case USERINSTANTJUMP_MOVE_USERCMD_PARA:	// 网关向主用户发送的瞬移指令
			{
				stUserInstantJumpMoveUserCmd* _pCmd = (stUserInstantJumpMoveUserCmd*) pCmd;
				CSceneObject::UpdatePosition(_pCmd->dwNextScreenPosition,_pCmd->wdNextOffsetX,_pCmd->wdNextOffsetY);
			}
			break;
		}
		break;
	case MAGIC_USERCMD:
		bHandle = OnLoopMagic((stMagicUserCmd*) pCmd);
		break;
	case RELIVE_USERCMD:	// 复活指令
		switch( pCmd->byParameterType ) 
		{
		case MAINUSERDEATH_RELIVE_USERCMD_PARA:	// 主用户死亡指令
			//TRACE("handle cmd death\n");
			m_bDieWaitingMagic = true;
			m_bDied = true;
			m_nDieWaitingTime = 1000;
			break;
		case MAINUSERRELIVE_RELIVE_USERCMD_PARA:	// 主用户复活指令
			{
				Relive((stMainUserReliveReliveUserCmd*)pCmd);
			}
			break;
		}
		break;
	case DATA_USERCMD:
		switch (pCmd->byParameterType)
		{
		case SETHPANDMP_DATA_USERCMD_PARA:	// 设置用户生命和魔法
			{
				stSetHPAndMPDataUserCmd* pSetHPAndMP = (stSetHPAndMPDataUserCmd*) pCmd;
				//Modifyed By david For 2006-3-14
				((CMainCharacter*)this)->SetHPAndMP(pSetHPAndMP->dwHP,pSetHPAndMP->dwMP,0/*pSetHPAndMP->dwSP*/);
			}
			break;
		case MAIN_USER_DATA_USERCMD_PARA:	// 主用户数据
			((CMainCharacter*)this)->SetAttrib( &((stMainUserDataUserCmd*)pCmd)->data );
			if ( ((stMainUserDataUserCmd*)pCmd)->data.skillPoint != GetScene()->GetMainCharacter()->GetSkillPoint() )
			{
				if (GetGameGuiManager()->m_guiSkill)
					GetGameGuiManager()->m_guiSkill->RefreshState();	
			}
			
			if (GetGameGuiManager()->m_guiItem)
			{   //新增积分相关
				GetGameGuiManager()->m_guiItem->SetGoldSalary(((stMainUserDataUserCmd*)pCmd)->data.gold,((stMainUserDataUserCmd*)pCmd)->data.salary); //工资
				GetGameGuiManager()->m_guiItem->SetTicket(((stMainUserDataUserCmd*)pCmd)->data.ticket);	 //积分
			  //GetGameGuiManager()->m_guiItem->SetChongzhi(((stMainUserDataUserCmd*)pCmd)->data.chongzhi);	 //充值点
			}
			

/*			if(GetGameGuiManager()->m_guiNpcDialog && GetGameGuiManager()->m_guiNpcDialog->IsMoveCanClose())
			{
				GetGameGuiManager()->EndNpcDialog();
			}*/			

			break;
		}
		break;
	case PROPERTY_USERCMD:
		switch( pCmd->byParameterType )
		{
		case SELECT_RETURN_STATES_PROPERTY_USERCMD_PARA:
			{
				stSelectReturnStatesPropertyUserCmd* cmd = (stSelectReturnStatesPropertyUserCmd*)pCmd;

				if ( (cmd->byType == MAPDATATYPE_USER) && (GetScene()->GetMainCharacter()->GetID() == cmd->dwTempID) )
				{
					//Add self state to GuiMain dlg
					if ( GetGameGuiManager()->m_guiMain )
					{
						for( int i=0;i<cmd->size;i++ )
						{
							//if ( isset_state( GetScene()->GetMainCharacter()->GetState(), cmd->states[i].state ) )
							/*GetGameGuiManager()->AddClientSystemMessage( "收到状态改变消息！！！" );
							GetGameGuiManager()->AddClientSystemMessage( avar( "%u", cmd->states[i].state ) );*/
							GetGameGuiManager()->m_guiMain->AddOneState( true, false, cmd->states[i].state, cmd->states[i].time, cmd->states[i].result );
						}
					}
				}
				else if ( ( GetScene()->m_pInterestNpc && (cmd->byType == MAPDATATYPE_NPC) )		//Other NPC( this time just the interest npc )
					|| ( GetScene()->m_pInterestNpc &&  (cmd->byType == MAPDATATYPE_USER) && (cmd->dwTempID == GetScene()->m_pInterestNpc->GetID() ) ) )
				{
					//Add interest npc state to GuiMain dlg
					if ( GetGameGuiManager()->m_guiMain )
					{
						for( int i=0;i<cmd->size;i++ )
						{
							//if ( isset_state( GetScene()->m_pInterestNpc->GetState(), cmd->states[i].state ) )
							GetGameGuiManager()->m_guiMain->AddOneState( false, false, cmd->states[i].state, cmd->states[i].time, cmd->states[i].result );
						}
					}
				}
			}
			break;
		}
	}

	return bHandle;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CNpc::HandleCommand()
{
	FUNCTION_BEGIN;

	if(IsCommandEmpty())
	{
		if(xtimeGetTime() - m_timeCommand > 16000)
		{
			OnCommandNotCome();
		}
		return false;
	}

	stNullUserCmd* pCmd = (stNullUserCmd* )m_queueCmd.GetFront();

	/*if (IsDie() && !m_bRelife)
	{
	if ( pCmd->byCmdType == RELIVE_USERCMD && pCmd->byParameterType == MAINUSERRELIVE_RELIVE_USERCMD_PARA )
	{
	m_bRelife = true;
	}
	else
	{
	m_queueCmd.Pop();
	return true;
	}
	}*/

	if (HandleCommand(pCmd))
	{
		m_queueCmd.Pop();
		return true;
	}
	return false;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \return 返回值的描述
*/
bool CNpc::OnLoopMove(stUserMoveMoveUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	if(IsMainRole())
	{
#if defined _DEBUG || defined RELEASE_TEST
	//	Engine_WriteLogF("收到主角移动指令，目标点(%d,%d)\n",pCmd->x,pCmd->y);
#endif
		((CMainCharacter*)this)->UnPauseMove();
	}
	//TRACE( "收到%s移动指令，速度：%d\n", GetName(), pCmd->bySpeed );
	if ( m_pCharacter->IsDie() )		// || IsDie() 
		return true; 

	int byDirect = pCmd->byDirect;
	int bySpeed = pCmd->bySpeed;

	POINT ptDes = GetScene()->GridCenterToPixelPos(stPointI(  pCmd->x,pCmd->y ));

	/*if ( GetTypeID() == 10032 )
	{
		TRACE("-处理移动指令，ID:%u,cur:(%u,%u),des:(%u,%u),speed:%u, time=%u\n",
			GetID(),GetGridPos().x,GetGridPos().y,pCmd->x,pCmd->y,pCmd->bySpeed, pCmd->dwTimestamp);
	}	*/

	/*if ( GetTypeID() == 10052 )
	{
		POINT ptPos = m_pCharacter->GetPos();
		TRACE("-处理移动指令，cur:(%u,%u),des:(%u,%u),speed:%u, time=%u\n",
			GetGridPos().x,GetGridPos().y,pCmd->x,pCmd->y,pCmd->bySpeed, pCmd->dwTimestamp);
	}*/

	if ( bySpeed == 0 )
	{
#ifdef _DEBUG
		if(IsMainRole())
		{
			TRACE( "MainCharacter接收服务器转方向指令\n" );
		}
#endif
		POINT ptPos = m_pCharacter->GetPos();
		if ( ptDes.x == ptPos.x && ptDes.y == ptPos.y )
		{
			if ( !m_pCharacter->IsMoving() )
			{
				m_pCharacter->TurnTo(byDirect);
			}
			return true;
		}
		bySpeed = 1;
	}

	if ( IsMainRole() && (m_nRecieveWalk < m_nSendWalk) )
	{
		//GetScene()->GetMainCharacter()->Goto( stPointI(pCmd->x, pCmd->y) );
		UpdateGridPosition(stPointI(pCmd->x,pCmd->y));
		return true;
	}
	
	if ( GetNpcTypeItem() == NpcTypeItem_Pet )
	{
		static int dir = m_pCharacter->GetDir();
		if ( m_pCharacter->GetDir() != dir )
		{
			dir = m_pCharacter->GetDir();
			//TRACE( "**************************************************************************Walk to dir = %u curPos(%u,%u), dstPos(%u,%u)\n", m_pCharacter->GetDir(), GetGridPos().x, GetGridPos().y, pCmd->x,pCmd->y );
		}
	}

	if (MoveTo(ptDes,bySpeed,true))
	{
		return true;
	}

	return false;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param ptDes : 描述
* \param nSpeed : 描述
* \param bServerLogic : 描述
* \return 返回值的描述
*/
bool CNpc::MoveTo(POINT ptDes,int nSpeed,bool bServerLogic)
{
	FUNCTION_BEGIN;

	if (m_pCharacter->IsMoving())
		return false;

	POINT ptCur = GetGridPos();
	POINT ptGridDes = GetScene()->PixelPosToGridPos(ptDes);

	float fChangeSpeed = 1.0f;
	long dis = Scene_GetDis(ptCur,ptGridDes);
	if(dis != nSpeed)
	{
		if(dis == 0)
		{
			m_pCharacter->Stop(ptDes);
			UpdatePosition();
			return true;
		}
		//fChangeSpeed = (float)nSpeed/(float)dis;
	}
	
	RemoveDynamicObstacle();
	AddDynamicObstacle(ptGridDes);

	bool  b = false;
	if (nSpeed == 1 || IsNpc())
	{
		b = m_pCharacter->WalkTo(ptDes,bServerLogic/*,fChangeSpeed*/);
	}
	else
		b = m_pCharacter->RunTo(ptDes,bServerLogic/*,fChangeSpeed*/);
	return b;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pSkill : 描述
* \return 返回值的描述
*/
int CNpc::ServerSkillToClientSkill( SkillBase_t* pSkill )
{
	FUNCTION_BEGIN;

	if( pSkill )
	{
		switch(pSkill->dwType)
		{
		case SkillType_Attack:
		case SkillType_StateAttack:
		case SkillType_Active:
			return pSkill->dwPic;
			break;
		}
	}
	return -1;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pSkill : 描述
* \return 返回值的描述
*/
inline int GetServerSkillLife(SkillBase_t* pSkill)
{
	FUNCTION_BEGIN;

	if(pSkill)
	{
		switch(pSkill->dwType)
		{
		case SkillType_StateAttack:
		case SkillType_State:
			return 3000;
			break;
		}
	}
	return 0;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pTarget : 描述
* \param iClientSkill : 描述
* \param bServerLogic : 描述
* \param nLife : 描述
* \param ptObs : 描述
* \param nAction : 描述
* \return 返回值的描述
*/
CNpc::enumCastMagicResult CNpc::CastMagic(CNpc* pTarget, int iClientSkill, bool bServerLogic,int nLife, POINT& ptObs,AniTypeEnum nAction)
{
	FUNCTION_BEGIN;

	iMagic* pMagic = NULL;
	int rt = 1;
	MagicProperty* pP = Property_GetTableManager()->GetMagicProperty(iClientSkill,m_pCharacter->GetMagicTableID());
	if (pP && pP->stBlock.eBlock & stBlockStruct::Block_Obstacle)
	{
		if(GetScene()->GetTileFlagsNum(GetGridPos(),pTarget->GetGridPos(),ptObs,TILE_MAGIC_BLOCK) >= 1)
		{
			// 如果中间有魔法阻挡。
			enumCastMagicResult e = CastMagicA(GetScene()->GridCenterToPixelPos(ptObs),iClientSkill,bServerLogic,nLife,nAction);
			if (eCastMagicResult_Succeeded != e)
				return e;
			return eCastMagicResult_Obstruct;
		}
	}

	enumCreateMagicResult rt2 = m_pCharacter->Magic(pTarget->m_pCharacter,iClientSkill,&pMagic,bServerLogic,nLife,nAction);

	if(rt2 == CreateMagicResult_AniError)
		return eCastMagicResult_ActionError;

	if ( pMagic == NULL )
		return eCastMagicResult_Succeeded;

	GetScene()->AddMagic(pMagic);

	pMagic->Release();

	return eCastMagicResult_Succeeded;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param ptDes : 描述
* \param iClientSkill : 描述
* \param bServerLogic : 描述
* \param nLife : 描述
* \param ptObs : 描述
* \param nAction : 描述
* \return 返回值的描述
*/
CNpc::enumCastMagicResult CNpc::CastMagic(POINT ptDes, int iClientSkill, bool bServerLogic,int nLife, POINT& ptObs,AniTypeEnum nAction)
{
	FUNCTION_BEGIN;

	iMagic* pMagic = NULL;
	if (!m_pCharacter)
		return eCastMagicResult_Other;

	MagicProperty* pP = Property_GetTableManager()->GetMagicProperty(iClientSkill,m_pCharacter->GetMagicTableID());
	if (pP && pP->stBlock.eBlock & stBlockStruct::Block_Obstacle)
	{
		if(GetScene()->GetTileFlagsNum(GetGridPos(),GetScene()->PixelPosToGridPos(ptDes),ptObs,TILE_MAGIC_BLOCK) >= 1)
		{
			// 如果中间有魔法阻挡。
			ptDes = GetScene()->GridCenterToPixelPos(ptObs);
		}
	}

	return CastMagicA(ptDes,iClientSkill,bServerLogic,nLife,nAction);

	FUNCTION_END;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \param ptDes : 描述
* \param iClientSkill : 描述
* \param bServerLogic : 描述
* \param nLife : 描述
* \param nAction : 描述
* \return 返回值的描述
*/
CNpc::enumCastMagicResult CNpc::CastMagicA(POINT ptDes, int iClientSkill, bool bServerLogic,int nLife,AniTypeEnum nAction)
{
	FUNCTION_BEGIN;

	iMagic* pMagic = NULL;
	if (!m_pCharacter)
		return eCastMagicResult_Other;

	enumCreateMagicResult rt = m_pCharacter->Magic(ptDes,iClientSkill,&pMagic,bServerLogic,nLife,nAction);

	if(rt == CreateMagicResult_AniError)
		return eCastMagicResult_ActionError;

	if ( pMagic == NULL )
		return eCastMagicResult_Succeeded;

	GetScene()->AddMagic(pMagic);
	pMagic->Release();

	return eCastMagicResult_Succeeded;

	FUNCTION_END;
}

bool CNpc::IsFireWall()
{
	DWORD id = GetTypeID();
	if ( ((id >= 42360) && (id <=42369))
		|| ((id >= 44021) && (id <= 44030)) )
		return true;

	return false;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param ptObs : 描述
* \param bServerLogic : 描述
* \return 返回值的描述
*/
CNpc::enumCastMagicResult CNpc::CastMagic( stAttackMagicUserCmd* pCmd,POINT& ptObs,bool bServerLogic )
{
	FUNCTION_BEGIN;

	if (pCmd->wdMagicType == -1)
		return eCastMagicResult_Succeeded;

	SkillBase_t* pSkill = NULL;
	//For flower npc we change skill to SKILL_ATTACK_FLOWER
	{
		CNpc* pNpc = GetScene()->FindNpcByID( pCmd->dwDefenceTempID );
		if ( pNpc && (pNpc->GetNpcTypeItem() == NpcTypeItem_Flower) )
			pSkill = GetSkillBase( SKILL_ATTACK_FLOWER );
		else if	( pNpc && (pNpc->GetNpcTypeItem() == NpcTypeItem_Resource) )
			pSkill = GetSkillBase( SKILL_ATTACK_FLOWER );
		else
			pSkill = GetSkillBase( pCmd->wdMagicType );			
	}
	//For combine state we change magic type for normal attack
	if ( isset_state(GetState(), USTATE_COMBIN_GOD_FIGHTER) )
	{
		if ( pCmd->wdMagicType == SERVER_SKILL_BOW_NORMAL )
			pSkill = GetSkillBase( SKILL_COMBIN_BOW_ARROW );
	}
	if ( isset_state(GetState(), USTATE_EAT_GHOST) )
	{
		if ( pCmd->wdMagicType == SERVER_SKILL_BOW_NORMAL )
			pSkill = GetSkillBase( SKILL_EAT_GHOST_NORMAL_SKILL );
	}
	if ( isset_state(GetState(), USTATE_COMBIN_XIAN_PET) )
	{
		if ( pCmd->wdMagicType == SERVER_SKILL_BOW_NORMAL )
			pSkill = GetSkillBase( SKILL_COMBIN_XIAN_PET_NORMAL_SKILL );
	}
	if ( isset_state(GetState(), USTATE_COMBIN_WIND_PET) )
	{
		if ( pCmd->wdMagicType == SERVER_SKILL_BOW_NORMAL )
			pSkill = GetSkillBase( SKILL_COMBIN_WIND_PET_NORMAL_SKILL );
	}
	if ( isset_state(GetState(), USTATE_COMBIN_FIRE_LEADER) )
	{
		if ( pCmd->wdMagicType == SERVER_SKILL_BOW_NORMAL )
			pSkill = GetSkillBase( SKILL_COMBIN_FIRE_LEADER_NORMAL_SKILL );
	}
	if ( isset_state(GetState(), USTATE_COMBIN_SIX_GHOST) )
	{
		if ( pCmd->wdMagicType == SERVER_SKILL_BOW_NORMAL )
			pSkill = GetSkillBase( SKILL_COMBIN_SIX_GHOST_NORMAL_SKILL );
	}
	if ( isset_state(GetState(), USTATE_COMBIN_THREE_DIM) )
	{
		if ( pCmd->wdMagicType == SERVER_SKILL_BOW_NORMAL )
			pSkill = GetSkillBase( SKILL_COMBIN_THREE_DIM_NORMAL_SKILL );
	}
	//end combin treat

	//SkillBase_t* pSkill = GetSkillBase( 8015 );		//Just a test
	//SkillBase_t* pSkill = GetSkillBase(pCmd->wdMagicType);
	if(!pSkill) return eCastMagicResult_Succeeded;

	stPointI ptDes = GetScene()->GridCenterToPixelPos( stPointI(pCmd->xDes,pCmd->yDes) );
	int iClientSkill = ServerSkillToClientSkill(pSkill);

	if (iClientSkill < 0) //被动技能
		return eCastMagicResult_Succeeded;

	int nLife = GetServerSkillLife(pSkill);

	CNpc* pNpc = NULL;
	if ( pCmd->dwDefenceTempID != -1 )
	{
		switch (pCmd->byAttackType)
		{
		case ATTACKTYPE_N2N:
		case ATTACKTYPE_U2N:
			pNpc = GetScene()->FindNpcByID(pCmd->dwDefenceTempID);
			break;
		case ATTACKTYPE_N2U:
		case ATTACKTYPE_U2U:
			pNpc = GetScene()->FindCharacterByID(pCmd->dwDefenceTempID);
			break;
		}
	}

	if ( ((AniTypeEnum)pCmd->byAction >= Ani_Num) )
	{
		GetInterface()->SetMagicRing( iClientSkill, 1000 );
		return eCastMagicResult_Succeeded;
	}

	if ( GetNpcTypeItem() == NpcTypeItem_CatchItem )
	{
		//We let defencer use this skill
		CNpc* pDef = GetScene()->FindNpcByID( pCmd->dwDefenceTempID );
		if ( (pCmd->byAttackType == ATTACKTYPE_U2N)
			|| (pCmd->byAttackType == ATTACKTYPE_N2N) )
		{
			pDef = GetScene()->FindNpcByID( pCmd->dwDefenceTempID );
		}
		else
			pDef = GetScene()->FindCharacterByID( pCmd->dwDefenceTempID );

		if ( pDef )
		{
			pDef->GetInterface()->SetMagicRing( iClientSkill, 1000 );		
			return eCastMagicResult_Succeeded;
		}
	}

	/*if ( pSkill->dwPic == 54 )
	{
		GetInterface()->SetMagicRing( 54, 1000 );
		return eCastMagicResult_Succeeded;
	}*/

	enumCastMagicResult e;
	if ( !pNpc )
		e = CastMagic(ptDes,iClientSkill,bServerLogic,nLife,ptObs,(AniTypeEnum)pCmd->byAction);
	else
		e = CastMagic(pNpc,iClientSkill,bServerLogic,nLife,ptObs,(AniTypeEnum)pCmd->byAction);

	if ( (e == eCastMagicResult_ActionError) && ((AniTypeEnum)pCmd->byAction >= Ani_Num) )
		e = eCastMagicResult_Succeeded;

	return e;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \return 返回值的描述
*/
bool CNpc::Back(stBackOffMagicUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	if (m_pCharacter->IsCreate())
	{
		POINT pt = GetScene()->GridCenterToPixelPos(stPointI(pCmd->x,pCmd->y));
		POINT ptPos = GetPos();
		if (abs(pt.x - ptPos.x) == 0 && abs(pt.y - ptPos.y) == 0)
		{
			return true;
		}
		return m_pCharacter->Back(pt,true);
	}
	return true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \return 返回值的描述
*/
bool CNpc::OnLoopMagic(stMagicUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParameterType)
	{
	case OBJECT_HPMP_POP_PARA:		//Pop HP/MP value effect
		{
			stObjectHpMpPopUserCmd* p = (stObjectHpMpPopUserCmd*)pCmd;
			if ( p->type < POP_NUM )
			{
				AddHpMpPopEffect( p->type, p->vChange );
				return true;
			}
		}
		break;
	case OBTAINEXP_USERCMD_PARA:	// 攻击得到经验值
		{
			if( IsMainRole() )
			{
				stObtainExpUserCmd* p = (stObtainExpUserCmd*)pCmd;
//#if defined _DEBUG || defined RELEASE_TEST
//				GetGameGuiManager()->AddClientSystemMessage(avar("攻击得到经验值[%d][%d][%d][%d]\n",p->dwTempID,p->byType,p->dwExp,p->dwUserExp));
//#endif
				if( p->dwTempID == 0 )
				{
					((CMainCharacter*)this)->AddExp( p->dwExp,p->dwUserExp );
					return true;
				}
				if(	(p->byType == MAPDATATYPE_NPC && GetScene()->FindNpcByID(p->dwTempID)) ||
					(p->byType == MAPDATATYPE_USER && GetScene()->FindCharacterByID(p->dwTempID)) 
					)
				{
					stObtainExp ex;
					stObtainExpUserCmd& cmd = *((stObtainExpUserCmd*)pCmd);
					ex.dwSrcID = cmd.dwTempID;
					ex.eSrcType = (enumMapDataType)cmd.byType;
					ex.dwExp = cmd.dwExp;
					ex.dwAbsExp = cmd.dwUserExp;
					ex.dwTime = xtimeGetTime();
					((CMainCharacter*)this)->m_listExpCmd.push_back(ex);
					return true;
				}
				else
				{
					((CMainCharacter*)this)->AddExp( p->dwExp,p->dwUserExp );
					return true;
				}
			}
			return true;
		}
		break;

	case RTMAGIC_POS_USERCMD_PARA:	// 返回目标坐标
		{

			stRTMagicPosUserCmd* _pCmd = (stRTMagicPosUserCmd*)pCmd;
			//TRACE("服务器返回目标坐标 %u: (%u,%u)\n",GetID(),_pCmd->pos.x,_pCmd->pos.y);
			if ( IsFireWall() )
			{
				TRACE("服务器返回目标坐标 %u: (%u,%u) : MainCharacterPos(%u,%u)\n",GetID(),_pCmd->pos.x,_pCmd->pos.y,GetScene()->GetMainCharacter()->GetGridPos().x,GetScene()->GetMainCharacter()->GetGridPos().y);
				/*_pCmd->pos.byDir = GetScene()->GetMainCharacter()->GetInterface()->GetDir();
				_pCmd->pos.x = GetScene()->GetMainCharacter()->GetGridPos().x;
				_pCmd->pos.y = GetScene()->GetMainCharacter()->GetGridPos().y;*/
			}
			UpdateGridPosition(stPointI(_pCmd->pos.x,_pCmd->pos.y));
			if(_pCmd->pos.byDir >= _DIR_UP && _pCmd->pos.byDir <= _DIR_LEFTUP)
			{
				if(m_pCharacter->IsWait())
				{
					m_pCharacter->TurnTo(_pCmd->pos.byDir,true);
					if ( (GetTypeID() >= 44021) && (GetTypeID() <= 44030) )
					{
						TRACE( "\nAdd fire way NPC id=%u   dir=%u", GetID(), _pCmd->pos.byDir );
					}
				}
			}
			return true;
		}
		break;
	case MAGIC_USERCMD_PARA:	// 攻击指令
		{
			if ( IsMainRole() && ( ((AniTypeEnum)((stAttackMagicUserCmd*)pCmd)->byAction) >= Ani_Num ) )
			{
				SkillBase_t* pSkill = GetSkillBase( ((stAttackMagicUserCmd*)pCmd)->wdMagicType );
				if ( pSkill )
				{
					if ( 0 )
					{
						int iClientSkill = ServerSkillToClientSkill( pSkill );
						if ( -1 != iClientSkill )
							GetInterface()->SetMagicRing( iClientSkill, 1000 );
					}
					else
					{
						stAttackMagicUserCmd* cmd = (stAttackMagicUserCmd*)pCmd;
						CNpc* pNpc = NULL;
						if ( cmd->byAttackType == ATTACKTYPE_U2U )
							pNpc = GetScene()->FindCharacterByID( cmd->dwUserTempID );

						else if(cmd->byAttackType == ATTACKTYPE_U2P)
						{
							pNpc = GetScene()->FindCharacterByID( cmd->dwUserTempID );
							if(!pNpc)
							{
								pNpc = GetScene()->FindNpcByID( cmd->dwUserTempID );
							}
						}
						else
							pNpc = GetScene()->FindNpcByID( cmd->dwUserTempID );

						if ( pNpc )
							pNpc->CastMagic( cmd, stPointI(cmd->xDes, cmd->yDes), true );
						else
						{
							int iClientSkill = ServerSkillToClientSkill( pSkill );
							if ( -1 != iClientSkill )
							{
								stPointI ptPos = GetScene()->GridCenterToPixelPos( stPointI(cmd->xDes, cmd->yDes) );							
								GetScene()->AddMagic( iClientSkill, ptPos );
							}
						}
					}
				}
				return true;
			}
			if (IsMainRole())
				return true;
			stPointI pt;
			enumCastMagicResult e = CastMagic((stAttackMagicUserCmd*)pCmd,pt,true);

			//If is attacked by my team leader, we save the object for Special Skill use
			if ( isset_state( GetScene()->GetMainCharacter()->GetState(), USTATE_TEAM_ATTACK_ONE_DIM ) )
			{
				CNpc* pNpc = NULL;
				if ( GetGameGuiManager()->m_guiTeam )
				{
					CGuiTeam* pTeam = GetGameGuiManager()->m_guiTeam;
					stAttackMagicUserCmd* cmd = (stAttackMagicUserCmd*)pCmd;
					//What we care is only the team leader
					if ( pTeam->IsTeam() && pTeam->IsTeamHead( GetID() ) )
					{
						if ( cmd->dwDefenceTempID != -1 )
						{
							switch (cmd->byAttackType)
							{
							case ATTACKTYPE_N2N:
							case ATTACKTYPE_U2N:
								pNpc = GetScene()->FindNpcByID(cmd->dwDefenceTempID);
								break;
							case ATTACKTYPE_N2U:
							case ATTACKTYPE_U2U:
								pNpc = GetScene()->FindCharacterByID(cmd->dwDefenceTempID);
								break;
							}
						}
						GetTeamInfo()->SetLeaderAttackNpc( pNpc );
					}
				}				
			}

			return (e != eCastMagicResult_ActionError);
			//return (e == eCastMagicResult_Succeeded || e == eCastMagicResult_Obstruct);
			return true;
		}
		break;
	case NPCDEATH_USERCMD_PARA:	// NPC死亡
		{
			m_bDieWaitingMagic = true;
			m_bDied = true;
			m_nDieWaitingTime = 1000;

			return true;
		}
		break;
	case BACKOFF_USERCMD_PARA:	// 防御方后退
		return Back((stBackOffMagicUserCmd*)pCmd);
		break;
	case LEVELUP_USERCMD_PARA:	// 人物等级提升
		LevelUp();
		return true;
		break;
	case FIREWORK_USERCMD_PARA:
		{
			stFireWorkUserCmd* cmd = (stFireWorkUserCmd*)pCmd;

			// 2010 9
			if ( cmd->dwFireID != 0 )		//We have pre played a particle sys for FireID == 0
			{
					GetScene()->AddMagic( cmd->dwFireID, GetInterface()->GetPos(), -1 );
			}
			else
			{
						if ( GetParticleSysMgr() )
						{
							//First we play a firework
							//And let Rose Rose IOU be his child
							D3DXVECTOR3 vEffect;
							vEffect.x = GetDevice()->GetWidth() / 2;
							vEffect.y = GetDevice()->GetHeight() / 3;
							vEffect.z = 0;

							float fStartTime = 0.0f;

							GetParticleSysMgr()->LoadOneParticleSystem( "Spring\\SpringFireWork0", vEffect );
							CParticleSystem* pSys = GetParticleSysMgr()->GetActiveParticleSystem();
							if ( pSys )
							{
								pSys->UseCharacterOffset( false );

								//Set sys life						
								pSys->Play( pSys->GetSysLife() );
								pSys->SetLoopPlay( false );

								fStartTime = pSys->GetSuppressTimeEnd();
							}
							//Load	RoseRoseIOU
							vEffect.y = 0;
							GetParticleSysMgr()->LoadOneParticleSystem( "Spring\\RoseRoseIOU", vEffect );
							pSys = GetParticleSysMgr()->GetActiveParticleSystem();
							if ( pSys )
							{
								pSys->UseCharacterOffset( false );

								//Set sys life						
								pSys->Play( 20 );
								pSys->SetLoopPlay( false );

								pSys->SetSuppressTimeStart( 0.0f );
								pSys->SetSuppressTimeEnd( fStartTime );
							}
						}
			}

			return true;
		}
		break;
	}
	return false;

	FUNCTION_END;
}

//#define ShowHelpCase(level, type) {case level: ShowHelpTip(type); break;}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CNpc::LevelUp()
{
	FUNCTION_BEGIN;

	if( IsMainRole() )
	{
		//soke 这个不用了
	//	::GetGameGuiManager()->m_guiMain->GetButton( 112 )->SetVisible( true );
	//	::GetGameGuiManager()->m_guiMain->GetButton( 113 )->SetVisible( true );
		int Level = GetScene()->GetMainCharacter()->GetLevel();

		/////////////////////////////////////////////////////
		/*
		 触发
		 如果累积属性点数>=1点，则系统自动弹出人物属性界面的第二分页
		 (属性点分配页面），并系统提示：
		 "你有多余的属性点未分配，适当分配后会提升相应的能力!"
		 
		 如果累积技能点数>=5点，则系统自动弹出技能界面，并系统提示：
		 "你有多余的技能点未分配，适当分配后会提升相应技能等级!"
		*/

		WORD points = GetScene()->GetMainCharacter()->GetProperty()->points;
		if (points >= 1)
		{
			//GetGameGuiManager()->AddUserInfo(1);
			if (GetGameGuiManager()->m_guiMain)
			{
				GetGameGuiManager()->m_guiMain->AddshuxBtn(true);
			}			
			//GetGameGuiManager()->AddMessageBox("你有多余的 属性点 未分配，适当分配后会提升相应技能等级!");
		}
		//soke 增加一个技能提示
		WORD skillpoints = GetScene()->GetMainCharacter()->GetProperty()->skillPoint;
							
		if (skillpoints >= 5)
		{
			if (GetGameGuiManager()->m_guiMain)
			{
				GetGameGuiManager()->m_guiMain->AddjinengBtn(true);
			}
		}

			//GetGameGuiManager()->AddSkill(true);
			//GetGameGuiManager()->AddMessageBox("你有多余的 技能点 未分配，适当分配后会提升相应技能等级!");
		//}
/////////////////////////////////////////////////////////////
		
		/*
		 触发
		 如果累积属性点数>=1点，则系统自动弹出人物属性界面的第二分页
		 (属性点分配页面），并系统提示：
		 "你有多余的属性点未分配，适当分配后会提升相应的能力!"
		 
		 如果累积技能点数>=5点，则系统自动弹出技能界面，并系统提示：
		 "你有多余的技能点未分配，适当分配后会提升相应技能等级!"
		*/

	/*	WORD points = GetScene()->GetMainCharacter()->GetProperty()->points;
		WORD skillpoints = GetScene()->GetMainCharacter()->GetProperty()->skillPoint;
		if (points >= 1)
		{
			//GetGameGuiManager()->AddUserInfo(1);
			if (GetGameGuiManager()->m_guiMain)
			{
				GetGameGuiManager()->m_guiMain->AddshuxBtn(true);
			}			
			//GetGameGuiManager()->AddMessageBox("你有多余的 属性点 未分配，适当分配后会提升相应技能等级!");
		}
		else
		{
			if (GetGameGuiManager()->m_guiMain)
			{
				//GetGameGuiManager()->m_guiMain->AddshuxBtn(false);
			}
		}
		if (skillpoints >= 5)
		{
			//GetGameGuiManager()->AddSkill(true);
			//GetGameGuiManager()->AddMessageBox("你有多余的 技能点 未分配，适当分配后会提升相应技能等级!");
		}*/
		//////////////////////////////////////////////////////////////////////////
		//触发"加点属性"事件
		OnTipHelp(USER_SETTING_HELP_ADDPROPPOINT);

		//触发"学习"事件
		OnTipHelp(USER_SETTING_HELP_STUDY);		

		//选择目标(等级提到10级)			
		OnTipHelp(USER_SETTING_HELP_SELECT,Level > 9 );		
		
		tLevelUpTipMap::iterator it = g_mapLevelup.find(Level);
		if( it != g_mapLevelup.end() )
		{
			char szTip[MAX_PATH];
			_snprintf(szTip,MAX_PATH,"你可以装备更高级的 %s 了",it->second.c_str() );
			GetGameGuiManager()->AddClientSystemMessage(szTip);
		}
	}
	stPointI pt;
	return CastMagic(GetPos(),0,true,0,pt) == eCastMagicResult_Succeeded; 

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param ptMapPixelOffset : 描述
* \return 返回值的描述
*/
void CNpc::DrawText(POINT ptMapPixelOffset)
{
	FUNCTION_BEGIN;

	if( !IsMainRole() && GetMaxHP() && GetHP() )
	{
		DrawHP(ptMapPixelOffset);
	}

	if( IsCharacter() && bIsShopStart() )
	{
		DrawShopName(ptMapPixelOffset);
	}
	else	//摆摊就不显示名字了！
	{
		if( (IsMainRole() || IsCharacter()) && 
			GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_PLAYERNAME ) )
		{
			DrawName(ptMapPixelOffset);
		}
		else 
		{
			if( (GetScene()->m_pMouseOverObject == this)
			|| ( (GetKeyState('Z') < 0) && (GetKeyState('X') < 0)) )
			{		
				DWORD color = -1;
				//宠物名字颜色另外显示
				if( m_pPetData )
				{
					color = CPet::GetNameTextColor((petType)m_pPetData->pet_type,GetLevel());
				}
				else  //桃子 BOSS名字颜色修改
				{
					if(GetNpcTypeItem() == NpcTypeItem_Enlisted) //8类型 禁卫绿色
					{
						color = COLOR_ARGB(255,0,245,10);
					}
					else if(GetNpcTypeItem() == NpcTypeItem_Boss) //2类型 绿色BOSS
					{
						color = COLOR_ARGB(255,0,245,10);
					}
					else if(GetNpcTypeItem() == NpcTypeItem_SmBoss) //3类型 精英BOSS
					{
						color = COLOR_ARGB(255,0,245,10);
					}
					else if(GetNpcTypeItem() == NpcTypeItem_Trade) 	//soke 增加6类型NPC鼠标移动名字是米黄色
					{
						color = COLOR_ARGB(255,255,250,150);
					}
					else if(GetNpcTypeItem() == NpcTypeItem_Task)  //soke 增加7类型NPC鼠标移动名字是米黄色
					{
						color = COLOR_ARGB(255,255,250,150);
					}
					else if(GetNpcTypeItem() == NpcTypeItem_Extract) // 4类型 橙色类型
					{
						color = COLOR_ARGB(255,255,140,0);
					}
					else if(GetNpcTypeItem() == NpcTypeItem_Gold)   // 5类型 红色怪物
					{
						color = COLOR_ARGB(255,255,0,0);
					}
					else if ( GetNpcTypeItem() == NpcTypeItem_ExGold ) // 31类型 紫色怪物
					{
						color = COLOR_ARGB(255,110,3,178);
					}
					else if(GetNpcTypeItem() == NpcTypeItem_RedBoss)   // 33类型 红色怪物
					{
						color = COLOR_ARGB(255,255,0,0);
					}
					else if(GetNpcTypeItem() == NpcTypeItem_MobileTrade) //19类型 绿色NPC
					{
						color = COLOR_ARGB(255,0,255,0);
					}	
					else if(GetNpcTypeItem() == NpcTypeItem_MailMan) //24类型 绿色NPC
					{
						color = COLOR_ARGB(255,110,3,178);
					}
					else if(GetNpcTypeItem() == NpcTypeItem_SavingBox) //15类型 绿色NPC
					{
						color = COLOR_ARGB(255,255,140,0);
					}	
					else if(GetNpcTypeItem() == NpcTypeItem_LiveNpc) //20类型 绿色NPC
					{
						color = COLOR_ARGB(255,255,0,0);
					}	
					else if(GetNpcTypeItem() == NpcTypeItem_Flower) //21类型 绿色采集类
					{
						color = COLOR_ARGB(255,0,255,0);
					}																								
				}
				DrawName(ptMapPixelOffset,color);
			}
			else
			{
				DWORD color = -1;
				//soke 增加6类型NPC显示名字是米黄色
				if(GetNpcTypeItem() == NpcTypeItem_Trade)
				{
					color = COLOR_ARGB(255,255,250,150);
					DrawName(ptMapPixelOffset,color);
				}
				//soke 增加7类型NPC显示名字是米黄色
				if(GetNpcTypeItem() == NpcTypeItem_Task)
				{
					color = COLOR_ARGB(255,255,250,150);
					DrawName(ptMapPixelOffset,color);
				}
				//soke 增加15类型NPC显示名字是橙色
				if(GetNpcTypeItem() == NpcTypeItem_SavingBox)
				{
					color = COLOR_ARGB(255,255,140,0);
					DrawName(ptMapPixelOffset,color);
				}	
				//soke 增加20类型NPC显示名字是红色
				if(GetNpcTypeItem() == NpcTypeItem_LiveNpc)
				{
					color = COLOR_ARGB(255,255,0,0);
					DrawName(ptMapPixelOffset,color);
				}							
				//soke 增加19类型NPC显示名字是绿色
				if(GetNpcTypeItem() == NpcTypeItem_MobileTrade)
				{
					color = COLOR_ARGB(255,0,255,0);
					DrawName(ptMapPixelOffset,color);
				}	
				//soke 增加24类型NPC显示名字是紫色
				if(GetNpcTypeItem() == NpcTypeItem_MailMan)
				{
					color = COLOR_ARGB(255,110,3,178);
					DrawName(ptMapPixelOffset,color);
				}	
			    if(GetNpcTypeItem() == NpcTypeItem_Flower) //21类型 绿色采集类
				{
					color = COLOR_ARGB(255,0,255,0);
					DrawName(ptMapPixelOffset,color);
				}											
			}
		}
	}

	DrawHurt(ptMapPixelOffset);	

	DrawEmotionIcon(ptMapPixelOffset);

	DrawCountryWarFlag(ptMapPixelOffset);


	if( !IsDie() ) 
	{
		DrawNineChat(ptMapPixelOffset);
	}

	FUNCTION_END;
}

void CNpc::DrawEmotionIcon(stPointI ptMapOffset)
{
	FUNCTION_BEGIN;
	FUNCTION_END;
}

void CNpc::DrawCountryWarFlag(stPointI ptMapOffset)
{
	FUNCTION_BEGIN;
	FUNCTION_END;
}
/**
* \brief 简短描述
* 
* 详细描述
* 
* \param ptMapPixelOffset : 描述
* \return 返回值的描述
*/
void CNpc::DrawHP(POINT ptMapPixelOffset)
{
	FUNCTION_BEGIN;

	//stPointI pt0 = GetPos() - ptMapPixelOffset;	
	//m_iDrawTop += 15;
	//pt0.y -= (m_iDrawTop);
	//float xScale = (float )GetHP() / (float )GetMaxHP();
	//GetNpcBlood()->Render(pt0,c_szFiveName[GetMainFive()],xScale);//,stPointF(0.8f,1.0f)

	FUNCTION_END;
}

void CNpc::DrawEffect(POINT ptMapPixelOffset)
{
	// Show effect
	std::map<std::string, CEffect*>::iterator iter;
	for (iter = m_mapEffects.begin(); iter != m_mapEffects.end(); ++iter)
	{
		if (iter->second)
		{
			stPointI ptChar = this->GetPos();
			ptChar -= ptMapPixelOffset;
			if (this->IsRide())
			{
			//	ptChar.y -= 50; // 骑马状态调整特效的高度
				ptChar.y -= 0; // 骑马状态调整特效的高度
			}
			else if (this->IsSitDown())
			{
			//	ptChar.y += 50; // 打坐状态调整特效的高度
				ptChar.y -= 0; // 骑马状态调整特效的高度
			}
			iter->second->Render(ptChar);
		}
	}
}

inline bool CNpc::bIsShowLevel()
{
	enumNpcTypeItem items[] = {
		NpcTypeItem_Guide,  // 16
		NpcTypeItem_Box,    // 17 
		NpcTypeItem_SavingBox, // 15 
		NpcTypeItem_LiveNpc,   // 20 
		NpcTypeItem_Flower,    // 21
		NpcTypeItem_Resource,	//32
		// some other functionnpc do not also show  
		NpcTypeItem_Trade,     // 6 
		NpcTypeItem_Task,      // 7
		NpcTypeItem_Enlisted,  // 8  
		NpcTypeItem_MobileTrade, //19
		NpcTypeItem_FlagPole,     // 22
		NpcTypeItem_CatchItem,
		NpcTypeItem_MailMan,
		NpcTypeItem_Auction
	};
		for(size_t i = 0 ;i < count_of(items); i++)
		{
			if(GetNpcTypeItem() == items[i]) return false;
		}
		return true;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param ptMapPixelOffset : 描述
* \param dwColor : 描述
* \return 返回值的描述
*/
void CNpc::DrawName(POINT ptMapPixelOffset,DWORD dwColor)
{
	FUNCTION_BEGIN;

	if(GetName() && !IsSurfaceType() )
	{
		stPointI pt0 = GetPos() - ptMapPixelOffset;
		int height = GetImageHeight();

		int iFontHeight = GetDevice()->GetFontHeight();
		m_iDrawTop += iFontHeight; 
		stRectI rcText(pt0.x - 500,pt0.y - m_iDrawTop,pt0.x + 500,0xffff);

		char szShow[256] ="";

		if(IsNpc())
		{
			if(m_pCharacter->IsDie())
			{
				//花花草草的不显示尸体二字
				if( (GetNpcTypeItem() != NpcTypeItem_Flower) 
					&& (GetNpcTypeItem() !=NpcTypeItem_Resource) )
				{
					sprintf(szShow,"%s的尸体",GetName());
				}				
			}
			else
			{
				if(m_pPetData)
				{
					if ( GetNpcTypeItem() == NpcTypeItem_CatchItem )
						sprintf( szShow, "%s", GetName() );
					else
					{
						sprintf(szShow,"(%s)",m_pPetData->masterName);
						GetDevice()->DrawString(szShow,rcText,dwColor,DT_TOP | DT_CENTER,FontEffect_Border);
						m_iDrawTop += iFontHeight;
						stRectI rcText(pt0.x - 500,pt0.y - m_iDrawTop,pt0.x + 500,0xffff);
						sprintf(szShow,"%s(%u级)",GetName(),GetLevel());
						
						GetDevice()->DrawString(szShow,rcText,dwColor,DT_TOP | DT_CENTER,FontEffect_Border);
						return ;
					}					
				}
				else
				{
#ifdef _DEBUG
					sprintf(szShow,"%s(%u级) (ID=%u)(%u,%u)",GetName(),GetLevel(),GetID(),GetGridPos().x, GetGridPos().y);
#else
					if( bIsShowLevel())
						sprintf( szShow,"%s(%u级)",GetName(),GetLevel() );
					else
						sprintf( szShow,"%s",GetName() );
#endif
				}
			}
		}
		else
		{
#if defined _DEBUG || defined _AUTOPLAY
			if ( IsChangeFace() )
			{
				NpcBase_t* pNpcBase = GetNpcBase( m_proChangingFace.m_dwChangingFaceID );
				if ( pNpcBase )
					sprintf( szShow, pNpcBase->strName );
				else
					sprintf( szShow, "" );
			}
			else
				sprintf(szShow,"%s(%s) (%u)(%u,%u)",GetName(),GetCountryName(),GetID(),GetGridPos().x,GetGridPos().y);
#else
			if ( IsChangeFace() )
			{
				NpcBase_t* pNpcBase = GetNpcBase( m_proChangingFace.m_dwChangingFaceID );
				if ( pNpcBase )
					sprintf( szShow, pNpcBase->strName );
				else
					sprintf( szShow, "" );
			}
			else
				sprintf(szShow,"%s(%s)",GetName(),GetCountryName());//
#endif
		}

		GetDevice()->DrawString(szShow,rcText,dwColor,DT_TOP | DT_CENTER,FontEffect_Border);
	}

	FUNCTION_END;
}

void CNpc::DrawShopName(POINT ptMapPixelOffset,DWORD dwColor)
{
	FUNCTION_BEGIN;

	stPointI pt0 = GetPos() - ptMapPixelOffset;

	stPointI pt( pt0.x,pt0.y - m_iDrawTop );
	m_ShopNameTip.Render(pt);
	stRectI rc = GetShopTextRect();
	m_iDrawTop += rc.Height();
	return ;	 

	FUNCTION_END;
}

void CNpc::DrawNineChat(POINT ptMapPixelOffset)
{
	FUNCTION_BEGIN;

	if( !m_NineChat.empty() )
	{
		stPointI pt = GetPos() - ptMapPixelOffset;

		m_NineChat.Render(pt - stPointI(0,m_iDrawTop));

		m_iDrawTop += m_NineChat.GetTipHeight();
	}	

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param ptMapPixelOffset : 描述
* \param dwColor : 描述
* \return 返回值的描述
*/
void CNpc::Draw(POINT ptMapPixelOffset,DWORD dwColor)
{
	FUNCTION_BEGIN;

	//When main character is hide state
	//We draw it half transparent
	if ( IsMainRole() && (isset_state(GetState(), USTATE_HIDE) || isset_state(GetState(),USTATE_HIDE_NOMOVE_STATE)) )
		dwColor &= 0x77ffffff;

	//For pet hide
	if ( isset_state(GetState(), USTATE_HIDE_PET_TRAINING) && IsPet() )
	{
		if ( IsMyPet() )
			dwColor &= 0x77ffffff;
		else		//Don't draw other player's pet which is in hidden state
			return;
	}

	m_iDrawTop = 0;

	if ( 0 )			//isset_state( GetState(), USTATE_CHANGEFACE )
	{
		if( m_pChangeFace && m_pChangeFace->IsCreate())
		{
			if(GetScene()->m_pMouseOverObject == this)
			{
				iCharacter::s_bHightlight = true;
			}

			if(IsDie())
			{
			}
			else
			{
				m_pChangeFace->ChangeAlpha(COLOR_GET_ALPHA(dwColor));
			}

			m_pChangeFace->Draw(ptMapPixelOffset);
			iCharacter::s_bHightlight = false;
			m_iDrawTop = GetImageHeight();
		}
	}
	else
	{
		if(m_pCharacter->IsCreate())
		{
			if(GetScene()->m_pMouseOverObject == this)
			{
				iCharacter::s_bHightlight = true;
			}

			if(IsDie())
			{
				if(m_bAshesFadeOut)
				{
					int alpha = m_nAshesFadeOutTime * 255 / 1000; 
					m_pCharacter->ChangeAlpha(alpha);
				}
			}
			else
			{
			#ifdef _DEBUG
				BYTE alpha = m_pCharacter->GetAlpha();
				if ( alpha != COLOR_GET_ALPHA( dwColor ) )
			#endif				
					m_pCharacter->ChangeAlpha(COLOR_GET_ALPHA(dwColor));
			}

			//if(IsNpc() || IsMainRole())
			m_pCharacter->Draw(ptMapPixelOffset);
			iCharacter::s_bHightlight = false;
			m_iDrawTop = GetImageHeight();
		}

	}

	this->DrawEffect(ptMapPixelOffset);

	FUNCTION_END;
}


void CNpc::Draw2(POINT ptMapPixelOffset,DWORD dwColor)
{
	FUNCTION_BEGIN;

	//When main character is hide state
	//We draw it half transparent
	if ( IsMainRole() && (isset_state(GetState(), USTATE_HIDE) || isset_state(GetState(),USTATE_HIDE_NOMOVE_STATE)) )
		dwColor &= 0x77ffffff;

	//For pet hide
	if ( isset_state(GetState(), USTATE_HIDE_PET_TRAINING) && IsPet() )
	{
		if ( IsMyPet() )
			dwColor &= 0x77ffffff;
		else		//Don't draw other player's pet which is in hidden state
			return;
	}

	m_iDrawTop = 0;

	if ( 0 )			//isset_state( GetState(), USTATE_CHANGEFACE )
	{
		if( m_pChangeFace && m_pChangeFace->IsCreate())
		{
			if(GetScene()->m_pMouseOverObject == this)
			{
				iCharacter::s_bHightlight = true;
			}

			if(IsDie())
			{
			}
			else
			{
				m_pChangeFace->ChangeAlpha(COLOR_GET_ALPHA(dwColor));
			}

			m_pChangeFace->Draw2(ptMapPixelOffset);
			iCharacter::s_bHightlight = false;
			m_iDrawTop = GetImageHeight();
		}
	}
	else
	{
		if(m_pCharacter->IsCreate())
		{
			if(GetScene()->m_pMouseOverObject == this)
			{
				iCharacter::s_bHightlight = true;
			}

			if(IsDie())
			{
				if(m_bAshesFadeOut)
				{
					int alpha = m_nAshesFadeOutTime * 255 / 1000; 
					m_pCharacter->ChangeAlpha(alpha);
				}
			}
			else
			{
			#ifdef _DEBUG
				BYTE alpha = m_pCharacter->GetAlpha();
				if ( alpha != COLOR_GET_ALPHA( dwColor ) )
			#endif				
					m_pCharacter->ChangeAlpha(COLOR_GET_ALPHA(dwColor));
			}

			//if(IsNpc() || IsMainRole())
			m_pCharacter->Draw2(ptMapPixelOffset);
			iCharacter::s_bHightlight = false;
			m_iDrawTop = GetImageHeight();
		}

	}

	this->DrawEffect(ptMapPixelOffset);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CNpc::IsDie()
{
	FUNCTION_BEGIN;

	return m_bDied;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CNpc::IsCart()
{
	if ( (GetTypeID() == Npc_CartID1)
		|| (GetTypeID() == Npc_CartID2) )
		return true;

	return false;
}

bool CNpc::IsMyCart()
{
	if ( !IsCart() )
		return false;

	if ( GetMasterID() != GetScene()->GetMainCharacter()->GetID() )
		return false;

	return true;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CNpc::OnLoopDie()
{
	FUNCTION_BEGIN;

	//if (m_pCharacter->IsDie())
	//{
	//	if (!IsMainRole())
	//	{
	//		// m_bDeleted = true;

	//		RemoveDynamicObstacle();
	//		TRACE("NPC 死亡 ID [%d] \n",m_dwID);
	//	}
	//	return true;
	//}
	return false;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param time_elasped : 描述
* \return 返回值的描述
*/
void CNpc::Run(long time_elasped)
{
	FUNCTION_BEGIN;

	//OnLoopDie();

	if( IsDie() )
	{
		// NPC已经死亡
		if( m_pCharacter->IsDie() )
		{
			// 死亡动画做完了
			m_bDieEnd = true;
			if(m_bAshesFadeOut)
			{
				// 服务器发了删除指令，尸体要淡出了
				m_nAshesFadeOutTime -= time_elasped;
				if(m_nAshesFadeOutTime <= 0)
				{
					m_nAshesFadeOutTime = 0;
					m_bDeleted = true;
					return;
				}
			}
		}
		else
		{
			// NPC死亡,等待攻击他的魔法结束
			m_nDieWaitingTime -= time_elasped;
			if ( m_nDieWaitingTime < 0 && m_pCharacter->GetAction() != Ani_Die )
			{
				Die();
			}
		}
	}

	//必须处理，否则重生和跳转有问题
	while(!m_queueHPCmd.empty())
	{
		stNullUserCmd* pCmd = (stNullUserCmd*)m_queueHPCmd.front();
		if( pCmd->byCmd != MAGIC_USERCMD || pCmd->byParam != RTMAGIC_USERCMD_PARA )
		{
			if(HandleCommand(pCmd))
			{
				delete pCmd;	
				m_queueHPCmd.pop_front();
			}
			else break;
		}
		else
		{
			stRTMagicUserCmd* pRTMagicCmd = (stRTMagicUserCmd*)m_queueHPCmd.front();
			if (xtimeGetTime() - pRTMagicCmd->dwTimestamp > 1200)
			{				
				HandleHurtCommand(*pRTMagicCmd);
				delete pCmd;
				m_queueHPCmd.pop_front();
			}
			else break;
		}
	}

	if(m_pCharacter->IsCreate())
	{
		//Refresh ride state
		RunRide();

		if (!IsMainRole() && m_queueCmd.GetSize() >= 2 && m_pCharacter->GetAction() != Ani_Die )
		{
			int realSize = m_queueCmd.GetSize();
			if ( HaveCommand( MAGIC_USERCMD, NPCDEATH_USERCMD_PARA )
				&& HaveCommand( MOVE_USERCMD, NPCMOVE_MOVE_USERCMD_PARA ) )
			{
				realSize = 1;
			}

			time_elasped *= realSize;
		}

		m_pCharacter->Run(time_elasped);

		/*if ( IsMainRole() )
		{
			int iii = 0;
			if ( HaveCommand( MOVE_USERCMD, USERMOVE_MOVE_USERCMD_PARA ) )
				int iiiiiii = 0;
		}*/
		UpdatePosition();
		SetFrameAllocator fa;
		tagListMagicGenerate listMagic;
		m_pCharacter->Generate(&listMagic);
		for(tagListMagicGenerate::iterator it = listMagic.begin();it != listMagic.end(); ++it)
		{
			GetScene()->AddMagic(*it);
			(*it)->Release();
		}
	}
	while(HandleCommand())
		;

	
	//Refresh sit-down state
	RunSitDown();

	FUNCTION_END;
}

void CNpc::ClearCmdQueue()
{
	if ( !m_queueCmd.Empty() )
		m_queueCmd.Clear();

	m_nRecieveWalk = m_nSendWalk;
	m_nRecieveAttack = m_nSendAttack;
}

bool CNpc::CombinationWithPet( DWORD dwID )
{
	if ( IsRide() )
		OnRide( false );

	//Back old dir
	int nDir = m_pCharacter->GetDir();
	
	if ( dwID != -1 )
	{
		//Get change face to npc image
		NpcBase_t* pNpcBase = GetNpcBase( dwID );
		if ( pNpcBase )
		{
			int iImageID = pNpcBase->dwPic;
			static std::vector<int> aID;
			aID.resize(1);
			aID[0] = iImageID;
			m_pCharacter->CreateCharacter( -1, aID, true );
			m_dwCombinationID = dwID;

			//Limited to walk state
			if ( IsMainRole() && GetGameGuiManager()->m_guiMain )
				GetGameGuiManager()->m_guiMain->SwitchWalk();

			m_pCharacter->Action( Ani_Stand );
			m_pCharacter->TurnTo( nDir );
			m_pCharacter->MoveTo( stPointI(GetPos().x+1, GetPos().y+1 ), Ani_Stand, true );

			if ( IsCharacter() )
			{
				CCharacter* pChar = static_cast<CCharacter*>(this);
				if ( pChar )
				{
					pChar->GetInterface()->SetMoveSpeed( (DWORD)pChar->GetAniSpeed()/2 );
				}
			}
		}
	}
	else
	{
		m_dwCombinationID = -1;

		ResetImage();		

		m_pCharacter->Action( Ani_Stand );
		m_pCharacter->TurnTo( nDir );
		m_pCharacter->MoveTo( stPointI(GetPos().x+1, GetPos().y+1 ), Ani_Stand, true );
	}

	//When cur npc is selected we change icon in character property dlg
	if ( !IsMainRole() && GetScene()->m_pInterestNpc && (GetScene()->m_pInterestNpc == this) )
	{
		if ( GetGameGuiManager()->m_guiCharacterProperty )
		{
			GetGameGuiManager()->m_guiCharacterProperty->InitIcon();
		}
	}

	return true;
}

bool CNpc::ChangeFace( DWORD dwID )
{
	if ( IsRide() )
		OnRide( false );
	
	//Back old dir
	int nDir = m_pCharacter->GetDir();

	if ( dwID != -1 )
	{
		//Change face
		if ( m_pChangeFace )
			m_pChangeFace->Release();

		NpcBase_t* pNpcBase = GetNpcBase( dwID );
		//NpcBase_t* pNpcBase = GetNpcBase( 16040 );
		if ( pNpcBase )
		{
			/*m_pChangeFace = Character_CreateCharacter();
			m_pChangeFace->SetData( this );*/

			int iImageID = pNpcBase->dwPic;
			static std::vector<int> aID;
			aID.resize(1);
			aID[0] = iImageID;
			//m_pChangeFace->CreateCharacter( -1, aID, true );
			m_pCharacter->CreateCharacter( -1, aID, true );
			m_proChangingFace.m_dwChangingFaceID = dwID;

			//Get ChangingFaceLevel
			{
				CNpc* pNpc = GetScene()->FindNearbySummon( dwID );
				if ( pNpc )
				{
					m_proChangingFace.m_dwChangingFaceLevel = pNpc->GetLevel();
					m_proChangingFace.m_nChangingFaceFiveType = pNpc->GetMainFive();
				}
				else
				{
					m_proChangingFace.m_dwChangingFaceLevel = 10;
					m_proChangingFace.m_nChangingFaceFiveType = FIVE_NONE;
				}
			}

			//Limited to walk state
			if ( IsMainRole() && GetGameGuiManager()->m_guiMain && !((dwID == 44031) || (dwID == 44032)) )
				GetGameGuiManager()->m_guiMain->SwitchWalk();

			m_pCharacter->Action( Ani_Stand );
			m_pCharacter->TurnTo( nDir );
			m_pCharacter->MoveTo( stPointI(GetPos().x+1, GetPos().y+1 ), Ani_Stand, true );

			//m_pChangeFace->SetPos( GetPos() );
			/*if (IsMoving())
			m_pChangeFace->MoveTo( GetPos(), GetAction() );
			else
			m_pChangeFace->Action( GetAction() );*/
		}
	}
	else
	{
		//Restore
		if ( m_pChangeFace )
		{
			m_pChangeFace->Release();
			m_pChangeFace = NULL;
		}
		m_proChangingFace.Init();

		ResetImage();
		
		m_pCharacter->Action( Ani_Stand );
		m_pCharacter->TurnTo( nDir );
		m_pCharacter->MoveTo( stPointI(GetPos().x+1, GetPos().y+1 ), Ani_Stand, true );
	}

	if ( !IsMainRole() && GetScene()->m_pInterestNpc && (GetScene()->m_pInterestNpc == this) )
	{
		if ( GetGameGuiManager()->m_guiCharacterProperty )
		{
			GetGameGuiManager()->m_guiCharacterProperty->InitIcon();
		}
	}

	return true;
}

bool CNpc::IsSitDown()
{
	//return isset_state( GetState(), USTATE_SITDOWN );
	return m_bIsSitting;
}

void CNpc::OnSit( bool sitDown )
{
	FUNCTION_BEGIN;

	if ( sitDown )
	{
		if ( IsMainRole() )
			((CMainCharacter*)this)->UnPauseMove();
	}

	if ( sitDown )
	{
		if ( m_pCharacter->Action( Ani_Sit, false ) )
			m_bIsSitting = true;	
	}
	else
	{
		if ( m_pCharacter->Action( Ani_Wait, false ) )
			m_bIsSitting = false;
	}

	FUNCTION_END;
}

bool CNpc::IsRideFightHorse()
{
	if ( IsRide() )
	{
		ObjectBase_t* pItemRef = GetObjectBase( GetRideID() );
		if ( pItemRef )
		{
			if ( (pItemRef->dwType == ItemType_FightHorse) )
				return true;
			else
				return false;
		}
	}

	return true;
}

/**
* \brief 简短描述
* 
* 详细描述
* 设置 骑马动画
* \param nSwitch : 描述
* \return 返回值的描述
*/
bool CNpc::OnRide(int nSwitch)
{
	FUNCTION_BEGIN;

	if (nSwitch)
	{
		NpcBase_t* pNpcBase = GetNpcBase(GetRideID());
		if(pNpcBase)
		{
			if(IsMainRole())
			{
				GetScene()->GetMainCharacter()->ClearPath();
				GetScene()->GetMainCharacter()->SetClientRideState(true);
			}
			if (!m_pCharacter->SetRide(pNpcBase->dwPic))
				return false;
		}
		set_state(GetState(),USTATE_RIDE);
	}
	else
	{
		{
			clear_state(GetState(),USTATE_RIDE);

			if(IsMainRole())
			{
				GetScene()->GetMainCharacter()->ClearPath();
				GetScene()->GetMainCharacter()->SetClientRideState(false);
			}

			if (!m_pCharacter->SetRide(0))
				return false;

			//m_pCharacter->Action(Ani_Wait, 1);

		}		
	}
	return true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \return 返回值的描述
*/
void CNpc::Relive(stMainUserReliveReliveUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	m_bDied = false;
	m_bDieWaitingMagic = false;
	m_nDieWaitingTime = 0;

	m_pCharacter->ResetSpeed();

	m_pCharacter->Init(false);
	
	if(GetGameGuiManager()->m_guiReliveDlg && 
		GetGameGuiManager()->m_guiReliveDlg->IsVisible())
	{
		GetGameGuiManager()->m_guiReliveDlg->Close();
	}

	if ( IsMainRole() )
	{
		if ( GetScene()->GetMainCharacter() )
			GetScene()->GetMainCharacter()->ClearRunEvent();
	}

	stRTMagicPosUserCmd  cmd;
	cmd.pos.byDir = -1;
	cmd.pos.dwTempID = GetID();
	cmd.pos.x = pCmd->x;
	cmd.pos.y = pCmd->y;
	cmd.byTarget = (IsCharacter() ? MAPDATATYPE_USER : MAPDATATYPE_NPC);

	PushCommand(&cmd,sizeof(cmd));

	FUNCTION_END;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CNpc::InScreen()
{
	FUNCTION_BEGIN;

	stRectI rc;
	if(!m_pCharacter->GetDrawRect(m_ptPixelOffset,rc)) return false;
	rc &= GetScene()->GetDrawPixelRect();
	return !rc.IsRectEmpty();

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param nDir : 描述
* \param bServerLogic : 描述
* \return 返回值的描述
*/
bool CNpc::TurnTo(int nDir,bool bServerLogic)
{
	FUNCTION_BEGIN;

	if ( GetTypeID() == 10052 )
	{
		int temp = 10;
	}
	return m_pCharacter->TurnTo(nDir,bServerLogic);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param dwCmdType : 描述
* \param dwParameterType : 描述
* \return 返回值的描述
*/
bool CNpc::HaveCommand(DWORD dwCmdType,DWORD dwParameterType)
{
	FUNCTION_BEGIN;

	for (CStreamQueue::iterator it  = m_queueCmd.begin(); it != m_queueCmd.end(); ++it)
	{
		stNullUserCmd* p = (stNullUserCmd* )(*it).buffer;
		if (p->byCmdType == dwCmdType && p->byParameterType == dwParameterType)
			return true;
	}
	return false;

	FUNCTION_END;
}

int CNpc::GetCommandNum( DWORD dwCmdType, DWORD dwParameterType )
{
	FUNCTION_BEGIN;

	int num=0;
	for( CStreamQueue::iterator it = m_queueCmd.begin(); it!= m_queueCmd.end(); ++it )
	{
		stNullUserCmd* p = (stNullUserCmd* )(*it).buffer;
		if ( (p->byCmdType == dwCmdType) && (p->byParameterType == dwParameterType) )
			num++ ;
	}

	return num;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pt : 描述
* \param bJustInRect : 描述
* \return 返回值的描述
*/
bool CNpc::IsAtPixelPos(POINT pt, bool bJustInRect)
{
	FUNCTION_BEGIN;

	return m_pCharacter->IsAt(pt,bJustInRect);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CNpc::OnSetCursor()
{
	FUNCTION_BEGIN;

	if ( ( GetGameCursor()->GetCursorImageType() == eCursorUseAccess )
		|| ( GetGameCursor()->GetCursorImageType() == eCursorTrade )
		|| ( GetGameCursor()->GetCursorImageType() == eCursorShowUser )
		|| ( GetGameCursor()->GetCursorImageType() == eCursorFollowUser )
		|| ( GetGameCursor()->GetCursorImageType() == eCursorSetAccess )
		|| ( GetGameCursor()->GetCursorImageType() == eCursorPetAttack )
		|| ( GetGameCursor()->GetCursorTypeEx() == eCursorGetMailItem )
		|| ( GetGameCursor()->IsDrag() && (GetGameCursor()->GetCursorImageType() != eCursorFreeMove) )
		|| ( GetGuiManager()->GetFloatItem() ) )
		return false;

	if( GetNpcTypeItem() == NpcTypeItem_Cartoon )
	{
		return false;
	}

	if ( IsFireWall() )
	{
		return false;
	}

	if(IsCanAttack())
	{
		if(IsDie()) return false;
		CPet* pPet = GetScene()->FindPetByID( GetID() );
		if(pPet)
		{
			if(pPet->GetPetType() != PET_TYPE_GUARDNPC ||
				GetScene()->GetMainCharacter()->GetPKMode() != PKMODE_ENTIRE)
				return false;
		}

		if ( GetNpcTypeItem() == NpcTypeItem_Grass )
			GetGameCursor()->SetCursor(CCursor::CursorType_Normal, eCursorAttack1);
		else
			GetGameCursor()->SetCursor(CCursor::CursorType_Normal,eCursorAttack);
	}
	else if(GetNpcTypeItem() != NpcTypeItem_Guide)
		GetGameCursor()->SetCursor(CCursor::CursorType_Normal,eCursorSpeak);
	else
		GetGameCursor()->SetCursor(CCursor::CursorType_Normal,eCursorNormal);
	return true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
DWORD CNpc::GetTypeID()
{
	FUNCTION_BEGIN;

	if(m_pNpcBase)
		return m_pNpcBase->dwID;
	return 0;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
//-------------------------------------------------------------------------------
//	显示
//-------------------------------------------------------------------------------
DWORD CNpc::GetHurtColor()
{
	FUNCTION_BEGIN;

	return 0xffffff00;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
int	CNpc::GetImageHeight()
{
	FUNCTION_BEGIN;

	return m_pCharacter->GetImageHeight();

	FUNCTION_END;
}

/**
* \brief 显示伤害值
* 
* 显示伤害值、获得经验值、躲避或失误
* 
* \param ptMapPixelOffset : 描述
* \return void
*/
void CNpc::DrawHurt(POINT ptMapPixelOffset)
{
	FUNCTION_BEGIN;

	if (m_listNumColor.empty())
		return ;

	stPointI ptText = GetPos() - ptMapPixelOffset;
	const int c_nHigh = 140;

	POINT pt = ptText - stPointI(30,0);

	stPointF CommScale(0.65f,0.65f);
	int imageHeight = GetImageHeight();

	for (tListNumColor::iterator it = m_listNumColor.begin(); it != m_listNumColor.end(); ++it)
	{		
		stNumColor& t = (*it);	
		
		// 每个伤害数字都从固定的基础位置开始，然后根据自己的height向上移动
		pt.y = ptText.y - 40 - imageHeight;
		// 每个伤害数字独立地根据自己的height向上移动
		pt.y -= t.height;
		
		// 根据伤害类型设置不同的X坐标偏移，避免重叠
		pt.x = ptText.x - 30; // 默认位置
		if (strstr(t.szNum, "切割") != NULL) {
			// 切割伤害显示在左侧
			// pt.y = ptText.y + 8;
			pt.y += 32;
		} else if (strstr(t.szNum, "绝技") != NULL) {
			// 绝技伤害显示在右侧
			// pt.y = ptText.y + 20;
			pt.y += 32;
		}
		
		if(t.height > c_nHigh) continue;
		
		// 计算透明度，随着高度增加而减少
		int alpha = (c_nHigh-t.height)*255 / c_nHigh;
		t.color &= 0x00ffffff;
		t.color |= alpha << 24;
		
		// 缓慢上移效果
		t.height += 0.8f;

		//如果是经验值
		if(t.byIsExp > 0) 
		{
			GetBmpTextout()->DrawString(pt,t.szNum,strlen(t.szNum),t.color,&(t.scale));
			continue;
		}

		//如果是幸运攻击(特殊效果显示)
		if(t.byLuck != 0)
		{
			if(t.height < 10)
			{			
				t.scale += stPointF(0.05f,0.05f);
			}
			else if(t.height >= 10 && t.height < 40)
			{
				t.scale = stPointF(1.0,1.0);
			}
			else if(t.height > 99 || t.scale.x < 0.5f)
			{
				t.scale = stPointF(0.5f,0.5f);
			}
			else
				t.scale -= stPointF(0.008f,0.008f);
			GetBmpTextout()->DrawString(pt,t.szNum,strlen(t.szNum),t.color,&(t.scale));
		}
		else
		{			
			GetBmpTextout()->DrawString(pt,t.szNum,strlen(t.szNum),t.color,&CommScale);
		}
	}

	tListNumColor::iterator it1;

	// 移除已经完全淡出的伤害数字
	for(it1 = m_listNumColor.begin() ; it1 != m_listNumColor.end() && (*it1).height >= c_nHigh ; ++it1)
	{}

	if( it1 != m_listNumColor.begin())
		m_listNumColor.erase(m_listNumColor.begin(),it1);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \return 返回值的描述
*/
bool CNpc::IsValidHurt(stRTMagicUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	switch(pCmd->byRetcode) 
	{
	case RTMAGIC_FAILURE:	// 攻击失败
		return false;
	case RTMAGIC_SUCCESS:	// 攻击成功
	case RTMAGIC_VAMPIRE:	// 吸血
	case RTMAGIC_ATTACKED:	// 被击
		return pCmd->sdwHP != 0;
	case RTMAGIC_VAMPIREMAGIC:	// 吸魔
		return true;
	default:
		return false;
	}

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \return 返回值的描述
*/
// 受伤,播放受伤动画
void CNpc::OnHurt(stRTMagicUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	bool bRandomUse = ((float)rand() / RAND_MAX > 0.25f) ? false : true;

	bool bCanHurt = m_pCharacter->CanChangeToAction(Ani_Hurt);
	bool bCurHurt = (m_pCharacter->GetAction() == Ani_Hurt);

	OnHurtAddHint(pCmd);
	if(IsValidHurt(pCmd) && bCanHurt && !bCurHurt && bRandomUse)
	{
		m_pCharacter->Hurt();	
	}

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param idType : 描述
* \param id : 描述
* \return 返回值的描述
*/
bool IsMainRole(DWORD idType,DWORD id)
{
	FUNCTION_BEGIN;

	return idType == MAPDATATYPE_USER && id == GetScene()->GetMainCharacter()->GetID();

	FUNCTION_END;
}

bool IsMyPet(DWORD id)
{
	return ( NULL != GetScene()->FindPetByID( id ));
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CNpc::IsMyPet()
{
	//在宠物列表里面寻找，如果该id存在，则说明是自己的宠物
	return ::IsMyPet( GetID() );
}

bool CNpc::IsMyCartoonPet()	//替身宝贝
{
	if( m_pPetData && 
		m_pPetData->pet_type == PET_TYPE_CARTOON &&
		m_pPetData->masterID == GetScene()->GetMainCharacter()->GetID() )
	{
		return true;
	}
	return false;
	
	//return ( NULL != GetScene()->GetCartoonPetList()->GetCartoonByNpcID(GetID()) );
}

bool CNpc::IsPet()//masterID == -1 代表没有主人
{
	if(IsCharacter()) return false;
	return (m_pPetData && m_pPetData->masterID != -1);
	//return m_data.masterID != -1;
}

// return -1 : no master
DWORD CNpc::GetMasterID()
{
	if(!IsPet())
	      return -1;
	return m_pPetData->masterID;
}

petType CNpc::GetPetType()
{
	if( m_pPetData )
	{
		return (petType)m_pPetData->pet_type;
	}
	return PET_TYPE_NOTPET;
}

void CNpc::AddEffect(const std::string& strName)
{
	std::map<std::string, CEffect*>::iterator iter = m_mapEffects.find(strName);
	if (iter == m_mapEffects.end())
	{
		CEffect* pEffect = new CEffect(strName);
		m_mapEffects.insert(std::make_pair(strName, pEffect));
	}
}

bool CNpc::HaveEffect(const std::string& strName)
{
	std::map<std::string, CEffect*>::iterator iter = m_mapEffects.find(strName);
	return iter != m_mapEffects.end();
}

void CNpc::ClearEffect(const std::string& strName)
{
	std::map<std::string, CEffect*>::iterator iter = m_mapEffects.find(strName);
	if (iter != m_mapEffects.end())
	{
		SAFE_DELETE(iter->second);
		m_mapEffects.erase(iter);
	}
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \return 返回值的描述
*/
// 受伤,不播放受伤动画
void CNpc::OnHurtAddHint(stRTMagicUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	if( !IsMainRole() && GetHP() && 
		(pCmd->byRetcode != RTMAGIC_FAILURE && 
		pCmd->byRetcode != RTMAGIC_DUCK))
		SetHP( pCmd->dwHP );

	if( !IsMainRole() &&										//不是主角
		!::IsMainRole( pCmd->bySrc,pCmd->dwSrcTempID) &&		//对方也不是主角
		!IsMyPet()  &&											//不是主角的宠物
		!::IsMyPet(pCmd->dwSrcTempID) )							//对方也不是主角的宠物
		return ;

	if( (GetNpcTypeItem() == NpcTypeItem_Flower)
		|| (GetNpcTypeItem() == NpcTypeItem_Resource) )				//这种 类型的NPC被攻击掉血什么的，不需要显示
	{
		return ;
	}

	stNumColor t;
	t.height = 0;
	switch(pCmd->byRetcode)
	{
	case RTMAGIC_FAILURE:	// 攻击失败
		{
			t.color = COLOR_ARGB(255,0,201,189);
			strcpy(t.szNum,"失误");			
			PushBackNumColor(t);

			//Add main character attack failure num
			if ( ::IsMainRole( pCmd->bySrc,pCmd->dwSrcTempID)
				&& (this == GetScene()->m_pInterestNpc)
				&& (GetGameApplication()->bClientSetted( enumAuto_Kill_Summon )) )
			{
				GetScene()->m_InterestNpcBaseInfo.nAttackFailNum++ ;
				if ( GetScene()->m_InterestNpcBaseInfo.nAttackFailNum >= 5 )
				{
					GetScene()->GetMainCharacter()->m_pAttackNpc = NULL;
					GetScene()->GetMainCharacter()->m_pActiveNpc = NULL;
					GetScene()->GetMainCharacter()->ClearRunEvent();
				}
			}
		}
		break;
	case RTMAGIC_DUCK:
		{
			t.color = COLOR_ARGB(255,0,201,189);			
			strcpy(t.szNum,"躲避");		
			PushBackNumColor(t);
		}
		break;
	case RTMAGIC_SUCCESS:	// 攻击成功			
	case RTMAGIC_VAMPIRE:	// 吸血
	case RTMAGIC_ATTACKED:	// 被击
		{		
					t.byLuck = 0;
				unsigned __int64 num = (unsigned __int64)pCmd->sdwHP; //by=>friday 修复64位伤害显示，使用VS2003兼容的无符号64位类型
		if(num != 0)
		{
			//by=>friday 根据伤害类型设置不同的颜色和消息
			switch(pCmd->byLuck)
			{
			case DAMAGE_TYPE_PHYSICAL_CRIT:
				t.color = COLOR_ARGB(255,0,244,0);
				t.byLuck = 1;
				break;
			case DAMAGE_TYPE_HOLY_CRIT:
				t.color = COLOR_ARGB(255,0,244,0);
				t.byLuck = 1;
				if (GetGameGuiManager()->m_guiMain)
				{
					GetGameGuiManager()->m_guiMain->AddMessage("神圣一击",CGuiMain::LuckAttackMessage,COLOR_ARGB(255,0,244,0));
				}
				break;
			case DAMAGE_TYPE_ULTIMATE:
				t.color = COLOR_ARGB(255,255,215,0); // 金色
				t.byLuck = 1;
				break;
			case DAMAGE_TYPE_SLASH:
				t.color = COLOR_ARGB(255,160,32,240); // 数字保持红橙色
				t.byLuck = 1;
				break;
			default: // DAMAGE_TYPE_NORMAL
				if(IsMainRole() || IsMyPet() )
				{
					t.color = COLOR_ARGB(255,255,0,30);
				}
				else
					t.color = COLOR_ARGB(255,255,72,24);
				break;
			}
			//by=>friday 根据伤害类型设置显示文本
			// 使用FormatLargeNumber格式化数字，使大数值更易读
			std::string formattedNum = FormatLargeNumber(num);
			
			if (pCmd->byLuck == DAMAGE_TYPE_SLASH) {
				// 使用特殊格式显示"切割"和数字，确保紧密对齐
				// 将"切割"和数字作为一个整体显示，使用特效动画
				// 通过设置byLuck=1启用特效显示，并使用更大的缩放比例
				sprintf(t.szNum, "切割%s", formattedNum.c_str());
				t.color = COLOR_ARGB(255,148,0,211); // 亮粉色，更醒目
				t.scale = stPointF(1.1f,1.1f); // 更大的缩放
				t.byLuck = 1; // 确保使用特效显示
			} else if (pCmd->byLuck == DAMAGE_TYPE_ULTIMATE) {
				// 使用特殊格式显示"绝技"和数字，确保紧密对齐
				// 将"绝技"和数字作为一个整体显示，使用特效动画
				// 通过设置byLuck=1启用特效显示，并使用更大的缩放比例
				sprintf(t.szNum, "绝技%s", formattedNum.c_str());
				t.color = COLOR_ARGB(255,255,215,0); // 金色
				t.scale = stPointF(1.1f,1.1f); // 更大的缩放
				t.byLuck = 1; // 确保使用特效显示
			} else {
				// 普通伤害直接使用格式化后的数字
				strncpy(t.szNum, formattedNum.c_str(), sizeof(t.szNum)-1);
				t.szNum[sizeof(t.szNum)-1] = 0; // 确保字符串以null结尾
			}

				// 不再强制调整现有伤害数字的高度，让它们自然上移
				// 这样新的伤害数字不会影响旧的伤害数字的显示
				PushBackNumColor(t);
			}
		}
		break;
	case RTMAGIC_VAMPIREMAGIC:	// 吸魔
		if (pCmd->sdwHP != 0)
		{
			t.color = 0xff808080;
			_ui64toa((unsigned __int64)pCmd->sdwHP,t.szNum,10); //by=>friday 使用VS2003兼容的无符号64位转字符串函数
			t.szNum[15] = 0;
			PushBackNumColor(t);
		}
		break;
	default:
		break;
	}

	FUNCTION_END;
}

void CNpc::AddHpMpPopEffect( enumPopEffect popType, int value )
{
	FUNCTION_BEGIN;

	stNumColor popEffect;

	//Set color
	if ( value > 0 )
		popEffect.color = 0xff0000ff;
	else
	{
		popEffect.color = 0xffff0000;
		value *= -1;
	}	
	sprintf( popEffect.szNum, "%d", value );
	PushBackNumColor(popEffect);

	FUNCTION_END;
}

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	描述: 		  开始播放死亡动画
*/
void CNpc::Die()
{
	FUNCTION_BEGIN;

	// 开始播死亡动画

	//TRACE("NPC 开始播放死亡动画, ID [%u]\n ",m_dwID);

	RemoveDynamicObstacle();

	m_pCharacter->Die();
	m_bDieWaitingMagic = false;
	m_nDieWaitingTime = 0;

	if(IsMainRole())
	{
		 ((CMainCharacter*)this)->m_listExpCmd.clear();
		//while(!((CMainCharacter*)this)->m_listExpCmd.empty())
	}

	//while(!m_recHurtCmd.empty()) m_recHurtCmd.pop();

	//m_listHurtCmd.resize(0);
	while(!m_queueHPCmd.empty())
	{
		delete m_queueHPCmd.front();
		m_queueHPCmd.pop_front();
	}

	if (IsMainRole())
	{
		((CMainCharacter*)this)->ClearRunEvent();
		((CMainCharacter*)this)->ClearPath();
	}
	else
	{
		if (GetScene()->GetMainCharacter())
			GetScene()->GetMainCharacter()->OnNpcDeath(this);
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
int CNpc::GetAttackSound()
{
	FUNCTION_BEGIN;

	return 0;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
int CNpc::GetHitSound()
{
	FUNCTION_BEGIN;

	return 0;

	FUNCTION_END;
}

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	用途: 		  播放击中（近身攻击）的声音
*/
bool CNpc::OnHit(CNpc* p)
{
	FUNCTION_BEGIN;

	if (p->IsCanAttack() && IsAttackAni(GetInterface()->GetAction()))
	{
		int sound = GetHitSound();
		if(sound)
		{
			char szSound[MAX_PATH];
			sprintf(szSound,"%d",sound);
			p->m_pCharacter->PlayHitSound(szSound);
		}
		return true;
	}
	return false;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param cmd : 描述
* \return 返回值的描述
*/
bool CNpc::HandleHurtCommand(stRTMagicUserCmd & cmd)
{
	FUNCTION_BEGIN;

	bool bCanHurt = m_pCharacter->CanChangeToAction(Ani_Hurt);
	bool bCurHurt = (m_pCharacter->GetAction() == Ani_Hurt);

	//如果npc死亡了,正在播放死亡动画时就不播受伤动画了
	if( IsValidHurt(&cmd) || !m_bDied)
	{
		//if( (!bCanHurt ||  bCurHurt) )
		//{
		//	// 这时不能播动画,则缓冲一下,等待可播动画的时间
		//	m_recHurtCmd.c.resize(0);
		//	m_recHurtCmd.push(cmd);
		//}
		//else
		{
			// 受伤,播放受伤动画
			OnHurt(&cmd);
		}
		return true;
	}
	else
	{
		// 受伤,不播放受伤动画
		OnHurtAddHint(&cmd);
		return false;
	}

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pSender : 描述
* \return 返回值的描述
*/
bool CNpc::OnMagicHurt(CNpc* pSender)
{
	FUNCTION_BEGIN;

	//TRACE("***************************************************%s time %d\n",__FUNCTION__,xtimeGetTime());

	// 受伤
	if (pSender != NULL)
	{
		// 播放打击声音
		pSender->OnHit(this);

	}

	bool bHurt = false;
	bool bFind = false;
	for(tListHPCmd::iterator it = m_queueHPCmd.begin();it != m_queueHPCmd.end();)
	{
		stNullUserCmd* pCmd = (*it);

		if(pCmd->byCmd != MAGIC_USERCMD || pCmd->byParam != RTMAGIC_USERCMD_PARA)
		{
			HandleCommand(pCmd);
			delete pCmd;
			it = m_queueHPCmd.erase(it);
			continue;
		}

		stRTMagicUserCmd& cmd = *(stRTMagicUserCmd*)(*it);

		//if(cmd.dwHP == 0)
		//{
		//	TRACE("OnMagicHurt at [%d]\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d \n",
		//		__LINE__,
		//		cmd.dwUserTempID,
		//		cmd.byTarget,
		//		cmd.byRetcode,
		//		cmd.byDirect,
		//		cmd.bySrc,
		//		cmd.dwSrcTempID,
		//		cmd.dwHP,
		//		cmd.wdHP,
		//		cmd.byLuck);
		//}

		if (pSender != NULL && cmd.dwSrcTempID == pSender->m_dwID)
		{
			// 找到了相应的指令
			if (!pSender->IsCharacter())
			{
				// 发送者是NPC
				if (cmd.bySrc == MAPDATATYPE_NPC)
				{
					bHurt = HandleHurtCommand(cmd);
					delete (*it);
					it = m_queueHPCmd.erase(it);
					bFind = true;
					break;
				}
			}
			else
			{
				// 发送者是人物
				if (cmd.bySrc == MAPDATATYPE_USER)
				{
					bHurt = HandleHurtCommand(cmd);
					delete (*it);
					it = m_queueHPCmd.erase(it);
					bFind = true;
					break;
				}
			}
			
		}
		else if (pSender == NULL)
		{
			bHurt = HandleHurtCommand(cmd);
			delete (*it);
			it = m_queueHPCmd.erase(it);
			bFind = true;
			break;
		}
		++it;
	}

	if(!bFind)
	{
		//Engine_WriteLogF("有碰撞产生，未收到被击指令!\n");
	}

	if (m_bDieWaitingMagic)
	{
		Die();
		return true;
	}

	//If this is main character we attack back automaticly
	if ( IsMainRole() && bHurt )
	{
		if ( pSender )
			GetScene()->GetMainCharacter()->AttackBackSummon( pSender );
	}

	if (!bHurt)	// 如果没有受伤
	{
		if (pSender == this) //follow 他的magic
		{
			// 如果发送者是自己
			while (!m_queueHPCmd.empty())
			{
				stNullUserCmd* pCmd = m_queueHPCmd.front();
				if(pCmd->byCmd != MAGIC_USERCMD || pCmd->byParam != RTMAGIC_USERCMD_PARA)
				{
					HandleCommand(pCmd);
					delete pCmd;
					m_queueHPCmd.pop_front();
					continue;
				}
				else
				{
					stRTMagicUserCmd& cmd = *(stRTMagicUserCmd*)(*it);
					OnHurtAddHint(&cmd);
					delete pCmd;
					m_queueHPCmd.pop_front();
				}
			}
		}
	}

	return bHurt;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
DWORD CNpc::GetBodySystemColor()
{
	FUNCTION_BEGIN;

	return m_pNpcBase->dwColor;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param type : 描述
 * \return 返回值的描述
 */
bool CNpc::IsNpcType(enumNpcType type) 
{
	FUNCTION_BEGIN;
 
	enumNpcTypeItem typeItem = GetNpcTypeItem();
	switch(type)
	{
	case NPC_TYPE_ATTACK:
		{
			bool bCanAttack = typeItem != NpcTypeItem_Trade && 
				typeItem != NpcTypeItem_Task && 
				typeItem != NpcTypeItem_Guide && 
				typeItem != NpcTypeItem_MobileTrade && 
				typeItem != NpcTypeItem_LiveNpc	&& 
				typeItem != NpcTypeItem_MailMan	&& 
				typeItem != NpcTypeItem_Auction	&& 
				typeItem != NpcTypeItem_Cartoon	&& 
				typeItem != NpcTypeItem_SavingBox;

			if ( !bCanAttack )
			{
           //**********************************************
           //soke 边境传送和所有车夫NPC(外国人也可以访问)
	      if( GetTypeID() == 95||GetTypeID() == 142
		    ||GetTypeID() ==143||GetTypeID() == 149
		    ||GetTypeID() ==150||GetTypeID() == 158
		    ||GetTypeID() ==301||GetTypeID() == 302
		    ||GetTypeID() ==496||GetTypeID() == 513
		    ||GetTypeID() ==843||GetTypeID() == 844
		    ||GetTypeID() ==845||GetTypeID() == 846
		    ||GetTypeID() ==847||GetTypeID() ==1023
			|| ((GetTypeID() >= 1070) && (GetTypeID() <= 1103))
			|| ((GetTypeID() >= 908) && (GetTypeID() <= 913))
			 ||GetTypeID() ==974)
		    return false;
	       //**********************************************
				//To see if this type is can attack
				//TBD
				if ( ((GetTypeID() >= 5000) && (GetTypeID() <= 6000))
					|| ((GetTypeID() >= 7000) && (GetTypeID() <= 7100)) 
					//自行车新增NPC
					|| ((GetTypeID() >= 52101) && (GetTypeID() <= 52111)) )
					return false;

				//To see if main character is at oversea
				if ( !GetScene()->IsCurMapIsMiddleCountry() )
				{
					DWORD nID = GetScene()->GetMainCharacter()->GetCountry();
					if( !CCountry::Instance()->CheckCountry( nID,GetScene()->GetMapCountryName() ) )
					{
						if ( !CCountry::Instance()->IsAllyCountry( GetScene()->GetMapCountryID() ) )
							return true;
					}
				}					

				return false;
			}
			else
				return true;
		}
		break;
	case NPC_TYPE_FUNCTION:
		{
			bool bFuncType = typeItem == NpcTypeItem_Trade || 
				typeItem == NpcTypeItem_Task ||
				typeItem == NpcTypeItem_MobileTrade||
				typeItem == NpcTypeItem_LiveNpc || 
				typeItem == NpcTypeItem_MailMan || 
				typeItem == NpcTypeItem_Auction ||
				typeItem == NpcTypeItem_SavingBox;

			if ( bFuncType )
			{
		   //**********************************************
           //soke 边境传送和所有车夫NPC(外国人也可以访问)
	      if( GetTypeID() == 95||GetTypeID() == 142
		    ||GetTypeID() ==143||GetTypeID() == 149
		    ||GetTypeID() ==150||GetTypeID() == 158
		    ||GetTypeID() ==301||GetTypeID() == 302
		    ||GetTypeID() ==496||GetTypeID() == 513
		    ||GetTypeID() ==843||GetTypeID() == 844
		    ||GetTypeID() ==845||GetTypeID() == 846
		    ||GetTypeID() ==847||GetTypeID() ==1023
			|| ((GetTypeID() >= 1070) && (GetTypeID() <= 1103))
			|| ((GetTypeID() >= 908) && (GetTypeID() <= 913))
			||GetTypeID() ==974)
		    return  true;
	       //**********************************************
				//To see if this type is can attack
				//TBD
				if ( ((GetTypeID() >= 5000) && (GetTypeID() <= 6000))
					|| ((GetTypeID() >= 7000) && (GetTypeID() <= 7100)) 
					//自行车新增NPC
					|| ((GetTypeID() >= 52101) && (GetTypeID() <= 52111)) )
					return true;

				//To see if main character is at oversea
				if ( !GetScene()->IsCurMapIsMiddleCountry() )
				{
					DWORD nID = GetScene()->GetMainCharacter()->GetCountry();
					if( !CCountry::Instance()->CheckCountry( nID,GetScene()->GetMapCountryName() ) )
					{
						if ( !CCountry::Instance()->IsAllyCountry( GetScene()->GetMapCountryID() ) )
							return false;
					}
				}					

				return true;
			}
			else
				return false;
		}
		break;
	}
	return false;

	FUNCTION_END;
}

bool CNpc::IsCanAttackFunctionNpc()
{
	if ( !IsNpcType( NPC_TYPE_FUNCTION) )
		return false;

	if ( GetScene()->GetMainCharacter() )
	{
		DWORD nID = GetScene()->GetMainCharacter()->GetCountry();
		if( CCountry::Instance()->CheckCountry( nID,GetScene()->GetMapCountryName() ) )
			return false;
		else if ( CCountry::Instance()->IsAllyCountry( GetScene()->GetMapCountryID() ) )
			return false;
		else
		{
		   //**********************************************
           //soke 边境传送和所有车夫NPC(外国人也可以访问)
	      if( GetTypeID() == 95||GetTypeID() == 142
		    ||GetTypeID() ==143||GetTypeID() == 149
		    ||GetTypeID() ==150||GetTypeID() == 158
		    ||GetTypeID() ==301||GetTypeID() == 302
		    ||GetTypeID() ==496||GetTypeID() == 513
		    ||GetTypeID() ==843||GetTypeID() == 844
		    ||GetTypeID() ==845||GetTypeID() == 846
		    ||GetTypeID() ==847||GetTypeID() ==1023
			|| ((GetTypeID() >= 1070) && (GetTypeID() <= 1103))
			|| ((GetTypeID() >= 908) && (GetTypeID() <= 913))
			||GetTypeID() ==974)
				return false;
			else
				return true;
	       //**********************************************
			if ( ((GetTypeID() >= 5000) && (GetTypeID() <= 6000))
				|| ((GetTypeID() >= 7000) && (GetTypeID() <= 7100))
				//自行车新增NPC
					|| ((GetTypeID() >= 52101) && (GetTypeID() <= 52111))  )
				return false;
			else
				return true;
		}
	}

	return false;
}

/**
 * \brief 是否同一个NPC
 * 
 * 
 * 
 * \param baseInfo : 要比较的NPC特征值，见结构定义
 * \return 是同一NPC则返回true, 否则返回false
 */
bool CNpc::IsSame( const stInterestNpcBaseInfo& baseInfo )
{
	if ( IsMainRole() )
		return false;

	if ( IsCharacter() )
	{
		if ( baseInfo.bCharacter )
		{
			if ( baseInfo.tmpID != GetID() )
				return false;
		}
		else
			return false;
	}	

	if ( IsNpc() )
	{
		if ( baseInfo.bNpc )
		{
			if ( baseInfo.tmpID != GetID() )
				return false;
		}
		else
			return false;
	}

	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pSkill : 描述
 * \param startTime : 描述
 * \return 返回值的描述
 */
bool CNpc::CanUseSkillByTime( SkillBase_t* pSkill, DWORD startTime )
{
	FUNCTION_BEGIN;

	if ( NULL == pSkill )
		return true;

	if ( m_aSkillUseTime.empty() )
		return true;

	if ( NULL == pSkill )
		return true;

	//*
	std::map<DWORD,DWORD>::iterator it;
	it = m_aSkillUseTime.find(pSkill->dwID);
	if( it == m_aSkillUseTime.end() )
	{
		return true;
	}

	DWORD tDisableTime = 0;
	if ( 0 )
	{
		if ( IsMainRole() )
		{
			int atkSpeed = GetScene()->GetMainCharacter()->GetAttackSpeedServer();
			if ( (pSkill->dwID == SERVER_SKILL_ATTACK_NORMAL)
				|| (pSkill->dwSerialType == SKILL_SERIAL_FIGHTER) )		//pSkill->dwID == SERVER_SKILL_ATTACK_NORMAL
				tDisableTime = (120*6+640) * (float)atkSpeed / 100.0f;
			else if ( (pSkill->dwID == SERVER_SKILL_BOW_NORMAL)
				|| (pSkill->dwSerialType) != SKILL_SERIAL_QIANG ) //sky
				tDisableTime = (100*8+800) * (float)atkSpeed / 100.0f;
			else
				tDisableTime = (120*8+960) * (float)atkSpeed / 100.0f;
		}
		else
		{
			tDisableTime = 1600;		//
		}
	}
	else
	{
		if ( IsMainRole() )
		{
			int atkSpeed = GetScene()->GetMainCharacter()->GetAttackSpeedServer();
			if ( (pSkill->dwID == SERVER_SKILL_ATTACK_NORMAL)
				|| (pSkill->dwID == SERVER_SKILL_BOW_NORMAL)
				|| (pSkill->dwSerialType == SKILL_SERIAL_FIGHTER)
				|| (pSkill->dwSerialType == SKILL_SERIAL_SHOOTER) )		//pSkill->dwID == SERVER_SKILL_ATTACK_NORMAL
				tDisableTime = (120*6+640) * (float)atkSpeed / 100.0f;
			else if ( pSkill->dwSerialType == SKILL_SERIAL_QIANG ) //sky
				tDisableTime = (120*8+960) * (float)atkSpeed / 100.0f;
			else
				tDisableTime = (100*8+800) * (float)atkSpeed / 100.0f;
		}
		else
		{
			tDisableTime = 1360;		//
		}

		if ( pSkill->dwID == SKILL_ATTACK_FLOWER )
			tDisableTime = 100;
	}
	
	
	
	DWORD tIntervalTime = pSkill->dwIntervalTime;
	CRoleSkill* pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( pSkill->dwID );
	SkillLevelBase_t* pSkillLevelBase = GetSkillLevelBase( pSkill->dwID, pRoleSkill ? pRoleSkill->GetTotalLevel() : 1 );
	if ( pSkillLevelBase )
		tIntervalTime = pSkillLevelBase->dwIntervalTime;

	if ( (((xtimeGetTime() - it->second) < tIntervalTime) && (it->second != 0))
		|| (xtimeGetTime() - startTime < tDisableTime) )			//public disable time	
	{
		//TRACE("************** CanSendMagicByTime = false time = %u***********************\n",xtimeGetTime() - it->second);
		return false;
	}
	//*/

//#ifdef _DEBUG
//	TRACE( "\n???????????????????????????????????????????????????Disable time is OK = %d", xtimeGetTime()-startTime );
//#endif
	return true;

	//return xtimeGetTime() - m_dwUseSkillTime > m_dwUseSkillIntervalTime;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pSkill : 描述
 * \return 返回值的描述
 */
void CNpc::SetUseSkillTime(SkillBase_t* pSkill, DWORD startTime)
{
	FUNCTION_BEGIN;

	//m_dwUseSkillTime = xtimeGetTime();
	//m_dwUseSkillIntervalTime = pSkill->dwIntervalTime;

	if ( IsCombinSkill( pSkill ) )
	{
		for( int i=0;i<COMBINATION_STATE_NUM;i++ )
		{
			//SkillBase_t* pCmbSkill = GetSkillBase( cmbSkill[i] );
			CRoleSkill* pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( cmbSkill[i] );
			if ( pRoleSkill )
				m_aSkillUseTime[cmbSkill[i]] = startTime;
		}
	}
	else if ( IsShooterJingSkill( pSkill ) )
	{
		for( int i=0;i<count_of(ShooterJingSkill);i++ )
		{
			CRoleSkill* pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( ShooterJingSkill[i] );
			if ( pRoleSkill )
				m_aSkillUseTime[ShooterJingSkill[i]] = startTime;
		}
	}
	else
		m_aSkillUseTime[pSkill->dwID] = startTime;


	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pSkill : 描述
 * \return 返回值的描述
 */
void CNpc::ClearUseSkillTime(SkillBase_t* pSkill)
{
	FUNCTION_BEGIN;

	m_aSkillUseTime[pSkill->dwID] = 0;

	FUNCTION_END;
}

//When character during special state can not sitdown
bool CNpc::IsCurStateHasSitDownAni()
{
	if ( !IsCharacter() )
		return false;

	if ( IsChangeFace()
		|| IsCombinationState() )
	{
		return false;
	}

	return true;
}

bool CNpc::IsChangeFace()
{
	return m_proChangingFace.m_dwChangingFaceID != -1;
	//return m_pChangeFace != NULL;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool CNpc::IsRide()
{
	FUNCTION_BEGIN;

	return m_pCharacter->IsRide();

	FUNCTION_END;
}


bool CNpc::IsAniStand()
{
	FUNCTION_BEGIN;

	AniTypeEnum ani = m_pCharacter->GetAction();
	return  (Ani_Stand == ani || Ani_Ride_Wait == ani);

	FUNCTION_END;
}

int CNpc::GetHorseLevel()
{
	FUNCTION_BEGIN;

	CPet* pPet = GetScene()->FindPetByType(PET_TYPE_RIDE);
	if(pPet)
	{
		CHorse* pHorse = static_cast<CHorse*>( pPet );
		if(pHorse)
		{
			return pHorse->GetHorseLevel();
		}
	}
	return 0;

	//return m_pCharacter->IsRide();

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptMapOffset : 描述
 * \return 返回值的描述
 */
void CNpc::AddLight(POINT ptMapOffset)
{
	FUNCTION_BEGIN;

	if(m_pCharacter->IsCreate())
		m_pCharacter->AddLight(ptMapOffset);

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pSkill : 描述
 * \return 返回值的描述
 */
DWORD CNpc::GetSkillStartTime( SkillBase_t* pSkill )
{
	FUNCTION_BEGIN;

	std::map<DWORD,DWORD>::iterator it = m_aSkillUseTime.find(pSkill->dwID);
	if(it == m_aSkillUseTime.end())
	{
		return 0;
	}

	return it->second;

	FUNCTION_END;
}

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	描述: 		获得技能的冷却时间（0 - 1，1表示可用了）
*/
float CNpc::GetSkillDisableStatus(SkillBase_t* pSkill)
{
	FUNCTION_BEGIN;

	if ( NULL == pSkill )
		return 1.0f;

	std::map<DWORD,DWORD>::iterator it = m_aSkillUseTime.find(pSkill->dwID);
	if(it == m_aSkillUseTime.end())
		return 1.0f;

	if ( it->second == 0 )
		return 1.0f;

	CRoleSkill* pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( pSkill->dwID );
	if ( NULL == pRoleSkill )
		return 1.0f;

	SkillLevelBase_t* pSkillLevel = GetSkillLevelBase( pSkill->dwID, pRoleSkill->GetTotalLevel() );
	if ( NULL == pSkillLevel )
		return 1.0f;

	if( xtimeGetTime() - it->second > pSkillLevel->dwIntervalTime ) 
		return 1.0f;

	if ( pSkillLevel->dwIntervalTime <= 0 )
		return 1.0f;

	return (float)(xtimeGetTime() - it->second)/(float)pSkillLevel->dwIntervalTime;

	FUNCTION_END;
}

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	描述: 		  绘制技能图标的冷却状态
*/
void CNpc::DrawSkillDisableStatus(SkillBase_t* pSkill,const RECT * prect)
{
	FUNCTION_BEGIN;

	float f = GetSkillDisableStatus(pSkill);
	if(fabs(f - 1.0f) < 0.0001)
	{
		DrawSkillMagicLower( pSkill, prect );
		return;
	}

	GetDevice()->DrawRectPie(prect,angle2radian(90),angle2radian( 90 - f*360),COLOR_ARGB(200,50,50,50));


	FUNCTION_END;
}

bool CNpc::IsCombinCanUseSkill( SkillBase_t* pSkill )
{
	FUNCTION_BEGIN;

	if ( !IsCombinationState() )
		return true;

	if ( NULL == pSkill )
		return true;

	/*if ( (pSkill->dwID == SERVER_SKILL_ATTACK_NORMAL) 
		|| (pSkill->dwID == SERVER_SKILL_BOW_NORMAL) )
	{
		return true;
	}*/

	for( int k=0;k<COMBINATION_STATE_NUM;k++ )
	{
		if ( isset_state( GetState(), cmbState[k] ) )
		{
			if ( pSkill->dwSP != cmbSkill[k] )
				return false;
		}
	}		

	FUNCTION_END;

	return true;
}

bool CNpc::SkillCanOnlyUseCombination( SkillBase_t* pSkill )
{
	FUNCTION_BEGIN;
	
	if ( NULL == pSkill )
		return true;

	SkillBase_t* pSkillCombin = GetSkillBase( pSkill->dwSP );
    if ( NULL == pSkillCombin )
		return true;

	for( int k=0;k<COMBINATION_STATE_NUM;k++ )
	{
		if ( pSkill->dwSP == cmbSkill[k] )
		{
			if ( isset_state( GetState(), cmbState[k] ) )
				return true;
		}
	}

	return false;

	FUNCTION_END;
}

void CNpc::DrawSkillMagicLower( SkillBase_t* pSkill, const RECT * prect )
{
	FUNCTION_BEGIN;
	
	if ( (GetMP() < pSkill->dwMP)
		|| (IsChangeFace() && (pSkill->dwID != SERVER_SKILL_CHANGE_FACE))
		|| !IsCombinCanUseSkill(pSkill)
		|| !SkillCanOnlyUseCombination(pSkill)
		/*|| (pSkill->dwTreeType != 1) */)
	{
		GetDevice()->FillRect( prect, 0x550000ff );
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
enumFiveType CNpc::GetMainFive()
{
	FUNCTION_BEGIN;

	if(!m_pNpcBase) return FiveType_Gold;
	return (m_pNpcBase->dwFive >= FiveType_Num ? FiveType_Gold : (enumFiveType)m_pNpcBase->dwFive) ;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
size_t CNpc::GetLevel()
{
	FUNCTION_BEGIN;

	//改为从服务器发送过来的等级
	return m_data.level;

	FUNCTION_END;
}

bool CNpc::bIsShopStart()
{
	return isset_state(GetState(),USTATE_PRIVATE_STORE);
}

//soke 飞行状态检测
bool CNpc::bIsShopFly()
{
	return isset_state(GetState(),USTATE_USER_FLY);
}

//soke 缩骨检测
bool CNpc::bIsSuogu()
{
	return isset_state(GetState(),USTATE_SUOGUDAN);
}

RECT CNpc::GetShopTextRect()
{
	if( bIsShopStart() )
	{
		return m_ShopNameTip.GetClipRect();
	}
	else
	{
		return stRectI(0,0,0,0);
	}
}

void CNpc::AddPetData(const t_MapPetData* pData)
{
	if(m_pPetData == NULL)
	{
		m_pPetData = new t_MapPetData;
	}
	memcpy(m_pPetData,pData,sizeof(t_MapPetData));
	Debug_String(m_pPetData->name);
	SetName(m_pPetData->name);
}

void CNpc::PushBackNumColor(const stNumColor& nc)
{	
	// 限制最大显示数量，但保留更多的伤害数字以实现更好的视觉效果
	if( m_listNumColor.size() > 30 ) // 增加上限，允许更多伤害数字同时显示
	{
		// 只移除最旧的几个伤害数字，保留更多的最近伤害
		tListNumColor::iterator it = m_listNumColor.begin();
        m_listNumColor.erase(it,it+10);
	}
	
	// 为新的伤害数字设置初始高度，确保它们从正确的位置开始显示
	stNumColor newNC = nc;
	
	// 添加新的伤害数字到列表末尾
	m_listNumColor.push_back(newNC);
}
