# 技能、战斗与特效同步链路
- 入口：`SceneTask` 接收攻击/施法命令；`ScenePk` 负责 U2U/U2N/N2U/N2N 伤害公式与保护规则。
- 技能对象：`zSkill/SkillManager/SceneSkill`；状态效果由 `SkillStatusManager.cpp` 超大函数族执行。
- 目标选择：Scene内按 tempid/可见范围/阵营校验。
- 广播：`stAttackMagicUserCmd` 等通过 `sendCmdToNine` 同步表现。
- 风险：
  1) `SkillStatusManager` 巨型 switch/函数表难回归；
  2) 攻防计算分散且存在条件分支叠加；
  3) 脚本与C++混合触发导致重入风险。

适合问题：伤害异常、技能失效、群战性能问题。
