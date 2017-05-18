//
// Created by zhs007 on 2017/5/14.
//

#ifndef HOLDEM_CORE_HOLDEMCARDLOGIC_H
#define HOLDEM_CORE_HOLDEMCARDLOGIC_H

#include "cardlogic.h"
#include <string>

const int HOLDEM_CARDTYPE_HIGHCARD      = 0;    // high card
const int HOLDEM_CARDTYPE_PAIR          = 1;    // pair
const int HOLDEM_CARDTYPE_2PAIR         = 2;    // two pair
const int HOLDEM_CARDTYPE_THREE         = 3;    // three of a kind
const int HOLDEM_CARDTYPE_STRAIGHT      = 4;    // straight
const int HOLDEM_CARDTYPE_FLUSH         = 5;    // flush
const int HOLDEM_CARDTYPE_FULLHOUSE     = 6;    // full house
const int HOLDEM_CARDTYPE_FOUR          = 7;    // four of a kind
const int HOLDEM_CARDTYPE_STRAIGHTFLUSH = 8;    // straight flush
const int HOLDEM_CARDTYPE_ROYALFLUSH    = 9;    // royal flush

const int MAX_HOLDEM_CARDTYPE           = 10;   // 牌型10种

void makeHoldemCardTypeStr(std::string& str, int hct);

//======================================================================================================================
// 牌型概率
class HoldemCardTypeProb {
public:
    HoldemCardTypeProb() { clear(); }
    ~HoldemCardTypeProb() {}
public:
    void analysisOthers(CardList& lstCards, CardList& lstExclude);
public:
    void clear();

    void addCardTypeNums(int ct, int nums);

    void output();
protected:
    void _analysisOthers_3_foreach(CardList& lstCards, CardList& lstExclude);
    void _analysisOthers_4_foreach(CardList& lstCards, CardList& lstExclude);
    void _analysisOthers_5_foreach(CardList& lstCards, CardList& lstExclude);

    void _analysisOthers_3(CardList& lstCards, CardList& lstExclude);

    int countTotalNums(int totalnums, int neednums);
protected:
    int m_numsCardType[MAX_HOLDEM_CARDTYPE];
    int m_numsTotal;
};

//======================================================================================================================
// 德州牌
class HoldemCardList : public CardList {
public:
    HoldemCardList();
    virtual ~HoldemCardList();
public:
    virtual void clear();
public:
    int buildWith(CardList& lstHand, CardList& lstCommon);

    int buildWith(CardList& lstCards);

    int getCardType() const { return m_cardtype; }
protected:
    int     m_cardtype;
};

#endif //HOLDEM_CORE_HOLDEMCARDLOGIC_H
