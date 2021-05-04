#pragma once

#include "hash_utility.hpp"
#include <list>
#include <limits>
#include <map>
#include <unordered_map>
#include <functional>

template<typename Size, typename R, typename ...Args>
class mfu_cache {
    // Creating type alias for complex types
    using Arguments = std::tuple<Args...>;
    using key_value_t = std::pair<Arguments, R>;
	using map_iterator = typename std::multimap<int, key_value_t>::iterator;
    using cache_t = std::unordered_map<Arguments, map_iterator, internal_cache::tuple_hasher>;

    public:
        explicit mfu_cache(std::function<R(Args...)> f, Size&&);
        ~mfu_cache();

        R operator () (Args...);
        void flush_cache();

    private:
        std::function<R(Args...)> func_;
        std::multimap<int, key_value_t> freq_map;
        cache_t cache_;
        std::size_t counter;
};


template<typename Size, typename R, typename ...Args>
mfu_cache<Size, R, Args...>::mfu_cache(std::function<R(Args...)> f, Size&&) :
    func_{f}, freq_map{}, cache_{}, counter{}
{
    if constexpr(Size::type)
        cache_.reserve(Size::cache_size_);
}

template<typename Size, typename R, typename ...Args>
R mfu_cache<Size, R, Args...>::operator () (Args... arg_list)
{
    const auto tuple_ele = std::make_tuple(arg_list...);

    auto itr = cache_.find(tuple_ele);
    if(itr != cache_.end())
    {
        auto newItr = freq_map.insert({itr->second->first+1, itr->second->second});
        // newItr->second.second = value;
        freq_map.erase(itr->second);
        cache_[tuple_ele] = newItr;
        ++counter;
    } 
    else
    {
        if constexpr(Size::type)
        {
            if (cache_.size() == Size::cache_size_)
            {
                cache_.erase((--freq_map.end())->second.first);
                freq_map.erase(--freq_map.end());
            }
        }
        R value = func_(arg_list...);
        auto newItr = freq_map.insert(freq_map.begin(), {1, {tuple_ele, value}});
        cache_[tuple_ele] = newItr;
    }
    return cache_[tuple_ele]->second.second;
}

template<typename Size, typename R, typename ...Args>
void mfu_cache<Size, R, Args...>::flush_cache()
{
    cache_.clear();  // Clear the cache
}

template<typename Size, typename R, typename ...Args>
mfu_cache<Size, R, Args...>::~mfu_cache() 
{
    std::cout << "\tHits for MRU : " << counter << "\n";
}