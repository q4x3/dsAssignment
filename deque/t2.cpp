#include <iostream>

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
    A awsl;
    A *p = &awsl;
    A::C awslcao(p);
    cout << awslcao.test() << endl;
}