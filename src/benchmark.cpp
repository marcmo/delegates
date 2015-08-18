/*
 * some benchmarks comparing delegates to
 *      * std::function
 *      * std::bind
 *      * lambdas
 */
#include <iostream>
#include <chrono>
#include <functional>
#include "delegate.h"
#include "benchmark_worker.h"

using namespace std;
using namespace dlgt;

extern int counter;

namespace bench
{

class reporter
{
public:
    void report(chrono::duration<double, milli> d)
    {
        printf("took %lfd ms\n", d.count());
    }
};
typedef std::chrono::duration<int, std::milli> milliseconds_type;
class benchmark
{
public:
    typedef delegate<void (reporter::*)(chrono::duration<double, milli>)> callback;
    benchmark(callback c)
        : start_(chrono::steady_clock::now())
        , callback_(c)
    {
    }
    ~benchmark()
    {
        auto end = chrono::steady_clock::now();
        auto duration = chrono::duration<double, milli>(end - start_);
        callback_(duration);
    }
    chrono::steady_clock::time_point start_;
    callback callback_;
};

enum { REPETITIONS = 100 * 1000 * 1000 };

template <typename F1, typename F2>
void benchmarkLoop(F1 increaser, F2 decreaser)
{
    reporter r;
    {
        benchmark b(make_delegate(&reporter::report, r));

        for (int i = 0; i < REPETITIONS; i++) {
            increaser();
            if (i % 42 == 0)
                decreaser();
        }
    }
    printf("counter=%d\n", counter);
}
} // namespace bench

using namespace bench;

int main()
{
    printf("********** delegates: **********\n");
    Worker w(100);
    auto increaser = make_delegate(&Worker::inc, w);
    auto decreaser = make_delegate(&Worker::dec, w);
    benchmarkLoop(increaser, decreaser);

    printf("********** std::function: **********\n");
    reporter r;
    Worker w2(100);
    std::function<int(Worker &)> increaser2 = &Worker::inc;
    std::function<int(Worker &)> decreaser2 = &Worker::dec;
    {
        benchmark b(make_delegate(&reporter::report, r));

        for (int i = 0; i < REPETITIONS; i++) {
            increaser2(w2);
            if (i % 42 == 0)
                decreaser2(w2);
        }
    }
    printf("counter=%d\n", counter);

    printf("********** std::bind: **********\n");
    Worker w3(100);
    auto increaser3 = bind(&Worker::inc, &w3);
    auto decreaser3 = bind(&Worker::dec, &w3);
    benchmarkLoop(increaser3, decreaser3);

    printf("********** lambda: **********\n");
    Worker w4(100);
    auto incLambda = [&w4]() { return w4.inc(); };
    auto decLambda = [&w4]() { return w4.dec(); };
    benchmarkLoop(incLambda, decLambda);

    return 0;
}
