#include <catch2/catch_test_macros.hpp>
#include <insight/buffer.hpp>
#include <string>
#include <cstring>

using namespace insight;

TEST_CASE("buffer tests", "[buffer]")
{
    SECTION("buffer constructor sets data")
    {
        SECTION("buffer constructor sets data string")
        {
            std::string str = "123456789";
            buffer buf((unsigned char *)(str.c_str()), str.length());
            REQUIRE(buf.data() == (unsigned char *)str.data());
            REQUIRE(buf.size() == str.length());
            REQUIRE(strcmp((const char *)buf.data(), str.data()) == 0);
        }

        SECTION("buffer constructor sets null data")
        {
            buffer buf(nullptr, 0);
            REQUIRE(buf.data() == nullptr);
            REQUIRE(buf.size() == 0);
        }
    }

    SECTION("buffer read tests")
    {
        std::vector<unsigned char> data = {0u, 0u, 0u, 1u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u, 13u, 14u, 15u, 16u};
        buffer buf(data.data(), data.size());
        REQUIRE(buf.tellg() == 0);

        SECTION("buffer::read(T &) return value matches size of data: int")
        {
            int i;
            size_t bytesRead = buf.read(i);
            REQUIRE(bytesRead == sizeof(i));
            REQUIRE(bytesRead == buf.tellg());
        }

        SECTION("buffer::read(T &) return value matches size of data: short")
        {
            short i;
            size_t bytesRead = buf.read(i);
            REQUIRE(bytesRead == sizeof(i));
            REQUIRE(bytesRead == buf.tellg());
        }

        SECTION("buffer::read(T &) return value matches size of data: arbitrary struct")
        {
            struct structure {
                char c[4];
                float f;
                int i;
            } i {};

            REQUIRE(sizeof(i) < buf.size());
            size_t bytesRead = buf.read(i);
            REQUIRE(bytesRead == sizeof(i));
            REQUIRE(bytesRead == buf.tellg());
        }

        SECTION("buffer::read(unsigned char *, size_t) return value matches size of data")
        {
            unsigned char chars[4];
            size_t bytesRead = buf.read(chars, 4);
            REQUIRE(bytesRead == sizeof(chars));
            REQUIRE(bytesRead == buf.tellg());
        }

        SECTION("reading over the limit throws an overflow_error")
        {
            struct large {
                char s[128];
            };

            bool didThrow = false;
            try {
                large l{};
                REQUIRE(sizeof(l) > buf.size());
                buf.read(l);
            }
            catch(const std::overflow_error &e)
            {
                didThrow = true;
            }
            REQUIRE(didThrow);
        }
    }

    SECTION("buffer free tests")
    {
        SECTION("buffer free resets size and data")
        {
            auto *data = (unsigned char *)malloc(100);
            REQUIRE(data);

            buffer buf(data, 100);
            REQUIRE(buf.size() == 100);
            REQUIRE(buf.data() != nullptr);
            REQUIRE(buf.current() != nullptr);
            buf.free();
            REQUIRE(buf.size() == 0);
            REQUIRE(buf.data() == nullptr);
            REQUIRE(buf.current() == nullptr);
            REQUIRE(buf.tellg() == 0);
        }
    }

    SECTION("buffer peek tests")
    {
        std::vector<unsigned char> data = {0u, 0u, 0u, 1u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u, 13u, 14u, 15u, 16u};
        buffer buf(data.data(), data.size());

        SECTION("buffer peek reads correct value")
        {
            buf.move(2);
            REQUIRE(buf.peek() == data[2]);
            buf.move(4);
            REQUIRE(buf.peek() == data[4]);
            buf.move(11);
            REQUIRE(buf.peek() == data[11]);
        }

        SECTION("buffer peek does not move current ptr")
        {
            REQUIRE(buf.peek() == 0);
            REQUIRE(buf.tellg() == 0);
            REQUIRE(buf.current() == buf.data());

            buf.move(10);
            REQUIRE(buf.tellg() == 10);
            REQUIRE(buf.peek() == data[10]);
            REQUIRE(buf.tellg() == 10);
            REQUIRE(buf.current() == buf.data() + 10);
        }
    }

    SECTION("buffer move tests")
    {
        std::string str = "123456789";
        buffer buf((unsigned char *)(str.c_str()), str.length());

        SECTION("buffer move and tellg match and point to correct data")
        {
            buf.move(2);
            REQUIRE(buf.tellg() == 2);
            REQUIRE(*buf.current() == str[2]);
            REQUIRE(buf.current() == buf.data() + 2);
            buf.move(0);
            REQUIRE(buf.tellg() == 0);
            REQUIRE(*buf.current() == str[0]);
            REQUIRE(buf.current() == buf.data());
            buf.move(5);
            REQUIRE(buf.tellg() == 5);
            REQUIRE(*buf.current() == str[5]);
            REQUIRE(buf.current() == buf.data() + 5);
        }

        SECTION("moving past limit throws overflow_error")
        {
            bool didThrow = false;
            try {
                // buffer can point to one after last, so we add + 1 to size
                buf.move(buf.size() + 1);
            }
            catch(const std::overflow_error &e)
            {
                didThrow = true;
            }

            REQUIRE(didThrow);
        }

        SECTION("moving past limit throws overflow_error on null data")
        {
            buffer nullBuf(nullptr, 0);
            bool didThrow = false;
            try {
                // buffer can point to one after last, so we add + 1 to size
                nullBuf.move(buf.size() + 1);
            }
            catch(const std::overflow_error &e)
            {
                didThrow = true;
            }

            REQUIRE(didThrow);
        }
    }
}
