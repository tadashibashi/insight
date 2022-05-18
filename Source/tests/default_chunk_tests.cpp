#include "insight_tests.hpp"
#include <insight/wav/default_chunk.hpp>
#include <sstream>

using namespace insight::wave;
using namespace insight;

TEST_CASE("default_chunk tests", "[default_chunk]")
{
    SECTION("Constructor tests")
    {
        SECTION("Constructor (const char *) sets members correctly")
        {
            default_chunk def("data");

            REQUIRE(def.id() == "data");
            REQUIRE(def.size() == 0);
            REQUIRE(!def.was_read());
        }

        SECTION("Constructor (const chunk_id &) sets members correctly")
        {
            default_chunk def(chunk_id("data"));

            REQUIRE(def.id() == "data");
            REQUIRE(def.size() == 0);
            REQUIRE(!def.was_read());
        }
    }

    SECTION("read and clear tests")
    {
        std::vector<unsigned char> data {1, 2, 3, 4, 5, 6, 7, 8};
        buffer buf(data.data(), data.size());
        default_chunk def(chunk_id("data"));

        SECTION("read reads correct number of bytes, sets flag")
        {
            REQUIRE(buf.tellg() == 0);
            def.read(buf, data.size());
            REQUIRE(buf.tellg() == data.size());
            REQUIRE(def.was_read());
        }

        SECTION("clear resets read bytes")
        {
            REQUIRE(!def.was_read());
            def.read(buf, data.size());
            REQUIRE(def.was_read());
            REQUIRE(def.size() == data.size());
            def.clear();
            REQUIRE(!def.was_read());
            REQUIRE(def.size() == 0);
        }
    }
}
