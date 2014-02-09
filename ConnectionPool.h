
#ifndef CONNECTIONPOOL_H
#define	CONNECTIONPOOL_H

#include <map>
#include <list>
#include "common.h"
#include "Mutex.h"
#include "Connection.h"
#include "ConnectionEventHandler.h"

class ConnectionPool {
    
public:
    ConnectionPool(ConnectionEventHandler *handler, bool isNonblock, int defaultSize);
    ~ConnectionPool();
    
    /*
     * create/get connection
     */
    Connection* createConnection();
    /*
     * release connection
     */
    bool releaseConnection ( int sockfd );
    
    /*
     * find connection in pool
     */
    Connection* findConnection( int sockfd );
    
private:
    
    bool isOutOfRange(int sockfd);
    
    ConnectionEventHandler *_handler;
    bool _isNonblock;
    
    /*
     * size of pool
     */
    int _size;
    /*
     * number of connection in pool 
     */
    int _num;
    
    Connection *_conns[MAX_CONNECTION];
    std::list<Connection*> _availableConns;
};

ConnectionPool::ConnectionPool(ConnectionEventHandler *handler, bool isNonblock, int defaultSize) : 
_handler(handler), _isNonblock(isNonblock), _num(defaultSize), _size(MAX_CONNECTION) {
    
    LOG_I("Initializing pool...");
    if(_num > _size) {
        _num = _size;
    }
    
    /* initial array with max size */
    for(int i = 0; i < _size; i++) {
        _conns[i] = NULL;
    }
    
    /* allocate by default size */
    for(int i = 0; i < _num; i++) {
        
        Connection *conn = new Connection(_handler, _isNonblock);
        /* put to connection array*/
        _conns[conn->_fd] = conn;
        /* put to available list */
        _availableConns.push_back(conn);
    }
    LOG_I("Initializing pool completed");
}

ConnectionPool::~ConnectionPool() { 
    
    /* clear list */
    _availableConns.clear();
    
    /* free array */
    int i;
    for(i = 0; i < _num; i++) {
        delete _conns[i];
    }
    delete[] _conns;
    
    LOG_I("Pool was destroyed");
}

Connection* ConnectionPool::createConnection() {
    Connection *conn = NULL;
    
    if(!_availableConns.empty()) {
        /*
         * if there is available connection, use it
         */
        conn = _availableConns.front();
        /* remove from available list */
        _availableConns.pop_front();
        
        LOG_I("Use existing connection");
    } else {
        
        /* exceed limitation*/
        if( _num >= _size) {
            LOG_W("Can not create connection, exceed pool size limitation.");
            return NULL;
        }
        /*
         * if not available, create new one
         */
        conn = new Connection(_handler, _isNonblock);
        
        /* put to connection array*/
        _conns[conn->_fd] = conn;
        _num++;
        
        /* put to available list */
        _availableConns.push_back(conn);
        LOG_I("Allocate new connection");
    }
    
    return conn;
}

inline bool ConnectionPool::isOutOfRange(int sockfd) {
    return (sockfd > 0 && sockfd < _size) ? false : true;
}

bool ConnectionPool::releaseConnection(int sockfd) {
    Connection *conn = findConnection(sockfd);
    if( !conn ) {
        LOG_E("Can't find connection to release");
        return false;
    }
    
    /* remove connection from */
    _conns[conn->_fd] = NULL;
    
    /* re-initial connection */
    conn->renew(_handler, _isNonblock);
    
    /* re-put to connection array*/
    _conns[conn->_fd] = conn;
    
    /* put to available list */
    _availableConns.push_back(conn);
    
    return true;
}

Connection* ConnectionPool::findConnection(int sockfd) {
    
    if(isOutOfRange(sockfd) ) {
        LOG_E("sockfd is out of range");
        return NULL;
    }
    return _conns[sockfd];
}

#endif	/* CONNECTIONPOOL_H */
