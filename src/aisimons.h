//
// Created by zhs007 on 2017/5/25.
//

#ifndef HOLDEM_CORE_AISIMONS_H
#define HOLDEM_CORE_AISIMONS_H

#include "holdemai.h"

// 操作类型
const int CTRLSTATE_FOLD     = 0;    // 不玩了
const int CTRLSTATE_LIMP     = 1;    // 跟进去
const int CTRLSTATE_OPEN     = 2;    // 主动的开口，不下大注
const int CTRLSTATE_REDICAL  = 3;    // 开口，且下大注

// preflop 玩家操作标记
const int PERFLOP_FOLD = 0;
const int PERFLOP_LIMP = 1;
const int PERFLOP_OPEN = 2;
const int PERFLOP_3BET = 3;

const int COMMON_FOLD       = 0;
const int COMMON_SMALLBET   = 1;
const int COMMON_BIGBET     = 2;
const int COMMON_SUPPERBET  = 3;

struct HoldemPlayer_simons {
    int             proflopCtrl;
    int             flopCtrl;
    int             turnCtrl;
    int             riverCtrl;
    bool            isMe;

    HoldemPlayer*   pPlayer;

    HoldemPlayer_simons()
            : isMe(false)
            , proflopCtrl(-1)
            , pPlayer(NULL)
            , flopCtrl(-1)
            , turnCtrl(-1)
            , riverCtrl(-1)
    {

    }
};

class HoldemAI_simons : public HoldemAI {
public:
    HoldemAI_simons(HoldemLogic& hl, int myStation);

    virtual ~HoldemAI_simons() {}
public:
    virtual void onCtrl(HoldemCtrl& ctrl);

    virtual void onRequest();
protected:
    void initAllPlayer();

    int onRequest_Preflop();

    int onRequest_Preflop4();

    void ctrlBet(int ctrlstate);
public:
    int                                 m_myHS;
    int                                 m_myRange;

    std::vector<HoldemPlayer_simons>    m_lstPlayer;

    std::vector<int>                    m_lstRaisePreflop;

    int                                 m_waitFirstCtrl;        // 剩下的第一次操作都没做的人
    int                                 m_lastPlayerNums;       // 还没有弃牌或者allin的玩家人数
    int                                 m_maxPlayer;            // 开局时最大的玩家人数
};

#endif //HOLDEM_CORE_AISIMONS_H
