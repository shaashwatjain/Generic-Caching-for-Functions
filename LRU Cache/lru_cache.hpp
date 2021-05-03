#include "lru_utility.hpp"

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