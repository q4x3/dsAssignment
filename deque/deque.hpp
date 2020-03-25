#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu { 

template<typename T>
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

template<typename T>
class deque;

template<typename T>
class block {
    friend class deque<T>;
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
    void backInsert(const T &dt) {
        ++ size;
        node<T> *p = new node<T>(dt), *q = rear->prev;
        q->next = p;
        p->prev = q;
        p->next = rear;
        rear->prev = p;
    }
    void backRemove() {
        -- size;
        node<T> *p = rear->prev;
        p->prev->next = rear;
        rear->prev = p->prev;
        delete p;
    }
    void frontRemove() {
        -- size;
        node<T> *p = head->next;
        head->next = p->next;
        p->next->prev = head;
        delete p;
    }
    int _size() {
        return size;
    }
    int index(const node<T>* pos) {
        node<T> *p = head->next;
        int ind = 0;
        while(p != pos) {
            ++ ind;
            p = p->next;
        }
        return ind;
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
    T& front() {
        return *(head->next->data);
    }
    T& back() {
        return *(rear->prev->data);
    }
    bool full() {
        return size >= 300;
    }
};

template<typename T>
class deque {
protected:
    block<T> *hd, *rr;
    int sz;
public:
    class const_iterator;
    class iterator {
        friend class deque<T>;
    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        deque<T> *iter;
        block<T> *self;
        node<T> *pos;
    public:
        iterator():iter(nullptr), self(nullptr), pos(nullptr) {}
        iterator(const iterator &other):iter(other.iter), self(other.self), pos(other.pos) {}
        iterator(const deque<T>* _iter, const block<T>* _self, const node<T>* _pos):iter(_iter), self(_self), pos(_pos) {}
        /**
         * return a new iterator which pointer n-next elements
         *   even if there are not enough elements, the behaviour is **undefined**.
         * as well as operator-
         */
        iterator operator+(const int &n) const {
            //TODO
            if(n == 0) return *this;
            if(n < 0) return *this - (- n);
            int _n = n + self->index(pos);
            block<T> *p = self;
            while(_n >= p->_size()) {
                _n -= p->_size();
                p = p->next;
            }
            node<T> *q = p->head->next;
            while(_n) {
                -- _n;
                q = q->next;
            }
            return iterator(iter, p, q);
        }
        iterator operator-(const int &n) const {
            //TODO
            if(n == 0) return *this;
            if(n < 0) return *this + (- n);
            int _n = self->index(pos) - n;
            block<T> *p = self->prev;
            while(_n < 0) {
                _n += p->_size();
                p = p->prev;
            }
            node<T> *q = p->head->next;
            while(_n) {
                -- _n;
                q = q->next;
            }
            return iterator(iter, p, q);
        }
        // return th distance between two iterator,
        // if these two iterators points to different vectors, throw invaild_iterator.
        int operator-(const iterator &rhs) const {
            //TODO
            if(iter != rhs.iter) throw invalid_iterator();
            int lind = self->index(pos), rind = rhs.self->index(rhs.pos);
            block<T> *lp = self, *rp = rhs.self, *tmp = iter->hd->next;
            while(tmp != lp) {
                lind += tmp->_size();
                tmp = tmp->next;
            }
            tmp = rhs.iter->hd->next;
            while(tmp != rp) {
                rind += tmp->_size();
                tmp = tmp->next;
            }
            return lind - rind;
        }
        iterator& operator+=(const int &n) {
            //TODO
            if(n == 0) return *this;
            if(n < 0) return (*this -= (- n));
            int _n = self->index(pos) + n;
            block<T> *p = self;
            while(_n >= p->_size()) {
                _n -= p->_size();
                p = p->next;
            }
            node<T> *q = p->head->next;
            while(_n) {
                -- _n;
                q = q->next;
            }
            self = p; pos = q;
            return *this;
        }
        iterator& operator-=(const int &n) {
            //TODO
            if(n == 0) return *this;
            if(n < 0) return (*this += (- n));
            int _n = self->index(pos) - n;
            block<T> *p = self->prev;
            while(_n < 0) {
                _n += p->_size();
                p = p->prev;
            }
            node<T> *q = p->head->next;
            while(_n) {
                -- _n;
                q = q->next;
            }
            self = p; pos = q;
            return *this;
        }
        /**
         * TODO iter++
         */
        iterator operator++(int) {
            iterator tmp(this->iter, this->self, this->pos);
            if(pos->next == self->rear) {
                self = self->next;
                pos = self->head->next;
            } else {
                pos = pos->next;
            }
            return tmp;
        }
        /**
         * TODO ++iter
         */
        iterator& operator++() {
            if(pos->next == self->rear) {
                self = self->next;
                pos = self->head->next;
            } else {
                pos = pos->next;
            }
            return *this;
        }
        /**
         * TODO iter--
         */
        iterator operator--(int) {
            iterator tmp(this->iter, this->self, this->pos);
            if(pos->prev == self->head) {
                self = self->prev;
                pos = self->rear->prev;
            } else {
                pos = pos->prev;
            }
            return tmp;
        }
        /**
         * TODO --iter
         */
        iterator& operator--() {
            if(pos->prev == self->head) {
                self = self->prev;
                pos = self->rear->prev;
            } else {
                pos = pos->prev;
            }
            return *this;
        }
        /**
         * TODO *it
         */
        T& operator*() const {
            return *(pos->data);
        }
        /**
         * TODO it->field
         */
        T* operator->() const noexcept {
            return pos->data;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ((iter == rhs.iter) && (self == rhs.self) && (pos == rhs.pos));
        }
        bool operator==(const const_iterator &rhs) const {
            return ((iter == rhs.iter) && (self == rhs.self) && (pos == rhs.pos));
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ((iter != rhs.iter) || (self != rhs.self) || (pos != rhs.pos));
        }
        bool operator!=(const const_iterator &rhs) const {
            return ((iter != rhs.iter) || (self != rhs.iter) || (pos != rhs.pos));
        }
    };
    class const_iterator {
        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
    private:
            // data members.
        const deque<T> *iter;
        block<T> *self;
        node<T> *pos;
    public:
        const_iterator():iter(nullptr), self(nullptr), pos(nullptr) {}
        const_iterator(const const_iterator &other):iter(other.iter), self(other.self), pos(other.pos) {}
        const_iterator(const iterator &other):iter(other.iter), self(other.self), pos(other.pos) {}
        const_iterator(const deque<T>* _iter, const block<T>* _self, const node<T>* _pos):iter(_iter), self(_self), pos(_pos) {}
        /**
         * return a new iterator which pointer n-next elements
         *   even if there are not enough elements, the behaviour is **undefined**.
         * as well as operator-
         */
        const_iterator operator+(const int &n) const {
            //TODO
            if(n == 0) return *this;
            if(n < 0) return *this - (- n);
            int _n = n + self->index(pos);
            block<T> *p = self;
            while(_n >= p->_size()) {
                _n -= p->_size();
                p = p->next;
            }
            node<T> *q = p->head->next;
            while(_n) {
                -- _n;
                q = q->next;
            }
            return const_iterator(iter, p, q);
        }
        const_iterator operator-(const int &n) const {
            //TODO
            if(n == 0) return *this;
            if(n < 0) return *this + (- n);
            int _n = self->index(pos) - n;
            block<T> *p = self->prev;
            while(_n < 0) {
                _n += p->_size();
                p = p->prev;
            }
            node<T> *q = p->head->next;
            while(_n) {
                -- _n;
                q = q->next;
            }
            return const_iterator(iter, p, q);
        }
        // return th distance between two iterator,
        // if these two iterators points to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const {
            //TODO
            if(iter != rhs.iter) throw invalid_iterator();
            int lind = self->index(pos), rind = rhs.self->index(rhs.pos);
            block<T> *lp = self, *rp = rhs.self, *tmp = iter->hd->next;
            while(tmp != lp) {
                lind += tmp->_size();
                tmp = tmp->next;
            }
            tmp = rhs.iter->hd->next;
            while(tmp != rp) {
                rind += tmp->_size();
                tmp = tmp->next;
            }
            return lind - rind;
        }
        const_iterator& operator+=(const int &n) {
            //TODO
            if(n == 0) return *this;
            if(n < 0) return (*this -= (- n));
            int _n = self->index(pos) + n;
            block<T> *p = self;
            while(_n >= p->_size()) {
                _n -= p->_size();
                p = p->next;
            }
            node<T> *q = p->head->next;
            while(_n) {
                -- _n;
                q = q->next;
            }
            self = p; pos = q;
            return *this;
        }
        const_iterator& operator-=(const int &n) {
            //TODO
            if(n == 0) return *this;
            if(n < 0) return (*this += (- n));
            int _n = self->index(pos) - n;
            block<T> *p = self->prev;
            while(_n < 0) {
                _n += p->_size();
                p = p->prev;
            }
            node<T> *q = p->head->next;
            while(_n) {
                -- _n;
                q = q->next;
            }
            self = p; pos = q;
            return *this;
        }
        /**
         * TODO iter++
         */
        const_iterator operator++(int) {
            const_iterator tmp(this->iter, this->self, this->pos);
            if(pos->next == self->rear) {
                self = self->next;
                pos = self->head->next;
            } else {
                pos = pos->next;
            }
            return tmp;
        }
        /**
         * TODO ++iter
         */
        const_iterator& operator++() {
            if(pos->next == self->rear) {
                self = self->next;
                pos = self->head->next;
            } else {
                pos = pos->next;
            }
            return *this;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            const_iterator tmp(this->iter, this->self, this->pos);
            if(pos->prev == self->head) {
                self = self->prev;
                pos = self->rear->prev;
            } else {
                pos = pos->prev;
            }
            return tmp;
        }
        /**
         * TODO --iter
         */
        const_iterator& operator--() {
            if(pos->prev == self->head) {
                self = self->prev;
                pos = self->rear->prev;
            } else {
                pos = pos->prev;
            }
            return *this;
        }
        /**
         * TODO *it
         */
        T& operator*() const {
            return *(pos->data);
        }
        /**
         * TODO it->field
         */
        T* operator->() const noexcept {
            return pos->data;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ((iter == rhs.iter) && (self == rhs.self) && (pos == rhs.pos));
        }
        bool operator==(const const_iterator &rhs) const {
            return ((iter == rhs.iter) && (self == rhs.self) && (pos == rhs.pos));
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ((iter != rhs.iter) || (self != rhs.self) || (pos != rhs.pos));
        }
        bool operator!=(const const_iterator &rhs) const {
            return ((iter != rhs.iter) || (self != rhs.iter) || (pos != rhs.pos));
        }
    };
    /**
     * TODO Constructors
     */
    deque():sz(0) {
        hd = new block<T>;
        rr = new block<T>;
        hd->next = rr;
        rr->prev = hd;
    }
    deque(const deque &other):sz(other.sz) {
        hd = new block<T>();
        rr = new block<T>();
        block<T> *p1 = hd, *p2 = other.hd->next, *q;
        while(p2 != other.rr) {
            q = p1;
            p1->next = new block<T>(*p2);
            p1 = p1->next;
            p1->next = q;
            p2 = p2->next;
        }
        p1->next = rr;
        rr->prev = p1;
    }
    /**
     * TODO Deconstructor
     */
    ~deque() {
        sz = 0;
        block<T> *p = hd->next, *q;
        while(p != rr) {
            q = p;
            p = p->next;
            delete q;
        }
        if(hd) delete hd;
        if(rr) delete rr;
    }
    /**
     * TODO assignment operator
     */
    deque &operator=(const deque &other) {
        if(&other == this) return *this;
        sz = other.sz;
        block<T> *p = hd->next, *p2 = other.hd->next, *q;
        while(p != rr) {
            q = p;
            p = p->next;
            delete q;
        }
        p = hd;
        while(p2 != other.rr) {
            q = p;
            p->next = new block<T>(*p2);
            p = p->next;
            p->prev = q;
            p2 = p2->next;
        }
        p->next = rr;
        rr->prev = p;
    }
    /**
     * access specified element with bounds checking
     * throw index_out_of_bound if out of bound.
     */
    T & at(const size_t &pos) {
        if(pos >= sz) throw index_out_of_bound();
        int nodeInd = pos;
        block<T> *p = hd->next;
        while(nodeInd >= p->_size()) {
            nodeInd -= p->_size();
            p = p->next;
        }
        return p->at(nodeInd);
    }
    const T & at(const size_t &pos) const {
        if(pos >= sz) throw index_out_of_bound();
        int nodeInd = pos;
        block<T> *p = hd->next;
        while(nodeInd >= p->_size()) {
            nodeInd -= p->_size();
            p = p->next;
        }
        return p->at(nodeInd);
    }
    T & operator[](const size_t &pos) {
        if(pos >= sz) throw index_out_of_bound();
        int nodeInd = pos;
        block<T> *p = hd->next;
        while(nodeInd >= p->_size()) {
            nodeInd -= p->_size();
            p = p->next;
        }
        return p->at(nodeInd);
    }
    const T & operator[](const size_t &pos) const {
        if(pos >= sz) throw index_out_of_bound();
        int nodeInd = pos;
        block<T> *p = hd->next;
        while(nodeInd >= p->_size()) {
            nodeInd -= p->_size();
            p = p->next;
        }
        return p->at(nodeInd);
    }
    /**
     * access the first element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if(sz == 0) throw container_is_empty();
        return hd->next->front();
    }
    /**
     * access the last element
     * throw container_is_empty when the container is empty.
     */
    const T & back() const {
        if(sz == 0) throw container_is_empty();
        return rr->prev->back();
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(this, hd->next, hd->next->head->next);
    }
    const_iterator cbegin() const {
        return const_iterator(this, hd->next, hd->next->head->next);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(this, rr->prev, rr->prev, rr->prev->rear->prev);
    }
    const_iterator cend() const {
        return const_iterator(this, rr->prev, rr->prev, rr->prev->rear->prev);
    }
    /**
     * checks whether the container is empty.
     */
    bool empty() const {
        return sz == 0;
    }
    /**
     * returns the number of elements
     */
    size_t size() const {
        return sz;
    }
    /**
     * clears the contents
     */
    void clear() {
        sz = 0;
        block<T> *p = hd->next, *q;
        while(p != rr) {
            q = p;
            p = p->next;
            delete q;
        }
        hd->next = rr;
        rr->prev = hd;
    }
    /**
     * inserts elements at the specified locat on in the container.
     * inserts value before pos
     * returns an iterator pointing to the inserted value
     *     throw if the iterator is invalid or it point to a wrong place.
     */
    iterator insert(iterator pos, const T &value) {
        block<T> *p = 

    }
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
    void push_back(const T &value) {
        ++ sz;
        block<T> *p = rr->prev;
        if(p == hd || p->full()) {
            block<T> *newBlock = new block<T>;
            newBlock->insert(value, 0);
            p->next = newBlock;
            newBlock->prev = p;
            newBlock->next = rr;
            rr->prev = newBlock;
        } else {
            p->backInsert(value);
        }
    }
    /**
     * removes the last element
     *     throw when the container is empty.
     */
    void pop_back() {
        if(sz == 0) throw container_is_empty();
        -- sz;
        rr->prev->backRemove();
        //if() disconnect
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        ++ sz;
        block<T> *p = hd->next;
        if(p == rr || p->full()) {
            block<T> *newBlock = new block<T>;
            newBlock->insert(value, 0);
            hd->next = newBlock;
            newBlock->prev = hd;
            newBlock->next = p;
            p->prev = newBlock;
        } else {
            p->insert(value, 0);
        }
    }
    /**
     * removes the first element.
     *     throw when the container is empty.
     */
    void pop_front() {
        if(sz == 0) throw container_is_empty();
        -- sz;
        block<T> *p = hd->next;
        p->frontRemove();
        //if() disconnect or merge
    }
};

}

#endif
