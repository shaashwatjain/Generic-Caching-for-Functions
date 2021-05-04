#include <iostream>
#include"cache.hpp"
#include <chrono>

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

void f_void() {}
long long f_int(int x) { return x * x; }
float f_float(int x, int y) { return 0.f; }
float test(A u) {return 0.f;}
// struct X { int f(int x, float y) { return 0; } };

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    A n{1, 2}, m{3, 4}, l{5, 6};
    // n = m;
    std::function o{test};
    std::function x{f_float};
    // cache<decltype(o()), > &p {new lru_cache{o}};
    my_cache cached_o{o, policy::LRU_CACHE()};
    // my_cache cached_o{o, policy::LRU_CACHE(), cache_size::RESTRICTED<2>()};
    // my_cache cached_o{o, policy::MRU_CACHE()};
    // my_cache cached_o{o, policy::MRU_CACHE(), cache_size::RESTRICTED<2>()};
    cached_o(n);  // Miss
    cached_o(m);  // Miss
    cached_o(l);  // Miss
    // ----
    cached_o(m);  // Miss
    cached_o(n);  // Hit
    cached_o.flush_cache();
    cached_o(m);  // Miss
    cached_o(m);  // Hit
    // delete p;
    // // ----------------------
    // std::function<decltype(f_int)> y{f_int};
    // // std::function<decltype(f_float)> x{f_float};
    // auto z = lru_cache{y};
    // // lru_cache{x};

    // for (int i = 5000; i < 10000; ++i)
    //     // f_int(i);
    //     z(i) ;

    // // for flushing (clearing) the cache
    // z.Flush_Lru();
    // std::cout << "here" << '\n';

    // for (int i = 5000; i < 10000; ++i)
    //     // f_int(i);
    //     z(i) ;

    /*  ##################################################
        # uncomment this to run the computation testcase #
        ################################################## */
    // long long int val;
    // std::vector<long long int>v;
    // for (int i = 0; i < 100000; ++i) {
    //     // f_int(i);
    //     val = i;
    //     val <<= 50;
    //     val += 1e8;
    //     v.push_back(val);
    //     z(val);
    //     // f_int(val);
    // }
    // // for(int i =0;i<100;i++)
    // // {
    // //     std::cout << v[i]<< '\t';
    // // }
    // // std::cout << '\n';
    //
    // // z.Flush_Lru();
    //
    //
    // for (int i = 0; i<100000; ++i) {
    //     // val = i;
    //     // val<<=50;
    //     // val+=1e8;
    //     // f_int(val);
    //     z(v[i]);
    // }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";

    return 0;
}
