#include <catch2/catch_session.hpp>
#include "insight_tests.hpp"
#include <cmath>

int main()
{
    return Catch::Session().run();
}

float
insight::tests::mroundf(float f)
{
    return std::roundf(f * 1000000.f) / 1000000.f;
}

long double
insight::tests::mround(long double ld)
{
    return std::roundl(ld * 1000000.0L) / 1000000.0L;
}