
#ifndef LOG_H
#define	LOG_H

#include "common.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

typedef enum LogLevel_S {
    LOG_DEBUG = 0x01,
    LOG_INFO = 0x02,
    LOG_WARN = 0x04,
    LOG_ERROR = 0x08,
    LOG_FATAL = 0x10
} LogLevel;

#define LOG_D(...) Log::logGen("DEBUG", __FILE__, __LINE__,__func__, __VA_ARGS__ );
#define LOG_I(...) Log::logGen("INFO", __FILE__, __LINE__,__func__, __VA_ARGS__ );
#define LOG_W(...) Log::logGen("WARN", __FILE__, __LINE__,__func__, __VA_ARGS__ );
#define LOG_E(...) Log::logGen("ERROR", __FILE__, __LINE__,__func__, __VA_ARGS__ );
#define LOG_F(...) Log::logGen("FATAL", __FILE__, __LINE__,__func__, __VA_ARGS__ );

class Log {

public:
    static void setLevel(LogLevel level);
    static void logGen(const char *logtype, const char *file, int line, const char* func, const char *fmt, ...);
    
private:
    static void logWrite(char *msg);
    
    static const int MAX_LOG_LENGTH =  (64 * 1024);
    static unsigned short level;
};


inline void Log::logWrite(char *msg) {
    printf(msg);
}

inline void Log::logGen(const char *logtype, const char *file, int line, const char* func, const char *fmt, ...) {
    char msg[MAX_LOG_LENGTH];
    char buf[MAX_LOG_LENGTH];

    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_LOG_LENGTH - 1, fmt, args);
    va_end(args);

#ifndef THREADED
    sprintf(buf, "%s|%s|%d|%s|%s|%d|%s\n", logtype, __TIME__, getpid(), file, func, line, msg);
#else
    sprintf(buf, "%s|%s|%d|%d|%s|%s|%d|%s\n", logtype, __TIME__, getpid(), (unsigned long int) pthread_self(), file, func, line, msg);
#endif
    logWrite(buf);
}

#endif	/* LOG_H */
