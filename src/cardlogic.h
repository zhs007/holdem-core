//
// Created by zhs007 on 2017/5/14.
//

#ifndef HOLDEM_CORE_CARDLOGIC_H
#define HOLDEM_CORE_CARDLOGIC_H

#include <cstdint>
#include <vector>

const int8_t CARD_SUIT_SPADES   =   0;
const int8_t CARD_SUIT_HEARTS   =   1;
const int8_t CARD_SUIT_CLUBS    =   2;
const int8_t CARD_SUIT_DIAMONDS =   3;

const int8_t CARD_RANK_1        =   1;
const int8_t CARD_RANK_2        =   2;
const int8_t CARD_RANK_3        =   3;
const int8_t CARD_RANK_4        =   4;
const int8_t CARD_RANK_5        =   5;
const int8_t CARD_RANK_6        =   6;
const int8_t CARD_RANK_7        =   7;
const int8_t CARD_RANK_8        =   8;
const int8_t CARD_RANK_9        =   9;
const int8_t CARD_RANK_10       =   10;
const int8_t CARD_RANK_J        =   11;
const int8_t CARD_RANK_Q        =   12;
const int8_t CARD_RANK_K        =   13;
const int8_t CARD_RANK_A        =   14;

const int CARD_TOTAL_NUMS       = 52;
const int CARD_RANK_NUMS        = 13;

const char CARDSTR_SUIT[] = "1234";
const char CARDSTR_RANK[] = "0123456789TJQKA";

struct CardInfo {
    int8_t rank;
    int8_t suit;

    CardInfo() {
        clear();
    }

    CardInfo(int rank, int suit) {
        this->rank = rank;
        this->suit = suit;
    }

    void clear() {
        rank = -1;
        suit = -1;
    }

    void makeString(char str[3]) {
        str[0] = CARDSTR_SUIT[suit];
        str[1] = CARDSTR_RANK[rank];
        str[2] = '\0';
    }

    bool isEqu(const CardInfo& right) {
        return rank == right.rank && suit == right.suit;
    }

    int getCardIndex() {
        return suit * CARD_RANK_NUMS + rank - 2;
    }

    void setWithStr(const char* str) {
        suit = str[0] - '1';
        if (str[1] == 'A' || str[1] == 'a') {
            rank = CARD_RANK_A;
        }
        else if (str[1] == 'K' || str[1] == 'k') {
            rank = CARD_RANK_K;
        }
        else if (str[1] == 'Q' || str[1] == 'q') {
            rank = CARD_RANK_Q;
        }
        else if (str[1] == 'J' || str[1] == 'j') {
            rank = CARD_RANK_K;
        }
        else if (str[1] == 'T' || str[1] == 't') {
            rank = CARD_RANK_10;
        }
        else {
            rank = str[1] - '0';
        }
    }
};

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

class CardList{
    typedef std::vector<CardInfo>::iterator iterator;
    typedef std::vector<CardInfo>::const_iterator const_iterator;
public:
    CardList();
    CardList(const CardList& lstCard);
    virtual ~CardList();
public:
    virtual void clear();

    int getCardNums();

    CardInfo& getCard(int index);
public:
    void addCard(int rank, int suit);

    void addCard(CardInfo& ci);

    void addCardList(CardList& cl);

    void cloneWith(const CardList& lstCard);

    void sort();

    bool hasRank(int rank);

    bool hasSuit(int ct);

    bool hasCard(int rank, int suit);

    // 查找同点数相等的牌，譬如4个相等的，返回第一个的索引，如果找不到，返回-1
    // 这里必须保证牌是从大到小排序后的
    int _findSameRankNums(int nums);

    // 就是取同点数的牌，如果找到了，会把源队列里面的牌删除掉
    void _make_SameRankNums(CardList& dest, int nums);

    // 查找花色
    int make_Suit(CardList& dest, int suit);

    // 查找顺子，返回第一个的索引，如果找不到，返回-1
    // 这里必须保证牌是从大到小排序，且同点数只有1张
    int _findStraight(int nums, int& ia);

    // 就是同点数只保留一张，大到小
    void _make_SingleRank(CardList& dest);

    // 统计在区间内牌的数量
    // 用于顺子检查，传入应该是同点数仅有一张的序列
    int _countCardNums_RankIn(int beginrank, int endrank);

    // 统计同点数牌的数量
    int countCardNums_Rank(int rank);

    // 取这个区间内，差的点数，花色不处理
    // 用于顺子缺牌的处理，传入应该是同点数仅有一张的序列
    // 注意：不能处理缺多余2张牌的局面！
    int _getRank_RankIn(CardInfo& c0, CardInfo& c1, int beginpt, int endpt);

    // 取这个点数里，缺的花色牌
    // 用于同点数缺牌的情况
    // 注意：不能处理缺多余2张牌的局面！
    int _getSuit_Rank(CardInfo& c0, CardInfo& c1, int rank);

    void buildTotalCardList();

    void swap(int i, int j);

    // 洗牌
    void shuffle(int nums);
public:
    std::vector<CardInfo>       m_lst;
};


#endif //HOLDEM_CORE_CARDLOGIC_H
