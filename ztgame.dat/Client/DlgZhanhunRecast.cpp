#include "public.h"

#include "Game.h"
#include "GameGuiManager.h"
#include "GameScene.h"
#include "MainCharacter.h"
#include "RoleItem.h"
#include "GuiNpcMake.h"
#include "GuiNpcDialog.h"

#include "DlgZhanhunRecast.h"

namespace
{
	const int TABLE_FOCUSITEM			= 80;  // 装备格子
	const int STATIC_NEEDMONEY			= 117; // 价格
	const int TABLE_MATERIAL1			= 120; // 材料格子
	
	const int STATIC_RESULTINFO			= 125; // 说明提示

    const int TIM_EQUIP_COMPOSE_ZH1 =   124; // 战魂升级
}

CDlgZhanhunRecast::CDlgZhanhunRecast()
{
	FUNCTION_BEGIN;

	m_bCloseIsHide = true;
	m_bPutFocusItem = true;
	m_unSoulMountNeedMoney = 0;
	for (int i = 0; i < TOTAL_MATERIAL_TABLE; ++i)
	{
		m_pTableMaterials[i] = NULL;
	}

	FUNCTION_END;
}

CDlgZhanhunRecast::~CDlgZhanhunRecast()
{
	FUNCTION_BEGIN;


	FUNCTION_END;
}

void CDlgZhanhunRecast::OnPutFocusItem(bool bPut)
{
	m_bPutFocusItem = bPut;
}

void CDlgZhanhunRecast::OnResult(stResponsePropertyUserCmd* pCmd)
{
	if (!pCmd)
	{
		return;
	}

	switch(pCmd->kind)
	{
	case EQUIP_COMPOSE33: //战魂升级

		{
			DWORD dwColor = 0;
			if (pCmd->status == 0) //	123
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
			m_pStaResult->SetTextColor(dwColor);
			m_pStaResult->SetText(pCmd->info);
		}
		break;
	}
}

void CDlgZhanhunRecast::OnClose(void)
{
	GetGameGuiManager()->m_pDlgZhanhunRecast = NULL;
}

void CDlgZhanhunRecast::OnCreate()
{
	CGuiDialog::OnCreate();

	m_BackElements.resize(1);

	CGuiElement* pEmt = new CGuiElement;
	pEmt->FontColor.Current = COLOR_ARGB(255,200,200,0); //soke 显示页面名称颜色
	pEmt->iFont = 2;
	pEmt->TextureColor.Current = COLOR_ARGB(128,0,0,0);
	pEmt->dwTextFormat = DT_CENTER | DT_CENTER;
	m_BackElements[0] = pEmt;

	m_pStaNeedMoney = this->GetStatic(STATIC_NEEDMONEY);

	m_pTableFocusItem = this->GetTable(TABLE_FOCUSITEM);
	m_pTableFocusItem->m_iTableType = OBJECTCELLTYPE_RECAST_ZH;
	m_pTableFocusItem->m_iTableID = TABLE_FOCUSITEM;
	m_pTableFocusItem->m_EquipPosition = stPointI(0,MAKECELLTYPE_EQUIP);

	m_pTableMaterial1 = this->GetTable(TABLE_MATERIAL1);
	m_pTableMaterial1->m_iTableType = OBJECTCELLTYPE_RECAST_ZH;
	m_pTableMaterial1->m_iTableID = TABLE_MATERIAL1;
	m_pTableMaterial1->m_EquipPosition = stPointI(0,MAKECELLTYPE_MATERIAL1);
	m_pTableMaterials[0] = m_pTableMaterial1;

	m_pStaResult = this->GetStatic(STATIC_RESULTINFO);

}

bool CDlgZhanhunRecast::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	FUNCTION_BEGIN;

	switch (nEvent)
	{
	case EVENT_BUTTON_CLICKED:
		{
			switch (nID)
			{
				case 2: //sky 关闭
				{
					SetVisible(false);
				}
				break;
			}

			if (nID == TIM_EQUIP_COMPOSE_ZH1) ///<战魂升级
			{
				if (this->CanComposeEquip_zh1())
				{
				    this->SendEquipRecastMsg(RECAST_ZH_ITEM_COMPOSE1);
				}
			}
		}
		break;
	}

	return CGuiDialog::OnGuiEvent(nEvent,nID,pControl);
	FUNCTION_END;
}
//
//void CDlgZhanhunRecast::OnSetCloseLocation()
//{
//	FUNCTION_BEGIN;
//
//	CGuiDialog::OnSetCloseLocation();
//
//	if(m_pBtnClose)
//	{
//		stPointI pt(m_pBtnClose->GetLocation());
//		m_pBtnClose->SetLocation(pt.x-21, pt.y-40);
//	}
//
//	FUNCTION_END;
//}

HRESULT CDlgZhanhunRecast::OnRender(float fElapsedTime)
{
	FUNCTION_BEGIN;

	HRESULT hr = CGuiDialog::OnRender(fElapsedTime);

	m_aniSuccess.Render(m_x+195,m_y-168);//soke 装特效位置149

	return hr;

	FUNCTION_END;
}

//soke 战魂升级
bool CDlgZhanhunRecast::CanComposeEquip_zh1()
{
	int nMaterialNum = 0;
	std::vector<CRoleItem*> vecMaterialItems;
	CRoleItem* pRoleItem = this->GetFocusItem();
	if (pRoleItem)
	{
		vecMaterialItems.push_back(pRoleItem);
		nMaterialNum += vecMaterialItems.back()->GetObject()->dwNum;
	}

	for (int i = 0; i < TOTAL_MATERIAL_TABLE; ++i)
	{
		pRoleItem = this->GetMaterialItem(i);
		if (pRoleItem)
		{
			vecMaterialItems.push_back(pRoleItem);
			nMaterialNum += vecMaterialItems.back()->GetObject()->dwNum;
		}
	}

	CRoleItem* pItemUp = NULL;
	int nItemUp = 0;
	int nItemC4 = 0; //战魂升级石

	DWORD dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);
	m_pStaResult->SetTextColor(dwColor);

	for (int i = 0; i < vecMaterialItems.size(); ++i)
	{
		//sky 放入材料计数
		if (vecMaterialItems[i]->GetObjectID() == 1739) //战魂升级石
		{
			nItemC4++;
			continue;
		}

		if ((vecMaterialItems[i]->GetObjectBase()->dwUp > 0) &&(3001 > vecMaterialItems[i]->GetObject()->canxin1))
		{
			++nItemUp;
			pItemUp = vecMaterialItems[i];
			continue;
		}

		m_pStaResult->SetText("存在非升级类道具");
		return false;
	}

	//sky 放入物品数量判断
	if ((nItemUp + nItemC4 ) < vecMaterialItems.size())
	{
		m_pStaResult->SetText("请放入对应的物品");
		return false;
	}

	//sky 放入装备
	if (nItemUp == 1)
	{
		if (!(pItemUp->GetItemType() ==80|| (pItemUp->GetItemType() >=88 && pItemUp->GetItemType() <=91)|| (pItemUp->GetItemType() >= 93 && pItemUp->GetItemType() <= 98) || pItemUp->GetItemType() ==140 ))
		{
			m_pStaResult->SetText("非附件不能在此升级战魂");
			return false;
		}
		else
		{
			if (!(pItemUp->GetObject()->dwObjectID ==3830 || pItemUp->GetObject()->dwObjectID ==3865 || pItemUp->GetObject()->dwObjectID ==3920 ||
				pItemUp->GetObject()->dwObjectID ==3921 || pItemUp->GetObject()->dwObjectID ==3922 || pItemUp->GetObject()->dwObjectID ==3930|| 
				pItemUp->GetObject()->dwObjectID ==3931 || (pItemUp->GetObject()->dwObjectID >=3900 && pItemUp->GetObject()->dwObjectID <=3904)
				|| pItemUp->GetObject()->dwObjectID ==1321|| pItemUp->GetObject()->dwObjectID ==1323|| pItemUp->GetObject()->dwObjectID ==15605
				|| pItemUp->GetObject()->dwObjectID ==18005|| pItemUp->GetObject()->dwObjectID ==18010|| pItemUp->GetObject()->dwObjectID ==18015
				|| pItemUp->GetObject()->dwObjectID ==18020|| pItemUp->GetObject()->dwObjectID ==18025))
			{
				m_pStaResult->SetText("非满级附件不能升级战魂");
				return false;
			}

			if (pItemUp->GetObjectID()== 3830 && pItemUp->GetObject()->dmedalt < 100)
			{
				m_pStaResult->SetText("非满级段位勋章不能在此升级战魂");
				return false;
			}
			else if (pItemUp->GetObjectID()== 3865 && pItemUp->GetObject()->retain1 < 100)
			{
				m_pStaResult->SetText("非满级寒冰麒麟不能在此升级战魂");
				return false;
			}
			else if ((pItemUp->GetObject()->dwObjectID >=3900 && pItemUp->GetObject()->dwObjectID <=3904) && pItemUp->GetObject()->retain4 < 1800)
			{
				m_pStaResult->SetText("非满级天使之翼不能在此升级战魂");
				return false;
			}
			else if ((pItemUp->GetItemType() >=94 && pItemUp->GetItemType()<=96) && pItemUp->GetObject()->retain5 < 600)
			{
				m_pStaResult->SetText("非满级神兵不能在此升级战魂");
				return false;
			}
			else if ((pItemUp->GetItemType() ==97 || pItemUp->GetItemType()==98) && pItemUp->GetObject()->retain6 < 600)
			{
				m_pStaResult->SetText("非满级神兽不能在此升级战魂");
				return false;
			}
			else if ((pItemUp->GetItemType() ==88) && pItemUp->GetObject()->retain8 < 4000)
			{
				m_pStaResult->SetText("非满级尚方宝剑不能在此升级战魂");
				return false;
			}
			else if ((pItemUp->GetItemType() ==89) && pItemUp->GetObject()->retain8 < 3000)
			{
				m_pStaResult->SetText("非满级紫金龙袍不能在此升级战魂");
				return false;
			}
			else if((pItemUp->GetObject()->dwObjectID == 18005 || pItemUp->GetObject()->dwObjectID == 18010 || pItemUp->GetObject()->dwObjectID == 18015 || pItemUp->GetObject()->dwObjectID == 18020 || pItemUp->GetObject()->dwObjectID == 18025)&& pItemUp->GetObject()->upgrade < 200)
			{
				m_pStaResult->SetText("非满级护身符不能在此升级战魂");
				return false;
			}
			else if ((pItemUp->GetItemType() ==140) && pItemUp->GetObject()->retain9 < 4500)
			{
				m_pStaResult->SetText("非满级马牌不能在此升级战魂");
				return false;
			}

			if (pItemUp->GetObject()->dwObjectID ==3830 || pItemUp->GetObject()->dwObjectID ==3865 || pItemUp->GetObject()->dwObjectID ==3920 ||
				pItemUp->GetObject()->dwObjectID ==3921 || pItemUp->GetObject()->dwObjectID ==3922)
			{
				//sky 根据龙槽等级判断需要的解封材料
				if (pItemUp->GetObject()->canxin1 >= 2000 )
				{
					m_pStaResult->SetText("当前附件战魂等级已经最高");
					return false;
				}
				else if (pItemUp->GetObject()->canxin1 < 2000 && nItemC4 == 0)
				{
					m_pStaResult->SetText("当前附件战魂升级需要战魂升级石");
					return false;
				}
			}
			else if ( pItemUp->GetObject()->dwObjectID ==3930|| pItemUp->GetObject()->dwObjectID ==3931 || (pItemUp->GetObject()->dwObjectID >=3900 && pItemUp->GetObject()->dwObjectID <=3904))
			{
				//sky
				if (pItemUp->GetObject()->canxin1 >= 1000 )
				{
					m_pStaResult->SetText("当前附件战魂等级已经最高");
					return false;
				}
				else if (pItemUp->GetObject()->canxin1 < 1000 && nItemC4 == 0)
				{
					m_pStaResult->SetText("当前附件战魂升级需要战魂升级石");
					return false;
				}
			}
			else if ( pItemUp->GetItemType() ==88 ) //尚方宝剑
			{
				//sky
				if (pItemUp->GetObject()->canxin1 >= 2000 )
				{
					m_pStaResult->SetText("当前附件战魂等级已经最高");
					return false;
				}
				else if (pItemUp->GetObject()->canxin1 < 2000 && nItemC4 == 0)
				{
					m_pStaResult->SetText("当前附件战魂升级需要战魂升级石");
					return false;
				}
			}
			else if ( pItemUp->GetItemType() ==89 ) //紫金龙袍
			{
				//sky
				if (pItemUp->GetObject()->canxin1 >= 1000 )
				{
					m_pStaResult->SetText("当前附件战魂等级已经最高");
					return false;
				}
				else if (pItemUp->GetObject()->canxin1 < 1000 && nItemC4 == 0)
				{
					m_pStaResult->SetText("当前附件战魂升级需要战魂升级石");
					return false;
				}
			}
			else if(pItemUp->GetObject()->dwObjectID == 18005 || pItemUp->GetObject()->dwObjectID == 18010 || pItemUp->GetObject()->dwObjectID == 18015 || pItemUp->GetObject()->dwObjectID == 18020 || pItemUp->GetObject()->dwObjectID == 18025)
			{
				//sky
				if (pItemUp->GetObject()->canxin1 >= 3000 )
				{
					m_pStaResult->SetText("当前附件战魂等级已经最高");
					return false;
				}
				else if (pItemUp->GetObject()->canxin1 < 3000 && nItemC4 == 0)
				{
					m_pStaResult->SetText("当前附件战魂升级需要战魂升级石");
					return false;
				}
			}
			else if ( pItemUp->GetItemType() ==140 ) //马牌
			{
				//sky
				if (pItemUp->GetObject()->canxin1 >= 3000 )
				{
					m_pStaResult->SetText("当前附件战魂等级已经最高");
					return false;
				}
				else if (pItemUp->GetObject()->canxin1 < 3000 && nItemC4 == 0)
				{
					m_pStaResult->SetText("当前附件战魂升级需要战魂升级石");
					return false;
				}
			}
		}
	}

	//sky 没放入装备
	if ((nItemUp != 1) || (( nItemC4 ) < 1))
	{
		m_pStaResult->SetText("战魂升级需要战魂升级石");
		return false;
	}

	if (pItemUp->GetObject()->dwObjectID ==3830 || pItemUp->GetObject()->dwObjectID ==3865 || pItemUp->GetObject()->dwObjectID ==3920 ||
		pItemUp->GetObject()->dwObjectID ==3921 || pItemUp->GetObject()->dwObjectID ==3922)
	{
		//sky 战魂最高等级
		if (pItemUp->GetObject()->canxin1 >= 2000)
		{
			m_pStaResult->SetText("当前附件战魂等级已经最高");
			return false;
		}
	}
	if ( pItemUp->GetObject()->dwObjectID ==3930|| pItemUp->GetObject()->dwObjectID ==3931 || (pItemUp->GetObject()->dwObjectID >=3900 && pItemUp->GetObject()->dwObjectID <=3904))
	{
		//sky 战魂最高等级
		if (pItemUp->GetObject()->canxin1 >= 1000)
		{
			m_pStaResult->SetText("当前附件战魂等级已经最高");
			return false;
		}
	}

	if ( pItemUp->GetItemType() ==88 ) //尚方宝剑
	{
		//sky
		if (pItemUp->GetObject()->canxin1 >= 2000 )
		{
			m_pStaResult->SetText("当前附件战魂等级已经最高");
			return false;
		}
	}
	if ( pItemUp->GetItemType() ==89 ) //紫金龙袍
	{
		//sky
		if (pItemUp->GetObject()->canxin1 >= 1000 )
		{
			m_pStaResult->SetText("当前附件战魂等级已经最高");
			return false;
		}
	}
	if(pItemUp->GetObject()->dwObjectID == 18005 || pItemUp->GetObject()->dwObjectID == 18010 || pItemUp->GetObject()->dwObjectID == 18015 || pItemUp->GetObject()->dwObjectID == 18020 || pItemUp->GetObject()->dwObjectID == 18025)
	{
		//sky
		if (pItemUp->GetObject()->canxin1 >= 3000 )
		{
			m_pStaResult->SetText("当前附件战魂等级已经最高");
			return false;
		}
	}
	if ( pItemUp->GetItemType() ==140 ) //马牌
	{
		//sky
		if (pItemUp->GetObject()->canxin1 >= 3000 )
		{
			m_pStaResult->SetText("当前附件战魂等级已经最高");
			return false;
		}
	}

	char szInfo[MAX_PATH] = { 0 };
	sprintf(szInfo, "-工资：5两");
	m_pStaNeedMoney->SetText(szInfo);

	return true;
}

CRoleItem* CDlgZhanhunRecast::GetFocusItem()
{
	CRoleItem* pRoleItem = NULL;
	CGuiItemCell* pItemCell = m_pTableFocusItem->GetItemCell(0, 0);
	if (pItemCell)
	{
		pRoleItem= static_cast<CRoleItem*>(pItemCell->m_pItemData);
	}

	return pRoleItem;
}

CRoleItem* CDlgZhanhunRecast::GetMaterialItem(int index)
{
	CRoleItem* pRoleItem = NULL;
	CGuiItemCell* pItemCell = m_pTableMaterials[index]->GetItemCell(0, 0);
	if (pItemCell)
	{
		pRoleItem = static_cast<CRoleItem*>(pItemCell->m_pItemData);
	}

	return pRoleItem;
}

bool CDlgZhanhunRecast::IsEquipItem(enumItemType eType)
{
	if (eType == 27|| eType == 49 ||eType == 140|| eType == 80|| (eType >= 88 && eType <= 91) || (eType >= 93 && eType <= 98))
	{
		return true;
	}

	return false;
}

void CDlgZhanhunRecast::SendEquipRecastMsg(RecastAction_zh eAction)
{
	CGuiItemCell* pItemCell = m_pTableFocusItem->GetItemCell(0, 0);
	if (!pItemCell)
	{
		return;
	}

	CRoleItem* pRoleItem= static_cast<CRoleItem*>(pItemCell->m_pItemData);
	if (pRoleItem)
	{
		ItemRecast_zhPropertyUserCmd cmd;
		cmd.act = eAction;
		switch (eAction)
		{

		case RECAST_ZH_ITEM_COMPOSE1:
			cmd.itemUID = pRoleItem->GetObject()->qwThisID;
			break;
		}

		SEND_USER_CMD(cmd);
	}
}

//soke 战魂升级
void CDlgZhanhunRecast::RefreshEquipCompose_zh1()
{
	bool bShow = false;
	CRoleItem* pRoleItem = this->GetFocusItem();
	if (pRoleItem && m_bPutFocusItem)
	{
		bShow = true;
	}

	m_pStaNeedMoney->SetVisible(bShow);
	if (bShow)
	{
		bool bEnable = this->CanComposeEquip_zh1();
		this->GetButton(TIM_EQUIP_COMPOSE_ZH1)->SetEnabled(bEnable);

		if (bEnable)
		{
			std::stringstream ss;
			ss << "需要工资：5两";
			m_pStaNeedMoney->SetText(ss.str().c_str());
			m_pStaResult->SetText("");
		}
		else
		{
			m_pStaNeedMoney->SetText("");
		}
	}
}


