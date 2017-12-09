#include <utility>

#include "heap.h"

template <typename T>
class SkewHeap : public IHeap<T> {
private:
    struct SkewTree {
        T key;
        SkewTree *left;
        SkewTree *right;

        SkewTree(T key): key(key), left(nullptr), right(nullptr) {}
    };

    static void deleteTree(SkewTree *tree) {
        if (tree == nullptr) return;
        deleteTree(tree->left);
        deleteTree(tree->right);
        delete tree;
    }

    static SkewTree* merge(SkewTree *left, SkewTree *right) {
        if (left == nullptr) return right;
        if (right == nullptr) return left;
        assert(left != right);

        if (left->key > right->key)
            std::swap(left, right);

        left->right = merge(left->right, right);
        std::swap(left->left, left->right);
        return left;
    }

    SkewTree *root;
    
public:
    SkewHeap(SkewHeap &&other): root(other.root) {
        other.root = nullptr;
    }

    SkewHeap(const SkewHeap&) = delete;
    SkewHeap& operator=(const SkewHeap &other) = delete;

    SkewHeap(): root(nullptr) {}

    ~SkewHeap() {
        deleteTree(root);
        root = nullptr;
    }

    virtual T getMin() const override {
        assert(root != nullptr);
        return root->key;
    }

    virtual void meld(IHeap<T> &&other) override {
        meld(dynamic_cast<SkewHeap&&>(other));
    }

    void meld(SkewHeap &&other) {
        if (this == &other) return;
        root = merge(root, other.root);
        other.root = nullptr;
    }

    virtual void insert(T key) override {
        root = merge(root, new SkewTree(key));
    }

    virtual void extractMin() override {
        assert(root != nullptr);
        SkewTree *oldRoot = root;
        root = merge(root->left, root->right);
        delete oldRoot;
    }

    bool empty() const {
        return (root == nullptr);
    }
};
