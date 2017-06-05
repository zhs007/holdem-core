//
// Created by zhs007 on 2017/5/16.
//

#include "holdemlogic.h"
#include "holdemai.h"

const int HS_10[] = {
        HOLDEM_STATION_BUTTON,
        HOLDEM_STATION_SB,
        HOLDEM_STATION_BB,
        HOLDEM_STATION_UTG,
        HOLDEM_STATION_UTG1,
        HOLDEM_STATION_UTG2,
        HOLDEM_STATION_UTG3,
        HOLDEM_STATION_UTG4,
        HOLDEM_STATION_HJ,
        HOLDEM_STATION_CO
};

const int HS_9[] = {
        HOLDEM_STATION_BUTTON,
        HOLDEM_STATION_SB,
        HOLDEM_STATION_BB,
        HOLDEM_STATION_UTG,
        HOLDEM_STATION_UTG1,
        HOLDEM_STATION_UTG2,
        HOLDEM_STATION_UTG3,
        HOLDEM_STATION_HJ,
        HOLDEM_STATION_CO
};

const int HS_8[] = {
        HOLDEM_STATION_BUTTON,
        HOLDEM_STATION_SB,
        HOLDEM_STATION_BB,
        HOLDEM_STATION_UTG,
        HOLDEM_STATION_UTG1,
        HOLDEM_STATION_UTG2,
        HOLDEM_STATION_HJ,
        HOLDEM_STATION_CO
};

const int HS_7[] = {
        HOLDEM_STATION_BUTTON,
        HOLDEM_STATION_SB,
        HOLDEM_STATION_BB,
        HOLDEM_STATION_UTG,
        HOLDEM_STATION_UTG1,
        HOLDEM_STATION_HJ,
        HOLDEM_STATION_CO
};

const int HS_6[] = {
        HOLDEM_STATION_BUTTON,
        HOLDEM_STATION_SB,
        HOLDEM_STATION_BB,
        HOLDEM_STATION_UTG,
        HOLDEM_STATION_HJ,
        HOLDEM_STATION_CO
};

const int HS_5[] = {
        HOLDEM_STATION_BUTTON,
        HOLDEM_STATION_SB,
        HOLDEM_STATION_BB,
        HOLDEM_STATION_UTG,
        HOLDEM_STATION_CO
};

const int HS_4[] = {
        HOLDEM_STATION_BUTTON,
        HOLDEM_STATION_SB,
        HOLDEM_STATION_BB,
        HOLDEM_STATION_UTG
};

const int HS_3[] = {
        HOLDEM_STATION_UTG,
        HOLDEM_STATION_SB,
        HOLDEM_STATION_BB
};

const int HS_2[] = {
        HOLDEM_STATION_BUTTON,
        HOLDEM_STATION_UTG
};

// 根据人数和当前位置与button位的差，求当前位置
// off从0开始
int getHoldemStationWithOff(int playernums, int off) {
    if (playernums == 10) {
        return HS_10[off];
    }

    if (playernums == 9) {
        return HS_9[off];
    }

    if (playernums == 8) {
        return HS_8[off];
    }

    if (playernums == 7) {
        return HS_7[off];
    }

    if (playernums == 6) {
        return HS_6[off];
    }

    if (playernums == 5) {
        return HS_5[off];
    }

    if (playernums == 4) {
        return HS_4[off];
    }

    if (playernums == 3) {
        return HS_3[off];
    }

    if (playernums == 2) {
        return HS_2[off];
    }

    return -1;
}

// 根据人数、button位，确定index位置
// index从0开始
int getHoldemStation(int playernums, int buttonindex, int index) {
    if (playernums <= 1) {
        return -1;
    }

    if (buttonindex < 0 || buttonindex >= playernums) {
        return -1;
    }

    if (index < 0 || index >= playernums) {
        return -1;
    }

    int off = 0;
    if (index > buttonindex) {
        off = index - buttonindex;
    }
    else if (index < buttonindex) {
        off = playernums - buttonindex - 1 + index;
    }

    return getHoldemStationWithOff(playernums, off);
}

void makeHoldemStationStr(std::string& str, int hs) {
    str.clear();

    switch (hs) {
        case HOLDEM_STATION_BUTTON:
            str = "BTN";
            break;
        case HOLDEM_STATION_SB:
            str = "SB";
            break;
        case HOLDEM_STATION_BB:
            str = "BB";
            break;
        case HOLDEM_STATION_UTG:
            str = "UTG";
            break;
        case HOLDEM_STATION_UTG1:
            str = "UTG1";
            break;
        case HOLDEM_STATION_UTG2:
            str = "UTG2";
            break;
        case HOLDEM_STATION_UTG3:
            str = "UTG3";
            break;
        case HOLDEM_STATION_UTG4:
            str = "UTG4";
            break;
        case HOLDEM_STATION_HJ:
            str = "HJ";
            break;
        case HOLDEM_STATION_CO:
            str = "CO";
            break;
    }
}

// 获取手牌range
// 0 - AA KK
// 1 - AKo AKs QQ
// 2 - KQs - 54s
// 3 - JJ TT 99
// 4 - AQs 88 77 66
// 5 - AQo AJs ATs
// 6 - AJo ATo KQo
// 7 - A9s-A2s A9o-A7o KJo KJs KTs 55 44 33 22
// 8 - A6o-A2o QTs J9s T8s 97s 86s 75s 64s
// 9 - QJo KTo QTo JTo K9o K9s K8s K7s Q9s Q8s J8s T7s 96s 85s 74s 63s
// 10 - others
int countRange(CardList& lstHand) {
    lstHand.sort();

    bool isssuit = false;
    bool issrank = false;

    if (lstHand.getCard(0).suit == lstHand.getCard(1).suit) {
        isssuit = true;
    }

    if (lstHand.getCard(0).rank == lstHand.getCard(1).rank) {
        issrank = true;
    }

    if (issrank) {
        if (lstHand.getCard(0).rank == CARD_RANK_A) {
            return 0;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_K) {
            return 0;
        }

        if (lstHand.getCard(0).rank == CARD_RANK_Q) {
            return 1;
        }

        if (lstHand.getCard(0).rank == CARD_RANK_J) {
            return 3;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_10) {
            return 3;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_9) {
            return 3;
        }

        if (lstHand.getCard(0).rank == CARD_RANK_8) {
            return 4;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_7) {
            return 4;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_6) {
            return 4;
        }

        if (lstHand.getCard(0).rank == CARD_RANK_5) {
            return 7;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_4) {
            return 7;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_3) {
            return 7;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_2) {
            return 7;
        }
    }

    if (lstHand.getCard(0).rank == CARD_RANK_A) {
        if (lstHand.getCard(1).rank == CARD_RANK_K) {
            return 1;
        }

        if (isssuit) {
            if (lstHand.getCard(1).rank == CARD_RANK_Q) {
                return 4;
            }

            if (lstHand.getCard(1).rank == CARD_RANK_J) {
                return 5;
            }
            if (lstHand.getCard(1).rank == CARD_RANK_10) {
                return 5;
            }

            return 7;
        }

        if (lstHand.getCard(1).rank == CARD_RANK_Q) {
            return 5;
        }

        if (lstHand.getCard(1).rank == CARD_RANK_J) {
            return 6;
        }
        if (lstHand.getCard(1).rank == CARD_RANK_10) {
            return 6;
        }

        if (lstHand.getCard(1).rank == CARD_RANK_9) {
            return 7;
        }
        if (lstHand.getCard(1).rank == CARD_RANK_8) {
            return 7;
        }
        if (lstHand.getCard(1).rank == CARD_RANK_7) {
            return 7;
        }

        return 8;
    }

    if (lstHand.getCard(0).rank == CARD_RANK_K) {
        if (isssuit) {
            if (lstHand.getCard(1).rank == CARD_RANK_Q) {
                return 2;
            }

            if (lstHand.getCard(1).rank == CARD_RANK_J) {
                return 7;
            }
            if (lstHand.getCard(1).rank == CARD_RANK_10) {
                return 7;
            }

            if (lstHand.getCard(1).rank == CARD_RANK_9) {
                return 9;
            }
            if (lstHand.getCard(1).rank == CARD_RANK_8) {
                return 9;
            }
            if (lstHand.getCard(1).rank == CARD_RANK_7) {
                return 9;
            }

            return 10;
        }

        if (lstHand.getCard(1).rank == CARD_RANK_Q) {
            return 6;
        }

        if (lstHand.getCard(1).rank == CARD_RANK_J) {
            return 7;
        }

        if (lstHand.getCard(1).rank == CARD_RANK_10) {
            return 9;
        }
        if (lstHand.getCard(1).rank == CARD_RANK_9) {
            return 9;
        }

        return 10;
    }

    if (isssuit) {
        if (lstHand.getCard(0).rank == CARD_RANK_Q && lstHand.getCard(1).rank == CARD_RANK_J) {
            return 2;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_J && lstHand.getCard(1).rank == CARD_RANK_10) {
            return 2;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_10 && lstHand.getCard(1).rank == CARD_RANK_9) {
            return 2;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_9 && lstHand.getCard(1).rank == CARD_RANK_8) {
            return 2;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_8 && lstHand.getCard(1).rank == CARD_RANK_7) {
            return 2;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_7 && lstHand.getCard(1).rank == CARD_RANK_6) {
            return 2;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_6 && lstHand.getCard(1).rank == CARD_RANK_5) {
            return 2;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_5 && lstHand.getCard(1).rank == CARD_RANK_4) {
            return 2;
        }

        if (lstHand.getCard(0).rank == CARD_RANK_Q && lstHand.getCard(1).rank == CARD_RANK_10) {
            return 8;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_J && lstHand.getCard(1).rank == CARD_RANK_9) {
            return 8;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_10 && lstHand.getCard(1).rank == CARD_RANK_8) {
            return 8;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_9 && lstHand.getCard(1).rank == CARD_RANK_7) {
            return 8;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_8 && lstHand.getCard(1).rank == CARD_RANK_6) {
            return 8;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_7 && lstHand.getCard(1).rank == CARD_RANK_5) {
            return 8;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_6 && lstHand.getCard(1).rank == CARD_RANK_4) {
            return 8;
        }

        if (lstHand.getCard(0).rank == CARD_RANK_Q && lstHand.getCard(1).rank == CARD_RANK_9) {
            return 9;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_J && lstHand.getCard(1).rank == CARD_RANK_8) {
            return 9;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_10 && lstHand.getCard(1).rank == CARD_RANK_7) {
            return 9;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_9 && lstHand.getCard(1).rank == CARD_RANK_6) {
            return 9;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_8 && lstHand.getCard(1).rank == CARD_RANK_5) {
            return 9;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_7 && lstHand.getCard(1).rank == CARD_RANK_4) {
            return 9;
        }
        if (lstHand.getCard(0).rank == CARD_RANK_6 && lstHand.getCard(1).rank == CARD_RANK_3) {
            return 9;
        }
    }

    if (lstHand.getCard(0).rank == CARD_RANK_Q && lstHand.getCard(1).rank == CARD_RANK_J) {
        return 9;
    }
    if (lstHand.getCard(0).rank == CARD_RANK_Q && lstHand.getCard(1).rank == CARD_RANK_10) {
        return 9;
    }
    if (lstHand.getCard(0).rank == CARD_RANK_J && lstHand.getCard(1).rank == CARD_RANK_10) {
        return 9;
    }

    return 10;
}

//======================================================================================================================
// HoldemLogic

HoldemLogic::HoldemLogic()
        : m_isOutputLog(false) {

    clear();
}

HoldemLogic::~HoldemLogic() {

}

void HoldemLogic::clear() {
    m_lstCtrl.clear();
    m_lstPlayer.clear();

    m_totalPool = 0;
    m_curplayer = -1;

    m_ante = -1;         // 如果-1表示没有ante
    m_straddle = -1;     // 如果-1表示没有straddle

    m_button = -1;

    m_curBet = 0;

    m_maxPlayer = -1;
    m_playerWaitBB = -1;

    m_commonCards.clear();
}

void HoldemLogic::init(int ante, int sb, int bb, int straddle, int playernums, int button) {
    clear();

    m_ante = ante;
    m_straddle = straddle;
    m_sb = sb;
    m_bb = bb;

    m_button = button;

    for (int i = 0; i < playernums; ++i) {
        HoldemPlayer hp;

        hp.station = i;

        m_lstPlayer.push_back(hp);
    }

    m_playerNums = playernums;
}

void HoldemLogic::setPlayer(int station, const char* platform, const char* name, bool isAI) {
    if (station >= 0 && station < m_lstPlayer.size()) {
        m_lstPlayer[station].platform = platform;
        m_lstPlayer[station].name = name;

        if (isAI) {
            m_lstPlayer[station].pAI = HoldemAIMgr::getSingleton().newAI(name, *this, station);
        }
    }
}

void HoldemLogic::start() {
    countAllHoldemStation();

    ctrl_ante();
    int sbp = ctrl_sb();
    int bbp = ctrl_bb(sbp);
    int sp = ctrl_straddle(bbp);

    m_curplayer = nextPlayer(sp);
    m_maxPlayer = sp;
    m_playerWaitBB = sp;

    requestPlayerCtrl(m_curplayer);
}

int HoldemLogic::nextPlayer(int curstation) {
    if (curstation >= m_lstPlayer.size() - 1) {
        for (int i = 0; i < curstation; ++i) {
            if (!m_lstPlayer[i].isFold && !m_lstPlayer[i].isAllIn) {
                return i;
            }
        }

        return -1;
    }

    for (int i = curstation + 1; i < m_lstPlayer.size(); ++i) {
        if (!m_lstPlayer[i].isFold && !m_lstPlayer[i].isAllIn) {
            return i;
        }
    }

    for (int i = 0; i < curstation; ++i) {
        if (!m_lstPlayer[i].isFold && !m_lstPlayer[i].isAllIn) {
            return i;
        }
    }

    return -1;
}

void HoldemLogic::playerBet(int station, int bet, bool isAllIn, bool isAddCtrl) {
    int lastbet = m_curBet;
    int curctrl = HOLDEM_CTRL_CHECK;

    if (bet < 0) {
        curctrl = HOLDEM_CTRL_FOLD;
    }
    else if (bet == 0) {
        curctrl = HOLDEM_CTRL_CHECK;
    }

    if (bet > 0) {
        m_lstPlayer[station].bet_turn += bet;
        m_totalPool += bet;

        if (m_curBet < m_lstPlayer[station].bet_turn) {
            m_curBet = m_lstPlayer[station].bet_turn;
        }

        if (m_playerWaitBB >= 0) {
            m_playerWaitBB = -1;
        }

        m_maxPlayer = station;
    }

    if (curctrl == HOLDEM_CTRL_FOLD) {
        m_lstPlayer[station].isFold = true;
    }

    if (isAllIn) {
        m_lstPlayer[station].isAllIn = true;

        curctrl = HOLDEM_CTRL_ALLIN;
    }
    else if (m_lstPlayer[station].bet_turn > lastbet) {
        curctrl = HOLDEM_CTRL_RAISE;
    }
    else if (m_lstPlayer[station].bet_turn == lastbet) {
        curctrl = HOLDEM_CTRL_CALL;
    }

    if (isAddCtrl) {
        CardList lstCards;

        pushCtrl(curctrl, station, bet, lstCards);

        if (m_isOutputLog) {
            printf("%d players bet %d\n", station, bet);
        }

        if (m_playerWaitBB == station) {
            newTurn();

            return ;
        }

        int ns = nextPlayer(station);
        if (ns == m_maxPlayer) {
            if (m_playerWaitBB == ns) {
                m_curplayer = ns;
                requestPlayerCtrl(m_curplayer);
            }
            else {
                //newTurn();
            }
        }
        else {
            m_curplayer = ns;
            requestPlayerCtrl(m_curplayer);
        }
    }
}

void HoldemLogic::newTurn() {
    for (int i = 0; i < m_lstPlayer.size(); ++i) {
        m_lstPlayer[i].bet_turn = 0;
    }

    m_curBet = 0;

    m_curplayer = nextPlayer(m_button);
    requestPlayerCtrl(m_curplayer);
}

void HoldemLogic::requestPlayerCtrl(int station) {
    if (m_isOutputLog) {
        printf("request %d ctrl...\n", station);

        if (m_lstPlayer[station].pAI != NULL) {
            m_lstPlayer[station].pAI->onRequest();
        }
    }
}

void HoldemLogic::setPlayerHandCards(int station, CardList& lstCards) {
    m_lstPlayer[station].lstCard.clear();
    m_lstPlayer[station].lstCard.addCardList(lstCards);

    char str0[3];
    char str1[3];
    lstCards.getCard(0).makeString(str0);
    lstCards.getCard(1).makeString(str1);

    if (m_isOutputLog) {
        printf("%d player handcards %s %s\n", station, str0, str1);
    }

    ctrl_handcards(station, lstCards);
}

void HoldemLogic::ctrl_ante() {
    if (m_ante > 0) {
        m_totalPool += m_lstPlayer.size() * m_ante;

        CardList lstCards;
        pushCtrl(HOLDEM_CTRL_ANTE, -1, m_ante, lstCards);

        if (m_isOutputLog) {
            printf("all players ante %d\n", m_ante);
        }
    }
}

int HoldemLogic::ctrl_sb() {
    int sbp = nextPlayer(m_button);
    playerBet(sbp, m_sb);

    CardList lstCards;
    pushCtrl(HOLDEM_CTRL_SB, sbp, m_sb, lstCards);

    if (m_isOutputLog) {
        printf("%d players bet sb %d\n", sbp, m_sb);
    }

    return sbp;
}

int HoldemLogic::ctrl_bb(int sbp) {
    int bbp = nextPlayer(sbp);
    playerBet(bbp, m_bb);

    CardList lstCards;
    pushCtrl(HOLDEM_CTRL_BB, bbp, m_bb, lstCards);

    if (m_isOutputLog) {
        printf("%d players bet bb %d\n", bbp, m_bb);
    }

    return bbp;
}

int HoldemLogic::ctrl_straddle(int bbp) {
    if (m_straddle > 0) {
        int sp = nextPlayer(bbp);
        playerBet(sp, m_straddle);

        CardList lstCards;
        pushCtrl(HOLDEM_CTRL_STRADDLE, sp, m_straddle, lstCards);

        return sp;
    }

    return bbp;
}

void HoldemLogic::ctrl_handcards(int station, CardList& lstCards) {
    pushCtrl(HOLDEM_CTRL_HANDCARDS, station, 0, lstCards);
}

void HoldemLogic::pushCtrl(int ctrlid, int station, int money, CardList& lstCards) {
    HoldemCtrl hc;

    hc.ctrlid = ctrlid;
    hc.player = station;
    hc.money = money;
    hc.lstCard = lstCards;

    m_lstCtrl.push_back(hc);

    for (int i = 0; i < m_lstPlayer.size(); ++i) {
        if (m_lstPlayer[i].pAI != NULL) {
            m_lstPlayer[i].pAI->onCtrl(hc);
        }
    }
}

void HoldemLogic::addCommonCards(CardList& lstCards) {

    if (m_commonCards.getCardNums() == 0) {
        m_commonCards.addCardList(lstCards);

        ctrl_flop(lstCards);
    }
    else if (m_commonCards.getCardNums() == 3) {
        m_commonCards.addCardList(lstCards);

        ctrl_turn(lstCards);
    }
    else if (m_commonCards.getCardNums() == 4) {
        m_commonCards.addCardList(lstCards);

        ctrl_river(lstCards);
    }
}

void HoldemLogic::ctrl_flop(CardList& lstCards) {
    pushCtrl(HOLDEM_CTRL_FLOP, -1, 0, lstCards);
}

void HoldemLogic::ctrl_turn(CardList& lstCards) {
    pushCtrl(HOLDEM_CTRL_TURN, -1, 0, lstCards);
}

void HoldemLogic::ctrl_river(CardList& lstCards) {
    pushCtrl(HOLDEM_CTRL_RIVER, -1, 0, lstCards);
}

// 根据button位，计算所有人的位置
void HoldemLogic::countAllHoldemStation() {
    int off = 0;
    for (int i = m_button; i < m_lstPlayer.size(); ++i) {
        if (!m_lstPlayer[i].isLeft) {
            m_lstPlayer[i].holdemstation = getHoldemStationWithOff(m_playerNums, off);
            ++off;
        }
    }

    for (int i = 0; i < m_button; ++i) {
        if (!m_lstPlayer[i].isLeft) {
            m_lstPlayer[i].holdemstation = getHoldemStationWithOff(m_playerNums, off);
            ++off;
        }
    }
}

void HoldemLogic::playerLeft(int station) {
    m_lstPlayer[station].isLeft = true;

    m_playerNums--;
}

void HoldemLogic::playerIn(int station) {
    m_lstPlayer[station].isLeft = false;

    m_playerNums++;
}