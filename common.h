/* 
 * File:   common.h
 * Author: Thitipong Jampajeen <jampajeen@gmail.com>
 *
 * Created on March 3, 2013, 5:00 AM
 * 
 * This library is distributed under the MIT License. See notice at the end of this file.
 * 
 */

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

/* For Equinox Project Only */
#define TASK_EV_BEFORE_SEND     0x0100
#define TASK_EV_AFTER_SENT      0x0200

#define MAX_BUF_SIZE    (1024 * 64)
#define MAX_CONNECTION  (1024 * 64)

#endif	/* COMMON_H */

/*
 * Copyright (c) 2013-2018 Thitipong Jampajeen <jampajeen@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
