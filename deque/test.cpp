// provided by your new friend Mercy. Enjoy!

#include <iostream>
#include <ctime>
#include <vector>
#include <deque>
#include <random>

#include "class-integer.hpp"
#include "class-matrix.hpp"
#include "class-bint.hpp"
#include "deque.hpp"

std::default_random_engine randnum(time(NULL));

static const int MAX_N = 15000;

template <typename Ans, typename Test>
bool isEqual(Ans &ans, Test &test) {
    if (ans.size() != test.size())
        return false;

    if (ans.empty()) return true;

    for (int i = 0; i < ans.size(); i++) {
        if (randnum() % 2) {
            if (ans[i] != test[i]) return false;
        } else {
            if (ans.at(i) != test.at(i)) return false;
        }
    }

    if (ans.empty() != test.empty() || ans.front() != test.front() ||
        ans.back() != test.back())
        return false;

    return true;
}

int ansCounter = 0, myCounter = 0, noUseCounter = 0;
// you should construct and deconstruct this class correctly
class DynamicType {
public:
    int *pct;
    double *data;
    DynamicType (int *p) : pct(p) , data(new double[2]) {
        (*pct)++;
    }
    DynamicType (const DynamicType &other) : pct(other.pct), data(new double[2]) {
        (*pct)++;
    }
    DynamicType &operator =(const DynamicType &other) {
        if (this == &other) return *this;
        (*pct)--;
        pct = other.pct;
        (*pct)++;
        delete [] data;
        data = new double[2];
        return *this;
    }
    ~DynamicType() {
        delete [] data;
        (*pct)--;
    }
    bool operator != (const DynamicType &rhs) const { return false; }
};

bool memoryTest() {
    // you should call the constructor and deconstructor correctly
    std::deque<DynamicType> ans;
    sjtu::deque<DynamicType> deq;

    for (int i = 0; i < MAX_N; i++) {
        ans.push_back(DynamicType(&ansCounter));
        deq.push_back(DynamicType(&myCounter));
    }

    for (int i = 0; i < MAX_N / 10; i++) {
        int index = randnum() % ans.size();
        switch(randnum() % 6) {
            case 0: ans.push_back(DynamicType(&ansCounter));
                deq.push_back(DynamicType(&myCounter));
                break;
            case 1: ans.push_front(DynamicType(&ansCounter));
                deq.push_front(DynamicType(&myCounter));
                break;
            case 2: ans.pop_back(); deq.pop_back();
                break;
            case 3: ans.pop_front(); deq.pop_front();
                break;
            case 4: ans.insert(ans.begin() + index, DynamicType(&ansCounter));
                deq.insert(deq.begin() + index, DynamicType(&myCounter));
                break;
            case 5: ans.erase(ans.begin() + index);
                deq.erase(deq.begin() + index);
                break;
            default : break;
        }

        if (ansCounter != myCounter)
            return false;
    }

    return isEqual(ans, deq);
}


int main() {
    std::cout << memoryTest() << std::endl;
    return 0;
}