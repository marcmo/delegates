delegates
=========

small delegate implementation. can be used as header-only library.  
To install, just drop in the Delegate.h and include it.

To build & run the examples, you can do a

    $ make test

---

## Why would I want delegates?

This implementation is just a tiny library that is

* Fast
* Portable C++
* header only (nothing to link or build)
* does NOT use dynamic memory allocation
* has a nice syntax
* can handle multiple function signatures
* suitable for embedded systems

In an older implementation I coded all template specializations from hand. But now with C++11 around variadic templates do the job just fine!

simple usage example:

```cpp
class A
{
public:
    int foo(int x) { return x*x; }
};
...
A a;
auto d = make_delegate(&A::foo, a);
d(42);
```

Delegates can be passed along and be stored for later usage:

```cpp
typedef delegate<int(*)(int)> ServiceDelegate;

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

auto d = make_delegate(&A::foo, a);
Service s;
registerDelegate(d);
...
s.notifyDelegate(42);
```

## Update

now works also with const member functions!

## Usage

Say you have a class A with a couple of member functions

```cpp
class A
{
public:
    int square(int x);
    int square_const(int x) const;
    ...
};
```

To create a delegate (callable entity that can be passed around by value):

### for regular member functions

```cpp
auto d = make_delegate(&A::square, a);
```

### for const member functions

```cpp
auto d = make_delegate(&A::square_const, a);
```

### for free functions

```cpp
static void myFreeFunction(){ ... }
...
auto d = make_delegate(&myFreeFunction);
```

Further examples can be found in the test directory. To build and execute the tests, just type

    $ make test

## Credits

inspired by the [The Impossibly Fast C++ Delegates](http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates) and [Lightweight Generic C++ Callbacks (or, Yet Another Delegate Implementation)](http://www.codeproject.com/Articles/136799/Lightweight-Generic-C-Callbacks-or-Yet-Another-Del)

