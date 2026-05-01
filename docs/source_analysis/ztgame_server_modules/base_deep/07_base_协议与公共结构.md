# base 协议与公共结构
## 1. 范围
`Command.h BillCommand.h DBAccessCommand.h CharBase.h`
## 2. 核心结论
- `Command.h` 为全局协议中心，定义 `Cmd::t_NullCmd` 衍生命令与枚举常量。
- 其他命令头按业务域拆分，但全依赖相同包头布局与命名空间。
## 3. 联动风险
- 修改公共 struct 字段顺序/长度会导致跨服务解包错位。
- 新增 para/cmd 编号若冲突，会在 msgParse 分发层产生不可预测路由。
## 4. 建议
- 仅追加、避免重排；保留版本号/兼容分支；同步更新所有服务解析函数。
## 5. RAG
- level: S
- keywords: Cmd,t_NullCmd,Command.h,DBAccessCommand,BillCommand,CharBase
