#ifndef _MESH_3D_HPP_
#define _MESH_3D_HPP_

#include <cassert>
#include <cstddef>
#include <complex>
#include <memory>

template <typename T>
struct vector3d_t
{
	typedef T value_type;

	vector3d_t(value_type i = 0,
	           value_type j = 0,
	           value_type k = 0)
		: x(i), y(j), z(k)
	{}

	T x;
	T y;
	T z;
};

template <typename T>
bool operator<(const vector3d_t<T> &u,
               const vector3d_t<T> &v)
{
	if (u.x != v.x)
	{
		return u.x < v.x;
	}
	if (u.y != v.y)
	{
		return u.y < v.y;
	}
	return u.z < v.z;
}

typedef vector3d_t<size_t> natural0_vector3d_t;
typedef vector3d_t<double> real_vector3d_t;

template <typename T, class F>
void for_cube_rowmajor(const vector3d_t<T> &left,
                       const vector3d_t<T> &right,
                       const vector3d_t<T> &delta,
                       F func)
{
	for (T i = left.x; i < right.x; i += delta.x)
	{
		for (T j = left.y; j < right.y; j += delta.y)
		{
			for (T k = left.z; k < right.z; k += delta.z)
			{
				func(vector3d_t<T>(i, j, k));
			}
		}
	}
}

template <typename T, class F>
void for_cube_columnmajor(const vector3d_t<T> &left,
                          const vector3d_t<T> &right,
                          const vector3d_t<T> &delta,
                          F func)
{
	for_cube_rowmajor(vector3d_t<T>(left.z, left.y, left.x),
	                  vector3d_t<T>(right.z, right.y, right.x),
	                  vector3d_t<T>(delta.z, delta.y, delta.x),
	                  func);
}

template <typename T>
class mesh3d_t
{
public:
	typedef T value_type;

	mesh3d_t(const natural0_vector3d_t &size)
		: m_size(size), m_data_size(size.x * size.y * size.z),
		m_data(new value_type[m_data_size])
	{
		assert(m_data_size != 0);
	}
	value_type &operator()(const natural0_vector3d_t &p)
	{
		assert(p < m_size);
		return m_data[data_index(p)];
	}
	const natural0_vector3d_t &size() const
	{
		return m_size;
	}
	value_type *data()
	{
		return m_data.get();
	}

private:
	size_t data_index(const natural0_vector3d_t &p)
	{
		size_t index = p.x * m_size.y * m_size.z + p.y * m_size.z + p.z;
		assert(index < m_data_size);
		return index;
	}

	const natural0_vector3d_t m_size;
	const size_t m_data_size;
	const std::unique_ptr<value_type[]> m_data;
};

typedef mesh3d_t<std::complex<double>> complex_mesh3d_t;

template <class Input, class M>
void read_mesh3d(Input &input, M &mesh)
{
	for_cube_columnmajor(natural0_vector3d_t(0, 0, 0),
	                     mesh.size(),
	                     natural0_vector3d_t(1, 1, 1),
	                     [&](const natural0_vector3d_t &v) {
                      	 	input >> mesh(v);
	});
}

template <class Output, class M>
void write_mesh3d(Output &output, M &mesh)
{
	for_cube_columnmajor(natural0_vector3d_t(0, 0, 0),
	                     mesh.size(),
	                     natural0_vector3d_t(1, 1, 1),
	                     [&](const natural0_vector3d_t &v) {
                      	 	output << mesh(v) << " ";
	});
}

template <class M>
void write_mesh3d(std::ostream &output, M &mesh)
{
	for_cube_columnmajor(natural0_vector3d_t(0, 0, 0),
	                     mesh.size(),
	                     natural0_vector3d_t(1, 1, 1),
	                     [&](const natural0_vector3d_t &v) {
                      	 	output << mesh(v) << " ";
	});
}

#endif	// _MESH_3D_HPP_
