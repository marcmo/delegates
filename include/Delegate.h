/**
 * Main template for delgates
 *
 * \tparam return_type  return type of the function that gets captured
 * \tparam params       variadic template list for possible arguments
 *                      of the captured function
 */
template<typename return_type, typename... params>
class Delegate; //forward declaration..

template<typename return_type, typename... params>
class BaseDelegate
{
protected:
    typedef return_type (*Pointer2Function)(void* callee, params...);
public:
    BaseDelegate(void* callee, Pointer2Function function)
        : fpCallee(callee)
        , fpCallbackFunction(function)
    {}

    return_type operator()(params... xs) const
    {
        return (*fpCallbackFunction)(fpCallee, xs...);
    }

    bool operator==(const BaseDelegate& other) const
    {
        return (fpCallee == other.fpCallee)
               && (fpCallbackFunction == other.fpCallbackFunction);
    }

private:
    void* fpCallee;
    Pointer2Function fpCallbackFunction;
};
template<typename return_type, typename... params>
class Delegate<return_type(params...)>
    : public BaseDelegate<return_type, params...>
{
    typedef BaseDelegate<return_type, params...> Base;
public:
    Delegate(void* callee, typename Base::Pointer2Function function)
        : Base(callee, function)
    {}
};

template<typename return_type, typename... params>
class Delegate<return_type(params...) const>
    : public BaseDelegate<return_type, params...>
{
    typedef BaseDelegate<return_type, params...> Base;
public:
    Delegate(void* callee, typename Base::Pointer2Function function)
        : Base(callee, function)
    {}
};

/**
 * A DelegateFactory is used to create a Delegate for a certain method call.
 * It takes care of setting up the function that will cast the object that is stored
 * inside the Delegate back to the correct type.
 */
template<typename T, typename return_type, typename... params>
struct DelegateFactory
{
    template<return_type (T::*Func)(params...)>
    static return_type MethodCaller(void* o, params... xs)
    {
        return (static_cast<T*>(o)->*Func)(xs...);
    }
    template<return_type (T::*Func)(params...) const>
    static return_type MethodCallerC(void* o, params... xs)
    {
        return (static_cast<T*>(o)->*Func)(xs...);
    }

    template <return_type (*TFnctPtr)(params...)>
	static return_type FunctionCaller(void*, params... xs)
	{
		return (TFnctPtr)(xs...);
	}

    template<return_type (T::*Func)(params...)>
    inline static Delegate<return_type(params...)> Create(T* o)
    {
        return Delegate<return_type(params...)>(o, &DelegateFactory::MethodCaller<Func>);
    }

    template<return_type (T::*Func)(params...) const>
    inline static Delegate<return_type(params...) const> CreateC(T* o)
    {
        return Delegate<return_type(params...) const>(o, &DelegateFactory::MethodCallerC<Func>);
    }

    template<return_type (*TFnctPtr)(params...)>
    inline static Delegate<return_type(params...)> CreateForFunction()
    {
        return Delegate<return_type(params...)>(0L, &DelegateFactory::FunctionCaller<TFnctPtr>);
    }
};
/**
 * helper function that is used to deduce the template arguments.
 * will return a DelegateFactory
 */
template<typename T, typename return_type, typename... params>
DelegateFactory<T, return_type, params... > MakeDelegate(return_type (T::*)(params...))
{
    return DelegateFactory<T, return_type, params...>();
}
template<typename T, typename return_type, typename... params>
DelegateFactory<T, return_type, params... > MakeDelegateC(return_type (T::*)(params...) const)
{
    return DelegateFactory<T, return_type, params...>();
}
class no_type{};
template<typename return_type, typename... params>
DelegateFactory<no_type, return_type, params... > MakeDelegateFree(return_type (*TFnctPtr)(params...))
{
    return DelegateFactory<no_type, return_type, params...>();
}

#define DELEGATE(func, thisPrtRef) (MakeDelegate(func).Create<func>(&thisPrtRef))
#define DELEGATE_CONST(func, thisPrtRef) (MakeDelegateC(func).CreateC<func>(&thisPrtRef))
#define DELEGATE_FREE(func) (MakeDelegateFree(func).CreateForFunction<func>())


