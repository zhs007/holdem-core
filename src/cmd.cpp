//
// Created by zhs007 on 2017/5/16.
//

#include <stdlib.h>
#include "cmd.h"
#include "holdemlogic.h"

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

    if (c == '-') {
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

void cmd_getposition(Command& cmd) {
    printf("cmd_getposition playernums:%s, buttonindex:%s, myindex:%s\n",
           cmd.lstParam[0].c_str(), cmd.lstParam[1].c_str(), cmd.lstParam[2].c_str());

    int playernums = atoi(cmd.lstParam[0].c_str());
    int buttonindex = atoi(cmd.lstParam[1].c_str());
    int index = atoi(cmd.lstParam[2].c_str());

    int hs = getHoldemStation(playernums, buttonindex, index);
    std::string hsstr;
    makeHoldemStationStr(hsstr, hs);

    printf("my station: %s\n", hsstr.c_str());
}

void cmd_proccards(Command& cmd) {
    CardList lstcards;

    std::string strp;
    for (int i = 0; i < cmd.lstParam.size(); ++i) {
        CardInfo ci;

        if (i > 0) {
            strp += ",";
        }

        strp += cmd.lstParam[i];

        ci.setWithStr(cmd.lstParam[i].c_str());
        lstcards.addCard(ci);
    }

    printf("cmd_proccard [%s]\n", strp.c_str());

    HoldemCardList hcl;
    hcl.buildWith(lstcards);

    std::string hctstr;
    makeHoldemCardTypeStr(hctstr, hcl.getCardType());

    std::string strcl;
    hcl.makeStr(strcl);

    printf("holdem cardtype: %s[%s]\n", hctstr.c_str(), strcl.c_str());
}

void cmd_analysiscards(Command& cmd) {
    CardList lstcards;
    CardList lstcards1;

    std::string strp;
    for (int i = 0; i < cmd.lstParam.size(); ++i) {
        CardInfo ci;

        if (i > 0) {
            strp += ",";
        }

        strp += cmd.lstParam[i];

        ci.setWithStr(cmd.lstParam[i].c_str());
        lstcards.addCard(ci);
    }

    printf("cmd_analysiscards [%s]\n", strp.c_str());

    HoldemCardTypeProb prob;
    HoldemCardList hcl;

    prob.analysisOthers(lstcards, lstcards1, hcl);
    prob.output();
}

void cmd_analysiscardsex(Command& cmd) {
    CardList lstHand;
    CardList lstCommon;
    CardList lstExclude;

    std::string strp;
    for (int i = 0; i < cmd.lstParam.size(); ++i) {
        CardInfo ci;

        if (i > 0) {
            strp += ",";
        }

        strp += cmd.lstParam[i];

        ci.setWithStr(cmd.lstParam[i].c_str());

        if (i < 2) {
            lstHand.addCard(ci);
        }
        else {
            lstCommon.addCard(ci);
        }
    }

    printf("cmd_analysiscardsex [%s]\n", strp.c_str());

    HoldemCardList hcl;
    hcl.buildWith(lstHand, lstCommon);
    std::string hctstr;
    makeHoldemCardTypeStr(hctstr, hcl.getCardType());
    std::string strcl;
    hcl.makeStr(strcl);
    printf("my cardtype: %s[%s]\n", hctstr.c_str(), strcl.c_str());

    if (lstCommon.getCardNums() < 5) {
        printf("=======> my prob:\n");
        HoldemCardTypeProb prob;
        prob.analysisMe(lstHand, lstCommon, lstExclude);
        prob.output();
    }

    {
        printf("=======> others prob:\n");
        HoldemCardTypeProb prob;
        int winnums = prob.analysisOthers(lstCommon, lstHand, hcl);
        prob.output();

        printf("my win prob: %.2f\n", 100.0f * (prob.getTotalNums() - winnums) / prob.getTotalNums());
    }
}

void cmd_procrange(Command& cmd) {
    CardList lstHand;

    std::string strp;
    for (int i = 0; i < cmd.lstParam.size(); ++i) {
        CardInfo ci;

        if (i > 0) {
            strp += ",";
        }

        strp += cmd.lstParam[i];

        ci.setWithStr(cmd.lstParam[i].c_str());

        lstHand.addCard(ci);
    }

    printf("cmd_procrange [%s]\n", strp.c_str());

    int range = countRange(lstHand);
    printf("my range: %d\n", range);
}

void cmd_begin(Command& cmd) {
    HoldemLogic& logic = CommandMgr::getSingleton().getLogic();

    int arri[6];
    if (cmd.lstParam.size() != 6) {
        printf("please input begin ante sb bb straddle playernums button\n");

        return ;
    }

    for (int i = 0; i < cmd.lstParam.size(); ++i) {
        arri[i] = atoi(cmd.lstParam[i].c_str());
    }

    printf("cmd_begin [%d, %d, %d, %d, %d, %d]\n", arri[0], arri[1], arri[2], arri[3], arri[4], arri[5]);

    logic.init(arri[0], arri[1], arri[2], arri[3], arri[4], arri[5]);
}

void cmd_playerinfo(Command& cmd) {
    HoldemLogic& logic = CommandMgr::getSingleton().getLogic();

    if (cmd.lstParam.size() != 4) {
        printf("please input spi station platform uname isai\n");

        return ;
    }

    int station = atoi(cmd.lstParam[0].c_str());
    std::string platform = cmd.lstParam[1];
    std::string uname = cmd.lstParam[2];
    int isAI = atoi(cmd.lstParam[3].c_str());

    printf("cmd_playerinfo [%d, %s, %s, %d]\n", station, platform.c_str(), uname.c_str(), isAI);

    logic.setPlayer(station, platform.c_str(), uname.c_str(), isAI);
}

void cmd_start(Command& cmd) {
    HoldemLogic& logic = CommandMgr::getSingleton().getLogic();

    printf("cmd_start\n");

    logic.start();
}

void cmd_sethandcards(Command& cmd) {
    HoldemLogic& logic = CommandMgr::getSingleton().getLogic();

    if (cmd.lstParam.size() != 3) {
        printf("please input shc station card0 card1\n");

        return ;
    }

    int station = atoi(cmd.lstParam[0].c_str());

    CardList lstHand;

    std::string strp;
    CardInfo ci0;
    strp += cmd.lstParam[1];
    ci0.setWithStr(cmd.lstParam[1].c_str());

    CardInfo ci1;
    strp += ",";
    strp += cmd.lstParam[2];
    ci1.setWithStr(cmd.lstParam[2].c_str());

    lstHand.addCard(ci0);
    lstHand.addCard(ci1);

    printf("cmd_sethandcards [%d, %s]\n", station, strp.c_str());

    logic.setPlayerHandCards(station, lstHand);
}

void cmd_bet(Command& cmd) {
    HoldemLogic& logic = CommandMgr::getSingleton().getLogic();

    if (cmd.lstParam.size() != 3) {
        printf("please input bet station money isallin\n");
    }

    int station = atoi(cmd.lstParam[0].c_str());
    int money = atoi(cmd.lstParam[1].c_str());
    int isallin = atoi(cmd.lstParam[2].c_str());

    printf("cmd_bet [%d, %d, %d]\n", station, money, isallin);

    logic.playerBet(station, money, isallin != 0, true);
}

void cmd_commoncards(Command& cmd) {
    HoldemLogic& logic = CommandMgr::getSingleton().getLogic();

    if (cmd.lstParam.size() != 3 && cmd.lstParam.size() != 1) {
        printf("please input cc cards(0||3)\n");
    }

    CardList lstCommon;
    std::string strp;
    for (int i = 0; i < cmd.lstParam.size(); ++i) {
        CardInfo ci;

        if (i > 0) {
            strp += ",";
        }

        strp += cmd.lstParam[i];

        ci.setWithStr(cmd.lstParam[i].c_str());

        lstCommon.addCard(ci);
    }

    printf("cmd_commonccards [%s]\n", strp.c_str());

    logic.addCommonCards(lstCommon);
    logic.newTurn();
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
    m_logic.setOutputLog(true);

    regCommand("exit", cmd_exit);                   // 退出 - exit
    regCommand("hs", cmd_getposition);              // 判断位置 - hs 1 2
    regCommand("pc", cmd_proccards);                // 处理牌局面 - pc 1a 2a 3a 4a 52
    regCommand("ac", cmd_analysiscards);            // 分析牌局面 - ac 1a 2a 3a 4a 52
    regCommand("acex", cmd_analysiscardsex);        // 分析牌局面2 - acex 1a 2a 3a 4a 52
    regCommand("pr", cmd_procrange);                // 确定手牌range - pr 1a 2a

    regCommand("begin", cmd_begin);                 // 开始牌局 - begin 0 1 2 0 6 3
    regCommand("spi", cmd_playerinfo);              // 设置牌局用户 - spi 0 ps zhs007 0
    regCommand("st", cmd_start);                    // 开局 - st
    regCommand("shc", cmd_sethandcards);            // 手牌 - shc 0 37 38
    regCommand("bet", cmd_bet);                     // 下注 - bet 0 100 0
    regCommand("cc", cmd_commoncards);              // 发公共牌 - cc 1a 2a 3a
}

CommandMgr::~CommandMgr() {

}