//
// Created by zhs007 on 2017/5/25.
//

#include "holdemai.h"

//======================================================================================================================
// HoldemAIMgr

HoldemAIMgr& HoldemAIMgr::getSingleton() {
    static HoldemAIMgr s_mgr;

    return s_mgr;
}

bool HoldemAIMgr::regAI(const char* ainame, FuncNewAI func) {
    _Pair p(ainame, func);

    std::pair<_Map::iterator, bool> ret = m_map.insert(p);

    return ret.second;
}

HoldemAI* HoldemAIMgr::newAI(const char* ainame, HoldemLogic& hl, int myStation) {
    _Map::iterator it = m_map.find(ainame);
    if (it != m_map.end()) {
        return it->second(hl, myStation);
    }

    return NULL;
}

HoldemAIMgr::HoldemAIMgr() {

}

HoldemAIMgr::~HoldemAIMgr() {

}