//
// Created by zhs007 on 2017/5/25.
//

#ifndef HOLDEM_CORE_AISIMONS_H
#define HOLDEM_CORE_AISIMONS_H

#include "holdemai.h"

// preflop 玩家操作标记
const int PERFLOP_FOLD = 0;
const int PERFLOP_LIMP = 1;
const int PERFLOP_OPEN = 2;
const int PERFLOP_3BET = 3;

struct HoldemPlayer_simons {
    int             proflopCtrl;
    bool            isMe;

    HoldemPlayer*   pPlayer;

    HoldemPlayer_simons()
            : isMe(false)
            , proflopCtrl(-1)
            , pPlayer(NULL)
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
public:
    int                                 m_myHS;
    int                                 m_myRange;

    std::vector<HoldemPlayer_simons>    m_lstPlayer;

    std::vector<int>                    m_lstRaisePreflop;
};

#endif //HOLDEM_CORE_AISIMONS_H
