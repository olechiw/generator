//
// Created by jolechiw on 8/2/23.
//

#include <stdexcept>
#include "TraitGenerator.h"

TraitGeneratorBuilder::TraitGeneratorBuilder(const std::vector<Trait> &traits) {
    for (const auto &trait : traits) {
        _traits[trait.identifier] = trait;
        for (const auto &groupIdentifier : trait.groups) {
            _traitsForGroup[groupIdentifier].insert(trait.identifier);
        }
    }
}


void TraitGeneratorBuilder::addUnionGroup(TraitGroupIdentifier newGroup, const std::vector<TraitGroupIdentifier> &groupsToUnion) {
    for (const auto &group : groupsToUnion) {
        if (!_traitsForGroup.contains(group)) {
            throw std::runtime_error("New Union Group depends on unknown Group");
        }
        _traitsForGroup[newGroup].insert(_traitsForGroup[group].begin(), _traitsForGroup[group].end());
    }
}

void TraitGeneratorBuilder::addIntersectionGroup(TraitGroupIdentifier newGroup, const std::vector<TraitGroupIdentifier> &groupsToIntersect) {
    bool first = true;
    for (const auto &group : groupsToIntersect) {
        if (!_traitsForGroup.contains(group)) {
            throw std::runtime_error("New Union Group depends on unknown Group");
        }
        if (first) {
            _traitsForGroup[newGroup] = _traitsForGroup[group];
            first = false;
        } else {
            for (auto it = _traitsForGroup[newGroup].begin(); it != _traitsForGroup[newGroup].end();) {
                if (!_traitsForGroup[group].contains(*it)) {
                    it = _traitsForGroup[newGroup].erase(it);
                } else {
                    it++;
                }
            }
        }
    }
}

void TraitGeneratorBuilder::addMutuallyExclusive(TraitTag x, TraitTag y) {
    _mutuallyExclusiveTraitTags[x].insert(y);
    _mutuallyExclusiveTraitTags[y].insert(x);
}

void TraitGeneratorBuilder::addMutuallyDependent(TraitTag x, TraitTag y) {
    _mutuallyDependentTraitTags[x].insert(y);
    _mutuallyDependentTraitTags[y].insert(x);
}

TraitGenerator TraitGeneratorBuilder::build() {
    std::unordered_map<TraitGroupIdentifier, TraitGenerator::TraitGroup> groupsOut;

    for (const auto &[groupIdentifier, group] : _traitsForGroup) {
        groupsOut.emplace(groupIdentifier, groupIdentifier);
        for (const auto &trait : group) {
            groupsOut[groupIdentifier].traitPool.insert(trait, _traits[trait].weight);
        }
    }
    return {std::move(_traits), std::move(_mutuallyExclusiveTraitTags), std::move(_mutuallyDependentTraitTags), std::move(groupsOut)};
}

TraitGenerator::TraitGenerator(std::unordered_map<TraitIdentifier, Trait> &&traits,
                               std::unordered_map<TraitTag, std::unordered_set<TraitTag>> &&mutuallyExclusiveTraitTags,
                               std::unordered_map<TraitTag, std::unordered_set<TraitTag>> &&mutuallyDependentTraitTags,
                               std::unordered_map<TraitGroupIdentifier, TraitGroup> &&groups) :
                               _traits{traits},
                               _mutuallyExclusiveTraitTags{mutuallyExclusiveTraitTags},
                               _mutuallyDependentTraitTags{mutuallyDependentTraitTags},
                               _traitGroups{groups}
                               {

}

std::vector<TraitIdentifier> TraitGenerator::generate(TraitGroupIdentifier group, size_t count) {
}

