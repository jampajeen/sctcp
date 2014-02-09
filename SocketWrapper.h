
#ifndef SOCKETWRAPPER_H
#define	SOCKETWRAPPER_H

#include "common.h"
#include "Log.h"

#include <sys/socket.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

class SocketWrapper {
public:
    /*
     * Socket options
     */
    static bool setOptionRcvbufSize(int fd, unsigned int size);
    static bool setOptionSndbufSize(int fd, unsigned int size);
    static bool setOptionReuseaddr(int fd);
    static bool setOptionNonblock(int fd, bool enable);
    static bool setOptionNodelay(int fd, bool enable);

    /*
     * Server operation
     */
    static int bindAndListen(unsigned int port);
    static int accept(int fd);

    /*
     * Client operation
     */
    static int connect(int sock, const char *host, unsigned short port);

    /*
     * Generic socket operation
     */
    static int socket();
    static bool close(int fd);
    static bool shutdown(int fd);
    static int recv(int fd,char *buf,unsigned int n,int flags);
    static int send(int fd,char *buf,unsigned int n,int flags);
    
    /*
     * getpeername
     */
    static bool getPeerName(int fd, char *ipOut, int ipOutSize, unsigned short *portOut);

};


bool SocketWrapper::setOptionRcvbufSize(int fd, unsigned int size) {
    unsigned int s = size;
    setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char *) &s, (int) sizeof (s));
    return true;
}

bool SocketWrapper::setOptionSndbufSize(int fd, unsigned int size) {
    unsigned int s = size;
    setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *) &s, (int) sizeof (s));
    return true;
}

bool SocketWrapper::setOptionReuseaddr(int fd) {
    int on = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof (on)) < 0) {
        return false;
    }
    return true;
}

bool SocketWrapper::setOptionNonblock(int fd, bool enable) {
    int re;
    int flags = 0;
#ifdef HAVE_IOCP
    flags = enable;
    re = ioctlsocket(sock->fd, FIONBIO, (u_long *) & flags);
#elif  defined(O_NONBLOCK)
    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return false;
    }
    if (enable)
        flags |= O_NONBLOCK;
    else
        flags &= !(O_NONBLOCK);
    re = fcntl(fd, F_SETFL, flags);
#else
    flags = enable;
    re = ioctl(sock->fd, FIOBIO, &flags);
#endif
    if (re < 0) {
        return false;
    }
    return true;
}

bool SocketWrapper::setOptionNodelay(int fd, bool enable) {
    int re;
    int flags = 0;
#ifdef HAVE_IOCP
    flags = enable;
    re = ioctlsocket(sock->fd, FIONBIO, (u_long) & flags);
#elif defined(O_NONBLOCK)
    flags = fcntl(fd, F_GETFL, 0);
    if ((flags == -1)) {
        return false;
    }
    if (enable)
        flags |= O_NDELAY;
    else
        flags &= !(O_NDELAY);
    re = fcntl(fd, F_SETFL, flags);
#else
    flags = enable;
    re = ioctl(sock->fd, FIONBIO, &flags);
#endif
    if (re < 0) {
        return false;
    }
    return true;
}

int SocketWrapper::bindAndListen(unsigned int port) {
    int fd;
    if ((fd = ::socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    /* Set socket Options */
    setOptionNonblock(fd, true);
    setOptionReuseaddr(fd);
    setOptionRcvbufSize(fd, 65535);
    setOptionSndbufSize(fd, 65535);

    /* Set Address support IPv6 */
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof (addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(port);
    addr.sin6_addr = in6addr_any;

    if (bind(fd, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
        return -1;
    }

    if (listen(fd, 10) < 0) {
        return -1;
    }

    return fd;
}

inline int SocketWrapper::accept(int fd) {
    /*int rfd = -1;
    do {
        rfd = ::accept(fd, NULL, NULL);
        if (rfd < 0) {
            if (errno != EWOULDBLOCK) {
                LOG_E("accept() failed");
            }
            break;
        }
    } while (rfd != -1);*/
    return ::accept(fd, NULL, NULL);
}

inline bool SocketWrapper::close(int sd) {
    if (::close(sd) < 0) {
        return false;
    }
    return true;
}

/*
 * SHUT_RD   = No more receptions;
 * SHUT_WR   = No more transmissions;
 * SHUT_RDWR = No more receptions or transmissions.
 */
inline bool SocketWrapper::shutdown(int fd) {
    if (::shutdown(fd, SHUT_WR) < 0) {
        return false;
    }
    return true;
}

inline int SocketWrapper::recv(int fd, char *buf,unsigned int n,int flags) {
    return ::recv(fd,buf,n,flags);
}

inline int SocketWrapper::send(int fd, char *buf,unsigned int n,int flags) {
    return ::send(fd,buf,n,flags);
}

inline bool SocketWrapper::getPeerName(int fd, char *ipOut, int ipOutSize, unsigned short *portOut) {
    
    if(ipOutSize < INET6_ADDRSTRLEN) {
        LOG_E("ipOutSize buffer size must be equal INET6_ADDRSTRLEN");
        return false;
    }
    
    sockaddr_storage addr;
    socklen_t len = sizeof(addr);
    
    ::getpeername(fd, (sockaddr*) &addr, &len);
    
    if (addr.ss_family == AF_INET) {
        /* IPv4 */
        sockaddr_in *s = (sockaddr_in *)&addr;
        *portOut = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, ipOut, INET6_ADDRSTRLEN);
        
    } else {
        /* IPv6 */
        sockaddr_in6 *s = (sockaddr_in6 *)&addr;
        *portOut = ntohs(s->sin6_port);
        inet_ntop(AF_INET6, &s->sin6_addr, ipOut, INET6_ADDRSTRLEN);
    }
    
    return true;
}

inline int SocketWrapper::socket() {
    return ::socket(AF_INET, SOCK_STREAM, 0);
}

inline int SocketWrapper::connect(int sock, const char *host, unsigned short port) {
    
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof (servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(host);
    servAddr.sin_port = htons(port);
    
    return ::connect(sock, (sockaddr *) &servAddr, sizeof(servAddr));
}

#endif	/* SOCKETWRAPPER_H */
