#ifndef CLOSURE_H_
#define CLOSURE_H_

#include "Delegate.h"

typedef Delegate<void()> VoidDelegate;

template<typename P1>
class Closure
{
public:
    typedef Delegate<int(P1)> ClosureDelegate;
    Closure()
    {}

    Closure
        (
         ClosureDelegate closureDelegate,
         P1 param1
        )
        : fClosureDelegate(closureDelegate)
        , fParameter1(param1)
    {}

    Closure(const Closure& closure) :
        fClosureDelegate(closure.fClosureDelegate),
        fParameter1(closure.fParameter1)
    {}

    Closure& operator=(const Closure& closure)
    {
        if (this != &closure)
        {
            fClosureDelegate = closure.fClosureDelegate;
            fParameter1 = closure.fParameter1;
        }
        return *this;
    }

    int operator()()
    {
        return fClosureDelegate(fParameter1);
    }

    template <class T, void (T::*MethodPointer)(P1)>
        static Closure fromObject(T& callee, P1 param1)
        {
            ClosureDelegate d(ClosureDelegate::template registerCallee< T, MethodPointer>(callee));
            return Closure( d, param1);
        }

    private:
    ClosureDelegate fClosureDelegate;
    P1 fParameter1;
};


#endif /* CLOSURE_H_ */
