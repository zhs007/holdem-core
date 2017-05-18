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

//======================================================================================================================
// 牌型概率
void HoldemCardTypeProb::clear() {
    m_numsTotal = 0;

    memset(m_numsCardType, 0, sizeof(int) * MAX_HOLDEM_CARDTYPE);
}

void HoldemCardTypeProb::addCardTypeNums(int ct, int nums) {
    m_numsCardType[ct] += nums;
    m_numsTotal += nums;
}

void HoldemCardTypeProb::analysisOthers(CardList& lstCards, CardList& lstExclude) {
    clear();

    int nums = lstCards.getCardNums();
    if (nums < 3 || nums > 5) {
        return ;
    }

    if (nums == 3) {
        _analysisOthers_3_foreach(lstCards, lstExclude);
    }

    if (nums == 4) {
        _analysisOthers_4_foreach(lstCards, lstExclude);
    }

    if (nums == 5) {
        _analysisOthers_5_foreach(lstCards, lstExclude);
    }
}

void HoldemCardTypeProb::_analysisOthers_3(CardList& lstCards, CardList& lstExclude) {
    int neednums = 4;
    int curlastnums = CARD_TOTAL_NUMS - lstCards.getCardNums() - lstExclude.getCardNums();
    m_numsTotal = countTotalNums(curlastnums, neednums);

    lstCards.sort();

    // HOLDEM_CARDTYPE_ROYALFLUSH
    // 首先判断皇家同花顺，3张公共牌里，只要出现1张10-A之间的牌就有概率
    CardList lstTA;
    lstCards.make_RankIn(lstTA, CARD_RANK_10, CARD_RANK_A);
    if (lstTA.getCardNums() > 0) {
        for (int suit = CARD_SUIT_SPADES; suit <= CARD_SUIT_DIAMONDS; ++suit) {
            CardList lstTAS;
            lstTA.make_Suit(lstTAS, suit);

            if (lstTAS.getCardNums() > 0) {
                if (!lstExclude.hasSuit_RankIn(suit, CARD_RANK_10, CARD_RANK_A)) {
                    int rn = 5 - lstTAS.getCardNums();
                    int trn = neednums - rn;
                    if (trn == 0) {
                        m_numsCardType[HOLDEM_CARDTYPE_ROYALFLUSH]++;
                    }
                    else if (trn > 0) {
                        m_numsCardType[HOLDEM_CARDTYPE_ROYALFLUSH] += countTotalNums(curlastnums - rn, trn);
                    }
                }
            }
        }
    }

    // HOLDEM_CARDTYPE_STRAIGHTFLUSH
    // 同花顺，算法几乎一样
    for (int rank = CARD_RANK_1; rank < CARD_RANK_10; ++rank) {
        for (int suit = CARD_SUIT_SPADES; suit <= CARD_SUIT_DIAMONDS; ++suit) {
            int cn = lstCards.count_Suit_RankIn(suit, rank, rank + 4);
            if (cn > 0) {
                if (!lstExclude.hasSuit_RankIn(suit, CARD_RANK_10, CARD_RANK_A)) {
                    int rn = 5 - cn;
                    int trn = neednums - rn;
                    if (trn == 0) {
                        m_numsCardType[HOLDEM_CARDTYPE_STRAIGHTFLUSH]++;
                    }
                    else if (trn > 0) {
                        m_numsCardType[HOLDEM_CARDTYPE_STRAIGHTFLUSH] += countTotalNums(curlastnums - rn, trn);
                    }
                }
            }
        }
    }

    // HOLDEM_CARDTYPE_FOUR
    // 4张，只要没有排除的牌都可能出
    for (int rank = CARD_RANK_2; rank < CARD_RANK_A; ++rank) {
        if (!lstExclude.hasRank(rank)) {
            int cn = lstCards.count_Rank(rank);
            int rn = 4 - cn;
            int trn = neednums - rn;
            if (cn == 0) {
                m_numsCardType[HOLDEM_CARDTYPE_FOUR]++;
            }
            else {
                m_numsCardType[HOLDEM_CARDTYPE_STRAIGHTFLUSH] += countTotalNums(curlastnums - rn, trn);
            }
        }
    }

    // HOLDEM_CARDTYPE_FULLHOUSE
    // 葫芦
}

int HoldemCardTypeProb::countTotalNums(int totalnums, int neednums) {
    int64_t n = 1;
    for (int i = 0; i < neednums; ++i) {
        n *= (totalnums - i);
    }

    for (int i = 0; i < neednums; ++i) {
        n /= (neednums - i);
    }

    return n;
}

void HoldemCardTypeProb::_analysisOthers_3_foreach(CardList& lstCards, CardList& lstExclude) {
    CardList lst;

    for (int suit = CARD_SUIT_SPADES; suit <= CARD_SUIT_DIAMONDS; ++suit) {
        for (int rank = CARD_RANK_2; rank <= CARD_RANK_A; ++rank) {
            if (!lstCards.hasCard(rank, suit) && !lstExclude.hasCard(rank, suit)) {
                lst.addCard(rank, suit);
            }
        }
    }

    for (int i0 = 0; i0 < lst.getCardNums() - 3; ++i0) {
        for (int i1 = i0 + 1; i1 < lst.getCardNums() - 2; ++i1) {
            for (int i2 = i1 + 1; i2 < lst.getCardNums() - 1; ++i2) {
                for (int i3 = i2 + 1; i3 < lst.getCardNums(); ++i3) {
                    CardList lstguess;

                    lstguess.addCard(lst.getCard(i0));
                    lstguess.addCard(lst.getCard(i1));
                    lstguess.addCard(lst.getCard(i2));
                    lstguess.addCard(lst.getCard(i3));

                    HoldemCardList hcl;
                    hcl.buildWith(lstCards, lstguess);

                    addCardTypeNums(hcl.getCardType(), 1);
                }
            }
        }
    }
}

void HoldemCardTypeProb::_analysisOthers_4_foreach(CardList& lstCards, CardList& lstExclude) {
    CardList lst;

    for (int suit = CARD_SUIT_SPADES; suit <= CARD_SUIT_DIAMONDS; ++suit) {
        for (int rank = CARD_RANK_2; rank <= CARD_RANK_A; ++rank) {
            if (!lstCards.hasCard(rank, suit) && !lstExclude.hasCard(rank, suit)) {
                lst.addCard(rank, suit);
            }
        }
    }

    for (int i0 = 0; i0 < lst.getCardNums() - 2; ++i0) {
        for (int i1 = i0 + 1; i1 < lst.getCardNums() - 1; ++i1) {
            for (int i2 = i1 + 1; i2 < lst.getCardNums(); ++i2) {
                CardList lstguess;

                lstguess.addCard(lst.getCard(i0));
                lstguess.addCard(lst.getCard(i1));
                lstguess.addCard(lst.getCard(i2));

                HoldemCardList hcl;
                hcl.buildWith(lstCards, lstguess);

                addCardTypeNums(hcl.getCardType(), 1);
            }
        }
    }
}

void HoldemCardTypeProb::_analysisOthers_5_foreach(CardList& lstCards, CardList& lstExclude) {
    CardList lst;

    for (int suit = CARD_SUIT_SPADES; suit <= CARD_SUIT_DIAMONDS; ++suit) {
        for (int rank = CARD_RANK_2; rank <= CARD_RANK_A; ++rank) {
            if (!lstCards.hasCard(rank, suit) && !lstExclude.hasCard(rank, suit)) {
                lst.addCard(rank, suit);
            }
        }
    }

    for (int i0 = 0; i0 < lst.getCardNums() - 1; ++i0) {
        for (int i1 = i0 + 1; i1 < lst.getCardNums(); ++i1) {
            CardList lstguess;

            lstguess.addCard(lst.getCard(i0));
            lstguess.addCard(lst.getCard(i1));

            HoldemCardList hcl;
            hcl.buildWith(lstCards, lstguess);

            addCardTypeNums(hcl.getCardType(), 1);
        }
    }
}

void HoldemCardTypeProb::output() {
    for (int i = HOLDEM_CARDTYPE_ROYALFLUSH; i >= HOLDEM_CARDTYPE_HIGHCARD; --i) {
        if (m_numsCardType[i] > 0) {
            std::string strct;
            makeHoldemCardTypeStr(strct, i);
            printf("%s - %.2f(%d/%d)\n", strct.c_str(), 100.0f * m_numsCardType[i] / m_numsTotal, m_numsCardType[i], m_numsTotal);
        }
    }
}

//======================================================================================================================
// 德州牌
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

    return buildWith(lstCard);
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
