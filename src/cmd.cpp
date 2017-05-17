//
// Created by zhs007 on 2017/5/16.
//

#include "cmd.h"
#include <stdlib.h>

bool isChar_az09(char c) {
    if (c >= '0' && c <= '9') {
        return true;
    }

    if (c >= 'a' && c <= 'z') {
        return true;
    }

    if (c >= 'A' && c <= 'Z') {
        return true;
    }

    return false;
}

void procString(std::vector<int>& lstb, std::vector<int>& lste, const char* str) {
    int len = strlen(str);
    int bi = -1;
    int ei = -1;
    for (int i = 0; i < len; ++i) {
        if (bi == -1) {
            if (isChar_az09(str[i])) {
                bi = i;
            }
        }
        else if (ei == -1) {
            if (!isChar_az09(str[i])) {
                ei = i;

                lstb.push_back(bi);
                lste.push_back(ei);

                bi = -1;
                ei = -1;
            }
        }
    }

    if (bi >= 0 && ei == -1) {
        ei = len;

        lstb.push_back(bi);
        lste.push_back(ei);
    }
}

void cmd_exit(Command& cmd) {
    printf("cmd_exit\n");
    exit(0);
}

CommandMgr& CommandMgr::getSingleton() {
    static CommandMgr s_mgr;

    return s_mgr;
}

Command* CommandMgr::newCommand(const char* cmd) {
    std::vector<int> lstbi;
    std::vector<int> lstei;

    procString(lstbi, lstei, cmd);

    if (lstbi.size() >= 1) {
        std::string cmdstr = cmd;
        Command* pCmd = new Command();
        pCmd->strCmd = cmdstr.substr(lstbi[0], lstei[0] - lstbi[0]);

        for (int i = 1; i < lstbi.size(); ++i) {
            std::string cstr = cmdstr.substr(lstbi[i], lstei[i] - lstbi[i]);
            pCmd->lstParam.push_back(cstr);
        }

        std::string strbuf;
        strbuf += pCmd->strCmd.c_str();
        strbuf += "(";
        if (pCmd->lstParam.size() > 0) {
            for (int i = 0; i < pCmd->lstParam.size(); ++i) {
                if (i > 0) {
                    strbuf += ",";
                }

                strbuf += pCmd->lstParam[i];
            }
        }
        strbuf += ")";

        printf("parse cmd: %s\n", strbuf.c_str());

        return pCmd;
    }

    return NULL;
}

void CommandMgr::deleteCommand(Command* pCmd) {
    if (pCmd != NULL) {
        delete pCmd;
    }
}

void CommandMgr::procCommand(Command& cmd) {
    _Map::iterator it = m_map.find(cmd.strCmd);
    if (it != m_map.end()) {
        it->second(cmd);
    }
}

void CommandMgr::regCommand(const char* str, FuncCommand func) {
    _Pair p(str, func);
    m_map.insert(p);
}

void CommandMgr::getCmd(char* strbuf, int len) {
    int i = 0;
    char c;
    do {
        c = getchar();
        strbuf[i++] = c;
    } while(c != '\n' && c != EOF && i < len - 1);

    strbuf[i] = '\0';
}

CommandMgr::CommandMgr() {
    regCommand("exit", cmd_exit);
}

CommandMgr::~CommandMgr() {

}