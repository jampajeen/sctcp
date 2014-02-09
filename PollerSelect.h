
#ifndef POLLERSELECT_H
#define	POLLERSELECT_H


#include "Poller.h"
#include "System.h"
#include "Log.h"

#include <sys/fcntl.h>

class PollerSelect : public Poller {
public:
    PollerSelect(int listenFd, PollerEventHandler *handler);
    virtual ~PollerSelect();

    bool init();
    bool add(int fd, unsigned short event);
    bool del(int fd);
    bool modify(int fd, unsigned short event);
    bool waitForEvents(unsigned int timeout);
    void processEvents(int rselect);
    bool destroy();
    
private:

    int calculateMaxFd();
    bool isFdFull();

    unsigned short *_fdEvent;
    int *_fds;
    int *_fdsIndex;
    int *_rfdsIndex;

    bool _isCalculateMaxFd;
    int _maxFd;

    fd_set _masterReadFdset;
    fd_set _masterWriteFdset;
    fd_set _workingReadFdset;
    fd_set _workingWriteFdset;

};

PollerSelect::PollerSelect(int listenFd, PollerEventHandler *handler) {
    _serverHandler.handler = handler;
    _serverHandler.listenFd = listenFd;
}

PollerSelect::~PollerSelect() {
    _serverHandler.handler = 0;
}

bool PollerSelect::isFdFull() {
    return ( _numPollerFd >= _maxPollerFd);
}

bool PollerSelect::destroy() {
    delete[] _fds;
    delete[] _fdsIndex;
    delete[] _rfdsIndex;
    delete[] _fdEvent;
    return true;
}

bool PollerSelect::init() {

    int sys_maxfd = System::maxFileDescriptor();
    if (sys_maxfd < FD_SETSIZE || sys_maxfd <= 0) {
        sys_maxfd = FD_SETSIZE;
    }

    _maxPollerFd = sys_maxfd;
    _maxSystemFd = sys_maxfd;
    _numPollerFd = 0;

    FD_ZERO(&_masterReadFdset);
    FD_ZERO(&_masterWriteFdset);

    _fds = new int[_maxPollerFd];
    _rfdsIndex = new int[_maxPollerFd];
    _fdsIndex = new int[_maxSystemFd];
    _fdEvent = new unsigned short[_maxSystemFd];

    _maxFd = -1;
    _isCalculateMaxFd = 0;

    for (int i = 0; i < _maxPollerFd; ++i) {
        _fds[i] = -1;
    }

    for (int i = 0; i < _maxSystemFd; ++i) {
        _fdsIndex[i] = -1;
        _fdEvent[i] = -1;
    }
    return true;
}

bool PollerSelect::add(int fd, unsigned short event) {

    if (isFdFull()) {
        return false;
    }

    _fds[_numPollerFd] = fd;

    if (event & POLLER_EV_READ) {
        FD_SET(fd, &_masterReadFdset);

    }
    if (event & POLLER_EV_WRITE) {
        FD_SET(fd, &_masterWriteFdset);
    }

    if (fd > _maxFd) {
        _maxFd = fd;
    }

    _fdsIndex[fd] = _numPollerFd;
    _fdEvent[fd] = event;
    _numPollerFd++;

    return true;
}

bool PollerSelect::del(int fd) {

    int idx = _fdsIndex[fd];

    if ((idx < 0) || (idx >= _maxPollerFd)) {
        return false;
    }

    _numPollerFd--;
    _fds[idx] = _fds[_numPollerFd];
    _fdsIndex[_fds[idx]] = idx;
    _fds[_numPollerFd] = -1;
    _fdsIndex[fd] = -1;
    _fdEvent[fd] = -1;

    FD_CLR(fd, &_masterReadFdset);
    FD_CLR(fd, &_masterWriteFdset);

    if (fd >= _maxFd) {
        _isCalculateMaxFd = 1;
    }

    return true;
}

bool PollerSelect::modify(int fd, unsigned short event) {
    /*
    if (fd >= _maxPollerFd_) {
        return false;
    }
    
    if (fd >= _maxFd_) {
        _isCalculateMaxFd_ = true;
    }

    if (event & EVENT_READ) {
        
        FD_CLR(fd, &_masterWriteFdset_);
        FD_SET(fd, &_masterReadFdset_);
        
    } else if(event & EVENT_WRITE) {
        
        FD_CLR(fd, &_masterReadFdset_);
        FD_SET(fd, &_masterWriteFdset_);
    }
    
    return true;
     * */

    bool ret = del(fd);

    if ((ret == false)) {
        return ret;
    }

    return add(fd, event);
}

inline int PollerSelect::calculateMaxFd() {

    if (_isCalculateMaxFd) {
        int i;
        _maxFd = -1;
        for (i = 0; i < _numPollerFd; ++i) {
            if (_fds[i] > _maxFd) {
                _maxFd = _fds[i];
            }
        }
        _isCalculateMaxFd = 0;
    }

    return _maxFd;
}

/* 
 * Reactor 's event loop 
 */
bool PollerSelect::waitForEvents(unsigned int timeout) {

    int r;
    int mfd = calculateMaxFd();
    _workingReadFdset = _masterReadFdset;
    _workingWriteFdset = _masterWriteFdset;

    if (timeout == INFINITE_TIMEOUT) {
        r = select(mfd + 1, &_workingReadFdset, &_workingWriteFdset, NULL, NULL);
    } else {
        struct timeval tv;
        tv.tv_sec = timeout / 1000L;
        tv.tv_usec = (timeout % 1000L) * 1000L;
        r = select(mfd + 1, &_workingReadFdset, &_workingWriteFdset, NULL, &tv);
    }
    
    if (r <= 0) {
        if (r == 0) {
            LOG_W("select timeout");
        } else if (r < 0) {
            LOG_E("select error(%d)", r);
        }
        return false;
    }

    processEvents(r);

    return true;
}

/* 
 * Event dispatcher 
 */
void PollerSelect::processEvents(int rselect) {
    int rindex = 0;
    int fd;
    unsigned short event;

    for (int i = 0; i < _numPollerFd; ++i) {
        event = 0;
        fd = _fds[i];

        if (_fdEvent[fd] & POLLER_EV_READ) {

            if (FD_ISSET(fd, &_workingReadFdset)) {

                if (_serverHandler.listenFd == fd) {
                    _serverHandler.handler->onPollerAccept(fd);
                } else {
                    event |= POLLER_EV_READ;
                    _serverHandler.handler->onPollerRead(fd);
                }

            }

        }
        if (_fdEvent[fd] & POLLER_EV_WRITE) {

            if (FD_ISSET(fd, &_workingWriteFdset)) {
                _serverHandler.handler->onPollerWrite(fd);
                event |= POLLER_EV_WRITE;
            }
        }

        if (event) {
            _fdsIndex[rindex++] = fd;

            if (rindex == rselect) {
                break;
            }
        }
    }
}


#endif	/* POLLERSELECT_H */
