#include "Closure.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "stdio.h"

class A
{
public:
    int twice(int x)
    {
        return 2 * x;
    }
    int adder(int a, int b)
    {
        return a + b;
    }
    double divider(int a, int b, double c)
    {
        return (static_cast<double>(a + b)) / c;
    }
};
TEST_CASE("ClosureTest: simple call", "[calls]")
{
    A a;
    SECTION("bind 1 parameter")
    {
        SECTION("call func with 1 parameter (1 bound, 0 free)")
        {
            auto c = make_closure(&A::twice, a, 42);
            REQUIRE(84 == c());
        }
        SECTION("call func with 2 parameter (1 bound, 1 free)")
        {
            auto c = make_closure(&A::adder, a, 11);
            REQUIRE(33 == c(22));
        }
        SECTION("call func with 3 parameter (1 bound, 2 free)")
        {
            auto c = make_closure(&A::divider, a, 10);
            REQUIRE(7.5 == c(5, 2.0));
        }
    }
    SECTION("bind 2 parameters")
    {
        SECTION("call func with 2 parameter (2 bound, 0 free)")
        {
            auto c = make_closure2(&A::adder, a, 11, 22);
            REQUIRE(33 == c());
        }
        SECTION("call func with 3 parameter (2 bound, 1 free)")
        {
            auto c = make_closure2(&A::divider, a, 10, 5);
            REQUIRE(7.5 == c(2.0));
        }
    }
}
