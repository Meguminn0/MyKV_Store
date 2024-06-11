/*
 * network 头文件
 * 定义网络层数据结构和接口
 */

#include <iostream>
#include <map>
#include <sys/epoll.h>

#define SENDBUFF_SIZE 1024
#define RECVBUFF_SIZE 1024

class reactor;
typedef void (*CALLBACK_CON)(int, reactor&);

// reactor 模型
class reactor
{
public:
    struct connector
    {
        int fd = -1;                         // 本连接fd
        size_t sendLen = 0;                 // 发送缓冲区数据大小
        size_t recvLen = 0;                 // 接收缓冲区数据大小
        char sendBuf[SENDBUFF_SIZE] = { 0 };    // 发送缓冲区
        char recvBuf[RECVBUFF_SIZE] = { 0};    // 接收缓冲区
        CALLBACK_CON func_cb = nullptr; // 回调函数
    };

public:
    // 单例模式：懒汉单例
    static reactor& getInstance();
    connector* getConnector(int fd);
    int epoll_add(int fd, epoll_event* event, CALLBACK_CON cb_Func);
    int epoll_mod(int fd, epoll_event* event, CALLBACK_CON cb_Func = nullptr);
    int epoll_del(int fd);
    int epoll_waiting(epoll_event* evnets, int maxEvents, int timeout);

protected:
    static void init();

private:
    reactor() {  }
    reactor(const reactor&) = delete;
    const reactor& operator=(const reactor&) = delete;

    static int epoll_fd;
    static std::map<int, connector*> fd_map;
};