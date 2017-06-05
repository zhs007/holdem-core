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

HoldemAI_simons::HoldemAI_simons(HoldemLogic& hl, int myStation)
    : HoldemAI(hl, myStation)
    , m_myHS(-1)
{

}

void HoldemAI_simons::onCtrl(HoldemCtrl& ctrl) {
    if (ctrl.ctrlid == HOLDEM_CTRL_HANDCARDS) {
        m_myHS = m_logic.getPlayer(m_myStation).holdemstation;
    }
}

void HoldemAI_simons::onRequest() {
    printf("request simons...\n");
}

