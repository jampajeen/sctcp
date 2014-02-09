
#ifndef POLLEREVENTHANDLER_H
#define	POLLEREVENTHANDLER_H


#include "common.h"

/*
 * Interface PollerEventHandler
 */
class PollerEventHandler {
    
public:
    virtual void onPollerAccept(int fd) = 0;
    virtual void onPollerRead(int fd) = 0;
    virtual void onPollerWrite(int fd) = 0;
};


#endif	/* POLLEREVENTHANDLER_H */
