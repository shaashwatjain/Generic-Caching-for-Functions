#pragma once

#include "hash_utility.hpp"
#include <list>
#include <random>


template<typename Size, typename R, typename ...Args>
class random_cache {
    // Creating type alias for complex types
    using Arguments = std::tuple<Args...>;
    using Values = R;

    // Creating type alias for complex types
    using cache_t = std::unordered_map<Arguments, Values, internal_cache::tuple_hasher>;
    using vector_it_t = std::list<Arguments>::iterator;
    using vector_const_it_t = std::list<Arguments>::const_iterator;

    public:
        explicit random_cache(std::function<R(Args...)> f, Size&&);
        R operator () (Args...);
        void flush_cache();
        ~random_cache();
    private:
        std::function<R(Args...)> func_;
        cache_t cache_;
        std::vector<Arguments> usage_tracker_;
        std::random_device rand_gen_; 
        int count;
};


template<typename Size, typename R, typename ...Args>
random_cache<Size, R, Args...>::random_cache(std::function<R(Args...)> f, Size&&) :
    func_{f}, cache_{}, usage_tracker_{}, rand_gen_{}, count{}
{
    if constexpr(Size::type)
        cache_.reserve(Size::cache_size_);
}

template<typename Size, typename R, typename ...Args>
R random_cache<Size, R, Args...>::operator () (Args... arg_list)
{
    const auto tuple_ele = std::make_tuple(arg_list...);

    // Cache Hit
    if (cache_.find(tuple_ele) != std::end(cache_)) {
        #if DEBUG
            std::cout << "Cache Hit!" << std::endl;
        #endif
        ++count;
    }
    else {
        if constexpr(Size::type) {
            // Eviction code
            if (cache_.size() == Size::cache_size_) {
                vector_const_it_t victim_it_ = std::begin(usage_tracker_) + rand_gen_() % usage_tracker_.size();
                cache_.erase(*victim_it_);  // Eviction by Key
                usage_tracker_.erase(victim_it_);  // Eviction by Iterator
            }
            usage_tracker_.push_back(tuple_ele);
        }

        cache_[tuple_ele] = Values{func_(arg_list...)};

        #if DEBUG
            std::cout << "Cache Miss" << std::endl;
            std::cout << "Value added to cache" << std::endl;
        #endif
    }

    return cache_[tuple_ele].first;
}

template<typename Size, typename R, typename ...Args>
void random_cache<Size, R, Args...>::flush_cache()
{
    cache_.clear();  // Clear the cache

     // Clear the bookkeeping
    if constexpr(Size::type) { usage_tracker_.clear(); }
}

template<typename Size, typename R, typename ...Args>
random_cache<Size, R, Args...>::~random_cache()
{
    std::cout << "Number of hits: " << count << std::endl;
}