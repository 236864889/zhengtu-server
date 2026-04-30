#include "public.h"

#include "Game.h"
#include "GameGuiManager.h"
#include "GameScene.h"
#include "MainCharacter.h"
#include "RoleItem.h"
#include "GuiNpcMake.h"
#include "GuiNpcDialog.h"
#include "MyTimer.h"
//#include "zObject.h"
//#include "zObjectB.h"

#include "DlgEquipBabyRecast.h"

namespace
{
	const int TABLE_FOCUSITEM		= 80;   // 装备格子
	const int TABLE_MATERIAL1		= 120;  // 材料格子1
	const int STATIC_NEEDMONEY		= 117;  // 价格显示
	const int STATIC_RESULTINFO		= 125;  // 说明提示
	const int BUTTON_UPGRADE		= 124;  // 强化进阶按钮
}

CDlgEquipBabyRecast::CDlgEquipBabyRecast()
{
	FUNCTION_BEGIN;

	m_bCloseIsHide = true;

	FUNCTION_END;
}

CDlgEquipBabyRecast::~CDlgEquipBabyRecast()
{
	FUNCTION_BEGIN;

	FUNCTION_END;
}

void CDlgEquipBabyRecast::OnClose(void)
{
	//by=>friday
	GetGameGuiManager()->m_pDlgEquipBabyRecast = NULL;
}

void CDlgEquipBabyRecast::OnCreate()
{
	//by=>friday
	CGuiDialog::OnCreate();

	m_pStaNeedMoney = this->GetStatic(STATIC_NEEDMONEY);

	m_pTableFocusItem = this->GetTable(TABLE_FOCUSITEM);
	m_pTableFocusItem->m_iTableType = OBJECTCELLTYPE_BABYRECAST;  // 改为专用包裹类型 //by=>friday
	m_pTableFocusItem->m_iTableID = TABLE_FOCUSITEM;
	m_pTableFocusItem->m_EquipPosition = stPointI(0,MAKECELLTYPE_EQUIP);

	m_pTableMaterial1 = this->GetTable(TABLE_MATERIAL1);
	m_pTableMaterial1->m_iTableType = OBJECTCELLTYPE_BABYRECAST;  // 改为专用包裹类型 //by=>friday
	m_pTableMaterial1->m_iTableID = TABLE_MATERIAL1;
	m_pTableMaterial1->m_EquipPosition = stPointI(0,MAKECELLTYPE_MATERIAL1);

	m_pStaResult = this->GetStatic(STATIC_RESULTINFO);
	m_pBtnUpgrade = this->GetButton(BUTTON_UPGRADE);

	// 设置初始说明文字 //by=>friday
	if (m_pStaResult)
	{
		m_pStaResult->SetText("请将孩子装备放入装备格子，将强化材料放入材料格子");
	}
}

bool CDlgEquipBabyRecast::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	//by=>friday
	FUNCTION_BEGIN;

	switch (nEvent)
	{
	case EVENT_DIALOG_SHOW:
		{
			if (m_pStaResult)
			{
				m_pStaResult->SetText("请将孩子装备放入装备格子，将强化材料放入材料格子");
			}
		}
		break;
	case EVENT_BUTTON_CLICKED:
		{
			switch (nID)
			{
				case 2: // 关闭按钮 //by=>friday
				{
					SetVisible(false);
				}
				break;
				case BUTTON_UPGRADE: // 强化进阶按钮 //by=>friday
				{
					// 检查是否有装备和材料 //by=>friday
					CRoleItem* pEquipItem = GetFocusItem();
					CRoleItem* pMaterialItem = GetMaterialItem(0);
					
					if (!pEquipItem)
					{
						if (m_pStaResult)
						{
							m_pStaResult->SetText("请先放入孩子装备");
						}
						return true;
					}
					
					if (!pMaterialItem)
					{
						if (m_pStaResult)
						{
							m_pStaResult->SetText("请先放入强化材料");
						}
						return true;
					}
					
					// 检查是否为孩子装备 //by=>friday
					if (!IsBabyEquipItem(pEquipItem))
					{
						if (m_pStaResult)
						{
							m_pStaResult->SetText("只能升级孩子装备");
						}
						return true;
					}
					
					// 发送升级命令 //by=>friday
					SendBabyEquipUpgradeMsg();
				}
				break;
			}
		}
		break;
	}
	
	return CGuiDialog::OnGuiEvent(nEvent, nID, pControl);
	FUNCTION_END;
}

void CDlgEquipBabyRecast::OnSetCloseLocation()
{
	//by=>friday
	FUNCTION_BEGIN;

	CGuiDialog::OnSetCloseLocation();

	if(m_pBtnClose)
	{
		stPointI pt(m_pBtnClose->GetLocation());
		m_pBtnClose->SetLocation(pt.x-22, pt.y-38);
	}

	FUNCTION_END;
}

HRESULT CDlgEquipBabyRecast::OnRender(float fElapsedTime)
{
	//by=>friday
	FUNCTION_BEGIN;

	HRESULT hr = CGuiDialog::OnRender(fElapsedTime);

	return hr;

	FUNCTION_END;
} 

CRoleItem* CDlgEquipBabyRecast::GetFocusItem()
{
	//by=>friday
    CRoleItem* pRoleItem = NULL;
    CGuiItemCell* pItemCell = m_pTableFocusItem->GetItemCell(0, 0);
    if (pItemCell)
    {
        pRoleItem = static_cast<CRoleItem*>(pItemCell->m_pItemData);
    }
    return pRoleItem;
}

CRoleItem* CDlgEquipBabyRecast::GetMaterialItem(int index)
{
	//by=>friday
    CRoleItem* pRoleItem = NULL;
    CGuiItemCell* pItemCell = m_pTableMaterial1->GetItemCell(0, 0);
    if (pItemCell)
    {
        pRoleItem = static_cast<CRoleItem*>(pItemCell->m_pItemData);
    }
    return pRoleItem;
}

bool CDlgEquipBabyRecast::IsBabyEquipItem(CRoleItem* pRoleItem)
{
	//by=>friday
    if (!pRoleItem)
    {
        return false;
    }
    
    // 检查是否为孩子装备类型 //by=>friday
    if (pRoleItem->GetObjectBase()->dwType >= ItemType_BABYFJ1 && pRoleItem->GetObjectBase()->dwType <= ItemType_BABYFJ6)
    {
        return true;
    }
    return false;
}

void CDlgEquipBabyRecast::SendBabyEquipUpgradeMsg()
{
	//by=>friday
    CGuiItemCell* pItemCell = m_pTableFocusItem->GetItemCell(0, 0);
    if (!pItemCell)
    {
        return;
    }

    CRoleItem* pRoleItem = static_cast<CRoleItem*>(pItemCell->m_pItemData);
    if (pRoleItem)
    {
        ItemRecastPropertyUserCmd cmd;
        cmd.act = RECAST_ITEM_COMPOSE109; // 孩子装备升级 //by=>friday
        cmd.itemUID = pRoleItem->GetObject()->qwThisID;
        
        SEND_USER_CMD(cmd);
        
        if (m_pStaResult)
        {
            m_pStaResult->SetText("正在升级孩子装备...");
        }
    }
}

void CDlgEquipBabyRecast::OnResult(stResponsePropertyUserCmd* pCmd)
{
	//by=>friday
	if (!pCmd)
	{
		return;
	}

	switch(pCmd->kind)
	{
	case EQUIP_COMPOSE109:   //孩子装备升级
		{
			DWORD dwColor = 0;
			if (pCmd->status == 0) // 成功
			{
				stResourceLocation rl;
				rl.SetFileName("data\\Interfaces.gl");
				rl.group = 31;
				rl.frame = 51;
				m_aniSuccess.Create(&rl);
				m_aniSuccess.SetSpeed(25);
				m_aniSuccess.SetLoopPlay(false);
				PlayUISound(10005);

				dwColor = D3DCOLOR_ARGB(255,0,255,0);
			}
			else // 失败
			{
				stResourceLocation rl;
				rl.SetFileName("data\\Interfaces.gl");
				rl.group = 31;
				rl.frame = 54;
				m_aniSuccess.Create(&rl);
				m_aniSuccess.SetLoopPlay(false);

				dwColor = D3DCOLOR_ARGB(255,255,0,0);
			}
			if (m_pStaResult)
			{
				m_pStaResult->SetTextColor(dwColor);
				m_pStaResult->SetText(pCmd->info);
			}
		}
		break;
	}
}

