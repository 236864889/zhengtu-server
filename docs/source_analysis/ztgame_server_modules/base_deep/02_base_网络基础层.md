# base 网络基础层
## 1. 范围
`zSocket* zTCPServer* zTCPClient* zTCPTask* zTCPTaskPool* zTCPClientTask* zTCPClientTaskPool* zHttpTask*`
## 2. 核心链路
- 入站：`zNetService::serviceCallback -> zTCPServer::accept -> newTCPTask -> zTCPTaskPool`。
- 读包：`zTCPTask::listensocket -> zSocket::recvToBuf_NoPoll -> recvToCmd_NoPoll -> cmdMsgParse`。
- 发包：`业务 sendCmd -> zTCPTask::sendCmd -> zSocket::sendCmd/sendCmdNoPack -> sendRawData(IM/NoPoll)`。
- 回收：`zTCPTaskPool` 内 `zRecycleThread` 回收断连/异常 task；client 侧 `zCheckconnectThread/zCheckwaitThread` 维护状态。
## 3. 职责边界
- `zSocket`: 纯 socket + 缓冲/封包/解包/加解密队列。
- `zTCPServer`: 监听与 accept。
- `zTCPClient`: 主动 connect + run 循环。
- `Task/Pool`: 生命周期、并发、回收、心跳/验证线程。
## 4. 高风险点
- `send/recv` EAGAIN 与 busy loop 风险。
- pack/unpack 长度处理和 `MAX_DATASIZE` 边界。
- 多线程队列锁粒度导致延迟尖刺。
## 5. RAG
- level: S
- keywords: zSocket,recvToCmd,sendCmd,zTCPTaskPool,zRecycleThread,zTCPClientTaskPool
## 6. 适合问题
- 网络断连、粘包、CPU 飙高、连接泄漏定位
