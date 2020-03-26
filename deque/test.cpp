#include "deque.hpp"

#include <ctime>
#include <iostream>
#include <deque>
#include <random>
#include <string>
#include <vector>
#include <ctime>

//std::default_random_engine randnum(time(NULL));

static const int N = 10000;
static const int N_SPEED = 33500;

typedef std::pair<const char *, std::pair<bool, double> (*)()> CheckerPair;

class Int{
private:
    int data;

public:
    Int() = default;
    Int(const int &data) : data(data) {}
    Int & operator =(const Int &rhs) = default;
    bool operator <(const Int &rhs) = delete;
    bool operator >(const Int &rhs) = delete;
    bool operator <=(const Int &rhs) = delete;
    bool operator >=(const Int &rhs) = delete;
    bool operator ==(const Int &rhs)const {
        return data == rhs.data;
    }
    bool operator !=(const Int &rhs)const {
        return data != rhs.data;
    }

};

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

class Timer{
private:
    long dfnStart, dfnEnd;

public:
    void init() {
        dfnEnd = dfnStart = clock();
    }
    void stop() {
        dfnEnd = clock();
    }
    double getTime() {
        return 1.0 * (dfnEnd - dfnStart) / CLOCKS_PER_SEC;
    }

};

Timer timer;

bool isEqual(std::deque<Int> &a, sjtu::deque<Int> &b) {
    static std::vector<Int> resultA, resultB;
    resultA.clear();
    resultB.clear();
    for (auto x : a) resultA.push_back(x);
    for (auto x : b) resultB.push_back(x);
    if (resultA.size() != resultB.size()) return false;
    for (int i = 0; i < (int)resultA.size(); i++) {
        if (resultA[i] != resultB[i]) return false;
    }
    return true;
}

std::pair<bool, double> popFrontChecker() {
    std::deque<Int> a;
    sjtu::deque<Int> b;
    for (int i = 0; i < N; i++) {
        int pos = rand() % (a.size() + 1);
        int tmp = rand();
        a.push_back(tmp);
        b.push_back(tmp);
    }
    timer.init();
    for (int i = 0; i < N; i++) {
        a.pop_front();
        b.pop_front();
    }
    timer.stop();
    if (!isEqual(a, b)) {
        return std::make_pair(false, 0);
    } else {
        return std::make_pair(true, timer.getTime());
    }
}

std::pair<bool, double> popFrontTimer() {
    sjtu::deque<int> a;
    for (int i = 0; i < N_SPEED; i++) {
        int op = rand() % 3;
        if (op == 0) a.push_back(rand());
        else if (op == 1) a.push_front(rand());
        else if (op == 2) a.insert(a.begin() + rand() % (a.size() + 1), rand());
    }
    timer.init();
    for (int i = 0; i < N_SPEED; i++) {
        a.pop_front();
    }
    timer.stop();
    return std::make_pair(true, timer.getTime());
}

static CheckerPair TEST_A[] = {
    //std::make_pair("Push Series -> push_back operation testing...", pushBackChecker),
    //std::make_pair("Push Series -> push_front operation testing...", pushFrontChecker),
    //std::make_pair("Pop Series -> pop_back operation testing...", popBackChecker),
    std::make_pair("Pop Series -> pop_front operation testing...", popFrontChecker),
    //std::make_pair("Push Series -> insert operation testing...", insertChecker),
    //std::make_pair("Pop Series -> erase operation testing...", eraseChecker),
    //std::make_pair("Visitation Series -> at operation testing...", atChecker),
    //std::make_pair("Visitation Series -> constantly at operation testing...", constAtChecker),
    //std::make_pair("Visitation Series -> [] operator testing...", bracketChecker),
    //std::make_pair("Visitation Series -> constantly [] operator testing...", constBracketChecker),
    //std::make_pair("Visitation Series -> front operation testing...", frontChecker),
    //std::make_pair("Visitation Series -> back operator testing...", backChecker),
    //std::make_pair("Iterator Series -> +n operator testing...", iteratorAddNChecker),
    //std::make_pair("Iterator Series -> -n operator testing...", iteratorRedNChecker),
    //std::make_pair("Iterator Series -> +=n operator testing...", iteratorAddENChecker),
    //std::make_pair("Iterator Series -> -=n operator testing...", iteratorRedENChecker),
    //std::make_pair("Iterator Serise: Prefix ++ operator testing...", iteratorPAddOneChecker),
    //std::make_pair("Iterator Series -> Prefix -- operator testing...", iteratorPRedOneChecker),
    //std::make_pair("Iterator Series -> Suffix ++ operator testing...", iteratorSAddOneChecker),
    //std::make_pair("Iterator Series -> Suffix -- operator testing...", iteratorSRedOneChecker),
    //std::make_pair("Iterator Series -> - operator testing...", iteratorMinusOperatorChecker),
    //std::make_pair("Iterator Series -> Iterator insert persistence testing...", iteratorInsertPersistenceChecker),
    //std::make_pair("Iterator Series -> Iterator erase persistence testing...", iteratorErasePersistenceChecker),
    //std::make_pair("Robustness Series -> Invalid visitation by []...", errorBracketChecker),
    //std::make_pair("Robustness Series -> Invalid visitation by at...", errorAtChecker),
    //std::make_pair("Robustness Series -> Invalid visitation by iterator...", errorIteratorChecker),
    //std::make_pair("Robustness Series -> Invalid insert operation...", errorInsertChecker),
    //std::make_pair("Robustness Series -> Invalid erase operation...", errorEraseChecker),
    //std::make_pair("Basic Series -> size function testing...", sizeChecker),
    //std::make_pair("Basic Series -> empty function testing...", emptyChecker),
    //std::make_pair("Independence Series -> Copy constructor testing...", copyConstructorChecker),
    //std::make_pair("Independence Series -> Operator = testing...", equalOperatorChecker),
    //std::make_pair("Memory Series -> Memory leak testing...", memoryLeakChecker),
    //std::make_pair("Synthesis Series -> Comprehensive testing...", synthesisChecker)
};

static CheckerPair TEST_B[] = {
    //std::make_pair("push_back", pushBackTimer),
    //std::make_pair("pop_back", popBackTimer),
    //std::make_pair("push_front", pushFrontTimer),
    std::make_pair("pop_front", popFrontTimer),
    //std::make_pair("front", frontTimer),
    //std::make_pair("back", backTimer),
    //std::make_pair("begin", beginTimer),
    //std::make_pair("end", endTimer),
    //std::make_pair("at", atTimer),
    //std::make_pair("[]", bracketTimer),
    //std::make_pair("iterator ++", iteratorAddOneTimer),
    //std::make_pair("iterator --", iteratorRedOneTimer),
    //std::make_pair("iterator +n", iteratorAddNTimer),
    //std::make_pair("iterator -n", iteratorRedNTimer),
    //std::make_pair("insert", insertTimer),
    //std::make_pair("erase", eraseTimer),
    //std::make_pair("Copy Constructor", copyConstructorTimer),
    //std::make_pair("Equal Operator", equalOperatorTimer),
};

#define __CORRECT_TEST
#define __OFFICAL

int main() {
#ifndef __OFFICAL
    puts("Deque CheckTool Package Version 1.5");
    puts("PAY SPECIAL ATTENTION: We assume your iterator is correct!!!");
#else
    puts("Deque CheckTool Package Version 1.5 Offical Version");
#endif
    puts("");
#ifdef __CORRECT_TEST
    puts("---------------------------------------------------------------------------");
    try{
        puts("Test Zone A: Correctness Testing...");
        int n = sizeof(TEST_A) / sizeof(CheckerPair);
        for (int i = 0; i < n; i++) {
            printf("Test %d: %-59s \n", i + 1, TEST_A[i].first);
            std::pair<bool, double> result = TEST_A[i].second();
            if (result.first) {
                puts("PASSED");
            } else {
                puts("FAILED");
            }
        }
    } catch(...) {
        puts("Unknown Error Occured");
    }
    puts("---------------------------------------------------------------------------");
#ifdef __SPEED_TEST
    puts("");
#endif
#endif

    return 0;
}
