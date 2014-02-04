/* 
 * File:   ConnectionEventHandler.h
 * Author: TJ
 *
 * Created on April 26, 2013, 4:12 PM
 */

#ifndef CONNECTIONEVENTHANDLER_H
#define	CONNECTIONEVENTHANDLER_H


class ConnectionEventHandler {
public:
    virtual void onBeginRead(int fd) = 0;
    virtual void onBeginWrite(int fd) = 0;
    
    virtual void onReadCompleted(int fd, char *buf, int buflen) = 0;
    virtual void onWriteCompleted(int fd, int buflen) = 0;
    
    virtual void onWriteError(int fd, int error) = 0;
    virtual void onReadError(int fd, int error) = 0;
    
    virtual void onOpened(int fd) = 0;
    virtual void onClosed(int fd) = 0;
    virtual void onOpenError(int fd, int error) = 0;
    //virtual void onOpenTimeout(int fd) = 0;
};

#endif	/* CONNECTIONEVENTHANDLER_H */

