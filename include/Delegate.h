
template<typename return_type, typename... params>
class Delegate
{
    typedef return_type (*Type)(void* callee, params... );
public:
    Delegate(void* callee, Type function)
        : fpCallee(callee)
        , fpCallbackFunction(function)
    {}

    template <class T, return_type (T::*TMethod)(params... )>
    static Delegate from_function(T* callee)
    {
        Delegate d(callee, &methodCaller<T, TMethod>);
        return d;
    }

    return_type operator()(params... xs) const
    {
        return (*fpCallbackFunction)(fpCallee, xs... );
    }

private:

    void* fpCallee;
    Type fpCallbackFunction;

    template <class T, return_type (T::*TMethod)(params... )>
    static return_type methodCaller(void* callee, params... xs)
    {
        T* p = static_cast<T*>(callee);
        return (p->*TMethod)(xs... );
    }
};
#define BIND(func, thisPrt) (Helper(func).Bind<func>(thisPrt))
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


