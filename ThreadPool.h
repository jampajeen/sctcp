/* 
 * File:   ThreadPool.h
 * Author: Thitipong Jampajeen <jampajeen@gmail.com>
 *
 * Created on April 22, 2013, 2:58 PM
 * 
 * This library is distributed under the MIT License. See notice at the end of this file.
 * 
 */

#ifndef THREADPOOL_H
#define	THREADPOOL_H

#include "common.h"
#include "Mutex.h"
#include "Condition.h"
#include "Task.h"

#include <queue>

class ThreadPool {

private:
    /*
     * start dispatching
     */
    static void *startDispatch(void *arg);
    /*
     * real job dispatcher
     */
    void dispatch();

public:
    ThreadPool(int numThread = 1, int queueSize = 0);
    ~ThreadPool();

    /*
     * Add task to thread pool, then store in queue
     */
    int addTask(void (*handler)(void *arg), void *arg);

private:
    /*
     * closing & destroying flags
     */
    bool _closing;
    bool _destroying;
    
    /*
     * Number of worker threads in pool
     */
    int _numThread;
    
    /*
     * Queue which is used to store tasks.
     */
    int _queueSize;
    std::queue<Task *> _tasks;

    /*
     * pthread_t of each threads in pool
     */
    pthread_t **_thThread;
    
    /*
     * Mutex lock
     */
    Mutex _taskMutex;

    /*
     * thread condition
     */
    Condition _condTaskEmpty;
    Condition _condTaskNotEmpty;
    Condition _condTaskNotFull;
};


#endif	/* THREADPOOL_H */

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
