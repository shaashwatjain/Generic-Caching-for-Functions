#include <unordered_map>
#include <functional>


#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP


template<typename R, typename ...Args>
class lru_cache{
    public:
    lru_cache(std::function<R(Args...)> f) : func_{f} {}
    void operator () (Args...) {
        return;
    }

    private:
    std::function<R(Args...)> func_;
};


#endif
