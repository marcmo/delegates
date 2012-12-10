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
typedef Delegate<void> ServiceDelegate;
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
int main()
{
    A a;
    auto d = DELEGATE(&A::foo, &a); // Delegate<int, int>::from_function<A, &A::foo>(&a);
    auto d2 = DELEGATE(&A::bar, &a);// Delegate<int, int, int, char>::from_function<A, &A::bar>(&a);
    auto d3 = DELEGATE(&A::crazy, &a);
    auto d4 = DELEGATE(&A::deadSimple, &a);
    printf("calling delegate with return value: d(42)=%d\n", d(42));
    printf("calling delegate d2 with return value: d2(42, 2, 'a')=%d\n", d2(42, 2, 'a'));
    const char* ss = "sheet";
    d3(5, 'a', 4.5, ss);

    Service s;
    s.registerDelegate(d4);
    s.notifyDelegate();

    return 0;
}
