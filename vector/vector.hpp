#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <iostream>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
protected:
	int _size;
	int _capacity;
	T* _elem;
	void copy(T const* elem, int lo, int hi) {
		_elem = (T*)operator new[](_capacity * sizeof(T));
		for(int i = 0;i < _size;++ i) {
			_elem[i] = elem[i + lo];
		}
	}
	//expand if full
	void expand() {
		if(_size < _capacity) return;
		if(_capacity < 3) {
			_capacity = 3;
			return;
		}
		T* prev_elem = _elem;
		int prev_capacity = _capacity;
		_capacity <<= 1;
		_elem = (T*)operator new[](_capacity * sizeof(T));
		for(int i = 0;i < _size;++ i)
			_elem[i] = prev_elem[i];
		for(int i = 0;i < _size;++ i)
			prev_elem[i].~T();
		operator delete [](prev_elem, prev_capacity * sizeof(T));
	}
	//shrink if 4 * s <= c
	void shrink() {
		if(_capacity < 6) return;
		if((_size << 2) > _capacity) return;
		T* prev_elem = _elem;
		int prev_capacity = _capacity;
		_capacity >>= 1;
		_elem = (T*)operator new[](_capacity * sizeof(T));
		for(int i = 0;i < _size;++ i)
			_elem[i] = prev_elem[i];
		for(int i = 0;i < _size;++ i)
			prev_elem[i].~T();
		operator delete [](prev_elem, prev_capacity * sizeof(T));
	}
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator {
		friend vector;
		friend const_iterator;
	protected:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		vector* _iter;
		int _pos;
		iterator(vector* iter, int pos):_iter(iter), _pos(pos) {}
	public:
		iterator():_iter(nullptr), _pos(0) {}
		iterator(const iterator &other):_iter(other._iter), _pos(other._pos) {}
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const {
			if(_pos + n >= this->_iter->_size) throw index_out_of_bound();//necessary?
			return iterator(_iter, _pos + n);
		}
		iterator operator-(const int &n) const {
			if(_pos - n < 0) throw index_out_of_bound();//necessary?
			return iterator(_iter, _pos - n);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			if(_iter != rhs._iter) throw invalid_iterator();
			return _pos - rhs._pos;
		}
		iterator& operator+=(const int &n) {
			if(_pos + n >= this->_iter->_size) throw index_out_of_bound();//necessary?
			_pos += n;
			return *this;
		}
		iterator& operator-=(const int &n) {
			if(_pos - n < 0) throw index_out_of_bound();//necessary?
			_pos -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			if(_pos >= this->_iter->_size) throw index_out_of_bound();
			iterator tmp(this->_iter, this->_pos);
			++ _pos;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			if(_pos >= this->_iter->_size) throw index_out_of_bound();
			++ _pos;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			if(_pos - 1 < 0) throw index_out_of_bound();
			iterator tmp(this->_iter, this->_pos);
			-- _pos;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			if(_pos - 1 < 0) throw index_out_of_bound();
			-- _pos;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return _iter->_elem[_pos];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return ((_iter == rhs._iter) && (_pos == rhs._pos));
		}
		bool operator==(const const_iterator &rhs) const {
			return ((_iter == rhs._iter) && (_pos == rhs._pos));
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return ((_iter != rhs._iter) || (_pos != rhs._pos));
		}
		bool operator!=(const const_iterator &rhs) const {
			return ((_iter != rhs._iter) || (_pos != rhs._pos));
		}
		/*bool operator>(const iterator &rhs) const {
			return _pos > rhs._pos;
		}
		bool operator>(const const_iterator &rhs) const {
			return _pos > rhs._pos;
		}
		bool operator<(const iterator &rhs) const {
			return _pos < rhs._pos;
		}
		bool operator<(const const_iterator &rhs) const {
			return _pos < rhs._pos;
		}*/
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator {
		friend vector;
		friend iterator;
	protected:
		const vector* _iter;
		int _pos;
		const_iterator(const vector* iter, int pos = 0):_iter(iter), _pos(pos) {}
	public:
		const_iterator():_iter(nullptr), _pos(0) {}
		const_iterator(const const_iterator &other):_iter(other._iter), _pos(other._pos) {}
		/*const_iterator operator+(const int &n) const {
			//if(_pos + n >= _size) throw index_out_of_bound();//necessary?
			return iterator(_iter, _pos + n);
		}
		const_iterator operator-(const int &n) const {
			if(_pos - n < 0) throw index_out_of_bound();//necessary?
			return iterator(_iter, _pos - n);
		}*/
		int operator-(const const_iterator &rhs) const {
			if(_iter != rhs._iter) throw invalid_iterator();
			return _pos - rhs._pos;
		}
		const_iterator& operator+=(const int &n) {
			if(_pos + n >= this->_iter->_size) throw index_out_of_bound();//necessary?
			_pos += n;
			return *this;
		}
		const_iterator& operator-=(const int &n) {
			if(_pos - n < 0) throw index_out_of_bound();//necessary?
			_pos -= n;
			return *this;
		}
		const_iterator operator++(int) {
			if(_pos >= this->_iter->_size) throw index_out_of_bound();
			iterator tmp(this->_iter, this->_pos);
			++ _pos;
			return tmp;
		}
		const_iterator& operator++() {
			if(_pos >= this->_iter->_size) throw index_out_of_bound();
			++ _pos;
			return *this;
		}
		const_iterator operator--(int) {
			if(_pos - 1 < 0) throw index_out_of_bound();
			iterator tmp(this->_iter, this->_pos);
			-- _pos;
			return tmp;
		}
		const_iterator& operator--() {
			if(_pos - 1 < 0) throw index_out_of_bound();
			-- _pos;
			return *this;
		}
		const T& operator*() const{
			return _iter->_elem[_pos];
		}
		bool operator==(const iterator &rhs) const {
			return ((_iter == rhs._iter) && (_pos == rhs._pos));
		}
		bool operator==(const const_iterator &rhs) const {
			return ((_iter == rhs._iter) && (_pos == rhs._pos));
		}
		bool operator!=(const iterator &rhs) const {
			return ((_iter != rhs._iter) || (_pos != rhs._pos));
		}
		bool operator!=(const const_iterator &rhs) const {
			return ((_iter != rhs._iter) || (_pos != rhs._pos));
		}
		/*bool operator>(const iterator &rhs) const {
			return _pos > rhs._pos;
		}
		bool operator>(const const_iterator &rhs) const {
			return _pos > rhs._pos;
		}
		bool operator<(const iterator &rhs) const {
			return _pos < rhs._pos;
		}
		bool operator<(const const_iterator &rhs) const {
			return _pos < rhs._pos;
		}*/
	};
	/**
	 * TODO Constructs
	 * Atleast two: default constructor, copy constructor
	 */
	//default constructor
	vector():_capacity(3), _size(0) {
		_elem = (T*)operator new[](_capacity * sizeof(T));
	}
	//copy constructor
	vector(const vector &other):_capacity(other._capacity), _size(other._size) {
		copy(other._elem, 0, other._size);
	}
	//destructor
	~vector() {
		for(int i = 0;i < _size;++ i)
			_elem[i].~T();
		operator delete [] (_elem, sizeof(T) * _capacity);
	}
	//assignment operator
	vector &operator=(const vector &other) {
		if(this == &other) return *this;
		if(_elem) {
			for(int i = 0;i < _size;++ i)
				_elem[i].~T();
			operator delete [] (_elem, sizeof(T) * _capacity);
		}
		copy(other._elem, 0, other._size);
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if(pos >= _size) throw index_out_of_bound();
		return _elem[pos];
	}
	const T & at(const size_t &pos) const {
		if(pos >= _size) throw index_out_of_bound();
		return _elem[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if(pos >= _size) throw index_out_of_bound();
		return _elem[pos];
	}
	const T & operator[](const size_t &pos) const {
		if(pos >= _size) throw index_out_of_bound();
		return _elem[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if(_size == 0) throw container_is_empty();
		return _elem[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if(_size == 0) throw container_is_empty();
		return _elem[_size - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(this, 0);
	}
	const_iterator cbegin() const {
		return const_iterator(this, 0);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(this, _size);
	}
	const_iterator cend() const {
		return const_iterator(this, _size);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return (_size == 0);
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return _size; //need cast?
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for(int i = 0;i < _size;++ i)
			_elem[i]->~T();
		operator delete [] (_elem, sizeof(T) * _capacity);
		_capacity = 3;
		_size = 0;
		_elem = (T*)operator new[] (_capacity * sizeof(T));
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		expand();
		for(vector<T>::iterator ite = this->end();ite != pos;-- ite) {
			*(ite) = *(ite - 1);
		}
		*(pos) = value;
		++ _size;
		return pos;
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if(ind > _size) throw index_out_of_bound();
		expand();
		for(int i = _size;i > ind;-- i) {
			_elem[i] = _elem[i - 1];
		}
		_elem[ind] = value;
		++ _size;
		return iterator(this, ind);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		for(vector<T>::iterator ite = pos;ite != this->end() - 1;++ ite) {
			*(ite) = *(ite + 1);
		}
		-- _size;
		shrink();
		return pos;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if(ind >= _size) throw index_out_of_bound();
		for(int i = ind;i < _size - 1;++ i) {
			_elem[i] = _elem[i + 1];
		}
		shrink();
		return iterator(this, ind);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		expand();
		++ _size;
		_elem[_size - 1] = value;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if(_size == 0) throw container_is_empty();
		-- _size;
		_elem[_size].~T();
		shrink();
	}
};


}

#endif