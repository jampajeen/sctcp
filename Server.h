/* 
 * File:   Server.h
 * Author: TJ
 *
 * Created on April 25, 2013, 11:24 PM
 */

#ifndef SERVER_H
#define	SERVER_H

#include "common.h"
#include "Log.h"
#include "PollerEventHandler.h"
#include "Poller.h"
#include "PollerSelect.h"
#include "SocketWrapper.h"

/*
 * Example of class Server that extended PollerEventHandler
 */
class Server : public PollerEventHandler {
    
private:
    /*
     * Server main loop
     */
    bool run();

    /*
     * Derived from PollerEventHandler
     */
    void onPollerAccept(int fd);
    void onPollerRead(int fd);
    void onPollerWrite(int fd);

public:
    Server() : _running(false) {}
    ~Server() {}

    /*
     * Server controller
     */
    bool start();
    bool stop();
    bool restart();

private:
    /*
     * listener fd
     */
    int _listenFd;
    
    /*
     * Running flags
     */
    bool _running;

    /*
     * IO buffer
     */
    Buffer _readBuf;
    Buffer _writeBuf;

    /*
     * Abstract Poller
     */
    Poller *_poller;
};


void Server::onPollerAccept(int fd) {
    
    int afd = SocketWrapper::accept(_listenFd);
    if (afd > 0) {
        LOG_I("Accept new connection");
        
        _poller->add(afd, POLLER_EV_READ);
    }
}

void Server::onPollerRead(int fd) {
    char buf[65535];
    /*
     * actual read from socket
     */
    memset(buf,0,sizeof(buf));
    int r = SocketWrapper::recv(fd, buf, sizeof(buf), 0);
    
    if(r > 0) {
        
        /* Testing class Buffer*/
        _readBuf.clear();
        _readBuf.append(buf, r);
        LOG_I("[%d]Recv: %s", fd, _readBuf.getChars());
        
        _writeBuf.clear();
        _writeBuf.append(_readBuf.getChars(), _readBuf.length());
        
        /*
         * Toggle write
         */
        _poller->modify(fd, POLLER_EV_WRITE);
        
    } else if(r == 0) {
        LOG_I("[%d]Connection closed by peer",fd);
        _poller->del(fd);
        
    } else if(r == -1) {
        LOG_E("[%d]Socket error",fd);
        _poller->del(fd);
    }
    
}

void Server::onPollerWrite(int fd) {
    /*
     * actual write to socket
     */
    SocketWrapper::send(fd, _writeBuf.getChars(), _writeBuf.length(), 0);
    LOG_I("[%d]Send: %s",fd, _writeBuf.getChars());
    
    /*
     * Toggle read
     */
    _poller->modify(fd, POLLER_EV_READ);
}

bool Server::run() {
    do {
        
        /*
         *  Wait for new event with infinity timeout
         */
        _poller->waitForEvents(INFINITE_TIMEOUT);

    } while (_running);

    return true;
}

bool Server::start() {
    
    _running = true;
    
    /* Bind socket */
    int port = 7070;
    _listenFd = SocketWrapper::bindAndListen(port);
    if (_listenFd <= 0) {
        LOG_E("Can not bind port %d", port);
        return false;
    }
    
    LOG_I("Socket listen on port %d ", port);

    /* Initial poller */
    _poller = new PollerSelect(_listenFd, this);
    _poller->init();
    _poller->add(_listenFd, POLLER_EV_READ);

    run();
}

#endif	/* SERVER_H */

