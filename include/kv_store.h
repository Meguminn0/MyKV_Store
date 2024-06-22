#ifndef KV_STORE_H_
#define KV_STORE_H_
#include <vector>
#include "network.h"

class KvStore
{
public:
    KvStore();
    void NetStart();
    void EventLoop();

protected:
    static void Accept_callBack(int listenfd);
    static void Recv_callBack(int clientfd);
    static void Send_callBack(int clientfd);

    static size_t ProcessCmd(const char* cmd, size_t cmd_len, std::string& result);
    static void Split(std::vector<std::string>& tokens, const char* cmd);

private:
    int listen_socketfd_;
    static Reactor& reactor_;
};

#endif