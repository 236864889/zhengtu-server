#include "public.h"
#include "miniusercommand.h"
#include "./GameGuiManager.h"
#include "./GuiBoardGameDlg.h"
#include "../gui/include/GuiManager.h"
#include "../gui/include/GuiButton.h"
#include "./Game.h"
#include "LordStrikeManager.h"
#include "LordStrikeLobby.h"
#include "SafetyUser.h"
#include "./guinpcdialog.h"
#include "./CountryInfo.h"
#include "./GuiItem.h"
#include "./Country.h"
#include "./GuiTop.h"
#include "./GuiTaiMiaoTop.h"
#include "./GuiHalofFameTop.h"
///////////////////////////////////////////////////////////////////////////////
#define ID_BUTTON_GJZHSJ  3    // 个人名次排行榜
#define ID_BUTTON_JZBH    4    // 个人战力排行榜
#define ID_BUTTON_GJYX    5    // 今日护国排行榜
#define ID_BUTTON_XHPH    6    // 昨日护国排行榜
#define ID_BUTTON_GRMC    7    // 个人积分排行榜


#define ID_BUTTON_CLOSE   1
///////////////////////////////////////////////////////////////////////////////

CGuiBoardGameDlg::CGuiBoardGameDlg()
{
	FUNCTION_BEGIN;

	m_bSmDialog = true;

	FUNCTION_END;
}

CGuiBoardGameDlg::~CGuiBoardGameDlg()
{
	FUNCTION_BEGIN;


	FUNCTION_END;
}

void CGuiBoardGameDlg::OnCreate(void)
{
	FUNCTION_BEGIN;

	CGuiDialog::OnCreate();

	CGuiButton* pBtnRank = GetButton(ID_BUTTON_GJZHSJ);
	CGuiButton* pBtnZhanLi = GetButton(ID_BUTTON_JZBH);
	CGuiButton* pBtnToday = GetButton(ID_BUTTON_GJYX);
	CGuiButton* pBtnYesterday = GetButton(ID_BUTTON_XHPH);
	CGuiButton* pBtnTicket = GetButton(ID_BUTTON_GRMC);

	if(pBtnRank)
	{
		pBtnRank->SetText("个人名次排行榜");
	}
	if(pBtnZhanLi)
	{
		pBtnZhanLi->SetText("个人战力排行榜");
	}
	if(pBtnToday)
	{
		pBtnToday->SetText("今日护国排行榜");
	}
	if(pBtnYesterday)
	{
		pBtnYesterday->SetText("昨日护国排行榜");
	}
	if(!pBtnTicket && pBtnYesterday)
	{
		int nSpace = 4;
		int nX = pBtnYesterday->GetX();
		int nY = pBtnYesterday->GetY() + pBtnYesterday->GetHeight() + nSpace;
		int nWidth = pBtnYesterday->GetWidth();
		int nHeight = pBtnYesterday->GetHeight();
		AddButton(ID_BUTTON_GRMC, "个人积分排行榜", nX, nY, nWidth, nHeight, 0, false, &pBtnTicket);
		if(nY + nHeight + nSpace > GetHeight())
		{
			SetSize(GetWidth(), nY + nHeight + nSpace);
		}
	}
	if(pBtnTicket)
	{
		pBtnTicket->SetText("个人积分排行榜");
	}

	FUNCTION_END;
}

bool CGuiBoardGameDlg::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	FUNCTION_BEGIN;

	if(nEvent == EVENT_BUTTON_CLICKED)
	{
		switch(nID)
		{
		case ID_BUTTON_GJZHSJ:  // 个人名次排行榜
			{
				   //soke 排行榜
				GetGameGuiManager()->AddTopDialog();
				GetGameGuiManager()->m_guiTopDialog->m_pListBoxRankList->SelectItem(0,true); //等级排行榜
			}
			break;
		case ID_BUTTON_JZBH:  // 个人战力排行榜
			{
				GetGameGuiManager()->AddTopDialog();
				GetGameGuiManager()->m_guiTopDialog->m_pListBoxRankList->SelectItem(1,true); //战力排行榜				
			
			}
			break;
		case ID_BUTTON_GJYX: // 今日护国排行榜
			{                                   
				GetGameGuiManager()->AddTopDialog();
				GetGameGuiManager()->m_guiTopDialog->m_pListBoxRankList->SelectItem(2,true); //今日护国榜

			}			
			break;
		case ID_BUTTON_XHPH: // 昨日护国排行榜
			{
				GetGameGuiManager()->AddTopDialog();
				GetGameGuiManager()->m_guiTopDialog->m_pListBoxRankList->SelectItem(3,true); //昨日护国榜
			}
			break;
		case ID_BUTTON_GRMC: // 个人积分排行榜
			{
				GetGameGuiManager()->AddTopDialog();
				GetGameGuiManager()->m_guiTopDialog->m_pListBoxRankList->SelectItem(4,true); //积分榜

			}
			break;
		case ID_BUTTON_CLOSE:
			{
			}
			break;
		}
		SetVisible(false);
		
		
		return true;
	}
	return CGuiDialog::OnGuiEvent(nEvent,nID,pControl);

	FUNCTION_END;
}


void CGuiBoardGameDlg::OnClose(void)
{
	FUNCTION_BEGIN;

	GetGameGuiManager()->m_guiBoardGame = NULL;

	FUNCTION_END;
}
