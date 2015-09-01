#ifndef BENCHMARK_VIRTUAL_WORKER_H_
#define BENCHMARK_VIRTUAL_WORKER_H_

#include "benchmark_worker.h"

namespace bench
{
class VirtualWorker : public Worker
{
public:

    VirtualWorker(int s)
        : Worker(s)
    {}
    virtual int incVirtual();
    virtual int decVirtual();
};

} // namespace bench

#endif /* end of include guard */
