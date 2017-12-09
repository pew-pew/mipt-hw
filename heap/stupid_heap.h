#include <set>
#include <iterator>

#include "heap.h"

template <typename T>
class StupidHeap : public IHeap<T> {
private:
    std::multiset<T> data;

public:
    StupidHeap(): data() {}

    virtual void insert(T val) override {
        data.insert(val);
    }

    virtual T getMin() const override {
        return *(data.begin());
    }

    virtual void extractMin() override {
        data.erase(data.begin());
    }

    virtual void meld(IHeap<T> &&other) override {
        meld(dynamic_cast<StupidHeap&&>(other));
    }

    void meld(StupidHeap&& other) {
        if (this == &other) return;
        data.insert(
                std::make_move_iterator(other.data.begin()),
                std::make_move_iterator(other.data.end())
        );
        other.data.clear();
    }

    virtual bool empty() const override {
        return data.empty();
    }
};
