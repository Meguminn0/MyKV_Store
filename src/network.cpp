#include <iostream>
#include <map>
#include <sys/epoll.h>
#include <assert.h>

#include "network.h"

int Reactor::epoll_fd_;
std::map<int, Reactor::Connector*> Reactor::fd_map_;

Reactor& Reactor::getInstance()
{
    static Reactor instance;
    instance.init();
    return instance;
}

Reactor::Connector* Reactor::getConnector(int fd)
{
    return fd_map_.find(fd)->second;
}

int Reactor::epoll_add(int fd, epoll_event* event, CALLBACK_FUNCTION function)
{
    if(fd_map_.find(fd) == fd_map_.end())
    {
        Connector *new_connctor = new Connector();
        new_connctor->fd = fd;
        new_connctor->call_back_function = function;
        fd_map_.insert(std::make_pair(fd, new_connctor));
    }
    return epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, event);
}

int Reactor::epoll_mod(int fd, epoll_event* event, CALLBACK_FUNCTION function)
{
    fd_map_[fd]->call_back_function = function;
    return epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, event);
}

int Reactor::epoll_del(int fd)
{
    if(fd_map_.find(fd) != fd_map_.end())
    {
        delete fd_map_[fd];
        fd_map_[fd] = nullptr;
        fd_map_.erase(fd);
    }
    return epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
}

int Reactor::epoll_waiting(epoll_event* events, int max_events, int timeout)
{
    return epoll_wait(epoll_fd_, events, max_events, timeout);
}

void Reactor::init()
{
    epoll_fd_ = epoll_create(1);
    assert(epoll_fd_ != -1);
}