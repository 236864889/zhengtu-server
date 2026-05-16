
#include "public.h"
#include "miniusercommand.h"
#include "./GameGuiManager.h"
#include "./GuiMagicBoxDlg.h"
#include "../gui/include/GuiManager.h"
#include "./Game.h"
#include "LordStrikeManager.h"
#include "LordStrikeLobby.h"
#include "SafetyUser.h"
#include "./GameScene.h"
#include "./character.h"
#include "./MainCharacter.h"
///////////////////////////////////////////////////////////////////////////////
#define ID_BUTTON_ROLE_SELECT2 3 // 领取登陆奖励

#define ID_BUTTON_CLOSE 0

#define MAGICBOX_PAGE_SIZE 6
#define MAGICBOX_JIEMIAN_PAGE_SIZE 2
///////////////////////////////////////////////////////////////////////////////

CGuiMagicBoxDlg::CGuiMagicBoxDlg()
{
	FUNCTION_BEGIN;

	m_bSmDialog = true;

	FUNCTION_END;
}

CGuiMagicBoxDlg::~CGuiMagicBoxDlg()
{
	FUNCTION_BEGIN;

	FUNCTION_END;
}


static bool MagicBoxHasName(const char *name)
{
	return name && name[0] != 0;
}

static void SetMagicBoxBodyResource(stResourceLocation &rl, const char *pic, DWORD bodyNum)
{
	rl.SetFileName("data\\body5.gl");
	rl.group = bodyNum;
	rl.frame = 0;
	if (!pic || pic[0] == 0)
		return;

	char path[64];
	strncpy(path, pic, sizeof(path) - 1);
	path[sizeof(path) - 1] = 0;
	for (int i = 0; path[i]; i++)
	{
		if (path[i] == '/')
			path[i] = '\\';
	}

	char *first = strchr(path, '\\');
	if (!first)
		return;
	char *second = strchr(first + 1, '\\');
	if (!second)
		return;
	char *groupText = second + 1;
	char *third = strchr(groupText, '\\');
	char *frameText = NULL;
	if (third)
	{
		*third = 0;
		frameText = third + 1;
	}
	*second = 0;

	char fileName[64];
	sprintf(fileName, "%s.gl", path);
	rl.SetFileName(fileName);
	int group = atoi(groupText);
	rl.group = group > 0 ? group : bodyNum;
	rl.frame = frameText ? atoi(frameText) : 0;
}

static int GetMagicBoxPageSize(int type)
{
	return (type == 4) ? MAGICBOX_JIEMIAN_PAGE_SIZE : MAGICBOX_PAGE_SIZE;
}

static int GetMagicBoxItemCount(CGuiMagicBoxDlg *dlg, int type)
{
	if (!dlg)
		return 0;
	int count = 0;
	int i = 0;
	if (type == 0)
	{
		for (i = 0; i < 400; i++)
			if (MagicBoxHasName(dlg->Shizhuang[i].name) || dlg->Shizhuang[i].bodyNum != 0)
				count = i + 1;
	}
	else if (type == 1)
	{
		for (i = 0; i < 100; i++)
			if (MagicBoxHasName(dlg->Pifeng[i].name) || dlg->Pifeng[i].itemNum != 0)
				count = i + 1;
	}
	else if (type == 2)
	{
		for (i = 0; i < 100; i++)
			if (MagicBoxHasName(dlg->Chibang[i].name) || dlg->Chibang[i].itemNum != 0)
				count = i + 1;
	}
	else if (type == 3)
	{
		for (i = 0; i < 100; i++)
			if (MagicBoxHasName(dlg->Zuoqi[i].name) || dlg->Zuoqi[i].npcNum != 0)
				count = i + 1;
	}
	else if (type == 4)
	{
		for (i = 0; i < 100; i++)
			if (MagicBoxHasName(dlg->Jiemian[i].name) || dlg->Jiemian[i].activityNum != 0)
				count = i + 1;
	}
	return count;
}

static int GetMagicBoxMaxPage(CGuiMagicBoxDlg *dlg, int type)
{
	int pageSize = GetMagicBoxPageSize(type);
	int count = GetMagicBoxItemCount(dlg, type);
	int maxPage = (count + pageSize - 1) / pageSize;
	return maxPage < 1 ? 1 : maxPage;
}

static void ClampMagicBoxPage(CGuiMagicBoxDlg *dlg)
{
	if (!dlg)
		return;
	int maxPage = GetMagicBoxMaxPage(dlg, dlg->type);
	if (dlg->page < 1)
		dlg->page = 1;
	if (dlg->page > maxPage)
		dlg->page = maxPage;
}

// 创建对话框
void CGuiMagicBoxDlg::OnCreate(void)
{
	FUNCTION_BEGIN;

	CGuiDialog::OnCreate();
	memset(Shizhuang, 0, sizeof(Shizhuang));
	memset(Pifeng, 0, sizeof(Pifeng));
	memset(Chibang, 0, sizeof(Chibang));
	memset(Zuoqi, 0, sizeof(Zuoqi));
	memset(Jiemian, 0, sizeof(Jiemian));
	shizhuang_select = 9999;
	pifeng_select = 9999;
	chibang_select = 9999;
	zuoqi_select = 9999;
	jiemian_select = 9999;
	m_pTab = GetTab(10);
	if (!m_pTab)
		return;
	m_pTab->AddItem("",NULL);
	m_pTab->AddItem("",NULL);

	
	m_pTab->AddControl(0,(CGuiControl*)GetImage(11));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(12));
	m_pTab->AddControl(0,(CGuiControl*)GetStatic(13));
	m_pTab->AddControl(0,(CGuiControl*)GetStatic(14));
	m_pTab->AddControl(0,(CGuiControl*)GetCheckBox(15));
	m_pTab->AddControl(0,(CGuiControl*)GetImage(16));
	m_pTab->AddControl(0,(CGuiControl*)GetButton(17));

	m_pTab->AddControl(0,(CGuiControl*)GetImage(21));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(22));
	m_pTab->AddControl(0,(CGuiControl*)GetStatic(23));
	m_pTab->AddControl(0,(CGuiControl*)GetStatic(24));
	m_pTab->AddControl(0,(CGuiControl*)GetCheckBox(25));
	m_pTab->AddControl(0,(CGuiControl*)GetImage(26));
	m_pTab->AddControl(0,(CGuiControl*)GetButton(27));
	

	m_pTab->AddControl(0,(CGuiControl*)GetImage(31));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(32));
	m_pTab->AddControl(0,(CGuiControl*)GetStatic(33));
	m_pTab->AddControl(0,(CGuiControl*)GetStatic(34));
	m_pTab->AddControl(0,(CGuiControl*)GetCheckBox(35));
	m_pTab->AddControl(0,(CGuiControl*)GetImage(36));
	m_pTab->AddControl(0,(CGuiControl*)GetButton(37));

	m_pTab->AddControl(0,(CGuiControl*)GetImage(41));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(42));
	m_pTab->AddControl(0,(CGuiControl*)GetStatic(43));
	m_pTab->AddControl(0,(CGuiControl*)GetStatic(44));
	m_pTab->AddControl(0,(CGuiControl*)GetCheckBox(45));
	m_pTab->AddControl(0,(CGuiControl*)GetImage(46));
	m_pTab->AddControl(0,(CGuiControl*)GetButton(47));

	m_pTab->AddControl(0,(CGuiControl*)GetImage(51));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(52));
	m_pTab->AddControl(0,(CGuiControl*)GetStatic(53));
	m_pTab->AddControl(0,(CGuiControl*)GetStatic(54));
	m_pTab->AddControl(0,(CGuiControl*)GetCheckBox(55));
	m_pTab->AddControl(0,(CGuiControl*)GetImage(56));
	m_pTab->AddControl(0,(CGuiControl*)GetButton(57));

	m_pTab->AddControl(0,(CGuiControl*)GetImage(61));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(62));
	m_pTab->AddControl(0,(CGuiControl*)GetStatic(63));
	m_pTab->AddControl(0,(CGuiControl*)GetStatic(64));
	m_pTab->AddControl(0,(CGuiControl*)GetCheckBox(65));
	m_pTab->AddControl(0,(CGuiControl*)GetImage(66));
	m_pTab->AddControl(0,(CGuiControl*)GetButton(67));


	//界面
	m_pTab->AddControl(1,(CGuiControl*)GetImage(101));
	m_pTab->AddControl(1,(CGuiControl*)GetTable(102));
	m_pTab->AddControl(1,(CGuiControl*)GetStatic(103));
	m_pTab->AddControl(1,(CGuiControl*)GetStatic(104));
	m_pTab->AddControl(1,(CGuiControl*)GetCheckBox(105));
	m_pTab->AddControl(1,(CGuiControl*)GetImage(106));
	m_pTab->AddControl(1,(CGuiControl*)GetButton(107));

	m_pTab->AddControl(1,(CGuiControl*)GetImage(201));
	m_pTab->AddControl(1,(CGuiControl*)GetTable(202));
	m_pTab->AddControl(1,(CGuiControl*)GetStatic(203));
	m_pTab->AddControl(1,(CGuiControl*)GetStatic(204));
	m_pTab->AddControl(1,(CGuiControl*)GetCheckBox(205));
	m_pTab->AddControl(1,(CGuiControl*)GetImage(206));
	m_pTab->AddControl(1,(CGuiControl*)GetButton(207));

	m_pTab->SetVisible(false);

	m_pTab->SetCurItem(0);
	
	type = 0; // 默认时装页
	page = 1;

	mohelevel = 0;
	moheexp = 0;
	mohemaxexp = 5000;
	shizhuangshu = 0;
	pifengshu = 0;
	chibangshu = 0;
	zuoqishu = 0;
	jiemianshu = 0;
	num1 = 0;
	num2 = 0;
	num3 = 0;
	num4 = 0;
	num5 = 0;
	num6 = 0;
	num7 = 0;
	shuaxin();
	FUNCTION_END;
}

bool CGuiMagicBoxDlg::OnGuiEvent(UINT nEvent, UINT nID, CGuiControl *pControl)
{
	FUNCTION_BEGIN;

	if (nEvent == EVENT_CHECKBOX_CHANGED)
	{
		if (type == 0)
		{
			if (!GetCheckBox(nID)->GetChecked())
			{
				// 请求取消佩戴称号
				stSetNotShizhuangUserCmd cmd1;
				CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
				if (pMainRole)
					cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd1.dwOldTempID = 0;
				cmd1.shizhuangid = (page - 1) * 6 + (nID / 10) - 1;
				SEND_USER_CMD(cmd1);

				stGetShizhuangPropertyUserCmd cmd2;
				if (pMainRole)
					cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd2.dwOldTempID = 0;
				SEND_USER_CMD(cmd2);
				return false;
			}
			if (GetCheckBox(15)->GetChecked() && nID == 15)
			{
				if (Shizhuang[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此时装无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetShizhuangUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.shizhuangid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(25)->GetChecked() && nID == 25)
			{
				if (Shizhuang[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此时装无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetShizhuangUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.shizhuangid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(35)->GetChecked() && nID == 35)
			{
				if (Shizhuang[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此时装无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetShizhuangUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.shizhuangid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(45)->GetChecked() && nID == 45)
			{
				if (Shizhuang[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此时装无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetShizhuangUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.shizhuangid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(55)->GetChecked() && nID == 55)
			{
				if (Shizhuang[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此时装无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetShizhuangUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.shizhuangid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(65)->GetChecked() && nID == 65)
			{
				if (Shizhuang[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此时装无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetShizhuangUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.shizhuangid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
			}
		}
		else if (type == 1) // 披风
		{

			if (!GetCheckBox(nID)->GetChecked())
			{
				// 请求取消佩戴披风
				stSetNotPifengUserCmd cmd1;
				CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
				if (pMainRole)
					cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd1.dwOldTempID = 0;
				cmd1.pifengid = (page - 1) * 6 + (nID / 10) - 1;
				SEND_USER_CMD(cmd1);

				stGetShizhuangPropertyUserCmd cmd2;
				if (pMainRole)
					cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd2.dwOldTempID = 0;
				SEND_USER_CMD(cmd2);
				return false;
			}
			if (GetCheckBox(15)->GetChecked() && nID == 15)
			{
				if (Pifeng[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此披风无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetPifengUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.pifengid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(25)->GetChecked() && nID == 25)
			{
				if (Pifeng[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此披风无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetPifengUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.pifengid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(35)->GetChecked() && nID == 35)
			{
				if (Pifeng[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此披风无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetPifengUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.pifengid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(45)->GetChecked() && nID == 45)
			{
				if (Pifeng[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此披风无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetPifengUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.pifengid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(55)->GetChecked() && nID == 55)
			{
				if (Pifeng[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此披风无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetPifengUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.pifengid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(65)->GetChecked() && nID == 65)
			{
				if (Pifeng[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此披风无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetPifengUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.pifengid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
			}
		}
		else if (type == 2) // 翅膀
		{

			if (!GetCheckBox(nID)->GetChecked())
			{
				// 请求取消佩戴翅膀
				stSetNotChibangUserCmd cmd1;
				CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
				if (pMainRole)
					cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd1.dwOldTempID = 0;
				cmd1.chibangid = (page - 1) * 6 + (nID / 10) - 1;
				SEND_USER_CMD(cmd1);

				stGetShizhuangPropertyUserCmd cmd2;
				if (pMainRole)
					cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd2.dwOldTempID = 0;
				SEND_USER_CMD(cmd2);
				return false;
			}
			if (GetCheckBox(15)->GetChecked() && nID == 15)
			{
				if (Chibang[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此翅膀无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetChibangUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.chibangid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(25)->GetChecked() && nID == 25)
			{
				if (Chibang[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此翅膀无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetChibangUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.chibangid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(35)->GetChecked() && nID == 35)
			{
				if (Chibang[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此翅膀无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetChibangUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.chibangid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(45)->GetChecked() && nID == 45)
			{
				if (Chibang[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此翅膀无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetChibangUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.chibangid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(55)->GetChecked() && nID == 55)
			{
				if (Chibang[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此翅膀无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetChibangUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.chibangid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(65)->GetChecked() && nID == 65)
			{
				if (Chibang[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此翅膀无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetChibangUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.chibangid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
			}
		}
		else if (type == 3) // 坐骑
		{

			if (!GetCheckBox(nID)->GetChecked())
			{
				// 请求取消佩戴坐骑
				stSetNotZuoqiUserCmd cmd1;
				CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
				if (pMainRole)
					cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd1.dwOldTempID = 0;
				cmd1.zuoqiid = (page - 1) * 6 + (nID / 10) - 1;
				SEND_USER_CMD(cmd1);

				stGetShizhuangPropertyUserCmd cmd2;
				if (pMainRole)
					cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd2.dwOldTempID = 0;
				SEND_USER_CMD(cmd2);
				return false;
			}
			if (GetCheckBox(15)->GetChecked() && nID == 15)
			{
				if (Zuoqi[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此坐骑无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetZuoqiUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.zuoqiid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(25)->GetChecked() && nID == 25)
			{
				if (Zuoqi[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此坐骑无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetZuoqiUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.zuoqiid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(35)->GetChecked() && nID == 35)
			{
				if (Zuoqi[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此坐骑无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetZuoqiUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.zuoqiid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(45)->GetChecked() && nID == 45)
			{
				if (Zuoqi[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此坐骑无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetZuoqiUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.zuoqiid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(55)->GetChecked() && nID == 55)
			{
				if (Zuoqi[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此坐骑无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetZuoqiUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.zuoqiid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(65)->SetChecked(false);
			}
			if (GetCheckBox(65)->GetChecked() && nID == 65)
			{
				if (Zuoqi[(page - 1) * 6 + (nID / 10) - 1].state != 1)
				{
					GameMessageBox("您未激活此坐骑无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetZuoqiUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.zuoqiid = (page - 1) * 6 + (nID / 10) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(15)->SetChecked(false);
				GetCheckBox(25)->SetChecked(false);
				GetCheckBox(35)->SetChecked(false);
				GetCheckBox(45)->SetChecked(false);
				GetCheckBox(55)->SetChecked(false);
			}
		}
		else if (type == 4) // 界面
		{

			if (!GetCheckBox(nID)->GetChecked())
			{
				// 请求取消佩戴界面
				stSetNotJiemianUserCmd cmd1;
				CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
				if (pMainRole)
					cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd1.dwOldTempID = 0;
				cmd1.jiemianid = (page - 1) * 2 + (nID / 100) - 1;
				SEND_USER_CMD(cmd1);

				stGetShizhuangPropertyUserCmd cmd2;
				if (pMainRole)
					cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd2.dwOldTempID = 0;
				SEND_USER_CMD(cmd2);
				return false;
			}
			if (GetCheckBox(105)->GetChecked() && nID == 105)
			{
				if (Jiemian[(page - 1) * 2 + (nID / 100) - 1].state != 1)
				{
					GameMessageBox("您未激活此界面无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetJiemianUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.jiemianid = (page - 1) * 2 + (nID / 100) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(205)->SetChecked(false);
			
			}
			if (GetCheckBox(205)->GetChecked() && nID == 205)
			{
				if (Jiemian[(page - 1) * 2 + (nID / 100) - 1].state != 1)
				{
					GameMessageBox("您未激活此界面无法穿戴！");
					GetCheckBox(nID)->SetChecked(false);
					return false;
				}
				else
				{
					// 请求佩戴称号
					stSetJiemianUserCmd cmd1;
					CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
					if (pMainRole)
						cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd1.dwOldTempID = 0;
					cmd1.jiemianid = (page - 1) * 2 + (nID / 100) - 1;
					SEND_USER_CMD(cmd1);

					stGetShizhuangPropertyUserCmd cmd2;
					if (pMainRole)
						cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
					else
						cmd2.dwOldTempID = 0;
					SEND_USER_CMD(cmd2);
				}
				GetCheckBox(105)->SetChecked(false);
			}
		}
	}

	if (nEvent == EVENT_BUTTON_CLICKED)
	{
		switch (nID)
		{
		case 1002:
		{
			if (GetGameGuiManager()->m_guiChenghao && ((CGuiDialog *)GetGameGuiManager()->m_guiChenghao)->IsVisible())
			{
				((CGuiDialog *)GetGameGuiManager()->m_guiChenghao)->Close();
			}
			else
			{
				GetGameGuiManager()->AddChenghao();
			}
		}
		break;
		case 1001: // 时装
		{
			page = 1;
			type = 0;
			m_pTab->SetCurItem(0);
			shuaxin();
		}
		break;
		case 1003: // 披风
		{
			page = 1;
			type = 1;
			m_pTab->SetCurItem(0);
			shuaxin();
		}
		break;
		case 1004: // 翅膀
		{
			page = 1;
			type = 2;
			m_pTab->SetCurItem(0);
			shuaxin();
		}
		break;
		case 1005: // 坐骑
		{
			page = 1;
			type = 3;
			m_pTab->SetCurItem(0);
			shuaxin();
		}
		break;
		case 1006: // 界面
		{
			page = 1;
			type = 4;
			m_pTab->SetCurItem(1);
			shuaxin();
		}
		break;
		case 1007: // Touxiang
		case 1008: // Zuji
		{
			GameMessageBox("该功能暂未开放");
		}
		break;
		case 1:
		{
			// Close();
			SetVisible(false);
		}
		break;
		case 3: // 上一页
		{
			if (page > 1)
				page -= 1;
			ClampMagicBoxPage(this);
			shuaxin();
		}
		break;
		case 4: // 下一页
		{
			int maxPage = GetMagicBoxMaxPage(this, type);
			if (page < maxPage)
				page += 1;
			ClampMagicBoxPage(this);
			shuaxin();
		}
		break;
		case 17: // 进阶
		case 27: // 进阶
		case 37: // 进阶
		case 47: // 进阶
		case 57: // 进阶
		case 67: // 进阶

		case 107: // 进阶
		case 207: // 进阶
		{
			if (type == 0)
			{
				CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
				stSetShizhuangJihuoUserCmd cmd1;
				if (pMainRole)
					cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd1.dwOldTempID = 0;
				cmd1.shizhuangid = (page - 1) * 6 + (nID / 10) - 1;
				SEND_USER_CMD(cmd1);

				stGetShizhuangPropertyUserCmd cmd2;
				if (pMainRole)
					cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd2.dwOldTempID = 0;
				SEND_USER_CMD(cmd2);
			}
			else if (type == 1) // 披风
			{
				CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
				stSetPifengJihuoUserCmd cmd1;
				if (pMainRole)
					cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd1.dwOldTempID = 0;
				cmd1.pifengid = (page - 1) * 6 + (nID / 10) - 1;
				SEND_USER_CMD(cmd1);

				stGetShizhuangPropertyUserCmd cmd2;
				if (pMainRole)
					cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd2.dwOldTempID = 0;
				SEND_USER_CMD(cmd2);
			}
			else if (type == 2) // 翅膀
			{
				CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
				stSetChibangJihuoUserCmd cmd1;
				if (pMainRole)
					cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd1.dwOldTempID = 0;
				cmd1.chibangid = (page - 1) * 6 + (nID / 10) - 1;
				SEND_USER_CMD(cmd1);

				stGetShizhuangPropertyUserCmd cmd2;
				if (pMainRole)
					cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd2.dwOldTempID = 0;
				SEND_USER_CMD(cmd2);
			}
			else if (type == 3) // 坐骑
			{
				CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
				stSetZuoqiJihuoUserCmd cmd1;
				if (pMainRole)
					cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd1.dwOldTempID = 0;
				cmd1.zuoqiid = (page - 1) * 6 + (nID / 10) - 1;
				SEND_USER_CMD(cmd1);

				stGetShizhuangPropertyUserCmd cmd2;
				if (pMainRole)
					cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd2.dwOldTempID = 0;
				SEND_USER_CMD(cmd2);
			}
			else if (type == 4) // 界面
			{
				CMainCharacter *pMainRole = (CMainCharacter *)GetScene()->GetMainCharacter();
				stSetJiemianJihuoUserCmd cmd1;
				if (pMainRole)
					cmd1.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd1.dwOldTempID = 0;
				cmd1.jiemianid = (page - 1) * 2 + (nID / 100) - 1;
				SEND_USER_CMD(cmd1);

				stGetShizhuangPropertyUserCmd cmd2;
				if (pMainRole)
					cmd2.dwOldTempID = pMainRole->GetProperty()->userid;
				else
					cmd2.dwOldTempID = 0;
				SEND_USER_CMD(cmd2);
			}
		}
		break;

			break;
		}

		return true;
	}
	return CGuiDialog::OnGuiEvent(nEvent, nID, pControl);

	FUNCTION_END;
}

HRESULT CGuiMagicBoxDlg::OnRender(float fElapsedTime)
{
	FUNCTION_BEGIN;

	HRESULT hr = CGuiDialog::OnRender(fElapsedTime);

	POINT pt;
	POINT pt2 = GetLocation();
	if (type == 0)
	{
		for (int i = 0; i < MAGICBOX_PAGE_SIZE; i++)
		{
			int index = (page - 1) * MAGICBOX_PAGE_SIZE + i;
			if (i < 3)
			{
				pt.x = -150 + 205 * i + pt2.x;
				pt.y = 68 + pt2.y;
			}
			else
			{
				pt.x = -150 + 205 * (i - 3) + pt2.x;
				pt.y = 248 + pt2.y;
			}
			m_AniBody[i].Render(pt.x, pt.y);
			// todo 披风这里缩小
		}
	}
	else if (type == 1)
	{
		for (int i = 0; i < MAGICBOX_PAGE_SIZE; i++)
		{
			int index = (page - 1) * MAGICBOX_PAGE_SIZE + i;
			if (i < 3)
			{
				pt.x = 20 + 205 * i + pt2.x;
				pt.y = 170 + pt2.y;
			}
			else
			{
				pt.x = 20 + 205 * (i - 3) + pt2.x;
				pt.y = 350 + pt2.y;
			}
			stPointF scale(0.4f, 0.4f);
			m_AniBody[i].Render(pt.x, pt.y, NULL, &scale);
			// todo 披风这里缩小
		}
	}
	else if (type == 2)
	{
		for (int i = 0; i < MAGICBOX_PAGE_SIZE; i++)
		{
			int index = (page - 1) * MAGICBOX_PAGE_SIZE + i;
			if (i < 3)
			{
				pt.x = -5 + 205 * i + pt2.x;
				pt.y = 180 + pt2.y;
			}
			else
			{
				pt.x = -5 + 205 * (i - 3) + pt2.x;
				pt.y = 360 + pt2.y;
			}
			stPointF scale(0.5f, 0.5f);
			m_AniBody[i].Render(pt.x, pt.y, NULL, &scale);
		}
	}
	else if (type == 3)
	{
		for (int i = 0; i < 6; i++)
		{

			int w = m_AniBody[i].GetWidth();
			int h = m_AniBody[i].GetHeight();
			if (i < 3)
			{
				
				if(w ==1024 && h==1024)
				{
					pt.x = -100 + 205 * i + pt2.x;
					pt.y = 0 + pt2.y;
				}
				else{
					pt.x = -15 + 205 * i + pt2.x;
					pt.y = 180 + pt2.y;
				}
				
			}
			else
			{
				if(w ==1024 && h==1024)
				{
					pt.x = -100 + 205 * (i - 3) + pt2.x;
					pt.y = 210 + pt2.y;
				}
				else{
					pt.x = -15 + 205 * (i - 3) + pt2.x;
					pt.y = 360 + pt2.y;
				}
			}
			stPointF scale(0.5f, 0.5f);
			m_AniBody[i].Render(pt.x, pt.y, NULL, &scale);
		}
	}
	else if (type == 4)
	{
		for (int i = 0; i < MAGICBOX_JIEMIAN_PAGE_SIZE; i++)
		{
			int index = (page - 1) * MAGICBOX_JIEMIAN_PAGE_SIZE + i;

			pt.x = 310 * i + pt2.x;
			pt.y = 200 + pt2.y;
			stPointF scale(1.0f,1.0f);
			m_AniBody[i].Render(pt.x, pt.y, NULL, &scale);
		}
	}


	return hr;

	FUNCTION_END;
}

void CGuiMagicBoxDlg::OnClose(void)
{
	FUNCTION_BEGIN;

	GetGameGuiManager()->m_guiMagicBox = NULL;

	FUNCTION_END;
}

void CGuiMagicBoxDlg::shuaxin(void)
{
	shizhuangshu = 0;
	for (int i = 0; i < 400; i++)
	{
		if (Shizhuang[i].state == 1)
		{
			shizhuangshu++;
		}
	}
	pifengshu = 0;
	for (int i = 0; i < 100; i++)
	{
		if (Pifeng[i].state == 1)
		{
			pifengshu++;
		}
	}

	chibangshu = 0;
	for (int i = 0; i < 100; i++)
	{
		if (Chibang[i].state == 1)
		{
			chibangshu++;
		}
	}
	zuoqishu = 0;
	for (int i = 0; i < 100; i++)
	{
		if (Zuoqi[i].state == 1)
		{
			zuoqishu++;
		}
	}
	jiemianshu = 0;
	for (int i = 0; i < 100; i++)
	{
		if (Jiemian[i].state == 1)
		{
			jiemianshu++;
		}
	}
	if (type == 0) // 时装
	{
		GetCheckBox(15)->SetChecked(false);
		GetCheckBox(25)->SetChecked(false);
		GetCheckBox(35)->SetChecked(false);
		GetCheckBox(45)->SetChecked(false);
		GetCheckBox(55)->SetChecked(false);
		GetCheckBox(65)->SetChecked(false);

		char msg[256];
		ClampMagicBoxPage(this);
			sprintf(msg, "%d/%d", page, GetMagicBoxMaxPage(this, type));
		GetStatic(6)->SetText(msg);
		
		GetProcess(9)->SetRange(0, mohemaxexp);
		GetProcess(9)->SetPos(moheexp);

		sprintf(msg, "等级：%d级", mohelevel);
		GetStatic(7)->SetText(msg);

		sprintf(msg, "%d/%d", moheexp, mohemaxexp);
		GetStatic(112)->SetText(msg);

		
		sprintf(msg, "当前拥有：[%d] 套时装", shizhuangshu);
		GetStatic(111)->SetText(msg);
		stResourceLocation rlAniBody;
		SetMagicBoxBodyResource(rlAniBody, NULL, 0);
		POINT pt;
		for (int i = 0; i < MAGICBOX_PAGE_SIZE; i++)
		{
			int index = (page - 1) * MAGICBOX_PAGE_SIZE + i;
			if (i < 3)
			{
				pt.x = 10 + 205 * i;
				pt.y = 200;
			}
			else
			{
				pt.x = 10 + 205 * (i - 3);
				pt.y = 380;
			}

			if (index >= GetMagicBoxItemCount(this, type))
			{
				rlAniBody.group = 0;
				rlAniBody.frame = 0;
				m_AniBody[i].Create(&rlAniBody, 16, 19);
				m_AniBody[i].SetColor(COLOR_ARGB(0, 255, 255, 255));
				GetStatic((i + 1) * 10 + 3)->SetText("");
				GetStatic((i + 1) * 10 + 4)->SetText("");
				GetImage((i + 1) * 10 + 6)->SetImage(stResourceLocation("data\interfaces6.gl", 130, 9999));
				continue;
			}
			SetMagicBoxBodyResource(rlAniBody, Shizhuang[index].pic, Shizhuang[index].bodyNum);
			m_AniBody[i].Create(&rlAniBody, 16, 19);
			m_AniBody[i].SetLoopPlay(true);
			m_AniBody[i].SetSpeed(200);

			if (Shizhuang[index].state == 1)
			{
				m_AniBody[i].SetColor(COLOR_ARGB(255, 255, 255, 255));
			}
			else
			{
				m_AniBody[i].SetColor(COLOR_ARGB(50, 255, 255, 255));
			}

			GetStatic((i + 1) * 10 + 3)->SetText(Shizhuang[index].name);
			GetStatic((i + 1) * 10 + 4)->SetText(Shizhuang[index].strDesc);
			if (Shizhuang[index].level >= 1)
			{
				GetImage((i + 1) * 10 + 6)->SetImage(stResourceLocation("data\\interfaces6.gl", 130, Shizhuang[index].level));
			}
			else
			{
				GetImage((i + 1) * 10 + 6)->SetImage(stResourceLocation("data\\interfaces6.gl", 130, 9999));
			}

			if (shizhuang_select == index)
			{
				GetCheckBox(((i + 1) * 10) + 5)->SetChecked(true);
			}
		}
	}
	else if (type == 1) // 披风
	{
		GetCheckBox(15)->SetChecked(false);
		GetCheckBox(25)->SetChecked(false);
		GetCheckBox(35)->SetChecked(false);
		GetCheckBox(45)->SetChecked(false);
		GetCheckBox(55)->SetChecked(false);
		GetCheckBox(65)->SetChecked(false);

		char msg[256];
		ClampMagicBoxPage(this);
			sprintf(msg, "%d/%d", page, GetMagicBoxMaxPage(this, type));
		GetStatic(6)->SetText(msg);

		GetProcess(9)->SetRange(0, mohemaxexp);
		GetProcess(9)->SetPos(moheexp);

		sprintf(msg, "等级：%d级", mohelevel);
		GetStatic(7)->SetText(msg);

		sprintf(msg, "%d/%d", moheexp, mohemaxexp);
		GetStatic(112)->SetText(msg);

		pifengshu = 0;
		for (int i = 0; i < 100; i++)
		{
			if (Pifeng[i].state == 1)
			{
				pifengshu++;
			}
		}
		sprintf(msg, "当前拥有：[%d] 套披风", pifengshu);
		GetStatic(111)->SetText(msg);
		stResourceLocation rlAniBody;
		rlAniBody.SetFileName("data\\items.gl");
		POINT pt;
		for (int i = 0; i < MAGICBOX_PAGE_SIZE; i++)
		{
			int index = (page - 1) * MAGICBOX_PAGE_SIZE + i;
			if (i < 3)
			{
				pt.x = 10 + 205 * i;
				pt.y = 200;
			}
			else
			{
				pt.x = 10 + 205 * (i - 3);
				pt.y = 380;
			}

			if (index >= GetMagicBoxItemCount(this, type))
			{
				rlAniBody.group = 0;
				rlAniBody.frame = 0;
				m_AniBody[i].Create(&rlAniBody);
				m_AniBody[i].SetColor(COLOR_ARGB(0, 255, 255, 255));
				GetStatic((i + 1) * 10 + 3)->SetText("");
				GetStatic((i + 1) * 10 + 4)->SetText("");
				GetImage((i + 1) * 10 + 6)->SetImage(stResourceLocation("data\interfaces6.gl", 130, 9999));
				continue;
			}
			rlAniBody.group = 3;
			rlAniBody.frame = Pifeng[index].itemNum;
			m_AniBody[i].Create(&rlAniBody);
			m_AniBody[i].SetLoopPlay(true);
			m_AniBody[i].SetSpeed(200);

			if (Pifeng[index].state == 1)
			{
				m_AniBody[i].SetColor(COLOR_ARGB(255, 255, 255, 255));
			}
			else
			{
				m_AniBody[i].SetColor(COLOR_ARGB(50, 255, 255, 255));
			}

			GetStatic((i + 1) * 10 + 3)->SetText(Pifeng[index].name);
			GetStatic((i + 1) * 10 + 4)->SetText(Pifeng[index].strDesc);
			if (Pifeng[index].level >= 1)
			{
				GetImage((i + 1) * 10 + 6)->SetImage(stResourceLocation("data\\interfaces6.gl", 130, Pifeng[index].level));
			}
			else
			{
				GetImage((i + 1) * 10 + 6)->SetImage(stResourceLocation("data\\interfaces6.gl", 130, 9999));
			}

			if (pifeng_select == index)
			{
				GetCheckBox(((i + 1) * 10) + 5)->SetChecked(true);
			}
		}
	}
	else if (type == 2) // 翅膀
	{
		GetCheckBox(15)->SetChecked(false);
		GetCheckBox(25)->SetChecked(false);
		GetCheckBox(35)->SetChecked(false);
		GetCheckBox(45)->SetChecked(false);
		GetCheckBox(55)->SetChecked(false);
		GetCheckBox(65)->SetChecked(false);

		char msg[256];
		ClampMagicBoxPage(this);
			sprintf(msg, "%d/%d", page, GetMagicBoxMaxPage(this, type));
		GetStatic(6)->SetText(msg);

		GetProcess(9)->SetRange(0, mohemaxexp);
		GetProcess(9)->SetPos(moheexp);

		sprintf(msg, "等级：%d级", mohelevel);
		GetStatic(7)->SetText(msg);

		sprintf(msg, "%d/%d", moheexp, mohemaxexp);
		GetStatic(112)->SetText(msg);

		chibangshu = 0;
		for (int i = 0; i < 100; i++)
		{
			if (Chibang[i].state == 1)
			{
				chibangshu++;
			}
		}
		sprintf(msg, "当前拥有：[%d] 套翅膀", chibangshu);
		GetStatic(111)->SetText(msg);
		stResourceLocation rlAniBody;
		rlAniBody.SetFileName("data\\items.gl");
		POINT pt;
		for (int i = 0; i < MAGICBOX_PAGE_SIZE; i++)
		{
			int index = (page - 1) * MAGICBOX_PAGE_SIZE + i;
			if (i < 3)
			{
				pt.x = 10 + 205 * i;
				pt.y = 200;
			}
			else
			{
				pt.x = 10 + 205 * (i - 3);
				pt.y = 380;
			}

			if (index >= GetMagicBoxItemCount(this, type))
			{
				rlAniBody.group = 0;
				rlAniBody.frame = 0;
				m_AniBody[i].Create(&rlAniBody);
				m_AniBody[i].SetColor(COLOR_ARGB(0, 255, 255, 255));
				GetStatic((i + 1) * 10 + 3)->SetText("");
				GetStatic((i + 1) * 10 + 4)->SetText("");
				GetImage((i + 1) * 10 + 6)->SetImage(stResourceLocation("data\interfaces6.gl", 130, 9999));
				continue;
			}
			rlAniBody.group = 3;
			rlAniBody.frame = Chibang[index].itemNum;
			m_AniBody[i].Create(&rlAniBody);
			m_AniBody[i].SetLoopPlay(true);
			m_AniBody[i].SetSpeed(200);

			if (Chibang[index].state == 1)
			{
				m_AniBody[i].SetColor(COLOR_ARGB(255, 255, 255, 255));
			}
			else
			{
				m_AniBody[i].SetColor(COLOR_ARGB(50, 255, 255, 255));
			}

			GetStatic((i + 1) * 10 + 3)->SetText(Chibang[index].name);
			GetStatic((i + 1) * 10 + 4)->SetText(Chibang[index].strDesc);
			if (Chibang[index].level >= 1)
			{
				GetImage((i + 1) * 10 + 6)->SetImage(stResourceLocation("data\\interfaces6.gl", 130, Chibang[index].level));
			}
			else
			{
				GetImage((i + 1) * 10 + 6)->SetImage(stResourceLocation("data\\interfaces6.gl", 130, 9999));
			}

			if (chibang_select == index)
			{
				GetCheckBox(((i + 1) * 10) + 5)->SetChecked(true);
			}
		}
	}
	else if (type == 3) // 坐骑
	{
		GetCheckBox(15)->SetChecked(false);
		GetCheckBox(25)->SetChecked(false);
		GetCheckBox(35)->SetChecked(false);
		GetCheckBox(45)->SetChecked(false);
		GetCheckBox(55)->SetChecked(false);
		GetCheckBox(65)->SetChecked(false);

		char msg[256];
		ClampMagicBoxPage(this);
			sprintf(msg, "%d/%d", page, GetMagicBoxMaxPage(this, type));
		GetStatic(6)->SetText(msg);

		GetProcess(9)->SetRange(0, mohemaxexp);
		GetProcess(9)->SetPos(moheexp);

		sprintf(msg, "等级：%d级", mohelevel);
		GetStatic(7)->SetText(msg);

		sprintf(msg, "%d/%d", moheexp, mohemaxexp);
		GetStatic(112)->SetText(msg);

		zuoqishu = 0;
		for (int i = 0; i < 100; i++)
		{
			if (Zuoqi[i].state == 1)
			{
				zuoqishu++;
			}
		}
		sprintf(msg, "当前拥有：[%d] 套坐骑", zuoqishu);
		GetStatic(111)->SetText(msg);
		stResourceLocation rlAniBody;
		rlAniBody.SetFileName("data\\npc11.gl");
		POINT pt;
		for (int i = 0; i < MAGICBOX_PAGE_SIZE; i++)
		{
			int index = (page - 1) * MAGICBOX_PAGE_SIZE + i;
			if (i < 3)
			{
				pt.x = 10 + 205 * i;
				pt.y = 200;
			}
			else
			{
				pt.x = 10 + 205 * (i - 3);
				pt.y = 380;
			}

			if (index >= GetMagicBoxItemCount(this, type))
			{
				rlAniBody.group = 0;
				rlAniBody.frame = 0;
				m_AniBody[i].Create(&rlAniBody, 10, 14);
				m_AniBody[i].SetColor(COLOR_ARGB(0, 255, 255, 255));
				GetStatic((i + 1) * 10 + 3)->SetText("");
				GetStatic((i + 1) * 10 + 4)->SetText("");
				GetImage((i + 1) * 10 + 6)->SetImage(stResourceLocation("data\interfaces6.gl", 130, 9999));
				continue;
			}
			rlAniBody.group = Zuoqi[index].npcNum;
			rlAniBody.frame = 14;
			m_AniBody[i].Create(&rlAniBody, 10, 14);
			m_AniBody[i].SetLoopPlay(true);
			m_AniBody[i].SetSpeed(200);

			if (Zuoqi[index].state == 1)
			{
				m_AniBody[i].SetColor(COLOR_ARGB(255, 255, 255, 255));
			}
			else
			{
				m_AniBody[i].SetColor(COLOR_ARGB(50, 255, 255, 255));
			}

			GetStatic((i + 1) * 10 + 3)->SetText(Zuoqi[index].name);
			GetStatic((i + 1) * 10 + 4)->SetText(Zuoqi[index].strDesc);
			if (Zuoqi[index].level >= 1)
			{
				GetImage((i + 1) * 10 + 6)->SetImage(stResourceLocation("data\\interfaces6.gl", 130, Zuoqi[index].level));
			}
			else
			{
				GetImage((i + 1) * 10 + 6)->SetImage(stResourceLocation("data\\interfaces6.gl", 130, 9999));
			}

			if (zuoqi_select == index)
			{
				GetCheckBox(((i + 1) * 10) + 5)->SetChecked(true);
			}
		}
	}
	else if (type == 4) // 界面
	{
		GetCheckBox(105)->SetChecked(false);
		GetCheckBox(205)->SetChecked(false);
		

		char msg[256];
		ClampMagicBoxPage(this);
			sprintf(msg, "%d/%d", page, GetMagicBoxMaxPage(this, type));
		GetStatic(6)->SetText(msg);

		GetProcess(9)->SetRange(0, mohemaxexp);
		GetProcess(9)->SetPos(moheexp);

		sprintf(msg, "等级：%d级", mohelevel);
		GetStatic(7)->SetText(msg);

		sprintf(msg, "%d/%d", moheexp, mohemaxexp);
		GetStatic(112)->SetText(msg);

		jiemianshu = 0;
		for (int i = 0; i < 100; i++)
		{
			if (Jiemian[i].state == 1)
			{
				jiemianshu++;
			}
		}
		sprintf(msg, "当前拥有：[%d] 套界面", jiemianshu);
		GetStatic(111)->SetText(msg);
		stResourceLocation rlAniBody;
		rlAniBody.SetFileName("data\\activity.gl");
		POINT pt;
		for (int i = 0; i < MAGICBOX_JIEMIAN_PAGE_SIZE; i++)
		{
			int index = (page - 1) * MAGICBOX_JIEMIAN_PAGE_SIZE + i;
			
			pt.x = 20 + 300 * i;
			pt.y = 240;
			

			if (index >= GetMagicBoxItemCount(this, type))
			{
				rlAniBody.group = 0;
				rlAniBody.frame = 0;
				m_AniBody[i].Create(&rlAniBody);
				m_AniBody[i].SetColor(COLOR_ARGB(0, 255, 255, 255));
				GetStatic((i + 1) * 100 + 3)->SetText("");
				GetStatic((i + 1) * 100 + 4)->SetText("");
				GetImage((i + 1) * 100 + 6)->SetImage(stResourceLocation("data\interfaces6.gl", 130, 9999));
				continue;
			}
			rlAniBody.group = Jiemian[index].activityNum;
			rlAniBody.frame = 3;

			m_AniBody[i].Create(&rlAniBody);
			m_AniBody[i].SetLoopPlay(true);
			m_AniBody[i].SetSpeed(50);

			if (Jiemian[index].state == 1)
			{
				m_AniBody[i].SetColor(COLOR_ARGB(255, 255, 255, 255));
			}
			else
			{
				m_AniBody[i].SetColor(COLOR_ARGB(50, 255, 255, 255));
			}

			GetStatic((i + 1) * 100 + 3)->SetText(Jiemian[index].name);
			GetStatic((i + 1) * 100 + 4)->SetText(Jiemian[index].strDesc);
			if (Jiemian[index].level >= 1)
			{
				GetImage((i + 1) * 100 + 6)->SetImage(stResourceLocation("data\\interfaces6.gl", 130, Jiemian[index].level));
			}
			else
			{
				GetImage((i + 1) * 100 + 6)->SetImage(stResourceLocation("data\\interfaces6.gl", 130, 9999));
			}

			if (jiemian_select == index)
			{
				GetCheckBox(((i + 1) * 100) + 5)->SetChecked(true);
			}
		}
	}
}
