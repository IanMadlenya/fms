// last.h - last item in an enumeration
// last, back = *last
#pragma once
#include <utility>
#include "enumerator/counted.h"
#include "enumerator/reverse.h"

namespace iter {

	/// iterate to just before end
	template<class E>
	inline E last(E e)
	{
		E l(e);

		while (e)
			l = e++;

		return l;
	}
	template<class E>
	inline E end(E e)
	{
		while (++e)
			;

		return e;
	}
	// counted reverse enumerator
	template<class R>
	inline auto rend(R r)
	{
		size_t n = r.size();
		std::advance(r, n);

		return r;//e(re(r),n);
	}
	template<class E>
	inline typename std::iterator_traits<E>::value_type back(E e)
	{
		return *last(e);
	}

} // iter

#ifdef _DEBUG
#include <cassert>

inline void test_last() {
	int a[] = {0,1,2};

	assert (*iter::last(e(a,3)) == 2);
/*
	auto ra = iter::rend(e(a,3));
	assert (ra && *ra == 2);
	++ra;
	assert (ra && *ra == 1);
	ra++;
	assert (ra && *ra == 0);
	ra++;
	assert (!ra);
*/
}

#endif // _DEBUG