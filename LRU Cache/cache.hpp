#pragma once

#include <unordered_map>
#include <functional>
#include <cstddef>
#include <iostream>
#include <memory>


#include "lru_cache.hpp"
#include "mru_cache.hpp"

namespace policy
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

namespace cache_size
{
    struct UNLIMITED
    {
        enum { type = 0 };
    };

    template<std::size_t Size>
    struct RESTRICTED
    {
        enum { cache_size_ = Size, type = 1 };
        RESTRICTED()
        {
            static_assert(Size > 0,
                          "Size of cache must be greater than 0");
        }
    };
}


template<int N, typename... T>
using static_switch = typename std::tuple_element<N, std::tuple<T...> >::type;

template<typename Policy, typename Cache_Size = cache_size::UNLIMITED, typename R = int, typename ...Args>
class my_cache
{
    // TODO: Add more cache policies
    using selected_cache_t = static_switch<Policy::type,
                                           lru_cache<Cache_Size, R, Args...>,
                                           mru_cache<Cache_Size, R, Args...>
                                           >;
    public:
        explicit my_cache(std::function<R(Args...)> f, Policy, Cache_Size&& obj = Cache_Size()) :
            cache_(f, std::move(obj))
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
