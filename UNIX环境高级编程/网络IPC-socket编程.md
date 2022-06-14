# 创建一个套接字
调用socket函数返回一个socket描述符，该socket描述符在某些功能上近似于文件描述符。如，socket函数支持read\write\dup
```C++
#include <sys/socket.h>
int socket(int domain, int type, int protocol)
```
1. 参数domain指定通信特性，包括地址的协议，常用的有两个
+ AF_INET(IPv4地址)
+ AF_INET6(IPv6地址)

2. 参数type指定通信特征
+ SOCK_STREAM(有序、可靠、双向、面向连接的字节流)
+ SOCK_DGRAM(固定长度、不可靠、无连接的报文船体)

3. 参数protocol一般都是0
当protocol指定为0时，函数会自动选择匹配的默认协议。(但有时你可能也会想自己指定协议)

# 关闭套接字
```C++
// 创建一个socket将被用于关闭
fd =socket(AF_INET, SOCK_STREAM, 0);
// 关闭的方法：
// 方法1
close(fd);
// 方法2
int shutdown(int sockfd, int how);
```
close()会关闭套接字，但是当套接字被引用时(dup),函数会等待该引用解除了再关闭。
但shutdown()允许使一个套接字处于不活动状态。并且shutdown的参数how可以指定是关闭socket的读端还是写端。