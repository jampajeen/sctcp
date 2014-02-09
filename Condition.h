
#ifndef CONDITION_H
#define	CONDITION_H

#include "common.h"
#include <pthread.h>

class Condition {
    
public:
    Condition() {
        pthread_cond_init(& _cond, NULL);
    }

    ~Condition() {
        pthread_cond_destroy(& _cond);
    }

    int wait(Mutex & m) {
        return pthread_cond_wait(& _cond, & m._mutex);
    }

    int signal() {
        return pthread_cond_signal(& _cond);
    }

    int broadcast() {
        return pthread_cond_broadcast(& _cond);
    }

private:
    pthread_cond_t _cond;

};

#endif	/* CONDITION_H */
