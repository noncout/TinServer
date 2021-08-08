#include "Config.h"

#include <unistd.h>

Config::Config() {
    PORT = 9006;

    // 日志写入方式，默认同步
    LogWrite = 0;

    //触发组合模式,默认listenfd LT + connfd LT
    TRIGMode = 0;

    //listenfd触发模式，默认LT
    LISTENTrigmode = 0;

    //connfd触发模式，默认LT
    CONNTrigmode = 0;
}

void Config::parse_arg(int argc, char **argv) {
    int opt;
    const char *str = "p:l:t:s:c:";
    while ((opt = getopt(argc, argv, str)) != -1) {
        switch (opt) {
            case 'p': {
                PORT = atoi(optarg);
                printf("PORT: %d\n", PORT);
                break;
            }
            case 'l': {
                LogWrite = atoi(optarg);
                printf("LogWrite: %d\n", LogWrite);
                break;
            }
            case 't': {
                TRIGMode = atoi(optarg);
                printf("TRIGMode: %d\n", TRIGMode);
                break;
            }
            case 's': {
                LISTENTrigmode = atoi(optarg);
                printf("LISTENTrigmode: %d\n", LISTENTrigmode);
                break;
            }
            case 'c': {
                CONNTrigmode = atoi(optarg);
                printf("CONNTrigmode: %d\n", CONNTrigmode);
                break;
            }
            default:
                break;
        }
    }
}
