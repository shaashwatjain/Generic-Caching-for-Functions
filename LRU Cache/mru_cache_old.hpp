#pragma once

#include "hash_utility.hpp"
#include <list>


template<typename R, typename ...Args>
class mru_cache {
    // Creating type alias for complex types
    using Arguments = std::tuple<Args...>;
    using Values = std::pair<R, typename std::list<Arguments>::iterator>;

    // Creating type alias for complex types
    using cache_size_t = std::optional<std::size_t>;
    using cache_t = std::unordered_map<Arguments, Values, internal_cache::tuple_hasher>;
    using list_it_t = std::list<Arguments>::iterator;
    using list_const_it_t = std::list<Arguments>::const_iterator;

    public:
        explicit mru_cache(std::function<R(Args...)> f, cache_size_t = std::nullopt);
        R operator () (Args...);
        void flush_cache();

    private:
        std::function<R(Args...)> func_;
        cache_size_t cache_size_;
        cache_t cache_;
        std::list<Arguments> usage_tracker_;
};


template<typename R, typename ...Args>
mru_cache<R, Args...>::mru_cache(std::function<R(Args...)> f, cache_size_t cache_size) :
    func_{f}, cache_size_{cache_size}, cache_{}, usage_tracker_{}
{
    if (cache_size_.has_value())
        cache_.reserve(cache_size_.value());
}

template<typename R, typename ...Args>
R mru_cache<R, Args...>::operator () (Args... arg_list)
{
    const auto tuple_ele = std::make_tuple(arg_list...);

    // Cache Hit
    if (cache_.find(tuple_ele) != std::end(cache_)) {
        #if DEBUG
            std::cout << "Cache Hit!" << std::endl;
        #endif
        if (cache_size_.has_value()) {
            list_it_t &list_it_ = cache_[tuple_ele].second;
            usage_tracker_.erase(list_it_);
            usage_tracker_.push_back(tuple_ele);
            list_it_ = --std::end(usage_tracker_);
        }
    }
    else {
        if (cache_size_.has_value()) {
            // Eviction code
            if (cache_.size() == cache_size_.value()) {
                list_const_it_t lru_const_it_ = --std::end(usage_tracker_);
                cache_.erase(*lru_const_it_);  // Eviction by Key
                usage_tracker_.erase(lru_const_it_);  // Eviction by Iterator
            }
            usage_tracker_.push_back(tuple_ele);
        }
        cache_[tuple_ele] = Values{func_(arg_list...), --std::end(usage_tracker_)};
        
        #if DEBUG
            std::cout << "Cache Miss" << std::endl;
            std::cout << "Value added to cache" << std::endl;
        #endif
    }

    return cache_[tuple_ele].first;
}

template<typename R, typename ...Args>
void mru_cache<R, Args...>::flush_cache()
{
    cache_.clear();  // Clear the cache

     // Clear the bookkeeping 
    if (!cache_size_.has_value()) { usage_tracker_.clear(); }
}
