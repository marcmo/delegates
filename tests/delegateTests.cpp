#include "delegate.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "stdio.h"

using namespace dlgt;
class A
{
public:
    int square(int x)
    {
        return x * x;
    }
    int square_const(int x) const
    {
        return x * x;
    }
    int addOrMultiply(int x, int y, char a)
    {
        return a == 'a' ? x + y : x * y;
    }
    float crazy(int I, char wanna, float go, const char *crazy)
    {
        printf("called crazy with I=%d, wanna=%c, go=%f, crazy=%s\n", I, wanna,
               go, crazy);
        return I + go;
    }
};
static bool myFreeFunctionGotCalled = false;
static void myFreeFunction()
{
    myFreeFunctionGotCalled = true;
}
static bool myFreeFunction2GotCalled = false;
static int myFreeFunction2parameter = 0;
static void myFreeFunction2(int x)
{
    myFreeFunction2GotCalled = true;
    myFreeFunction2parameter = x;
}

typedef delegate<void (*)()> ServiceDelegate;
class Service
{
public:
    void registerDelegate(ServiceDelegate &d)
    {
        mD = &d;
    }
    void notifyDelegate()
    {
        (*mD)();
    }

private:
    ServiceDelegate *mD;
};
typedef delegate<void (*)(int)> ServiceDelegate2;
class Service2
{
public:
    void registerDelegate(ServiceDelegate2 &d)
    {
        mD = &d;
    }
    void notifyDelegate(int x)
    {
        (*mD)(x);
    }

private:
    ServiceDelegate2 *mD;
};

TEST_CASE("DelegateTest: member function calls", "[calls]")
{
    A a;
    SECTION("call const function")
    {
        auto d2 = make_delegate(&A::square_const, a);
        static_assert(
            std::is_same<delegate<int (A::*)(int) const>, decltype(d2)>::value,
            "!");
        REQUIRE(25 == d2(5));
    }
    SECTION("call with 1 parameter")
    {
        auto d = make_delegate(&A::square, a);
        static_assert(
            std::is_same<delegate<int (A::*)(int)>, decltype(d)>::value, "!");
        REQUIRE(25 == d(5));
    }
    SECTION("call with multiple parameters")
    {
        auto d = make_delegate(&A::addOrMultiply, a);
        static_assert(
            std::is_same<delegate<int (A::*)(int, int, char)>, decltype(d)>::value,
            "!");
        REQUIRE(9 == d(5, 4, 'a'));
        REQUIRE(20 == d(5, 4, 'm'));
    }
    SECTION("call with crazy parameters")
    {
        auto d = make_delegate(&A::crazy, a);
        static_assert(
            std::is_same<delegate<float (A::*)(int, char, float, const char *)>,
                         decltype(d)>::value,
            "!");
        const char *ss = "sheet";
        REQUIRE(9.5 == d(5, 'a', 4.5, ss));
    }
}

TEST_CASE("DelegateTest: free function calls", "[calls]")
{
    SECTION("calling free function")
    {
        auto d = make_delegate(&myFreeFunction);
        static_assert(std::is_same<delegate<void (*)()>, decltype(d)>::value,
                      "!");
        REQUIRE(!myFreeFunctionGotCalled);
        Service s;
        s.registerDelegate(d);
        s.notifyDelegate();
        REQUIRE(myFreeFunctionGotCalled);
    }
    SECTION("calling free function with argument")
    {
        auto d = make_delegate(&myFreeFunction2);
        static_assert(std::is_same<delegate<void (*)(int)>, decltype(d)>::value,
                      "!");
        REQUIRE(!myFreeFunction2GotCalled);
        Service2 s2;
        s2.registerDelegate(d);
        s2.notifyDelegate(345);
        REQUIRE(myFreeFunction2GotCalled);
        REQUIRE(345 == myFreeFunction2parameter);
    }
}
