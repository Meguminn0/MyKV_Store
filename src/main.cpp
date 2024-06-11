#include <iostream>

#include "network.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

void recv_callBack(int clientfd, reactor& rtor);
void send_callBack(int clientfd, reactor& rtor);

void send_callBack(int clientfd, reactor& rtor)
{
    reactor::connector* clinetConnector = rtor.getConnector(clientfd);
    int newSendLen = send(clientfd, clinetConnector->sendBuf, clinetConnector->sendLen, 0);

    if(newSendLen < 0)
    {
        printf("send() error: %s\n", strerror(errno));
        close(clientfd);
        rtor.epoll_del(clientfd);
        return;
    }
    
    memset(clinetConnector->sendBuf, '\0', SENDBUFF_SIZE);
    clinetConnector->sendLen = 0;

    struct epoll_event ev;
    ev.data.fd = clientfd;
    ev.events = EPOLLIN;
    rtor.epoll_mod(clientfd, &ev, recv_callBack);
}

void recv_callBack(int clientfd, reactor& rtor)
{
    reactor::connector* clinetConnector = rtor.getConnector(clientfd);
    int newRecvLen = recv(clientfd, clinetConnector->recvBuf + clinetConnector->recvLen, RECVBUFF_SIZE, 0);

    if(newRecvLen < 0)
    {
        printf("recv() error: %s\n", strerror(errno));
        close(clientfd);
        rtor.epoll_del(clientfd);
        return;
    }
    else if(newRecvLen == 0)
    {
        // 客户端主动断开连接
        close(clientfd);
        rtor.epoll_del(clientfd);
        std::cout << "fd: " << clientfd << "closed!!!" << std::endl;
    }
    else
    {
        clinetConnector->recvLen += newRecvLen;

        std::cout << "fd: " << clientfd << "recv data: " << clinetConnector->recvBuf << std::endl;
        // 处理客户端发送的数据

        memset(clinetConnector->sendBuf, '\0', SENDBUFF_SIZE);
        memcpy(clinetConnector->sendBuf, clinetConnector->recvBuf, clinetConnector->recvLen);
        clinetConnector->sendLen = clinetConnector->recvLen;
        
        memset(clinetConnector->recvBuf, '\0', SENDBUFF_SIZE);
        clinetConnector->recvLen = 0;

        struct epoll_event ev;
        ev.data.fd = clientfd;
        ev.events = EPOLLOUT;
        rtor.epoll_mod(clientfd, &ev, send_callBack);
    }
}

void accept_callBack(int listenfd, reactor& rtor)
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientfd = accept(listenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if(clientfd == -1)
    {
        printf("accept error\n");
        return;
    }

    struct epoll_event ev;
    ev.data.fd = clientfd;
    ev.events = EPOLLIN;
    rtor.epoll_add(clientfd, &ev, recv_callBack);
}

int main(int argc, char* argv[])
{
    reactor& rtor = reactor::getInstance();

    int listenSocketfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenSocketfd != -1);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(2024);

    if(-1 == bind(listenSocketfd, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)))
    {
        std::cout << "bind error" << std::endl;
        exit(-1);
    }

    listen(listenSocketfd, 10);
    
    std::cout << "listen port: " << 2024 << "sockfd: " << listenSocketfd << std::endl;
    struct epoll_event ev;
    ev.data.fd = listenSocketfd;
    ev.events = EPOLLIN;
    rtor.epoll_add(listenSocketfd, &ev, accept_callBack);

    struct epoll_event events[1024] = { 0 };
    while(1)
    {
        int count = rtor.epoll_waiting(events, 1024, -1);
        assert(count != -1);
        if(count != 0)
        {
            for(int i = 0; i < count; ++i)
            {
                int fd = events[i].data.fd;
                reactor::connector* cnnt = rtor.getConnector(fd);
                if(EPOLLIN & events[i].events) 
                {
                    if(fd == listenSocketfd)
                    {
                        std::cout << "new clinet come in" << std::endl;
                    }
                    cnnt->func_cb(fd, rtor);
                }
                else if(EPOLLOUT & events[i].events)
                {
                    cnnt->func_cb(fd, rtor);
                }
            }
        }
    }

    return 0;
}