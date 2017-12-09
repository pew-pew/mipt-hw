#include <utility>

#include "heap.h"

template <typename T>
class LeftistHeap : public IHeap<T> {
private:
    struct LeftistTree {
        T key;
        LeftistTree *left;
        LeftistTree *right;
        size_t dist;

        LeftistTree(T key): key(key), left(nullptr), right(nullptr), dist(1) {}
    };

    static void deleteTree(LeftistTree *tree) {
        if (tree == nullptr) return;
        deleteTree(tree->left);
        deleteTree(tree->right);
        delete tree;
    }

    static size_t dist(const LeftistTree *tree) {
        return (tree == nullptr ? 0 : tree->dist);
    }

    static LeftistTree* merge(LeftistTree *left, LeftistTree *right) {
        if (left == nullptr) return right;
        if (right == nullptr) return left;
        assert(left != right);

        if (left->key > right->key)
            std::swap(left, right);

        left->right = merge(left->right, right);
        if (dist(left->left) < dist(left->right))
            std::swap(left->left, left->right);
        left->dist = std::min(dist(left->left), dist(right->right)) + 1;
        return left;
    }

    LeftistTree *root;
    
public:
    LeftistHeap(LeftistHeap &&other): root(other.root) {
        other.root = nullptr;
    }

    LeftistHeap(const LeftistHeap&) = delete;
    LeftistHeap& operator=(const LeftistHeap &other) = delete;

    LeftistHeap(): root(nullptr) {}

    ~LeftistHeap() {
        deleteTree(root);
        root = nullptr;
    }

    virtual T getMin() const override {
        assert(root != nullptr);
        return root->key;
    }

    virtual void meld(IHeap<T> &&other) override {
        meld(dynamic_cast<LeftistHeap&&>(other));
    }

    void meld(LeftistHeap &&other) {
        if (this == &other) return;
        root = merge(root, other.root);
        other.root = nullptr;
    }

    virtual void insert(T key) override {
        root = merge(root, new LeftistTree(key));
    }

    virtual void extractMin() override {
        assert(root != nullptr);
        LeftistTree *oldRoot = root;
        root = merge(root->left, root->right);
        delete oldRoot;
    }

    bool empty() const {
        return (root == nullptr);
    }
};
