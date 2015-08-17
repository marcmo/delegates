#ifndef CLOSURE_H_
#define CLOSURE_H_

template<typename T>
class closure;
template<typename T, typename R, typename B, typename... Params>
class closure<R (T::*)(B, Params...)>
{
public:
    typedef R (T::*func_type)(B, Params...);

    closure(func_type func, T& callee, B bound)
        : fpCallee(&callee)
        , fpFunc(func)
        , fBound(bound)
    {}

    R operator()(Params... args) const
    {
        return (fpCallee->*fpFunc)(fBound, args...);
    }

    bool operator==(const closure& other) const
    {
        return (fpCallee == other.fpCallee)
               && (fpFunc == other.fpFunc)
               && (fBound == other.fBound);
    }

private:
    T* fpCallee;
    func_type fpFunc;
    B fBound;
};
template<typename F, typename T, typename B>
closure<F> make_closure(F func, T& obj, B b)
{
    return closure<F>(func, obj, b);
}

#endif /* CLOSURE_H_ */
