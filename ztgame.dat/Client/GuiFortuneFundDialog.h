 
#pragma once
#include "..\gui\include\guidialog.h"
 
class CGuiFortuneFundDialog :
	public CGuiDialog
{	
public:
	CGuiFortuneFundDialog(void);
	~CGuiFortuneFundDialog(void);

	bool OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl);
	void OnClose(void);
	void OnCreate();
	void OnIDOK();

	HRESULT OnRender(float fElapsedTime);
	void UpdatePointNum();
	void ShowFortunGiveNum(int Fortun0_num,int Fortun1_num,int Fortun2_num);
	
	private:
	CGuiEditBox* m2_pEditBox;

	int	m2_iInputPointNum;

	CGuiTab* m_pTab;
};
