#include "SceneUser.h"
#include "SceneNpcManager.h"
#include "NpcTrade.h"
#include "RebuildObject.h"
#include "CharBase.h"
#include "Command.h"
#include "fjconfig.h"
#include <string>

template <class T>
static DWORD getConfigMaxLevel(const std::vector<T> &list)
{
	DWORD maxLevel = 0;
	for (typename std::vector<T>::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		if (it->level > maxLevel)
			maxLevel = it->level;
	}
	return maxLevel;
}

static DWORD getMaxLevelWithFallback(const char *key, DWORD fallback)
{
	DWORD value = atoi(Zebra::global[key].c_str());
	if (value == 0 && fallback > 0)
		return fallback;
	return value;
}
// 附件 Sc附件 附件系统 by:醉梦
bool SceneUser::doFujianCmd(const Cmd::stFujianUserCmd *ptCmd, unsigned int cmdLen)
{
	DWORD MAX_JIANZHONG = getMaxLevelWithFallback("max_jianzhong", getConfigMaxLevel(fjconfig::getInstance().fjlist));
	DWORD MAX_JINGMAI = getMaxLevelWithFallback("max_jingmai", getConfigMaxLevel(fjconfig::getInstance().jingmailist));
	DWORD MAX_SHENGXIAO = getMaxLevelWithFallback("max_shengxiao", getConfigMaxLevel(fjconfig::getInstance().shengxiaolist));
	DWORD MAX_SHENGQI = getMaxLevelWithFallback("max_shengqi", getConfigMaxLevel(fjconfig::getInstance().shengqilist));
	DWORD MAX_ZHENGTUZHUAN = getMaxLevelWithFallback("max_zhengtuzhuan", getConfigMaxLevel(fjconfig::getInstance().ztzlist));
	
	using namespace Cmd;
	switch (ptCmd->byParam) //这里就是判断子命令了byParam
	{
	case FUJIANJIANZHONG_USERCMD:
	{
		// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
		Cmd::stFujianJianzhongReturn rev;
		rev.jian1 = charbase.jian1;
		rev.jian2 = charbase.jian2;
		rev.jian3 = charbase.jian3;
		rev.jian4 = charbase.jian4;
		rev.jian5 = charbase.jian5;
		rev.jian6 = charbase.jian6;
		this->sendCmdToMe(&rev, sizeof(rev));
	}
	break;
	case FUJIANJIANZHONG_LEVELUP:
	{
		stFujianJianzhongLevelup *cmd = (stFujianJianzhongLevelup *)(ptCmd);
		DWORD state = 0;
		Cmd::stFujianJianzhongLevelupReturn rev;
		zObject *itemobj = this->packs.uom.getObjectByThisID(cmd->itemUID);
		if (itemobj)
		{

			switch (cmd->jian)
			{
			case 1:
				if (charbase.jian1+1>MAX_JIANZHONG)
				{
					// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 2:
				if (charbase.jian2+1>MAX_JIANZHONG)
				{
					// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 3:
				if (charbase.jian3+1>MAX_JIANZHONG)
				{
					// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 4:
				if (charbase.jian4+1>MAX_JIANZHONG)
				{
					// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 5:
				if (charbase.jian5+1>MAX_JIANZHONG)
				{
					// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 6:
				if (charbase.jian6+1>MAX_JIANZHONG)
				{
					// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:

				break;
			}
			switch (cmd->jian)
			{
			case 1:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().fjlist[charbase.jian1].costID )
				{
					// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 2:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().fjlist[charbase.jian2].costID )
				{
					// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 3:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().fjlist[charbase.jian3].costID)
				{
					// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 4:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().fjlist[charbase.jian4].costID )
				{
					// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 5:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().fjlist[charbase.jian5].costID)
				{
					// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 6:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().fjlist[charbase.jian6].costID)
				{
					// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:

				break;
			}

			DWORD objnum=1;
			switch (cmd->jian)
			{
			case 1:
				if (charbase.jian1 + 1 <= MAX_JIANZHONG)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().fjlist[charbase.jian1].salary, "剑冢升级"))
					{
						objnum =fjconfig::getInstance().fjlist[charbase.jian1].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.jian1++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 2:
				if (charbase.jian2 + 1 <= MAX_JIANZHONG)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().fjlist[charbase.jian2].salary, "剑冢升级"))
					{
						objnum =fjconfig::getInstance().fjlist[charbase.jian2].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.jian2++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 3:
				if (charbase.jian3 + 1 <= MAX_JIANZHONG)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().fjlist[charbase.jian3].salary, "剑冢升级"))
					{
						objnum =fjconfig::getInstance().fjlist[charbase.jian3].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.jian3++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 4:
				if (charbase.jian4 + 1 <= MAX_JIANZHONG)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().fjlist[charbase.jian4].salary, "剑冢升级"))
					{
						objnum =fjconfig::getInstance().fjlist[charbase.jian4].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.jian4++;
						state = 1;	
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				break;
			case 5:
				if (charbase.jian5 + 1 <= MAX_JIANZHONG)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().fjlist[charbase.jian5].salary, "剑冢升级"))
					{
						objnum =fjconfig::getInstance().fjlist[charbase.jian5].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.jian5++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 6:
				if (charbase.jian6 + 1 <= MAX_JIANZHONG)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().fjlist[charbase.jian6].salary, "剑冢升级"))
					{
						objnum =fjconfig::getInstance().fjlist[charbase.jian6].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.jian6++;
						state = 1;
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:

				break;
			}

			// 刷新用户数据
			this->setupCharBase();
			Cmd::stMainUserDataUserCmd userinfo;
			this->full_t_MainUserData(userinfo.data);
			sendCmdToMe(&userinfo, sizeof(userinfo));
			if (itemobj->data.dwNum > objnum) // soke 大于9朵直接-9
			{
				itemobj->data.dwNum-=objnum;
				Cmd::stRefCountObjectPropertyUserCmd send;
				send.qwThisID = itemobj->data.qwThisID;
				send.dwNum = itemobj->data.dwNum;
				sendCmdToMe(&send, sizeof(send));
			}
			else
			{
				this->packs.removeObject(itemobj);
			}
		}

		// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
		rev.state = state;
		sendCmdToMe(&rev, sizeof(rev));
	}
	break;
	case FUJIANZTZ_USERCMD: //刚刚客户端 加载征途传的时候发过来的是这个 对吧
		{
			//然后 我们这里写代码  服务端收到了客户端发来的请求玩家征途传信息的指令

			//那我们不需要多做处理 直接把玩家征途传等级直接给返回去
			//我们创建一个指令 刚刚定义的第二个
			Cmd::stFujianZtzReturn rev;
			//给里面的参数赋值
			rev.zhuan1 = charbase.zhuan1;
			rev.zhuan2 = charbase.zhuan2;
			rev.zhuan3 = charbase.zhuan3;
			rev.zhuan4 = charbase.zhuan4;
			rev.zhuan5 = charbase.zhuan5;
			rev.zhuan6 = charbase.zhuan6;
			rev.zhuan7 = charbase.zhuan7;
			rev.zhuan8 = charbase.zhuan8;
			rev.zhuan9 = charbase.zhuan9;
			rev.zhuan10 = charbase.zhuan10;

			//这里明白吗  charbase就是玩家的信息 刚刚加数据库字段那
			//把玩家的十个信息 赋值给 我们定义好的指令 携带的参数里面 然后

			this->sendCmdToMe(&rev, sizeof(rev));
			//固定用法  直接返回客户端

			//我们编译先
		}
		break;
	case FUJIANZTZ_LEVELUP: //这一段哪里看不懂 你看看这一段你 哪里 看不懂
	{
		stFujianZtzLevelup *cmd = (stFujianZtzLevelup *)(ptCmd);
		DWORD state = 0;
		Cmd::stFujianZtzLevelupReturn rev;
		zObject *itemobj = this->packs.uom.getObjectByThisID(cmd->itemUID); //获取道具 //这里先取一下客户端的道具 客户端放的什么东西 itemobj 就是什么东西
		if (itemobj) //如果道具存在
		{

			switch (cmd->zhuan) //判断征途传
			{
			case 1:
				if (charbase.zhuan1+1>MAX_ZHENGTUZHUAN ) 
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 2: //同样以此类推 如果选择的是征途传 2 那就去 征途传2 等级 的那条xml配置的 costID  和 道具id进行比对
				if (charbase.zhuan2+1>MAX_ZHENGTUZHUAN ) 
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 3:
				if (charbase.zhuan3+1>MAX_ZHENGTUZHUAN ) 
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 4:
				if (charbase.zhuan4+1>MAX_ZHENGTUZHUAN ) 
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 5:
				if (charbase.zhuan5+1>MAX_ZHENGTUZHUAN ) 
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 6:
				if (charbase.zhuan6+1>MAX_ZHENGTUZHUAN ) 
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 7:
				if (charbase.zhuan7+1>MAX_ZHENGTUZHUAN ) 
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 8:
				if (charbase.zhuan8+1>MAX_ZHENGTUZHUAN ) 
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 9:
				if (charbase.zhuan9+1>MAX_ZHENGTUZHUAN ) 
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 10:
				if (charbase.zhuan10+1>MAX_ZHENGTUZHUAN ) 
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:
				
				break;
			}
			switch (cmd->zhuan) //判断征途传
			{
			case 1:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().ztzlist[charbase.zhuan1].costID ) //这里 判断一下 fjconfig::getInstance().ztzlist[charbase.zhuan1].costID 取得是charbase.zhuan1 级别 的那条xml  需要的道具id是多少 然后进行判断 
				{
					//如果对不上 返回结束了
					//如果对不上 也就是说玩家的东西放的不对 返回3 看懂了吗
					// 返回玩家征途传升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 2: //同样以此类推 如果选择的是征途传 2 那就去 征途传2 等级 的那条xml配置的 costID  和 道具id进行比对
				if (itemobj->data.dwObjectID != fjconfig::getInstance().ztzlist[charbase.zhuan2].costID )
				{
					// 返回玩家征途传升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 3:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().ztzlist[charbase.zhuan3].costID)
				{
					// 返回玩家征途传升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 4:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().ztzlist[charbase.zhuan4].costID )
				{
					// 返回玩家征途传升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 5:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().ztzlist[charbase.zhuan5].costID)
				{
					// 返回玩家征途传升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 6:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().ztzlist[charbase.zhuan6].costID)
				{
					// 返回玩家征途传升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 7:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().ztzlist[charbase.zhuan7].costID)
				{
					// 返回玩家征途传升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 8:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().ztzlist[charbase.zhuan8].costID)
				{
					// 返回玩家征途传升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 9:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().ztzlist[charbase.zhuan9].costID)
				{
					// 返回玩家征途传升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 10:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().ztzlist[charbase.zhuan10].costID)
				{
					// 返回玩家征途传升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:
				
				break;
			}
			//如果走到了这里 说明什么 说明 上面的 if判断 全部不成立 的对不对  也就是说明 道具对上了


			DWORD objnum=1;
			//然后 继续判断 选择的 是哪一个 征途传
			switch (cmd->zhuan)
			{
			case 1:
				//这里 zhuan1 是 目前的等级  比如目前是 100级 那么 +1 就是我们即将要做的事情 对吧  如果当前等级+1 <=100 才会进行升级操作
				if (charbase.zhuan1 + 1 <= MAX_ZHENGTUZHUAN)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().ztzlist[charbase.zhuan1].salary, "征途传升级")) //这里removeMoney 执行了扣金子操作 fjconfig::getInstance().ztzlist[charbase.zhuan1].salary 是金额
					{
						//如果成功了 则 征途传1等级自增 并且 state为1
						objnum =fjconfig::getInstance().ztzlist[charbase.zhuan1].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.zhuan1++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
						//2 就是没钱
					}
				}
				else{ //否则 如果+1之后大于100了 就返回0 失败   懂了吗
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
				//下面都是一样的
			case 2:
				if (charbase.zhuan2 + 1 <= MAX_ZHENGTUZHUAN)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().ztzlist[charbase.zhuan2].salary, "征途传升级"))
					{
						objnum =fjconfig::getInstance().ztzlist[charbase.zhuan2].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.zhuan2++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 3:
				if (charbase.zhuan3 + 1 <= MAX_ZHENGTUZHUAN)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().ztzlist[charbase.zhuan3].salary, "征途传升级"))
					{
						objnum =fjconfig::getInstance().ztzlist[charbase.zhuan3].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.zhuan3++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 4:
				if (charbase.zhuan4 + 1 <= MAX_ZHENGTUZHUAN)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().ztzlist[charbase.zhuan4].salary, "征途传升级"))
					{
						objnum =fjconfig::getInstance().ztzlist[charbase.zhuan4].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.zhuan4++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				break;
			case 5:
				if (charbase.zhuan5 + 1 <= MAX_ZHENGTUZHUAN)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().ztzlist[charbase.zhuan5].salary, "征途传升级"))
					{
						objnum =fjconfig::getInstance().ztzlist[charbase.zhuan5].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.zhuan5++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 6:
				if (charbase.zhuan6 + 1 <= MAX_ZHENGTUZHUAN)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().ztzlist[charbase.zhuan6].salary, "征途传升级"))
					{
						objnum =fjconfig::getInstance().ztzlist[charbase.zhuan6].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.zhuan6++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 7:
				if (charbase.zhuan7 + 1 <= MAX_ZHENGTUZHUAN)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().ztzlist[charbase.zhuan7].salary, "征途传升级"))
					{
						objnum =fjconfig::getInstance().ztzlist[charbase.zhuan7].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.zhuan7++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 8:
				if (charbase.zhuan8 + 1 <= MAX_ZHENGTUZHUAN)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().ztzlist[charbase.zhuan8].salary, "征途传升级"))
					{
						objnum =fjconfig::getInstance().ztzlist[charbase.zhuan8].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.zhuan8++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 9:
				if (charbase.zhuan9 + 1 <= MAX_ZHENGTUZHUAN)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().ztzlist[charbase.zhuan9].salary, "征途传升级"))
					{
						objnum =fjconfig::getInstance().ztzlist[charbase.zhuan9].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.zhuan9++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 10:
				if (charbase.zhuan10 + 1 <= MAX_ZHENGTUZHUAN)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().ztzlist[charbase.zhuan10].salary, "征途传升级"))
					{
						objnum =fjconfig::getInstance().ztzlist[charbase.zhuan10].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.zhuan10++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:

				break;
			}


			//走到这 

			// 刷新用户数据
			this->setupCharBase();
			Cmd::stMainUserDataUserCmd userinfo;
			this->full_t_MainUserData(userinfo.data);
			sendCmdToMe(&userinfo, sizeof(userinfo));
			//上面四行都是刷新用户的属性 固定用法 需要的时候直接粘贴即可

			//这里 是 扣除道具
			//走到了这里 就意味着 itemobj 就是配置文件里面的对应的道具 所以直接执行扣除
			//itemobj->data.dwNum 的意思是 客户端的框里 放的道具目前的数量 如果大于一 则减1 如果=1 则直接移除道具
			if (itemobj->data.dwNum > objnum) // soke 大于9朵直接-9
			{
				itemobj->data.dwNum-=objnum; //这里 -- 表示自减 比如要扣五个的话就是这样 刚刚那样就是扣五个 itemobj 现在就是799 因为在上面取得道具 明白了吗
				Cmd::stRefCountObjectPropertyUserCmd send;
				send.qwThisID = itemobj->data.qwThisID;
				send.dwNum = itemobj->data.dwNum;
				sendCmdToMe(&send, sizeof(send));
			}
			else
			{
				this->packs.removeObject(itemobj);
			}
			//也是固定用法 这一段直接粘即可 这是我从鲜花扣除的地方直接站过来的
		}

		//到这我们服务端所有逻辑就全部写完了 我们只需要去客户端 接收一下返回的state 状态进行操作即可1

		// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
		rev.state = state;
		sendCmdToMe(&rev, sizeof(rev));
	}
	break;
	case FUJIANSHENGXIAO_USERCMD: //十二生肖
		{
			//然后 我们这里写代码  服务端收到了客户端发来的请求玩家征途传信息的指令

			//那我们不需要多做处理 直接把玩家征途传等级直接给返回去
			//我们创建一个指令 刚刚定义的第二个
			Cmd::stFujianShengxiaoReturn rev;
			//给里面的参数赋值
			rev.shengx1 = charbase.shengx1;
			rev.shengx2 = charbase.shengx2;
			rev.shengx3 = charbase.shengx3;
			rev.shengx4 = charbase.shengx4;
			rev.shengx5 = charbase.shengx5;
			rev.shengx6 = charbase.shengx6;
			rev.shengx7 = charbase.shengx7;
			rev.shengx8 = charbase.shengx8;
			rev.shengx9 = charbase.shengx9;
			rev.shengx10 = charbase.shengx10;
			rev.shengx11 = charbase.shengx11;
			rev.shengx12 = charbase.shengx12;

			//这里明白吗  charbase就是玩家的信息 刚刚加数据库字段那
			//把玩家的十二个信息 赋值给 我们定义好的指令 携带的参数里面 然后

			this->sendCmdToMe(&rev, sizeof(rev));
			//固定用法  直接返回客户端

			//我们编译先
		}
		break;
	case FUJIANSHENGXIAO_LEVELUP: //这一段哪里看不懂 你看看这一段你 哪里 看不懂
	{
		stFujianShengxiaoLevelup *cmd = (stFujianShengxiaoLevelup *)(ptCmd);
		DWORD state = 0;
		Cmd::stFujianShengxiaoLevelupReturn rev;
		zObject *itemobj = this->packs.uom.getObjectByThisID(cmd->itemUID); //获取道具 //这里先取一下客户端的道具 客户端放的什么东西 itemobj 就是什么东西
		if (itemobj) //如果道具存在
		{
			switch (cmd->shengx) //判断征途传
			{
			case 1:
				if (charbase.shengx1+1>MAX_SHENGXIAO )
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 2: //同样以此类推 如果选择的是征途传 2 那就去 征途传2 等级 的那条xml配置的 costID  和 道具id进行比对
				if (charbase.shengx2+1>MAX_SHENGXIAO )
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 3:
				if (charbase.shengx3+1>MAX_SHENGXIAO )
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 4:
				if (charbase.shengx4+1>MAX_SHENGXIAO )
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 5:
				if (charbase.shengx5+1>MAX_SHENGXIAO )
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 6:
				if (charbase.shengx6+1>MAX_SHENGXIAO )
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 7:
				if (charbase.shengx7+1>MAX_SHENGXIAO )
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 8:
				if (charbase.shengx8+1>MAX_SHENGXIAO )
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 9:
				if (charbase.shengx9+1>MAX_SHENGXIAO )
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 10:
				if (charbase.shengx10+1>MAX_SHENGXIAO )
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 11:
				if (charbase.shengx11+1>MAX_SHENGXIAO )
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 12:
				if (charbase.shengx12+1>MAX_SHENGXIAO )
				{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:
				
				break;
			}
			switch (cmd->shengx) //判断征途传
			{
			case 1:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengxiaolist[charbase.shengx1].costID ) //这里 判断一下 fjconfig::getInstance().ztzlist[charbase.zhuan1].costID 取得是charbase.zhuan1 级别 的那条xml  需要的道具id是多少 然后进行判断 
				{
					//如果对不上 返回结束了
					//如果对不上 也就是说玩家的东西放的不对 返回3 看懂了吗
					// 返回玩家十二生肖升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 2: //同样以此类推 如果选择的是征途传 2 那就去 征途传2 等级 的那条xml配置的 costID  和 道具id进行比对
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengxiaolist[charbase.shengx2].costID )
				{
					// 返回玩家十二生肖升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 3:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengxiaolist[charbase.shengx3].costID)
				{
					// 返回玩家十二生肖升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 4:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengxiaolist[charbase.shengx4].costID )
				{
					// 返回玩家十二生肖升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 5:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengxiaolist[charbase.shengx5].costID)
				{
					// 返回玩家十二生肖升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 6:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengxiaolist[charbase.shengx6].costID)
				{
					// 返回玩家十二生肖升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 7:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengxiaolist[charbase.shengx7].costID)
				{
					// 返回玩家十二生肖升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 8:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengxiaolist[charbase.shengx8].costID)
				{
					// 返回玩家十二生肖升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 9:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengxiaolist[charbase.shengx9].costID)
				{
					// 返回玩家十二生肖升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 10:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengxiaolist[charbase.shengx10].costID)
				{
					// 返回玩家十二生肖升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 11:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengxiaolist[charbase.shengx11].costID)
				{
					// 返回玩家十二生肖升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 12:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengxiaolist[charbase.shengx12].costID)
				{
					// 返回玩家十二生肖升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:
				
				break;
			}
			//如果走到了这里 说明什么 说明 上面的 if判断 全部不成立 的对不对  也就是说明 道具对上了



			//然后 继续判断 选择的 是哪一个 十二生肖
			DWORD objnum=1;
			switch (cmd->shengx)
			{
			case 1:
				//这里 shengx1 是 目前的等级  比如目前是 100级 那么 +1 就是我们即将要做的事情 对吧  如果当前等级+1 <=100 才会进行升级操作
				if (charbase.shengx1 + 1 <= MAX_SHENGXIAO)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengxiaolist[charbase.shengx1].salary, "十二生肖升级")) //这里removeMoney 执行了扣金子操作 fjconfig::getInstance().ztzlist[charbase.shengx1].salary 是金额
					{
						//如果成功了 则 征途传1等级自增 并且 state为1
						objnum =fjconfig::getInstance().shengxiaolist[charbase.shengx1].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengx1++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
						//2 就是没钱
					}
				}
				else{ //否则 如果+1之后大于100了 就返回0 失败   懂了吗
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
				//下面都是一样的
			case 2:
				if (charbase.shengx2 + 1 <= MAX_SHENGXIAO)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengxiaolist[charbase.shengx2].salary, "十二生肖升级"))
					{
						objnum =fjconfig::getInstance().shengxiaolist[charbase.shengx2].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengx2++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 3:
				if (charbase.shengx3 + 1 <= MAX_SHENGXIAO)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengxiaolist[charbase.shengx3].salary, "十二生肖升级"))
					{
						objnum =fjconfig::getInstance().shengxiaolist[charbase.shengx3].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengx3++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 4:
				if (charbase.shengx4 + 1 <= MAX_SHENGXIAO)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengxiaolist[charbase.shengx4].salary, "十二生肖升级"))
					{
						objnum =fjconfig::getInstance().shengxiaolist[charbase.shengx4].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengx4++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				break;
			case 5:
				if (charbase.shengx5 + 1 <= MAX_SHENGXIAO)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengxiaolist[charbase.shengx5].salary, "十二生肖升级"))
					{
						objnum =fjconfig::getInstance().shengxiaolist[charbase.shengx5].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengx5++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 6:
				if (charbase.shengx6 + 1 <= MAX_SHENGXIAO)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengxiaolist[charbase.shengx6].salary, "十二生肖升级"))
					{
						objnum =fjconfig::getInstance().shengxiaolist[charbase.shengx6].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengx6++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 7:
				if (charbase.shengx7 + 1 <= MAX_SHENGXIAO)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengxiaolist[charbase.shengx7].salary, "十二生肖升级"))
					{
						objnum =fjconfig::getInstance().shengxiaolist[charbase.shengx7].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengx7++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 8:
				if (charbase.shengx8 + 1 <= MAX_SHENGXIAO)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengxiaolist[charbase.shengx8].salary, "十二生肖升级"))
					{
						objnum =fjconfig::getInstance().shengxiaolist[charbase.shengx8].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengx8++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 9:
				if (charbase.shengx9 + 1 <= MAX_SHENGXIAO)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengxiaolist[charbase.shengx9].salary, "十二生肖升级"))
					{
						objnum =fjconfig::getInstance().shengxiaolist[charbase.shengx9].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengx9++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 10:
				if (charbase.shengx10 + 1 <= MAX_SHENGXIAO)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengxiaolist[charbase.shengx10].salary, "十二生肖升级"))
					{
						objnum =fjconfig::getInstance().shengxiaolist[charbase.shengx10].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengx10++;
						state = 1;	
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 11:
				if (charbase.shengx11 + 1 <= MAX_SHENGXIAO)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengxiaolist[charbase.shengx11].salary, "十二生肖升级"))
					{
						objnum =fjconfig::getInstance().shengxiaolist[charbase.shengx11].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengx11++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 12:
				if (charbase.shengx12 + 1 <= MAX_SHENGXIAO)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengxiaolist[charbase.shengx12].salary, "十二生肖升级"))
					{
						objnum =fjconfig::getInstance().shengxiaolist[charbase.shengx12].costNum;
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengx12++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:

				break;
			}


			//走到这 

			// 刷新用户数据
			this->setupCharBase();
			Cmd::stMainUserDataUserCmd userinfo;
			this->full_t_MainUserData(userinfo.data);
			sendCmdToMe(&userinfo, sizeof(userinfo));
			//上面四行都是刷新用户的属性 固定用法 需要的时候直接粘贴即可

			//这里 是 扣除道具
			//走到了这里 就意味着 itemobj 就是配置文件里面的对应的道具 所以直接执行扣除
			//itemobj->data.dwNum 的意思是 客户端的框里 放的道具目前的数量 如果大于一 则减1 如果=1 则直接移除道具
			if (itemobj->data.dwNum > objnum) // soke 大于9朵直接-9
			{
				itemobj->data.dwNum-=objnum; //这里 -- 表示自减 比如要扣五个的话就是这样 刚刚那样就是扣五个 itemobj 现在就是799 因为在上面取得道具 明白了吗
				Cmd::stRefCountObjectPropertyUserCmd send;
				send.qwThisID = itemobj->data.qwThisID;
				send.dwNum = itemobj->data.dwNum;
				sendCmdToMe(&send, sizeof(send));
			}
			else
			{
				this->packs.removeObject(itemobj);
			}
			//也是固定用法 这一段直接粘即可 这是我从鲜花扣除的地方直接站过来的
		}

		//到这我们服务端所有逻辑就全部写完了 我们只需要去客户端 接收一下返回的state 状态进行操作即可1

		// 返回玩家剑冢升级状态 0满级 1成功 2没钱 3物品不对
		rev.state = state;
		sendCmdToMe(&rev, sizeof(rev));
	}
	    break;

	case FUJIANSHENGQI_USERCMD:
	    {
			Cmd::stFujianShengqiReturn rev;
			//给里面的参数赋值
			rev.shengq1 = charbase.shengq1;
			this->sendCmdToMe(&rev, sizeof(rev));
			//固定用法  直接返回客户端

			//我们编译先
		}
		break;
	case FUJIANSHENGQI_LEVELUP: 
	{
		stFujianShengqiLevelup *cmd = (stFujianShengqiLevelup *)(ptCmd);
		DWORD state = 0;
		Cmd::stFujianShengqiLevelupReturn rev;
		zObject *itemobj = this->packs.uom.getObjectByThisID(cmd->itemUID); //获取道具 //这里先取一下客户端的道具 客户端放的什么东西 itemobj 就是什么东西
		if (itemobj) //如果道具存在
		{
			switch (cmd->shengq) 
			{
			case 1:
				if (charbase.shengq1+1>MAX_SHENGQI )
				{
					// 返回玩家圣器升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:				
				break;
			}
			switch (cmd->shengq) 
			{
			case 1:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().shengqilist[charbase.shengq1].costID )
				{
					// 返回玩家圣器升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:				
				break;
			}
			DWORD objnum=1;
			switch (cmd->shengq)
			{
			case 1:
				if (charbase.shengq1 + 1 <= MAX_SHENGQI)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().shengqilist[charbase.shengq1].salary, "圣器升级"))
					{						
						objnum =fjconfig::getInstance().shengqilist[charbase.shengq1].costNum;  //取配置文件 需求道具数量
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.shengq1++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{ //否则 如果+1之后大于100了 就返回0 失败   懂了吗
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:
				break;
			}
			// 刷新用户数据
			this->setupCharBase();
			Cmd::stMainUserDataUserCmd userinfo;
			this->full_t_MainUserData(userinfo.data);
			sendCmdToMe(&userinfo, sizeof(userinfo));
			//上面四行都是刷新用户的属性 固定用法 需要的时候直接粘贴即可

			//这里 是 扣除道具
			//走到了这里 就意味着 itemobj 就是配置文件里面的对应的道具 所以直接执行扣除
			//itemobj->data.dwNum 的意思是 客户端的框里 放的道具目前的数量 如果大于一 则减1 如果=1 则直接移除道具
			if (itemobj->data.dwNum > objnum) // soke 大于9朵直接-9 
			{
				//现在 附件升级需要的道具数量就是根据 xml获取得了
				itemobj->data.dwNum-=objnum; //这里 -- 表示自减 比如要扣五个的话就是这样 刚刚那样就是扣五个 itemobj 现在就是799 因为在上面取得道具 明白了吗
				Cmd::stRefCountObjectPropertyUserCmd send;
				send.qwThisID = itemobj->data.qwThisID;
				send.dwNum = itemobj->data.dwNum;
				sendCmdToMe(&send, sizeof(send));
			}
			else
			{
				this->packs.removeObject(itemobj);
			}
			//也是固定用法 这一段直接粘即可 这是我从鲜花扣除的地方直接站过来的
		}

		//到这我们服务端所有逻辑就全部写完了 我们只需要去客户端 接收一下返回的state 状态进行操作即可1

		// 返回玩家圣器升级状态 0满级 1成功 2没钱 3物品不对
		rev.state = state;
		sendCmdToMe(&rev, sizeof(rev));
	}
	    break;

		case FUJIANJINGMAI_USERCMD://经脉
	    {
			Cmd::stFujianJingmaiReturn rev;
			//给里面的参数赋值
			rev.jingm1 = charbase.jingm1;
			rev.jingm2 = charbase.jingm2;
			rev.jingm3 = charbase.jingm3;
			this->sendCmdToMe(&rev, sizeof(rev));
			//固定用法  直接返回客户端

			//我们编译先
		}
		break;
	case FUJIANJINGMAI_LEVELUP: 
	{
		stFujianJingmaiLevelup *cmd = (stFujianJingmaiLevelup *)(ptCmd);
		DWORD state = 0;
		Cmd::stFujianJingmaiLevelupReturn rev;
		zObject *itemobj = this->packs.uom.getObjectByThisID(cmd->itemUID); //获取道具 //这里先取一下客户端的道具 客户端放的什么东西 itemobj 就是什么东西
		if (itemobj) //如果道具存在
		{
			switch (cmd->jingm) 
			{
			case 1:
				if (charbase.jingm1+1>MAX_JINGMAI )
				{
					// 返回玩家经脉升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 2:
				if (charbase.jingm2+1>MAX_JINGMAI )
				{
					// 返回玩家经脉升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 3:
				if (charbase.jingm3+1>MAX_JINGMAI )
				{
					// 返回玩家经脉升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:				
				break;
			}
			switch (cmd->jingm) 
			{
			case 1:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().jingmailist[charbase.jingm1].costID )
				{
					// 返回玩家经脉升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 2:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().jingmailist[charbase.jingm2].costID )
				{
					// 返回玩家经脉升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			case 3:
				if (itemobj->data.dwObjectID != fjconfig::getInstance().jingmailist[charbase.jingm3].costID )
				{
					// 返回玩家经脉升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:				
				break;
			}
			DWORD objnum=1;
			switch (cmd->jingm)
			{
			case 1:
				if (charbase.jingm1 + 1 <= MAX_JINGMAI)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().jingmailist[charbase.jingm1].salary, "经脉升级"))
					{						
						objnum =fjconfig::getInstance().jingmailist[charbase.jingm1].costNum;  //取配置文件 需求道具数量
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.jingm1++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{ //否则 如果+1之后大于100了 就返回0 失败   懂了吗
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
				case 2:
				if (charbase.jingm2 + 1 <= MAX_JINGMAI)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().jingmailist[charbase.jingm2].salary, "经脉升级"))
					{						
						objnum =fjconfig::getInstance().jingmailist[charbase.jingm2].costNum;  //取配置文件 需求道具数量
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.jingm2++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{ //否则 如果+1之后大于100了 就返回0 失败   懂了吗
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
				case 3:
				if (charbase.jingm3 + 1 <= MAX_JINGMAI)
				{
					if (this->packs.removeSalary(fjconfig::getInstance().jingmailist[charbase.jingm3].salary, "经脉升级"))
					{						
						objnum =fjconfig::getInstance().jingmailist[charbase.jingm3].costNum;  //取配置文件 需求道具数量
						if(itemobj->data.dwNum < objnum)
						{
							rev.state = 4;
							sendCmdToMe(&rev, sizeof(rev));
							return true;
						}
						charbase.jingm3++;
						state = 1;
						
					}
					else
					{
						rev.state = 2;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}
				}
				else{ //否则 如果+1之后大于100了 就返回0 失败   懂了吗
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				break;
			default:
				break;
			}
			
			// 刷新用户数据
			this->setupCharBase();
			Cmd::stMainUserDataUserCmd userinfo;
			this->full_t_MainUserData(userinfo.data);
			sendCmdToMe(&userinfo, sizeof(userinfo));
			if (itemobj->data.dwNum > objnum) 
			{
				itemobj->data.dwNum-=objnum; 
				Cmd::stRefCountObjectPropertyUserCmd send;
				send.qwThisID = itemobj->data.qwThisID;
				send.dwNum = itemobj->data.dwNum;
				sendCmdToMe(&send, sizeof(send));
			}
			else
			{
				this->packs.removeObject(itemobj);
			}
		}
		
		rev.state = state;
		sendCmdToMe(&rev, sizeof(rev));
	}
	    break;
	case FUJIANJTOUXIAN_USERCMD://头衔系统
		{
			Cmd::stFujianTouxianReturn rev;
			rev.level = charbase.touxianlevel+1;
			rev.exp = charbase.touxianexp;
			if(rev.level<16)
			{
				for(int i=0;i<rev.level;i++)
				{
					rev.num1 = fjconfig::getInstance().touxianlist[i].pDam;
					rev.num2 = fjconfig::getInstance().touxianlist[i].pDef;
					rev.num3 = fjconfig::getInstance().touxianlist[i].hp;

					rev.num4 = fjconfig::getInstance().touxianlist[charbase.touxianlevel+1].pDam;
					rev.num5 = fjconfig::getInstance().touxianlist[charbase.touxianlevel+1].pDef;
					rev.num6 = fjconfig::getInstance().touxianlist[charbase.touxianlevel+1].hp;
				}
				
			
			}
			else{
				for(int i=0;i<rev.level;i++)
				{
					rev.num1 = fjconfig::getInstance().touxianlist[i].pDam;
					rev.num2 = fjconfig::getInstance().touxianlist[i].pDef;
					rev.num3 = fjconfig::getInstance().touxianlist[i].hp;
				}

				rev.num4 = 0;
				rev.num5 = 0;
				rev.num6 = 0;
			}
			
			this->sendCmdToMe(&rev, sizeof(rev));
		}
		break;
	case FUJIANTOUXIAN_LEVELUP://头衔系统
		{
			stFujianTouxianLevelup *cmd = (stFujianTouxianLevelup *)(ptCmd);
			Cmd::stFujianTouxianLevelupReturn rev;
			zObject *itemobj = this->packs.uom.getObjectByThisID(cmd->itemUID); //获取道具 //这里先取一下客户端的道具 客户端放的什么东西 itemobj 就是什么东西
			if (itemobj) //如果道具存在
			{
			
				if (charbase.touxianlevel+1>15 )
				{
					// 返回玩家头衔升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 0;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				
				
				if (itemobj->data.dwObjectID != fjconfig::getInstance().touxianlist[charbase.touxianlevel].costID )
				{
					// 返回玩家头衔升级状态 0满级 1成功 2没钱 3物品不对
					rev.state = 3;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				DWORD objnum=1;
				if (this->packs.removeMoney(fjconfig::getInstance().touxianlist[charbase.touxianlevel].salary, "头衔升级"))
				{						
					if(itemobj->data.dwNum < objnum)
					{
						rev.state = 4;
						sendCmdToMe(&rev, sizeof(rev));
						return true;
					}

					charbase.touxianexp+=(fjconfig::getInstance().touxianlist[charbase.touxianlevel].exp);
					if(charbase.touxianexp>=10000)
					{
						charbase.touxianexp-=10000;
						charbase.touxianlevel++;
					}

					rev.state = 1;
					sendCmdToMe(&rev, sizeof(rev));
				}
				else
				{
					rev.state = 2;
					sendCmdToMe(&rev, sizeof(rev));
					return true;
				}
				// 刷新用户数据
				this->setupCharBase();
				Cmd::stMainUserDataUserCmd userinfo;
				this->full_t_MainUserData(userinfo.data);
				sendCmdToMe(&userinfo, sizeof(userinfo));
				if (itemobj->data.dwNum > objnum) 
				{
					itemobj->data.dwNum-=objnum; 
					Cmd::stRefCountObjectPropertyUserCmd send;
					send.qwThisID = itemobj->data.qwThisID;
					send.dwNum = itemobj->data.dwNum;
					sendCmdToMe(&send, sizeof(send));
				}
				else
				{
					this->packs.removeObject(itemobj);
				}
			}			
		}
		break;
	}
	return true;
}
