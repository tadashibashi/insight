//
// Created by Aaron Ishibashi on 5/18/22.
//
#pragma once
#include <string>
#include <cmath>
namespace insight
{
    static const int PRIME_CONST = 29;

    /// implementation from https://dev.to/muiz6/string-hashing-in-c-1np3
    constexpr int get_hash(const char *key)
    {
        int hashCode = 0;
        int i = 0;

        for (const char *c = key; *c != '\0'; ++c)
        {
            int primeToIPower = 1;
            for (int p = 0; p < i; ++p)
                primeToIPower *= PRIME_CONST;

            hashCode += (int)*c * primeToIPower;
            ++i;
        }

        return hashCode;
    }
}
