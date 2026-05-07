 #pragma once 

class CGuiMagicBoxDlg : public CGuiDialog
{	
private:
	CAnimation m_AniBody[6];
	CGuiImage* pImage[6];
	

public:

	CGuiMagicBoxDlg();
	~CGuiMagicBoxDlg();
	CGuiTab * m_pTab;
	int type;

	//时装
	struct one_shizhuang
	{
		char name[MAX_NAMESIZE];
		DWORD bodyNum;
		DWORD level;
		DWORD state;
	};
	struct one_shizhuang Shizhuang[300]; 
	//披风
	struct one_pifeng
	{
		char name[MAX_NAMESIZE];
		DWORD itemNum;
		DWORD level;
		DWORD state;
	};
	struct one_pifeng Pifeng[100]; 
	//翅膀
	struct one_chibang
	{
		char name[MAX_NAMESIZE];
		DWORD itemNum;
		DWORD level;
		DWORD state;
	};
	
	struct one_chibang Chibang[100]; 
	//坐骑
	struct one_zuoqi
	{
		char name[MAX_NAMESIZE];
		DWORD npcNum;
		DWORD level;
		DWORD state;
	};
	struct one_zuoqi Zuoqi[100]; 

	//魔盒界面
	struct one_jiemian
	{
		char name[MAX_NAMESIZE];
		DWORD activityNum;
		DWORD dongtai;
		DWORD level;
		DWORD state;
	};
	struct one_jiemian Jiemian[100]; 
	DWORD mohelevel;
	DWORD moheexp;
	DWORD mohemaxexp;
	DWORD shizhuang_select;
	DWORD pifeng_select;
	DWORD chibang_select;
	DWORD zuoqi_select;
	DWORD jiemian_select;
	int shizhuangshu;
	int pifengshu;
	int chibangshu;
	int zuoqishu;
	int jiemianshu;
	DWORD num1;
	DWORD num2;
	DWORD num3;
	DWORD num4;
	DWORD num5;
	int page;
	void shuaxin(void);
	void OnClose(void);
	void OnCreate(void);
	bool OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl);
	HRESULT OnRender(float fElapsedTime);
};
