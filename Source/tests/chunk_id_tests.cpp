#include <catch2/catch_test_macros.hpp>
#include <insight/wav/chunk_id.hpp>

using namespace insight::wave;

TEST_CASE("chunk_id tests", "[chunk_id]")
{
    SECTION("Constructor sets values correctly")
    {
        SECTION("default constructor")
        {
            auto id = chunk_id();
            REQUIRE(id.to_string().empty());
        }

        SECTION("chunk_id(const char *) constructor")
        {
            auto id = chunk_id("bext");
            REQUIRE(id.to_string() == "bext");
        }

        SECTION("copy assignment")
        {
            auto id = chunk_id("wave");
            auto assigned = id;
            REQUIRE(id.to_string() == "wave");
            REQUIRE(assigned.to_string() == "wave");
        }

        SECTION("copy constructor")
        {
            auto id = chunk_id("list");
            auto copied(id);
            REQUIRE(id.to_string() == "list");
            REQUIRE(copied.to_string() == "list");
        }

        SECTION("chunk_id(const char *) trims strings longer than 4")
        {
            auto id = chunk_id("abcdefghijklmnop");
            std::string str = id.to_string();
            REQUIRE(str.length() == 4);
            REQUIRE(str == "abcd");
        }

        SECTION("chunk_id(const char *) nullptr produces empty string")
        {
            auto id = chunk_id(nullptr);
            std::string str = id.to_string();
            REQUIRE(str.empty());
        }
    }

    // TODO: Test read, clear, comparison operators

}