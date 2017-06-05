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
    , m_myRange(-1)
{

}

void HoldemAI_simons::onCtrl(HoldemCtrl& ctrl) {
    if (ctrl.ctrlid == HOLDEM_CTRL_HANDCARDS) {
        m_myHS = m_logic.getPlayer(m_myStation).holdemstation;
        m_myRange = countRange(ctrl.lstCard);

        initAllPlayer();
    }

    int hgs = m_logic.getHoldemGameState();
    if (hgs == HOLDEMGAMESTATE_PREFLOP) {
        if (ctrl.ctrlid == HOLDEM_CTRL_FOLD) {
            m_lstPlayer[ctrl.player].proflopCtrl = PERFLOP_FOLD;
        }
        else if (ctrl.ctrlid == HOLDEM_CTRL_RAISE) {
            m_lstRaisePreflop.push_back(ctrl.player);
            int raisenums = m_lstRaisePreflop.size();
            if (raisenums == 1) {
                m_lstPlayer[ctrl.player].proflopCtrl = PERFLOP_OPEN;
            }
            else {
                m_lstPlayer[ctrl.player].proflopCtrl = PERFLOP_3BET;
            }
        }
        else if (ctrl.ctrlid == HOLDEM_CTRL_CALL || ctrl.ctrlid == HOLDEM_CTRL_CHECK) {
            if (m_lstPlayer[ctrl.player].proflopCtrl == -1) {
                m_lstPlayer[ctrl.player].proflopCtrl = PERFLOP_LIMP;
            }
        }
    }
}

void HoldemAI_simons::onRequest() {
    printf("request simons...\n");
}

void HoldemAI_simons::initAllPlayer() {
    m_lstRaisePreflop.clear();
    m_lstPlayer.clear();

    for (int i = 0; i <= m_logic.getMaxStation(); ++i) {
        HoldemPlayer_simons cp;
        HoldemPlayer& lcp = m_logic.getPlayer(i);

        cp.pPlayer = &lcp;
        if (i == m_myStation) {
            cp.isMe = true;
        }

        m_lstPlayer.push_back(cp);
    }
}
