#include <iostream>
#include "include/smartList.hpp"

using std::cout;

const auto print = [](const auto N) { cout << N << ' '; };

int main()
{
    SmartList<int> list;
    std::cout << std::boolalpha << list.size() << ' ' << list.empty() << '\n';
    list.insert({ 2, 1, 1, 3, 4, 5 });
    list.insert(6);
    list.forEach(print);
    cout << '\n';
    std::cout << std::boolalpha << list.size() << ' ' << list.empty() << '\n';

    list.remove(1);
    list.forEach(print);
    cout << '\n';

    list.forEach([](auto &num) { num *= 10; });
    list.forEach(print);
    cout << '\n';


    list.reverse();
    list.forEach(print);
    cout << '\n';
}
