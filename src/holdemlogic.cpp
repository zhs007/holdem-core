//
// Created by zhs007 on 2017/5/16.
//

#include "holdemlogic.h"

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
