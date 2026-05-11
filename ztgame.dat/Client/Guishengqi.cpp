/**
*\file		GuiTop.cpp
*\version	$Id$
*\author	$$$01公司
*\			$$$01公司@###.com
*\date		2006-7-19  21:11:00 中国标准时间
*\brief	    经脉对话框
*
* 
*
*/
#include "public.h"
#include "../gui/include/GuiManager.h"	
#include "./GameGuiManager.h"
#include "./Game.h"
#include "./Guishengqi.h"
#include "./table.h"
#include "./RoleItem.h"
#include "./GameScene.h"
#include "Maincharacter.h"
///////////////////////////////////////////////////////////////////////////////
#define ID_BUTTON_JIANZHONG  200    // 剑冢
#define ID_BUTTON_JINGMAI    201    // 经脉
#define ID_BUTTON_SHENGXIAO  202   // 生肖
#define ID_BUTTON_SHENGQI    203    // 圣器
#define ID_BUTTON_ZHENHGTUZHUAN  204 // 征途传
#define ID_BUTTON_TISHENG     29 // 提升
 
#define ID_BUTTON_CLOSE       1   //关闭
///////////////////////////////////////////////////////////////////////////////
CGuishengqiDlg::CGuishengqiDlg()
{
	FUNCTION_BEGIN;

	m_bSmDialog = true;

	FUNCTION_END;
}

CGuishengqiDlg::~CGuishengqiDlg()
{
	FUNCTION_BEGIN;


	FUNCTION_END;
}

void CGuishengqiDlg::OnCreate()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnCreate();

	select = 0;
	sq1=0;

	shengq1 = GetButton(12);
	wenzi = GetStatic(31);

	cltext = GetStatic(27);
	moneytext = GetStatic(28);
	
	m_pTableMaterial1 = this->GetTable(130);
	m_pTableMaterial1->m_iTableType = OBJECTCELLTYPE_FUJIAN;
	m_pTableMaterial1->m_iTableID = 0;
	m_pTableMaterial1->m_EquipPosition = stPointI(0,4);

	FUNCTION_END;
}

bool CGuishengqiDlg::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	FUNCTION_BEGIN;

	if(nEvent == EVENT_BUTTON_CLICKED)
	{
		switch(nID)
		{
		case ID_BUTTON_CLOSE:  // 关闭
			{
				SetVisible(false);
			}
			break;
		case ID_BUTTON_JINGMAI:  // 经脉
			{
				GetGameGuiManager()->Addjingmai();
				SetVisible(false);
			}
			break;
		case ID_BUTTON_JIANZHONG:  // 剑冢
			{
				GetGameGuiManager()->Addjianzhong();
				SetVisible(false);
			}
			break;
		case ID_BUTTON_SHENGXIAO:  // 生肖
			{
				GetGameGuiManager()->Addshengxiao();
				SetVisible(false);
			}
			break;
		case ID_BUTTON_SHENGQI:  // 圣器
			{
				
			}
			break;
		case ID_BUTTON_ZHENHGTUZHUAN:  // 征途转
			{
				GetGameGuiManager()->Addzhengtuzhuan();
				SetVisible(false);
			}
			break;
			case 12:
			{
				char a[32];
				select = 1;
			    this->update(sq1);
				if(sq1==0)
				{
					wenzi->SetText("圣器尚未激活");
				}
				else
				{
			        sprintf(a,"圣器 当前等级：%d",sq1);
			        wenzi->SetText(a); 				
				}
	    	}
			break;
			case 29:
			{
				if(GetScene()->GetMainCharacter()->GetLevel()<200)
				{
					cltext->SetText("200级开放圣器功能！");
					return false;
				}
				if(select==0)
				{
					return false;
				}
				CRoleItem* pRoleItem = this->GetMaterialItem();
				if(!pRoleItem) 
				{
					return false;
				}
				stFujianShengqiLevelup cmd;
				cmd.shengq=select; 
			    cmd.itemUID = pRoleItem->GetObject()->qwThisID;
			    SEND_USER_CMD(cmd);
			}			
			break;		
		}		
		return true;
	}
	return CGuiDialog::OnGuiEvent(nEvent,nID,pControl);
	FUNCTION_END;
}

CRoleItem* CGuishengqiDlg::GetMaterialItem()
{
	CRoleItem* pRoleItem = NULL;
	CGuiItemCell* pItemCell = m_pTableMaterial1->GetItemCell(0, 0);
	if (pItemCell)
	{
		pRoleItem = static_cast<CRoleItem*>(pItemCell->m_pItemData);
	}

	return pRoleItem;
}

void CGuishengqiDlg::update(int num)
{
    CGuiStatic *level = GetStatic(109);
    char a[256];
	sprintf(a, "%d级", num);
	level->SetText(a);
	CGuiStatic *level2 = GetStatic(116);
	sprintf(a, "%d级", num + 1);
	level2->SetText(a);

	int pDam=0;
	int mDam=0;
	int pDef=0;
	int mDef=0;
	int hp=0;
	int qiegeattack=0;
	int qiegedefence=0;

	shengqipeizhi sjs;
	for(int i=0;i<num;i++)
	{
		sjs = g_shengqi[i]; 
		pDam+=sjs.pDam;
		mDam+=sjs.mDam;
		pDef+=sjs.pDef;
		mDef+=sjs.mDef;
		hp+=sjs.hp;
		qiegeattack+=sjs.qiegeattack;
		qiegedefence+=sjs.qiegedefence;
	}

	char nums[256];
	sprintf(nums,"常规属性 攻击 +%d 防御 +%d",pDam+mDam,pDef+mDef);
	GetStatic(110)->SetText(nums);
	sprintf(nums,"生命值 +%d",hp);
	GetStatic(111)->SetText(nums);
	sprintf(nums,"PVE属性 切割攻击 +%d 切割防御 +%d",qiegeattack,qiegedefence);
	GetStatic(112)->SetText(nums);

	//下一级
	shengqipeizhi zt = g_shengqi[num];
	sprintf(nums,"常规属性 攻击 +%d 防御 +%d",zt.pDam+zt.mDam,zt.pDef+zt.mDef);
	GetStatic(117)->SetText(nums);
	sprintf(nums,"??? +%d",zt.hp);
	GetStatic(118)->SetText(nums);
	sprintf(nums,"PVE属性 切割攻击 +%d 切割防御 +%d",zt.qiegeattack,zt.qiegedefence);
	GetStatic(119)->SetText(nums);


	int money=zt.salary;

	int ding =0;
	int liang =0;
	char moneytexts[32];
	if(money>10000){
		ding = money/10000;
		liang = (money-(ding*10000))/100;
		sprintf(moneytexts,"             需要银子:%d锭%d两",ding,liang);
	}
	else{
		liang = money/100;
		sprintf(moneytexts,"             需要银子:%d两",liang);
	}
	if(num<2000)
	{
		moneytext->SetText(moneytexts);
		ObjectBase_t* pObject = GetObjectBase(zt.costID);
		char cailiao[256];
		sprintf(cailiao,"           需要%s 数量:%d",pObject->strName,zt.costNum);
		cltext->SetText(cailiao);
	}
}
void CGuishengqiDlg::SHUAXIN()
{
	if(sq1==0)
	{
		shengq1->SetBitmap(stResourceLocation("data\\interfaces3.gl", 5, 11),1);
	}
	else
	{
		CGuiImage *img = GetImage(612);
		img->SetImage(stResourceLocation("data\\interfaces3.gl",5,13));
	}

	int pDam=0;
	int mDam=0;
	int pDef=0;
	int mDef=0;
	int hp=0;
	int qiegeattack=0;
	int qiegedefence=0;

	shengqipeizhi sjs;

	for(int i=0;i<sq1;i++)
	{
		sjs = g_shengqi[i];
		pDam+=sjs.pDam;
		mDam+=sjs.mDam;
		pDef+=sjs.pDef;
		mDef+=sjs.mDef;
		hp+=sjs.hp;
		qiegeattack+=sjs.qiegeattack;
		qiegedefence+=sjs.qiegedefence;
	}
	
	char num[256];
	sprintf(num,"常规属性");
	GetStatic(125)->SetText(num);
	sprintf(num,"物攻 +%d 魔攻 +%d",pDam,mDam);
	GetStatic(126)->SetText(num);
	sprintf(num,"物防 +%d 魔防 +%d",pDef,mDef);
	GetStatic(127)->SetText(num);
	sprintf(num,"生命值 +%d",hp);
	GetStatic(128)->SetText(num);
	sprintf(num,"PVE属性 切割攻击 +%d 切割防御 +%d",qiegeattack,qiegedefence);
	GetStatic(129)->SetText(num);

	char a[32]; 
	switch(select)
	{
		case 1:
			{
				this->update(sq1);
				if(sq1==0)
				{
					wenzi->SetText("圣器尚未激活");
				}
				else
				{
			        sprintf(a,"圣器 当前等级：%d",sq1);
			        wenzi->SetText(a); 				
				}
			}
			break;
	}		
	update(sq1);
}
void CGuishengqiDlg::OnClose(void)
{
	FUNCTION_BEGIN;

	GetGameGuiManager()->m_guishengqi = NULL;

	FUNCTION_END;
}