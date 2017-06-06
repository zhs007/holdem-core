//
// Created by zhs007 on 2017/6/6.
//

#include "base.h"
#include <cstdlib>

// roll
bool roll(int max, int in) {
    int cur = rand() % max;
    return cur < in;
}