//
// Created by zhs007 on 2017/5/14.
//

#include "cardlogic.h"
#include <stdlib.h>

bool greater4sort(const CardInfo& left, const CardInfo& right) {
    if (left.rank > right.rank) {
        return true;
    }

    if (left.rank == right.rank) {
        if (left.suit > right.suit) {
            return true;
        }
    }

    return false;
}

CardList::CardList() {
}

CardList::CardList(const CardList& lstCard) {
    cloneWith(lstCard);
}

CardList::~CardList() {
}

void CardList::makeStr(std::string& str) {
    str.clear();

    for (int i = 0; i < (int)m_lst.size(); ++i) {
        if (i > 0) {
            str += ",";
        }

        char strci[3];
        m_lst[i].makeString(strci);

        str += strci;
    }
}

void CardList::addCard(int rank, int suit) {
    CardInfo c(rank, suit);
    m_lst.push_back(c);
}

void CardList::addCard(CardInfo& ci) {
    m_lst.push_back(ci);
}

void CardList::addCardList(CardList& cl) {
    for (int i = 0; i < (int)cl.m_lst.size(); ++i) {
        addCard(cl.m_lst[i]);
    }
}

void CardList::cloneWith(const CardList& lstCard) {
    m_lst.clear();

    for (const_iterator it = lstCard.m_lst.begin(); it != lstCard.m_lst.end(); ++it) {
        m_lst.push_back(*it);
    }
}

void CardList::clear() {
    m_lst.clear();
}

void CardList::sort() {
    std::sort(m_lst.begin(), m_lst.end(), greater4sort);
}

bool CardList::hasRank(int rank) {
    for (int i = 0; i < (int)m_lst.size(); ++i) {
        if (m_lst[i].rank == rank) {
            return true;
        }
    }

    return false;
}

bool CardList::hasSuit(int suit) {
    for (int i = 0; i < (int)m_lst.size(); ++i) {
        if (m_lst[i].suit == suit) {
            return true;
        }
    }

    return false;
}

bool CardList::hasCard(int rank, int suit) {
    for (int i = 0; i < (int)m_lst.size(); ++i) {
        if (m_lst[i].rank == rank && m_lst[i].suit == suit) {
            return true;
        }
    }

    return false;
}

// 查找同点数相等的牌，譬如4个相等的，返回第一个的索引，如果找不到，返回-1
// 这里必须保证牌是从大到小排序后的
int CardList::_findSameRankNums(int nums) {
    if (nums >= 2) {
        int max = (int)m_lst.size();
        for (int i = 0; i <= max - nums; ++i) {
            if (m_lst[i].rank == m_lst[i + nums - 1].rank) {
                return i;
            }
        }
    }

    return -1;
}

// 就是取同点数的牌，如果找到了，会把源队列里面的牌删除掉
void CardList::_make_SameRankNums(CardList& dest, int nums) {
    dest.clear();

    int curi = _findSameRankNums(nums);
    while (curi >= 0) {
        iterator b = m_lst.begin();
        iterator e = m_lst.begin();

        std::advance(b, curi);
        std::advance(e, curi + nums);

        dest.m_lst.insert(dest.m_lst.end(), b, e);

        m_lst.erase(b, e);

        curi = _findSameRankNums(nums);
    }
}

// 查找花色
int CardList::make_Suit(CardList& dest, int suit) {
    dest.clear();

    int max = (int)m_lst.size();

    for (int i = 0; i < max; ++i) {
        if (suit == m_lst[i].suit) {
            dest.m_lst.push_back(m_lst[i]);
        }
    }

    return (int)dest.m_lst.size();
}

// 查找顺子，返回第一个的索引，如果找不到，返回-1
// 这里必须保证牌是从大到小排序，且同点数只有1张
int CardList::_findStraight(int nums, int& ia) {
    ia = -1;

    if (nums >= 2) {
        int max = (int)m_lst.size();
        for (int i = 0; i <= max - nums; ++i) {
            if (m_lst[i].rank == m_lst[i + nums - 1].rank + nums - 1) {
                return i;
            }
        }

        // 判断54321
        if (nums == 5 && m_lst[0].rank == CARD_RANK_A && max >= 5) {
            ia = 0;

            if (m_lst[max - 1].rank == 2 && m_lst[max - 4].rank == 5) {
                return max - 4;
            }
        }
    }

    return -1;
}

// 就是同点数只保留一张，大到小
void CardList::_make_SingleRank(CardList& dest) {
    dest.clear();

    int max = (int)m_lst.size();
    int lastpt = -1;

    for (int i = 0; i < max; ++i) {
        if (lastpt != m_lst[i].rank) {
            dest.m_lst.push_back(m_lst[i]);

            lastpt = m_lst[i].rank;
        }
    }
}

// 统计在区间内牌的数量
// 用于顺子检查，传入应该是同点数仅有一张的序列
int CardList::_countCardNums_RankIn(int beginrank, int endrank) {
    int nums = 0;
    for (int i = 0; i < m_lst.size(); ++i) {
        if (m_lst[i].rank >= beginrank && m_lst[i].rank <= endrank) {
            ++nums;
        }
        else if (beginrank == CARD_RANK_1 && m_lst[i].rank == CARD_RANK_A) {
            ++nums;
        }
    }

    return nums;
}

// 统计同点数牌的数量
int CardList::countCardNums_Rank(int rank) {
    int nums = 0;
    for (int i = 0; i < m_lst.size(); ++i) {
        if (m_lst[i].rank == rank) {
            ++nums;
        }
    }

    return nums;
}

// 取这个区间内，差的点数，花色不处理
// 用于顺子缺牌的处理，传入应该是同点数仅有一张的序列
// 注意：不能处理缺多余2张牌的局面！
int CardList::_getRank_RankIn(CardInfo& c0, CardInfo& c1, int beginrank, int endrank) {
    int nums = 0;

    c0.clear();
    c1.clear();

    for (int i = beginrank; i < endrank; ++i) {
        if (!hasRank(i)) {
            if (nums == 0) {
                c0.rank = i;
                ++nums;
            }
            else {
                c1.rank = i;
                ++nums;

                return nums;
            }
        }
    }

    return nums;
}

// 取这个点数里，缺的花色牌
// 用于同点数缺牌的情况
// 注意：不能处理缺多余2张牌的局面！
int CardList::_getSuit_Rank(CardInfo& c0, CardInfo& c1, int rank) {
    int nums = 0;

    c0.clear();
    c1.clear();

    for (int i = 0; i < 4; ++i) {
        if (!hasCard(rank, i)) {
            if (nums == 0) {
                c0.suit = i;
                ++nums;
            }
            else {
                c1.suit = i;
                ++nums;

                return nums;
            }
        }
    }

    return nums;
}

void CardList::buildTotalCardList() {
    clear();

    for (int t = CARD_SUIT_SPADES; t <= CARD_SUIT_DIAMONDS; ++t) {
        for (int p = CARD_RANK_2; p <= CARD_RANK_A; ++p) {
            CardInfo c(p, t);
            m_lst.push_back(c);
        }
    }
}

void CardList::swap(int i, int j) {
    if (i == j) {
        return ;
    }

    if (i < 0 || i >= m_lst.size()) {
        return ;
    }

    if (j < 0 || j >= m_lst.size()) {
        return ;
    }

    std::swap(m_lst[i].rank, m_lst[j].rank);
    std::swap(m_lst[i].suit, m_lst[j].suit);
}

// 洗牌
void CardList::shuffle(int nums) {
    for (int i = 0; i < nums / 2; ++i) {
        int i0 = rand() % m_lst.size();
        int i1 = rand() % m_lst.size();

        swap(i0, i1);
    }

    for (int i = 0; i < m_lst.size(); ++i) {
        int i1 = rand() % m_lst.size();

        swap(i, i1);
    }

    for (int i = 0; i < nums / 2; ++i) {
        int i0 = rand() % m_lst.size();
        int i1 = rand() % m_lst.size();

        swap(i0, i1);
    }
}

int CardList::getCardNums() {
    return m_lst.size();
}

CardInfo& CardList::getCard(int index) {
    return m_lst[index];
}