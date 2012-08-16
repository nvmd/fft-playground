#ifndef _FFT_BASE_HPP_
#define _FFT_BASE_HPP_

#include <cassert>
#include "math_base.hpp"

template <class F, class I>
class fast_fourier_transform_3d_base_t : public invertible_function<I>
{
public:
	fast_fourier_transform_3d_base_t(F &in_out)
		: m_in(in_out), m_out(in_out)
	{
		assert(m_in.size().x != 0
				&& m_in.size().y != 0
				&& m_in.size().z != 0);
	}
	fast_fourier_transform_3d_base_t(F &in, F &out)
		: m_in(in), m_out(out)
	{
		assert(m_in.size() == m_out.size());
		assert(m_in.size().x != 0
				&& m_in.size().y != 0
				&& m_in.size().z != 0);
	}
	virtual ~fast_fourier_transform_3d_base_t()
	{}
	//F &operator()() const
	//{}
protected:
	F &m_in;
	F &m_out;
};

#endif	// _FFT_BASE_HPP_
