//
// Created by zhs007 on 2017/5/25.
//

#include "aisimons.h"
#include "holdemai.h"
#include "base.h"

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

    if (ctrl.ctrlid == HOLDEM_CTRL_BET) {
        if (m_waitFirstCtrl > 0) {
            m_waitFirstCtrl--;
        }

        if (ctrl.isFold || ctrl.isAllin) {
            m_lastPlayerNums--;
        }
    }

    int hgs = m_logic.getHoldemGameState();
    if (hgs == HOLDEMGAMESTATE_PREFLOP) {
        if (ctrl.ctrlid == HOLDEM_CTRL_BET) {
            if (ctrl.isForce) {

            }
            else if (ctrl.isFold) {
                m_lstPlayer[ctrl.player].proflopCtrl = PERFLOP_FOLD;
            }
            else if (ctrl.isRaise) {
                m_lstRaisePreflop.push_back(ctrl.player);
                int raisenums = m_lstRaisePreflop.size();
                if (raisenums == 1) {
                    m_lstPlayer[ctrl.player].proflopCtrl = PERFLOP_OPEN;
                }
                else {
                    m_lstPlayer[ctrl.player].proflopCtrl = PERFLOP_3BET;
                }
            }
            else {
                if (m_lstPlayer[ctrl.player].proflopCtrl == -1) {
                    m_lstPlayer[ctrl.player].proflopCtrl = PERFLOP_LIMP;
                }
            }
        }
    }
}

void HoldemAI_simons::onRequest() {
    printf("request simons...\n");
    int hgs = m_logic.getHoldemGameState();
    if (hgs == HOLDEMGAMESTATE_PREFLOP) {
        int ctrlstate = onRequest_Preflop();

        printf("simons %d\n", ctrlstate);
        ctrlBet(ctrlstate);
    }
}

int HoldemAI_simons::onRequest_Preflop() {
    if (m_maxPlayer >= 4) {
        return onRequest_Preflop4();
    }

    // 先不考虑角色不全的情况
    return CTRLSTATE_FOLD;
}

int HoldemAI_simons::onRequest_Preflop4() {
    int raiseplayers = m_lstRaisePreflop.size();
    if (raiseplayers == 0) {    // 没人加注
        if (m_lstPlayer[m_myStation].pPlayer->holdemstation == HOLDEM_STATION_UTG) {
            if (m_myRange <= 4) {
                return CTRLSTATE_OPEN;
            }
            else if (m_myRange >= 9) {
                return CTRLSTATE_LIMP;
            }

            return CTRLSTATE_FOLD;
        }
        else if (m_lstPlayer[m_myStation].pPlayer->holdemstation == HOLDEM_STATION_BUTTON) {
            if (m_myRange <= 4) {
                return CTRLSTATE_OPEN;
            }
            else if (m_myRange <= 9) {
                if (roll(100, 50)) {
                    return CTRLSTATE_OPEN;
                }
                else {
                    return CTRLSTATE_LIMP;
                }
            }

            return CTRLSTATE_FOLD;
        }

        if (m_waitFirstCtrl == 1) {
            if (m_myRange <= 4) {
                return CTRLSTATE_OPEN;
            }
            else if (m_myRange <= 9) {
                if (roll(100, 20)) {
                    return CTRLSTATE_OPEN;
                }
                else {
                    return CTRLSTATE_LIMP;
                }
            }

            return CTRLSTATE_FOLD;
        }

        if (m_myRange <= 4) {
            return CTRLSTATE_OPEN;
        }
        else if (m_myRange <= 9) {
            if (roll(100, 40 - m_waitFirstCtrl * 2)) {
                return CTRLSTATE_OPEN;
            }
            else {
                return CTRLSTATE_LIMP;
            }
        }

        return CTRLSTATE_FOLD;
    }

    int callbet = m_logic.getCallBet();
    int mycallbet = m_logic.getPlayerCallBet(m_myStation);
    bool iswarning = false;

    // 如果我还没操作过
    if (m_lstPlayer[m_myStation].proflopCtrl == -1) {
        if (mycallbet > m_logic.getBB() * 5) {
            iswarning = true;
        }

        if (raiseplayers >= 2) {
            iswarning = true;
        }

        if (iswarning) {
            if (m_myRange <= 1) {
                return CTRLSTATE_LIMP;
            }

            return CTRLSTATE_FOLD;
        }

        if (m_myRange <= 1) {
            return CTRLSTATE_OPEN;
        }
        else if (m_myRange <= 4) {
            return CTRLSTATE_LIMP;
        }

        return CTRLSTATE_FOLD;
    }

    if (m_myRange <= 1) {
        return CTRLSTATE_REDICAL;
    }
    else if (m_myRange <= 4) {
        if (roll(100, 50)) {
            return CTRLSTATE_OPEN;
        }
        else {
            return CTRLSTATE_LIMP;
        }
    }

    return CTRLSTATE_FOLD;
}

void HoldemAI_simons::initAllPlayer() {
    m_lstRaisePreflop.clear();
    m_lstPlayer.clear();

    m_waitFirstCtrl = 0;
    m_lastPlayerNums = 0;
    m_maxPlayer = 0;

    for (int i = 0; i <= m_logic.getMaxStation(); ++i) {
        HoldemPlayer_simons cp;
        HoldemPlayer& lcp = m_logic.getPlayer(i);

        cp.pPlayer = &lcp;
        if (i == m_myStation) {
            cp.isMe = true;
        }

        m_lstPlayer.push_back(cp);

        if (!lcp.isLeft) {
            m_waitFirstCtrl++;
            m_lastPlayerNums++;
            m_maxPlayer++;
        }
    }
}

void HoldemAI_simons::ctrlBet(int ctrlstate) {
    int mycallbet = m_logic.getPlayerCallBet(m_myStation);

    if (ctrlstate == CTRLSTATE_FOLD) {
        m_logic.playerCheckOrFold(m_myStation);
    }
    else if (ctrlstate == CTRLSTATE_LIMP) {
        m_logic.playerCall(m_myStation);
    }
    else if (ctrlstate == CTRLSTATE_OPEN) {
        m_logic.playerBet(m_myStation, mycallbet + m_logic.getBB() * 3, false, true);
    }
    else if (ctrlstate == CTRLSTATE_REDICAL) {
        m_logic.playerBet(m_myStation, mycallbet + m_logic.getPOT(), false, true);
    }
    else {
        m_logic.playerCheckOrFold(m_myStation);
    }
}