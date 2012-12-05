/**
 * Main template for delgates
 *
 * \tparam return_type  return type of the function that gets captured
 * \tparam params       variadic template list for possible arguments
 *                      of the captured function
 */
template<typename return_type, typename... params>
class Delegate
{
    typedef return_type (*Pointer2Function)(void* callee, params...);
public:
    Delegate(void* callee, Pointer2Function function)
        : fpCallee(callee)
        , fpCallbackFunction(function)
    {}

    return_type operator()(params... xs) const
    {
        return (*fpCallbackFunction)(fpCallee, xs...);
    }

    bool operator==(const Delegate& other) const
    {
        return (fpCallee == other.fpCallee)
               && (fpCallbackFunction == other.fpCallbackFunction);
    }

private:

    void* fpCallee;
    Pointer2Function fpCallbackFunction;
};

template<typename T, typename return_type, typename... params>
struct HelperTag
{
    template<return_type (T::*Func)(params...)>
    static return_type Wrapper(void* o, params... xs)
    {
        return (static_cast<T*>(o)->*Func)(xs...);
    }

    template<return_type (T::*Func)(params...)>
    inline static Delegate<return_type, params...> Bind(T* o)
    {
        return Delegate<return_type, params...>(o, &HelperTag::Wrapper<Func>);
    }
};
template<typename T, typename return_type, typename... params>
HelperTag<T, return_type, params... > Helper(return_type (T::*)(params...))
{
    return HelperTag<T, return_type, params...>();
}

#define DELEGATE(func, thisPrt) (Helper(func).Bind<func>(thisPrt))


