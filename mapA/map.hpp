/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

enum color_type {RED, BLACK};
template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	typedef pair<const Key, T> value_type;
private:
	struct node {
		value_type *data;
		node *left, *right, *parent;
		color_type color;
		node():data(nullptr), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
		node(const value_type &obj,
		     node* l = nullptr, node* r = nullptr, node* p = nullptr,
		     color_type co = RED):left(l), right(r), parent(p), color(co) {
			data = new value_type(obj);
		}
		~node() {
			if(data) delete(data);
		}
		// find the nextNode of p
		node* nextNode(node* p, node* n, int size) {
			if(size == 0) return nullptr;
			if(p == n) return nullptr;
			if(p->right == n) {
				while(p->parent->right == p && p != n) p = p->parent;
				return p->parent;
			}
			p = p->right;
			while(p->left != n) p = p->left;
			return p;
		}
		// find the prevNode of p
		node* prevNode(node* p, node* n, node* rt, int size) {
			if(size == 0) return nullptr;
			if(p == n) {
				while(rt->right != n) rt = rt->right;
				return rt;
			}
			if(p->left == n) {
				while(p->parent->left == p && p != n) p = p->parent;
				if(p->parent == n) return nullptr;
				return p->parent;
			}
			p = p->left;
			while(p->right != n) p = p->right;
			return p;
		}
	};
	// start of data members of map
	node *_root, *_nil;
	Compare _comparator;
	int _size;
	// end of data members of map
	// start of utilities of map
	// if l < r, return -1, l == r, return 0, l > r, return 1
	inline int compare(const Key &left, const Key &right) const {
		if(_comparator(left, right)) return -1;
		else if(_comparator(right, left)) return 1;
		else return 0;
	}
	// if the key exists, return the corresponding node
	// if doesn't, return nil
	inline node* searchNode(node* rt, const Key &key) const {
		int sig;
		while(1) {
			if(rt == _nil) return rt;
			sig = compare(key, rt->data->first);
			if(sig == 0) return rt;
			else if(sig < 0) rt = rt->left;
			else rt = rt->right;
		}
	}
	// a util for fix-up
	inline void leftRotate(node* x) {
		node* y = x->right;
		if(y == _nil) return;
		x->right = y->left;
		if(y->left != _nil) y->left->parent = x;
		y->parent = x->parent;
		if(x->parent == _nil) _root = y;
		else if(x == x->parent->left) x->parent->left = y;
		else x->parent->right = y;
		y->left = x;
		x->parent = y;
	}
	// a util for fix-up
	inline void rightRotate(node* x) {
		node* y = x->left;
		if(y == _nil) return;
		x->left = y->right;
		if(y->right != _nil) y->right->parent = x;
		y->parent = x->parent;
		if(x->parent == _nil) _root = y;
		else if(x == x->parent->left) x->parent->left = y;
		else x->parent->right = y;
		y->right = x;
		x->parent = y;
	}
	// the fix-up for insert operation
	inline void insertFixUp(node* z) {
		while(z->parent->color == RED) {
			if(z->parent == z->parent->parent->left) {
				node *y = z->parent->parent->right;
				// case 1
				if(y->color == RED) {
					z->parent->color = BLACK;
					y->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
				}
				else {
					// case 2
					if(z == z->parent->right) {
						z = z->parent;
						leftRotate(z);
					}
					// case 3
					z->parent->color = BLACK;
					z->parent->parent->color = RED;
					rightRotate(z->parent->parent);
				}
			}
			else {
				node *y = z->parent->parent->left;
				// case 1
				if(y->color == RED) {
					z->parent->color = BLACK;
					y->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
				}
				else {
					// case 2
					if (z == z->parent->left) {
						z = z->parent;
						rightRotate(z);
					}
					// case 3
					z->parent->color = BLACK;
					z->parent->parent->color = RED;
					leftRotate(z->parent->parent);
				}
			}
		}
		_root->color = BLACK;
	}
	// if the key already exists, return the corresponding node
	// if doesn't, return the inserted node
	inline pair<node*, bool> _insert(const value_type &obj) {
		node *y = _nil, *x = _root;
		Key key = obj.first;
		while(x != _nil) {
			y = x;
			int sig = compare(key, x->data->first);
			if(sig == 0) return pair<node*, bool>(x, 0);
			else if(sig < 0) x = x->left;
			else x = x->right;
		}
		node *z = new node(obj, nullptr, nullptr, y, RED);
		if(y == _nil) _root = z;
		else if(compare(key, y->data->first) == -1) y->left = z;
		else y->right = z;
		z->left = _nil;
		z->right = _nil;
		insertFixUp(z);
		return pair<node*, bool>(z, 1);
	}
	// a util to delete the sons of node u
	inline void _clear(node* u) {
		if(u == _nil) return;
		_clear(u->left);
		_clear(u->right);
		delete(u);
	}
	// a util for erase
	// replace u with v
	inline void transplant(node* u, node* v) {
		node *_u = u;
		if(u->parent == _nil) _root = v;
		else if(u == u->parent->left) u->parent->left = v;
		else u->parent->right = v;
		v->parent = u->parent;
	}
	// the fix-up for erase operation
	inline void eraseFixUp(node* x) {
		while(x != _root && x->color == BLACK) {
			if(x == x->parent->left) {
				node *w = x->parent->right;
				// case 1
				if(w->color == RED) {
					w->color = BLACK;
					x->parent->color = RED;
					leftRotate(x->parent);
					w = x->parent->right;
				}
				// case 2
				if(w->left->color == BLACK && w->right->color == BLACK) {
					w->color = RED;
					x = x->parent;
				} else {
					// case 3
					if(w->right->color == BLACK) {
						w->left->color = BLACK;
						w->color = RED;
						rightRotate(w);
						w = x->parent->right;
					}
					// case 4
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->right->color = BLACK;
					leftRotate(x->parent);
					x = _root;
				}
			} else {
				node *w = x->parent->left;
				if(w->color == RED) {
					w->color = BLACK;
					x->parent->color = RED;
					rightRotate(x->parent);
					w = x->parent->left;
				}
				if(w->right->color == BLACK && w->left->color == BLACK) {
					w->color = RED;
					x = x->parent;
				} else {
					if(w->left->color == BLACK) {
						w->right->color = BLACK;
						w->color = RED;
						leftRotate(w);
						w = x->parent->left;
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->left->color = BLACK;
					rightRotate(x->parent);
					x = _root;
				}
			}
		}
		x->color = BLACK;
	}
	// erase operation
	inline void _erase(node* z) {
		node *y = z, *x;
		color_type yOriginalColor = y->color;
		if(z->left == _nil) {
			x = z->right;
			transplant(z, z->right);
		}
		else if(z->right == _nil) {
			x = z->left;
			transplant(z, z->left);
		}
		else {
			y = z->right;
			while(y->left != _nil) y = y->left;
			yOriginalColor = y->color;
			x = y->right;
			if(y->parent == z) x->parent = y;
			else {
				transplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}
			transplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete(z);
		if(yOriginalColor == BLACK) eraseFixUp(x);
	}
	// a util for copy operation
	inline void copy(node* rt, node* obj, node* nilO) {
		if(obj->left) {
			if(obj->left == nilO) rt->left = _nil;
			else {
				rt->left = new node(*(obj->left->data), nullptr, nullptr, rt, obj->left->color);
				copy(rt->left, obj->left, nilO);
			}
		}
		if(obj->right) {
			if(obj->right == nilO) rt->right = _nil;
			else {
				rt->right = new node(*(obj->right->data), nullptr, nullptr, rt, obj->right->color);
				copy(rt->right, obj->right, nilO);
			}
		}
	}
	// end of utilities of map
public:
	class const_iterator;
	class iterator {
		friend class const_iterator;
	private:
		map* self;
		node* pos;
	public:
		iterator():self(nullptr), pos(nullptr) {}
		iterator(const iterator &other):self(other.self), pos(other.pos) {}
		iterator(map* _self, node* _pos):self(_self), pos(_pos) {}
		iterator operator++(int) {
			node *tmp = pos;
			pos = pos->nextNode(pos, self->_nil, self->_size);
			if(pos == nullptr) throw invalid_iterator();
			return iterator(self, tmp);
		}
		iterator & operator++() {
			pos = pos->nextNode(pos, self->_nil, self->_size);
			if(pos == nullptr) throw invalid_iterator();
			return *this;
		}
		iterator operator--(int) {
			node *tmp = pos;
			pos = pos->prevNode(pos, self->_nil, self->_root, self->_size);
			if(pos == nullptr) throw invalid_iterator();
			return iterator(self, tmp);
		}
		iterator & operator--() {
			pos = pos->prevNode(pos, self->_nil, self->_root, self->_size);
			if(pos == nullptr) throw invalid_iterator();
			return *this;
		}
		value_type & operator*() const {
			return *(pos->data);
		}
		bool operator==(const iterator &rhs) const {
			return (self == rhs.self) && (pos == rhs.pos);
		}
		bool operator==(const const_iterator &rhs) const {
			return (self == rhs.self) && (pos == rhs.pos);
		}
		bool operator!=(const iterator &rhs) const {
			return (pos != rhs.pos) || (self != rhs.self);
		}
		bool operator!=(const const_iterator &rhs) const {
			return (pos != rhs.pos) || (self != rhs.self);
		}
		value_type* operator->() const noexcept {
			return pos->data;
		}
		map* getSelf() {
			return self;
		}
		node* getPos() {
			return pos;
		}
	};
	class const_iterator {
		friend class iterator;
	private:
		const map* self;
		node* pos;
	public:
		const_iterator():self(nullptr), pos(nullptr) {}
		const_iterator(const const_iterator &other):self(other.self), pos(other.pos) {}
		const_iterator(const iterator &other):self(other.self), pos(other.pos) {}
		const_iterator(const map* _self, node* _pos):self(_self), pos(_pos) {}
		const_iterator operator++(int) {
			node *tmp = pos;
			pos = pos->nextNode(pos, self->_nil, self->_size);
			if(pos == nullptr) throw invalid_iterator();
			return const_iterator(self, tmp);
		}
		const_iterator & operator++() {
			pos = pos->nextNode(pos, self->_nil, self->_size);
			if(pos == nullptr) throw invalid_iterator();
			return *this;
		}
		const_iterator operator--(int) {
			node *tmp = pos;
			pos = pos->prevNode(pos, self->_nil, self->_root, self->_size);
			if(pos == nullptr) throw invalid_iterator();
			return const_iterator(self, tmp);
		}
		const_iterator & operator--() {
			pos = pos->prevNode(pos, self->_nil, self->_root, self->_size);
			if(pos == nullptr) throw invalid_iterator();
			return *this;
		}
		value_type & operator*() const {
			return *(pos->data);
		}
		bool operator==(const iterator &rhs) const {
			return (self == rhs.self) && (pos == rhs.pos);
		}
		bool operator==(const const_iterator &rhs) const {
			return (self == rhs.self) && (pos == rhs.pos);
		}
		bool operator!=(const iterator &rhs) const {
			return (pos != rhs.pos) || (self != rhs.self);
		}
		bool operator!=(const const_iterator &rhs) const {
			return (pos != rhs.pos) || (self != rhs.self);
		}
		value_type* operator->() const noexcept {
			return pos->data;
		}
	};
	map() {
		_root = _nil = new node;
		_root->parent = _nil;
		_nil->left = _nil->right = _nil;
		_root->color = BLACK;
		_nil->color = BLACK;
		_size = 0;
	}
	map(const map &other) {
		_comparator = other._comparator;
		_size = other._size;
		_root = _nil = new node;
		_root->parent = _nil;
		_nil->left = _nil->right = _nil;
		if(other._root != other._nil) {
			_root = new node(*(other._root->data), nullptr, nullptr, _nil, BLACK);
			copy(_root, other._root, other._nil);
		}
	}
	map & operator=(const map &other) {
		if(&other == this) return *this;
		if(_root != _nil) _clear(_root);
		_root = _nil;
		_root->parent = _nil;
		_nil->left = _nil->right = _nil;
		if(other._root != other._nil) {
			_root = new node(*(other._root->data), nullptr, nullptr, _nil, BLACK);
			copy(_root, other._root, other._nil);
		}
		_size = other._size;
		_comparator = other._comparator;
	}
	~map() {
		if(_root != _nil) _clear(_root);
		delete(_nil);
	}
	T & at(const Key &key) {
		node *tmp = searchNode(_root, key);
		if(tmp == _nil) throw index_out_of_bound();
		else return tmp->data->second;
	}
	const T & at(const Key &key) const {
		node *tmp = searchNode(_root, key);
		if(tmp == _nil) throw index_out_of_bound();
		else return tmp->data->second;
	}
	T & operator[](const Key &key) {
		node *tmp = searchNode(_root, key);
		if(tmp == _nil) {
			++ _size;
			pair<node*, bool> res = _insert(value_type(key, T()));
			return (res.first)->data->second;
		}
		else return tmp->data->second;
	}
	const T & operator[](const Key &key) const {
		node *tmp = searchNode(_root, key);
		if(tmp == _nil) throw index_out_of_bound();
		else return tmp->data->second;
	}
	iterator begin() {
		if(_size == 0) return iterator(this, _nil);
		else {
			node *p = _root;
			while(p->left != _nil) p = p->left;
			return iterator(this, p);
		}
	}
	const_iterator cbegin() const {
		if(_size == 0) return const_iterator(this, _nil);
		else {
			node *p = _root;
			while(p->left != _nil) p = p->left;
			return const_iterator(this, p);
		}
	}
	iterator end() {
		return iterator(this, _nil);
	}
	const_iterator cend() const {
		return const_iterator(this, _nil);
	}
	bool empty() const {
		return _size == 0;
	}
	size_t size() const {
		return _size;
	}
	void clear() {
		if(_root != _nil) _clear(_root);
		_root = _nil;
		_root->left = nullptr;
		_root->right = nullptr;
		_size = 0;
	}
	pair<iterator, bool> insert(const value_type &value) {
		node *tmp = searchNode(_root, value.first);
		if(tmp == _nil) {
			++ _size;
			pair<node*, bool> res = _insert(value);
			return pair<iterator, bool>(iterator(this, res.first), 1);
		}
		else return pair<iterator, bool>(iterator(this, tmp), 0);
	}
	void erase(iterator pos) {
		if(pos.getSelf() != this || pos.getPos() == this->_nil) throw invalid_iterator();
		-- _size;
		_erase(pos.getPos());
	}
	size_t count(const Key &key) const {
		return searchNode(_root, key) != _nil;
	}
	iterator find(const Key &key) {
		return iterator(this, searchNode(_root, key));
	}
	const_iterator find(const Key &key) const {
		return const_iterator(this, searchNode(_root, key));
	}
};

}

#endif
