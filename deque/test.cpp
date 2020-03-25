#include "deque.hpp"
#include <iostream>

using namespace std;

int main() {
    sjtu::deque<int> d;
    for(int i = 1;i <= 400;++ i)
        d.push_back(i);
    cout << d.at(300) << d.back() << endl;
    return 0;
}