#pragma once

#include "..\gui\include\guidialog.h"

/**
 * \brief 孩子装备升级界面
 * 
 * 用于孩子装备的强化升级功能
 * 
 */
class CDlgEquipBabyRecast :
	public CGuiDialog
{
public:
	CDlgEquipBabyRecast();
	~CDlgEquipBabyRecast();

	void OnPutFocusItem(bool bPut); //by=>friday
	void OnResult(stResponsePropertyUserCmd* pCmd); //by=>friday


private:
	void OnClose(void);
	void OnCreate();
	bool OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl);
	void OnSetCloseLocation();
	HRESULT OnRender(float fElapsedTime);

	bool CanComposeEquip109();  //是否可以进行孩子装备升级 //by=>friday
	void RefreshPage();         //刷新页面信息 //by=>friday
	void RefreshEquipCompose109(); //刷新孩子装备升级页面信息 //by=>friday

	CRoleItem* GetFocusItem();
    CRoleItem* GetMaterialItem(int index);
    bool IsBabyEquipItem(CRoleItem* pRoleItem);
    void SendBabyEquipUpgradeMsg();

public:
	CGuiTable*	m_pTableFocusItem; // 待操作的孩子装备
	CGuiTable*	m_pTableMaterial1; // 材料格子1

private:
	CGuiStatic*	m_pStaNeedMoney;   // 所需费用显示
	CGuiStatic*	m_pStaResult;      // 说明文字
	CGuiButton*	m_pBtnUpgrade;     // 强化进阶按钮
	CAnimation	m_aniSuccess;      // 成功失败动画效果 
	bool		m_bPutFocusItem;   // 是否放入了装备 //by=>friday


}; 