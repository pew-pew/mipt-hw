#include <iostream>
#include <gtest/gtest.h>
#include <string>
#include <deque>
#include <cstdlib>

#include "deque.h"


// force compiler to compile all methods/etc.
template class Deque<int>;

template <typename T>
bool compareDeques(const Deque<T> &d1, const std::deque<T> &d2) {
    if (d1.empty() != d2.empty()) return false;
    if (d1.size() != d2.size()) return false;

    if (d1.front() != d2.front()) return false;
    if (d1.back()  != d2.back())  return false;
    
    for (size_t i = 0; i < d1.size(); i++)
        if (d1[i] != d2[i]) return false;
    return true;
}

template <typename Iter1, typename Iter2>
bool compareIters(Iter1 it1, Iter1 end1, Iter2 it2, Iter2 end2) {
    while (true) {
        bool isEnd1 = (it1 == end1);
        bool isEnd2 = (it2 == end2);
        if (isEnd1 != isEnd2) return false;
        if (isEnd1) break;

        if (*it1 != *it2) return false;
        it1++;
        it2++;
    }
    return true;
}

template <typename T>
bool compareDequesIters(Deque<T> &d1, std::deque<T> &d2) {
    return (compareIters(d1.begin(),   d1.end(),   d2.begin(),   d2.end()) &&
            compareIters(d1.rbegin(),  d1.rend(),  d2.rbegin(),  d2.rend()) &&
            compareIters(d1.cbegin(),  d1.cend(),  d2.cbegin(),  d2.cend()) &&
            compareIters(d1.crbegin(), d1.crend(), d2.crbegin(), d2.crend()));
}

template <typename T>
void applyRandomOp(Deque<T> &d1, std::deque<T> &d2, T valToPush) {
    if (!d2.empty() && rand() % 3 == 0) {
        if (rand() % 2) {
            d1.pop_back();
            d2.pop_back();
        } else {
            d1.pop_front();
            d2.pop_front();
        }
    } else {
        if (rand() % 2) {
            d1.push_back(valToPush);
            d2.push_back(valToPush);
        } else {
            d1.push_front(valToPush);
            d2.push_front(valToPush);
        }
    }
}


TEST(testCaseName, testName) {
    Deque<int> deque;
    std::deque<int> ref;

    for (int i = 0; i < 1000; i++) {
        EXPECT_EQ(deque.empty(), ref.empty());
        applyRandomOp(deque, ref, i);
        
        bool compDeqRes = compareDeques(deque, ref);
        EXPECT_TRUE(compDeqRes);
        if (!compDeqRes) break;

        bool compItRes = compareDequesIters(deque, ref);
        EXPECT_TRUE(compItRes);
        if (!compItRes) break;
    }
}

// TODO: TIME MEASUREMENT TEST

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
