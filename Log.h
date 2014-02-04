/* 
 * File:   Log.h
 * Author: Thitipong Jampajeen <jampajeen@gmail.com>
 *
 * Created on March 3, 2013, 4:55 AM
 * 
 * This library is distributed under the MIT License. See notice at the end of this file.
 * 
 */

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
