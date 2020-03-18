#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu { 

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
    friend class deque;
    friend class iterator;
    friend class const_iterator;
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

template<class T>
class deque {
public:
    class const_iterator;
    class iterator {
    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
    public:
        /**
         * return a new iterator which pointer n-next elements
         *   even if there are not enough elements, the behaviour is **undefined**.
         * as well as operator-
         */
        iterator operator+(const int &n) const {
            //TODO
        }
        iterator operator-(const int &n) const {
            //TODO
        }
        // return th distance between two iterator,
        // if these two iterators points to different vectors, throw invaild_iterator.
        int operator-(const iterator &rhs) const {
            //TODO
        }
        iterator& operator+=(const int &n) {
            //TODO
        }
        iterator& operator-=(const int &n) {
            //TODO
        }
        /**
         * TODO iter++
         */
        iterator operator++(int) {}
        /**
         * TODO ++iter
         */
        iterator& operator++() {}
        /**
         * TODO iter--
         */
        iterator operator--(int) {}
        /**
         * TODO --iter
         */
        iterator& operator--() {}
        /**
         * TODO *it
         */
        T& operator*() const {}
        /**
         * TODO it->field
         */
        T* operator->() const noexcept {}
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {}
        bool operator==(const const_iterator &rhs) const {}
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {}
        bool operator!=(const const_iterator &rhs) const {}
    };
    class const_iterator {
        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
        private:
            // data members.
        public:
            const_iterator() {
                // TODO
            }
            const_iterator(const const_iterator &other) {
                // TODO
            }
            const_iterator(const iterator &other) {
                // TODO
            }
            // And other methods in iterator.
            // And other methods in iterator.
            // And other methods in iterator.
    };
    /**
     * TODO Constructors
     */
    deque() {}
    deque(const deque &other) {}
    /**
     * TODO Deconstructor
     */
    ~deque() {}
    /**
     * TODO assignment operator
     */
    deque &operator=(const deque &other) {}
    /**
     * access specified element with bounds checking
     * throw index_out_of_bound if out of bound.
     */
    T & at(const size_t &pos) {}
    const T & at(const size_t &pos) const {}
    T & operator[](const size_t &pos) {}
    const T & operator[](const size_t &pos) const {}
    /**
     * access the first element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {}
    /**
     * access the last element
     * throw container_is_empty when the container is empty.
     */
    const T & back() const {}
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {}
    const_iterator cbegin() const {}
    /**
     * returns an iterator to the end.
     */
    iterator end() {}
    const_iterator cend() const {}
    /**
     * checks whether the container is empty.
     */
    bool empty() const {}
    /**
     * returns the number of elements
     */
    size_t size() const {}
    /**
     * clears the contents
     */
    void clear() {}
    /**
     * inserts elements at the specified locat on in the container.
     * inserts value before pos
     * returns an iterator pointing to the inserted value
     *     throw if the iterator is invalid or it point to a wrong place.
     */
    iterator insert(iterator pos, const T &value) {}
    /**
     * removes specified element at pos.
     * removes the element at pos.
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid or it points to a wrong place.
     */
    iterator erase(iterator pos) {}
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {}
    /**
     * removes the last element
     *     throw when the container is empty.
     */
    void pop_back() {}
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {}
    /**
     * removes the first element.
     *     throw when the container is empty.
     */
    void pop_front() {}
};

}

#endif
