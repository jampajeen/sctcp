
#ifndef POLLER_H
#define	POLLER_H


#include "common.h"

#include "PollerEventHandler.h"

class Poller {
public:
    virtual bool init() = 0;
    virtual bool add(int fd, unsigned short event) = 0;
    virtual bool del(int fd) = 0;
    virtual bool modify(int fd, unsigned short event) = 0;
    virtual bool waitForEvents(unsigned int timeout) = 0;
    virtual void processEvents(int rselect) = 0;
    virtual bool destroy() = 0;
    
protected:
    
    /*
     * Handler
     */
    struct HandlerRegistration {
        PollerEventHandler *handler;
        int listenFd;
    } _serverHandler;

    /*
     * Max fds in the system 
     */
    int _maxSystemFd;
    
    /*
     * Max fds in this poller 
     */
    int _maxPollerFd; 
    
    /*
     * Current fds in this poller 
     */
    int _numPollerFd; 
    
};

#endif	/* POLLER_H */
