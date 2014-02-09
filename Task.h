
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
