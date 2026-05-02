# ztgame 服务端多进程协同关系导图（Gateway / Session / Scenes / Bill / Record）

> 目的：帮助天枢团队建立 `ztgame` 多进程协同的“系统级认知”。  
> 范围：源码导读，不涉及改造实施。

## 1. 多进程协同总览

```text
客户端请求
  -> GatewayServer（连接接入/消息转发）
    -> SessionServer（会话与账号态）
      -> ScenesServer（玩法与场景核心）
        -> BillServer（计费/点数相关链路）
        -> RecordServer（行为/账变/日志记录）
          -> 数据库
```

补充：`MiniServer / SuperServer / FLServer` 等服务通常承担特定子功能或协调角色，需结合运行配置验证。

---

## 2. 核心服务职责导读

## 2.1 GatewayServer

建议先回答：

1. 哪些协议包在网关层被分流。
2. 网关如何识别连接与会话绑定关系。
3. 网关层是否做限流/黑名单/基础校验。

重点目录：`ztgame/GatewayServer/`

## 2.2 SessionServer

建议先回答：

1. 账号登录态在哪维护。
2. 断线重连与会话恢复逻辑在哪。
3. 与数据库账户信息的交互入口在哪。

重点目录：`ztgame/SessionServer/`

## 2.3 ScenesServer

建议先回答：

1. 场景状态与角色状态主循环在哪里。
2. 玩法事件如何触发任务/奖励脚本。
3. 与 `quest/newquest/script` 的接口边界在哪里。

重点目录：`ztgame/ScenesServer/`、`ztgame/quest/`、`ztgame/newquest/`、`ztgame/script/`

## 2.4 BillServer / RecordServer

建议先回答：

1. 发奖/扣费/点数变化由谁主导。
2. 账变记录与行为日志谁负责落库。
3. 异常重试与幂等控制是否存在。

重点目录：`ztgame/BillServer/`、`ztgame/RecordServer/`

---

## 3. 三条必须走通的跨服务链路

## 链路 A：登录链

```text
Gateway -> Session -> Scenes
```

验收：能解释“连接建立、会话校验、进场景确认”三个交接点。

## 链路 B：任务链

```text
Scenes -> quest/newquest/script -> Scenes状态更新
```

验收：能定位任务接取、状态推进、完成判定的关键文件。

## 链路 C：奖励链

```text
Scenes/任务事件 -> Bill/Record -> 数据库
```

验收：能定位奖励触发点、账变记录点、落库点。

---

## 4. 天枢团队走读任务单（跨服务版）

## Task A：服务边界图

- 输出：5大核心服务的输入/输出/依赖关系图。

## Task B：消息流样本

- 任选一个业务消息，输出从网关到场景（或账务）的转发路径。

## Task C：异常路径样本

- 任选一种异常（断线、数据库失败、重复请求），输出处理分支与恢复策略。

---

## 5. 读码时的高风险误区

1. 只看单进程，不看跨服务消息回路。
2. 只看 C++ 服务，不看 `quest/newquest/script`。
3. 只看成功路径，不看异常与重试分支。
4. 把日志当事实，未核对实际落库。

---

## 6. 建议交付格式（固定）

每次提交都必须附：

1. 文件路径列表
2. 关键函数列表
3. 上下游调用关系
4. 一条复现实验步骤
5. 仍待确认的问题

---

## 7. 与导读文档的组合关系

- 总读法：`13_天枢团队源码深度认知导读.md`
- 服务端业务链：`14_服务端源码调用链导图_登录任务奖励.md`
- 客户端链：`15_ztgame_dat客户端源码调用链导图_UI渲染技能资源.md`
- 登录器链：`16_登录器源码调用链导图_配置更新启动.md`
- 本文定位：服务端“跨进程协同层”专项导图
