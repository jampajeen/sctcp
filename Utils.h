/* 
 * File:   Utils.h
 * Author: TJ
 *
 * Created on April 25, 2013, 2:33 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <stdlib.h>
#include <sys/time.h>


class Utils {
    
public:
    static time_t microsecondsTimeDiff (struct timeval * t2, struct timeval * t1);
    static timeval timevalNow();
    static int toInt(const char *str);
    static int strCaseCmp(const char *s1, const char *s2);
};

inline timeval Utils::timevalNow() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv;
}

inline int Utils::toInt(const char *str) {
    return strtol(str, NULL, 10);
}

inline int Utils::strCaseCmp(const char *s1, const char *s2) {
    //return strcasecmp(s1, s2);
}

inline time_t Utils::microsecondsTimeDiff(struct timeval * t2, struct timeval * t1) {
    time_t sec = t2 -> tv_sec - t1 -> tv_sec;
    time_t usec = t2 -> tv_usec - t1 -> tv_usec;

    if (usec < 0) {
        usec += 1000000;
        --sec;
    }
    return (sec * 1000000) +usec;
}

#endif	/* UTILS_H */

