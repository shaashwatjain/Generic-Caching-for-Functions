#include <iostream>
#include "cache.hpp"


std::vector<long long int> prime_factor(long long int n) {
    std::vector<long long int> v;

    while(n % 2 == 0) {
        v.push_back(2);
        n /= 2;
    }

    for (long long int i = 3; i <= sqrt(n); i = i + 2)
    {
        while (n % i == 0)
        {
             v.push_back(i);
            n = n/i;
        }
    }
 
    if (n > 2)
        v.push_back(n);

    return v; // Returning the vector

}

#if 1
int main() {
    std::cout << "\n" << "Usage-1\n" << "For Prime Factors with vector return : \n";
    #if 0
    std::cout << "\n" << "Not Cached\n";
    for(long long int i = 100'000'000'000'000; i < 100'000'000'001'000; ++i)
    {            
        prime_factor(i);  // Uncached function call
    }
    for(long long int i = 100'000'000'000'000; i < 100'000'000'001'000; ++i)
    { 
        prime_factor(i);  // Uncached function call
    }
    #endif

    // for(auto& i: prime_factor(100))
    //     std::cout << i << "\t";
    // std::cout << "\n";

    #if 1
    std::cout << "\n" << "Cached\n";
    std::function func{prime_factor};
    my_cache func_cached{func, policy::LRU_CACHE()}; // Unlimited size cache
    for(long long int i = 100'000'000'000'000; i < 100'000'000'001'000; ++i)
    {     
        func_cached(i);  // Cached function call
    }
    for(long long int i = 100'000'000'000'000; i < 100'000'000'001'000; ++i)
    {
        func_cached(i);  // Cached function call
    }

    // for(auto& i: func(100))
    //     std::cout << i << "\t";
    // std::cout << "\n";
    #endif

}
#endif








#if 0
#include <random>
int main() {
    std::cout << "\n" << "Usage-1\n" << "For Prime Factors with vector return : \n";
    #if 1
    std::cout << "\n" << "Not Cached\n";
    for(long long int i = 0; i < 1000; ++i)
    {            
        prime_factor(rand()%100 + 100'000'000'000);  // Uncached function call
    }
    #endif

    #if 0
    std::cout << "\n" << "Cached\n";
    std::function func{prime_factor};
    my_cache func_cached{func, policy::LRU_CACHE(), cache_size::RESTRICTED<50>()}; // Unlimited size cache
    for(long long int i = 0; i < 1000; ++i)
    {
        func_cached(rand()%100 + 100'000'000'000);  // Cached function call
    }
    #endif

}
#endif