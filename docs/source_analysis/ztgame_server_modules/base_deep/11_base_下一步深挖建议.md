# base 下一步深挖建议
1. 函数级继续拆
- `zSocket.cpp`: send/recv 宏路径、加密队列切换分支。
- `zTCPTaskPool.cpp` / `zTCPClientTaskPool.cpp`: 多线程状态机与回收条件。
- `zMysqlDBConnPool.cpp`: SQL 构造函数族与错误码分级。
2. 需补调用链图
- `zSubNetService <-> SuperClient` 启动注册链。
- `zService::main` 到各派生类 `serviceCallback`。
3. 影响其他服务的函数
- `sendCmd/recvToCmd/cmdMsgParse`（Gateway/Session/Scenes 全依赖）。
- `zConfile::parse`（所有服务启动配置依赖）。
4. 历史编译修复需固化
- log4cxx ABI、mysql libs_r、xml2/gd/jpeg 链接参数。
5. 下一目录建议
- 优先 `ztgame/SessionServer/`：它对协议头与网络层耦合最重，最能验证 base 分析结论。
