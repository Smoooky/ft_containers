//
// Created by matsony on 14.06.22.
//

#ifndef FT_CONTAINERS_UTILITY_H
#define FT_CONTAINERS_UTILITY_H

# include <iostream>
# include <memory>
# include <limits>
# include <cstddef>
//# include <type_traits>
//# include <type_traits>

namespace ft{
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag {};
	struct bidirectional_iterator_tag {};
	struct random_access_iterator_tag {};

	template<
			class Category,
			class T,
			class Distance = std::ptrdiff_t,
			class Pointer = T*,
			class Reference = T&>
	struct iterator
	{
		typedef Category		iterator_category;
		typedef T				value_type;
		typedef Distance		difference_type;
		typedef Pointer			pointer;
		typedef Reference		reference;
	};

	template<class Iter>
	struct iterator_traits{
		typedef Iter 								iterator_type;
		typedef typename Iter::difference_type		difference_type;
		typedef typename Iter::value_type			value_type;
		typedef typename Iter::pointer				pointer;
		typedef typename Iter::reference			reference;
		typedef typename Iter::iterator_category	iterator_category;

	};

	template<class T>
	struct iterator_traits<T*>{
		typedef std::ptrdiff_t				difference_type;
		typedef T							value_type;
		typedef T*							pointer;
		typedef T&							reference;
		typedef random_access_iterator_tag	iterator_category;
	};

	template<class T>
	struct iterator_traits<const T*>{
		typedef std::ptrdiff_t				difference_type;
		typedef T							value_type;
		typedef const T*					pointer;
		typedef const T&					reference;
		typedef random_access_iterator_tag	iterator_category;
	};

	template <class T>
	struct is_iterator {
	private:
		template <class C>
		static typename C::iterator_category f(int a) {
			(void)a;
			return C::iterator_category();
		};

		template <class C>
		static double	f(...) {return 0;};
	public:
		static const bool value = sizeof(f<T>(1)) != sizeof(double);
	};

	template <class T>
	struct is_iterator<T *> {
		static const bool value = true;
	};

	template< class Iter, class category = typename iterator_traits<Iter>::iterator_category>
	class reverse_iterator
	{
	public:
		typedef	Iter 	iterator_type;
		typedef	typename iterator_traits<Iter>::iterator_category	iterator_category;
		typedef	typename iterator_traits<Iter>::value_type			value_type;
		typedef	typename iterator_traits<Iter>::difference_type		difference_type;
		typedef	typename iterator_traits<Iter>::pointer				pointer;
		typedef	typename iterator_traits<Iter>::reference			reference;
		reverse_iterator() : __iter() {}
		explicit reverse_iterator (Iter it): __iter(it) {}
		reverse_iterator(const reverse_iterator & other) :
				__iter(other.__iter) {}
//		template<bool IsConst>
//		reverse_iterator(const reverse_iterator::<IsConst> &other) :
//				__iter(other.base()) {}
		~reverse_iterator() {}
		iterator_type base() const
		{ return (__iter); }

		reference operator*() const
		{
			iterator_type tmp = __iter;
			return (*(--tmp));
		}

		reverse_iterator  &operator=(const reverse_iterator &inst)
		{
			if (this == &inst)
				return *this;
			__iter = inst.base();
			return *this;
		}

		template<typename Iter1>
		reverse_iterator  &operator=(const reverse_iterator<Iter1> &inst)
		{
//			if (__iter == inst.base())
//				return *this;
			__iter = inst.base();
			return *this;
		}

		reverse_iterator& operator++()
		{
			--__iter;
			return (*this);
		}

		reverse_iterator operator++(int)
		{
			reverse_iterator tmp = *this;
			++(*this);
			return (tmp);
		}

		reverse_iterator& operator--()
		{
			++__iter;
			return (*this);
		}

		reverse_iterator operator--(int)
		{
			reverse_iterator tmp = *this;
			--(*this);
			return (tmp);
		}

		reverse_iterator<Iter> &operator+=(difference_type offset)
		{
			__iter -= offset;
			return *this;
		}

		reverse_iterator<Iter> &operator-=(difference_type offset)
		{
			__iter += offset;
			return *this;
		}

		pointer operator->() const { return &(operator*()); }

		friend	bool operator==(reverse_iterator lhs, reverse_iterator rhs)
		{return lhs.__iter == rhs.__iter;}

		friend	bool operator!=(reverse_iterator lhs, reverse_iterator rhs)
		{return lhs.__iter != rhs.__iter; }

		reference operator[](difference_type n) { return (this->base()[-n- 1]); }

	private:
		iterator_type	__iter;
	};

	template <typename Iter>
	reverse_iterator<Iter> operator+(const reverse_iterator<Iter> &ite, size_t offset)
	{
		reverse_iterator<Iter> r(ite.base() - offset);
		return r;
	}

	template <typename Iter>
	reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type offset, const reverse_iterator<Iter> &ite)
	{
		reverse_iterator<Iter> r(ite.base() - offset);
		return r;
	}

	template <typename Iter>
	reverse_iterator<Iter> operator-(const reverse_iterator<Iter> &ite, typename reverse_iterator<Iter>::difference_type offset)
	{
		reverse_iterator<Iter> r(ite.base() + offset);
		return r;
	}

	template <typename Iter>
	ptrdiff_t operator-(const reverse_iterator<Iter> &lhs, const reverse_iterator<Iter> &rhs)
	{
		return rhs.base() - lhs.base();
	}

	template <typename Iter1, typename Iter2>
	bool operator==(const reverse_iterator<Iter1> lhs, const reverse_iterator<Iter2> rhs)
		{return lhs.base() == rhs.base();}
//
//	template <typename Iter1, typename Iter2>
//	bool operator!=(const reverse_iterator<Iter1> lhs, const reverse_iterator<Iter2> rhs)
//		{return lhs.base() != rhs.base();}
//
	template <typename Iter, typename Iter2>
	ptrdiff_t operator-(const reverse_iterator<Iter> &lhs, const reverse_iterator<Iter2> &rhs)
	{
		return rhs.base() - lhs.base();
	}

	namespace detail {
		template<class It>
		typename ft::iterator_traits<It>::difference_type
		__do_distance(It first, It last, ft::input_iterator_tag)
		{
			typename ft::iterator_traits<It>::difference_type result = 0;
			while (first != last) {
				++first;
				++result;
			}
			return result;
		}

		template<class It>
		typename ft::iterator_traits<It>::difference_type
		__do_distance(It first, It last, ft::random_access_iterator_tag)
		{
			return last - first;
		}

		template<class It>
		typename ft::iterator_traits<It>::difference_type
		__do_distance(It first, It last, std::input_iterator_tag)
		{
			typename ft::iterator_traits<It>::difference_type result = 0;
			while (first != last) {
				++first;
				++result;
			}
			return result;
		}

		template<class It>
		typename ft::iterator_traits<It>::difference_type
		__do_distance(It first, It last, std::random_access_iterator_tag)
		{
			return last - first;
		}
	}

	template<class It>
	typename ft::iterator_traits<It>::difference_type
	distance(It first, It last)
	{
		return detail::__do_distance(first, last,
									 typename ft::iterator_traits<It>::iterator_category());
	}

	template<bool B, class T = void>
	struct enable_if {};

	template<class T>
	struct enable_if<true, T> { typedef T type; };

	template<class InputIt1, class InputIt2>
	bool	lexicographical_compare(InputIt1 first1, InputIt1 last1,
									InputIt2 first2, InputIt2 last2)
	{
		for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
			if (*first1 < *first2)
				return true;
			if (*first2 < *first1)
				return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	template<class T>
	void	swap(T & a, T & b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}

	template <class T>
	const T&	min( const T& a, const T& b )
	{
		if (a < b)
			return a;
		return b;
	}

	template <class T>
	const T&	max( const T& a, const T& b )
	{
		if (a > b)
			return a;
		return b;
	}

	template<class InputIt1, class InputIt2>
	bool	equal(InputIt1 first1, InputIt1 last1,
				  InputIt2 first2)
	{
		for (; first1 != last1; ++first1, ++first2) {
			if (!(*first1 == *first2)) {
				return false;
			}
		}
		return true;
	}

	const bool NotConst = false;
	const bool Const = true;

	template<bool B, typename NotConst, typename Const>
	struct conditional {typedef NotConst   type;};

	template<typename NotConst, typename Const>
	struct conditional<true, NotConst, Const> {typedef Const   type;};

	template<bool B, typename NotConst, typename Const>
	struct conditional_t
	{typedef typename conditional<B, NotConst, Const>::type   type;};
}
#endif //FT_CONTAINERS_UTILITY_H
