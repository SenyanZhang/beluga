#include "Epoll.h"
#include <cstring>
//
// Created by zhsy on 19-6-8.
//

Epoll::Epoll(int size):_waitFd(epoll_create(size)),_events(){

}

Epoll::~Epoll(){

}

bool Epoll::addChannel(ptrChannel channel){

    channelPool[channel->getFd()] = channel;
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = channel->getEvents();
    ev.data.fd = channel->getFd();

    if (epoll_ctl(_waitFd, EPOLL_CTL_ADD, channel->getFd(), &ev) < 0)
    {
        fprintf(stderr, "epoll set modify error: fd=%d\n", channel->getFd());
        return -1;
    }

}
int Epoll::updateChannel(ptrChannel channel){

    channelPool[channel->getFd()] = channel;
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = channel->getEvents();
    ev.data.fd = channel->getFd();

    if (epoll_ctl(_waitFd, EPOLL_CTL_MOD, channel->getFd(), &ev) < 0)
    {
        fprintf(stderr, "epoll set insertion error: fd=%d\n", channel->getFd());
        return -1;
    }

}

bool Epoll::removeChannel(ptrChannel){

}


Epoll::VectorCh Epoll::poll(){
    int nfds = epoll_wait(_waitFd, _events, 100, -1);
    if (nfds == -1)
    {
        perror("epoll_wait");
    }
    VectorCh v;  //内置指针不能隐式转化为智能指针

    for(int i=0;i<nfds;i++){
        int readfd=_events[i].data.fd;
        channelPool[readfd]->setRevents(_events[i].events);
        v.push_back(channelPool[readfd]);
    }
    return v;
}