#define NONIUS_RUNNER
#include <nonius_single.h++>

#include <delegate.h>
#include "benchmark_worker.h"
#include "benchmark_virtualworker.h"

using namespace dlgt;
using namespace bench;

extern int vcounter;

NONIUS_BENCHMARK("*****delegates*****", [](nonius::chronometer meter) {
    Worker w(100);
    auto increaser = make_delegate(&Worker::inc, w);
    auto decreaser = make_delegate(&Worker::dec, w);
    meter.measure([&]() { increaser(); decreaser(); });
})

NONIUS_BENCHMARK("*****std::function*****", [](nonius::chronometer meter) {
    Worker w(100);
    std::function<int(Worker&)> increaser = &Worker::inc;
    std::function<int(Worker&)> decreaser = &Worker::dec;
    meter.measure([&]() { increaser(w); decreaser(w); });
})

NONIUS_BENCHMARK("*****std::bind*****", [](nonius::chronometer meter) {
    Worker w(100);
    auto increaser = std::bind(&Worker::inc, &w);
    auto decreaser = std::bind(&Worker::dec, &w);
    meter.measure([&]() { increaser(w); decreaser(w); });
})

NONIUS_BENCHMARK("*****lambda*****", [](nonius::chronometer meter) {
    Worker w(100);
    auto increaser = std::bind(&Worker::inc, &w);
    auto decreaser = std::bind(&Worker::dec, &w);
    meter.measure([&]() { increaser(w); decreaser(w); });
})

NONIUS_BENCHMARK("*****direct call*****", [](nonius::chronometer meter) {
    Worker w(100);
    meter.measure([&]() { w.inc(); w.dec(); });
})

NONIUS_BENCHMARK("*****virtual call*****", [](nonius::chronometer meter) {
    VirtualWorker vw(100);
    Worker* w = &vw;
    meter.measure([&]() { w->incVirtual(); w->decVirtual(); });
})

