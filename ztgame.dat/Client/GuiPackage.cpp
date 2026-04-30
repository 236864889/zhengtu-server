#include "public.h"
#include "./GameGuiManager.h"
#include "./GameScene.h"
#include "./MainCharacter.h"
#include "./RoleItem.h"

#include ".\guipackage.h"

#include "guiItem.h"

CGuiPackage::CGuiPackage(CRoleItem* pRoleItem)
:m_pRoleItem(pRoleItem)
{
	m_bCloseIsHide = true;
}

void CGuiPackage::OnCreate()
{
	CGuiDialog::OnCreate();

	this->SetCaptionText(m_pRoleItem->GetObjectName());

	m_pTable = GetTable(2);

	m_pTable->m_iTableType = OBJECTCELLTYPE_PACKAGE;
	m_pTable->m_iTableID = m_pRoleItem->GetThisID();
	m_pTable->m_EquipPosition = stPointI(m_pRoleItem->GetLocation().x,m_pRoleItem->GetLocation().y);
	
	m_pTable->m_showRowCount = m_pRoleItem->GetObjectBase()->maxmdam;
	m_pTable->ResetSize(m_pRoleItem->GetObjectBase()->maxpdam,
		m_pRoleItem->GetObjectBase()->maxmdam);

	// 2010 8	
	//GetStatic(10)->SetText(m_pRoleItem->GetName());

	switch(m_pRoleItem->GetObjectBase()->dwID) 
	{ //soke 额外包裹打开的图片乾坤袋
	case 662:
		SetBackImage(stResourceLocation("data\\interfaces1.gl",26,2));
		break;
	case 663:
		SetBackImage(stResourceLocation("data\\interfaces1.gl",26,3));
		break;
	case 664:
		SetBackImage(stResourceLocation("data\\interfaces1.gl",26,40));
		break;
	case 810:
		SetBackImage(stResourceLocation("data\\interfaces1.gl",26,41));
		break;
	case 811:
		SetBackImage(stResourceLocation("data\\interfaces1.gl",26,1));
		break;
	}

}

bool CGuiPackage::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	FUNCTION_BEGIN;

	//by=>friday 添加完整的事件处理，确保只有关闭按钮才能关闭包裹
	switch(nEvent)
	{
		case EVENT_BUTTON_CLICKED:
		{
			switch(nID)
			{
				case 4: //by=>friday 关闭按钮
				{
					SetVisible(false);
					return true;
				}
				break;
				//by=>friday 处理其他按钮事件但不关闭窗口
				default:
				{
					//其他按钮的处理逻辑可以在这里添加
					break;
				}
			}
			return true;
		}
		break;
		
		case EVENT_DIALOG_MOVE:
		{
			//by=>friday 对话框移动事件，不关闭窗口，正常处理
			return false; //让基类处理移动逻辑
		}
		break;
		
		case EVENT_DIALOG_SHOW:
		case EVENT_DIALOG_HIDE:
		case EVENT_DIALOG_RESIZE:
		case EVENT_DIALOG_CREATE:
		{
			//by=>friday 这些事件都不应该关闭包裹窗口
			return false; //让基类处理
		}
		break;
		
		case EVENT_BUTTON_DRAG_BEGIN:
		case EVENT_BUTTON_DRAG_MOVE:
		case EVENT_BUTTON_DRAG_END:
		{
			//by=>friday 按钮拖拽事件不应该关闭包裹窗口
			return false; //让基类处理
		}
		break;
		
		default:
		{
			//by=>friday 其他所有事件都不关闭窗口，正常处理
			break;
		}
	}
	
	return CGuiDialog::OnGuiEvent(nEvent,nID,pControl);

	FUNCTION_END;
}

void CGuiPackage::OnSetCloseLocation()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnSetCloseLocation();


	if(m_pBtnClose)
	{
		stPointI pt(m_pBtnClose->GetLocation());

        //soke 包裹关闭窗口
		m_pBtnClose->SetLocation(pt.x,pt.y-86);

		//switch(m_pRoleItem->GetObjectBase()->dwID) 
		//{
		//case 662:
		//	m_pBtnClose->SetLocation(pt.x-63,pt.y);
		//	break;
		//case 663:
		//	m_pBtnClose->SetLocation(pt.x-63,pt.y);
		//	break;
		//case 664:
		//	m_pBtnClose->SetLocation(pt.x-63,pt.y);
		//	break;
		//case 810:
		//	m_pBtnClose->SetLocation(pt.x-63,pt.y);
		//	break;
		//}
	}


	FUNCTION_END;
}

HRESULT CGuiPackage::OnRender(float fElapsedTime)
{
	FUNCTION_BEGIN;

	GetButton( 4 )->SetLocation( 270, 5 );

	//by=>friday 彻底阻止额外包裹界面的自动关闭逻辑
	//无论主背包界面状态如何，额外包裹窗口都应该保持独立
	//只有玩家主动点击关闭按钮时才关闭窗口
	
	//by=>friday 注释掉自动关闭逻辑，防止背包界面意外关闭
	//if( ! ::GetGameGuiManager()->m_guiItem->IsVisible() )
	//{
	//	this->SetVisible( false );
	//}

	//by=>friday 额外保护：检查窗口是否应该保持可见状态
	//如果窗口被意外设置为不可见，但不是通过正常关闭流程，则重新显示
	//这可以防止其他代码意外关闭包裹窗口
	if (!IsVisible() && m_pRoleItem && m_pRoleItem->GetObjectBase())
	{
		//只有当包裹物品仍然存在且有效时，才重新显示窗口
		//这确保了窗口不会在不应该关闭的时候被关闭
		//SetVisible(true);  //暂时注释，避免无限循环
	}

	HRESULT h = CGuiDialog::OnRender(fElapsedTime);

	// Title 这里是包裹的名字位置
	int iOldFont = GetDevice()->SetFont(1);
	int w = -82;
	int H = 32 - 4;
	stRectI rect( 210 + w, 12 + H, 370 + w, 30 + H );

	rect.OffsetRect(m_x,m_y);
	GetDevice()->DrawString( m_pRoleItem->GetName(), rect );
	GetDevice()->SetFont( iOldFont );

	return h;
	
	FUNCTION_END;
}

void CGuiPackage::OnClose(void)
{
	if (!m_bCloseIsHide)
	{
		switch(m_pTable->m_EquipPosition.x) 
		{
		case EQUIPCELLTYPE_LEFT:
			GetGameGuiManager()->m_guiPackage1 = NULL;
			break;
		case EQUIPCELLTYPE_RIGHT:
			GetGameGuiManager()->m_guiPackage2 = NULL;
			break;
		}
	}
}
