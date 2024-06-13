#include <iostream>

#include "kv_store.h"
#include "network.h"

int main(int argc, char* argv[])
{
    KvStore kv_store;
    kv_store.netStart();
    
    kv_store.eventLoop();

    return 0;
}