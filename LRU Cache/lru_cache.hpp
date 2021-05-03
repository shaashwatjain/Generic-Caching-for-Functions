#pragma once

#include <unordered_map>
#include <functional>
#include <cstddef>
#include <iostream>
#include <memory>

#include "lru_utility.hpp"

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

template<typename R, typename ...Args>
class lru_cache {
    public:
        explicit lru_cache(std::function<R(Args...)> f, std::optional<int> cache_size = std::nullopt) :
            cache_{}, func_{f}, cache_size_{cache_size} 
        {
            if (cache_size.has_value())
                cache_.reserve(cache_size.value());
        }
        R operator () (Args... arg_list)
        {
            auto tuple_ele = std::make_tuple(arg_list...);

            if (cache_.find(tuple_ele) != std::end(cache_)) {
                std::cout << "Cache Hit!" << std::endl;
                ++cache_[tuple_ele].second;
                return cache_[tuple_ele].first;
            }

            cache_[tuple_ele] = Values{func_(arg_list...), 1};
            std::cout << "Cache Miss" << std::endl;
            std::cout << "Value added to cache" << std::endl;
            return cache_[tuple_ele].first;
        }
        void flush_cache()
        {
            cache_.clear();
        }


    private:
        using counter_t = std::size_t;
        using Arguments = std::tuple<Args...>;
        using Values = std::pair<R, counter_t>;
        using cache_t = std::unordered_map<Arguments, Values, internal_cache::tuple_hasher>;
        cache_t cache_;
        std::function<R(Args...)> func_;
        std::optional<int> cache_size_;
};


template<int N, typename... T>
using static_switch = typename std::tuple_element<N, std::tuple<T...> >::type;

template<typename Policy, typename R, typename ...Args>
class my_cache
{
    using selected_cache_t = static_switch<Policy::type, lru_cache<R, Args...>>;
    public:
        explicit my_cache(Policy, std::function<R(Args...)> f) :
            cache_{f}
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

#if 0
template<typename R, typename ...Args>
class cache {
    public:
        cache(std::function<R(Args...)> f) : func_{f} {
            static_assert(!std::is_void<R>::value,
                        "Return type of the function-to-wrap must not be void!");
        }

        virtual R operator () (Args... arg_list) = 0;
        virtual void flush_cache() = 0;
        virtual ~cache();

    private:
        std::function<R(Args...)> func_;
};


template<typename Policy, typename R, typename... Args>
class cache: public Policy<R, Args...> {
  public:
    cache(std::function<R(Args...)> f) : func_{f}, cache{} {
        static_assert(!std::is_void<R>::value,
                      "Return type of the function-to-wrap must not be void!");
    }
    auto operator () (Args... arg_list) {
        auto tuple_ele = std::make_tuple(arg_list...);

        if (cache.find(tuple_ele) != std::end(cache)) {
            std::cout << "Cache Hit!" << std::endl;
            ++cache[tuple_ele].second;
            return cache[tuple_ele].first;
        }

        cache[tuple_ele] = Values{func_(arg_list...), 1};
        std::cout << "Cache Miss" << std::endl;
        std::cout << "Value added to cache" << std::endl;
        return cache[tuple_ele].first;
    }

    void Flush_Lru() {
        cache.clear();

        // for(auto it : cache)
        //     printf("%lld", it.second.second);
    }


  private:
    using counter_t = std::size_t;
    using Arguments = std::tuple<Args...>;
    using Values = std::pair<R, counter_t>;
    using cache_t = std::unordered_map<Arguments, Values, Internal_LRU::tuple_hasher>;
    std::function<R(Args...)> func_;
    cache_t cache;
};

#endif
