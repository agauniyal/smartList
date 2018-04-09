#include <iostream>
#include "include/smartList.hpp"

using std::cout;

int main()
{
    SmartList<int> list;
    list.insert({ 1, 2, 3, 4, 5 });
    list.insert(6);

    const auto print = [](const auto N) { cout << N << ' '; };
    const auto mul10 = [](auto &num) { num *= 10; };

    list.forEach(mul10);
    list.forEach(print);
    cout << '\n';

    list.reverse();
    list.forEach(print);
    cout << '\n';
}
