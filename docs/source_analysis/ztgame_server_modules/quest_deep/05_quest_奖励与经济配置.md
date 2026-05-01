# 奖励与经济配置
- quests.xml 描述层出现经验、时长、道具展示（说明文本）；实际发奖以 source XML 动作为准。
- 奖励类型关注：经验、银两/绑银、道具、称号、技能（具体字段待在 QuestAction/动作节点中二次确认）。

## 高风险经济奖励点
- 活动批量 on_use（2023xx/2024xx）与高编号任务，易发生倍率误配。
- `on_other/levelup.xml`、`on_kill_by_level/*.xml` 涉及周期性触发，若重复触发会放大经济产出。
- 单点入口 `quests.xml` 改错会连带大批 source 生效范围。

## 审计建议
- 按 source 提取 reward 相关节点并做上限阈值告警。
- 变更前后 diff 重点看 `value/count/rate`。
- 灰度期限定角色白名单验证。

RAG标签：`quest-reward`, `economy-risk`, `activity-on_use`。
