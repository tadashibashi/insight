#ifndef swap_endian_h_
#define swap_endian_h_
#include <cstdlib>
#include <climits>

/// @function swap_endian
/// @returns The value of the passed item with its byte order reversed
template <typename T>
T swap_endian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

/// @function is_big_endian
/// @brief Lets us know if this OS is big endian or not
/// @returns True if big endian; False if little endian
inline bool is_big_endian()
{
    union {
       uint32_t word;
       uint8_t bytes[4];
    } test;
    test.word = 1u;
    
    return (test.bytes[0] == 0);
}

#endif
