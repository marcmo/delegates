namespace dlgt
{
/**
 * non specialized template declaration for delegate
 */
template <typename T>
class delegate;

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
{
public:
    typedef R (T::*func_type)(Params...);

    delegate(func_type func, T &callee)
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
    T *callee_;
    func_type func_;
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
} // namespace delegate
