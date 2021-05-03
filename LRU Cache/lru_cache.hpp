#pragma once

#include "lru_utility.hpp"
#include <list>


template<typename R, typename ...Args>
class lru_cache {
    public:
        explicit lru_cache(std::function<R(Args...)> f, std::optional<int> cache_size = std::nullopt) :
            func_{f}, cache_size_{cache_size}, cache_{}, usage_tracker_{}
        {
            if (cache_size.has_value())
                cache_.reserve(cache_size.value());
        }
        R operator () (Args... arg_list)
        {
            const char Placeholder = ' ';
            auto tuple_ele = std::make_tuple(arg_list...);

            if (cache_.find(tuple_ele) != std::end(cache_)) {
                std::list<char>::const_iterator &const_it_ = cache_[tuple_ele].second;
                std::cout << "Cache Hit!" << std::endl;
                usage_tracker_.erase(const_it_);
                usage_tracker_.push_back(Placeholder);
                const_it_ = std::end(usage_tracker_);
            }
            else {
                // TODO: Eviction code
                usage_tracker_.push_back(Placeholder);
                cache_[tuple_ele] = Values{func_(arg_list...), std::end(usage_tracker_)};
                std::cout << "Cache Miss" << std::endl;
                std::cout << "Value added to cache" << std::endl;
            }

            return cache_[tuple_ele].first;
        }
        void flush_cache()
        {
            cache_.clear();
        }


    private:
        using counter_t = std::size_t;
        using Arguments = std::tuple<Args...>;
        using Values = std::pair<R, std::list<char>::const_iterator>;
        using cache_t = std::unordered_map<Arguments, Values, internal_cache::tuple_hasher>;
        std::function<R(Args...)> func_;
        std::optional<int> cache_size_;
        cache_t cache_;
        std::list<char> usage_tracker_;
};