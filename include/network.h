/*
 * network 头文件
 * 定义网络层数据结构和接口
 */
#ifndef NET_WORK_H_
#define NET_WORK_H_

#include <iostream>
#include <map>
#include <sys/epoll.h>

#define SEND_BUFFER_SIZE 1024
#define RECV_BUFFER_SIZE 1024
#define CMD_SIZE RECV_BUFFER_SIZE

typedef void (*CALLBACK_FUNCTION)(int);

// Reactor 模型
class Reactor
{
public:
    struct Connector
    {
        int fd = -1;                         // 本连接fd
        size_t send_len = 0;                 // 发送缓冲区数据大小
        size_t recv_len = 0;                 // 接收缓冲区数据大小
        char send_buffer[SEND_BUFFER_SIZE] = { 0 };    // 发送缓冲区
        char recv_buffer[RECV_BUFFER_SIZE] = { 0};    // 接收缓冲区
        CALLBACK_FUNCTION call_back_function = nullptr; // 回调函数
    };

public:
    // 单例模式：懒汉单例
    static Reactor& getInstance();
    Connector* getConnector(int fd);
    int epoll_add(int fd, epoll_event* event, CALLBACK_FUNCTION function);
    int epoll_mod(int fd, epoll_event* event, CALLBACK_FUNCTION function = nullptr);
    int epoll_del(int fd);
    int epoll_waiting(epoll_event* evnets, int max_events, int timeout);

protected:
    static void init();

private:
    Reactor();
    ~Reactor();
    Reactor(const Reactor&) = delete;
    const Reactor& operator=(const Reactor&) = delete;

    static int epoll_fd_;
    static std::map<int, Connector*> fd_map_;
};

#endif