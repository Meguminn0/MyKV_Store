#include <iostream>
#include <map>
#include <sys/epoll.h>
#include <assert.h>

#include "network.h"

int reactor::epoll_fd;
std::map<int, reactor::connector*> reactor::fd_map;

reactor& reactor::getInstance()
{
    static reactor instance;
    instance.init();
    return instance;
}

reactor::connector* reactor::getConnector(int fd)
{
    
    return fd_map.find(fd)->second;
}

int reactor::epoll_add(int fd, epoll_event* event, CALLBACK_CON func_cb)
{
    if(fd_map.find(fd) == fd_map.end())
    {
        connector *newConnctor = new connector();
        newConnctor->fd = fd;
        newConnctor->func_cb = func_cb;
        fd_map.insert(std::make_pair(fd, newConnctor));
    }
    return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, event);
}

int reactor::epoll_mod(int fd, epoll_event* event, CALLBACK_CON func_cb)
{
    fd_map[fd]->func_cb = func_cb;
    return epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, event);
}

int reactor::epoll_del(int fd)
{
    if(fd_map.find(fd) != fd_map.end())
    {
        delete fd_map[fd];
        fd_map[fd] = nullptr;
        fd_map.erase(fd);
    }
    return epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
}

int reactor::epoll_waiting(epoll_event* events, int maxEvents, int timeout)
{
    return epoll_wait(epoll_fd, events, maxEvents, timeout);
}

void reactor::init()
{
    epoll_fd = epoll_create(1);
    assert(epoll_fd != -1);
}