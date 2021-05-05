#include <iostream>
#include "cache.hpp"


long long int gcd(long long int n1, long long int n2)
{
    while(n1 != n2)
    {
        if(n1 > n2)
            n1 -= n2;
        else
            n2 -= n1;
    }
    return n1;
}

#if 1
int main() {
    std::cout << "\n" << "Usage-2\n" << "For GCD with Scalar return : \n";
    
    #if 0
    std::cout << "\n" << "Not Cached\n";
    for(int i = 1'000'000; i < 1'000'200; ++i)
    {
        gcd(i, i+1);  // Uncached function call
    }
    for(int i = 1'000'000; i < 1'000'200; ++i)
    {
        gcd(i, i+1);  // Uncached function call
    }
    #endif

    #if 1
    std::cout << "\n" << "Cached\n";
    std::function func{gcd};
    my_cache func_cached{func, policy::MRU_CACHE(), cache_size::RESTRICTED<150>()}; // Unlimited size cache
    for(int i = 1'000'000; i < 1'000'200; ++i)
    {
        func_cached(i, i+1);  // Cached function call
    }
    for(int i = 1'000'000; i < 1'000'200; ++i)
    {
        func_cached(i, i+1);  // Cached function call
    }
    #endif

    return 0;
}
#endif









#if 0
#include <random>
int main() {
    std::cout << "\n" << "Usage-2\n" << "For GCD with Scalar return : \n";
    srand(1);
    
    #if 0
    std::cout << "\n" << "Not Cached\n";
    for(long long int i = 0; i < 900; ++i)
    { 
        gcd(rand()%200 + 100'000'000, rand()%200 + 100'000'000);  // Uncached function call
    }
    #endif

    #if 1
    std::cout << "\n" << "Cached\n";
    std::function func{gcd};
    my_cache func_cached{func, policy::LRU_CACHE()}; // Unlimited size cache
    for(long long int i = 0; i < 900; ++i)
    {
        func_cached(rand()%200 + 100'000'000, rand()%200 + 100'000'000);  // Cached function call
    }
    #endif
}
#endif