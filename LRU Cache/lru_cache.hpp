#include <unordered_map>
#include <functional>


#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP


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
    using Arguments = std::tuple<Args...>;

    std::function<R(Args...)> func_;
};


#endif
