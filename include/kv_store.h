#ifndef KV_STORE_H_
#define KV_STORE_H_
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

private:
    int listen_socketfd_;
    static Reactor& reactor_;
};

#endif