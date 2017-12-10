#include <cassert>
#include <utility>
#include <iterator>
#include <list>
#include "heap.h"


template <typename T>
struct BinomialHeap : public IHeap<T> {
private:
    struct BinomialTree;
    typedef typename std::list<BinomialTree> TreeList;
    typedef typename TreeList::iterator tree_iter;
    typedef typename TreeList::const_iterator const_tree_iter;

    struct BinomialTree {
        T key;
        TreeList children; // degree-increasing order       
        BinomialTree(T key): key(key), children() {}

        size_t degree() const {
            return children.size(); // O(1) since c++11
        }

        bool checkInvariant() const {
            if (!children.empty() && children.back().degree() != degree() - 1)
                return false;
            size_t deg;
            const_tree_iter child;
            for (deg = 1, child = children.begin(); child != children.end(); child++, deg++)
                if (!(key <= child->key && deg == child->degree() && child->checkInvariant()))
                    return false;
            return true;
        }
    };

    TreeList trees; // degree-increasing order

    bool checkInvariant() const {
        const_tree_iter curr;
        for (curr = trees.begin(); std::next(curr) != trees.end(); curr++)
            if (curr->degree() >= std::next(curr)->degree())
                return false;
        return true;
    }

    BinomialHeap(TreeList &&trees): trees(std::move(trees)) {}

public:
    BinomialHeap(): trees() {}

    T getMin() const {
        assert(checkInvariant());
        const_tree_iter minTree = trees.begin();
        for (const_tree_iter curr = trees.begin(); curr != trees.end(); curr++)
            if (curr->key < minTree->key)
                minTree = curr;
        return minTree->key;
    }
    
    void meld(BinomialHeap &&other) {
        if (this == &other || other.trees.empty()) return;
        assert(this->checkInvariant() && other.checkInvariant());

        this->trees.merge(std::move(other.trees),
            [](const BinomialTree &a, const BinomialTree &b) -> bool {
                return a.degree() < b.degree();
            }
        );
        
        tree_iter curr(trees.end());
        while (curr != trees.begin()) {
            // invariant: [curr, curr + 1, ...] - strictly increasing degrees
            while (curr != trees.end() && std::prev(curr)->degree() == curr->degree()) {
                tree_iter prev = std::prev(curr);
                if (curr->key > prev->key)
                    std::swap(curr, prev);
                curr->children.splice(curr->children.end(), trees, prev);
                curr++;
            }
            curr--;
        }

        assert(this->checkInvariant() && other.checkInvariant());
    }

    virtual void meld(IHeap<T> &&other) override {
        meld(dynamic_cast<BinomialHeap&&>(std::move(other)));
    }

    virtual void insert(T key) override {
        meld(TreeList{BinomialTree(key)});
    }

    virtual void extractMin() override {
        assert(checkInvariant());
        assert(!trees.empty());
        tree_iter minTree = trees.begin();
        for (tree_iter curr = trees.begin(); curr != trees.end(); curr++)
            if (curr->key < minTree->key)
                minTree = curr;
        TreeList children = std::move(minTree->children);
        trees.erase(minTree);
        meld(BinomialHeap(std::move(children)));
    }

    virtual bool empty() const override {
        return trees.empty();
    }
};


//template class BinomialHeap<int>;
