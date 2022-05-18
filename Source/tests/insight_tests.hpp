#pragma once
#include <catch2/catch_test_macros.hpp>

namespace insight::tests
{
    /// Rounds float to the millionth's place.
    /// Intended to be used when comparing results.
    float mroundf(float f);

    /// Rounds float to the millionth's place.
    /// Intended to be used when comparing results.
    long double mround(long double d);
}

using namespace insight;
using namespace insight::tests;
