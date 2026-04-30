#ifndef _GAME_PUBLIC_H
#define _GAME_PUBLIC_H

#include "../engine/include/engine.h"
#include "../luabind/public.h"
#include "../Zlib/zlib.h"
#include "command.h"
#include "client.h"
#include "table.h"
#include <stack>
 
#include <string> // 需要 std::string
#include <cstdio>  // 需要 _snprintf

#ifndef radian2angle
#define radian2angle(X) ( (X) * 180.f/ PI )
#endif

#ifndef angle2radian
#define angle2radian(X) ( (X) * PI / 180.f )
#endif

#ifdef _DEBUG
#else

#ifdef RELEASE_TEST
#define _REGISTER_ERROR
#endif

#endif

#ifdef _REGISTER_ERROR

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
//#define FUNCTION_BEGIN \
//	try{\
//
//#define FUNCTION_END } \
//	catch(...) {\
//		Engine_WriteLogF( "exception: %s(%d)\n" , __FILE__ , __LINE__ );\
//		throw; \
//	}\

struct stCallStackInfo{
	const char* filename;
	const char* sTime;
	int			lineno;
};

extern __declspec(thread) stCallStackInfo g_callStack[256];
extern __declspec(thread) int			  g_callStackIndex;

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 */
class CSetCallStack{
	bool bSetStack;
public:
	CSetCallStack( const char* filename,int lineno,const char* pszTime ){
		if(g_callStackIndex < (int)count_of(g_callStack))
		{
			bSetStack = true;
			++g_callStackIndex;
			g_callStack[g_callStackIndex].filename = filename;
			g_callStack[g_callStackIndex].lineno = lineno;
			//g_callStack[g_callStackIndex].sDate = pszDate;
			g_callStack[g_callStackIndex].sTime = pszTime;
		}
		else bSetStack = false;
	}
	~CSetCallStack()
	{
		if(bSetStack) --g_callStackIndex;
	}
};

#if defined _DEBUG || defined RELEASE_TEST
	#define FUNCTION_BEGIN CSetCallStack theRegCallStack(__FUNCTION__,0,__TIMESTAMP__)
#else
	#define FUNCTION_BEGIN CSetCallStack theRegCallStack(__FILE__,__LINE__,__TIMESTAMP__)
#endif

#define FUNCTION_END 


#else

#define FUNCTION_BEGIN 
#define FUNCTION_END 

#endif

extern int RegErrorCallStack(DWORD code,LPEXCEPTION_POINTERS pError);

template < typename T>
inline void _Yjt_Monitoring_Int(T t,const char *FileName,int Line,const char *pszText)
{	
#ifdef RELEASE_TEST
	Engine_WriteLog(avar("%s(%d)\t%s[%d]\n",FileName,Line,pszText,t));
#else
	TRACE(avar("%s(%d)\t%s[%d]\n",FileName,Line,pszText,t));
#endif
}

template < typename T>
inline void _Yjt_Monitoring_Char(T t,const char *FileName,int Line,const char *pszText)
{	
#ifdef RELEASE_TEST
	Engine_WriteLog(avar("%s(%d)\t%s[%c]\n",FileName,Line,pszText,t));
#else
	TRACE(avar("%s(%d)\t%s[%c]\n",FileName,Line,pszText,t));
#endif
}

inline void _Yjt_Monitoring_String(const char* t,const char *FileName,int Line,const char *pszText)
{
#ifdef RELEASE_TEST
	Engine_WriteLog(avar("%s(%d)\t%s[%s]\n",FileName,Line,pszText,t));
#else
	TRACE(avar("%s(%d)\t%s[%s]\n",FileName,Line,pszText,t));
#endif
}

inline void _Yjt_Monitoring_Bool(bool b,const char *FileName,int Line,const char *pszText)
{
#ifdef RELEASE_TEST
	if(b) Engine_WriteLog(avar("%s(%d)\t%s[true]\n",FileName,Line,pszText));
	else  Engine_WriteLog(avar("%s(%d)\t%s[false]\n",FileName,Line,pszText));
#else
	if(b) TRACE(avar("%s(%d)\t%s[true]\n",FileName,Line,pszText));
	else  TRACE(avar("%s(%d)\t%s[false]\n",FileName,Line,pszText));
#endif
}

//监视变量
#define Yjt_Monitoring_Int(x)		_Yjt_Monitoring_Int(x,__FILE__,__LINE__,#x)
#define Yjt_Monitoring_Char(x)		_Yjt_Monitoring_Char(x,__FILE__,__LINE__,#x)
#define Yjt_Monitoring_String(x)	_Yjt_Monitoring_String(x,__FILE__,__LINE__,#x)
#define Yjt_Monitoring_Bool(x)		_Yjt_Monitoring_Bool(x,__FILE__,__LINE__,#x)
#define Yjt_Monitoring_Log(x)		_Yjt_Monitoring_String(x,__FILE__,__LINE__,"")

#define AssertCreateCtrl(ctrl,str) if( NULL == ctrl ) {Assert(0);Engine_WriteLog(str);GetGameGuiManager()->AddMessageBox(str);Close();return;}

#define USE_EXECRYPT_API
#ifdef USE_EXECRYPT_API
#include "./execryptor.h"
#define FUNCTION_BEGIN_NEW \
	CRYPT_START FUNCTION_BEGIN


#define FUNCTION_END_NEW \
	FUNCTION_END CRYPT_END
#else
#define FUNCTION_BEGIN_NEW \
	FUNCTION_BEGIN

#define FUNCTION_END_NEW \
	FUNCTION_END
#endif

extern bool GetFontPointList(const WORD& cName, stPointI* pPointList, int& numPoint);
 
 
 
 // by=>friday 大数值格式化函数，带单位，自动去除末尾无效的0
 inline std::string FormatLargeNumber(uint64_t num){
    char buffer[80];
    
    if (num < 10000ULL) { // 小于1万
        _snprintf(buffer, sizeof(buffer)-1, "%I64u", num);
    } else {
        double val;
        const char* unit;
        
        // by=>friday 确定单位和除数
        if (num < 100000000ULL) { // 1万 到 1亿-1
            val = static_cast<double>(num) / 10000.0;
            unit = "万";
        } else if (num < 1000000000000ULL) { // 1亿 到 1兆-1 (1万亿)
            val = static_cast<double>(num) / 100000000.0;
            unit = "亿";
        } else if (num < 10000000000000000ULL) { // 1兆 到 1京-1 (1万兆)
            val = static_cast<double>(num) / 1000000000000.0;
            unit = "兆";
        } else { // 大于等于1京
            val = static_cast<double>(num) / 10000000000000000.0;
            unit = "京";
        }
        
        // by=>friday 格式化并去除末尾无效的0
        _snprintf(buffer, sizeof(buffer)-1, "%.4f", val);
        char* p = buffer + strlen(buffer) - 1;
        while (p > buffer && *p == '0') {
            *p = '\0';
            p--;
        }
        if (p > buffer && *p == '.') { // 如果小数点后全是0，去除小数点
            *p = '\0';
        }
        strcat(buffer, unit); // 添加单位
    }
    
    buffer[sizeof(buffer)-1] = '\0'; // 确保字符串以null结尾
    return std::string(buffer);
}




//#include "Game.h"
#include "../gui/include/guiTypes.h"
#include "GameGuiManager.h"
//#include "GameScene.h"
//#include "MainCharacter.h"
#endif


