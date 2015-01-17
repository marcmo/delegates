#include "Delegate.h"
#include "stdio.h"

class A
{
public:
    void deadSimple()
    {
        printf("no params whatsoever\n");
    }
    int foo(int x)
    {
        return x*x;
    }
    int bar(int x, int y, char a)
    {
        return a == 'a' ? x+y : x*y;
    }
    void crazy(int I, char wanna, float go, const char* crazy)
    {
        printf("called crazy with I=%d, wanna=%c, go=%f, crazy=%s\n", I, wanna, go, crazy);
    }
};

void myFreeFunction()
{
    printf("myFreeFunction got called!\n");
}
void myFreeFunction2(int x)
{
    printf("myFreeFunction2 got called with %d!\n", x);
}

typedef Delegate<void()> ServiceDelegate;
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
typedef Delegate<int(int)> ServiceDelegate2;
class Service2
{
public:
    void registerDelegate(ServiceDelegate2& d)
    {
        mD = &d;
    }
    void notifyDelegate()
    {
        (*mD)(42);
    }

private:
    ServiceDelegate2* mD;
};
int main()
{
    A a;
    auto d = DELEGATE(&A::foo, a); // Delegate<int, int>::from_function<A, &A::foo>(&a);
    auto d2 = DELEGATE(&A::bar, a);// Delegate<int, int, int, char>::from_function<A, &A::bar>(&a);
    auto d3 = DELEGATE(&A::crazy, a);
    auto d4 = DELEGATE(&A::deadSimple, a);
    printf("calling delegate with return value: d(42)=%d\n", d(42));
    printf("calling delegate d2 with return value: d2(42, 2, 'a')=%d\n", d2(42, 2, 'a'));
    const char* ss = "sheet";
    d3(5, 'a', 4.5, ss);
    auto d5 = DELEGATE2(&myFreeFunction);
    auto d6 = DELEGATE2(&myFreeFunction2);
    d5();
    d6(5);

    Service s;
    s.registerDelegate(d4);
    s.notifyDelegate();

    Service2 s2;
    s2.registerDelegate(d);
    s2.notifyDelegate();

    return 0;
}
