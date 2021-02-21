#include <catch2/catch.hpp>
#include <functional>
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

TEST_CASE("Halve number", "[half]")
{
	REQUIRE(half(0) == 0);
	REQUIRE(half(1) == 0);
	REQUIRE(half(29) == 14);
	REQUIRE(half(74) == 37);
}

TEST_CASE("Multiplying using power", "[power]")
{
	REQUIRE(power_semigroup(1, 1, std::multiplies<int>()) == 1);
	REQUIRE(power_semigroup(10, 1, std::multiplies<int>()) == 10);
	REQUIRE(power_semigroup(1, 10, std::multiplies<int>()) == 1);
	REQUIRE(power_semigroup(2, 2, std::multiplies<int>()) == 4);
	REQUIRE(power_semigroup(2, 10, std::multiplies<int>()) == 1024);
	REQUIRE(power_semigroup(10, 2, std::multiplies<int>()) == 100);
}

TEST_CASE("Greatest common divisor", "[gcd]")
{
	REQUIRE(gcd(54, 24) == 6);
	REQUIRE(gcd(29, 0) == 29);
	REQUIRE(gcd(9, 28) == 1);
	REQUIRE(gcd(42, 56) == 14);
	REQUIRE(gcd(18, 84) == 6);
	REQUIRE(gcd(48, 180) == 12);
}
