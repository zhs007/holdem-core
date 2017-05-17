#include <iostream>
#include <stdio.h>
#include "src/cmd.h"

int main() {
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