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

### Mac OS Yosemite

#### clang 3.6.0

    clang++ -c -O3 -std=c++11 -Iinclude -Ibenchmarks/include -Ibenchmarks/include/nonius -c -o BuildDir/bench.o benchmarks/src/bench.cpp
    clang++ -c -O3 -std=c++11 -Iinclude -Ibenchmarks/include -Ibenchmarks/include/nonius -c -o BuildDir/benchmark_worker.o benchmarks/src/benchmark_worker.cpp
    clang++ BuildDir/bench.o BuildDir/benchmark_worker.o -o BuildDir/benchmark.exe
    ./BuildDir/benchmark.exe -s 1000
    clock resolution: mean is 25.5675 ns (20480002 iterations)

    benchmarking *****delegates*****
    collecting 1000 samples, 6569 iterations each, in estimated 19.707 ms
    mean: 3.538 ns, lb 3.51967 ns, ub 3.56083 ns, ci 0.95
    std dev: 0.330145 ns, lb 0.281938 ns, ub 0.387139 ns, ci 0.95
    found 95 outliers among 1000 samples (9.5%)
    variance is severely inflated by outliers

    benchmarking *****std::function*****
    collecting 1000 samples, 3153 iterations each, in estimated 25.224 ms
    mean: 6.01022 ns, lb 5.98227 ns, ub 6.04701 ns, ci 0.95
    std dev: 0.514202 ns, lb 0.423941 ns, ub 0.640059 ns, ci 0.95
    found 30 outliers among 1000 samples (3%)
    variance is severely inflated by outliers

    benchmarking *****std::bind*****
    collecting 1000 samples, 4165 iterations each, in estimated 24.99 ms
    mean: 3.45979 ns, lb 3.44323 ns, ub 3.48198 ns, ci 0.95
    std dev: 0.306551 ns, lb 0.248918 ns, ub 0.371409 ns, ci 0.95
    found 20 outliers among 1000 samples (2%)
    variance is severely inflated by outliers

    benchmarking *****lambda*****
    collecting 1000 samples, 6350 iterations each, in estimated 25.4 ms
    mean: 4.71213 ns, lb 4.64886 ns, ub 4.78463 ns, ci 0.95
    std dev: 1.09138 ns, lb 0.970559 ns, ub 1.24622 ns, ci 0.95
    found 22 outliers among 1000 samples (2.2%)
    variance is severely inflated by outliers

    benchmarking *****direct call*****
    collecting 1000 samples, 3764 iterations each, in estimated 22.584 ms
    mean: 3.48541 ns, lb 3.47389 ns, ub 3.5054 ns, ci 0.95
    std dev: 0.237423 ns, lb 0.156561 ns, ub 0.343597 ns, ci 0.95
    found 9 outliers among 1000 samples (0.9%)
    variance is severely inflated by outliers

#### gcc 5.2

    g++-5 -c -O3 -std=c++11 -Iinclude -Ibenchmarks/include -Ibenchmarks/include/nonius -c -o BuildDir/bench.o benchmarks/src/bench.cpp
    g++-5 -c -O3 -std=c++11 -Iinclude -Ibenchmarks/include -Ibenchmarks/include/nonius -c -o BuildDir/benchmark_worker.o benchmarks/src/benchmark_worker.cpp
    g++-5 BuildDir/bench.o BuildDir/benchmark_worker.o -o BuildDir/benchmark.exe
    ./BuildDir/benchmark.exe -s 1000
    clock resolution: mean is 43.144 ns (20480002 iterations)

    benchmarking *****delegates*****
    collecting 1000 samples, 11981 iterations each, in estimated 35.943 ms
    mean: 3.81396 ns, lb 3.7799 ns, ub 3.8506 ns, ci 0.95
    std dev: 0.570816 ns, lb 0.534648 ns, ub 0.609415 ns, ci 0.95
    found 70 outliers among 1000 samples (7%)
    variance is severely inflated by outliers

    benchmarking *****std::function*****
    collecting 1000 samples, 7364 iterations each, in estimated 36.82 ms
    mean: 6.2997 ns, lb 6.24172 ns, ub 6.36502 ns, ci 0.95
    std dev: 0.99184 ns, lb 0.907838 ns, ub 1.13345 ns, ci 0.95
    found 262 outliers among 1000 samples (26.2%)
    variance is severely inflated by outliers

    benchmarking *****std::bind*****
    collecting 1000 samples, 11402 iterations each, in estimated 34.206 ms
    mean: 3.54473 ns, lb 3.5307 ns, ub 3.56306 ns, ci 0.95
    std dev: 0.25708 ns, lb 0.21151 ns, ub 0.327045 ns, ci 0.95
    found 48 outliers among 1000 samples (4.8%)
    variance is severely inflated by outliers

    benchmarking *****lambda*****
    collecting 1000 samples, 11981 iterations each, in estimated 35.943 ms
    mean: 3.54202 ns, lb 3.52283 ns, ub 3.56506 ns, ci 0.95
    std dev: 0.340631 ns, lb 0.298032 ns, ub 0.421152 ns, ci 0.95
    found 89 outliers among 1000 samples (8.9%)
    variance is severely inflated by outliers

    benchmarking *****direct call*****
    collecting 1000 samples, 12188 iterations each, in estimated 36.564 ms
    mean: 3.77494 ns, lb 3.73613 ns, ub 3.82171 ns, ci 0.95
    std dev: 0.686637 ns, lb 0.602415 ns, ub 0.883583 ns, ci 0.95
    found 175 outliers among 1000 samples (17.5%)
    variance is severely inflated by outliers

