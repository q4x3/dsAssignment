#include <iostream>
#include <deque>
#include "deque.hpp"

using namespace std;

class B {
    friend class A;
    int b;
};

class A {
    private:
    B* a;
    public:
    class C {
        friend class A;
        A* c;
    public:
        C(A* a):c(a) {}
        int test() {
            return c->a->b;
        }
    };
    int test(C cc) {
        cout << cc.test() << endl;
    }
};

int main() {
    /*A awsl;
    A *p = &awsl;
    A::C awslcao(p);
    cout << awslcao.test() << endl;*/
    sjtu::deque<int> ddint;
    /*for(int i = 0;i < 20;++ i) {
        ddint.push_back(i);
    }
    for(int i = 0;i < 20;++ i) {
        ddint.pop_front();
    }
    for(auto a : ddint)
        cout << a << endl;
    cout << endl;*/
}