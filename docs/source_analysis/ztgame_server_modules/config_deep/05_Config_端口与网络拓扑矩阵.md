# 端口与网络拓扑矩阵

## 逻辑拓扑
客户端 -> FL(login_port) -> Gateway -> Session/Scenes/Record/Bill/Mini
                              \-> Super(服务发现)

## 端口矩阵（字段级）
- Super: `port`（被各子服务 `server+port` 连接）
- FL: `login_port`,`inside_port`,`ping_port`
- Gateway/Session/Scenes/Record/Bill/Mini: 各自监听端口 + Super 出口端口
- 外网端口：`ServerEntry.extport`
- 内网端口：`ServerEntry.port`

## IP 映射
- 本机内网IP：`ifname` -> `getIPByIfName`
- 公网IP：DB `SERVERLIST.EXTIP`

## 风险
- 单网卡容器里 `eth0` 以外名称导致上报空IP。
- `EXTIP/EXTPORT` 与实际 NAT/SLB 不一致导致“登录成功但进图失败”。
