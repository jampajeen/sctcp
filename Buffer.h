/*
 * File:   Buffer.h
 * Author: Thitipong Jampajeen <jampajeen@gmail.com>
 *
 * Created on April 1, 2013, 3:30 AM
 * 
 * This library is distributed under the MIT License. See notice at the end of this file.
 * 
 */

#ifndef BUFFER_H
#define	BUFFER_H

#include "common.h"
#include "Log.h"
#include <string.h>
#include <string>

class Buffer {
    
public:
    /* 
     * with default size 
     */
    Buffer();
    
    /* 
     * with specify size
     */
    Buffer(int size);
    ~Buffer();

    /* 
     * get actual data length in buffer
     */
    int length() const;
    
    /* 
     * get size of buffer
     */
    int size() const;
    
    /* 
     * get buffer as pointer 
     */
    char *getChars() const;
    
    /* 
     * get buffer as std::string
     */
    std::string getString() const;
    
    /*
     * buffer manipulation
     */
    bool adjustSize(int newSize);
    bool append(const char *buf, int buflen);
    bool clear();

private:
    /* 
     * actual buffer 
     */
    char *_buffer;
    
    /* 
     * data length in buffer
     */
    int _length;
    
    /* 
     * size of block 
     */
    int _size;
    
    static const int DEFAULT_SIZE = (64 * 1024);
    static const int MAX_SIZE = (1024 * 1024);
    static const bool AUTO_ADJUST = true;
};


Buffer::Buffer() : _buffer(NULL), _length(0), _size(DEFAULT_SIZE) {
#ifdef CPP_VER_11
    Buffer(_size);
#else
    _buffer = (char*) malloc(_size);

    if (_buffer == NULL) {
        LOG_F("Error allocating memory");
        exit(1);
    }
#endif
}

inline Buffer::Buffer(int size) : _buffer(NULL), _length(0), _size(size) {
    _buffer = (char*) malloc(_size);

    if (_buffer == NULL) {
        LOG_F("Error allocating memory");
        exit(1);
    }
}

Buffer::~Buffer() {
    delete _buffer;
}

inline int Buffer::length() const {
    return _length;
}

inline int Buffer::size() const {
    return _size;
}

inline char *Buffer::getChars() const {
    return _buffer;
}

inline std::string Buffer::getString() const {
    return _buffer;
}

inline bool Buffer::adjustSize(int newSize) {
    char *nbuf = (char*) realloc(_buffer, newSize * sizeof (char));
    if (nbuf == NULL) {
        return false;
    }

    _buffer = nbuf;
    _size = newSize;

    return true;
}

inline bool Buffer::append(const char *buf, int buflen) {
    int nSize = (_length + buflen);
    if (nSize >= _size) {
        if (AUTO_ADJUST && nSize < MAX_SIZE) {
            if (adjustSize(nSize) == false) {
                LOG_E("Can not adjust memory block");
                return false;
            }
        } else {
            LOG_E("Auto adjust flags was not set (OR) exceed max size limitation");
            return false;
        }
    }

    memcpy(_buffer + _length, buf, buflen);
    _length += buflen;
    _buffer[_length] = '\0';

    return true;
}

inline bool Buffer::clear() {
    memset(_buffer, 0, _size);
    _length = 0;
    return true;
}

#endif  /* BUFFER_H */

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
