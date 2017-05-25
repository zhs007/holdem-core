//
// Created by zhs007 on 2017/5/25.
//

#ifndef HOLDEM_CORE_AISIMONS_H
#define HOLDEM_CORE_AISIMONS_H

#include "holdemai.h"

class HoldemAI_simons : public HoldemAI {
public:
    HoldemAI_simons(HoldemLogic& hl, int myStation) : HoldemAI(hl, myStation) {}

    virtual ~HoldemAI_simons() {}
public:
    virtual void onCtrl(HoldemCtrl& ctrl);
};

#endif //HOLDEM_CORE_AISIMONS_H
