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

class A {
    int i; int b;
    public:
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

void f_void() {}
long long f_int(int x) { return x * x; }
float f_float(int x, int y) { return 0.f; }
float test(A u) {return 0.f;}
// struct X { int f(int x, float y) { return 0; } };

int main() {

    A n{}, m{};
    n = m;
    std::function<decltype(test)> o{test};
    // std::function<decltype(f_float)> x{f_float};
    auto p = lru_cache{o};
    p(n);
    p(m);

    // ----------------------
    std::function<decltype(f_int)> y{f_int};
    // std::function<decltype(f_float)> x{f_float};
    auto z = lru_cache{y};
    // lru_cache{x};

    for (int i = 5000; i < 10000; ++i)
        // f_int(i);
        z(i) ;

    z.Flush_Lru();
    std::cout << "here" << '\n';

    for (int i = 5000; i < 10000; ++i)
        // f_int(i);
        z(i) ;

    return 0;
}
