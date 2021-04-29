#include <unordered_map>
#include <functional>


#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP


namespace Internal_LRU
{
    auto lazyHasher = [](size_t hash, auto&&... values)
    {
        auto lazyCombiner = [&hash](auto&& val)
        {
            hash ^= std::hash<std::decay_t<decltype(val)>>{}(val) + 0Xeeffddcc + (hash << 5) + (hash >> 3);
        };
        (lazyCombiner(std::forward<decltype(values)>(values)), ...);
        return hash;
    };

    struct tuple_hasher
    {
        template<typename... T>
        std::size_t operator()(const std::tuple<T...>& tup)
        {
            size_t hash = 0;
            std::apply(lazyHasher, std::tuple_cat(std::tuple(0), tup));
            return hash;
        }
    };
}




template<typename R, typename... Args>
class lru_cache{
    public:
    lru_cache(std::function<R(Args...)> f) : func_{f} {
        static_assert(!std::is_void<R>::value,
                  "Return type of the function-to-wrap must not be void!");
    }
    void operator () (Args...) {
        return;
    }

    private:
    using counter_t = size_t
    using Arguments = std::tuple<Args...>;
    using Values = std::pair<R, counter_t>;
    using lru_cache_t = std::unordered_map<Arguments,Values,Internal_LRU::tuple_hasher>;
    std::function<R(Args...)> func_;
};


#endif
