// accumulate.h - std::accumulate for enumerations
// accumulate, sum = accumulate<plus>, prod = accumulate<multiplies>
#pragma once
#include <limits>
#include "enumerator.h"

namespace iter {

	// o(t,i[0]), o(o(t,i[0]), i[1]), ...
	template<class O, class I, 
		class T = typename std::iterator_traits<I>::value_type
	>
	class accumulate_ : public I {
		O o;
		T t;
	public:
		accumulate_()
		{ }
		accumulate_(O o, I i, T t)
			: I(i), o(o), t(i ? o(t,*i) : t)
		{ }

		I iterator()
		{
			return *this;
		}
		explicit operator bool() const
		{
			return I::operator bool();
		}
		T operator*() const
		{
			return t;
		}
		accumulate_& operator++()
		{
			if (I::operator bool())
				t = o(t, *I::operator++());

			return *this;
		}
		accumulate_ operator++(int)
		{
			accumulate_ a(*this);

			operator++();

			return a;
		}
	};
	template<class O, class I, 
		class T = typename std::iterator_traits<I>::value_type
	>
	inline auto accumulate(O o, I i, T t)
	{
		return accumulate_<O,I,T>(o, i, t);
	}

	// running sum of enumerator values
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	inline auto sum(I i, T t = T(0))
	{
		return accumulate(std::plus<T>{}, i, t);
	}

	// running product of enumerator values
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	inline auto prod(I i, T t = T(1))
	{
		return accumulate_<std::multiplies<T>,I,T>(std::multiplies<T>{}, i, t);
	}

	// can't use std::min directly???
	template<class T>
	struct min_ {
		const T& operator()(const T& t, const T& u)
		{
			return std::min(t,u);
		}
	};
	// running min enumerator values
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	inline auto min(I i, T t = std::numeric_limits<T>::max())
	{
		return accumulate_<min_<T>,I,T>(min_<T>{}, i, t);
	}

	template<class T>
	struct max_ {
		const T& operator()(const T& t, const T& u)
		{
			return std::max(t,u);
		}
	};
	// running max enumerator values
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	inline auto max(I i, T t = -std::numeric_limits<T>::max())
	{
		return accumulate_<max_<T>,I,T>(max_<T>{}, i, t);
	}

} // iter

#ifdef _DEBUG
#include <functional>
#include "include/ensure.h"

inline void test_accumulate()
{
	int a[] = {1,2,3};
	{	
		auto b = accumulate(std::plus<int>{}, e(a), 0);
		auto c(b);
		b = c;
		ensure (*b++ == 1);
		ensure (*b == 3);
		ensure (*++b == 6);
	}
	{	
		auto b = sum(e(a));
		auto c(b);
		b = c;
		ensure (*b++ == 1);
		ensure (*b == 3);
		ensure (*++b == 6);
	}
	{	
		auto b = prod(e(a));
		auto c(b);
		b = c;
		ensure (*b++ == 1);
		ensure (*b == 2);
		ensure (*++b == 6);
	}
	{
		auto b = e(std::reverse_iterator<int*>(a + 3));
		auto c(b);
		b = c;
		ensure (*b == a[2]);
		ensure (b);
		
		int x = 4;
		// capturing lambdas have deleted copy assignment
//		auto d = accumulate([](const int& a, const int& b) { return x*a + b; }, b, 0);

		std::function<int(int,int)> horner1 = [x](const int& a, const int& b) { return x*a + b; };
		static_assert (24 <= sizeof(horner1), "MSVC: 24, gcc: 32");
//		auto d = accumulate(horner1, b, 0);

		// capture by hand
		struct horner {
			int x;
			horner(int x) : x(x) { }
			int operator()(const int& a, const int& b) { return x*a + b; }
		};
		auto h = horner(1);
		auto g(h);
		h = g;
		static_assert (sizeof(int) == sizeof(horner), "time for a new compiler");
		auto d = accumulate(horner(x), b, 0);
		ensure (*d == a[2]);
		d++;
		ensure (*d == a[1] + x*a[2]);
		ensure (*++d == a[0] + x*(a[1] + x*a[2]));

		auto f(d);
		d = f;
		ensure (d);
	}
}

#endif // _DEBUG