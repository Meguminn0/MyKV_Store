#include <iostream>
#include <vector>

#include "rbtree.h"
#include "kv_store.h"
#include "network.h"

int main()
{
    KvStore kv_store;

    kv_store.NetStart();



    kv_store.EventLoop();

	return 0;
}