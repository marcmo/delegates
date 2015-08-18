#ifndef BENCHMARK_WORKER_H_
#define BENCHMARK_WORKER_H_

namespace bench
{
class Worker
{
public:
    Worker(int s);
    int inc();
    int dec();
};
} // namespace bench

#endif /* end of include guard */
