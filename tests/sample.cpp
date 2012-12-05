#include "Delegate.h"
#include "stdio.h"

class A
{
public:
    int foo(int x)
    {
        return x*x;
    }
    int bar(int x, int y, char a)
    {
        return x*y;
    }
    void crazy(int I, char wanna, float go, char* crazy)
    {
        printf("called crazy with I=%d, wanna=%c, go=%f, crazy=%s\n", I, wanna, go, crazy);
    }
};
int main()
{
    A a;
    auto d = BIND(&A::foo, &a); // Delegate<int, int>::from_function<A, &A::foo>(&a);
    auto d2 = BIND(&A::bar, &a);// Delegate<int, int, int, char>::from_function<A, &A::bar>(&a);
    auto d3 = BIND(&A::crazy, &a);
    printf("calling delegate with return value: d(42)=%d\n", d(42));
    printf("calling delegate d2 with return value: d2(42, 2, 'a')=%d\n", d2(42, 2, 'a'));
    d3(5, 'a', 4.5, "sheeeet!");
    return 0;
}
