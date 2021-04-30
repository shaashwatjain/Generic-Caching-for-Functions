#include <unordered_map>
#include <functional>
#include <cstddef>
#include <iostream>
#include <memory>

#include "lru_utility.hpp"

#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP


template<typename R, typename... Args>
class lru_cache{
    public:
    lru_cache(std::function<R(Args...)> f) : func_{f}, cache{} {
        static_assert(!std::is_void<R>::value,
                  "Return type of the function-to-wrap must not be void!");
    }
    auto operator () (Args... arg_list) {
        auto tuple_ele = std::make_tuple(arg_list...);
        if (cache.find(tuple_ele) != std::end(cache)) {
            // std::cout << "Cache Hit!" << std::endl;
            ++cache[tuple_ele].second;
            return cache[tuple_ele].first;
        }

        cache[tuple_ele] = Values{func_(arg_list...), 1};
        // std::cout << "Cache Miss" << std::endl;
        // std::cout << "Value added to cache" << std::endl;
        return cache[tuple_ele].first;
    }

    private:
    using counter_t = std::size_t;
    using Arguments = std::tuple<Args...>;
    using Values = std::pair<R, counter_t>;
    using lru_cache_t = std::unordered_map<Arguments,Values,Internal_LRU::tuple_hasher>;
    std::function<R(Args...)> func_;
    lru_cache_t cache;
};


#endif
