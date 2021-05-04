namespace policy
{
    struct CACHE_POLICY {};
    struct LRU_CACHE : CACHE_POLICY
    {
        enum {type = 0};
    };

    struct MRU_CACHE : CACHE_POLICY
    {
        enum {type = 1};
    };

    struct LFU_CACHE : CACHE_POLICY
    {
        enum {type = 2};
    };

    struct MFU_CACHE : CACHE_POLICY
    {
        enum {type = 3};
    };

    struct RANDOM_CACHE : CACHE_POLICY
    {
        enum {type = 4};
    };
} // namespace policy
