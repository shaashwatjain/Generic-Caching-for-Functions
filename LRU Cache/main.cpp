#include <iostream>
#include"lru_cache.hpp"

// template<typename R, typename... A>
// R ret(R(*)(A...));

// template<typename C, typename R, typename... A>
// R ret(R(C::*)(A...));

// template<typename T>
// void print_name(T f)
// {
//     printf("%s\n", typeid(decltype(ret(f))).name());
// }


void f_void() {}
int f_int(int x) { return 0; }
float f_float(int x, int y) { return 0.f; }
// struct X { int f(int x, float y) { return 0; } };

int main()
{

    std::function<int(int)> y{f_int};
    std::function<decltype(f_float)> x{f_float};
    auto z = lru_cache{y};
    lru_cache{x};
    std::cout << z(10) << std::endl;
    std::cout << z(10) << std::endl;

    return 0;
}
