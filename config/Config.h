//
// Created by iocout on 6/12/20.
//

#ifndef TINSERVER_CONFIG_H
#define TINSERVER_CONFIG_H

#include <iostream>

using namespace std;

struct Config {
public:
    Config();

    ~Config() {};

    void parse_arg(int argc, char *argv[]);

public:
    int PORT;

    int LogWrite;

    int TRIGMode;

    int LISTENTrigmode;

    int CONNTrigmode;
};


#endif //TINSERVER_CONFIG_H
