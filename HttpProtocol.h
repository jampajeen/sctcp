/* 
 * File:   HttpProtocol.h
 * Author: TJ
 *
 * Created on May 2, 2013, 10:13 PM
 */

#ifndef HTTPPROTOCOL_H
#define	HTTPPROTOCOL_H

#define CRLF                            "\r\n"
#define HTTP_METHOD                     "POST"
#define HTTP_PATH                       "/"
#define HTTP_VERSION                    "HTTP/1.1"
#define HTTP_REQUEST_LINE               HTTP_METHOD " " HTTP_PATH " " HTTP_VERSION
#define HTTP_HOST                       "Host: "
#define HTTP_CONTENT_LENGTH             "Content-Length: "
#define HTTP_CONTENT_TYPE               "Content-Type: "
#define HTTP_RESPONSE_STRING            HTTP_VERSION " 200 "
#define HTTP_CONTENT_TYPE_STRING	HTTP_CONTENT_TYPE "text/xml;charset=UTF8"
#define HTTP_HEADER_SEPARATOR		CRLF CRLF

#define HTTP_CHUNKED            "Transfer-Encoding: chunked"
#define HTTP_CHUNKED_TERMINATE  (CRLF "0")

#endif	/* HTTPPROTOCOL_H */

