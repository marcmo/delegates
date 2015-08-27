#include "delegate.h"

using namespace dlgt;

class listener {
public:
    void msgReceived(int len) { /* ... */ }
};

int main(int argc, char const* argv[]) {
    listener l;
    auto d = make_delegate(&listener::msgReceived, l);
    // ...
    d(42);
    return 0;
}




