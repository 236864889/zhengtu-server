LUA_PATH = "./newquest/?.lua";
--require "./newquest/test"
require "./newquest/npc/npc_848"

function die()
	--print("die");
	--player = me();
	--sys(player, 1, "死亡测试");		
	return 1;
end

--by=>friday 添加缺失的enter和quit函数，修复掉场景问题
function enter()
	--玩家进入游戏场景时调用
	--print("player enter game");
	return 1;
end

function quit()
	--玩家退出游戏场景时调用  
	--print("player quit game");
	return 1;
end
