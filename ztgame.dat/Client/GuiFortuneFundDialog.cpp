
#include "public.h"
#include "GameGuiManager.h"
#include "game_script.h"
#include "./RoleItem.h"
#include "./GameScene.h"
#include "./MainCharacter.h"
#include ".\GuiFortuneFundDialog.h"

#define ID_BUTTON_CLOSE  99
#define IDC_EDIT_BOX2	 200

#define _SetLabelText(id,txt)	{CGuiStatic* pLabel = GetStatic(id); if( pLabel ) {pLabel->SetText(txt);}}

CGuiFortuneFundDialog::CGuiFortuneFundDialog(void)
{
	m2_iInputPointNum = 0;
	m_bSmDialog = true;
}

CGuiFortuneFundDialog::~CGuiFortuneFundDialog(void)
{
	GetGameGuiManager()->m_guiFortuneFundDlg = NULL;
}

void CGuiFortuneFundDialog::OnClose(void)
{
	GetGameGuiManager()->m_guiFortuneFundDlg = NULL;
	GetGameGuiManager()->OnNpcActionEnd();
}

void CGuiFortuneFundDialog::OnCreate()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnCreate();
	
	GetButton(20)->SetButtonGroup(1);
	GetButton(21)->SetButtonGroup(1);	
	GetButton(22)->SetButtonGroup(1);

	m_pTab = GetTab(7);
	m_pTab->AddItem("",NULL);
	m_pTab->AddItem("",NULL);
	m_pTab->AddItem("",NULL);	

	m_pTab->AddControl(0,(CGuiControl*)GetButton(23)); // 确定
	m_pTab->AddControl(0,(CGuiControl*)GetEditBox(200));
	m_pTab->AddControl(0,GetControl(301)); // 当前等级
	m_pTab->AddControl(0,GetControl(302)); // 现有游戏点数
	m_pTab->AddControl(0,GetControl(303)); // 我要投入
	m_pTab->AddControl(0,GetControl(304)); // 点  折合金子          购买增值保险
	m_pTab->AddControl(0,GetControl(350)); // 投入 价格
	m_pTab->AddControl(0,GetControl(380)); // 价格
	m_pTab->AddControl(0,GetControl(305)); // 即刻返还
	m_pTab->AddControl(0,GetControl(306)); // 60级：
	m_pTab->AddControl(0,GetControl(307)); // 70级：
	m_pTab->AddControl(0,GetControl(308)); // 80级：
	m_pTab->AddControl(0,GetControl(309)); // 90级：
	m_pTab->AddControl(0,GetControl(310)); // 100级：
	m_pTab->AddControl(0,GetControl(311)); // 110级：
	m_pTab->AddControl(0,GetControl(312)); // 120级：
	m_pTab->AddControl(0,GetControl(313)); // 130级：
	m_pTab->AddControl(0,GetControl(314)); // 140级：
	m_pTab->AddControl(0,GetControl(315)); // 150级：
	m_pTab->AddControl(0,GetControl(316)); // 160级：
	m_pTab->AddControl(0,GetControl(317)); // 返还总计(金子)：
	m_pTab->AddControl(0,GetControl(384)); // 总价格

	m_pTab->AddControl(1,GetControl(318)); // 当前等级
	m_pTab->AddControl(1,GetControl(319)); // 已购买增值保险       点，折合金子     锭    两
	m_pTab->AddControl(1,GetControl(381)); // 价格
	m_pTab->AddControl(1,GetControl(382)); // 还可购买增值保险：
	m_pTab->AddControl(1,GetControl(320)); // 各级返还总计：
	m_pTab->AddControl(1,GetControl(321)); // 60级
	m_pTab->AddControl(1,GetControl(322)); // 70级
	m_pTab->AddControl(1,GetControl(323)); // 80级
	m_pTab->AddControl(1,GetControl(324)); // 90级
	m_pTab->AddControl(1,GetControl(325)); // 100级
	m_pTab->AddControl(1,GetControl(326)); // 110级
	m_pTab->AddControl(1,GetControl(327)); // 120级
	m_pTab->AddControl(1,GetControl(328)); // 130级
	m_pTab->AddControl(1,GetControl(329)); // 140级
	m_pTab->AddControl(1,GetControl(330)); // 150级
	m_pTab->AddControl(1,GetControl(331)); // 160级
	m_pTab->AddControl(1,GetControl(332)); // 返还总计：
	m_pTab->AddControl(1,GetControl(383)); // 返还价格
	m_pTab->AddControl(1,(CGuiControl*)GetButton(24)); // 账目明细

	m_pTab->AddControl(2,GetControl(333)); // 当前等级
	m_pTab->AddControl(2,GetControl(334)); // 已领取（金子）：
	m_pTab->AddControl(2,GetControl(336)); // 未来可领取（金子）：
	m_pTab->AddControl(2,GetControl(335)); // 当前可领取（金子）：
	m_pTab->AddControl(2,(CGuiControl*)GetButton(25)); // 领取

	m_pTab->SetVisible(false);
//	GetEditBox(200)->SetEnabled(true);

	OnGuiEvent(EVENT_BUTTON_CLICKED,20,GetButton(20));

	m2_pEditBox = GetEditBox(IDC_EDIT_BOX2);
	if( !m2_pEditBox )
	{
		GetGameGuiManager()->AddMessageBox("点卡兑换五倍基金错误");
		Engine_WriteLog("点卡兑换五倍基金错误");
		Close();
		return ;
	}
 
	FUNCTION_END;
}

void CGuiFortuneFundDialog::UpdatePointNum()
{
	/*CMainCharacter* pMainRole = GetScene()->GetMainCharacter();
	if( pMainRole )
	{
		m2_pEditBox->SetStyle(CGuiEditBox::GUI_ES_NUMBER);

		m2_pEditBox->SetMinNumber(0);
		m2_pEditBox->SetMaxNumber(pMainRole->GetPointNum());
	}	*/
}

//sky 获取基金返回
void CGuiFortuneFundDialog::ShowFortunGiveNum(int Fortun0_num,int Fortun1_num,int Fortun2_num)
{	
	//sky 已购买数量
	int forpint = Fortun0_num;

	//sky 总点数
	DWORD POINT = 10000000;//投保上限 佛系

	//sky 限制输入的点数
	int forpinr = (POINT-Fortun0_num)/1000;

	CMainCharacter* pMainRole = GetScene()->GetMainCharacter();
	if( pMainRole )
	{
		m2_pEditBox->SetStyle(CGuiEditBox::GUI_ES_NUMBER);

		m2_pEditBox->SetMinNumber(0);
		m2_pEditBox->SetMaxNumber(forpinr);
	}

	//sky 剩余购买数量
	int forpint2 = (POINT - Fortun0_num);

	char temp[128];
	_snprintf(temp,128,"已购买增值保险 %u 点，折合金子",forpint);//sky 已购买增值保险
	_SetLabelText(319,temp);

	char temp1[128];
	_snprintf(temp1,128,"    %s ",Money0String(forpint));//sky 折合金子价格显示
	_SetLabelText(381,temp1);

	char temp2[128];
	_snprintf(temp2,128," ，还可购买增值保险：%d点",forpint2);
	_SetLabelText(382,temp2);

	//sky 各级返还总计
	int forpint3 = (Fortun0_num)*4.8f;

	char temp3[128];
	_snprintf(temp3,128,"各级返还总计：%s",Money0String(forpint3));
	_SetLabelText(320,temp3);

	//sky 返还20%
	int forpint4 = (Fortun0_num)*0.2f;

	//sky 返还30%
	int forpint5 = (Fortun0_num)*0.3f;

	//sky 返还50%
	int forpint6 = (Fortun0_num)*0.5f;

	//sky 返还100%
	int forpint7 = (Fortun0_num);

	//sky 返还150%
	int forpint8 = (Fortun0_num)*1.5f;

	//sky 返还总计
	int forpint9 = (Fortun0_num)*5.0f;

	for(int i = 6;i<12;i++)
	{
		char temp4[128];
		_snprintf(temp4,128,"%u级：%s",i*10,Money0String(forpint4));
		_SetLabelText(i+315,temp4);
	}

	char temp5[128];
	_snprintf(temp5,128,"120级：%s",Money0String(forpint5));
	_SetLabelText(327,temp5);

	char temp6[128];
	_snprintf(temp6,128,"130级：%s",Money0String(forpint5));
	_SetLabelText(328,temp6);

	char temp7[128];
	_snprintf(temp7,128,"140级：%s",Money0String(forpint6));
	_SetLabelText(329,temp7);

	char temp8[128];
	_snprintf(temp8,128,"150级：%s",Money0String(forpint7));
	_SetLabelText(330,temp8);

	char temp9[128];
	_snprintf(temp9,128,"160级：%s",Money0String(forpint8));
	_SetLabelText(331,temp9);

	char temp10[256];
	_snprintf(temp10,256,"返还总计：              （包括即刻返还 %s）",Money0String(forpint4)); //sky 返还总计
	_SetLabelText(332,temp10);

	char temp11[128];
	_snprintf(temp11,128,"%s",Money0String(forpint9)); //sky 即刻返还
	_SetLabelText(383,temp11);

	//第三页

	char temps[128];
	_snprintf(temps,128,"已领取（金子）：%s",Money0String(Fortun1_num)); //sky 已领取（金子）
	_SetLabelText(334,temps);

	char temps1[128];
	_snprintf(temps1,128,"未来可领取（金子）：%s",Money0String(Fortun2_num)); //sky 未来可领取（金子）
	_SetLabelText(336,temps1);

	if( GetScene()->GetMainCharacter()->GetProperty()->level < 60)
	{
		char temps2[128];
		_snprintf(temps2,128,"当前可领取（金子）：0两"); //sky 当前可领取（金子）
		_SetLabelText(335,temps2);
	}
	else if( GetScene()->GetMainCharacter()->GetProperty()->level >= 60 && GetScene()->GetMainCharacter()->GetProperty()->level < 70)
	{
		//sky 当前可领取
		int forpintd3 = (Fortun0_num*0.4f) - Fortun1_num;

		char temps2[128];
		_snprintf(temps2,128,"当前可领取（金子）：%s",Money0String(forpintd3)); //sky 当前可领取（金子）
		_SetLabelText(335,temps2);
	}
	else if( GetScene()->GetMainCharacter()->GetProperty()->level >= 70 && GetScene()->GetMainCharacter()->GetProperty()->level < 80)
	{
		//sky 当前可领取
		int forpintd3 = (Fortun0_num*0.6f) - Fortun1_num;

		char temps2[128];
		_snprintf(temps2,128,"当前可领取（金子）：%s",Money0String(forpintd3)); //sky 当前可领取（金子）
		_SetLabelText(335,temps2);
	}
	else if( GetScene()->GetMainCharacter()->GetProperty()->level >= 80 && GetScene()->GetMainCharacter()->GetProperty()->level < 90)
	{
		//sky 当前可领取
		int forpintd3 = (Fortun0_num*0.8f) - Fortun1_num;

		char temps2[128];
		_snprintf(temps2,128,"当前可领取（金子）：%s",Money0String(forpintd3)); //sky 当前可领取（金子）
		_SetLabelText(335,temps2);
	}
	else if( GetScene()->GetMainCharacter()->GetProperty()->level >= 90 && GetScene()->GetMainCharacter()->GetProperty()->level < 100)
	{
		//sky 当前可领取
		int forpintd3 = (Fortun0_num*1.0f) - Fortun1_num;

		char temps2[128];
		_snprintf(temps2,128,"当前可领取（金子）：%s",Money0String(forpintd3)); //sky 当前可领取（金子）
		_SetLabelText(335,temps2);
	}
	else if( GetScene()->GetMainCharacter()->GetProperty()->level >= 100 && GetScene()->GetMainCharacter()->GetProperty()->level < 110)
	{
		//sky 当前可领取
		int forpintd3 = (Fortun0_num*1.2f) - Fortun1_num;

		char temps2[128];
		_snprintf(temps2,128,"当前可领取（金子）：%s",Money0String(forpintd3)); //sky 当前可领取（金子）
		_SetLabelText(335,temps2);
	}
	else if( GetScene()->GetMainCharacter()->GetProperty()->level >= 110 && GetScene()->GetMainCharacter()->GetProperty()->level < 120)
	{
		//sky 当前可领取
		int forpintd3 = (Fortun0_num*1.4f) - Fortun1_num;

		char temps2[128];
		_snprintf(temps2,128,"当前可领取（金子）：%s",Money0String(forpintd3)); //sky 当前可领取（金子）
		_SetLabelText(335,temps2);
	}
	else if( GetScene()->GetMainCharacter()->GetProperty()->level >= 120 && GetScene()->GetMainCharacter()->GetProperty()->level < 130)
	{
		//sky 当前可领取
		int forpintd3 = (Fortun0_num*1.7f) - Fortun1_num;

		char temps2[128];
		_snprintf(temps2,128,"当前可领取（金子）：%s",Money0String(forpintd3)); //sky 当前可领取（金子）
		_SetLabelText(335,temps2);
	}
	else if( GetScene()->GetMainCharacter()->GetProperty()->level >= 130 && GetScene()->GetMainCharacter()->GetProperty()->level < 140)
	{
		//sky 当前可领取
		int forpintd3 = (Fortun0_num*2.0f) - Fortun1_num;

		char temps2[128];
		_snprintf(temps2,128,"当前可领取（金子）：%s",Money0String(forpintd3)); //sky 当前可领取（金子）
		_SetLabelText(335,temps2);
	}
	else if( GetScene()->GetMainCharacter()->GetProperty()->level >= 140 && GetScene()->GetMainCharacter()->GetProperty()->level < 150)
	{
		//sky 当前可领取
		int forpintd3 = (Fortun0_num*2.5f) - Fortun1_num;

		char temps2[128];
		_snprintf(temps2,128,"当前可领取（金子）：%s",Money0String(forpintd3)); //sky 当前可领取（金子）
		_SetLabelText(335,temps2);
	}
	else if( GetScene()->GetMainCharacter()->GetProperty()->level >= 150 && GetScene()->GetMainCharacter()->GetProperty()->level < 160)
	{
		//sky 当前可领取
		int forpintd3 = (Fortun0_num*3.5f) - Fortun1_num;

		char temps2[128];
		_snprintf(temps2,128,"当前可领取（金子）：%s",Money0String(forpintd3)); //sky 当前可领取（金子）
		_SetLabelText(335,temps2);
	}
	else if( GetScene()->GetMainCharacter()->GetProperty()->level >= 160)
	{
		//sky 当前可领取
		int forpintd3 = (Fortun0_num*5.0f) - Fortun1_num;

		char temps2[128];
		_snprintf(temps2,128,"当前可领取（金子）：%s",Money0String(forpintd3)); //sky 当前可领取（金子）
		_SetLabelText(335,temps2);
	}
}

bool CGuiFortuneFundDialog::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	FUNCTION_BEGIN;

	switch(nEvent)
	{
	case EVENT_BUTTON_CLICKED:
		{
			switch(nID)
			{
				case 20:
				case 21:
				case 22:
				{
					m_pTab->SetCurItem(nID -20);
					GetButton(nID)->SetChecked(true);

					//sky 切换页面 刷新
					stRequestFortunGiveTradeUserCmd cmd;
					SEND_USER_CMD(cmd);

					//sky 切换的时候清除输入
					GetEditBox(200)->ClearText();
					SetFocus(GetEditBox(200));
				}
				break;
			case 23://sky 确定
				{
					if(GetScene()->GetMainCharacter()->GetProperty()->level < 0)
					{
						return true;
					}
					else
					{
						OnIDOK();

						//sky 切换页面 刷新
						stRequestFortunGiveTradeUserCmd cmd2;
						SEND_USER_CMD(cmd2);
					}
				}
				break;
			case 24: //sky 账目明细
				{
					stFortGoldGiveTradeUserCmd cmd;
					cmd.type= 1;
					SEND_USER_CMD(cmd);
				}
				break;
			case 25: //sky 确定
				{
					stFortGoldGiveTradeUserCmd cmd;
					cmd.type= 0;
					SEND_USER_CMD(cmd);

					//sky 切换页面 刷新
					stRequestFortunGiveTradeUserCmd cmd2;
					SEND_USER_CMD(cmd2);
				}
				break;
			case ID_BUTTON_CLOSE:
				{
					//Close();
					SetVisible(false);
				}
				break;
			}
	    }
		case EVENT_EDITBOX_CHANGE:
		{
			if( nID == IDC_EDIT_BOX2 )
			{
				const char* pszText = m2_pEditBox->GetText();
				m2_iInputPointNum = strtoul(pszText,NULL,10);

				return true;
			}
		}
	}
	return CGuiDialog::OnGuiEvent(nEvent,nID,pControl);

	FUNCTION_END;
}

HRESULT CGuiFortuneFundDialog::OnRender(float fElapsedTime)
{
	FUNCTION_BEGIN;
	HRESULT hr = CGuiDialog::OnRender(fElapsedTime);

	UpdatePointNum();

	CGuiControl* pTitle = GetControl(301); //sky 当前等级
	char szText[100]={};
	sprintf(szText,"当前等级： %u", GetScene()->GetMainCharacter()->GetProperty()->level);
	pTitle->SetText(szText);

	CGuiControl* pTitle2 = GetControl(302); //sky 现有游戏点数
	char szText2[100]={};
	sprintf(szText2,"现有游戏点数： %u", GetScene()->GetMainCharacter()->GetPointNum());
	pTitle2->SetText(szText2);

    //sky 输入的数值
	const char* pszText3 = m2_pEditBox->GetText();
	m2_iInputPointNum = strtoul(pszText3,NULL,10);
	int point = m2_iInputPointNum*1000;
    //sky 20%价格
	int point2 = (m2_iInputPointNum*1000)*0.2f;
	//sky 30%价格
	int point3 = (m2_iInputPointNum*1000)*0.3f;
	//sky 50%价格
	int point4 = (m2_iInputPointNum*1000)*0.5f;
	//sky 150%价格
	int point5 = (m2_iInputPointNum*1000)*1.5f;
	//sky 总价格
	int point6 = (m2_iInputPointNum*1000)*5.0f;

	CGuiControl* pTitle3 = GetControl(380); //sky 投入价格
	char szText3[100]={};
	sprintf(szText3,"%s", Money0String(point));
	pTitle3->SetText(szText3);

	CGuiControl* pTitle01 = GetControl(305); //sky 即刻返还
	char szText01[100]={};
	sprintf(szText01,"即刻返还：%s", Money0String(point2));
	pTitle01->SetText(szText01);

	for(int i = 6;i<12;i++)
	{
		CGuiControl* pTitle4 = GetControl(i+300); //sky 60级
		char szText4[100]={};
		sprintf(szText4,"%u级：%s", i*10, Money0String(point2));
		pTitle4->SetText(szText4);
	}

	CGuiControl* pTitle5 = GetControl(312); //sky 120
	char szText5[100]={};
	sprintf(szText5,"120级：%s", Money0String(point3));
	pTitle5->SetText(szText5);

	CGuiControl* pTitle6 = GetControl(313); //sky 130
	char szText6[100]={};
	sprintf(szText6,"130级：%s", Money0String(point3));
	pTitle6->SetText(szText6);

	CGuiControl* pTitle7 = GetControl(314); //sky 140
	char szText7[100]={};
	sprintf(szText7,"140级：%s", Money0String(point4));
	pTitle7->SetText(szText7);

	CGuiControl* pTitle8 = GetControl(315); //sky 150
	char szText8[100]={};
	sprintf(szText8,"150级：%s", Money0String(point));
	pTitle8->SetText(szText8);

	CGuiControl* pTitle9 = GetControl(316); //sky 160
	char szText9[100]={};
	sprintf(szText9,"160级：%s", Money0String(point5));
	pTitle9->SetText(szText9);

	CGuiControl* pTitle10 = GetControl(384); //sky 返还总计(金子)
	char szText10[100]={};
	sprintf(szText10,"%s", Money0String(point6));
	pTitle10->SetText(szText10);

    //sky 第二页

	CGuiControl* pTitlex = GetControl(318); //sky 当前等级
	char szTextx[100]={};
	sprintf(szTextx,"当前等级： %u", GetScene()->GetMainCharacter()->GetProperty()->level);
	pTitlex->SetText(szTextx);

	
	//sky 第三页

	CGuiControl* pTitlek = GetControl(333); //sky 当前等级
	char szTextk[100]={};
	sprintf(szTextk,"当前等级： %u", GetScene()->GetMainCharacter()->GetProperty()->level);
	pTitlek->SetText(szTextk);


	return hr;
	FUNCTION_END;
}

//sky 确定以后执行的
void CGuiFortuneFundDialog::OnIDOK()
{
	const char* pszText = m2_pEditBox->GetText();
	m2_iInputPointNum = strtoul(pszText,NULL,10);
	if( m2_iInputPointNum > 0 )
	{
		stRequestRedeemMonthCard cmd;
		cmd.dwNum = m2_iInputPointNum*1000;
		SEND_USER_CMD(cmd);

		//sky 清除输入
		GetEditBox(200)->ClearText();
		SetFocus(GetEditBox(200));	
	}	
}
