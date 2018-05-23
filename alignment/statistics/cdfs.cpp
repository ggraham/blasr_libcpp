#include <alignment/statistics/cdfs.hpp>

float NormalCDF(float mu, float sigmaSq, float x)
{
    float nStdDev = (x - mu) / sqrt(sigmaSq);
    if ((int)nStdDev <= -10.0) return 0;
    if ((int)nStdDev >= 10.0) return 1;

    int cdfindex = 1000 + 100 * nStdDev;
    assert(cdfindex >= 0);
    assert(cdfindex <= 2000);
    if (cdfindex == 2000) {
        return 1;
    }
    //	std::cout << "nstdev: " << nStdDev << " mu " << mu
    //       << " sigma " << sigmaSq << " x " << x
    //       << " norm cdf index: " << cdfindex << std::endl;
    return NormCDFTable[cdfindex];
}

float PoissonCDF(float lambda, int a, int b)
{
    float cdf = 0;
    int i;
    for (i = a; i <= b; i++) {
        // std::cout << "poiss " << lambda << ", " << i
        //      << " " << Poisson(lambda, i) << std::endl;
        cdf += Poisson(lambda, i);
    }
    return cdf;
}

float PoissonCDF(float lambda, int a)
{

    float cdf = 0;
    int i;
    float p;
    float epsilon = 0.000000000001;
    if (lambda > (int)(FactorialTableLength * 2 / 3)) {
        // lambda is too large, use normal approximation.
        // std::cout << "using table: lambda: " << lambda
        //      << " a: " << a << std::endl;
        return NormalCDF(lambda, lambda, a);
    }

    for (i = 0; i <= a; i++) {
        p = Poisson(lambda, i);
        if (p < epsilon and i > (int)lambda) break;
        // std::cout << "p: " << p << std::endl;
        cdf += p;
    }
    return cdf;
}
