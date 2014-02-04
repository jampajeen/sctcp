/* 
 * File:   Poller.h
 * Author: Thitipong Jampajeen <jampajeen@gmail.com>
 *
 * Created on April 22, 2013, 2:35 PM
 * 
 * This library is distributed under the MIT License. See notice at the end of this file.
 * 
 */

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
