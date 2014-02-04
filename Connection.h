/* 
 * File:   Connection.h
 * Author: TJ
 *
 * Created on April 25, 2013, 2:23 PM
 */

#ifndef CONNECTION_H
#define	CONNECTION_H

#include <sys/socket.h>
#include <time.h>

#include "Buffer.h"
#include "Utils.h"
#include "SocketWrapper.h"
#include "TaskEventHandler.h"
#include "ConnectionEventHandler.h"

/*
 * Connection Status
 */

#define CONN_STATUS_CLOSED     0x0000
#define CONN_STATUS_IDLE       0x0001
#define CONN_STATUS_WRITING    0x0002
#define CONN_STATUS_READING    0x0004
#define CONN_STATUS_CONNECTING  0x0008

class Connection {
    friend class ConnectionPool;
private:
    /* re-use connection, used by ThreadPool */
    void renew( ConnectionEventHandler *handler, bool isNonblock);
    
public:
    
    /*
     * get socket fd
     */
    int fd() const { return _fd; }
    
    /*
     * set connection status
     */
    void setStatus(unsigned short status);
    unsigned short status() const;
    
    /*
     * I/O read/write
     */
    virtual int read();
    virtual int write(char *buf, int buflen);
    
    /*
     * Open/Close connection
     */
    virtual int openAsync(char *host, unsigned short port);
    virtual void open(char *host, unsigned short port);
    virtual void close();
    
protected:
    
    Connection( ConnectionEventHandler *handler, bool isNonblock );
    virtual ~Connection();
    
    /*
     * socket fd
     */
    int _fd;
    
    /*
     * established.
     */
    timeval _openedTime;
    
    /*
     * closed.
     */
    timeval _closedTime;
    
    /*
     * sent time
     */
    timeval _writeTime;
    /*
     * received time
     */
    timeval _readTime;
    
    
    /*
     * Status of connection
     */
    unsigned short _status;
    
    /*
     * IO buffer
     */
    char _readBuf[MAX_BUF_SIZE];
    char _writeBuf[MAX_BUF_SIZE];
    
    /*
     * Remote address
     */
    char _peerIp[INET6_ADDRSTRLEN];
    unsigned short _peerPort;
    
    /*
     * handler
     */
    ConnectionEventHandler *_handler;

    bool _isNonblock;
    
};

Connection::Connection( ConnectionEventHandler *handler, bool isNonblock ) : _isNonblock(isNonblock), _status(CONN_STATUS_CLOSED), _handler(handler) {
    _fd = SocketWrapper::socket();
}

Connection::~Connection() {
}

void Connection::renew( ConnectionEventHandler *handler, bool isNonblock) {
    _fd = SocketWrapper::socket();
    _handler = handler;
    _isNonblock = isNonblock;
    _status = CONN_STATUS_CLOSED;
}

void Connection::setStatus(unsigned short status) {
    _status = status;
}

unsigned short Connection::status() const {
    return _status;
}

int Connection::read() {
    /* before reading */
    _handler->onBeginRead(_fd);
    
    /* read from socket */
    int r = SocketWrapper::recv(_fd, _readBuf, sizeof(_readBuf), 0);
    
    if(r > 0) {
        /* update info. */
        _readTime = Utils::timevalNow();
        
        _handler->onReadCompleted(_fd, _readBuf, r);
        
        if (_isNonblock) {
            _status = (CONN_STATUS_WRITING | CONN_STATUS_READING);
        } else {
            _status = CONN_STATUS_WRITING;
        }
        
    } else if(r == 0) {
        LOG_D("[%d]Connection closed by peer",_fd);
        
        /* clear connection */
        close();
        
    } else if(r < 0) {
        
        _handler->onReadError(_fd, r);
    }
}

int Connection::write(char *buf, int buflen) {
    
    /* before writing */
    _handler->onBeginWrite(_fd);
    
    int r = SocketWrapper::send(_fd, buf, buflen, 0);
    
    if(r < 0) {
        _handler->onWriteError(_fd, r);
        
    } else {
        /* update info. */
        _writeTime = Utils::timevalNow();
        
        _handler->onWriteCompleted(_fd, r);
        
        if (_isNonblock) {
            _status = (CONN_STATUS_WRITING | CONN_STATUS_READING);
        } else {
            _status = CONN_STATUS_READING;
        }
    }
    
}

int Connection::openAsync(char *host, unsigned short port) {
    int ret;
    
    SocketWrapper::setOptionNonblock(_fd, true);
    
    if ( (ret = SocketWrapper::connect( _fd, host , port )) == -1) {
        if (errno == EINPROGRESS) {
            /*
             * Nonblocking fine
             */
            _status = CONN_STATUS_CONNECTING;
            LOG_D("EINPROGRESS OK");
        } else {
            /*
             * connection refused immediately
             */
            LOG_W("error connect: %s\n", strerror(errno));
            /* tick handler*/
            _handler->onOpenError(_fd, ret);
        }
    } else {
        /*
         * maybe connected immediately @ local
         */
        LOG_D("Connected");
    }
    
    return ret;
}

void Connection::open(char *host, unsigned short port) {
    
    /* non-blocking mode? */
    SocketWrapper::setOptionNonblock(_fd, _isNonblock);

    /* connect to remote host */
    int r = SocketWrapper::connect( _fd, host , port );
    if(r < 0) {
        
        /* tick handler*/
        _handler->onOpenError(_fd, r);
        
    } else {
        /* update connection info. */
        _openedTime = Utils::timevalNow();
        SocketWrapper::getPeerName(_fd, _peerIp, sizeof (_peerIp), &_peerPort);
        
        /* tick handler */
        _handler->onOpened(_fd);
        
        if (_isNonblock) {
            /* connection status */
            _status = (CONN_STATUS_WRITING | CONN_STATUS_READING);

        } else {
            /* connection status */
            _status = (CONN_STATUS_WRITING);
        }
    }
}

void Connection::close() {
    /* close socket*/
    SocketWrapper::close(_fd);
    
    /* update info.*/
    _closedTime = Utils::timevalNow();
    _status = CONN_STATUS_CLOSED;
    
    /* tick handler */
    _handler->onClosed(_fd);
    
}

#endif	/* CONNECTION_H */
