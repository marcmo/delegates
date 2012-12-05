delegates
=========

small delegate implementation

I wanted a delegate implementation that is

* Fast
* Portable C++
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
    auto d = DELEGATE(&A::foo, &a);
    d(42);

inspired from the [The Impossibly Fast C++ Delegates](http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates) and [Lightweight Generic C++ Callbacks (or, Yet Another Delegate Implementation)](http://www.codeproject.com/Articles/136799/Lightweight-Generic-C-Callbacks-or-Yet-Another-Del)

