#ifndef CUBIMG_ALGOSET_H
#define CUBIMG_ALGOSET_H

#include <array>
#include <string_view>

namespace cubimg::ALGOSET {

struct Algo {
    std::string_view name;
    std::string_view algo;
    std::string_view setup;
    std::string_view category;
    std::string_view comment;

    constexpr Algo(
        std::string_view name_,
        std::string_view algo_,
        std::string_view setup_ = "",
        std::string_view category_ = "",
        std::string_view comment_ = ""
    ) : name(name_), algo(algo_),
        setup(setup_), category(category_), comment(comment_) {}
    constexpr void setSetup(std::string_view setup_) { setup = setup_; }
    constexpr void setCategory(std::string_view category_) { category = category_; }
    constexpr void setComment(std::string_view comment_) { comment = comment_; }
};

template<size_t M>
class AlgoSet {
public:
    constexpr AlgoSet(
        std::string_view algoset_name,
        const std::array<Algo, M>& algos
    ) : name(algoset_name), algo_set(algos) {}
    constexpr std::string_view getName() const { return name; }

    // Total number of algorithms in the algo set.
    constexpr size_t size() const { return M; }
    constexpr auto begin() const { return algo_set.begin(); }
    constexpr auto end() const { return algo_set.end(); }

private:
    std::array<Algo, M> algo_set;
    std::string_view name;
};

}

#endif  // CUBIMG_ALGOSET_H