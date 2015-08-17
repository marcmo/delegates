/**
 * Main template for delgates
 *
 * \tparam return_type  return type of the function that gets captured
 * \tparam params       variadic template list for possible arguments
 *                      of the captured function
 */

template<typename T>
class delegate
{};
template<typename T, typename R, typename... Params>
class delegate<R (T::*)(Params...)>
{
public:
    typedef R (T::*func_type)(Params...);

    delegate(func_type func, T& callee)
        : fpCallee(&callee)
        , fpFunc(func)
    {}

    R operator()(Params... args) const
    {
        return (fpCallee->*fpFunc)(args...);
    }

    bool operator==(const delegate& other) const
    {
        return (fpCallee == other.fpCallee)
               && (fpFunc == other.fpFunc);
    }

private:
    T* fpCallee;
    func_type fpFunc;
};

template<typename T, typename R, typename... Params>
class delegate<R (T::*)(Params...) const>
{
public:
    typedef R (T::*func_type)(Params...) const;

    delegate(func_type func, const T& callee)
        : fpCallee(&callee)
        , fpFunc(func)
    {}

    R operator()(Params... args) const
    {
        return (fpCallee->*fpFunc)(args...);
    }

    bool operator==(const delegate& other) const
    {
        return (fpCallee == other.fpCallee)
               && (fpFunc == other.fpFunc);
    }

private:
    const T* fpCallee;
    func_type fpFunc;
};

template<typename R, typename... Params>
class delegate<R (*)(Params...)>
{
public:
    typedef R (*func_type)(Params...);

    delegate(func_type func)
        : fpFunc(func)
    {}

    R operator()(Params... args) const
    {
        return (*fpFunc)(args...);
    }

    bool operator==(const delegate& other) const
    {
        return fpFunc == other.fpFunc;
    }

private:
    func_type fpFunc;
};

template<typename F, typename T>
delegate<F> make_delegate(F func, T& obj)
{
    return delegate<F>(func, obj);
}

template<typename T>
delegate<T> make_delegate(T func)
{
    return delegate<T>(func);
}


