
#ifndef SYSTEM_H
#define	SYSTEM_H

#include <sys/wait.h>
#include <sys/stat.h>

class System {
public:
    /*
     * Get max fd in system
     */
    static  int maxFileDescriptor();
};

int System::maxFileDescriptor() {
    
    struct rlimit limit;
    if (getrlimit(RLIMIT_NOFILE, &limit) < 0) {
        return 0;
    }
    
    return limit.rlim_max;
}

#endif	/* SYSTEM_H */
