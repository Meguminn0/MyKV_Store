#ifndef KV_STORE_H_
#define KV_STORE_H_
#include "network.h"

class KvStore
{
public:
    KvStore();
    void netStart();
    void eventLoop();

protected:
    static void accept_callBack(int listenfd);
    static void recv_callBack(int clientfd);
    static void send_callBack(int clientfd);

private:
    int listen_socketfd_;
    static Reactor& reactor_;
};

#endif