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

//======================================================================================================================
// ctrl

const int HOLDEM_CTRL_NULL          =   0;  // null
const int HOLDEM_CTRL_ANTE          =   1;  // ante
const int HOLDEM_CTRL_SB            =   2;  // sb
const int HOLDEM_CTRL_BB            =   3;  // bb
const int HOLDEM_CTRL_STRADDLE      =   4;  // straddle
const int HOLDEM_CTRL_FOLD          =   4;  // fold
const int HOLDEM_CTRL_CALL          =   5;  // call
const int HOLDEM_CTRL_CHECK         =   6;  // check
const int HOLDEM_CTRL_RAISE         =   7;  // raise
const int HOLDEM_CTRL_ALLIN         =   8;  // all in
const int HOLDEM_CTRL_FLOP          =   9;  // flop
const int HOLDEM_CTRL_TURN          =   10;  // turn
const int HOLDEM_CTRL_RIVER         =   11;  // river
const int HOLDEM_CTRL_RESULT        =   12;  // result

//======================================================================================================================
// HoldemCtrl

struct HoldemCtrl {
    int         player;
    int         ctrlid;
    CardList    lstCard;
    int         money;

    HoldemCtrl()
            : player(-1)
            , ctrlid(HOLDEM_CTRL_NULL)
            , money(0) {

    }
};

//======================================================================================================================
// HoldemUserInfo

struct HoldemPlayer {
    int         station;
    CardList    lstCard;
    int         money;

    int         bet_turn;

    bool        isFold;
    bool        isAllIn;

    HoldemPlayer()
            : station(-1)
            , money(0)
            , bet_turn(0)
            , isFold(false)
            , isAllIn(false) {

    }
};

//======================================================================================================================
// HoldemLogic

class HoldemLogic {
public:
    HoldemLogic();
    ~HoldemLogic();
public:
    void clear();

    void start(int ante, int sb, int bb, int straddle, int playernums, int button);
public:
    int nextPlayer(int curstation);

    void playerBet(int station, int bet);

    void newTurn();
public:
    void ctrl_ante();

    int ctrl_sb();

    int ctrl_bb(int sbp);

    int ctrl_straddle(int bbp);

    void pushCtrl(int ctrlid, int station, int money, CardList& lstCards);
protected:
    std::vector<HoldemCtrl>     m_lstCtrl;
    std::vector<HoldemPlayer>   m_lstPlayer;

    int                         m_totalPool;
    int                         m_curplayer;

    int                         m_ante;         // 如果-1表示没有ante
    int                         m_straddle;     // 如果-1表示没有straddle

    int                         m_sb;           // sb
    int                         m_bb;           // bb

    int                         m_button;

    int                         m_curBet;       // 当前最大下注
};

#endif //HOLDEM_CORE_HOLDEMLOGIC_H
