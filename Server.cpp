//
// Created by zhsy on 19-6-8.
//

#include "Server.h"
#include "base/Util.h"
#include <cstring>
#include <arpa/inet.h>
#include <iostream>
#include "Connection.h"
#include "task/TaskQueue.h"


Server::Server(int port):_listenFd(socketCreate(port)),_running(false),_acceptThreads(4,150000),
        _dealThreads(8),_taskQueue(new TaskQueue()){
    std::cout<<"Create Socket: Port="<<port<<std::endl;
    _acceptThreads.setTaskQueue(_taskQueue);
    _dealThreads.setTaskQueue(_taskQueue);
    std::cout<<"Create Server"<<std::endl;

}
Server::~Server(){

}
int Server::registerDealer(Dealer dealer){

}
void Server::startListen(){
    std::cout<<"Start listen"<<std::endl;
    _acceptThreads.startLoop();
    _dealThreads.startLoop();
    std::cout<<"has detach ..."<<std::endl;
    _running=true;
    struct sockaddr_in client_addr;
    socklen_t socklen=sizeof(struct sockaddr_in);
    while(_running){
        std::cout<<"listening ..."<<std::endl;
        memset(&client_addr,0,socklen);
        int connFd;
        connFd=accept(_listenFd,(struct sockaddr*)&client_addr,&socklen);
        std::cout << "Get a connect:IP=" << inet_ntoa(client_addr.sin_addr) <<
                " Port="<<ntohs(client_addr.sin_port)<<std::endl;
        if (setnonblocking(connFd) < 0) {
            perror("setnonblock error");
        }
        _acceptThreads.putConnection(std::shared_ptr<Connection>
                (new Connection(connFd,client_addr)));

    }
}
void Server::quit(){
    _running=false;
}
void Server::setMessageCallBack(WorkFunctor onMessage){
    _dealThreads.setMessageCallBack(onMessage);
}