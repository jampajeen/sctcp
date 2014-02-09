
#ifndef POLLEREPOLL_H
#define	POLLEREPOLL_H

#include <sys/epoll.h>
#include "Poller.h"
#include "Log.h"
#include "System.h"

class PollerEpoll : public Poller {
public:
    PollerEpoll(int listenFd, PollerEventHandler *handler);
    virtual ~PollerEpoll();

    bool init();
    bool add(int fd, unsigned short event);
    bool del(int fd);
    bool modify(int fd, unsigned short event);
    bool waitForEvents(unsigned int timeout);
    void processEvents(int rselect);
    bool destroy();

private:
    bool isFdFull();

    int _fd;
    struct epoll_event *_epollEvents;
    int *_fdsIndex;
};

PollerEpoll::PollerEpoll(int listenFd, PollerEventHandler *handler) {
    _serverHandler.handler = handler;
    _serverHandler.listenFd = listenFd;
}

PollerEpoll::~PollerEpoll() {
    _serverHandler.handler = 0;
}

bool PollerEpoll::isFdFull() {
    return ( _numPollerFd >= _maxPollerFd);
}

bool PollerEpoll::init() {

    int sys_maxfd = System::maxFileDescriptor();
    if (sys_maxfd < FD_SETSIZE || sys_maxfd <= 0) {
        sys_maxfd = FD_SETSIZE;
    }

    _maxPollerFd = sys_maxfd;
    _maxSystemFd = sys_maxfd;
    _numPollerFd = 0;

    _fd = -1;
    _epollEvents = new epoll_event[_maxPollerFd];
    _fdsIndex = new int[_maxSystemFd];

    if (_epollEvents == NULL || _fdsIndex == NULL) {
        destroy();
        return false;
    }

    for (int re = 0; re < _maxSystemFd; re++) {
        _fdsIndex[re] = -1;
    }

    _fd = epoll_create(_maxPollerFd);
    if (_fd < 0) {
        destroy();
        return false;
    }

    int re = fcntl(_fd, F_SETFD, FD_CLOEXEC);
    if (re < 0) {
        destroy();
        return false;
    }

    return true;
}

bool PollerEpoll::add(int fd, unsigned short event) {
    struct epoll_event ev;

    if (isFdFull()) {
        return false;
    }

    ev.data.u64 = 0;
    ev.data.fd = fd;
    switch (event) {
        case POLLER_EV_READ:
            ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
            break;
        case POLLER_EV_WRITE:
            ev.events = EPOLLOUT | EPOLLERR | EPOLLHUP;
            break;
        default:
            ev.events = 0;
            return false;
    }

    if (epoll_ctl(_fd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        return false;
    }

    _numPollerFd++;

    return true;
}

bool PollerEpoll::del(int fd) {
    struct epoll_event ev;

    ev.events = 0;
    ev.data.u64 = 0;
    ev.data.fd = fd;

    int idx = _fdsIndex[fd];

    if ((idx < 0) || (idx >= _maxPollerFd)) {
        return false;
    }

    if (epoll_ctl(_fd, EPOLL_CTL_DEL, fd, &ev) < 0) {
        return false;
    }

    _numPollerFd--;
    return true;
}

bool PollerEpoll::modify(int fd, unsigned short event) {
    struct epoll_event ev;

    ev.data.u64 = 0;
    ev.data.fd = fd;

    switch (event) {
        case POLLER_EV_READ:
            ev.events = EPOLLIN;
            break;
        case POLLER_EV_WRITE:
            ev.events = EPOLLOUT;
            break;
        default:
            ev.events = 0;
            return false;
    }

    if (epoll_ctl(_fd, EPOLL_CTL_MOD, fd, &ev) < 0) {
        return false;
    }

    return true;
}

bool PollerEpoll::waitForEvents(unsigned int timeout) {

    int r = epoll_wait(_fd, _epollEvents, _maxPollerFd, timeout);
    if (r < 1)
        return false;

    processEvents(r);

    return true;
}

void PollerEpoll::processEvents(int numEvents) {
    int rindex;
    uint32_t events;
    int fd;

    for (int i = 0; i < numEvents; ++i) {

        _fdsIndex[_epollEvents[i].data.fd] = i;

        fd = _epollEvents[i].data.fd;
        if (fd < 0 || fd >= _maxSystemFd)
            break;

        rindex = _fdsIndex[fd];
        if (rindex < 0 || rindex >= numEvents)
            continue;

        if (rindex >= _maxPollerFd) {
            break;
        }

        if (_epollEvents[rindex].data.fd != fd)
            continue;

        events = _epollEvents[rindex].events;

        switch (events) {
            case POLLER_EV_READ:
                events = events & (EPOLLIN | EPOLLERR | EPOLLHUP);
            case POLLER_EV_WRITE:
                events = events & (EPOLLOUT | EPOLLERR | EPOLLHUP);
            default:
                break;
        }

    }
}

bool PollerEpoll::destroy() {

    if (_fd >= 0)
        close(_fd);

    delete (_epollEvents);
    delete (_fdsIndex);

    return true;
}

#endif	/* POLLEREPOLL_H */
