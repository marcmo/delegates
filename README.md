delegates
=========

small delegate implementation. can be used as header-only library.  
To install, just drop in the Delegate.h and include it.

To build & run the examples, you can do a

    $ make test

---

I wanted a delegate implementation that is

* Fast
* Portable C++
* does NOT use dynamic memory allocation
* has a nice syntax
* can handle multiple function signatures

In an older implementation I coded all template specializations from hand. But now with C++11 around variadic templates do the job just fine!

simple usage example:

    class A
    {
    public:
        int foo(int x) { return x*x; }
    };
    ...
    A a;
    auto d = DELEGATE(&A::foo, a);
    d(42);

Delegates can be passed along and be stored for later usage:

    typedef Delegate<int(int)> ServiceDelegate;

    class Service
    {
    public:
        void registerDelegate(ServiceDelegate& d) {
            mD = &d;
        }
        void notifyDelegate(int x) {
            (*mD)(x);
        }

    private:
        ServiceDelegate* mD;
    };

    auto d = DELEGATE(&A::foo, a);
    Service s;
    registerDelegate(d);
    ...
    s.notifyDelegate(42);

inspired by the [The Impossibly Fast C++ Delegates](http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates) and [Lightweight Generic C++ Callbacks (or, Yet Another Delegate Implementation)](http://www.codeproject.com/Articles/136799/Lightweight-Generic-C-Callbacks-or-Yet-Another-Del)

