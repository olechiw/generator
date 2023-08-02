#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <unordered_set>
#include <set>
#include <memory>
#include "RandomSelectionTree.h"
#include "TraitGenerator.h"

// Trait Group
// Gadget = x Trait Groups + Frame(Frame...)

// Traits
// Identifier: UUID
// Base Rarity
// Text



int main() {
    std::cout << "Hello, World!" << std::endl;
    RandomSelectionTree<int> tree;
    tree.insert(5, 1);
    tree.insert(10, 1);
    tree.insert(15, 1);
    tree.insert(3, 2);
    tree.insert(11, 2);
    tree.remove(5);
    std::unordered_map<int, int> counter;
    for (int i = 0; i < 100000; ++i) {
        counter[tree.randomlySelectValue()]++;
    }
    for (auto [key, value] : counter) {
        std::cout << key << ": " << value << std::endl;
    }
    return 0;
}
