
#ifndef PROTOCOL_H
#define	PROTOCOL_H

#include "common.h"

/*
 * 2 bytes length
 */
typedef struct Length_S {
    char byte0;
    char byte1;
} Length;

/*
 * 4 bytes source address
 */
typedef struct SrcAddr_S {
    char byte0;
    char byte1;
    char byte2;
    char byte3;
}SrcAddr;

/*
 * 4 bytes destination address
 */
typedef struct DstAddr_S {
    char byte0;
    char byte1;
    char byte2;
    char byte3;
}DstAddr;

/*
 * 4 bytes transmission sequence number
 */
typedef struct Tsn_S {
    char byte0;
    char byte1;
    char byte2;
    char byte3;
}Tsn;

/*
 * 2 bytes stream identifier
 */
typedef struct Sid_S {
    char byte0;
    char byte1;
}Sid;

/*
 * 2 bytes stream sequence number
 */
typedef struct Ssn_S {
    char byte0;
    char byte1;
}Ssn;


/*
 * SCTCP Header
 */

typedef enum {
    SCTCP_H_CHUNK_FALSE = 0x00,
    SCTCP_H_CHUNK_TRUE = 0x01
} ScTcpChunkType;

#define SCTCP_H_RESERVED        0x00

typedef struct ScTcpHeader_S {
    unsigned char chunkType;
    unsigned char reserved;
    Length length;
    Tsn tsn;
    Sid sid;
    Ssn ssn;
} ScTcpHeader;

/*
 * RCTCP Header
 */
typedef struct RcTcpHeader_S {
    unsigned char signature;
    unsigned char flag;
    Length length;
    SrcAddr srcAddr;
    DstAddr dstAddr;
} RcTcpHeader;

#define RCTCP_PACKET_SIZE               (64 * 1024)
#define RCTCP_PAYLOAD_SIZE              (RCTCP_PACKET_SIZE - sizeof(RcTcpHeader))

/*
 * RCTCP Signature
 */
#define RCTCP_H_SIGNATURE               0xF7

/*
 * RCTCP Command Flags
 */
typedef enum {
    RCTCP_H_FLAG_PING_FROM_HOST = 0x01,
    RCTCP_H_FLAG_PONG_TO_HOST = 0x02,
    RCTCP_H_FLAG_PING_FROM_OTHERS = 0x03,
    RCTCP_H_FLAG_PONG_TO_OTHERS = 0x04,
    RCTCP_H_FLAG_SCTCP = 0x0A
} RcTcpFlag;

#define SCTCP_BYTE_PADDING      4
#define SCTCP_HEADER_SIZE       sizeof(ScTcpHeader)
#define SCTCP_DATA_SIZE         ( RCTCP_PAYLOAD_SIZE - SCTCP_HEADER_SIZE - SCTCP_BYTE_PADDING )

/*
 * SCTCP packet structure
 */
typedef struct ScTcp_S {
    ScTcpHeader header;
    char data[SCTCP_DATA_SIZE];
} ScTcp;


/*
 * RCTCP Payload
 */
typedef struct PingFromHost_S {
    unsigned long long reserved;
} PingFromHost;

typedef struct PongToHost_S {
    unsigned long long reserved;
} PongToHost;

typedef struct PingFromOther_S {
    unsigned long long reserved;
} PingFromOther;

typedef struct PongToOther_S {
    unsigned long long reserved;
} PongToOther;

typedef union {
    PingFromHost pingFromHost;
    PongToHost pongToHost;
    PingFromOther pingFromOther;
    PongToOther pongToOther;
    ScTcp sctcp;
} RcTcpPayload;

/*
 * RCTCP Packet structure
 */
typedef struct RcTcp_S {
    RcTcpHeader header;
    RcTcpPayload payload;
} RcTcp;


class Protocol {
public:
    /*
     * Useful method
     */
    static unsigned int toInt32(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3);
    static unsigned short toInt16(unsigned char b0, unsigned char b1);
    static unsigned int fromInt32(unsigned int val, char out[4]);
    static unsigned short fromInt16(unsigned short val, char out[4]);
    /*
     * Construct RCTCP
     */
    static unsigned int buildRcTcp(RcTcpFlag flag, int src, int dst,const RcTcpPayload *payload, RcTcp *rctcpOut, int rctcpOutLen);
    /*
     * Construct SCTCP
     */
    static unsigned int buildScTcp(ScTcpChunkType type, int tsn, short sid, short ssn, const char *data, int datalen, ScTcp *sctcpOut, int sctcpOutLen);

};


inline unsigned int Protocol::toInt32(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3) {
    return ( (b0 << 24) | (b1 << 16) | (b2 << 8) | (b3 & 0xFF));
}

inline unsigned short Protocol::toInt16(unsigned char b0, unsigned char b1) {
    return ( (b0 << 8) | (b1 & 0xFF));
}

unsigned int buildRcTcp(RcTcpFlag flag, int src, int dst, const RcTcpPayload *payload, RcTcp *rctcpOut, int rctcpOutLen) {
    
    RcTcp rctcp;
    
    /* packet length */
    unsigned int rctcpLength = (sizeof(RcTcp) + sizeof(payload));
    if(rctcpLength > rctcpOutLen) {
        LOG_E("insufficient output buffer");
        return 0;
    }
    rctcp.header.length.byte0 = rctcpLength & 0xFF;
    rctcp.header.length.byte1 = rctcpLength >> 8;
    
    /* Signature */
    rctcp.header.signature = RCTCP_H_SIGNATURE;
    
    /* flag */
    rctcp.header.flag = flag;
    
    /* Source address */
    rctcp.header.srcAddr.byte0 = src & 0xFF;
    rctcp.header.srcAddr.byte1 = src >> 8;
    rctcp.header.srcAddr.byte2 = src >> 16;
    rctcp.header.srcAddr.byte3 = src >> 24;
    
    /* destination address */
    rctcp.header.dstAddr.byte0 = dst & 0xFF;
    rctcp.header.dstAddr.byte1 = dst >> 8;
    rctcp.header.dstAddr.byte2 = dst >> 16;
    rctcp.header.dstAddr.byte3 = dst >> 24;
    
    /* construct payload */
    memcpy(&rctcp.payload, payload, sizeof(payload));

    /* copy to output buffer*/
    memcpy(rctcpOut, &rctcp, sizeof(RcTcp));
    
    return rctcpLength;
}

unsigned int buildScTcp(ScTcpChunkType type, int tsn, short sid, short ssn, const char *data, int datalen, ScTcp *sctcpOut, int sctcpOutLen) {
    
    ScTcp sctcp;
    
    /* ScTcp packet length */
    unsigned int sctcpLength = (sizeof(ScTcp) + datalen);
    if(sctcpLength > sctcpOutLen) {
        LOG_E("insufficient output buffer");
        return 0;
    }
    sctcp.header.length.byte0 = sctcpLength & 0xFF;
    sctcp.header.length.byte1 = sctcpLength >> 8;

    /* Chunk Type */
    sctcp.header.chunkType = type;
    
    /* Reserved */
    sctcp.header.reserved = SCTCP_H_RESERVED;
    
    /* TSN */
    sctcp.header.tsn.byte0 = tsn & 0xFF;
    sctcp.header.tsn.byte1 = tsn >> 8;
    sctcp.header.tsn.byte2 = tsn >> 16;
    sctcp.header.tsn.byte3 = tsn >> 24;
    
    /* SID */
    sctcp.header.sid.byte0 = sid & 0xFF;
    sctcp.header.sid.byte1 = sid >> 8;
    
    /* SSN */
    sctcp.header.ssn.byte0 = ssn & 0xFF;
    sctcp.header.ssn.byte0 = ssn >> 8;
    
    /* construct data */
    memcpy(&sctcp.data, data, datalen);
    
    /* Copy to output buffer*/
    memcpy(sctcpOut, &sctcp, sizeof(ScTcp));
    
    return sctcpLength;
}

#endif	/* PROTOCOL_H */
