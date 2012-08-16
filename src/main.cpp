
#include <iostream>
#include "fft_fftw.hpp"
#include "mesh_3d.hpp"

template <class Input, class OutputIterator>
void read_data(Input &input, size_t size, OutputIterator out)
{
	while (size--)
	{
		input >> *out++;
	}
}

template <class Output, class OutputIterator>
void write_data(Output &output, size_t size, OutputIterator out)
{
	while (size--)
	{
		output << *out++;
	}
}

template <class InputIterator>
void write_data(std::ostream &output, size_t size, InputIterator out)
{
	while (size--)
	{
		output << *out++ << " ";
	}
}

int main(int argc, char **argv)
{
	//std::ifstream input("in.txt");
	//std::ofstream output("out.txt");
	std::istream &input = std::cin;
	std::ostream &output = std::cout;
	
	natural0_vector3d_t size;
	input >> size.x >> size.y >> size.z;

	complex_mesh3d_t mesh(size);
	read_mesh3d(input, mesh);
	
	output << "\ndata: \n";
	write_mesh3d(output, mesh);

	output << "\nforward: \n";
	complex_fft_3d_fftw_t fft(mesh);
	fft();
	write_mesh3d(output, mesh);

	output << "\ninverse: \n";
	complex_fft_3d_fftw_t::inverse fft_inv(mesh);
	fft_inv();
	write_mesh3d(output, mesh);

	return 0;
}
