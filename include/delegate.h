#ifndef DLGT_DELEGATE_H_
#define DLGT_DELEGATE_H_


namespace dlgt
{
#if __cplusplus > 199711L
// we have C++11 support...yeah!
/**
 * non specialized template declaration for delegate
 */
template <typename T>
class delegate;

template <typename T, typename R, typename D, typename... Params>
class delegate_base
{
protected:
    typedef R (T::*func_type)(Params...);
    T& callee_;
    func_type callbackFunction_;
    delegate_base(func_type mfp, T& callee)
        :   callee_(callee),
            callbackFunction_(mfp)
    {}
public:
    bool operator==(const D& other) const
    {
        return (callee_ == other.callee_)
                && (callbackFunction_ == other.callbackFunction_);
    }
};
/**
 * specialization for member functions
 *
 * \tparam T            class-type of the object who's member function to call
 * \tparam R            return type of the function that gets captured
 * \tparam params       variadic template list for possible arguments
 *                      of the captured function
 */
template <typename T, typename R, typename... Params>
class delegate<R (T::*)(Params...)>
    : public delegate_base<T, R, delegate<R (T::*)(Params...)>, Params...>
{
public:
    typedef delegate_base<T, R, delegate<R (T::*)(Params...)>, Params...> tBase;

    delegate(typename tBase::func_type func, T &callee)
        : tBase(func, callee)
    {
    }

    R operator()(Params... args) const
    {
        return (tBase::callee_.*tBase::callbackFunction_)(args...);
    }
};

/**
 * specialization for const member functions
 */
template <typename T, typename R, typename... Params>
class delegate<R (T::*)(Params...) const>
{
public:
    typedef R (T::*func_type)(Params...) const;

    delegate(func_type func, const T &callee)
        : callee_(&callee)
        , func_(func)
    {
    }

    R operator()(Params... args) const
    {
        return (callee_->*func_)(args...);
    }

    bool operator==(const delegate &other) const
    {
        return (callee_ == other.callee_) && (func_ == other.func_);
    }

private:
    const T *callee_;
    func_type func_;
};

/**
 * specialization for free functions
 *
 * \tparam R            return type of the function that gets captured
 * \tparam params       variadic template list for possible arguments
 *                      of the captured function
 */
template <typename R, typename... Params>
class delegate<R (*)(Params...)>
{
public:
    typedef R (*func_type)(Params...);

    delegate(func_type func)
        : func_(func)
    {
    }

    R operator()(Params... args) const
    {
        return (*func_)(args...);
    }

    bool operator==(const delegate &other) const
    {
        return func_ == other.func_;
    }

private:
    func_type func_;
};

/**
 * function to deduce template parameters from call-context
 */
template <typename F, typename T>
delegate<F> make_delegate(F func, T &obj)
{
    return delegate<F>(func, obj);
}

template <typename T>
delegate<T> make_delegate(T func)
{
    return delegate<T>(func);
}

// a little backward compatilbility layer
#define DELEGATE make_delegate
#define DELEGATE_CONST make_delegate
#define DELEGATE_FREE make_delegate

#else // hmmm...no C++11 support...yak
template <typename T>
class delegate;

template <typename R>
class delegate<R (*)()>
{
public:
    typedef R (*func_type)();

    delegate(func_type func)
        : func_(func)
    {
    }

    R operator()() const
    {
        return (*func_)();
    }

    bool operator==(const delegate &other) const
    {
        return (func_ == other.func_);
    }

private:
    func_type func_;
};
template <typename R, typename P>
class delegate<R (*)(P)>
{
public:
    typedef R (*func_type)(P);

    delegate(func_type func)
        : func_(func)
    {
    }

    R operator()(P x) const
    {
        return (*func_)(x);
    }

    bool operator==(const delegate &other) const
    {
        return (func_ == other.func_);
    }

private:
    func_type func_;
};
/**
 * specialization for member functions with 0 parmeter
 *
 * \tparam T            class-type of the object who's member function to call
 * \tparam R            return type of the function that gets captured
 */
template <typename T, typename R>
class delegate<R (T::*)()>
{
public:
    typedef R (T::*func_type)();

    delegate(func_type func, T &callee)
        : callee_(&callee)
        , func_(func)
    {
    }

    R operator()() const
    {
        return (callee_->*func_)();
    }

    bool operator==(const delegate &other) const
    {
        return (callee_ == other.callee_) && (func_ == other.func_);
    }

private:
    T *callee_;
    func_type func_;
};
template <typename T, typename R>
class delegate<R (T::*)() const>
{
public:
    typedef R (T::*func_type)() const;

    delegate(func_type func, T &callee)
        : callee_(&callee)
        , func_(func)
    {
    }

    R operator()() const
    {
        return (callee_->*func_)();
    }

    bool operator==(const delegate &other) const
    {
        return (callee_ == other.callee_) && (func_ == other.func_);
    }

private:
    T *callee_;
    func_type func_;
};
/**
 * specialization for member functions with 1 parmeter
 *
 * \tparam T            class-type of the object who's member function to call
 * \tparam R            return type of the function that gets captured
 * \tparam P            type of first parameter of the captured function
 */
template <typename T, typename R, typename P>
class delegate<R (T::*)(P)>
{
public:
    typedef R (T::*func_type)(P);

    delegate(func_type func, T &callee)
        : callee_(&callee)
        , func_(func)
    {
    }

    R operator()(P arg) const
    {
        return (callee_->*func_)(arg);
    }

    bool operator==(const delegate &other) const
    {
        return (callee_ == other.callee_) && (func_ == other.func_);
    }

private:
    T *callee_;
    func_type func_;
};
template <typename T, typename R, typename P>
class delegate<R (T::*)(P) const>
{
public:
    typedef R (T::*func_type)(P) const;

    delegate(func_type func, T &callee)
        : callee_(&callee)
        , func_(func)
    {
    }

    R operator()(P arg) const
    {
        return (callee_->*func_)(arg);
    }

    bool operator==(const delegate &other) const
    {
        return (callee_ == other.callee_) && (func_ == other.func_);
    }

private:
    T *callee_;
    func_type func_;
};
template <typename T, typename R, typename P1, typename P2>
class delegate<R (T::*)(P1, P2)>
{
public:
    typedef R (T::*func_type)(P1, P2);

    delegate(func_type func, T &callee)
        : callee_(&callee)
        , func_(func)
    {
    }

    R operator()(P1 arg, P2 arg2) const
    {
        return (callee_->*func_)(arg, arg2);
    }

    bool operator==(const delegate &other) const
    {
        return (callee_ == other.callee_) && (func_ == other.func_);
    }

private:
    T *callee_;
    func_type func_;
};
template <typename T, typename R, typename P1, typename P2, typename P3>
class delegate<R (T::*)(P1, P2, P3)>
{
public:
    typedef R (T::*func_type)(P1, P2, P3);

    delegate(func_type func, T &callee)
        : callee_(&callee)
        , func_(func)
    {
    }

    R operator()(P1 arg, P2 arg2, P3 arg3) const
    {
        return (callee_->*func_)(arg, arg2, arg3);
    }

    bool operator==(const delegate &other) const
    {
        return (callee_ == other.callee_) && (func_ == other.func_);
    }

private:
    T *callee_;
    func_type func_;
};
template <typename T, typename R, typename P1, typename P2, typename P3, typename P4>
class delegate<R (T::*)(P1, P2, P3, P4)>
{
public:
    typedef R (T::*func_type)(P1, P2, P3, P4);

    delegate(func_type func, T &callee)
        : callee_(&callee)
        , func_(func)
    {
    }

    R operator()(P1 arg, P2 arg2, P3 arg3, P4 arg4) const
    {
        return (callee_->*func_)(arg, arg2, arg3, arg4);
    }

    bool operator==(const delegate &other) const
    {
        return (callee_ == other.callee_) && (func_ == other.func_);
    }

private:
    T *callee_;
    func_type func_;
};
/**
 * function to deduce template parameters from call-context
 */
template <typename F, typename T>
delegate<F> make_delegate(F func, T &obj)
{
    return delegate<F>(func, obj);
}

template <typename T>
delegate<T> make_delegate(T func)
{
    return delegate<T>(func);
}

#endif // #if __cplusplus > 199711L

} // namespace delegate

#endif /* end of include guard */
