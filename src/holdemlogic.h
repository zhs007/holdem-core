//
// Created by zhs007 on 2017/5/16.
//

#ifndef HOLDEM_CORE_HOLDEMLOGIC_H
#define HOLDEM_CORE_HOLDEMLOGIC_H

#include <string>
#include "holdemcardlogic.h"

const int HOLDEM_STATION_BUTTON     =   0;  // button
const int HOLDEM_STATION_SB         =   1;  // small blind
const int HOLDEM_STATION_BB         =   2;  // big blind
const int HOLDEM_STATION_UTG        =   3;  // under the gun
const int HOLDEM_STATION_UTG1       =   4;  // UTG1
const int HOLDEM_STATION_UTG2       =   5;  // UTG2
const int HOLDEM_STATION_MP1        =   6;  // middle position
const int HOLDEM_STATION_MP2        =   7;  // MP2
const int HOLDEM_STATION_HJ         =   8;  // hijack
const int HOLDEM_STATION_CO         =   9;  // cut off

void makeHoldemStationStr(std::string& str, int hs);

// 根据人数、button位，确定index位置
// index从0开始
int getHoldemStation(int playernums, int buttonindex, int index);

#endif //HOLDEM_CORE_HOLDEMLOGIC_H
