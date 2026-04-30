#pragma once

#include "..\gui\include\guidialog.h"
#include "GuiNpcMake.h"

enum EQUIPZHANHUN_PAGE
{
	EQUIPZ_COMPOSE11,   //战魂升级
};

enum RecastAction_zh;
struct stResponsePropertyUserCmd;
class CDlgZhanhunRecast :
	public CGuiDialog
{
	enum
	{
		TOTAL_MATERIAL_TABLE = 1,// 材料格子的总数
	};

public:
	CDlgZhanhunRecast();
	~CDlgZhanhunRecast();

	void OnPutFocusItem(bool bPut);
	void OnResult(stResponsePropertyUserCmd* pCmd);
	
public:
	CGuiTable*	m_pTableFocusItem; // 待操作的item
	CGuiTable*	m_pTableMaterial1;

private:
	void OnClose(void);
	void OnCreate();
	bool OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl);
	//void OnSetCloseLocation();
	HRESULT OnRender(float fElapsedTime);

private:

	bool CanComposeEquip_zh1();    //是否可以进行

	CRoleItem* GetFocusItem();
	CRoleItem* GetMaterialItem(int index); // 根据格子索引得到CRoleItem（索引为0,1,2,3）
	bool IsEquipItem(enumItemType eType);  // 是否为装备类物品

	void SendEquipRecastMsg(RecastAction_zh eAction);
	void OnTimeUp();
	void SetBGImage();

	void RefreshPage();
	void RefreshEquipCompose_zh1();    //刷新页面信息


private:
	CGuiStatic*			m_pStaNeedMoney;
	CGuiStatic*			m_pStaResult;

	CGuiTable*			m_pTableMaterials[TOTAL_MATERIAL_TABLE];

	CMakeData	m_MakeData; // 打造数据类
	std::string m_strTitle;
	CAnimation	m_aniSuccess;
	bool		m_bPutFocusItem;
	UINT		m_unSoulMountNeedMoney;
};
