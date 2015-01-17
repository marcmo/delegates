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
    typedef Closure<int> IntClosure;
    SECTION("call with 1 parameter")
    {
        A a;
        IntClosure::ClosureDelegate d = DELEGATE(&A::foo, a); // Delegate<int, int>::from_function<A, &A::foo>(&a);
        IntClosure c(d, 42);
        REQUIRE(84 == c());
    }

}





