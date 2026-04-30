#include "public.h"

#include "GuiWare.h"
#include "MallDataMgr.h"
#include "GameScene.h"
#include "RoleItem.h"
#include "MainCharacter.h"


#include "DlgMall.h"

namespace
{
	const int BUTTON_CLOSE				= 1; //关闭按钮
	const int BUTTON_ACCOUNTRECHARGE	= 2; //充值按钮
	const int BUTTON_PAGEUP				= 3; //上一页
	const int BUTTON_PAGEDOWN			= 4; //下一页
	const int LISTBOX_WAREREGION		= 5; //引导页面
	const int STATIC_CURPAGE			= 6; //翻页1/2那个
	const int STATIC_CHONGZHI			= 7; //剩余点数

	//by=>friday
	const int BUTTON_CATEGORY_LEFT      = 98; //类别左滑按钮
	const int BUTTON_CATEGORY_RIGHT     = 99; //类别右滑按钮

	const int GUIWARE_BASE_ID			= 11;
	const int GUIWARE_BASE_ID_BUTTON	= 14;
	const int GUIWARE_CONTROLNUM		= 5;

	const std::string STR_WAREREGION[WAREREGION_COUNT] =
	{
		std::string("商品限购区"),
	    std::string("热卖商品区"), //by=>friday
		std::string("便利道具区"), //by=>friday
		std::string("坐骑系列区"), //by=>friday
		std::string("精品材料区"), //by=>friday
		std::string("其他商品区"), //by=>friday
		std::string("VIP专区"),    //by=>friday
		std::string("时装专区"),   //by=>friday
		std::string("武器专区"),   //by=>friday
		std::string("宠物专区"),   //by=>friday
	};
}

CDlgMall::CDlgMall()
{
	FUNCTION_BEGIN;

	for (int i = 0; i < MAX_WARE_PER_PAGE; ++i)
	{
		m_pGuiWare[i] = NULL;
	}

	m_eCurRegion = WAREREGION_DISCOUNT;
	m_nCurPage = 1;
	
	//by=>friday
	m_nCategoryOffset = 0; // 类别滚动偏移量初始化为0

	FUNCTION_END;
}

CDlgMall::~CDlgMall()
{
	FUNCTION_BEGIN;

	for (int i = 0; i < MAX_WARE_PER_PAGE; ++i)
	{
		SAFE_DELETE(m_pGuiWare[i]);
	}

	FUNCTION_END;
}

void CDlgMall::Refresh()
{
	FUNCTION_BEGIN;

	this->ReleaseWares();

	this->InitWares();

	// 刷新当前页号显示
	if (m_pStaCurPage)
	{
		unsigned int unTotalPage = this->GetMaxPage();

		std::stringstream ss;
		ss << m_nCurPage << "/" << unTotalPage;
		m_pStaCurPage->SetText(ss.str().c_str());
	}

	FUNCTION_END
}

void CDlgMall::OnClose(void)
{
	FUNCTION_BEGIN;

	this->ReleaseWares();
	GetGameGuiManager()->m_pDlgMall = NULL;

	FUNCTION_END
}

void CDlgMall::OnCreate()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnCreate();

	m_pListBoxWareRegion = this->GetListBox(LISTBOX_WAREREGION);
	if (m_pListBoxWareRegion)
	{
		for (int i = 0; i < WAREREGION_COUNT; ++i)
		{
			m_pListBoxWareRegion->AddItem("", NULL);
			m_pListBoxWareRegion->SetItemText(i, 0, STR_WAREREGION[i].c_str(), -1, GuiListBoxColFmt_Center);
		}
		m_pListBoxWareRegion->SetCurItem(0);
		m_pListBoxWareRegion->SetTextHeight(27);
		
		//by=>friday 完全隐藏ListBox控件，使用按钮导航
		m_pListBoxWareRegion->SetVisible(false);
	}

	for (int i = 0; i < MAX_WARE_PER_PAGE; ++i)
	{
		CGuiImage* pImage = this->GetImage(GUIWARE_BASE_ID + GUIWARE_CONTROLNUM * i);
		CGuiButton* pBtn = this->GetButton(GUIWARE_BASE_ID + GUIWARE_CONTROLNUM * i + 3);
		CGuiStatic* pSta1 = this->GetStatic(GUIWARE_BASE_ID + GUIWARE_CONTROLNUM * i + 1);
		CGuiStatic* pSta2 = this->GetStatic(GUIWARE_BASE_ID + GUIWARE_CONTROLNUM * i + 4);
		CGuiTable* pTable = this->GetTable(GUIWARE_BASE_ID + GUIWARE_CONTROLNUM * i + 2);
		pTable->m_iTableType = OBJECTCELLTYPE_MALL;
		pTable->m_EquipPosition = stPointI(0, i);
		m_pGuiWare[i] = new CGuiWare(pImage, pBtn, pSta1, pSta2, pTable);
	}

	m_pStaCurPage = this->GetStatic(STATIC_CURPAGE);
	m_pStaChongzhi = this->GetStatic(STATIC_CHONGZHI); //充值点
	
	//by=>friday
	// 初始化类别标题文本控件
	m_pStaCategory = this->GetStatic(256);
	if (m_pStaCategory)
	{
		m_pStaCategory->SetText(STR_WAREREGION[m_eCurRegion].c_str());
	}
	
	// 确保左右滑动按钮可见
	CGuiButton* pLeftBtn = this->GetButton(BUTTON_CATEGORY_LEFT);
	CGuiButton* pRightBtn = this->GetButton(BUTTON_CATEGORY_RIGHT);
	
	if (pLeftBtn)
	{
		pLeftBtn->SetVisible(true);
		pLeftBtn->SetEnabled(false); // 初始时不能向左滑动
	}
	
	if (pRightBtn)
	{
		pRightBtn->SetVisible(true);
		// 判断是否需要启用右滑按钮
		const int BUTTON_WIDTH = 85;   // 保持按钮宽度为85
		const int BUTTON_SPACING = 45; // 将按钮间距增加到45
		const int VISIBLE_LEFT = 85;
		const int VISIBLE_RIGHT = 845;
		int maxVisibleButtons = (VISIBLE_RIGHT - VISIBLE_LEFT) / (BUTTON_WIDTH + BUTTON_SPACING);
		if (maxVisibleButtons > WAREREGION_COUNT)
			maxVisibleButtons = WAREREGION_COUNT;
		pRightBtn->SetEnabled(WAREREGION_COUNT > maxVisibleButtons);
	}
	
	// 初始化类别按钮位置
	UpdateCategoryButtonsPosition();

	FUNCTION_END;
}

bool CDlgMall::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	FUNCTION_BEGIN;

	switch (nEvent)
	{
	case EVENT_BUTTON_CLICKED:
		{
			int nIndex = (nID - GUIWARE_BASE_ID_BUTTON) / 5;
			int nMode = (nID - GUIWARE_BASE_ID_BUTTON) % 5;
			if (nMode == 0 && nIndex >= 0 && nIndex <= 14) // 新商城点击购买物品按钮 10个就写9个
			{
				int nSerialNum = (m_nCurPage - 1) * MAX_WARE_PER_PAGE + nIndex;
				DWORD dwID = GetMallDataMgr().GetWareID(m_eCurRegion, nSerialNum);

				stBuyObjectMallUserCmd cmd;
				cmd.dwObjectID = dwID;
				cmd.dwNum = 1;
				cmd.itemLevel = 0;

				SEND_USER_CMD(cmd);
			}

			switch (nID)
			{
			case BUTTON_CLOSE:
				this->Close();
				break;
			case BUTTON_ACCOUNTRECHARGE:
				break;
			case BUTTON_PAGEUP:
				{
					unsigned int unNum = GetMallDataMgr().GetWaresNum(m_eCurRegion);
					unsigned int unTotalPage = this->GetMaxPage();
					if (m_nCurPage > 1)
					{
						--m_nCurPage;
					}

					this->Refresh();
				}
				break;
			case BUTTON_PAGEDOWN:
				{
					unsigned int unNum = GetMallDataMgr().GetWaresNum(m_eCurRegion);
					unsigned int unTotalPage = this->GetMaxPage();
					if (m_nCurPage < unTotalPage)
					{
						++m_nCurPage;
					}

					this->Refresh();
				}
				break;
			//by=>friday 类别左右滑动按钮事件
			case BUTTON_CATEGORY_LEFT:
				{
					if (m_nCategoryOffset > 0)
					{
						m_nCategoryOffset--;
						UpdateCategoryButtonsPosition();
					}
				}
				break;
			case BUTTON_CATEGORY_RIGHT:
				{
					// 计算最大可滑动偏移量
					const int BUTTON_WIDTH = 85;
					const int BUTTON_SPACING = 30;
					const int VISIBLE_LEFT = 85;
					const int VISIBLE_RIGHT = 845;
					int maxVisibleButtons = (VISIBLE_RIGHT - VISIBLE_LEFT) / (BUTTON_WIDTH + BUTTON_SPACING);
					if (maxVisibleButtons > WAREREGION_COUNT)
						maxVisibleButtons = WAREREGION_COUNT;
					int maxOffset = WAREREGION_COUNT - maxVisibleButtons;
					if (maxOffset < 0) maxOffset = 0;
					
					if (m_nCategoryOffset < maxOffset)
					{
						m_nCategoryOffset++;
						UpdateCategoryButtonsPosition();
					}
				}
				break;
			//by=>friday 类别按钮点击事件
			case 101: // 商品限购区
				{
					m_eCurRegion = WAREREGION_DISCOUNT;
					m_nCurPage = 1;
					// 更新类别标题
					if (m_pStaCategory)
					{
						m_pStaCategory->SetText(STR_WAREREGION[m_eCurRegion].c_str());
					}
					this->Refresh();
				}
				break;
			case 102: // 热卖商品区
				{
					m_eCurRegion = WAREREGION_HOT;
					m_nCurPage = 1;
					// 更新类别标题
					if (m_pStaCategory)
					{
						m_pStaCategory->SetText(STR_WAREREGION[m_eCurRegion].c_str());
					}
					this->Refresh();
				}
				break;
			case 103: // 便利道具区
				{
					m_eCurRegion = WAREREGION_PROPS;
					m_nCurPage = 1;
					// 更新类别标题
					if (m_pStaCategory)
					{
						m_pStaCategory->SetText(STR_WAREREGION[m_eCurRegion].c_str());
					}
					this->Refresh();
				}
				break;
			case 104: // 坐骑系列区
				{
					m_eCurRegion = WAREREGION_RIDE;
					m_nCurPage = 1;
					// 更新类别标题
					if (m_pStaCategory)
					{
						m_pStaCategory->SetText(STR_WAREREGION[m_eCurRegion].c_str());
					}
					this->Refresh();
				}
				break;
			case 105: // 精品材料区
				{
					m_eCurRegion = WAREREGION_MATERIAL;
					m_nCurPage = 1;
					// 更新类别标题
					if (m_pStaCategory)
					{
						m_pStaCategory->SetText(STR_WAREREGION[m_eCurRegion].c_str());
					}
					this->Refresh();
				}
				break;
			case 106: // 其他商品区
				{
					m_eCurRegion = WAREREGION_OTHER;
					m_nCurPage = 1;
					// 更新类别标题
					if (m_pStaCategory)
					{
						m_pStaCategory->SetText(STR_WAREREGION[m_eCurRegion].c_str());
					}
					this->Refresh();
				}
				break;
			case 107: // VIP专区
				{
					m_eCurRegion = WAREREGION_VIP;
					m_nCurPage = 1;
					// 更新类别标题
					if (m_pStaCategory)
					{
						m_pStaCategory->SetText(STR_WAREREGION[m_eCurRegion].c_str());
					}
					this->Refresh();
				}
				break;
			case 108: // 时装专区
				{
					m_eCurRegion = WAREREGION_FASHION;
					m_nCurPage = 1;
					// 更新类别标题
					if (m_pStaCategory)
					{
						m_pStaCategory->SetText(STR_WAREREGION[m_eCurRegion].c_str());
					}
					this->Refresh();
				}
				break;
			case 109: // 武器专区
				{
					m_eCurRegion = WAREREGION_WEAPON;
					m_nCurPage = 1;
					// 更新类别标题
					if (m_pStaCategory)
					{
						m_pStaCategory->SetText(STR_WAREREGION[m_eCurRegion].c_str());
					}
					this->Refresh();
				}
				break;
			case 110: // 宠物专区
				{
					m_eCurRegion = WAREREGION_PET;
					m_nCurPage = 1;
					// 更新类别标题
					if (m_pStaCategory)
					{
						m_pStaCategory->SetText(STR_WAREREGION[m_eCurRegion].c_str());
					}
					this->Refresh();
				}
				break;
			}
		}
		break;
	case EVENT_LISTBOX_SELECTION: //by=>friday
		{
			if (LISTBOX_WAREREGION == nID)
			{
				// 保留ListBox的选择事件处理，但由于ListBox已被隐藏，此处代码不会执行
				int nIndex = m_pListBoxWareRegion->GetCurItem();
				if (nIndex != static_cast<int>(m_eCurRegion))
				{
					m_eCurRegion = static_cast<WAREREGION_CLASS>(nIndex);
					m_nCurPage = 1;
					// 更新类别标题
					if (m_pStaCategory)
					{
						m_pStaCategory->SetText(STR_WAREREGION[m_eCurRegion].c_str());
					}
					this->Refresh();
				}
			}
		}
		break;
	}

	return CGuiDialog::OnGuiEvent(nEvent,nID,pControl);
	FUNCTION_END;
}

HRESULT CDlgMall::OnRender(float fElapsedTime)
{
	FUNCTION_BEGIN;

	HRESULT hr = CGuiDialog::OnRender(fElapsedTime);

	// 刷新当前金钱数
	DWORD dwChongzhi = GetScene()->GetMainCharacter()->GetProperty()->chongzhi; //充值点
	m_pStaChongzhi->SetText(Chongzhi2String(dwChongzhi));

	return hr;

	FUNCTION_END;
}

unsigned int CDlgMall::GetMaxPage()
{
	unsigned int unTotalPage = 0;
	unsigned int unNum = GetMallDataMgr().GetWaresNum(m_eCurRegion);
	if (unNum != 0)
	{
		unTotalPage = (unNum - 1) / MAX_WARE_PER_PAGE + 1;
	}
	
	return unTotalPage;
}

void CDlgMall::InitWares()
{
	//by=>friday
	// 先隐藏所有商品相关控件
	for (int i = 0; i < MAX_WARE_PER_PAGE; ++i)
	{
		// 隐藏商品控件
		if (m_pGuiWare[i])
		{
			m_pGuiWare[i]->Show(false);
			
			// 同时隐藏商品框
			if (i < 5) // 第一列
			{
				for (int j = 0; j < 3; j++)
				{
					int id = 200 + i*3 + j;
					CGuiStatic* pSta = this->GetStatic(id);
					if (pSta) pSta->SetVisible(false);
				}
			}
			else if (i < 10) // 第二列
			{
				for (int j = 0; j < 3; j++)
				{
					int id = 215 + (i-5)*3 + j;
					CGuiStatic* pSta = this->GetStatic(id);
					if (pSta) pSta->SetVisible(false);
				}
			}
			else if (i < 15) // 第三列
			{
				for (int j = 0; j < 3; j++)
				{
					int id = 230 + (i-10)*3 + j;
					CGuiStatic* pSta = this->GetStatic(id);
					if (pSta) pSta->SetVisible(false);
				}
			}
		}
	}

	// 获取当前页的商品信息
	unsigned int unNum = GetMallDataMgr().GetWaresNum(m_eCurRegion);
	unsigned int unTotalPage = this->GetMaxPage();

	unsigned int unStart = (m_nCurPage - 1) * MAX_WARE_PER_PAGE;
	unsigned int unEnd = unNum;
	if (unEnd > (unStart + MAX_WARE_PER_PAGE))
	{
		unEnd = unStart + MAX_WARE_PER_PAGE;
	}

	// 记录已经显示过的商品ID，防止重复显示
	std::set<DWORD> shownItems;
	
	// 商品展示位置索引
	int showIndex = 0;

	// 显示当前页的商品
	for (unsigned int i = unStart; i < unEnd; ++i)
	{
		// 获取商品ID
		DWORD dwID = GetMallDataMgr().GetWareID(m_eCurRegion, i);
		
		// 检查此商品是否已经显示过，如果是则跳过
		if (shownItems.find(dwID) != shownItems.end())
		{
			continue;
		}
		
		// 获取商品信息
		ObjectBase_t* pObject = GetObjectBase(dwID);
		if (pObject && showIndex < MAX_WARE_PER_PAGE)
		{
			// 记录此商品ID已经显示
			shownItems.insert(dwID);
			
			// 显示商品
			if (m_pGuiWare[showIndex])
			{
				// 设置商品信息
				m_pGuiWare[showIndex]->SetPrice(Chongzhi2String(pObject->price));
				m_pGuiWare[showIndex]->SetName(pObject->strName);
				m_pGuiWare[showIndex]->Show(true);

				// 显示描述文字
				if (showIndex < 5) // 第一列
				{
					int id1 = 200 + showIndex*3;
					int id2 = 201 + showIndex*3;
					int id3 = 202 + showIndex*3;
					
					CGuiStatic* pSta1 = this->GetStatic(id1);
					CGuiStatic* pSta2 = this->GetStatic(id2);
					CGuiStatic* pSta3 = this->GetStatic(id3);
					
					if (pSta1) pSta1->SetVisible(true);
					if (pSta2) pSta2->SetVisible(true);
					if (pSta3) pSta3->SetVisible(true);
				}
				else if (showIndex < 10) // 第二列
				{
					int id1 = 215 + (showIndex-5)*3;
					int id2 = 216 + (showIndex-5)*3;
					int id3 = 217 + (showIndex-5)*3;
					
					CGuiStatic* pSta1 = this->GetStatic(id1);
					CGuiStatic* pSta2 = this->GetStatic(id2);
					CGuiStatic* pSta3 = this->GetStatic(id3);
					
					if (pSta1) pSta1->SetVisible(true);
					if (pSta2) pSta2->SetVisible(true);
					if (pSta3) pSta3->SetVisible(true);
				}
				else if (showIndex < 15) // 第三列
				{
					int id1 = 230 + (showIndex-10)*3;
					int id2 = 231 + (showIndex-10)*3;
					int id3 = 232 + (showIndex-10)*3;
					
					CGuiStatic* pSta1 = this->GetStatic(id1);
					CGuiStatic* pSta2 = this->GetStatic(id2);
					CGuiStatic* pSta3 = this->GetStatic(id3);
					
					if (pSta1) pSta1->SetVisible(true);
					if (pSta2) pSta2->SetVisible(true);
					if (pSta3) pSta3->SetVisible(true);
				}

				// 添加物品到游戏角色
				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				strncpy(cmd.object.strName,pObject->strName,sizeof(cmd.object.strName));
				cmd.object.dwObjectID = pObject->dwID;
				cmd.object.qwThisID = INVALID_THISID;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_MALL;
				cmd.object.pos.dwTableID = 0;
				cmd.object.pos.x = 0;
				cmd.object.pos.y = showIndex;
				cmd.object.color = -1;
				cmd.object.dwNum = 1;
				GetScene()->GetMainCharacter()->AddItem(&cmd);

				// 增加showIndex，指向下一个位置
				showIndex++;
			}
		}
	}
}

void CDlgMall::ReleaseWares()
{
	for (int i = 0; i < MAX_WARE_PER_PAGE; ++i)
	{
		GetScene()->GetMainCharacter()->RemoveItem(OBJECTCELLTYPE_MALL, stPointI(0, i));
	}
}

//by=>friday
void CDlgMall::UpdateCategoryButtonsPosition()
{
	// 类别按钮基础位置
	const int BASE_X = 75;  // 从85改为75，整体左移10像素
	const int BASE_Y = 66;  // 将Y坐标从86调回到66，使按钮向上移动
	const int BUTTON_WIDTH = 85;   // 保持按钮宽度为85
	const int BUTTON_SPACING = 45; // 将按钮间距增加到45
	const int VISIBLE_LEFT = 75;   // 可见区域左边界也相应左移10像素
	const int VISIBLE_RIGHT = 845; // 可见区域右边界不变
	
	// 计算可见按钮数量（一行最多显示多少个按钮）
	int maxVisibleButtons = (VISIBLE_RIGHT - VISIBLE_LEFT) / (BUTTON_WIDTH + BUTTON_SPACING);
	if (maxVisibleButtons > WAREREGION_COUNT)
		maxVisibleButtons = WAREREGION_COUNT;
	
	// 按钮ID范围：101-110
	for (int i = 0; i < WAREREGION_COUNT; i++)
	{
		int buttonID = 101 + i;
		CGuiButton* pButton = this->GetButton(buttonID);
		if (pButton)
		{
			// 计算按钮X坐标，考虑偏移量
			int xPos = BASE_X + (i - m_nCategoryOffset) * (BUTTON_WIDTH + BUTTON_SPACING);
			
			// 设置按钮位置
			pButton->SetLocation(xPos, BASE_Y);
			
			// 根据位置决定是否可见
			// 只有在可见区域内的按钮才显示
			bool visible = (xPos >= VISIBLE_LEFT - 5 && xPos + BUTTON_WIDTH <= VISIBLE_RIGHT + 5);
			pButton->SetVisible(visible);
		}
	}
	
	// 根据偏移量控制左右滑动按钮的可见性
	CGuiButton* pLeftBtn = this->GetButton(BUTTON_CATEGORY_LEFT);
	CGuiButton* pRightBtn = this->GetButton(BUTTON_CATEGORY_RIGHT);
	
	if (pLeftBtn)
	{
		// 确保左滑按钮始终可见，只是在无法滑动时禁用
		pLeftBtn->SetVisible(true);
		pLeftBtn->SetEnabled(m_nCategoryOffset > 0);
	}
	
	if (pRightBtn)
	{
		// 计算最大可滑动偏移量
		int maxOffset = WAREREGION_COUNT - maxVisibleButtons;
		if (maxOffset < 0) maxOffset = 0;
		
		// 确保右滑按钮始终可见，只是在无法滑动时禁用
		pRightBtn->SetVisible(true);
		pRightBtn->SetEnabled(m_nCategoryOffset < maxOffset);
	}
}

