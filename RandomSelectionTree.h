//
// Created by jolechiw on 8/2/23.
//

#ifndef GENERATOR_RANDOMSELECTIONTREE_H
#define GENERATOR_RANDOMSELECTIONTREE_H

#include <memory>
#include <cassert>
#include <random>

template<typename T, typename W = uint32_t, typename A = std::allocator<T>>
class RandomSelectionTree {
    struct Node;
    using NodePointer = std::shared_ptr<Node>;
    struct Node {
        T value;
        W weight;
        W cumulativeWeight;
        NodePointer left;
        NodePointer right;
    };
public:
    RandomSelectionTree() : root { nullptr }, rd{}, rng{rd()} {

    }

    void insert(const T& value, W weight) {
        if (!root) {
            root = makeNode(value, weight);
        } else {
            insertHelper(root, value, weight);
        }
    }

    void remove(const T& value) {
        root = removeHelper(root, value);
        recalculateCumulativeSum(root);
    }

    template <typename F>
    void removeIf(F predicate) {

    }

    T randomlySelectValue() {
        assert(root);

        std::uniform_int_distribution<W> distribution(1, root->cumulativeWeight);
        W value = distribution(rng);

        return randomlySelectHelper(root, value);
    }

    RandomSelectionTree(RandomSelectionTree<T, W, A> &&other) noexcept : rd { std::move(other.rd) }, rng { std::move(other.rng)} {
        this->root = other.root;
        this->allocator = other.allocator;
        other.root = nullptr;
    }

    RandomSelectionTree(const RandomSelectionTree<T, W, A> &other) {
        *this = other;
    }

    RandomSelectionTree &operator=(const RandomSelectionTree<T, W, A> &other) {
        if (&other == this) {
            return *this;
        }
        this->root = copyHelper(other.root);
        return *this;
    }

private:
    T randomlySelectHelper(NodePointer n, W targetWeight) {
        W leftWeight = n->left ? n->left->cumulativeWeight : 0;
        if (targetWeight <= leftWeight) {
            return randomlySelectHelper(n->left, targetWeight);
        } else if (targetWeight <= (leftWeight + n->weight)) {
            return n->value;
        } else {
            return randomlySelectHelper(n->right, targetWeight - leftWeight - n->weight);
        }
    }

    NodePointer removeHelper(NodePointer n, const T& value) {
        if (!n) {
            return n;
        }
        // O(n) search because we sort by weight not value
        n->left = removeHelper(n->left, value);
        n->right = removeHelper(n->right, value);
        W leftWeight = n->left ? n->left->cumulativeWeight : 0;
        W rightWeight = n->right ? n->right->cumulativeWeight : 0;
        n->cumulativeWeight = leftWeight + rightWeight + n->weight;
        if (n->value != value) {
            return n;
        }

        if (!n->left) {
            return n->right;
        }
        if (!n->right) {
            return n->left;
        }
        NodePointer successorParent = n;
        NodePointer successor = n->right;
        while (successor->left) {
            successorParent = successor;
            successor = successor->left;
        }
        if (successorParent != n) {
            successorParent->left = successor->right;
        } else {
            successorParent->right = successor->right;
        }
        n->value = successor->value;
        n->weight = successor->weight;
        return n;
    }

    NodePointer copyHelper(NodePointer other) {
        if (!other) {
            return nullptr;
        }

        NodePointer n = makeNode(other->value, other->weight);
        n->cumulativeWeight = other->cumulativeWeight;
        n->left = copyHelper(other->left);
        n->right = copyHelper(other->right);

        return n;
    }

    void recalculateCumulativeSum(NodePointer n) {
        if (!n) {
            return;
        }
        recalculateCumulativeSum(n->left);
        recalculateCumulativeSum(n->right);
        W leftWeight = n->left ? n->left->cumulativeWeight : 0;
        W rightWeight = n->right ? n->right->cumulativeWeight : 0;
        n->cumulativeWeight = leftWeight + rightWeight + n->weight;
    }


    void insertHelper(NodePointer n, const T& value, W weight) {
        assert(n);

        if (weight < n->weight) {
            if (!n->left) {
                n->left = makeNode(value, weight);
            } else {
                insertHelper(n->left, value, weight);
            }
        } else {
            if (!n->right) {
                n->right = makeNode(value, weight);
            } else {
                insertHelper(n->right, value, weight);
            }
        }
        n->cumulativeWeight += weight;
    }

    NodePointer makeNode(const T& value, W weight) {
        return std::allocate_shared<Node>(allocator, value, weight, weight, nullptr, nullptr);
    }

    A allocator;
    NodePointer root;

    std::random_device rd;
    std::mt19937 rng;
};
#endif //GENERATOR_RANDOMSELECTIONTREE_H
