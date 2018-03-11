// Invocation line: g++ -std=c++17 -O3

#include <tuple>
extern "C" void f(int *out);
int c() {
    int r;
    f(&r);
    return r;
}
auto cpp() {
    std::tuple<int> r;
    f(&std::get<0>(r));
    return r;
}