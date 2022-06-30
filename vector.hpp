//
// Created by matsony on 03.06.22.
//

#ifndef VECTOR_H
# define VECTOR_H

# include <limits>
#include "utility.h"

namespace ft {
	template<typename T, typename Allocator = std::allocator<T> >
	class vector {
	public:
		typedef T									value_type;
		typedef Allocator							allocator_type;
		typedef typename Allocator::reference		reference;
		typedef typename Allocator::const_reference	const_reference;
		typedef typename Allocator::pointer			pointer;
		typedef typename Allocator::const_pointer	const_pointer;
		typedef std::ptrdiff_t						difference_type;
		typedef std::size_t							size_type;

		template<bool IsConst>
		class common_iterator;
		typedef vector::common_iterator<NotConst>           iterator;
		typedef vector::common_iterator<Const>              const_iterator;
		typedef ft::reverse_iterator<iterator>          	reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>     	const_reverse_iterator;


//		class const_iterator;
//
//		typedef ft::reverse_iterator<iterator> reverse_iterator;
//		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

		vector();
		explicit vector(const Allocator &alloc);
		explicit vector(size_type count, const T &value = T(), const Allocator &alloc = Allocator());
		template<class InputIt>
		vector( typename enable_if<is_iterator<InputIt>::value, InputIt>::type first, InputIt last,
			   const Allocator &alloc = Allocator());
		vector(const vector &other);
		~vector();

		vector &operator=(const vector &other);
		void assign(size_type count, const T &value);
		template<class InputIt>
		void assign(typename enable_if<is_iterator<InputIt>::value, InputIt>::type first, InputIt last); //why is iter?
		allocator_type get_allocator() const;
		reference at(size_type pos);
		const_reference at(size_type pos) const;
		reference operator[](size_type pos);
		const_reference operator[](size_type pos) const;
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
		T *data();
		const T *data() const;
		bool empty() const;
		size_type size() const;
		size_type max_size() const;
		void reserve(size_type new_cap);
		void clear();
		void push_back(const T &value);
		size_type capacity() const;
		void pop_back();
		void resize(size_type count, T value = T());
		void swap(vector &other);

		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;

		iterator insert(iterator pos, const T &value);
		void insert(iterator pos, size_type count, const T &value);

		template<class InputIt>
		void insert(iterator pos, typename enable_if<is_iterator<InputIt>::value, //why is iter>
				InputIt>::type first, InputIt last);
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);
	private:
		size_type __size;
		size_type __capacity;
		T *__data;
		Allocator __alloc;

		void clear_all(); // why?
	};


	template<class T, class Allocator> //why?
	void vector<T, Allocator>::clear_all() {
		for (size_type i = 0; i < __size; ++i)
			__alloc.destroy(__data + i);
		__alloc.deallocate(__data, __capacity);
		__data = 0;
		__capacity = 0;
		__size = 0;
	}

	template<class T, class Allocator>
	vector<T, Allocator>::vector() : __size(0), __capacity(0), __alloc() {
		__data = __alloc.allocate(0);
	}

	template<class T, class Allocator>
	vector<T, Allocator>::vector(const Allocator& alloc) : __size(0), __capacity(0), __alloc(alloc){
		__data = __alloc.allocate(0);
	}

	template<class T, class Allocator>
	vector<T, Allocator>::vector(vector<T, Allocator>::size_type count, const T& value, const Allocator& alloc):
	__size(count), __capacity(count), __alloc(alloc){
		__data = __alloc.allocate(__capacity);
		size_type i;
		try{
			for (i = 0; i < count; i++)
				__alloc.construct(__data + i, value);
		}
		catch (...){
			for (; i > 0; --i)
				__alloc.destroy(__data + i - 1);
			__alloc.deallocate(__data, __capacity);
			throw ;
		}
	}

	template<class T, class Allocator>
	template<class InputIt>
	vector<T, Allocator>::vector(typename enable_if<is_iterator<InputIt>::value, InputIt>::type first, InputIt last,
								 const Allocator& alloc) : __alloc(alloc), __size(0), __capacity(0){
		difference_type n = ft::distance(first, last);
		__data = __alloc.allocate(n);
		size_type i;
		try{
			for(i = 0; first != last; ++first, ++i)
				__alloc.construct(__data + i, *first);
		}
		catch (...){
			for(; i > 0; --i)
				__alloc.destroy(__data + i - 1);
			__alloc.deallocate(__data, n);
			throw ;
		}
		__size = n;
		__capacity = n;
	}

	template<class T, class Allocator>
	vector<T, Allocator>::vector(const vector& other) : __size(other.__size), __capacity(other.__capacity),
	__alloc(other.__alloc){
		__data = __alloc.allocate(__capacity);
		size_type i;
		try{
			for(i = 0; i < __size; i++)
				__alloc.construct(__data + i, other.__data[i]);
		}
		catch(...){
			for (; i > 0; --i)
				__alloc.destroy(__data + i - 1);
			__alloc.deallocate(__data, __capacity);
			throw ;
		}
	}

	template<class T, class Allocator>
	vector<T, Allocator>::~vector(){
		for(size_type i = 0; i < __size; i++)
			__alloc.destroy(__data + i);
		__alloc.deallocate(__data, __capacity);
	}

	template<class T, class Allocator>
	vector<T, Allocator>& vector<T, Allocator>::operator=(const vector<T, Allocator> &other) {
		T * tmp = __alloc.allocate(other.__capacity);
		size_type i;
		try{
			for(i = 0; i < other.__size; ++i)
				__alloc.construct(tmp + i, other.__data[i]);
		}
		catch (...){
			for (; i > 0; --i)
				__alloc.destroy(tmp + i - 1);
			__alloc.deallocate(tmp, __capacity);
			throw ;
		}
		this->clear_all();
		__capacity = other.__capacity;
		__size = other.__size;
		__data = tmp;
		return *this;
	}

	template <class T, class Allocator>
	void vector<T, Allocator>::assign(size_type count, const T &value) {
		if (count < __capacity){
			for (size_type i = 0; i < __size; ++i)
				__alloc.destroy(__data + i);
			for (size_type i = 0; i < count; ++i) {
				try {
					__alloc.construct(__data + i, value);
				}
				catch (...) {
					for (size_type j = 0; j < i; ++j)
						__alloc.destroy(__data + i);
					__size = 0;
					throw;
				}
			}
			__size = count;
			return ;
		}
		T * tmp = __alloc.allocate(count);
		size_type i;
		try {
			for (i = 0; i < count; ++i)
				__alloc.construct(tmp + i, value);
		}
		catch (...){
			for (; i > 0; --i)
				__alloc.destroy(tmp + i - 1);
			__alloc.deallocate(tmp, count);
			throw ;
		}
		clear_all();
		__capacity = count;
		__size = count;
		__data = tmp;
	}

	template<class T, class Allocator>
	template<class InputIt>
	void vector<T, Allocator>::assign(typename enable_if<is_iterator<InputIt>::value, InputIt>::type first, InputIt last){
		difference_type n = ft::distance(first, last);
		if (n < __capacity)
		{
			for (size_type i = 0; i < __size; ++i)
				__alloc.destroy(__data + i);
			for (size_type i = 0; i < n; ++i, ++first){
				try {
					__alloc.construct(__data + i, *first);
				}
				catch (...){
					for (size_type j = 0; j < i; ++j)
						__alloc.destroy(__data + i);
					__size = 0;
					throw ;
				}
			}
			__size = n;
			return;
		}
		T* tmp = __alloc.allocate(n);
		size_type i;
		try{
			for(i = 0; first != last; ++first, ++i)
				__alloc.construct(tmp + i, *first);
		}
		catch (...){
			for (; i > 0; --i)
				__alloc.destroy(tmp + i - 1);
			__alloc.deallocate(tmp, n);
			throw ;
		}
		clear_all();
		__data = tmp;
		__size = n;
		__capacity = n;
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator() const {
		return __alloc;
	}

	template<class T, class Allocator>
	void vector<T, Allocator>::clear() {
		for (size_type i = 0; i < __size; ++i)
			__alloc.destroy(__data + i);
		__size = 0;
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type pos) {
		if (pos >= __size)
			throw std::out_of_range("vector");
		return __data[pos];
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type pos) const {
		if (pos >= __size)
			throw std::out_of_range("vector");
		return __data[pos];
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](size_type pos) {
		return __data[pos];
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::const_reference  vector<T, Allocator>::operator[](size_type pos) const {
		return __data[pos];
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::front() {
		return __data[0];
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::front() const {
		return __data[0];
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::back() {
		return __data[__size - 1];
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const {
		return __data[__size - 1];
	}

	template<class T, class Allocator>
	T* vector<T, Allocator>::data() {
		return __data;
	}

	template<class T, class Allocator>
	const T* vector<T, Allocator>::data() const{
		return __data;
	}

	template<class T, class Allocator>
	bool vector<T, Allocator>::empty() const {
		return __size == 0;
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const {
		return __size;
	}

	template<class T, class Allocator>
	void vector<T, Allocator>::push_back(const T &value) {
		if (__capacity == 0){
			__alloc.deallocate(__data, 0);
			__data = __alloc.allocate(1);
			__capacity = 1;
		} else if (__size == __capacity){
			T* tmp = __alloc.allocate(__capacity << 1);
			size_type i;
			try {
				for (i = 0; i < __size; ++i)
					__alloc.construct(tmp + i, __data[i]);
			}
			catch (...) {
				for (; i > 0; --i)
					__alloc.destroy(tmp + i - 1);
				__alloc.deallocate(tmp, __capacity << 1);
				throw ;
			}
			for (i = 0; i < __size; ++i)
				__alloc.destroy(__data + i);
			__alloc.deallocate(__data, __capacity);
			__data = tmp;
			__capacity <<= 1;
		}
		__alloc.construct(__data + __size, value);
		__size++;
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const {
		return __capacity;
	}

	template<class T, class Allocator>
	void vector<T, Allocator>::pop_back() {
		__alloc.destroy(__data + __size);
		--__size;
	}

	template<class T, class Allocator>
	bool	operator==(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs){
		if (lhs.size() != rhs.size())
			return false;
		return equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template<class T, class Allocator>
	bool	operator!=(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs){
		return !(lhs == rhs);
	}

	template<class T, class Allocator>
	bool	operator<(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template<class T, class Allocator>
	bool	operator<=(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return !(rhs < lhs);
	}

	template<class T, class Allocator>
	bool	operator>(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return (rhs < lhs);
	}

	template<class T, class Allocator>
	bool	operator>=(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return !(lhs < rhs);
	}

	template<class T, class Allocator>
	void vector<T, Allocator>::reserve(size_type new_cap) {
		if (new_cap > max_size())
			throw std::length_error("vector");
		if (new_cap > __capacity){
			T* new_data = __alloc.allocate(new_cap);
			size_type i;
			try{
				for (i = 0; i < __size; ++i)
					__alloc.construct(new_data + i, __data[i]);
			}
			catch (...){
				for(; i > 0; --i)
					__alloc.destroy(new_data + i - 1);
				__alloc.deallocate(new_data, new_cap);
				throw ;
			}
			for (i = 0; i < __size; ++i)
				__alloc.destroy(__data + i);
			__alloc.deallocate(__data, __capacity);
			__data = new_data;
			__capacity = new_cap;
		}
	}

	template<class T, class Allocator>
	void vector<T, Allocator>::resize(size_type count, T value) {
		if (count < __size) {
			for (size_type i = count; i < __size; ++i)
				__alloc.destroy(__data + i);
		} else if (count > __size) {
			size_type old_size = __size;
			for (size_type i = __size; i < count; ++i){
				try {
					push_back(value);
				}
				catch (...) {
					for (size_type j = __size; j < i; ++j)
						__alloc.destroy(__data + j);
					__size = old_size;
					throw ;
				}
			}
		}
		__size = count;
	}

	template<class T, class Allocator>
	void	vector<T, Allocator>::swap(vector& other) {
		ft::swap(__data, other.__data);
		ft::swap(__capacity, other.__capacity);
		ft::swap(__size, other.__size);
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::size_type	vector<T, Allocator>::max_size() const {
		return (allocator_type().max_size());
	}



	template<class T, class Allocator>
	template<bool IsConst>
	class vector<T, Allocator>::common_iterator : public ft::iterator<ft::random_access_iterator_tag, typename conditional<IsConst, value_type, const value_type>::type> {
	private:
		T* __ptr;
	public:
		typedef T* iterator_type;
		common_iterator() {};
		common_iterator(T* ptr) : __ptr(ptr) {};
		common_iterator(const common_iterator& other) : __ptr(other.__ptr) {};
		T* base() const {return __ptr;}
		common_iterator& operator=(const common_iterator& other) {
			__ptr = other.__ptr;
			return *this;
		}
		T& operator*() {
			return *__ptr;
		}
		T* operator->() {
			return __ptr;
		}
		common_iterator& operator++() {
			__ptr++;
			return *this;
		}
		common_iterator operator++(int) {
			common_iterator tmp(*this);
			++(*this);
			return tmp;
		}
		common_iterator& operator--(){
			__ptr--;
			return *this;
		}
		common_iterator operator--(int) {
			common_iterator tmp(*this);
			--(*this);
			return tmp;
		}
		common_iterator&	operator+=(difference_type n) {
			__ptr += n;
			return *this;
		}
		common_iterator&	operator-=(difference_type n) {
			__ptr -= n;
			return *this;
		}
		common_iterator	operator+(difference_type n) {
			common_iterator tmp(*this);
			tmp += n;
			return tmp;
		}
		common_iterator	operator-(difference_type n) const {
			common_iterator tmp(*this);
			tmp -= n;
			return tmp;
		}
		T&	operator[](difference_type n) {
			return *(__ptr + n);
		}
//		operator	vector<T, Allocator>::const_iterator() const {return vector<T, Allocator>::const_iterator(__ptr);}

		friend difference_type	operator-(common_iterator left, common_iterator right){return (left.base() - right.base());}
		friend bool	operator==(common_iterator left, common_iterator right) {return (left.base() == right.base());}
		friend bool operator<=(common_iterator left, common_iterator right) {return (left.base() <= right.base());}
		friend bool operator>=(common_iterator left, common_iterator right) {return (left.base() >= right.base());}
		friend bool operator!=(common_iterator left, common_iterator right) {return (left.base() != right.base());}
		friend bool operator<(common_iterator left, common_iterator right) {return (left.base() < right.base());}
		friend bool operator>(common_iterator left, common_iterator right) {return (left.base() > right.base());}
		friend common_iterator operator+(difference_type n, common_iterator iter) {return iter + n;}
		friend common_iterator operator-(difference_type n, common_iterator iter) {return iter - n;}
	};


//	template<class T, class Allocator>
//	class vector<T, Allocator>::iterator : public ft::iterator<ft::random_access_iterator_tag, T, std::ptrdiff_t, T*, T&>{
//	private:
//		T* __ptr;
//	public:
//		typedef T* iterator_type;
//		iterator() {};
//		iterator(T* ptr) : __ptr(ptr) {};
//		iterator(const iterator& other) : __ptr(other.__ptr) {};
//		T* base() const {return __ptr;}
//		iterator& operator=(const iterator& other) {
//			__ptr = other.__ptr;
//			return *this;
//		}
//		T& operator*() {
//			return *__ptr;
//		}
//		T* operator->() {
//			return __ptr;
//		}
//		iterator& operator++() {
//			__ptr++;
//			return *this;
//		}
//		iterator operator++(int) {
//			iterator tmp(*this);
//			++(*this);
//			return tmp;
//		}
//		iterator& operator--(){
//			__ptr--;
//			return *this;
//		}
//		iterator operator--(int) {
//			iterator tmp(*this);
//			--(*this);
//			return tmp;
//		}
//		iterator&	operator+=(difference_type n) {
//			__ptr += n;
//			return *this;
//		}
//		iterator&	operator-=(difference_type n) {
//			__ptr -= n;
//			return *this;
//		}
//		iterator	operator+(difference_type n) {
//			iterator tmp(*this);
//			tmp += n;
//			return tmp;
//		}
//		iterator	operator-(difference_type n) const {
//			iterator tmp(*this);
//			tmp -= n;
//			return tmp;
//		}
//		T&	operator[](difference_type n) {
//			return *(__ptr + n);
//		}
//		operator	vector<T, Allocator>::const_iterator() const {return vector<T, Allocator>::const_iterator(__ptr);}
//
//		friend difference_type	operator-(iterator left, iterator right){return (left.base() - right.base());}
//		friend bool	operator==(iterator left, iterator right) {return (left.base() == right.base());}
//		friend bool operator<=(iterator left, iterator right) {return (left.base() <= right.base());}
//		friend bool operator>=(iterator left, iterator right) {return (left.base() >= right.base());}
//		friend bool operator!=(iterator left, iterator right) {return (left.base() != right.base());}
//		friend bool operator<(iterator left, iterator right) {return (left.base() < right.base());}
//		friend bool operator>(iterator left, iterator right) {return (left.base() > right.base());}
//		friend iterator operator+(difference_type n, iterator iter) {return iter + n;}
//		friend iterator operator-(difference_type n, iterator iter) {return iter - n;}
//	};
//

//	template <class T, class Allocator>
//	class vector<T, Allocator>::const_iterator: public ft::iterator<ft::random_access_iterator_tag, T, std::ptrdiff_t, const T*, const T&>{
//	private:
//		const T *__ptr;
//	public:
//		typedef const T *iterator_type;
//		const_iterator() {};
//		const_iterator(const T *ptr) : __ptr(ptr) {};
//		const_iterator(const const_iterator &other) : __ptr(other.__ptr) {};
//		const T *base() const { return __ptr; }
//		const_iterator &operator=(const const_iterator &other) {
//			__ptr = other.__ptr;
//			return *this;
//		}
//
//		const T &operator*() const { return *__ptr; }
//
//		const T *operator->() const { return __ptr; }
//
//		const_iterator &operator++() {
//			__ptr++;
//			return *this;
//		}
//
//		const_iterator operator++(int) {
//			const_iterator tmp(*this);
//			++(*this);
//			return tmp;
//		}
//
//		const_iterator &operator--() {
//			__ptr--;
//			return *this;
//		}
//
//		const_iterator operator--(int) {
//			const_iterator tmp(*this);
//			--(*this);
//			return tmp;
//		}
//
//		const_iterator &operator+=(difference_type n) {
//			__ptr += n;
//			return *this;
//		}
//
//		const_iterator &operator-=(difference_type n) {
//			__ptr -= n;
//			return *this;
//		}
//
//		const_iterator operator+(difference_type n) {
//			const_iterator tmp(*this);
//			tmp += n;
//			return tmp;
//		}
//
//		const_iterator operator-(difference_type n) {
//			const_iterator tmp(*this);
//			tmp -= n;
//			return tmp;
//		}
//
//		const T &operator[](difference_type n) { return *(__ptr + n); }
////		operator	vector<T, Allocator>::iterator() const {return vector<T, Allocator>::iterator(__ptr);}
//
//		friend difference_type operator-(const_iterator left, const_iterator right) {return left.base() - right.base();}
//		friend bool operator==(const_iterator left, const_iterator right) { return left.base() == right.base(); }
//		friend bool operator<=(const_iterator left, const_iterator right) { return left.base() <= right.base(); }
//		friend bool operator>=(const_iterator left, const_iterator right) { return left.base() >= right.base(); }
//		friend bool operator!=(const_iterator left, const_iterator right) { return left.base() != right.base(); }
//		friend bool operator<(const_iterator left, const_iterator right) { return left.base() < right.base(); }
//		friend bool operator>(const_iterator left, const_iterator right) { return left.base() > right.base(); }
//		friend const_iterator operator+(difference_type n, const_iterator iter) {return iter + n;}
//		friend const_iterator operator-(difference_type n, const_iterator iter) {return iter - n;}
//	};


	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::begin() {return __data;}

	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::end() {return __data + __size;}

	template<class T, class Allocator>
	typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin() const {return __data;}

	template<class T, class Allocator>
	typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const {return __data + __size;}

	template<class T, class Allocator>
	typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rbegin() {
		return reverse_iterator(end());
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rbegin() const {
		return const_reverse_iterator(end());
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rend() {
		return reverse_iterator(begin());
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rend() const {
		return const_reverse_iterator(begin());
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(iterator pos, const T& value) {
		if (__size == __capacity){
			T* tmp = __alloc.allocate(__capacity * 2);
			size_type	count = pos - begin();
			size_type	i;
			try {
				for(i = 0; i < count; ++i)
					__alloc.construct(tmp + i, __data[i]);
				__alloc.construct(tmp + i, value);
			}
			catch (...){
				for (; i > 0; --i)
					__alloc.destroy(tmp + i - 1);
				__alloc.deallocate(tmp, __size * 2);
				throw ;
			}
			try {
				for (i = count; i < __size; ++i)
					__alloc.construct(tmp + i + 1, __data[i]);
			}
			catch (...){
				for (; i > 0; --i)
					__alloc.destroy(tmp + i - 1);
				__alloc.deallocate(tmp, __size * 2);
				throw ;
			}
			for (i = 0; i < __size; i++)
				__alloc.destroy(__data + i);
			__alloc.deallocate(__data, __capacity);
			__data = tmp;
			if (__capacity == 0)
				++__capacity;
			else
				__capacity *= 2;
			__size++;
			return vector<T, Allocator>::iterator(__data + count);
		}

		size_type count = pos - begin();
		if (__size != 0){
			size_type i;
			for (i = __size; i > count; --i)
			{
				try{
					__alloc.construct(__data + i, *(__data - 1 + i));
				}
				catch (...){
					for (size_type j = 0; j < i; ++j)
						__alloc.destroy(__data + j);
					for (size_type j = i + 1; j < __size + 1; j++)
						__alloc.destroy(__data + j);
					__size = 0;
					throw ;
				}
				__alloc.destroy(__data - 1 + i);
			}
		}
		try{
			__alloc.construct(__data + count, value);
		}
		catch (...) {
			for (size_type i = 0; i < count; ++i)
				__alloc.destroy(__data + i);
			for (size_type i = count + 1; i < __size + 1; ++i)
				__alloc.destroy(__data + i);
			__size = 0;
			throw ;
		}
		__size++;
		return vector<T, Allocator>::iterator(__data + count);
	}

	template <class T, class Allocator>
	void vector<T, Allocator>::insert(iterator pos, size_type count, const T& value) {
		if (__size + count > __capacity){
			size_type new_cap = __capacity * 2;
			if (new_cap < __capacity + count)
				new_cap = __capacity + count;
			T* tmp = __alloc.allocate(new_cap);
			size_type n = pos - begin();
			size_type i;
			try{
				for (i = 0; i < n; ++i)
					__alloc.construct(tmp + i, __data[i]);
			}
			catch (...){
				for (; i >0; --i)
					__alloc.destroy(tmp + i - 1);
				__alloc.deallocate(tmp, new_cap);
				throw ;
			}
			try {
				for (i = n; i < n + count; ++i)
					__alloc.construct(tmp + i, value);
			}
			catch (...) {
				for (; i > 0; --i)
					__alloc.destroy(tmp + i - 1);
				__alloc.deallocate(tmp, new_cap);
				throw ;
			}
			try {
				for (i = n + count; i < __size + count; ++i)
					__alloc.construct(tmp + i, __data[i - count]);
			}
			catch (...){
				for (; i > 0; --i)
					__alloc.destroy(tmp + i - 1);
				__alloc.deallocate(tmp, new_cap);
				throw ;
			}
			size_type _tmp = __size;
			this->clear_all();
			__capacity = new_cap;
			__size = _tmp + count;
			__data = tmp;
			return ;
		}

		size_type n = pos - begin();
		if (__size != 0){
			size_type i = __size - 1;
			for (; i >= n; --i){
				try {
					__alloc.construct(__data + i + count, __data[i]);
				}
				catch (...){
					for (size_type j = 0; j <= i; ++j)
						__alloc.destroy(__data + j);
					for (size_type j = i + count + 1; j < __size + count; ++j)
						__alloc.destroy(__data + j);
					__size = 0;
					throw ;
				}
				__alloc.destroy(__data + i);
			}
		}
		size_type i = 0;
		for (; i < count; ++i) {
			try {
				__alloc.construct(__data + n + i, value);
			}
			catch (...) {
				for (size_type j = 0; j < n + i; ++j)
					__alloc.destroy(__data + j);
				for (size_type j = n + count; j < __size + count; ++j)
					__alloc.destroy(__data + j);
				__size = 0;
				throw ;
			}
		}
		__size += count;
	}

	template <class T, class Allocator>
	template< class InputIt >
	void vector<T, Allocator>::insert(iterator pos,
									  typename enable_if<is_iterator<InputIt>::value, InputIt>::type first,
									  InputIt last) {
		size_type count = ft::distance(first, last);
		if (__size + count > __capacity) {
			size_type new_cap = __capacity * 2;
			if (new_cap < __capacity + count)
				new_cap = __capacity + count;
			T* tmp = __alloc.allocate(new_cap);
			size_type n = pos - begin();
			size_type i;
			try {
				for (i = 0; i < n; ++i)
					__alloc.construct(tmp + i, __data[i]);
			}
			catch (...) {
				for (; i > 0; --i)
					__alloc.destroy(tmp + i - 1);
				__alloc.deallocate(tmp, new_cap);
				throw ;
			}
			try {
				for (i = n; i < n + count; ++i, ++first)
					__alloc.construct(tmp + i, *first);
			}
			catch (...) {
				for (; i > 0; --i)
					__alloc.destroy(tmp + i - 1);
				__alloc.deallocate(tmp, new_cap);
				throw ;
			}
			try {
				for (i = n + count; i < __size + count; ++i)
					__alloc.construct(tmp + i, __data[i - count]);
			}
			catch (...) {
				for (; i > 0; --i)
					__alloc.destroy(tmp + i + 1);
				__alloc.deallocate(tmp, new_cap);
				throw ;
			}
			size_type old_size = __size;
			clear_all();
			__capacity = new_cap;
			__size = old_size + count;
			__data = tmp;
			return ;
		}
		size_type n = pos - begin();
		if (__size != 0) {
			size_type i = __size - 1;
			for (; i >= n; --i) {
				try {
					__alloc.construct(__data + i + count, __data[i]);
				}
				catch (...) {
					for (size_type j = 0; j <= i; ++j)
						__alloc.destroy(__data + j);
					for (size_type j = i + count + 1; j < __size + count; ++j)
						__alloc.destroy(__data + j);
					__size = 0;
					throw ;
				}
				__alloc.destroy(__data + i);
			}
		}
		size_type	i = 0;
		for (; i < count; ++i, ++first){
			try {
				__alloc.construct(__data + n + i, *first);
			}
			catch (...) {
				for (size_type j = 0; j < n + i; ++j)
					__alloc.destroy(__data + j);
				for (size_type j = n + count; j < __size + count; ++j)
					__alloc.destroy(__data + j);
				__size = 0;
				throw ;
			}
		}
		__size += count;
	}

	template <class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(iterator pos) {
		size_type n = pos - begin();
		for (size_type i = n; i + 1 < __size; ++i) {
			__alloc.destroy(__data + i);
			try {
				__alloc.construct(__data + i, __data[i + 1]);
			}
			catch (...) {
				for (size_type j = 0; j < i; ++j)
					__alloc.destroy(__data + j);
				for (size_type j = i + 1; j < __size; ++j)
					__alloc.destroy(__data + j);
				__size = 0;
				throw ;
			}
		}
		__alloc.destroy(__data + __size - 1);
		if (n == __size)
			n--;
		__size--;
		return (__data + n);
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(iterator first, iterator last) {
		size_type n = first - begin();
		size_type count = last - first;
		if (count == 0)
			return last;
		for (size_type i = 0; i < count; ++i)
			__alloc.destroy(__data + n + i);
		for (size_type i = n + count; i < __size; ++i) {
			try {
				__alloc.construct(__data + i - count, __data[i]);
			}
			catch (...) {
				for (size_type j = 0; j < i - count; ++j)
					__alloc.destroy(__data + j);
				for (size_type j = i; j < __size; ++j)
					__alloc.destroy(__data + j);
				__size = 0;
				throw ;
			}
			__alloc.destroy(__data + i);
		}
		if (last == end())
			n = __size - count;
		__size -= count;
		return iterator(__data + n);
	}
}

namespace std
{
	template <class T, class Allocator>
	void	swap( ft::vector<T, Allocator> & lhs,
				  ft::vector<T, Allocator> & rhs)
	{
		lhs.swap(rhs);
	}
}

#endif //VECTOR_H
