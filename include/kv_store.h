#ifndef KV_STORE_H_
#define KV_STORE_H_
#include <vector>
#include <string>
#include "network.h"
#include "cmdfactory.h"

class KvStore
{
public:
    KvStore();
    ~KvStore();
    void NetStart();
    void EngineInit();
    void EventLoop();

protected:
    static void Accept_callBack(int listenfd);
    static void Recv_callBack(int clientfd);
    static void Send_callBack(int clientfd);

    static size_t ExecuteCmd(std::string cmd, size_t cmd_len, std::string& result);
    static void Split(std::vector<std::string>& tokens, std::string cmd);

private:
    int listen_socketfd_;
    static Reactor& reactor_;
    static CmdFactory& cmd_factory_;
};

#endif