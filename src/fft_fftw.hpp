#ifndef _FFT_FFTW_HPP_
#define _FFT_FFTW_HPP_

#include <cassert>
#include "fft_base.hpp"
#include "mesh_3d.hpp"
#include <fftw3.h>

typedef int fftw_fft_direction;

template <fftw_fft_direction fft_direction, fftw_fft_direction fft_reverse_direction, class F>
class fftw_fft_3d_base_t : public fast_fourier_transform_3d_base_t<F, 
									fftw_fft_3d_base_t<fft_reverse_direction, fft_direction, F> >
{
public:
	fftw_fft_3d_base_t(F &in_out)
		: fast_fourier_transform_3d_base_t<F, 
									fftw_fft_3d_base_t<fft_reverse_direction, fft_direction, F> >(in_out),
		m_plan(generate_plan(this->m_in, this->m_out, 0))
	{}
	fftw_fft_3d_base_t(F &in, F &out)
		: fast_fourier_transform_3d_base_t<F, 
									fftw_fft_3d_base_t<fft_reverse_direction, fft_direction, F> >(in, out),
		m_plan(generate_plan(this->m_in, this->m_out, 0))
	{}
	virtual ~fftw_fft_3d_base_t()
	{
		fftw_destroy_plan(m_plan);
	}
	F &operator()() const
	{
		fftw_execute(m_plan);
		return this->m_out;
	}

	// On-demand fftw plan generation methods

	template <class G>
	G &operator()(F &f, G &g) const
	{
		natural0_vector3d_t size(f.size());
		//using FFTW_ESTIMATE so input/output arrays won't be overwritten
		fftw_plan plan = generate_plan(f, g, FFTW_ESTIMATE);
		fftw_execute(plan);
		fftw_destroy_plan(plan);
		return g;
	}
	F &operator()(F &f) const
	{
		return (*this)(f, f);
	}
private:
	fftw_plan generate_plan(F &in, F &out, unsigned int flags)
	{
		natural0_vector3d_t size(in.size());
		fftw_plan plan = fftw_plan_dft_3d(size.x, size.y, size.z,
								reinterpret_cast<fftw_complex*>(in.data()),
								reinterpret_cast<fftw_complex*>(out.data()),
								fft_direction, flags);
		assert(plan != NULL);
		return plan;
	}
	const fftw_plan m_plan;
};

template <class F>
struct fftw_fft_3d_t
{
	typedef fftw_fft_3d_base_t<FFTW_FORWARD,FFTW_BACKWARD,F> type;
};

#endif	// _FFT_FFTW_HPP_
