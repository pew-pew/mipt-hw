#include <vector>
#include <cstdlib>
#include <iostream>
#include <memory>

#include <gtest/gtest.h>

#include "heap.h"
#include "stupid_heap.h"
#include "binomial_heap.h"
#include "leftist_heap.h"
#include "skew_heap.h"


template <typename T, typename HeapT>
class HeapCollection {
private:
    std::vector<std::unique_ptr<IHeap<T>>> heaps;

public:    
    void addHeap(T key) {
        heaps.push_back(std::unique_ptr<IHeap<T>>(new HeapT()));
        heaps.back()->insert(key);
    }

    void insert(size_t index, T key) {
        assert(index < heaps.size());
        heaps[index]->insert(key);
    }

    T getMin(size_t index) const {
        assert(index < heaps.size());
        return heaps[index]->getMin();
    }

    void extractMin(size_t index) {
        assert(index < heaps.size() && !heaps[index]->empty());
        heaps[index]->extractMin();
        if (heaps[index]->empty())
            heaps.erase(heaps.begin() + index);
    }

    void meld(size_t index1, size_t index2) {
        assert(index1 < heaps.size() && index2 < heaps.size());
        heaps[index1]->meld(std::move(*heaps[index2]));
        if (index1 != index2)
            heaps.erase(heaps.begin() + index2);
    }

    bool empty() const {
        return heaps.empty();
    }

    size_t size() const {
        return heaps.size();
    }
};


enum Op {
    ADD_HEAP = 0,
    INSERT   = 1,
    GET_MIN  = 2,
    EXTRACT  = 3,
    MELD     = 4
};


template <typename HeapA, typename HeapB>
void TestCompareHeaps(size_t iterations) {
    const size_t ops_cnt = 5;

    HeapCollection<int, HeapA> heapsA;
    HeapCollection<int, HeapB> heapsB;
    for (size_t iter = 0; iter < iterations; iter++) {
        ASSERT_EQ(heapsA.size(), heapsB.size());

        //std::cout << iter << std::endl;
        
        int key = 1 + rand() % 100;
        Op op = static_cast<Op>(rand() % ops_cnt);
        if (heapsA.empty() || op == ADD_HEAP) {
            //std::cout << "ADD " << key << std::endl;
            heapsA.addHeap(key);
            heapsB.addHeap(key);
            continue;
        }

        size_t index1 = rand() % heapsA.size();
        size_t index2 = rand() % heapsA.size();
        if (op == INSERT) {
            //std::cout << "INS " << index1 << " " << key << std::endl;
            heapsA.insert(index1, key);
            heapsB.insert(index1, key);
        } else if (op == GET_MIN) {
            //std::cout << "GM " << index1 << std::endl;
            int m1 = heapsA.getMin(index1);
            int m2 = heapsB.getMin(index1);
            ASSERT_EQ(m1, m2);
        } else if (op == EXTRACT) {
            //std::cout << "EX " << index1 << std::endl;
            heapsA.extractMin(index1);
            heapsB.extractMin(index1);
        } else {
            //std::cout << "MELD " << index1 << " " << index2 << std::endl;
            heapsA.meld(index1, index2);
            heapsB.meld(index1, index2);
        }
    }
}


TEST(Heap, BinomialHeap) {
    TestCompareHeaps<StupidHeap<int>, BinomialHeap<int>>(10000);
}

TEST(Heap, LeftistHeap) {
    TestCompareHeaps<StupidHeap<int>, LeftistHeap<int>>(10000);
}

TEST(Heap, SwekHeap) {
    TestCompareHeaps<StupidHeap<int>, SkewHeap<int>>(10000);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
