#ifndef CLOSURE_H_
#define CLOSURE_H_

namespace dlgt
{
template <typename T>
class closure;

template <typename T, typename R, typename B, typename... Params>
class closure<R (T::*)(B, Params...)>
{
public:
    typedef R (T::*func_type)(B, Params...);

    closure(func_type func, T &callee, B bound)
        : callee_(&callee)
        , func_(func)
        , bound_(bound)
    {
    }

    R operator()(Params... args) const
    {
        return (callee_->*func_)(bound_, args...);
    }

    bool operator==(const closure &other) const
    {
        return (callee_ == other.callee_) && (func_ == other.func_) &&
               (bound_ == other.bound_);
    }

private:
    T *callee_;
    func_type func_;
    B bound_;
};

template <typename T>
class closure2;
template <typename T, typename R, typename B1, typename B2, typename... Params>
class closure2<R (T::*)(B1, B2, Params...)>
{
public:
    typedef R (T::*func_type)(B1, B2, Params...);

    closure2(func_type func, T &callee, B1 bound1, B2 bound2)
        : callee_(&callee)
        , func_(func)
        , bound_(bound1)
        , bound2_(bound2)
    {
    }

    R operator()(Params... args) const
    {
        return (callee_->*func_)(bound_, bound2_, args...);
    }

    bool operator==(const closure2 &other) const
    {
        return (callee_ == other.callee_) && (func_ == other.func_) &&
               (bound_ == other.bound_) && (bound2_ == other.bound2_);
    }

private:
    T *callee_;
    func_type func_;
    B1 bound_;
    B2 bound2_;
};
template <typename F, typename T, typename B>
closure<F> make_closure(F func, T &obj, B b)
{
    return closure<F>(func, obj, b);
}
template <typename F, typename T, typename B1, typename B2>
closure2<F> make_closure2(F func, T &obj, B1 b1, B2 b2)
{
    return closure2<F>(func, obj, b1, b2);
}
}// namespace dlgt

#endif /* CLOSURE_H_ */
