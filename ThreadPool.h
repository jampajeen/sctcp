
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
