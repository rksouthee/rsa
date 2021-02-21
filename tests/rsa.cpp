#include <catch2/catch.hpp>
#include "rsa_key.h"

TEST_CASE("Check number is even", "[even]")
{
	REQUIRE(even(0));
	REQUIRE(even(74));
	REQUIRE_FALSE(even(1));
	REQUIRE_FALSE(even(29));
}

TEST_CASE("Check number is odd", "[odd]")
{
	REQUIRE(odd(1));
	REQUIRE(odd(29));
	REQUIRE_FALSE(odd(0));
	REQUIRE_FALSE(odd(74));
}
