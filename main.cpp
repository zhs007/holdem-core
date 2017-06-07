#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include "src/cmd.h"

const char* CMD[] = {
        "begin 1 2 4 0 7 5",
        "spi 0 dpq simons 1",
        "st",
        "shc 0 37 38",
        "bet 1 -1 0",
        "bet 2 -1 0",
        "bet 3 16 0",
        "bet 4 16 0",
        "bet 5 16 0",
        "bet 6 -1 0",
        "bet 0 12 0",
};

void procCmdArr(const char* arr[], int len) {
    for (int i = 0; i < len; ++i) {
        Command* pCmd = CommandMgr::getSingleton().newCommand(arr[i]);
        if (pCmd != NULL) {
            CommandMgr::getSingleton().procCommand(*pCmd);
            CommandMgr::getSingleton().deleteCommand(pCmd);
        }
    }
}

int main() {
    srand(time(NULL));

//    procCmdArr(CMD, 11);

//    return 0;

    printf("please input cmd...\n");

    while(true) {
        char cmd[1024];
        CommandMgr::getSingleton().getCmd(cmd, 1024);

        Command* pCmd = CommandMgr::getSingleton().newCommand(cmd);
        if (pCmd != NULL) {
            CommandMgr::getSingleton().procCommand(*pCmd);
            CommandMgr::getSingleton().deleteCommand(pCmd);
        }
    }

    return 0;
}