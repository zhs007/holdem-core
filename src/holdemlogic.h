//
// Created by zhs007 on 2017/5/16.
//

#ifndef HOLDEM_CORE_HOLDEMLOGIC_H
#define HOLDEM_CORE_HOLDEMLOGIC_H

#include <string>
#include "holdemcardlogic.h"

class HoldemAI;

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

// 根据人数和当前位置与button位的差，求当前位置
// off从0开始
int getHoldemStationWithOff(int playernums, int off);

// 获取手牌range
int countRange(CardList& lstHand);

//======================================================================================================================
// HoldemGameState

const int HOLDEMGAMESTATE_PREFLOP   =   0;
const int HOLDEMGAMESTATE_FLOP      =   1;
const int HOLDEMGAMESTATE_TURN      =   2;
const int HOLDEMGAMESTATE_RIVER     =   3;

//======================================================================================================================
// ctrl

const int HOLDEM_CTRL_NULL          =   0;  // null
const int HOLDEM_CTRL_FLOP          =   1;  // flop
const int HOLDEM_CTRL_TURN          =   2;  // turn
const int HOLDEM_CTRL_RIVER         =   3;  // river
const int HOLDEM_CTRL_RESULT        =   4;  // result
const int HOLDEM_CTRL_HANDCARDS     =   5;  // handcards
const int HOLDEM_CTRL_BET           =   6;  // bet

//======================================================================================================================
// HoldemBetType

const int HOLDEM_BET_ANTE           =   1;  // ante
const int HOLDEM_BET_SB             =   2;  // sb
const int HOLDEM_BET_BB             =   3;  // bb
const int HOLDEM_BET_STRADDLE       =   4;  // straddle
const int HOLDEM_BET_FOLD           =   5;  // fold
const int HOLDEM_BET_CALL           =   6;  // call
const int HOLDEM_BET_CHECK          =   7;  // check
const int HOLDEM_BET_RAISE          =   8;  // raise

//======================================================================================================================
// HoldemCtrl

struct HoldemCtrl {
    int         player;
    int         ctrlid;
    CardList    lstCard;

    bool        isForce;    // 是否强制，ante、sb、bb、straddle都算强制
    bool        isRaise;    // 是否加注
    bool        isAllin;    // 是否allin
    bool        isFold;     // 是否fold

    int         money;

    int         bettype;    // 下注类型

    HoldemCtrl()
            : player(-1)
            , ctrlid(HOLDEM_CTRL_NULL)
            , money(0)
            , bettype(-1)
            , isForce(false)
            , isRaise(false)
            , isAllin(false)
            , isFold(false)
    {

    }
};

//======================================================================================================================
// HoldemPlayer

struct HoldemPlayer {
    std::string platform;
    std::string name;

    int         station;
    CardList    lstCard;
    int         money;

    int         bet_turn;

    bool        isFold;
    bool        isAllIn;

    bool        isLeft;

    int         holdemstation;

    HoldemAI*   pAI;

    HoldemPlayer()
            : station(-1)
            , money(0)
            , bet_turn(0)
            , isFold(false)
            , isAllIn(false)
            , pAI(NULL)
            , holdemstation(-1)
            , isLeft(false)
    {

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

    void setOutputLog(bool isOutputLog) { m_isOutputLog = isOutputLog; }

    void init(int ante, int sb, int bb, int straddle, int playernums, int button);

    void setPlayer(int station, const char* platform, const char* name, bool isAI);

    void playerLeft(int station);

    void playerIn(int station);

    void start();
public:
    int nextPlayer(int curstation);

    void playerBet(int station, int bet, bool isAllIn = false, bool isAddCtrl = false);

    void newTurn();

    void requestPlayerCtrl(int station);

    void setPlayerHandCards(int station, CardList& lstCards);

    void addCommonCards(CardList& lstCards);
public:
    // 根据button位，计算所有人的位置
    void countAllHoldemStation();
public:
    void ctrl_ante();

    int ctrl_sb();

    int ctrl_bb(int sbp);

    int ctrl_straddle(int bbp);

    void ctrl_handcards(int station, CardList& lstCards);

    void ctrl_flop(CardList& lstCards);

    void ctrl_turn(CardList& lstCards);

    void ctrl_river(CardList& lstCards);

    void pushCtrl(int ctrlid, int station, int money, CardList& lstCards, int bettype, bool isRaise, bool isAllin);
public:
    HoldemPlayer& getPlayer(int station) { return m_lstPlayer[station]; }

    int getMaxStation() { return m_maxStation; }

    int getHoldemGameState() { return m_holdemState; }

    int getBB() { return m_bb; }
protected:
    std::vector<HoldemCtrl>     m_lstCtrl;
    std::vector<HoldemPlayer>   m_lstPlayer;
    int                         m_playerNums;
    int                         m_maxStation;

    int                         m_totalPool;
    int                         m_curplayer;

    int                         m_ante;         // 如果-1表示没有ante
    int                         m_straddle;     // 如果-1表示没有straddle

    int                         m_sb;           // sb
    int                         m_bb;           // bb

    int                         m_button;

    int                         m_curBet;       // 当前最大下注

    bool                        m_isOutputLog;

    int                         m_maxPlayer;    // 当前最大注玩家
    int                         m_playerWaitBB; // 这个不为-1时，表示最后到BB或straddle玩家行动（最大注玩家是BB或straddle时）

    CardList                    m_commonCards;

    int                         m_holdemState;
};

#endif //HOLDEM_CORE_HOLDEMLOGIC_H
