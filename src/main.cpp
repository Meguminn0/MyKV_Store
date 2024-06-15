#include <iostream>

#include "kv_store.h"
#include "network.h"

int main(int argc, char* argv[])
{
    KvStore kv_store;
    kv_store.NetStart();
    
    kv_store.EventLoop();

    return 0;
}