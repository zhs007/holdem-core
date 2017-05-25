//
// Created by zhs007 on 2017/5/25.
//

#include "aisimons.h"
#include "holdemai.h"

HoldemAI* newAI_simons(HoldemLogic& hl, int myStation) {
    return new HoldemAI_simons(hl, myStation);
}

static const char* s_simonsai_name = "simons";
static bool s_simonsai_isinit = HoldemAIMgr::getSingleton().regAI(s_simonsai_name, newAI_simons);

void HoldemAI_simons::onCtrl(HoldemCtrl& ctrl) {

}

