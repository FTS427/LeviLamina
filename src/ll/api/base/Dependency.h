#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ll {
template <class T>
class Dependency {
public:
    std::unordered_map<T, std::unordered_set<T>> data;
    std::unordered_map<T, size_t>                sizes;

    struct SortResult {
        std::vector<T> sorted;
        std::vector<T> unsorted;

        [[nodiscard]] constexpr bool hasCycles() const { return !unsorted.empty(); }
    };

    [[nodiscard]] bool contains(T const& node) const { return data.contains(node); }

    [[nodiscard]] bool emplace(T node) {
        if (contains(node)) {
            return false;
        }
        data.emplace(std::move(node), {});
        return true;
    }
    [[nodiscard]] bool contains(T const& node, T const& dependency) const {
        if (!contains(dependency)) {
            return false;
        }
        return data.at(dependency).contains(node);
    }

    [[nodiscard]] bool emplace(T const& node, T const& dependency) {
        if (dependency != node) {
            auto& dependents = data[dependency];
            if (dependents.find(node) == dependents.end()) {
                dependents.insert(node);
                sizes[node]++;
                return true;
            }
        }
        return false;
    }
    [[nodiscard]] bool emplace(T const& node, std::unordered_set<T> const& dependencies) {
        for (auto& dependency : dependencies) {
            if (!emplace(node, dependency)) {
                return false;
            }
        }
        return true;
    }
    void clear() { data.clear(); }

    [[nodiscard]] SortResult sort() const {
        std::vector<T>                sorted;
        std::vector<T>                unsorted;
        std::unordered_map<T, size_t> csize(sizes);
        for (auto& [node, dependents] : data) {
            csize[node] = dependents.size();
            if (dependents.size() == 0) {
                sorted.push_back(node);
            }
        }
        for (size_t i = 0; i < sorted.size(); i++) {
            for (const auto& node : data.at(sorted[i])) {
                if (--csize[node] == 0) {
                    sorted.push_back(node);
                }
            }
        }
        for (auto& [node, size] : csize) {
            if (size != 0) {
                unsorted.push_back(node);
            }
        }
        return {sorted, unsorted};
    }
};
} // namespace ll