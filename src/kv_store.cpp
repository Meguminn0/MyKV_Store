#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <memory>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "kv_store.h"
#include "cmdfactory.h"
#include "cmdstrategy.h"
#include "t_string.h"
#include "keycommand.h"

Reactor& KvStore::reactor_ = Reactor::GetInstance();

KvStore::KvStore() : listen_socketfd_(-1)
{

}

KvStore::~KvStore()
{

}

void KvStore::NetStart()
{
    listen_socketfd_ = socket(AF_INET, SOCK_STREAM, 0);
    assert(listen_socketfd_ != -1);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(2024);

    if(-1 == bind(listen_socketfd_, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)))
    {
        std::cout << "bind error" << std::endl;
        exit(-1);
    }

    listen(listen_socketfd_, 10);
    
    std::cout << "listen port: " << 2024 << " sockfd: " << listen_socketfd_ << std::endl;
}

void KvStore::EngineInit()
{
    RBTree* rbtree = new RBTree();
    std::set<std::string>* key_set = new std::set<std::string>();
    StringCmdInit(rbtree);
    KeyCmdInit(key_set);
}

void KvStore::EventLoop()
{
    struct epoll_event ev;
    ev.data.fd = listen_socketfd_;
    ev.events = EPOLLIN;
    reactor_.epoll_add(listen_socketfd_, &ev, Accept_callBack);

    struct epoll_event events[1024] = { 0 };
    while(1)
    {
        int count = reactor_.epoll_waiting(events, 1024, -1);
        assert(count != -1);
        if(count != 0)
        {
            for(int i = 0; i < count; ++i)
            {
                int fd = events[i].data.fd;
                Reactor::Connector* connect = reactor_.getConnector(fd);
                if(EPOLLIN & events[i].events) 
                {
                    if(fd == listen_socketfd_)
                    {
                        std::cout << "new clinet come in" << std::endl;
                    }
                    connect->call_back_function(fd);
                }
                else if(EPOLLOUT & events[i].events)
                {
                    connect->call_back_function(fd);
                }
            }
        }
    }
}

void KvStore::Accept_callBack(int listenfd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int clientfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(clientfd == -1)
    {
        printf("accept error\n");
        close(clientfd);
        return;
    }

    struct epoll_event ev;
    ev.data.fd = clientfd;
    ev.events = EPOLLIN;
    reactor_.epoll_add(clientfd, &ev, Recv_callBack);
}

void KvStore::Recv_callBack(int clientfd)
{
    Reactor::Connector* clinet_connector = reactor_.getConnector(clientfd);
    int new_recv_len = recv(clientfd, clinet_connector->recv_buffer + clinet_connector->recv_len, RECV_BUFFER_SIZE, 0);

    if(new_recv_len < 0)
    {
        printf("recv() error: %s\n", strerror(errno));
        close(clientfd);
        reactor_.epoll_del(clientfd);
    }
    else if(new_recv_len == 0)
    {
        // 客户端主动断开连接
        close(clientfd);
        reactor_.epoll_del(clientfd);
        std::cout << "fd: " << clientfd << " Active shutdown!!!" << std::endl;
    }
    else
    {
        clinet_connector->recv_len += new_recv_len;

        std::cout << "fd: " << clientfd << " recv data: " << clinet_connector->recv_buffer << std::endl;

        // 处理客户端发送的数据
        std::string result_str;
        ExecuteCmd(clinet_connector->recv_buffer, clinet_connector->recv_len, result_str);
        
        memset(clinet_connector->send_buffer, '\0', SEND_BUFFER_SIZE);
        memcpy(clinet_connector->send_buffer, result_str.c_str(), result_str.size());
        clinet_connector->send_len = result_str.size();
        
        memset(clinet_connector->recv_buffer, '\0', RECV_BUFFER_SIZE);
        clinet_connector->recv_len = 0;

        struct epoll_event ev;
        ev.data.fd = clientfd;
        ev.events = EPOLLOUT;
        reactor_.epoll_mod(clientfd, &ev, Send_callBack);
    }
}

void KvStore::Send_callBack(int clientfd)
{
    Reactor::Connector* clinet_connector = reactor_.getConnector(clientfd);
    int new_send_len = send(clientfd, clinet_connector->send_buffer, clinet_connector->send_len, 0);

    if(new_send_len < 0)
    {
        printf("send() error: %s\n", strerror(errno));
        close(clientfd);
        reactor_.epoll_del(clientfd);
        return;
    }
    
    memset(clinet_connector->send_buffer, '\0', SEND_BUFFER_SIZE);
    clinet_connector->send_len = 0;

    struct epoll_event ev;
    ev.data.fd = clientfd;
    ev.events = EPOLLIN;
    reactor_.epoll_mod(clientfd, &ev, Recv_callBack);
}

// 执行Cmd命令
size_t KvStore::ExecuteCmd(const char* cmd, size_t cmd_len, std::string& result)
{
    std::vector<std::string> tokens;
    Split(tokens, cmd, cmd_len);

    CmdStrategy* cmd_strategy = CmdFactory::GetInstance().GetCmdStrategy(tokens[0]);

    cmd_strategy->Execute(tokens, result);

    return result.size();
}

// 将命令进行拆分
void KvStore::Split(std::vector<std::string>& tokens, const char* cmd, size_t cmd_len)
{
    int count = 0;
    int idx = 0;
    char tmp_cmd[CMD_SIZE] = { '\0' };
    memcpy(tmp_cmd, cmd, cmd_len);

    char* p = strtok(tmp_cmd + idx, " ");
    while(p != nullptr)
    {
        tokens.push_back(p);
        idx = tokens[count].size() + 1;
        ++count;
        p = strtok(p + idx, " ");
    }

    // for(auto& item : tokens)
    // {
    //     std::cout << item << std::endl;
    // }
}
