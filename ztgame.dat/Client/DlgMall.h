#pragma once

#include "..\gui\include\guidialog.h"
#include <set> // by=>friday

class CGuiWare;
enum WAREREGION_CLASS;
class CDlgMall :
	public CGuiDialog
{
	enum
	{
		MAX_WARE_PER_PAGE = 15, // 新商城每页最多显示的商品数
	};

public:
	CDlgMall();
	~CDlgMall();

	void Refresh(); // 刷新当前页的显示

public:
	CGuiWare*	 m_pGuiWare[MAX_WARE_PER_PAGE];

private:
	void OnClose(void);
	void OnCreate();
	bool OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl);
	HRESULT OnRender(float fElapsedTime);

private:
	unsigned int GetMaxPage(); // 得到当前区的最大页数
	void InitWares(); // 初始化当前页商品
	void ReleaseWares(); // 清除当前页商品
	
	//by=>friday
	void UpdateCategoryButtonsPosition(); // 更新类别按钮位置

private:
	CGuiListBox* m_pListBoxWareRegion;
	CGuiStatic*  m_pStaCurPage;
	CGuiStatic*	 m_pStaChongzhi; //充值点
	CGuiStatic*  m_pStaCategory; //by=>friday 类别标题

	WAREREGION_CLASS m_eCurRegion;
	int				 m_nCurPage;
	
	//by=>friday
	int              m_nCategoryOffset; // 类别按钮滑动偏移量
};
