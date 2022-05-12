#include <catch2/catch_test_macros.hpp>
#include <insight/wav/chunk_id.hpp>
#include <sstream>

using namespace insight::wave;
using namespace insight;

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

    SECTION("chunk_id::read tests")
    {
        SECTION("read bext")
        {
            std::string header = "bext";
            buffer buf((unsigned char *)header.data(), 4);
            chunk_id id;

            size_t bytesRead = id.read(buf);

            REQUIRE(bytesRead == 4);
            REQUIRE(id.to_string() == "bext");
        }

        SECTION("read: buffer overflow errors do not mutate chunk_id")
        {
            std::string header = "bex";
            buffer buf((unsigned char *)header.data(), 3);
            chunk_id id;

            size_t bytesRead = 0;
            bool didThrow = false;
            try {
                bytesRead = id.read(buf);
            }
            catch(const std::overflow_error &e)
            {
                didThrow = true;
            }

            REQUIRE(didThrow);
            REQUIRE(bytesRead == 0);
            REQUIRE(id.to_string().empty());
        }
    }

    SECTION("chunk_id::clear tests")
    {
        SECTION("Clear makes return string empty")
        {
            chunk_id id("bext");

            REQUIRE(id.to_string() == "bext");
            id.clear();

            REQUIRE(id.to_string().empty());
        }

        SECTION("Clear nullptr return string empty, does not throw")
        {
            chunk_id id(nullptr);

            REQUIRE(id.to_string().empty());

            bool didThrow = false;
            try {
                id.clear();
            }
            catch(const std::exception &e)
            {
                didThrow = true;
            }

            REQUIRE(!didThrow);
            REQUIRE(id.to_string().empty());
        }
    }

    SECTION("chunk_id comparison operators")
    {
        SECTION("chunk_id == chunk_id")
        {
            SECTION("typical test")
            {
                chunk_id idsame1("abcd");
                chunk_id idsame2("abcd");
                chunk_id iddif("wxyz");

                REQUIRE((idsame1 == idsame2));
                REQUIRE(!(idsame1 == iddif));
            }

            SECTION("nullptrs equal")
            {
                chunk_id idnull1(nullptr);
                chunk_id idnull2(nullptr);
                chunk_id idnorm("abcd");

                REQUIRE((idnull1 == idnull2));
                REQUIRE(!(idnull1 == idnorm));
            }

            SECTION("trimmed down to equal ids")
            {
                chunk_id idsame1("abcdefghijklmnop");
                chunk_id idsame2("abcd");

                REQUIRE((idsame1 == idsame2));
            }
        }

        SECTION("chunk_id != chunk_id")
        {   // contains same tests as operator ==,
            // but testing for opposite result
            SECTION("typical test")
            {
                chunk_id idsame1("abcd");
                chunk_id idsame2("abcd");
                chunk_id iddif("wxyz");

                REQUIRE(!(idsame1 != idsame2));
                REQUIRE((idsame1 != iddif));
            }

            SECTION("nullptrs equal")
            {
                chunk_id idnull1(nullptr);
                chunk_id idnull2(nullptr);
                chunk_id idnorm("abcd");

                REQUIRE(!(idnull1 != idnull2));
                REQUIRE((idnull1 != idnorm));
            }

            SECTION("trimmed down to equal ids")
            {
                chunk_id idsame1("abcdefghijklmnop");
                chunk_id idsame2("abcd");

                REQUIRE(!(idsame1 != idsame2));
            }
        }
    }

    SECTION("std::ostream & << const chunk_id &")
    {
        std::stringstream ss;

        SECTION("ostream receives correct 4-char sequence")
        {
            ss << chunk_id("bext");

            REQUIRE(ss.str() == "bext");
        }

        SECTION("ostream returns correct stream")
        {
            std::stringstream &returned = (std::stringstream &)(ss << chunk_id("bext"));

            REQUIRE(returned.str() == ss.str());
        }
    }
}
