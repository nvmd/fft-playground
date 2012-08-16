#ifndef _MATH_BASE_HPP_
#define _MATH_BASE_HPP_

namespace math_constants
{
const double pi = 3.141592653589793238462643383279502884197;
const double two_pi = 6.283185307179586476925286766559005768394;
}

template <class T>
struct invertible_function
{
	typedef T inverse;
};

#endif	// _MATH_BASE_HPP_
