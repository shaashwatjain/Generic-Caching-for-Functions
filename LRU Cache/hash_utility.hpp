#pragma once

#include <iostream>
#include <tuple>

namespace internal_cache
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
        std::size_t operator()(const std::tuple<T...>& tup) const
        {
            size_t hash = 0;
            std::apply(lazyHasher, std::tuple_cat(std::tuple(0), tup));
            return hash;
        }
    };
}
