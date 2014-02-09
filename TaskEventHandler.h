
#ifndef TASKEVENTHANDLER_H
#define	TASKEVENTHANDLER_H

class TaskEventHandler {
public:
    /*virtual void onParseRctcpHeader() = 0;
    virtual void onProcessPingPong() = 0;
    virtual void onParseSctcpHeader() = 0;
    virtual void onProcessSctcpData() = 0;
    virtual void onConstructPingPong() = 0;
    virtual void onConstructSctcp() = 0;
    virtual void onConstructSctcp() = 0;
    virtual void onConstructRctcp() = 0;
    virtual void onCompleted() = 0;
    */
    /*
     * For Equinox Project Only
     */
    virtual void onBeforeSend() = 0;
    virtual void onAfterSent() = 0;
    virtual void onAfterRecieved() = 0;
};

#endif	/* TASKEVENTHANDLER_H */
