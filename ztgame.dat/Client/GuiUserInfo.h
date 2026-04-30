/**
*\file		GuiUserInfo.h
*\version	$Id$
*\author	$$$01公司
*\			$$$01公司@###.com
*\date		2006-7-19  21:11:00 中国标准时间
*\brief	    人物属性界面对话框
*
* 
*
*/


#pragma once
#include "..\gui\include\guidialog.h"



#define   GETSHOWCOLOR(total,base)  total==base?0xffffffff:(total>base?0xff00ff00:0xffff0000)
#define   GETSHOWCOLOR2(total,base)  total==base?0xffffffff:(total>base?0xff646464:0xffff0000)
#define   GETSHOWCOLOR3(total,base)  total==base?0xffffffff:(total>base?0xffff7100:0xffff0000)
extern void *pComboItem;


/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 */

class CGuiUserInfo :
	public CGuiDialog
{
	struct stTitle
	{
		int      key;
		stRectI  rect;
		byte     index;
	};

	struct stEquipByName
	{
		byte  numBlue;
		byte  numGlod;
		byte  numHoly;
		byte  numPurple;
		byte  numGreen;
		byte  num2Purple;
		byte  num3Purple;
		byte  num5Purple;
		byte  num8Purple;
		byte  num10Purple;
		byte  num12Purple;
		byte  num13Purple;
		byte  num14Purple;
		byte  num15Purple;
		byte  num21Purple;
		byte  num22Purple;
		byte  num23Purple;
		byte  num24Purple;
		byte  num25Purple;
		//MYY 新增装备套装声明
		byte  num165Purple;
		byte  num170Purple;
		byte  num175Purple;
		byte  num180Purple;
		byte  num185Purple;
		byte  num190Purple;
		byte  num197Purple;
		byte  num202Purple;
     	byte  num205Purple;
		byte  num210Purple;
		byte  num215Purple;
		byte  num220Purple;
		//MYY 新增斗魂套装声明
		byte  num100Purple;  
		byte  num101Purple;
		byte  num102Purple;
		byte  num103Purple;
		byte  num104Purple;
		byte  num001Purple;

		//MYY 图鉴套装声明
		byte handbook_white;
		byte handbook_bule;
		byte handbook_yloer;
		byte handbook_green;
		byte handbook_purple;
        byte handbook_orange;
		byte handbook_red;

		byte  numx5Purple;
		byte  numx8Purple;
		byte  numx11Purple;
		byte  numx13Purple;
		byte  numx15Purple;

		byte  numzb2Purple;
		byte  numzb3Purple;
		byte  numzb4Purple;
		byte  numzb5Purple;
		byte  numzb6Purple;

		byte  numsq1Purple;
		byte  numsq2Purple;
		byte  numsq3Purple;
		byte  numsq4Purple;
		byte  numsq5Purple;
		byte  numsq6Purple;
		byte  numsq7Purple;

		byte  numbt1Purple;
		byte  numbt2Purple;
		byte  numbt3Purple;
		byte  numbt4Purple;
		byte  numbt5Purple;

 		byte  numshenlong1;
		byte  numshenlong2;
		byte  numshenlong3;
		byte  numshenlong4;
		byte  numshenlong5;
		byte  numshenlong6;

		byte  numnitian1;
		byte  numnitian2;
		byte  numnitian3;
		byte  numnitian4;
		byte  numnitian5;

		byte  numdouzhansf1;
		byte  numdouzhansf2;
		byte  numdouzhansf3;
		byte  numdouzhansf4;

		byte  numaystone1;
		byte  numaystone2;
		byte  numaystone3;
		byte  numaystone4;
		byte  numaystone5;

		byte  lfy1;
		byte  lfy2;
		byte  lfy3;
		byte  lfy4;
		byte  lfy5;        

	};

	struct stEquipByMaker
	{
		char strMakerName[MAX_NAMESIZE];
		stEquipByName numSetList;

		void Init()
		{
			strMakerName[0] = 0L;
			numSetList.numBlue = 0;
			numSetList.numGlod = 0;
			numSetList.numHoly = 0;
			numSetList.numPurple = 0;
			numSetList.numGreen = 0;
			numSetList.num2Purple = 0;
			numSetList.num3Purple = 0;
			numSetList.num5Purple = 0;
			numSetList.num8Purple = 0;
			numSetList.num10Purple = 0;
			numSetList.num12Purple = 0;
			numSetList.num13Purple = 0;
			numSetList.num14Purple = 0;
			numSetList.num15Purple = 0;
			numSetList.num21Purple = 0;
			numSetList.num22Purple = 0;
			numSetList.num23Purple = 0;
			numSetList.num24Purple = 0;
			numSetList.num25Purple = 0;
            //MYY 装备套装声明
            numSetList.num165Purple = 0;
			numSetList.num170Purple = 0;
			numSetList.num175Purple = 0;
			numSetList.num180Purple = 0;
			numSetList.num185Purple = 0;
			numSetList.num190Purple = 0;
			numSetList.num197Purple = 0;
			numSetList.num202Purple = 0;
			numSetList.num205Purple = 0;
			numSetList.num210Purple = 0;
			numSetList.num215Purple = 0;
			numSetList.num220Purple = 0;
            //MYY 斗魂套装声明
			numSetList.num100Purple = 0;
			numSetList.num101Purple = 0;
			numSetList.num102Purple = 0;
			numSetList.num103Purple = 0;
			numSetList.num104Purple = 0;
            numSetList.num001Purple = 0;

			//MYY 图鉴套装声明
			numSetList.handbook_white = 0;
			numSetList.handbook_bule = 0;
			numSetList.handbook_yloer = 0;
			numSetList.handbook_green = 0;
			numSetList.handbook_purple = 0;
			numSetList.handbook_orange = 0;
			numSetList.handbook_red = 0;

			numSetList.numx5Purple = 0;
			numSetList.numx8Purple = 0;
			numSetList.numx11Purple = 0;
			numSetList.numx13Purple = 0;
			numSetList.numx15Purple = 0;

			numSetList.numzb2Purple = 0;
			numSetList.numzb3Purple = 0;
			numSetList.numzb4Purple = 0;
			numSetList.numzb5Purple = 0;
			numSetList.numzb6Purple = 0;

			numSetList.numsq1Purple = 0;
			numSetList.numsq2Purple = 0;
			numSetList.numsq3Purple = 0;
			numSetList.numsq4Purple = 0;
			numSetList.numsq5Purple = 0;
			numSetList.numsq6Purple = 0;
			numSetList.numsq7Purple = 0;

			numSetList.numbt1Purple = 0;
			numSetList.numbt2Purple = 0;
			numSetList.numbt3Purple = 0;
			numSetList.numbt4Purple = 0;
			numSetList.numbt5Purple = 0;


			numSetList.numshenlong1 = 0;
			numSetList.numshenlong2 = 0;
			numSetList.numshenlong3 = 0;
			numSetList.numshenlong4 = 0;
			numSetList.numshenlong5 = 0;
			numSetList.numshenlong6 = 0;

			numSetList.numnitian1 = 0;
			numSetList.numnitian2 = 0;
			numSetList.numnitian3 = 0;
			numSetList.numnitian4 = 0;
			numSetList.numnitian5 = 0;

			numSetList.numdouzhansf1 = 0;
			numSetList.numdouzhansf2 = 0;
			numSetList.numdouzhansf3 = 0;
			numSetList.numdouzhansf4 = 0;

			numSetList.numaystone1 = 0;
			numSetList.numaystone2 = 0;
			numSetList.numaystone3 = 0;
			numSetList.numaystone4 = 0;
			numSetList.numaystone5 = 0;

			numSetList.lfy1 = 0;
			numSetList.lfy2 = 0;
			numSetList.lfy3 = 0;
			numSetList.lfy4 = 0;
			numSetList.lfy5 = 0;

		}
	};
	enum enmEuipByMakerType
	{
		EquipByMaker_T_NONE	= -1,
		EquipByMaker_T_BUG_BLUE,
		EquipByMaker_T_BUG_GOLD,
		EquipByMaker_T_BUG_HOLY,
		EquipByMaker_T_BUG_PURPLE,
		EquipByMaker_T_WONDER_BLUE,
		EquipByMaker_T_WONDER_GOLD,
		EquipByMaker_T_WONDER_HOLY,
		EquipByMaker_T_WONDER_PURPLE,
	};

	enum enm1EuipByMakerType
	{
		EquipByMaker1_T_NONE	= -1,
		EquipByMaker1_T_WONDER_GREEN,
		EquipByMaker1_T_WONDER_PURPLE,
	};

	enum enm2EuipByMakerType
	{
		EquipByMaker2_T_NONE	= -1,
		EquipByMaker3_T_WONDER_PURPLE,
		EquipByMaker5_T_WONDER_PURPLE,
		EquipByMaker8_T_WONDER_PURPLE,
		EquipByMaker10_T_WONDER_PURPLE,
		EquipByMaker12_T_WONDER_PURPLE,
		EquipByMaker13_T_WONDER_PURPLE,
		EquipByMaker14_T_WONDER_PURPLE,
		EquipByMaker15_T_WONDER_PURPLE,
	};


	enum enm3EuipByMakerType
	{
		EquipByMaker3_T_NONE	= -1,
		EquipByMaker100_T_WONDER_PURPLE,  //斗魂3星套装
		EquipByMaker101_T_WONDER_PURPLE,  //斗魂6星套装
		EquipByMaker102_T_WONDER_PURPLE,  //斗魂9星套装
		EquipByMaker103_T_WONDER_PURPLE,  //斗魂12星套装
		EquipByMaker104_T_WONDER_PURPLE,  //斗魂15星套装
		EquipByMaker001_T_WONDER_PURPLE,  //斗魂C页面特效
	};


	enum enm4EuipByMakerType
	{
		EquipByMaker4_T_NONE	= -1,
		EquipByMaker45_T_WONDER_PURPLE,
		EquipByMaker48_T_WONDER_PURPLE,
		EquipByMaker411_T_WONDER_PURPLE,
		EquipByMaker413_T_WONDER_PURPLE,
		EquipByMaker415_T_WONDER_PURPLE,
	};

	enum enm5EuipByMakerType
	{
		EquipByMaker5_T_NONE	= -1,
		EquipByMaker52_T_WONDER_PURPLE,
		EquipByMaker53_T_WONDER_PURPLE,
		EquipByMaker54_T_WONDER_PURPLE,
		EquipByMaker55_T_WONDER_PURPLE,
		EquipByMaker56_T_WONDER_PURPLE,
	};

	enum enm6EuipByMakerType
	{
		EquipByMaker6_T_NONE	= -1,
		EquipByMaker61_T_WONDER_PURPLE,
		EquipByMaker62_T_WONDER_PURPLE,
		EquipByMaker63_T_WONDER_PURPLE,
		EquipByMaker64_T_WONDER_PURPLE,
		EquipByMaker65_T_WONDER_PURPLE,
		EquipByMaker66_T_WONDER_PURPLE,
		EquipByMaker67_T_WONDER_PURPLE,
	};

	enum enm7EuipByMakerType
	{
		EquipByMaker7_T_NONE	= -1,
		EquipByMaker71_T_WONDER_PURPLE,
		EquipByMaker72_T_WONDER_PURPLE,
		EquipByMaker73_T_WONDER_PURPLE,
		EquipByMaker74_T_WONDER_PURPLE,
		EquipByMaker75_T_WONDER_PURPLE,
	};

    enum enm11EuipByMakerType
	{
		EquipByMaker11_T_NONE	= -1,
		EquipByMaker165_T_WONDER_PURPLE,  //桃子 乾坤套装
		EquipByMaker170_T_WONDER_PURPLE,  //桃子 炙天套装
		EquipByMaker175_T_WONDER_PURPLE,  //桃子 轩辕套装
		EquipByMaker180_T_WONDER_PURPLE,  //桃子 卓越套装
		EquipByMaker185_T_WONDER_PURPLE,  //桃子 传说套装
		EquipByMaker190_T_WONDER_PURPLE,  //桃子 天尊套装
		EquipByMaker197_T_WONDER_PURPLE,  //桃子 英雄套装
		EquipByMaker202_T_WONDER_PURPLE,  //桃子 圣者套装
		EquipByMaker205_T_WONDER_PURPLE,  //桃子 战神套装
		EquipByMaker210_T_WONDER_PURPLE,  //桃子 帝王套装
		EquipByMaker215_T_WONDER_PURPLE,  //桃子 弑天套装
		EquipByMaker220_T_WONDER_PURPLE,  //桃子 永恒套装		
	};

	enum enm12EuipByMakerType   //bright moon
	{  
		EquipByMaker12_T_NONE	= -1,
		EquipByMaker12_T_SHENLONG_LV1,  //小龙套装
		EquipByMaker12_T_SHENLONG_LV2,  //金龙套装 
		EquipByMaker12_T_SHENLONG_LV3,  //苍龙套装
		EquipByMaker12_T_SHENLONG_LV4,  //巨龙套装
		EquipByMaker12_T_SHENLONG_LV5,  //神苍龙套装
		EquipByMaker12_T_SHENLONG_LV6,  //神巨龙套装
		
	};

	enum enm13EuipByMakerType   //bright moon
	{  
		EquipByMaker13_T_NONE	= -1,
		EquipByMaker13_T_NITIAN_LV1,  //逆天15阶套装
		EquipByMaker13_T_NITIAN_LV2,  //逆天30阶套装 
		EquipByMaker13_T_NITIAN_LV3,  //逆天50阶套装
		EquipByMaker13_T_NITIAN_LV4,  //逆天80阶套装
		EquipByMaker13_T_NITIAN_LV5,  //逆天100阶套装
		
	};

	enum enm14EuipByMakerType   //bright moon
	{  
		EquipByMaker14_T_NONE	= -1,
		EquipByMaker14_T_DOUZHANSF_LV1,  //神魂宝石25阶套装
		EquipByMaker14_T_DOUZHANSF_LV2,  //神魂宝石50阶套装 
		EquipByMaker14_T_DOUZHANSF_LV3,  //神魂宝石75阶套装
		EquipByMaker14_T_DOUZHANSF_LV4,  //神魂宝石100阶套装
	};	

	enum enm15EuipByMakerType   //bright moon
	{  
		EquipByMaker15_T_NONE	= -1,
		EquipByMaker15_T_AYSTONE_LV1,  //暗影宝石20阶套装
		EquipByMaker15_T_AYSTONE_LV2,  //暗影宝石40阶套装 
		EquipByMaker15_T_AYSTONE_LV3,  //暗影宝石60阶套装
		EquipByMaker15_T_AYSTONE_LV4,  //暗影宝石80阶套装
		EquipByMaker15_T_AYSTONE_LV5,  //暗影宝石100阶套装
	};	


    enum enm21EuipByMakerType
	{
		EquipByMaker21_T_NONE	= -1,
		EquipByMaker21_T_WONDER_PURPLE,
		EquipByMaker22_T_WONDER_PURPLE,
		EquipByMaker23_T_WONDER_PURPLE,
		EquipByMaker24_T_WONDER_PURPLE,
		EquipByMaker25_T_WONDER_PURPLE,
	};

    enum enm22EuipByMakerType
	{
		EquipByMaker22_T_NONE	= -1,
		EquipByMaker22_T_HANDBOOK_WHITE,  //白色图鉴套装
		EquipByMaker22_T_HANDBOOK_BULE,   //蓝色图鉴套装
		EquipByMaker22_T_HANDBOOK_YLOER,  //黄色图鉴套装
		EquipByMaker22_T_HANDBOOK_GREEN,  //绿色图鉴套装
		EquipByMaker22_T_HANDBOOK_PURPLE, //紫色图鉴套装
		EquipByMaker22_T_HANDBOOK_ORANGE, //橙色图鉴套装    
		EquipByMaker22_T_HANDBOOK_RED,    //红色图鉴套装
	};

	enum enm24EuipByMakerType   //bright moon
	{  
		EquipByMaker24_T_NONE	= -1,
		EquipByMaker24_T_LFY_LV1,  //龙凤吟8阶
		EquipByMaker24_T_LFY_LV2,  //龙凤吟10阶
		EquipByMaker24_T_LFY_LV3,  //龙凤吟12阶
		EquipByMaker24_T_LFY_LV4,  //龙凤吟14阶
		EquipByMaker24_T_LFY_LV5,  //龙凤吟16阶
	};	


	struct stUserData
	{
		::t_MapUserData*     pMapUserData;
		t_MainUserData*    pMainUserData;
		::t_MapUserData      tMapUserData;
		t_MainUserData     tMainUserData;
	};
	  //sky 马匹属性控件
	struct HorseProperty 
	{
		std::string name;
		DWORD value;
		bool isPer;
	};

	typedef std::vector<HorseProperty>	CGuiPetDialog_HorseProperty;
    //sky 一键换装

	std::map<int,CGuiTable*> m_tables;
	CGuiTab*               m_pTab;
	bool                   m_bMainUser;
	CGuiImage*              m_pImage[5];
	CToolTips               m_ToolTips;
	CToolTips 				m_JJSuitToolTips;  //by=>friday 永恒星级套装专用Tips
	WORD                   m_LevelNum;
	
	stUserData              m_UserData;
	
	std::vector<stTitle>     m_titleList;
	std::vector<stTitle>     m_equipList;
	bool                     m_bShowBody;

	std::vector<stEquipByMaker>		m_vEquipByMaker;
	enmEuipByMakerType		m_equipByMakerType;
	enm1EuipByMakerType		m1_equipByMakerType;
	enm2EuipByMakerType		m2_equipByMakerType;
	enm3EuipByMakerType		m3_equipByMakerType;
	enm4EuipByMakerType		m4_equipByMakerType;
	enm5EuipByMakerType		m5_equipByMakerType;
	enm6EuipByMakerType		m6_equipByMakerType;
	enm7EuipByMakerType		m7_equipByMakerType;

	enm11EuipByMakerType	m11_equipByMakerType;
	enm12EuipByMakerType	m12_equipByMakerType;
	enm13EuipByMakerType	m13_equipByMakerType;
	enm14EuipByMakerType	m14_equipByMakerType;
	enm15EuipByMakerType	m15_equipByMakerType;
	enm21EuipByMakerType	m21_equipByMakerType;
	enm22EuipByMakerType	m22_equipByMakerType;
	enm24EuipByMakerType	m24_equipByMakerType;
	char					m_strSetMakerName[MAX_NAMESIZE];

	bool                  m_bEquipe ;
	bool                  m_bClothes ;
	std::vector<CRoleItem*> m_listActiveItem;

	std::lookaside_allocator<CRoleItem> m_allocRoleItem;
	std::vector<CRoleItem*> m_listItem;
	
	
	void SetUserBody(byte EquipPosition,t_Object* pObject);
	void SetImage(CGuiImage* pImage,byte EquipPosition,t_Object* pObject);
	void SetTitleList();
	void TipsRender();
	void SetUserData(void* pData);
	void ShowUserInfoText1();
	void ShowUserInfoText2();
	void ShowUserInfoText3();
	void ShowUserInfoText4();
	void ShowJJSuitStatus();  //by=>friday 显示永恒星级套装状态
	void ControlImage(byte pos);
	void ShowEquipByName();
	void RefreshGoodnessOnXiXinGeMianState();
	void RefreshTaoZzhuangNameState();

	

public:
	//展示法宝
	bool fabao_show;
	bool ShowFabao(bool type);
	void ParseAccessBtnClickEvent(UINT nEvent, UINT nID);
	 //sky 一键换装
	One_Key_Equip  m_OneKeyEquip[10];
	void SetHorseData( t_HorseData* pData );
	//CAnimation m_aniBlaze;
	//IBitmap* m_bmpSex[2];
	CAnimation m_aniBody[3];

	CGuiUserInfo(bool bMainUser = true);
	void OnClose(void);
	char* GetSetMakerName();
	BYTE GetSetEquipType();

	//sky 一键换装快捷键
	void EquipSaveItem(int nIndex);
	void AutomaticProperty(int nPosition);
	void SendAddUserProperty( WORD Wdtype , int nCount );

	//法宝判断
	void OnPutFocusItem(int index,CRoleItem* pRoleItem);
	void UpdateFabao(void);
	void OnPutFocusItem2(int index,CRoleItem* pRoleItem);
	void UpdateYuanshen(void);
	int fbsx1;
	int fbsx2;
	int fbsx3;
	int fbsx4;
	int fbsx5;
	
	int fblevel;
	int fbfjlevel;

	int yssx1;
	int yssx2;
	int yssx3;
	int yssx4;
	int yssx5;
	int yslevel;
	int ysfjlevel;


	int hanbing_state;
	int chibang_state;

	//一键换装冷却
	long long onekey_time;

	

	//void InitCustomElements();
	HRESULT OnRender(float fElapsedTime);
	bool OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl);
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void RefreshButtonState();
	void RefreshGetBoxGeMianState();
	void RefreshGetBoxQianming();
	void OnCreate();
	void OnSetCloseLocation();
	void BodyRender(stPointI ptRender);
	void SetCharType(int type,void* pData = NULL);
	void SetUserImage(byte EquipPosition,t_Object* pObject = NULL);
	void RefreshShowState();
	void SetLevelNum(WORD LevelNum)
	{
		if (LevelNum!=0)
			m_LevelNum = LevelNum;
		else
			m_LevelNum = (WORD)-1;
	}

	void InitIcon();
	bool IsMainInfo(){ return m_bMainUser;}
	bool IsEquipedAutoHpItem(bool bMainUser = true);

	void RefreshBodyColor(DWORD color);
	bool IsTonic();
	CRoleItem* GetShowClothes();
	bool IsActiveItem(CRoleItem* pItem);
	void RefreshActiveItem();
	void InsertActiveItem(CRoleItem* pRoleItem);
	void SetEquipByName(CRoleItem* pItem, bool bMainCharacter = true);

	CGuiTable* & GetEquipTable(int equip_type,int x = 0)
	{
		int value = (equip_type << 16 | x);
		return m_tables[value];
	}

	bool IsAdornObject(DWORD dwObjectID);
	bool IsAdornObjectByType(DWORD  dwType);

	//
	CRoleItem* FindItemInEquipListByFiveTypeConstID(const DWORD& dwID, const DWORD& fiveType);
	void ShowActiveFiveEffect();

	CRoleItem* GetEquipItem(int equip_type,int x = 0);

	CGuiPetDialog_HorseProperty	m_vecHorseProperty; //sky

	//sky ListBox控件
	CGuiListBox * m_pHorseListBox;

	CToolTips* GetTaoZhuangTooltips(int nIndex);
	
	std::string getTaoZhuang_Name(int index);
	
	std::string getTaoZhuang_SName(int index);

	void setActiveIcon(CGuiButton* btn){m_ActiveIcon=btn;}
	
	CGuiButton* getActiveIcon(){ return m_ActiveIcon; }
	
	int getActiveIndex();

	void equipTaoZhuang(int index);
	void on_equipTaoZhuang(int index);

	int getUsedIndex() { return m_usedIndex; }

	CGuiButton* getTaoZhuangButton(){ return GetButton(11);}

	CGuiButton* m_pBtn_Skill;
	stResourceLocation m_rl_Skill;
	int m_frameNum_Skill;

	DWORD m_dwSkillIconID;

	bool SetAccessKey( const int& keyID );
	void setSkillIconID(DWORD dwID = -1);
	
	private:
		CGuiButton* m_ActiveIcon;
	    int m_usedIndex;
};


