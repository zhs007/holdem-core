//
// Created by zhs007 on 2017/5/25.
//

#ifndef HOLDEM_CORE_HOLDEMAI_H
#define HOLDEM_CORE_HOLDEMAI_H

#include <functional>
#include <map>
#include "holdemlogic.h"

class HoldemAI;
typedef std::function< HoldemAI*(HoldemLogic&, int) > FuncNewAI;

//======================================================================================================================
// HoldemAI

class HoldemAI{
public:
    HoldemAI(HoldemLogic& hl, int myStation)
        : m_logic(hl)
        , m_myStation(myStation) {

    }

    virtual ~HoldemAI() {}
public:
    virtual void onCtrl(HoldemCtrl& ctrl) = 0;

    virtual void onRequest() = 0;
protected:
    HoldemLogic&    m_logic;
    int             m_myStation;
};

//======================================================================================================================
// HoldemAIMgr

class HoldemAIMgr{
public:
    typedef std::map<std::string, FuncNewAI> _Map;
    typedef std::pair<std::string, FuncNewAI> _Pair;
public:
    static HoldemAIMgr& getSingleton();
public:
    bool regAI(const char* ainame, FuncNewAI func);

    HoldemAI* newAI(const char* ainame, HoldemLogic& hl, int myStation);
protected:
    HoldemAIMgr();
    ~HoldemAIMgr();
protected:
    _Map m_map;
};

#endif //HOLDEM_CORE_HOLDEMAI_H
