#pragma once

#include <unordered_map>
#include <functional>
#include <cstddef>
#include <iostream>
#include <memory>

#include "lru_cache.hpp"


namespace Policy
{
    struct LRU_CACHE
    {
        enum {type = 0};
    };

    struct MRU_CACHE 
    {
        enum {type = 1};
    };

    struct LFU_CACHE
    {
        enum {type = 2};
    };

    struct MFU_CACHE
    {
        enum {type = 3};
    };

} // namespace Policy


template<int N, typename... T>
using static_switch = typename std::tuple_element<N, std::tuple<T...> >::type;

template<typename Policy, typename R, typename ...Args>
class my_cache
{
    // TODO: Add more cache policies
    using selected_cache_t = static_switch<Policy::type, lru_cache<R, Args...>>;
    using cache_size_t = std::optional<std::size_t>;
    public:
        explicit my_cache(Policy, std::function<R(Args...)> f, cache_size_t cache_size = std::nullopt) :
            cache_{f, cache_size}
        {
            static_assert(!std::is_void<R>::value,
                        "Return type of the function-to-wrap must not be void!");
        }

        R operator () (Args... arg_list)
        {
            return cache_(arg_list...);
        }
        void flush_cache()
        {
            cache_.flush_cache();
        }

    private:
        selected_cache_t cache_;
};
