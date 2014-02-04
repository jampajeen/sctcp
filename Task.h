/* 
 * File:   Task.h
 * Author: Thitipong Jampajeen <jampajeen@gmail.com>
 *
 * Created on April 22, 2013, 2:58 PM
 * 
 * This library is distributed under the MIT License. See notice at the end of this file.
 * 
 */

#ifndef TASK_H
#define	TASK_H

#include "common.h"

class Task {
    friend class ThreadPool;
    
private:
    /*
     * Constructor, parameter are the handler 's function pointer and itself argument
     */
    Task(void (*hdr)(void *arg), void *a) : handler(hdr), arg(a) { }
    
    /*
     * handler
     */
    void (*handler)(void *arg);
    /*
     * args of handler
     */
    void *arg;
    /*
     * call handler
     */
    void run() { handler(arg); }
};

#endif	/* TASK_H */

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
