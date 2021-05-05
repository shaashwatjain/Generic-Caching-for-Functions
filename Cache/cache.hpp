#pragma once

#include <unordered_map>
#include <functional>
#include <cstddef>
#include <iostream>
#include <memory>
#include <concepts>

#include "lru_cache.hpp"
#include "mru_cache.hpp"
#include "lfu_cache.hpp"
#include "mfu_cache.hpp"
#include "random_cache.hpp"

#include "cache_policy.hpp"
#include "cache_size_options.hpp"


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


template<typename Arg, typename... Args>
    requires std::equality_comparable<Arg>
constexpr bool Equality_Comparable() 
{ 
    if constexpr(sizeof...(Args) != 0U)
        Equality_Comparable<Args...>();
    return true;
}

template<typename... T>
concept Equality_Comparable_Types = Equality_Comparable<T...>();

template<typename... T>
concept Arguments = sizeof...(T) != 0;

// template<typename Arg, typename... Args>
// using Hashable_Types = typename Hashable<Arg>() && Hashable_Types<Args...>;

template<int N, typename... T>
using static_switch = typename std::tuple_element<N, std::tuple<T...> >::type;

template<typename T>
concept Cache_Policy = std::is_base_of_v<policy::CACHE_POLICY, T> && 
                        (!std::is_same_v<policy::CACHE_POLICY, T>);

template<typename T>
concept Cache_Size = std::is_base_of_v<cache_size::CACHE_SIZE, T> && 
                        (!std::is_same_v<cache_size::CACHE_SIZE, T>);

template<typename T>
concept Non_Void_Return = !std::is_void_v<T>;


template<typename Policy, typename Size = cache_size::UNLIMITED, typename R = int, typename ...Args>
    requires Cache_Policy<Policy> && Cache_Size<Size> && Non_Void_Return<R> && Arguments<Args...> && Equality_Comparable_Types<Args...> && Hashable_Types<Args...>
class my_cache
{
    using selected_cache_t = static_switch<Policy::type,
                                           lru_cache<Size, R, Args...>,
                                           mru_cache<Size, R, Args...>,
                                           lfu_cache<Size, R, Args...>,
                                           mfu_cache<Size, R, Args...>,
                                           random_cache<Size, R, Args...>
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
