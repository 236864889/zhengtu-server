# RecordServer 启动与生命周期
- `main`：初始化默认配置 -> 解析 XML + CLI -> 设置日志 -> 可 daemon -> `RecordService::main()`。  
- 继承关系：`RecordService:zSubNetService`，生命周期继承 `zService` 骨架。  
- `init`：初始化 DB 连接池、线程池、网卡IP、父类 init。  
- `validate`：未在 Record 显式覆写，沿用基类。  
- `serviceCallback`：未在 Record 显式覆写，沿用基类网络循环。  
- `final`：等待 `RecordSessionManager` 为空 -> 停 taskPool -> 父类 final -> 销毁 session manager/db pool。

## 启动失败点
1. mysql URL 缺失/连接失败。  
2. threadPoolCapacity 非法导致 taskPool init 失败。  
3. ifname 无效导致 IP 解析异常。  
4. zSubNetService 初始化失败（super 注册/网络端口）。

## RAG 标签
`main,init,final,zSubNetService,dbConnPool,taskPool,daemon`
