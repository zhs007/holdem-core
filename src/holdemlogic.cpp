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
        HOLDEM_STATION_MP1,
        HOLDEM_STATION_MP2,
        HOLDEM_STATION_HJ,
        HOLDEM_STATION_CO
};

const int HS_9[] = {
        HOLDEM_STATION_BUTTON,
        HOLDEM_STATION_SB,
        HOLDEM_STATION_BB,
        HOLDEM_STATION_UTG,
        HOLDEM_STATION_UTG1,
        HOLDEM_STATION_MP1,
        HOLDEM_STATION_MP2,
        HOLDEM_STATION_HJ,
        HOLDEM_STATION_CO
};

const int HS_8[] = {
        HOLDEM_STATION_BUTTON,
        HOLDEM_STATION_SB,
        HOLDEM_STATION_BB,
        HOLDEM_STATION_UTG,
        HOLDEM_STATION_UTG1,
        HOLDEM_STATION_MP1,
        HOLDEM_STATION_HJ,
        HOLDEM_STATION_CO
};

const int HS_7[] = {
        HOLDEM_STATION_BUTTON,
        HOLDEM_STATION_SB,
        HOLDEM_STATION_BB,
        HOLDEM_STATION_UTG,
        HOLDEM_STATION_MP1,
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

// 根据人数、button位，确定index位置
// index从0开始
int getHoldemStation(int playernums, int buttonindex, int index) {
    if (playernums <= 5) {
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

    return -1;
}