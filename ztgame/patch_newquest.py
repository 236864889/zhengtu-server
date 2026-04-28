import os
from pathlib import Path

root = Path("/home/zhengtu/ztgame/newquest")

repls = [
    # 英文 charbase -> 扁平
    ("player.charbase.level", "player.level"),
    ("player.charbase.honor", "player.honor"),
    ("player.charbase.name", "player.name"),
    ("player.charbase.face", "player.face"),
    ("player.charbase.country", "player.country"),
    ("player.charbase.mapName", "player.mapName"),
    ("player.charbase.changejob", "player.changejob"),
    ("player.charbase.gold", "player.gold"),
    ("player.charbase.ticket", "player.ticket"),
    ("player.charbase.salary", "player.salary"),
    ("player.charbase.chongzhi", "player.chongzhi"),

    # 英文 quest -> 扁平方法
    ("player.quest:vars(", "player:quest_vars("),
    ("player.quest:add(", "player:quest_add("),
    ("player.quest:refresh(", "player:quest_refresh("),
    ("player.quest_list:vars(", "player:quest_vars("),
    ("player.quest_list:add(", "player:quest_add("),
    ("player.quest_list:refresh(", "player:quest_refresh("),

    # 中文角色信息 -> 扁平
    ("当前角色.角色信息.性别", "当前角色.性别"),
    ("当前角色.角色信息.名称", "当前角色.名称"),
    ("当前角色.角色信息.等级", "当前角色.等级"),
    ("当前角色.角色信息.国家", "当前角色.国家"),
    ("当前角色.角色信息.荣誉", "当前角色.荣誉"),

    # 中文任务 -> 扁平方法
    ("当前角色.任务:vars(", "当前角色:quest_vars("),
    ("当前角色.任务:add(", "当前角色:quest_add("),
    ("当前角色.任务:refresh(", "当前角色:quest_refresh("),

    # 高风险对象链
    (".data.dwObjectID", ".dwObjectID"),
    (".base.level", ".level"),
    (".base.name", ".name"),
]

for p in root.rglob("*.lua"):
    data = p.read_bytes()
    text = None
    enc = None
    for e in ("utf-8", "gbk", "latin1"):
        try:
            text = data.decode(e)
            enc = e
            break
        except:
            pass
    if text is None:
        continue

    old = text
    for a, b in repls:
        text = text.replace(a, b)

    if text != old:
        p.write_text(text, encoding=enc)
        print("patched:", p)