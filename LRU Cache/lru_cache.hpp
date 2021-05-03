#pragma once

#include "hash_utility.hpp"
#include <list>


template<typename Cache_Size, typename R, typename ...Args>
class lru_cache {
    // Creating type alias for complex types
    using Arguments = std::tuple<Args...>;
    using Values = std::pair<R, typename std::list<Arguments>::iterator>;

    // Creating type alias for complex types
    using cache_t = std::unordered_map<Arguments, Values, internal_cache::tuple_hasher>;
    using list_it_t = std::list<Arguments>::iterator;
    using list_const_it_t = std::list<Arguments>::const_iterator;

    public:
        explicit lru_cache(std::function<R(Args...)> f, Cache_Size&&);
        R operator () (Args...);
        void flush_cache();

    private:
        std::function<R(Args...)> func_;
        cache_t cache_;
        std::list<Arguments> usage_tracker_;
};


template<typename Cache_Size, typename R, typename ...Args>
lru_cache<Cache_Size, R, Args...>::lru_cache(std::function<R(Args...)> f, Cache_Size&&) :
    func_{f}, cache_{}, usage_tracker_{}
{
    if constexpr(Cache_Size::type)
        cache_.reserve(Cache_Size::cache_size_);
}

template<typename Cache_Size, typename R, typename ...Args>
R lru_cache<Cache_Size, R, Args...>::operator () (Args... arg_list)
{
    const auto tuple_ele = std::make_tuple(arg_list...);

    // Cache Hit
    if (cache_.find(tuple_ele) != std::end(cache_)) {
        #if DEBUG
            std::cout << "Cache Hit!" << std::endl;
        #endif
        if constexpr(Cache_Size::type) {
            list_it_t &list_it_ = cache_[tuple_ele].second;
            usage_tracker_.erase(list_it_);
            usage_tracker_.push_back(tuple_ele);
            list_it_ = --std::end(usage_tracker_);
        }
    }
    else {
        if constexpr(Cache_Size::type) {
            // Eviction code
            if (cache_.size() == Cache_Size::cache_size_) {
                list_const_it_t lru_const_it_ = std::begin(usage_tracker_);
                cache_.erase(*lru_const_it_);  // Eviction by Key
                usage_tracker_.erase(lru_const_it_);  // Eviction by Iterator
            }
            usage_tracker_.push_back(tuple_ele);
        }

        if constexpr(Cache_Size::type)
            cache_[tuple_ele] = Values{func_(arg_list...), --std::end(usage_tracker_)};
        else
            cache_[tuple_ele] = Values{func_(arg_list...), std::end(usage_tracker_)};
        
        #if DEBUG
            std::cout << "Cache Miss" << std::endl;
            std::cout << "Value added to cache" << std::endl;
        #endif
    }

    return cache_[tuple_ele].first;
}

template<typename Cache_Size, typename R, typename ...Args>
void lru_cache<Cache_Size, R, Args...>::flush_cache()
{
    cache_.clear();  // Clear the cache

     // Clear the bookkeeping 
    if constexpr(Cache_Size::type) { usage_tracker_.clear(); }
}
