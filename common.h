
#ifndef COMMON_H
#define	COMMON_H

#if (__cplusplus > 199711L)
#define CPP_VER_11
#endif

#define CPP_BEGIN_DECLARE       extern "C" {
#define CPP_END_DECLARE         }

#define HAVE_SELECT             0x00000001

#if defined(__linux__) && defined(UnderTestingWithSelect)
#define HAVE_EPOLL              0x00000004
#endif

#if defined(__sun) && defined(__SVR4)
#define HAVE_PORT               0x00000008
#endif

#if defined(__WIN32) || defined(__WIN64)
#define HAVE_IOCP               0x00000100
#endif

#if (defined(__unix__) && !defined(__linux__)) || (defined(__APPLE__) && defined(__MACH__))
#define HAVE_KQUEUE             0x00010000
#endif

#define POLLER_EV_NONE          0x0000
#define POLLER_EV_READ          0x0001
#define POLLER_EV_WRITE         0x0002

#define INFINITE_TIMEOUT        0
#define NO_LISTEN_FD            0
#define NOT_AVAILABLE           0

#define TASK_EV_NONE                    0x0000
#define TASK_EV_PARSE_RCTCP_HEADER      0x0001
#define TASK_EV_PROCESS_PING_PONG       0x0002
#define TASK_EV_PARSE_SCTCP_HEADER      0x0004
#define TASK_EV_PROCESS_SCTCP_DATA      0x0008
#define TASK_EV_CONSTRUCT_PING_PONG     0x0010
#define TASK_EV_CONSTRUCT_SCTCP         0x0020
#define TASK_EV_CONSTRUCT_RCTCP         0x0040
#define TASK_EV_COMPLETED               0x0080

#define MAX_BUF_SIZE    (1024 * 64)
#define MAX_CONNECTION  (1024 * 64)

#endif	/* COMMON_H */
