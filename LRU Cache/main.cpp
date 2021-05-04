#include <iostream>
#include "cache.hpp"
#include "test_functions.hpp"
#include <chrono>

class A {
    int i;
    int b;
  public:
    A(int i, int b): i{i}, b{b} {}
    bool operator == (const A& rhs) const {
        return i == rhs.i && b == rhs.b;
    }
};

template<>
class std::hash<A> {
  public:
    std::size_t operator () (const A& obj) const {
        return 0;
    }
};

template<typename wrapped_func>
void test_case(wrapped_func& _func)
{
    auto start = std::chrono::high_resolution_clock::now();
    test_case_gen(_func);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}

template<typename func>
void test_function(func& _func)
{
    using namespace std;
    {
        cout << "For non-cached function :\n\t";
        test_case(_func);
    }
    {
        my_cache cached_func_lru{_func,policy::LRU_CACHE()};
        cout << "For LRU cached function (unlimited size) :\n\t";
        test_case(cached_func_lru);
    }
    {    
        my_cache cached_func_lru_limit{_func,policy::LRU_CACHE(), cache_size::RESTRICTED<200>()} ;
        cout << "For LRU cached function (limited size) :\n\t";
        test_case(cached_func_lru_limit);
    }
    {
        my_cache cached_func_mru{_func,policy::MRU_CACHE()};
        cout << "For MRU cached function (unlimited size) :\n\t";
        test_case(cached_func_mru);
    }
    {
        my_cache cached_func_mru_limit{_func,policy::MRU_CACHE(),  cache_size::RESTRICTED<200>()};
        cout << "For MRU cached function (limited size) :\n\t";
        test_case(cached_func_mru_limit);
    }
    {
        my_cache cached_func_lfu{_func,policy::LFU_CACHE()};
        cout << "For LFU cached function (unlimited size) :\n\t";
        test_case(cached_func_lfu);
    }
    {    
        my_cache cached_func_lfu_limit{_func,policy::LFU_CACHE(), cache_size::RESTRICTED<200>()} ;
        cout << "For LFU cached function (limited size) :\n\t";
        test_case(cached_func_lfu_limit);
    }
    {
        my_cache cached_func_mfu{_func,policy::MFU_CACHE()};
        cout << "For MFU cached function (unlimited size) :\n\t";
        test_case(cached_func_mfu);
    }
    {    
        my_cache cached_func_mfu_limit{_func,policy::MFU_CACHE(), cache_size::RESTRICTED<200>()} ;
        cout << "For MFU cached function (limited size) :\n\t";
        test_case(cached_func_mfu_limit);
    }
}

int main() {
    std::cout << "\n" << "For Mathematical computation : \n";
    std::function comp{test_case_compute};
    test_function(comp);
    std::cout << "\n\n" << "For heavy header return computation : \n";
    std::function http{http_header_gen};
    test_function(http);
    std::cout << "\n\n" << "For shared pointer return computation : \n";
    std::function http_ptr{http_header_gen_ptr};
    test_function(http_ptr);
    return 0;
}