/* 
 * File:   main.cpp
 * Author: Thitipong Jampajeen <jampajeen@gmail.com>
 *
 * Created on April 13, 2013, 2:00 PM
 */

#include <cstdlib>
#include <iostream>
#include "common.h"
#include "Log.h"
#include "Configuration.h"
#include "Server.h"

#include "pugixml/pugixml.hpp"

int main(int argc, char** argv) {
    LOG_I("Welcome to SCTCP development!");
    if(strcmp(argv[1], "server") == 0) {
        LOG_I("Starting server...");
        Server serv;
        serv.start();
    }
    return 0;
}

