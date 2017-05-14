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

#endif //HOLDEM_CORE_CARDLOGIC_H
