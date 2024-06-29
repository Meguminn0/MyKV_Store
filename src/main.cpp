#include <iostream>
#include <vector>
#include <signal.h>

#include "rbtree.h"
#include "kv_store.h"
#include "network.h"

void signalHandler(int signal) 
{
    std::cout << "\nMyKV_Store is now ready to exit, bye bye..." << std::endl;
    exit(signal);
}

int main()
{
    KvStore kv_store;

    signal(SIGINT, signalHandler);

    kv_store.NetStart();

    kv_store.EngineInit();

    kv_store.EventLoop();

	return 0;
}