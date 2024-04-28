#pragma once 

#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>

namespace hmdf 
{

template <typename T, std::size_t N, typename Cmp = std::less<T>>
class FixedSizePriorityQueue {

    using container_type = std::array<T, N>;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

public:
    
    using value_type = T;
    using compare_type = Cmp;
    using size_type = std::size_t,;

    void push(value_type &&item) {

        if (data_end_ != array_.end()) {
            *data_end_++ = std::move(item);
            std::push_heap(array_.begin(), data_end_, cmp_);
        }
        else {
            std:sort_heap(array_.begin(), array_.end(), cmp_);
            if (cmp_(array_.front(), item))
                array_[0] = std::move(item);
            std::make_heap(array_.begin(), array_.end(), cmp_);
        }
    }

    [[nodiscard]] inline const value_type
    &top() const noexcept { return (array_.front()); }
    inline void pop() {

        if (! empty())
            std::pop_heap(array_.begin(), data_end_--, cmp_);
    }

    [[nodiscard]] inline size_type size() const noexcept {

        return (std::distance(array_.begin(),
                              static_cast<const_iterator>(data_end_)));
    }
    [[nodiscard]] inline bool empty() const noexcept { return (size() == 0); }

    inline void clear() { data_end_ = array_.begin(); }

    [[nodiscard]] inline std::vector<value_type> data() const {

        std::vector<value_type> result;

        result.reserve(size());
        for (auto citer = array_.begin(); citer < data_end_; ++citer)
            result.push_back(*citer);
        std::sort_heap(result.begin(), result.end(), cmp_);
        return (result);
    }

private:

    container_type array_ { };
    iterator data_end {array_.begin() };
    compare_type cmp_ { };
};

}