//
// Created by zhs007 on 2017/5/14.
//

#include "holdemcardlogic.h"

void makeHoldemCardTypeStr(std::string& str, int hct) {
    switch (hct) {
        case HOLDEM_CARDTYPE_HIGHCARD:
            str = "HIGHCARD";
            break;
        case HOLDEM_CARDTYPE_PAIR:
            str = "PAIR";
            break;
        case HOLDEM_CARDTYPE_2PAIR:
            str = "2PAIR";
            break;
        case HOLDEM_CARDTYPE_THREE:
            str = "THREE";
            break;
        case HOLDEM_CARDTYPE_STRAIGHT:
            str = "STRAIGHT";
            break;
        case HOLDEM_CARDTYPE_FLUSH:
            str = "FLUSH";
            break;
        case HOLDEM_CARDTYPE_FULLHOUSE:
            str = "FULLHOUSE";
            break;
        case HOLDEM_CARDTYPE_FOUR:
            str = "FOUR";
            break;
        case HOLDEM_CARDTYPE_STRAIGHTFLUSH:
            str = "STRAIGHTFLUSH";
            break;
        case HOLDEM_CARDTYPE_ROYALFLUSH:
            str = "ROYALFLUSH";
            break;
    }
}

HoldemCardList::HoldemCardList()
        : m_cardtype(HOLDEM_CARDTYPE_HIGHCARD) {

}

HoldemCardList::~HoldemCardList() {

}

void HoldemCardList::clear() {
    m_cardtype = HOLDEM_CARDTYPE_HIGHCARD;

    CardList::clear();
}

int HoldemCardList::buildWith(CardList& lstHand, CardList& lstCommon) {
    CardList lstCard;
    lstCard.addCardList(lstHand);
    lstCard.addCardList(lstCommon);

    buildWith(lstCard);
}

int HoldemCardList::buildWith(CardList& lstCard) {
    clear();

    lstCard.sort();

    CardList lstPtEqu(lstCard);
    CardList lstPt[4];

    // 找到4同点数的牌
    lstPtEqu._make_SameRankNums(lstPt[3], 4);

    // 如果有4张牌，剩下的牌里面取最大的
    if (lstPt[3].getCardNums() > 0) {
        addCardList(lstPt[3]);
        addCard(lstPtEqu.getCard(0));

        m_cardtype = HOLDEM_CARDTYPE_FOUR;

        return m_cardtype;
    }

    lstPtEqu._make_SameRankNums(lstPt[2], 3);
    lstPtEqu._make_SameRankNums(lstPt[1], 2);

    // 如果有3张牌，可能有2组
    int p3n = lstPt[2].getCardNums();
    int p2n = lstPt[1].getCardNums();
    if (p3n > 0) {
        // 2组
        if (p3n == 6) {
            addCard(lstPt[2].getCard(0));
            addCard(lstPt[2].getCard(1));
            addCard(lstPt[2].getCard(2));

            addCard(lstPt[2].getCard(3));
            addCard(lstPt[2].getCard(4));

            m_cardtype = HOLDEM_CARDTYPE_FULLHOUSE;

            return m_cardtype;
        }

        if (p2n > 0) {
            addCard(lstPt[2].getCard(0));
            addCard(lstPt[2].getCard(1));
            addCard(lstPt[2].getCard(2));

            addCard(lstPt[1].getCard(0));
            addCard(lstPt[1].getCard(1));

            m_cardtype = HOLDEM_CARDTYPE_FULLHOUSE;

            return m_cardtype;
        }
    }

    CardList lstSPt;

    // 2 + 2 + 2
    if (p2n == 6) {
        addCard(lstPt[1].getCard(0));
        addCard(lstPt[1].getCard(1));

        addCard(lstPt[1].getCard(2));
        addCard(lstPt[1].getCard(3));

        CardInfo& c0 = lstPt[1].getCard(4);
        CardInfo& c1 = lstPtEqu.getCard(0);

        if (c0.rank > c1.rank) {
            addCard(c0);
        }
        else {
            addCard(c1);
        }

        m_cardtype = HOLDEM_CARDTYPE_2PAIR;

        return m_cardtype;
    }

    CardList lstType[4];
    bool isth = false;
    int tht = -1;
    int lastnums = lstCard.getCardNums();

    for (int ct = 0; ct < 4; ++ct) {
        if (lastnums < 5) {
            break;
        }

        int cnum = lstCard.make_Suit(lstType[ct], ct);
        if (cnum >= 5) {
            isth = true;
            tht = ct;

            break;
        }

        lastnums -= cnum;
    }

    if (isth) {
        int ia1 = -1;
        int si1 = lstType[tht]._findStraight(5, ia1);
        if (si1 >= 0) {
            if (ia1 >= 0) {
                addCard(lstType[tht].getCard(si1));
                addCard(lstType[tht].getCard(si1 + 1));
                addCard(lstType[tht].getCard(si1 + 2));
                addCard(lstType[tht].getCard(si1 + 3));
                addCard(lstType[tht].getCard(ia1));

                m_cardtype = HOLDEM_CARDTYPE_STRAIGHTFLUSH;

                return m_cardtype;
            }

            addCard(lstType[tht].getCard(si1));
            addCard(lstType[tht].getCard(si1 + 1));
            addCard(lstType[tht].getCard(si1 + 2));
            addCard(lstType[tht].getCard(si1 + 3));
            addCard(lstType[tht].getCard(si1 + 4));

            if (getCard(0).rank == CARD_RANK_A) {
                m_cardtype = HOLDEM_CARDTYPE_ROYALFLUSH;
            }
            else {
                m_cardtype = HOLDEM_CARDTYPE_STRAIGHTFLUSH;
            }

            return m_cardtype;
        }

        addCard(lstType[tht].getCard(0));
        addCard(lstType[tht].getCard(1));
        addCard(lstType[tht].getCard(2));
        addCard(lstType[tht].getCard(3));
        addCard(lstType[tht].getCard(4));

        m_cardtype = HOLDEM_CARDTYPE_FLUSH;

        return m_cardtype;
    }

    lstCard._make_SingleRank(lstSPt);
    int ia = -1;
    int si = lstSPt._findStraight(5, ia);

    // 顺子
    if (si >= 0) {
        if (ia >= 0) {
            addCard(lstSPt.getCard(si));
            addCard(lstSPt.getCard(si + 1));
            addCard(lstSPt.getCard(si + 2));
            addCard(lstSPt.getCard(si + 3));
            addCard(lstSPt.getCard(ia));

            m_cardtype = HOLDEM_CARDTYPE_STRAIGHT;

            return m_cardtype;
        }

        addCard(lstSPt.getCard(si));
        addCard(lstSPt.getCard(si + 1));
        addCard(lstSPt.getCard(si + 2));
        addCard(lstSPt.getCard(si + 3));
        addCard(lstSPt.getCard(si + 4));

        m_cardtype = HOLDEM_CARDTYPE_STRAIGHT;

        return m_cardtype;
    }

    if (p3n == 3) {
        addCard(lstPt[2].getCard(0));
        addCard(lstPt[2].getCard(1));
        addCard(lstPt[2].getCard(2));

        addCard(lstPtEqu.getCard(0));
        addCard(lstPtEqu.getCard(1));

        m_cardtype = HOLDEM_CARDTYPE_THREE;

        return m_cardtype;
    }

    if (p2n == 4) {
        addCard(lstPt[1].getCard(0));
        addCard(lstPt[1].getCard(1));

        addCard(lstPt[1].getCard(2));
        addCard(lstPt[1].getCard(3));

        addCard(lstPtEqu.getCard(0));

        m_cardtype = HOLDEM_CARDTYPE_2PAIR;

        return m_cardtype;
    }
    else if (p2n == 2) {
        addCard(lstPt[1].getCard(0));
        addCard(lstPt[1].getCard(1));

        addCard(lstPtEqu.getCard(0));
        addCard(lstPtEqu.getCard(1));
        addCard(lstPtEqu.getCard(2));

        m_cardtype = HOLDEM_CARDTYPE_PAIR;

        return m_cardtype;
    }

    m_cardtype = HOLDEM_CARDTYPE_HIGHCARD;

    addCard(lstPtEqu.getCard(0));
    addCard(lstPtEqu.getCard(1));
    addCard(lstPtEqu.getCard(2));
    addCard(lstPtEqu.getCard(3));
    addCard(lstPtEqu.getCard(4));

    return m_cardtype;
}