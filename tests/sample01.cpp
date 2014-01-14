#include "Delegate01.h"
#include "stdio.h"

class A
{
public:
    void deadSimple(int x)
    {
        printf("with one parameter: %d\n", x);
    }
};

int main()
{
    A a;
    auto d = Delegate::from_function<A, &A::deadSimple>(a);
    printf("calling delegate with return value: d(42)\n");
    d(42);
    auto d2 = d;
    d2(42);
    return 0;
}
