#include <complex>
#include <iostream>
#include <fstream>

#include "math_base.hpp"
#define M_PI math_constants::pi

struct dft_reference_t
{
public:
	void forward(std::complex<double> *input, std::complex<double> *output, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			output[i] = forward(input, i, size);
		}
	}

	void backward(std::complex<double> *input, std::complex<double> *output, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			output[i] = backward(input, i, size);
		}
	}

private:
	std::complex<double> forward(std::complex<double> *val, size_t point, size_t size)
	{
		return dft(1.0, 1.0, val, point, size);
	}

	// aka inverse transform
	std::complex<double> backward(std::complex<double> *val, size_t point, size_t size)
	{
		return dft(1.0 / size, -1.0, val, point, size);
	}

	std::complex<double> dft(double sum_factor, double exp_arg_sign,
                             std::complex<double> *val, size_t point, size_t size)
	{
		const double inverse_size   = 1.0 / size;
		const double exp_arg_factor = exp_arg_sign * math_constants::two_pi * point * inverse_size;
		
		std::complex<double> sum = 0;
		for (size_t i = 0; i < size; ++i)
		{
			sum += val[i] * std::exp(std::complex<double>(0, exp_arg_factor * i));
		}
		return sum_factor * sum;
	}
};

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <vector>
#include <complex>
     
using namespace std;
     
typedef complex<double> cd;
typedef vector<cd> vcd;
     
vcd fft(const vcd &as) {
    int n = as.size();
    int k = 0; // Длина n в битах
    while ((1 << k) < n) k++;
    vector<int> rev(n);
    rev[0] = 0;
    int high1 = -1;
    for (int i = 1; i < n; i++) {
    if ((i & (i - 1)) == 0) // Проверка на степень двойки. Если i ей является, то i-1 будет состоять из кучи единиц.
        high1++;
    rev[i] = rev[i ^ (1 << high1)]; // Переворачиваем остаток
    rev[i] |= (1 << (k - high1 - 1)); // Добавляем старший бит
    }
     
    vcd roots(n);
    for (int i = 0; i < n; i++) {
    double alpha = 2 * M_PI * i / n;
    roots[i] = cd(cos(alpha), sin(alpha));
    }
     
    vcd cur(n);
    for (int i = 0; i < n; i++)
    cur[i] = as[rev[i]];
     
    for (int len = 1; len < n; len <<= 1) {
    vcd ncur(n);
    int rstep = roots.size() / (len * 2);
    for (int pdest = 0; pdest < n;) {
        int p1 = pdest;
        for (int i = 0; i < len; i++) {
        cd val = roots[i * rstep] * cur[p1 + len];
        ncur[pdest] = cur[p1] + val;
        ncur[pdest + len] = cur[p1] - val;
        pdest++, p1++;
        }
        pdest += len;
    }
    cur.swap(ncur);
    }
    return cur;
}
     
vcd fft_rev(const vcd &as) {
    vcd res = fft(as);
    for (int i = 0; i < (int)res.size(); i++) res[i] /= as.size();
    reverse(res.begin() + 1, res.end());
    return res;
}
     
int fft_hbr_start() {
    int n;
    scanf("%d", &n);
    vcd as(n);
    for (int i = 0; i < n; i++) {
    int x;
    scanf("%d", &x);
    as[i] = x;
    }

	for (int i = 0; i < n; i++)
	//printf("%.4lf %.4lf\n", as[i].real(), as[i].imag());
    printf("%e %e\n", as[i].real(), as[i].imag());
     
    clock_t stime = clock();
    vcd res = fft(as);
    fprintf(stderr, "fft: %d\n", (int)(clock() - stime));
    for (int i = 0; i < n; i++)
    //printf("%.4lf %.4lf\n", res[i].real(), res[i].imag());
	printf("%e %e\n", res[i].real(), res[i].imag());
     
    stime = clock();
    vcd as2 = fft_rev(res);
    fprintf(stderr, "fft_rev: %d\n", (int)(clock() - stime));
    for (int i = 0; i < n; i++)
    //printf("%.4lf %.4lf\n", as2[i].real(), as2[i].imag());
	printf("%e %e\n", as2[i].real(), as2[i].imag());

    return 0;
}
