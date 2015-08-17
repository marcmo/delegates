#include "Closure.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "stdio.h"

class A
{
public:
    int foo(int x)
    {
        printf("called foo with x=%d", x);
        return 2*x;
    }
};
TEST_CASE("ClosureTest: simple call", "[calls]")
{
    SECTION("call with 1 parameter")
    {
        A a;
        // Delegate<int, int>::from_function<A, &A::foo>(&a);
        auto c = make_closure(&A::foo, a, 42);
        printf("type or c: %s\n", typeid(c).name());
        REQUIRE(84 == c());
    }

}





