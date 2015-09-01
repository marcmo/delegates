#include "benchmark_virtualworker.h"

int vcounter;

namespace bench
{
int VirtualWorker::incVirtual()
{
    return ++vcounter;
}
int VirtualWorker::decVirtual()
{
    return --vcounter;
}
} // namespace bench
