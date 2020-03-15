#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <iostream>

namespace sjtu {
template<typename T>
class vector {
protected:
	int _size;
	int _capacity;
	T* _elem;
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
			new(_elem + i) T(prev_elem[i]);
		for(int i = 0;i < _size;++ i)
			prev_elem[i].~T();
		operator delete [](prev_elem, prev_capacity * sizeof(T));
	}
public:
	class const_iterator;
	class iterator {
		friend vector;
		friend const_iterator;
	protected:
		vector* _iter;
		int _pos;
		iterator(vector* iter, int pos):_iter(iter), _pos(pos) {}
	public:
		iterator():_iter(nullptr), _pos(0) {}
		iterator(const iterator &other):_iter(other._iter), _pos(other._pos) {}
		iterator operator+(const int &n) const {
			return iterator(_iter, _pos + n);
		}
		iterator operator-(const int &n) const {
			return iterator(_iter, _pos - n);
		}
		int operator-(const iterator &rhs) const {
			return _pos - rhs._pos;
		}
		iterator& operator+=(const int &n) {
			_pos += n;
			return *this;
		}
		iterator& operator-=(const int &n) {
			_pos -= n;
			return *this;
		}
		iterator operator++(int) {
			iterator tmp(this->_iter, this->_pos);
			++ _pos;
			return tmp;
		}
		iterator& operator++() {
			++ _pos;
			return *this;
		}
		iterator operator--(int) {
			iterator tmp(this->_iter, this->_pos);
			-- _pos;
			return tmp;
		}
		iterator& operator--() {
			-- _pos;
			return *this;
		}
		T& operator*() const{
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
	};
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
		int operator-(const const_iterator &rhs) const {
			if(_iter != rhs._iter) throw invalid_iterator();
			return _pos - rhs._pos;
		}
		const_iterator& operator+=(const int &n) {
			_pos += n;
			return *this;
		}
		const_iterator& operator-=(const int &n) {
			_pos -= n;
			return *this;
		}
		const_iterator operator++(int) {
			iterator tmp(this->_iter, this->_pos);
			++ _pos;
			return tmp;
		}
		const_iterator& operator++() {
			++ _pos;
			return *this;
		}
		const_iterator operator--(int) {
			iterator tmp(this->_iter, this->_pos);
			-- _pos;
			return tmp;
		}
		const_iterator& operator--() {
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
	};
	//default constructor
	vector():_capacity(3), _size(0) {
		_elem = (T*)operator new[](_capacity * sizeof(T));
	}
	//copy constructor
	vector(const vector &other):_capacity(other._capacity), _size(other._size) {
		_elem = (T*)operator new[](_capacity * sizeof(T));
		for(int i = 0;i < other._size;++ i) {
			new(_elem + i) T(other._elem[i]);
		}
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
		_size = other._size;
		_capacity = other._capacity;
		_elem = (T*)operator new[](_capacity * sizeof(T));
		for(int i = 0;i < other._size;++ i) {
			new(_elem + i) T(other._elem[i]);
		}
		return *this;
	}
	T & at(const size_t &pos) {
		if(pos >= _size) throw index_out_of_bound();
		return _elem[pos];
	}
	const T & at(const size_t &pos) const {
		if(pos >= _size) throw index_out_of_bound();
		return _elem[pos];
	}
	T & operator[](const size_t &pos) {
		if(pos >= _size) throw index_out_of_bound();
		return _elem[pos];
	}
	const T & operator[](const size_t &pos) const {
		if(pos >= _size) throw index_out_of_bound();
		return _elem[pos];
	}
	const T & front() const {
		if(_size == 0) throw container_is_empty();
		return _elem[0];
	}
	const T & back() const {
		if(_size == 0) throw container_is_empty();
		return _elem[_size - 1];
	}
	iterator begin() {
		return iterator(this, 0);
	}
	const_iterator cbegin() const {
		return const_iterator(this, 0);
	}
	iterator end() {
		return iterator(this, _size);
	}
	const_iterator cend() const {
		return const_iterator(this, _size);
	}
	bool empty() const {
		return (_size == 0);
	}
	size_t size() const {
		return _size;
	}
	void clear() {
		for(int i = 0;i < _size;++ i)
			_elem[i]->~T();
		operator delete [] (_elem, sizeof(T) * _capacity);
		_capacity = 3;
		_size = 0;
		_elem = (T*)operator new[] (_capacity * sizeof(T));
	}
	iterator insert(iterator pos, const T &value) {
		expand();
		new(_elem + this->end()._pos) T();
		for(vector<T>::iterator ite = this->end();ite != pos;-- ite) {
			*(ite) = *(ite - 1);
		}
		*(pos) = value;
		++ _size;
		return pos;
	}
	iterator insert(const size_t &ind, const T &value) {
		if(ind > _size) throw index_out_of_bound();
		expand();
		new(_elem + this->end()._pos) T();
		for(int i = _size;i > ind;-- i) {
			_elem[i] = _elem[i - 1];
		}
		_elem[ind] = value;
		++ _size;
		return iterator(this, ind);
	}
	iterator erase(iterator pos) {
		for(vector<T>::iterator ite = pos;ite != this->end() - 1;++ ite) {
			*(ite) = *(ite + 1);
		}
		-- _size;
		return pos;
	}
	iterator erase(const size_t &ind) {
		if(ind >= _size) throw index_out_of_bound();
		for(int i = ind;i < _size - 1;++ i) {
			_elem[i] = _elem[i + 1];
		}
		return iterator(this, ind);
	}
	void push_back(const T &value) {
		expand();
		++ _size;
		new(_elem + _size - 1) T(value);
	}
	void pop_back() {
		if(_size == 0) throw container_is_empty();
		-- _size;
		_elem[_size].~T();
	}
};
}
#endif
