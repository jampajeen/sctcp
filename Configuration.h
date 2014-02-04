/* 
 * File:   Configuration.h
 * Author: TJ
 *
 * Created on April 25, 2013, 5:56 PM
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#include <iostream>
#include <list>
#include <string>
#include "pugixml/pugixml.hpp"
#include "Utils.h"
#include "Buffer.h"

typedef enum {
    HTTP,
    LDAP,
    DIAMETER
} ProtocolType;

class Configuration {
    
public:
    /*
     * constructor with default log path
     */
    Configuration() : _cfgPath("released/conf/load.cfg.xml"), _maxConnection(1) {}
    ~Configuration() {}
    
    /*
     * load configuration from file
     */
    bool load();
    
    std::string configPath() const;
    std::list<std::string> messages() const;
    
    /*
     * Peer info
     */
    std::string remoteIp() const;
    unsigned short remotePort() const;
    
    /*
     * Other getter method
     */
    unsigned int sendInterval() const;
    unsigned int tps() const;
    unsigned int timeout() const;
    unsigned int maxConnection() const;
    bool isSendRepeat() const;
    ProtocolType protocol() const;
    
private:
    /*
     * remote ip
     */
    std::string _remoteIp;
    
    /*
     * remote port
     */
    unsigned short _remotePort;
    
    /*
     * repeat flag
     */
    bool _isSendRepeat;
    /*
     * interval between send (msec.)
     */
    unsigned int _sendInterval;
    /*
     * desired TPS
     */
    unsigned int _tps;
    /*
     * Protocol
     */
    ProtocolType _protocol;
    
    /*
     * configuration path
     */
    std::string _cfgPath;
    
    /*
     * message list in payload file
     */
    std::list<std::string> _messages;
    
    /*
     * timeout (sec.)
     */
    unsigned int _timeout;
    
    unsigned int _maxConnection;
};

bool Configuration::load() {
    LOG_I("Start loading configuration");
    const char *res = NULL;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(_cfgPath.c_str());

    if (result) {
        /*
         * RemoteAddress
         */
        LOG_I("Loading [RemoteAddress] ...");
        res = doc.child("Configuration").child("Service").child("RemoteAddress").child_value();
        if(res) {
            _remoteIp = res;
        } else {
            LOG_E("Error [RemoteAddress]");
            return false;
        }
        
        /*
         * Remote port
         */
        LOG_I("Loading [RemoteAddress] ...");
        res = doc.child("Configuration").child("Service").child("RemotePort").child_value();
        if(res) {
            _remotePort = Utils::toInt(res);
        } else {
            LOG_E("Error [RemotePort]");
            return false;
        }
        
        /*
         * MaxConnection
         */
        LOG_I("Loading [MaxConnection] ...");
        res = doc.child("Configuration").child("Profile").child("MaxConnection").child_value();
        if(res) {
            _maxConnection = Utils::toInt(res);
        } else {
            LOG_E("Error [MaxConnection]");
            return false;
        }
        
        /*
         * repeat
         */
        LOG_I("Loading [SendRepeat] ...");
        res = doc.child("Configuration").child("Profile").child("SendRepeat").child_value();
        if(res) {
            _isSendRepeat = ( Utils::strCaseCmp(res, "true") ) == 0 ? true : false;
        } else {
            LOG_E("Error [SendRepeat]");
            return false;
        }
        
        /*
         * SendInterval
         */
        LOG_I("Loading [SendInterval] ...");
        res = doc.child("Configuration").child("Profile").child("SendInterval").child_value();
        if(res) {
            _sendInterval = Utils::toInt(res);
        } else {
            LOG_E("Error [SendInterval]");
            return false;
        }
        
        /*
         * Tps
         */
        LOG_I("Loading [Tps] ...");
        res = doc.child("Configuration").child("Profile").child("Tps").child_value();
        if(res) {
            _tps = Utils::toInt(res);
        } else {
            LOG_E("Error [Tps]");
            return false;
        }
        
        /*
         * Protocol
         */
        LOG_I("Loading [Protocol] ...");
        res = doc.child("Configuration").child("Profile").child("Protocol").child_value();
        if(res) {
            if( Utils::strCaseCmp(res, "HTTP") == 0 ) {
                _protocol = HTTP;
            }
            else if( Utils::strCaseCmp(res, "LDAP") == 0 ) {
                _protocol = LDAP;
            }
            else if( Utils::strCaseCmp(res, "DIAMETER") == 0 ) {
                _protocol = DIAMETER;
            }
        } else {
            LOG_E("Error [Protocol]");
            return false;
        }
        
        
        //std::cout << "Load result: "  << " VirtualAddress: " << doc.child("Configuration").child("Profile").child("VirtualAddress").child_value() << std::endl;
        //std::cout << "Load result: "  << " Service type: " << doc.child("Configuration").child("Service").attribute("type").value() << std::endl;
        /*
         * payload file
         */
        LOG_I("Loading [Message] ...");
        pugi::xml_node nodes = doc.child("Configuration");
        if(nodes) {
            for (pugi::xml_node node = nodes.child("Message"); node; node = node.next_sibling("Message")) {
                
                _messages.push_back( node.child_value() );
            }
        } else {
            LOG_E("Error [Message]");
            return false;
        }
    }
    else {
        LOG_E("Error description: %s", result.description());
        return false;
    }
    LOG_I("Load configuration completed");
    
    return true;
}

std::string Configuration::remoteIp() const {
    return _remoteIp;
}

unsigned short Configuration::remotePort() const {
    return _remotePort;
}

bool Configuration::isSendRepeat() const {
    return _isSendRepeat;
}

unsigned int Configuration::sendInterval() const {
    return _sendInterval;
}

unsigned int Configuration::tps() const {
    return _tps;
}

ProtocolType Configuration::protocol() const {
    return _protocol;
}

std::string Configuration::configPath() const {
    return _cfgPath;
}

unsigned int Configuration::timeout() const {
    return _timeout;
}

std::list<std::string> Configuration::messages() const {
    return _messages;
}

unsigned int Configuration::maxConnection() const {
    return _maxConnection;
}

#endif	/* CONFIGURATION_H */

