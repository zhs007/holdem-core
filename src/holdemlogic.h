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
const int HOLDEM_STATION_UTG3       =   6;  // UTG3
const int HOLDEM_STATION_UTG4       =   7;  // UTG4
const int HOLDEM_STATION_HJ         =   8;  // hijack
const int HOLDEM_STATION_CO         =   9;  // cut off

void makeHoldemStationStr(std::string& str, int hs);

// 根据人数、button位，确定index位置
// index从0开始
int getHoldemStation(int playernums, int buttonindex, int index);

// 获取手牌range
int countRange(CardList& lstHand);

#endif //HOLDEM_CORE_HOLDEMLOGIC_H
