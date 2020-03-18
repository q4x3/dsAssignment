#include <iostream>

using namespace std;

template<class T>
struct node
{
    T *data;
    node *prev, *next;
    node():data(nullptr), prev(nullptr), next(nullptr) {}
    node(node* pr, node* nx):data(nullptr), prev(pr), next(nx) {}
    node(const T &dt):prev(nullptr), next(nullptr) {
        data = (T*)operator new(sizeof(T));
        new(data) T(dt);
    }
    node(const node &other):prev(nullptr), next(nullptr) {
        if(other.data) {
            data = (T*)operator new(sizeof(T));
            new(data) T(*(other.data));
        } else data = nullptr;
    }
    ~node() {
        if(data) {
            (*data).~T();
            operator delete(data, sizeof(T));
        }
    }
};

template<class T>
class block {
protected:
    node<T> *head, *rear;
    int size;
    block *prev, *next;
public:
    block():size(0) {
        head = new node<T>();
        rear = new node<T>();
        head->next = rear;
        rear->prev = head;
    }
    block(const block &other):size(other.size), prev(nullptr), next(nullptr) {
        head = new node<T>();
        rear = new node<T>();
        node<T> *p1 = head, *p2 = other.head->next, *q;
        while(p2 != other.rear) {
            q = p1;
            p1->next = new node<T>(*p2);
            p1 = p1->next;
            p1->prev = q;
            p2 = p2->next;
        }
        p1->next = rear;
        rear->prev = p1;
    }
    ~block() {
        size = 0;
        node<T> *p = head->next, *q;
        while(p != rear) {
            q = p;
            p = p->next;
            delete q;
        }
        if(head) delete head;
        if(rear) delete rear;
    }
    void insert(const T &dt,const int pos) {
        node<T> *p = head, *q;
        int _pos = pos;
        while(_pos) {
            p = p->next;
            -- _pos;
        }
        node<T> *newnode = new node<T>(dt);
        q = p->next;
        p->next = newnode;
        newnode->prev = p;
        newnode->next = q;
        q->prev = newnode;
        ++ size;
    }
    int _size() {
        return size;
    }
    node<T>* hd() {
        return head;
    }
    T& at(const int pos) {
        node<T> *p = head->next;
        int _pos = pos;
        while(_pos) {
            p = p->next;
            -- _pos;
        }
        return *(p->data);
    }
};

int main() {
    block<int> b;
    b.insert(6, 0);
    b.insert(10, 1);
    node<int> *p = b.hd();
    cout << b._size() << " " << b.at(1) << endl;
    return 0;
}