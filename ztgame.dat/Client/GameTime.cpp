#include "public.h"
#include ".\gametime.h"
#include "./GameScene.h"
#include "./MainCharacter.h"

CGameTime g_GameTime;


const DWORD c_days1[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
const DWORD c_days2[12] = {31,29,31,30,31,30,31,31,30,31,30,31};

const DWORD c_yearDays1 = c_days1[0] + c_days1[1] + c_days1[2] + c_days1[3] + c_days1[4] + c_days1[5] + c_days1[6] + c_days1[7] + c_days1[8] + c_days1[9] + c_days1[10] + c_days1[11];
const DWORD c_yearDays2 = c_days2[0] + c_days2[1] + c_days2[2] + c_days2[3] + c_days2[4] + c_days2[5] + c_days2[6] + c_days2[7] + c_days2[8] + c_days2[9] + c_days2[10] + c_days2[11];

stGameDateTime GetRandDateTime()
{
	stGameDateTime st;
	st.tm_hour = rand()%24;
	st.tm_mday = rand()%30;
	st.tm_year = rand()%2005;
	st.tm_mon  = rand()%12;
	return st;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param second : 描述
 * \return 返回值的描述
 */
inline QWORD timeSecondToGameSecond(QWORD second)
{
	FUNCTION_BEGIN;

	// 现实世界的 2 天等于游戏世界的 1 年
	// 得到游戏世界的秒数
	return second * (366 / 3);

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param msec : 描述
 * \return 返回值的描述
 */
inline QWORD timeMSecToGameSec(QWORD msec)
{
	FUNCTION_BEGIN;

	return (msec * (366 / 3))/1000;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param year : 描述
 * \param days : 描述
 * \param mon : 描述
 * \param day : 描述
 * \return 返回值的描述
 */
static void GetMonDay(DWORD year,DWORD days,DWORD & mon,DWORD & day)
{
	FUNCTION_BEGIN;

	mon = 0;
	if((year % 4) == 0)
	{
		// 二月有29天
		for(size_t i=0; i< 12;++i)
		{
			if(days >= c_days2[i])
			{
				days -= c_days2[i];
				mon ++;
			}
			else
			{
				day = days;
				break;
			}
		}
	}
	else
	{
		// 二月有28天
		for(size_t i=0; i< 12;++i)
		{
			if(days >= c_days1[i])
			{
				days -= c_days1[i];
				mon ++;
			}
			else
			{
				day = days;
				break;
			}
		}
	}

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param year : 描述
 * \param mon : 描述
 * \param day : 描述
 * \return 返回值的描述
 */
static DWORD GetDays(DWORD year,DWORD & mon,DWORD & day)
{
	FUNCTION_BEGIN;

	DWORD days = 0;
	if((year % 4) == 0)
	{
		// 二月有29天
		for(size_t i=0; i< mon;++i)
		{
			days += c_days2[i];
		}
	}
	else
	{
		// 二月有28天
		for(size_t i=0; i< mon;++i)
		{
			days += c_days1[i];
		}
	}
	days += day;
	return days;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void timeGetGameTime(stGameDateTime * pRet,QWORD qwSecond/*系统时间(服务器运行了多少秒)*/)
{
	FUNCTION_BEGIN;

	timeGetTimeData(pRet,timeSecondToGameSecond(qwSecond));

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void timeGetTimeData(stGameDateTime * pRet,QWORD qwSecond/*(秒)*/)
{
	FUNCTION_BEGIN;

	// 计算日期时间
	pRet->tm_sec = (qwSecond % 60) ;
	pRet->tm_min = (qwSecond / 60) % 60 ;
	pRet->tm_hour = (qwSecond / (60 * 60) ) % 24;
	QWORD days = (qwSecond / (60 * 60 * 24) );

	int days2 = days % (c_yearDays1*3 + c_yearDays2);
	int year1 = 0;
	if(days2 >= c_yearDays2) 
	{
		days2 -= c_yearDays2;
		year1++;

		if(days2 >= c_yearDays1) 
		{
			days2 -= c_yearDays1;
			year1++;

			if(days2 >= c_yearDays1) 
			{
				days2 -= c_yearDays1;
				year1++;
			}
		}
	}

	pRet->tm_year = days/(c_yearDays1*3 + c_yearDays2) * 4 + year1;

	GetMonDay(pRet->tm_year,days2,pRet->tm_mon,pRet->tm_mday);

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param dt : 描述
 * \return 返回值的描述
 */
QWORD timeGetTimeSecond(stGameDateTime * dt)
{
	FUNCTION_BEGIN;

	QWORD days;
	days = dt->tm_year/4 * (c_yearDays1*3 + c_yearDays2);
	long year1 = dt->tm_year%4;
	if(year1 > 0)
	{
		year1 --;
		days += c_yearDays2;
		if(year1 > 0)
		{
			year1 --;
			days += c_yearDays1;

			if(year1 > 0)
			{
				year1 --;
				days += c_yearDays1;
			}
		}
	}

	days += GetDays(dt->tm_year,dt->tm_mon,dt->tm_mday);

	return days * (24 * 60 * 60 ) + dt->tm_hour * 60 * 60 + dt->tm_min * 60 + dt->tm_sec;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param dwSecond : 描述
 * \return 返回值的描述
 */
const char* GetTimeString(QWORD dwSecond)
{
	FUNCTION_BEGIN;

	stGameDateTime st;
	timeGetGameTime(&st,dwSecond);
	static char szTime[256];
	sprintf(szTime,"%04u年%02u月%02u日%02u时%02u分",st.tm_year,st.tm_mon,st.tm_mday,st.tm_hour,st.tm_min);
	return szTime;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */

const char* CGameTime::GetZSTimeString()
{
	FUNCTION_BEGIN;

	__time64_t long_time((xtimeGetTime() - m_dwTimer) / 1000 + m_qwServerTime);
	tm* server_time = _localtime64(&long_time);
	//auto server_time = _localtime64(&long_time);

	static char szTime[256];
	//sprintf(szTime, "%02u:%02u:%02u", server_time->tm_hour, server_time->tm_min, server_time->tm_sec);
	sprintf(szTime, " %04u年%02u月%02u日%02u时%02u分%02u秒", server_time->tm_year+ 1900, server_time->tm_mon+1, server_time->tm_mday, server_time->tm_hour, server_time->tm_min, server_time->tm_sec);
	return szTime;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
const char* CGameTime::GetZTTimeString()
{
	FUNCTION_BEGIN;

	__time64_t long_time((xtimeGetTime() - m_dwTimer) / 1000 + m_qwServerTime);
	tm* server_time = _localtime64(&long_time);
	//auto server_time = _localtime64(&long_time);

	static char szTime[256];
	sprintf(szTime, "%02u:%02u:%02u", server_time->tm_hour, server_time->tm_min, server_time->tm_sec);
	//sprintf(szTime, " 服务器时间: %04u年%02u月%02u日%02u时%02u分%02u秒", server_time->tm_year+ 1900, server_time->tm_mon+1, server_time->tm_mday, server_time->tm_hour, server_time->tm_min, server_time->tm_sec);
	return szTime;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
CGameTime::CGameTime(void)
: m_qwLastConfirmTimer(0)
, m_dwTimer(0)
, m_nTimeInit(0)
, m_bConnWay(true)
{
	FUNCTION_BEGIN;

	memset(&m_time,0,sizeof(m_time));
	m_time = GetRandDateTime();
	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
CGameTime::~CGameTime(void)
{
	FUNCTION_BEGIN;


	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CGameTime::InitTimestamp()
{
	FUNCTION_BEGIN;

	m_nTimeInit = xtimeGetTime();

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
DWORD CGameTime::GetTimestamp()
{
	FUNCTION_BEGIN;

	//return  xtimeGetTime() - m_nTimeInit;

	return time(NULL);  //封杀网关

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pCmd : 描述
 * \return 返回值的描述
 */
void  CGameTime::SetGameTime(stGameTimeTimerUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	m_qwServerTime = pCmd->qwServerTime;
	m_qwLastConfirmTimer = pCmd->qwGameTime;
	m_dwTimer = xtimeGetTime();
	TRACE("设置游戏时间: %llu\n", pCmd->qwGameTime);

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CGameTime::Confirm()
{
	FUNCTION_BEGIN;

	if (m_bConnWay==false)
	{
		m_bConnWay = true;
		InitTimestamp();
	}

	stUserGameTimeTimerUserCmd cmd;
	cmd.dwUserTempID = 0;
	if(GetScene()->GetMainCharacter())
		cmd.dwUserTempID = GetScene()->GetMainCharacter()->GetID();

	cmd.qwGameTime = (xtimeGetTime()-m_dwTimer)/1000 + m_qwLastConfirmTimer;

	SEND_USER_CMD(cmd);

	


	TRACE("发送时间指令: %llu\n", cmd.qwGameTime);

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CGameTime::Run()
{
	FUNCTION_BEGIN;

	QWORD gametime = (xtimeGetTime()-m_dwTimer)/1000 + m_qwLastConfirmTimer;
	m_qwSecond = timeSecondToGameSecond(gametime);
	timeGetTimeData(&m_time,m_qwSecond);
	//Assert(timeGetTimeSecond(&m_time) == m_qwSecond);

	FUNCTION_END;
}


/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	描述: 		  获得一年中的天数
*/
int	  CGameTime::GetDayNum()
{
	stGameDateTime t;
	memset(&t,0,sizeof(t));
	t.tm_year = m_time.tm_year;
	QWORD s1 = timeGetTimeSecond(&t);
	s1 = m_qwSecond-s1;
	return s1 / (60 * 60 * 24);
}


bool CGameTime::IsDayNumChanged()
{
	static int oldDayNum = GetDayNum();
	if ( GetDayNum() != oldDayNum )
	{
		oldDayNum = GetDayNum();
		return true;		
	}

	return false;
}

//by=>friday 添加获取当前服务器时间的方法
/**
 * \brief 获取当前服务器时间戳
 * 
 * 根据服务器推送的时间和客户端计时器计算当前服务器时间
 * 
 * \return 当前服务器时间戳
 */
time_t CGameTime::GetCurrentServerTime()
{
	FUNCTION_BEGIN;
	
	if(m_dwTimer == 0) //还没收到服务器时间，返回本地时间
	{
		return time(NULL);
	}
	
	//计算当前服务器时间 = 服务器推送时间 + (当前客户端时间 - 推送时的客户端时间)
	time_t current_server_time = m_qwServerTime + (xtimeGetTime() - m_dwTimer) / 1000;
	return current_server_time;
	
	FUNCTION_END;
}

/**
 * \brief 获取当前服务器时间结构
 * 
 * 将服务器时间转换为tm结构
 * 
 * \param pTm 输出的时间结构指针
 */
void CGameTime::GetCurrentServerTimeTM(struct tm* pTm)
{
	FUNCTION_BEGIN;
	
	if(pTm == NULL) return;
	
	time_t server_time = GetCurrentServerTime();
	struct tm* tm_ptr = localtime(&server_time);
	if(tm_ptr)
	{
		*pTm = *tm_ptr;
	}
	
	FUNCTION_END;
}