//
// Created by jolechiw on 8/2/23.
//

#ifndef GENERATOR_TRAITGENERATOR_H
#define GENERATOR_TRAITGENERATOR_H

#include <stdint.h>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "RandomSelectionTree.h"

class TraitGenerator;

enum class TraitIdentifier : uint64_t {};
enum class TraitGroupIdentifier : uint64_t {};
enum class TraitTag : uint64_t {};

struct Trait {
    using Weight = uint32_t;

    TraitIdentifier identifier;
    Weight weight;
    std::vector<TraitTag> tags;
    std::vector<TraitGroupIdentifier> groups;
};

class TraitGeneratorBuilder {
public:
    explicit TraitGeneratorBuilder(const std::vector<Trait> &traits);
    void addIntersectionGroup(TraitGroupIdentifier newGroup, const std::vector<TraitGroupIdentifier> &groupsToIntersect);
    void addUnionGroup(TraitGroupIdentifier newGroup, const std::vector<TraitGroupIdentifier> &groupsToUnion);
    void addMutuallyExclusive(TraitTag x, TraitTag y);
    void addMutuallyDependent(TraitTag x, TraitTag y);

    TraitGenerator build();

private:
    std::unordered_map<TraitIdentifier, Trait> _traits;

    std::unordered_map<TraitTag, std::unordered_set<TraitTag>> _mutuallyExclusiveTraitTags;
    std::unordered_map<TraitTag, std::unordered_set<TraitTag>> _mutuallyDependentTraitTags;

    std::unordered_map<TraitGroupIdentifier, std::unordered_set<TraitIdentifier>> _traitsForGroup;
};

class TraitGenerator {
public:
    // Support some kind of modifiers here?
    std::vector<TraitIdentifier> generate(TraitGroupIdentifier group, size_t count);
private:
    friend TraitGenerator TraitGeneratorBuilder::build();
    struct TraitGroup {
        TraitGroupIdentifier identifier;
        RandomSelectionTree<TraitIdentifier> traitPool;
    };
    TraitGenerator(
            std::unordered_map<TraitIdentifier, Trait> &&traits,
            std::unordered_map<TraitTag, std::unordered_set<TraitTag>> &&mutuallyExclusiveTraitTags,
            std::unordered_map<TraitTag, std::unordered_set<TraitTag>> &&mutuallyDependentTraitTags,
            std::unordered_map<TraitGroupIdentifier, TraitGroup> &&groups);

    std::unordered_map<TraitIdentifier, Trait> _traits;
    std::unordered_map<TraitTag, std::unordered_set<TraitTag>> _mutuallyExclusiveTraitTags;
    std::unordered_map<TraitTag, std::unordered_set<TraitTag>> _mutuallyDependentTraitTags;
    std::unordered_map<TraitGroupIdentifier, TraitGroup> _traitGroups;

};


#endif //GENERATOR_TRAITGENERATOR_H
