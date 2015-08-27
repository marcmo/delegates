[![Build Status](https://travis-ci.org/marcmo/delegates.svg?branch=master)](http://travis-ci.org/marcmo/delegates)

delegates
=========

Small delegate implementation. Can be used as header-only library.  
To install, just drop in the Delegate.h and include it.

To build & run the examples, you can do a

    $ make test

## Why would I use delegates?

This implementation is just a tiny header-only library that is

* Fast
* Portable C++ (also supports non C++03)
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

* now works also with const member functions!
* no macros needed anymore!

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

inspired by

* the [The Impossibly Fast C++ Delegates](http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates)
* and [Lightweight Generic C++ Callbacks (or, Yet Another Delegate Implementation)](http://www.codeproject.com/Articles/136799/Lightweight-Generic-C-Callbacks-or-Yet-Another-Del)

## Benchmarks

### Linux/Ubuntu

#### clang 3.4.2

    ~/tmp/delegates.git <master> $ make bench
    clang++ -c -Wall -O0 -g3 -std=c++11 -Iinclude -oBuildDir/benchmark.o src/benchmark.cpp
    clang++ -c -Wall -O0 -g3 -std=c++11 -Iinclude -oBuildDir/benchmark_worker.o src/benchmark_worker.cpp
    clang++ -o BuildDir/benchmark.exe BuildDir/benchmark_worker.o BuildDir/benchmark.o
    ./BuildDir/benchmark.exe
    ********** delegates: **********
    took 745.138018d ms
    counter=97619147
    ********** std::function: **********
    took 3244.926665d ms
    counter=97619147
    ********** std::bind: **********
    took 2197.354233d ms
    counter=97619147
    ********** lambda: **********
    took 646.538980d ms
    counter=97619147

#### gcc 4.8.3

    ~/tmp/delegates.git <master> $ make bench
    g++ -c -Wall -O0 -g3 -std=c++11 -Iinclude -oBuildDir/benchmark.o src/benchmark.cpp
    g++ -c -Wall -O0 -g3 -std=c++11 -Iinclude -oBuildDir/benchmark_worker.o src/benchmark_worker.cpp
    g++ -o BuildDir/benchmark.exe BuildDir/benchmark_worker.o BuildDir/benchmark.o
    ./BuildDir/benchmark.exe
    ********** delegates: **********
    took 628.410903d ms
    counter=97619147
    ********** std::function: **********
    took 3504.065734d ms
    counter=97619147
    ********** std::bind: **********
    took 2948.029087d ms
    counter=97619147
    ********** lambda: **********
    took 535.643261d ms
    counter=97619147

