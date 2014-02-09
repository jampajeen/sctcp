
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
