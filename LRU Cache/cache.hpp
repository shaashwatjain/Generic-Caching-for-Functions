#pragma once

#include <unordered_map>
#include <functional>
#include <cstddef>
#include <iostream>
#include <memory>
#include <concepts>

#include "lru_cache.hpp"
#include "mru_cache.hpp"

namespace policy
{
    struct CACHE_POLICY {};
    struct LRU_CACHE : CACHE_POLICY
    {
        enum {type = 0};
    };

    struct MRU_CACHE : CACHE_POLICY
    {
        enum {type = 1};
    };

    struct LFU_CACHE : CACHE_POLICY
    {
        enum {type = 2};
    };

    struct MFU_CACHE : CACHE_POLICY
    {
        enum {type = 3};
    };

} // namespace Policy

namespace cache_size
{
    struct CACHE_SIZE {};
    struct UNLIMITED : CACHE_SIZE
    {
        enum { type = 0 };
    };

    template<std::size_t Size>
    struct RESTRICTED : CACHE_SIZE
    {
        enum { cache_size_ = Size, type = 1 };
        RESTRICTED()
        {
            static_assert(Size > 0,
                          "Size of cache must be greater than 0");
        }
    };
}

template<typename T>
concept Hashable_Type = requires(T Args) {
    { std::hash<T>{}(Args) } -> std::convertible_to<std::size_t>;
};

// template<typename T>
// using Hashable = std::is_convertible_v<std::hash<T>(), std::size_t>;

template<typename Arg, typename... Args>
    requires Hashable_Type<Arg>
constexpr bool Hashable() 
{ 
    if constexpr(sizeof...(Args) != 0U)
        Hashable<Args...>();
    return true;
}


template<typename... T>
concept Hashable_Types = Hashable<T...>();

template<typename... T>
concept Arguments = sizeof...(T) != 0;

// template<typename Arg, typename... Args>
// using Hashable_Types = typename Hashable<Arg>() && Hashable_Types<Args...>;

template<int N, typename... T>
using static_switch = typename std::tuple_element<N, std::tuple<T...> >::type;

template<typename T>
concept Cache_Policy = std::is_base_of_v<policy::CACHE_POLICY, T>;

template<typename T>
concept Cache_Size = std::is_base_of_v<cache_size::CACHE_SIZE, T>;

template<typename T>
concept Non_Void_Return = !std::is_void_v<T>;


template<typename Policy, typename Size = cache_size::UNLIMITED, typename R = int, typename ...Args>
    requires Cache_Policy<Policy> && Cache_Size<Size> && Non_Void_Return<R> && Arguments<Args...> && Hashable_Types<Args...>
class my_cache
{
    // TODO: Add more cache policies
    using selected_cache_t = static_switch<Policy::type,
                                           lru_cache<Size, R, Args...>,
                                           mru_cache<Size, R, Args...>
                                           >;
    public:
        explicit my_cache(std::function<R(Args...)> f, Policy, Size&& obj = Size()) :
            cache_(f, std::move(obj))
        {}

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
