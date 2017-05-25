//
// Created by zhs007 on 2017/5/25.
//

#ifndef HOLDEM_CORE_HOLDEMAI_H
#define HOLDEM_CORE_HOLDEMAI_H

#include "holdemlogic.h"

class HoldemAI{
public:
    HoldemAI(HoldemLogic& hl, int myStation)
        : m_logic(hl)
        , m_myStation(myStation) {

    }

    virtual ~HoldemAI() {}
public:
    virtual void onCtrl(HoldemCtrl& ctrl) = 0;
protected:
    HoldemLogic&    m_logic;
    int             m_myStation;
};

#endif //HOLDEM_CORE_HOLDEMAI_H
