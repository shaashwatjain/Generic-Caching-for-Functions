#include<bits/stdc++.h>

namespace detailed
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
}

struct TupleHasher
{
    template<typename... T>
    size_t operator()(const std::tuple<T...>& tup)
    {
        size_t hash = 0;
        std::apply(detailed::lazyHasher, std::tuple_cat(std::tuple(0), tup));
        return hash;
    }
};

int main()
{
    return TupleHasher{}(std::tuple(1, 1., 2.f, "xxxx", 100));
}

