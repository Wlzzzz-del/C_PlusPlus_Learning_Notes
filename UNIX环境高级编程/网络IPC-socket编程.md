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
+ SOCK_DGRAM(固定长度、不可靠、无连接的报文传递)

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

# 字节序转换
TCP/IP协议使用大端字节序，一般的处理器使用小端字节序。因此在传递数据(IP地址\端口)时需要将数据转为大端字节序。
常用的有四种转换字节序方法:
```C++
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostint32);
uint16_t htons(uint16_t hostint16);

uint32_t ntohs(uint32_t hostint32);
uint16_t ntohs(uint16_t hostint16);

```
h表示host主机、n表示net网络、l表示long32位长整数、s表示short16位长整数。

# 地址格式
1. sockaddr
通常使用sockaddr_in结构体填入地址格式、ip地址、端口，然后将sockaddr_in强转为sockaddr供connect函数使用。
原因是sockaddr_in将ip地址与端口分为两个参数，便于赋值。
但是sockaddr将这两者混淆在一个参数内，赋值较为麻烦。
2. 点分十进制
有时候我们也需要打印出地址格式，因此linux提供了两个函数用于将二进制地址格式转为点分十进制字符的函数。
```C++
// 将二进制转为点分十进制字符串
const char* inet_ntop(int domain, const void* restrict addr, char restrict* str, socklen_t size);// 若成功返回字符串指针，若出错返回NULL

// 将字符串格式转为二进制地址
int inet_pton(int domain, const char* restrict str, void* restrict addr);
```
# 地址查询
## 通过域名查询IP地址
调用gethostent会得到hostent结构体，其中的成员char** h_addr_list是指向网络地址数组的指针,返回的IP地址采用网络字节序。
```C++
struct hostent *gethostent(void);// 若成功返回指针，失败返回NULL
```
注:gethostbyname已经过时，较少被采用.
## 获取网络名与网络号
```C++
struct netent *getnetbyaddr (uint32_t net, int type);
struct netent *getnetbyname (const char* name);
struct netent *getnetent(void);
```
网络号按照网络字节序返回
network number一般指主机号,区别于host number，指在主机内的地址。

## 协议名字和协议编号的映射
```C++
struct protoent* getprotobyname(const char* name);
struct protoent* getprotobynumber(int proto);
struct protoent* getprotoent(void);
```
## 将端口号映射到服务名
```C++
struct servent *getservbyname(const char* name, const char* proto);
struct servent *getserbyport(int port, const char* proto);
struct servent *getservent(void);
```
## 用来代替gethostbyname的新函数
getaddrinfo函数允许将主机名和服务名映射到一个地址。当然可以只提供一个，另一个应该为空指针
```C++
int getaddrinfo(const char* restrict host,
const char* restrict service,
const struct addrinfo * restrict hint,
struct addrinfo **restrict res);// 成功时返回0，不成功返回错误码
// 成功时返回的res是一个addrinfo链表
// 通过设置hint的ai_family\ai_flag\ai_protocol\ai_socktype可以用于过滤地址,其他整数字段必须设置为0

const char* gai_strerror(int error);// 将错误码转为错误消息

void freeaddrinfo(struct addrinfo *ai);// 用来释放addrinfo链表
```
