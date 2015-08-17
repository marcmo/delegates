#include "Delegate.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "stdio.h"

class A
{
public:
    int square(int x)
    {
        return x*x;
    }
    int square_const(int x) const
    {
        return x*x;
    }
    int addOrMultiply(int x, int y, char a)
    {
        return a == 'a' ? x+y : x*y;
    }
    float crazy(int I, char wanna, float go, const char* crazy)
    {
        printf("called crazy with I=%d, wanna=%c, go=%f, crazy=%s\n", I, wanna, go, crazy);
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

typedef delegate<void(*)()> ServiceDelegate;
class Service
{
public:
    void registerDelegate(ServiceDelegate& d)
    {
        mD = &d;
    }
    void notifyDelegate()
    {
        (*mD)();
    }
private:
    ServiceDelegate* mD;
};
typedef delegate<void(*)(int)> ServiceDelegate2;
class Service2
{
public:
    void registerDelegate(ServiceDelegate2& d)
    {
        mD = &d;
    }
    void notifyDelegate(int x)
    {
        (*mD)(x);
    }
private:
    ServiceDelegate2* mD;
};

TEST_CASE("DelegateTest: member function calls", "[calls]")
{
    A a;
    SECTION("call const function")
    {
        auto d = DELEGATE(&A::square, a); // Delegate<int, int>::from_function<A, &A::square_const>(&a);
        REQUIRE(25 == d(5));
        auto d2 = make_delegate(&A::square_const, a); // Delegate<int, int>::from_function<A, &A::square_const>(&a);
        REQUIRE(25 == d2(5));
    }
    SECTION("call with 1 parameter")
    {
        auto d = make_delegate(&A::square, a); // Delegate<int, int>::from_function<A, &A::square>(&a);
        REQUIRE(25 == d(5));
    }
    SECTION("call with multiple parameters")
    {
        auto d = make_delegate(&A::addOrMultiply, a);// Delegate<int, int, int, char>::from_function<A, &A::addOrMultiply>(&a);
        REQUIRE(9 == d(5,4,'a'));
        REQUIRE(20 == d(5,4,'m'));
    }
    SECTION("call with crazy parameters")
    {
        auto d = make_delegate(&A::crazy, a);
        const char* ss = "sheet";
        REQUIRE(9.5 == d(5, 'a', 4.5, ss));
    }
}

TEST_CASE("DelegateTest: free function calls", "[calls]")
{
    SECTION("calling free function")
    {
        typedef delegate<void(*)()> delegateType;
        auto d = make_delegate(&myFreeFunction);
        static_assert( std::is_same< delegateType, decltype(d) >::value, "!");
        REQUIRE(!myFreeFunctionGotCalled);
        Service s;
        s.registerDelegate(d);
        s.notifyDelegate();
        REQUIRE(myFreeFunctionGotCalled);
    }
    SECTION("calling free function with argument")
    {
        auto d = make_delegate(&myFreeFunction2);
        REQUIRE(!myFreeFunction2GotCalled);
        Service2 s2;
        s2.registerDelegate(d);
        s2.notifyDelegate(345);
        REQUIRE(myFreeFunction2GotCalled);
        REQUIRE(345 == myFreeFunction2parameter);
    }
}


