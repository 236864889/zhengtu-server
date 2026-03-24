#ifndef _GM_TOOL_COMMAND_
#define _GM_TOOL_COMMAND_

#include "zNullCmd.h"
#pragma pack(1)

namespace Cmd
{
	namespace GmTool
	{
		const BYTE CMD_GMTOOL = 128;

		//聊天消息,Gateway->Super->Info->GmTool
		const BYTE PARA_CHAT_GMTOOL = 1;
		struct t_Chat_GmTool : t_NullCmd
		{
			GameZone_t zone;
			char userName[MAX_NAMESIZE];
			char gmName[MAX_NAMESIZE];
			DWORD countryID;
			DWORD sceneID;
			DWORD dwType;                   /**< 消息类型 */
			char content[256];    /**< 对话内容 */
			char server[MAX_NAMESIZE];
			BYTE size;                              // 物品数量
			stTradeObject tobject_array[0]; // 交易物品列表
			t_Chat_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_CHAT_GMTOOL)
			{
				bzero(userName, sizeof(userName));
				bzero(content, sizeof(content));
				bzero(server, sizeof(server));
				size = 0;
			}       
		};

		//GM定单,Scene->Super->Info
		const BYTE PARA_MSG_GMTOOL = 2;
		struct t_Msg_GmTool : t_NullCmd
		{
			GameZone_t zone;
			char userName[MAX_NAMESIZE];
			DWORD accid;
			char country[MAX_NAMESIZE];
			DWORD type;
			char content[512];
			WORD contact;
			char tele[101];
			int isDeal;
			DWORD startTime;
			char gmName[32];
			t_Msg_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_MSG_GMTOOL)
			{
				bzero(userName, sizeof(userName));
				bzero(country, sizeof(country));
				bzero(content, sizeof(content));
				bzero(tele, sizeof(tele));
				isDeal = 0;
				startTime = 0;
				bzero(gmName, sizeof(gmName));
			}
		};

		//GM定单回复,GmTool->Info->Super->Session
		const BYTE PARA_MSG_REPLY_GMTOOL = 3;
		struct t_Msg_Reply_GmTool : t_NullCmd
		{
			GameZone_t zone;
			char userName[MAX_NAMESIZE];
			char content[32];
			char gmName[32];
			char reply[256-32];
			DWORD userID;
			t_Msg_Reply_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_MSG_REPLY_GMTOOL)
			{
				bzero(userName, sizeof(userName));
				bzero(content, sizeof(content));
				bzero(gmName, sizeof(gmName));
				bzero(reply, sizeof(reply));
				userID = 0;
			}
		};

		//GM处罚，GmTool->Info->Super->Scene->Super->Info
		const BYTE PARA_PUNISH_GMTOOL = 4;
		struct t_Punish_GmTool : t_NullCmd
		{
			GameZone_t zone;
			char userName[MAX_NAMESIZE];
			char country[MAX_NAMESIZE];
			char server[MAX_NAMESIZE];
			char GM[MAX_NAMESIZE];
			char reason[128];
			WORD operation;
			int delay;
			WORD level;
			DWORD accid;
			DWORD startTime;
			t_Punish_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_PUNISH_GMTOOL)
			{
				bzero(userName, sizeof(userName));
				bzero(country, sizeof(country));
				bzero(server, sizeof(server));
				bzero(reason, sizeof(reason));
				operation = 0;
				delay = 0;
				level = 0;
				accid = 0;
				startTime = 0;
			}
		};

		//公告
		const BYTE PARA_BROADCAST_GMTOOL = 5;
		struct t_Broadcast_GmTool : t_NullCmd
		{
			GameZone_t zone;
			DWORD country;
			char content[256];
			DWORD time;
			DWORD interval;
			char GM[MAX_NAMESIZE];
			BYTE id;
			DWORD mapID;
			t_Broadcast_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_BROADCAST_GMTOOL)
			{
				bzero(content, sizeof(content));
				time = 0;
				interval = 0;
				country = 0;
				bzero(GM, sizeof(GM));
				id = 0;
				mapID = 0;
			}
		};

		//新GM定单,Scene->Super->Info
		const BYTE PARA_NEW_MSG_GMTOOL = 6;
		struct t_NewMsg_GmTool : t_NullCmd
		{
			GameZone_t zone;//区
			char userName[MAX_NAMESIZE];//举报者
			DWORD accid;//举报者accid
			char userCountry[MAX_NAMESIZE];//举报者国家
			DWORD type;//定单类型
			char content[512];//主要内容
			BYTE contact;//联系方式
			char tele[101];//地址、电话
			char hisName[MAX_NAMESIZE];//被举报者
			char bugCountry[8];//bug发生国家
			char bugMap[16];//bug发生地图
			char bugPos[8];//bug发生位置
			char bugTime[64];//发生时间
			char progName[64];//宜似外挂名称
			BYTE behavior;//不良游戏现象
			DWORD userID;//举报者ID
			t_NewMsg_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_NEW_MSG_GMTOOL)
			{
				bzero(userName, sizeof(userName));
				bzero(userCountry, sizeof(userCountry));
				bzero(content, sizeof(content));
				bzero(tele, sizeof(tele));
				bzero(hisName, sizeof(hisName));
				bzero(bugCountry, sizeof(bugCountry));
				bzero(bugMap, sizeof(bugMap));
				bzero(bugPos, sizeof(bugPos));
				bzero(bugTime, sizeof(bugTime));
				bzero(progName, sizeof(progName));
				behavior = 0;
				userID = 0;
			}
		};

		//日志
		const BYTE PARA_LOG_GMTOOL = 7;
		struct t_Log_GmTool : t_NullCmd
		{
			GameZone_t zone;//区
			DWORD time;//时间
			DWORD charID;
			DWORD accID;
			char GM[MAX_NAMESIZE];//GM名字
			char pos[MAX_NAMESIZE];//地图名
			char cmd[MAX_NAMESIZE];//指令名
			char content[256];//内容
			t_Log_GmTool() : t_NullCmd(CMD_GMTOOL, PARA_LOG_GMTOOL)
			{
				bzero(pos, sizeof(pos));
				bzero(cmd, sizeof(cmd));
				time = 0;
				charID = 0;
				accID = 0;
				bzero(GM, sizeof(GM));
				bzero(content, sizeof(content));
			}
		};
	};
};

#pragma pack()
#endif

/*
# Host: 192.168.2.35
# Database: RecordServer
# Table: 'GMMESSAGE'
# 
CREATE TABLE `GMMESSAGE` (
		`ID` int(10) unsigned NOT NULL auto_increment,
		`ZONE` int(10) unsigned NOT NULL default '0',
		`NAME` varchar(33) NOT NULL default '',
		`TYPE` smallint(5) unsigned NOT NULL default '0',
		`CONTENT` varchar(255) NOT NULL default '',
		`CONTACT` smallint(5) unsigned NOT NULL default '0',
		`TELE` varchar(100) NOT NULL default '',
		`START_TIME` varchar(100) NOT NULL default '',
		`STATE` int(11) NOT NULL default '0',
		`REPLY` varchar(100) NOT NULL default '',
		`ISDEAL` smallint(6) NOT NULL default '0',
		`ACCNAME` varchar(100) NOT NULL default '',
		`CONTRY` varchar(100) NOT NULL default '',
		`GMNAME` varchar(100) NOT NULL default '',
		`SERVER` varchar(100) NOT NULL default '',
		PRIMARY KEY  (`ID`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

# Host: 192.168.2.35
# Database: RecordServer
# Table: 'operator'
# 
CREATE TABLE `operator` (
		`id` int(11) NOT NULL auto_increment,
		`gmname` varchar(100) NOT NULL default '',
		`time` varchar(100) NOT NULL default '',
		`content` varchar(100) NOT NULL default '',
		`recordid` varchar(100) NOT NULL default '',
		PRIMARY KEY  (`id`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

struct stTradeObject
{
	DWORD dwObjectID;
	DWORD dwObjectTempID;
	char strName[MAX_NAMESIZE];
	BYTE upgrade;                         // 等级
	WORD kind;      //物品类型, 0普通, 1蓝色, 2金色, 4神圣, 8套装
};

*/
