#include "delegate.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "stdio.h"

using namespace dlgt;
static uint32_t _simple_function_calls = false;
class A
{
public:
    void simple()
    {
        ++_simple_function_calls;
    }
    void simpleOther()
    {}
    void simple_const() const
    {
        ++_simple_function_calls;
    }
    int simpleInt()
    {
        return 42;
    }
    int add(int a, int b)
    {
        return a + b;
    }
    int square(int x)
    {
        return x * x;
    }
    int squareStar(int* x)
    {
        return *x * *x;
    }
    int square_const(int x) const
    {
        return x * x;
    }
    int addOrMultiply(int x, int y, char a)
    {
        return a == 'a' ? x + y : x * y;
    }
    float crazy(int I, char wanna, float go, const char* crazy)
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

typedef delegate<void()> ServiceDelegate;
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
// typedef delegate<void(int)> ServiceDelegate2;
// class Service2
// {
// public:
//     void registerDelegate(ServiceDelegate2& d)
//     {
//         mD = &d;
//     }
//     void notifyDelegate(int x)
//     {
//         (*mD)(x);
//     }
//
// private:
//     ServiceDelegate2* mD;
// };
//
TEST_CASE("DelegateTest: member function calls", "[calls]")
{
    A a;
    typedef delegate<void()> VoidDelegate;
    SECTION("call void function with 0 parameter")
    {
        _simple_function_calls = 0;
        VoidDelegate d = VoidDelegate::create<A, &A::simple>(a);
        d();
        REQUIRE(_simple_function_calls == 1);
    }
    SECTION("call function with 0 parameter, returning int")
    {
        delegate<int()> d = delegate<int()>::create<A, &A::simpleInt>(a);
        REQUIRE(d() == 42);
    }
    SECTION("call with 1 parameter, returning int")
    {
        delegate<int(int)> d = delegate<int(int)>::create<A, &A::square>(a);
        REQUIRE(25 == d(5));
    }
    SECTION("call with 1 parameter (pointer), returning int")
    {
        delegate<int(int*)> d = delegate<int(int*)>::create<A, &A::squareStar>(a);
        int in = 5;
        REQUIRE(25 == d(&in));
    }
    SECTION("call const void function with 0 parameter")
    {
        // void simple_const() const
        _simple_function_calls = 0;
        delegate<void()> d = delegate<void()>::create<A, &A::simple_const>(a);
        d();
        REQUIRE(_simple_function_calls == 1);
    }
    SECTION("call const function with 1 parameter, returning int")
    {
        // int square_const(int x) const
        delegate<int(int)> d = delegate<int(int)>::create<A, &A::square_const>(a);
        REQUIRE(25 == d(5));
    }
    // SECTION("call with 2 parameter, returning int")
    // {
    //     auto d = make_delegate(&A::add, a);
    //     static_assert(
    //         std::is_same<delegate<int (A::*)(int, int)>, decltype(d)>::value, "!");
    //     REQUIRE(9 == d(4, 5));
    // }
    // SECTION("call with multiple parameters")
    // {
    //     auto d = make_delegate(&A::addOrMultiply, a);
    //     static_assert(
    //         std::is_same<delegate<int (A::*)(int, int, char)>, decltype(d)>::value,
    //         "!");
    //     REQUIRE(9 == d(5, 4, 'a'));
    //     REQUIRE(20 == d(5, 4, 'm'));
    // }
    // SECTION("call with crazy parameters")
    // {
    //     auto d = make_delegate(&A::crazy, a);
    //     static_assert(
    //         std::is_same<delegate<float (A::*)(int, char, float, const char*)>,
    //         decltype(d)>::value,
    //         "!");
    //     const char* ss = "sheet";
    //     REQUIRE(9.5 == d(5, 'a', 4.5, ss));
    // }
}
//
// TEST_CASE("DelegateTest: free function calls", "[calls]")
// {
//     SECTION("calling free function")
//     {
//         auto d = make_delegate(&myFreeFunction);
//         static_assert(std::is_same<delegate<void (*)()>, decltype(d)>::value,
//                       "!");
//         REQUIRE(!myFreeFunctionGotCalled);
//         Service s;
//         s.registerDelegate(d);
//         s.notifyDelegate();
//         REQUIRE(myFreeFunctionGotCalled);
//     }
//     SECTION("calling free function with argument")
//     {
//         auto d = make_delegate(&myFreeFunction2);
//         static_assert(std::is_same<delegate<void (*)(int)>, decltype(d)>::value,
//                       "!");
//         REQUIRE(!myFreeFunction2GotCalled);
//         Service2 s2;
//         s2.registerDelegate(d);
//         s2.notifyDelegate(345);
//         REQUIRE(myFreeFunction2GotCalled);
//         REQUIRE(345 == myFreeFunction2parameter);
//     }
// }
// TEST_CASE("DelegateTest: compare a copied delegate", "[assignement]")
// {
//     A a;
//     SECTION("compare: void function with 0 parameter")
//     {
//         auto d = make_delegate(&A::simple, a);
//         auto dother = make_delegate(&A::simpleOther, a);
//         auto d2 = d;
//         REQUIRE(d == d2);
//         REQUIRE(d != dother);
//     }
//     SECTION("compare: with 1 parameter, returning int")
//     {
//         auto d = make_delegate(&A::square, a);
//         auto d2 = d;
//         REQUIRE(d == d2);
//     }
//     SECTION("compare: with 1 parameter (pointer), returning int")
//     {
//         auto d = make_delegate(&A::squareStar, a);
//         auto d2 = d;
//         REQUIRE(d == d2);
//     }
//     SECTION("compare: const void function with 0 parameter")
//     {
//         auto d = make_delegate(&A::simple_const, a);
//         auto d2 = d;
//         REQUIRE(d == d2);
//     }
//     SECTION("compare: const function with 1 parameter, returning int")
//     {
//         auto d = make_delegate(&A::square_const, a);
//         auto d2 = d;
//         REQUIRE(d == d2);
//     }
//     SECTION("call with 2 parameter, returning int")
//     {
//         auto d = make_delegate(&A::add, a);
//         auto d2 = d;
//         REQUIRE(d == d2);
//     }
//     SECTION("call with multiple parameters")
//     {
//         auto d = make_delegate(&A::addOrMultiply, a);
//         auto d2 = d;
//         REQUIRE(d == d2);
//     }
//     SECTION("call with crazy parameters")
//     {
//         auto d = make_delegate(&A::crazy, a);
//         auto d2 = d;
//         REQUIRE(d == d2);
//     }
// }
TEST_CASE("DelegateTest: store delegates in vector", "[store]")
{
    static uint32_t fooCalled = 0;
    class Tmp
    {
    public:
        void foo()
        {
            ++fooCalled;
        }
    };
    A a;
    Tmp tmp;
    typedef delegate<void()> VoidDelegate;
    SECTION("store multiple delegates")
    {
        fooCalled = 0;
        _simple_function_calls = 0;
        VoidDelegate d = VoidDelegate::create<A, &A::simple>(a);
        VoidDelegate d2 = VoidDelegate::create<Tmp, &Tmp::foo>(tmp);
        std::vector<VoidDelegate> v;
        v.push_back(d);
        v.push_back(d2);
        for (std::vector<VoidDelegate>::iterator iter = v.begin();
                iter != v.end(); ++iter)
        {
            (*iter)();
        }
        REQUIRE(_simple_function_calls == 1);
        REQUIRE(fooCalled == 1);
    }
}

