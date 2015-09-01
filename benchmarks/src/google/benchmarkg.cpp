#include "benchmark/benchmark_api.h"
#include <functional>
#include <delegate.h>
#include "benchmark_worker.h"
#include "benchmark_virtualworker.h"

using namespace dlgt;
using namespace bench;

extern int vcounter;
enum { REPETITIONS = 100 };

static void Delegates(benchmark::State &state)
{
    Worker w(100);
    auto increaser = make_delegate(&Worker::inc, w);
    auto decreaser = make_delegate(&Worker::dec, w);
    while (state.KeepRunning()) {
        for (int i = 0; i < REPETITIONS; i++) {
            increaser();
            decreaser();
        }
    }
}
BENCHMARK(Delegates);

static void StdFunction(benchmark::State &state)
{
    Worker w(100);
    std::function<int(Worker &)> increaser = &Worker::inc;
    std::function<int(Worker &)> decreaser = &Worker::dec;
    while (state.KeepRunning()) {
        for (int i = 0; i < REPETITIONS; i++) {
            increaser(w);
            decreaser(w);
        }
    }
}
BENCHMARK(StdFunction);

static void StdBind(benchmark::State &state)
{
    Worker w(100);
    auto increaser = std::bind(&Worker::inc, &w);
    auto decreaser = std::bind(&Worker::dec, &w);
    while (state.KeepRunning()) {
        for (int i = 0; i < REPETITIONS; i++) {
            increaser(w);
            decreaser(w);
        }
    }
}
BENCHMARK(StdBind);

static void Lambda(benchmark::State &state)
{
    Worker w(100);
    auto increaser = std::bind(&Worker::inc, &w);
    auto decreaser = std::bind(&Worker::dec, &w);
    while (state.KeepRunning()) {
        for (int i = 0; i < REPETITIONS; i++) {
            increaser(w);
            decreaser(w);
        }
    }
}
BENCHMARK(Lambda);

static void DirectCall(benchmark::State &state)
{
    Worker w(100);
    while (state.KeepRunning()) {
        for (int i = 0; i < REPETITIONS; i++) {
            w.inc();
            w.dec();
        }
    }
}
BENCHMARK(DirectCall);

static void VirtualCall(benchmark::State &state)
{
    VirtualWorker vw(100);
    Worker *w = &vw;
    while (state.KeepRunning()) {
        for (int i = 0; i < REPETITIONS; i++) {
            w->incVirtual();
            w->decVirtual();
        }
    }
}
BENCHMARK(VirtualCall);

static void DelegatesStatic(benchmark::State &state)
{
    auto increaser = make_delegate(&Worker::incStatic);
    auto decreaser = make_delegate(&Worker::decStatic);
    while (state.KeepRunning()) {
        for (int i = 0; i < REPETITIONS; i++) {
            increaser();
            decreaser();
        }
    }
}
BENCHMARK(DelegatesStatic);

static void StdFunctionStatic(benchmark::State &state)
{
    auto increaser = &Worker::incStatic;
    auto decreaser = &Worker::decStatic;
    while (state.KeepRunning()) {
        for (int i = 0; i < REPETITIONS; i++) {
            increaser();
            decreaser();
        }
    }
}
BENCHMARK(StdFunctionStatic);

static void StdBindStatic(benchmark::State &state)
{
    auto increaser = std::bind(&Worker::incStatic);
    auto decreaser = std::bind(&Worker::decStatic);
    while (state.KeepRunning()) {
        for (int i = 0; i < REPETITIONS; i++) {
            increaser();
            decreaser();
        }
    }
}
BENCHMARK(StdBindStatic);

static void LambdaStatic(benchmark::State &state)
{
    auto increaser = std::bind(&Worker::incStatic);
    auto decreaser = std::bind(&Worker::decStatic);
    while (state.KeepRunning()) {
        for (int i = 0; i < REPETITIONS; i++) {
            increaser();
            decreaser();
        }
    }
}
BENCHMARK(LambdaStatic);

static void DirectCallStatic(benchmark::State &state)
{
    while (state.KeepRunning()) {
        for (int i = 0; i < REPETITIONS; i++) {
            Worker::incStatic();
            Worker::decStatic();
        }
    }
}
BENCHMARK(DirectCallStatic);

BENCHMARK_MAIN();
