/* 
 * File:   Mutex.h
 * Author: Thitipong Jampajeen <jampajeen@gmail.com>
 *
 * Created on April 22, 2013, 2:56 PM
 * 
 * This library is distributed under the MIT License. See notice at the end of this file.
 * 
 */

#ifndef MUTEX_H
#define	MUTEX_H

#include "common.h"
#include <pthread.h>

class Mutex {
    friend class Condition;
public:

    Mutex() {
        pthread_mutexattr_init(& _mutexAttr);
        pthread_mutex_init(& _mutex, & _mutexAttr);
    }

    ~Mutex() {
        pthread_mutex_destroy(& _mutex);
        pthread_mutexattr_destroy(& _mutexAttr);
    }

    int trylock() {
        return pthread_mutex_trylock(& _mutex);
    }
    
    int lock() {
        return pthread_mutex_lock(& _mutex);
    }

    int unlock() {
        return pthread_mutex_unlock(& _mutex);
    }

private:
    pthread_mutex_t _mutex;
    pthread_mutexattr_t _mutexAttr;
};

#endif	/* MUTEX_H */

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
