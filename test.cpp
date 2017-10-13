#include <iostream>
#include "smartList.hpp"

using std::cout;
using std::endl;

int main()
{
    SL<int> list;
    list.insert({ 1, 2, 3, 4, 5 });

    list.forEach([](auto &num) { num *= 10; });
    list.forEach([](const auto number) { cout << number << ' '; });
    cout << '\n';

    list.reverse();
    list.forEach([](const auto number) { cout << number << ' '; });
    cout << '\n';
}
