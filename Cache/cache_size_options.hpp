#include <cstddef>


namespace cache_size
{
    struct CACHE_SIZE {};
    struct UNLIMITED : CACHE_SIZE
    {
        enum { type = 0 };
    };

    template<std::size_t Size>
    struct RESTRICTED : CACHE_SIZE
    {
        enum { cache_size_ = Size, type = 1 };
        RESTRICTED()
        {
            static_assert(Size > 0,
                          "Size of cache must be greater than 0");
        }
    };
}
