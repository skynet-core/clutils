#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "clutils/clutils.h"
#include <tuple>

TEST_CASE("ranges must be properly initialized", "[init_ranges]")
{
    auto ranges = init_ranges(256, 64, 3);
    auto global = std::get<1>(ranges);
    REQUIRE(global.get()[0] == 256);
}