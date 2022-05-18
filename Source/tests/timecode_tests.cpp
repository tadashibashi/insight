#include "insight_tests.hpp"
#include <insight/timecode.hpp>

using namespace insight;

TEST_CASE("timecode tests", "[timecode]")
{
    SECTION("constructor tests")
    {
        SECTION("default constructor assigns correct values")
        {
            timecode t;
            REQUIRE(t.framerate() == 30); // default
            REQUIRE(t.subframe_base() == 80);
            REQUIRE(t.timecode_seconds() == 0);
        }

        SECTION("param constructor assigns correct non-default values")
        {
            timecode t(3600, 24, 100);
            REQUIRE(t.framerate() == 24); // non-default values
            REQUIRE(t.subframe_base() == 100);
            REQUIRE(t.timecode_seconds() == 3600);
        }
    }

    SECTION("string conversion tests: 30 fps, 80 subframes")
    {
        SECTION("zero")
        {
            timecode t(0, 30, 80);
            REQUIRE(t.to_string() == "00:00:00:00.00");
        }

        SECTION("one hour")
        {
            timecode t(3600, 30, 80);
            REQUIRE(t.to_string() == "01:00:00:00.00");
        }

        SECTION("precise on large time amounts") // TODO: how to calculate these ahead of time from objective source?
        {
            timecode t(123456.789, 30, 80);
            REQUIRE(t.to_string() == "34:17:36:23.54");
        }
    }
}
