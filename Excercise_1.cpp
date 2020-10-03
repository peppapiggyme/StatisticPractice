#include "Excercise_1.h"
#include "Utils.h"

#include <gsl/gsl_fit.h>

#include "TGraph.h"
#include "TF1.h"

/**
 * int gsl_fit_linear(const double * x, const size_t xstride,
 * const double * y, const size_t ystride,
 * size_t n,
 * double * c0, double * c1,
 * double * cov00, double * cov01, double * cov11,
 * double * sumsq)
 *
 * @name Ex1
 * @author Bowen Zhang
 *
 */

using namespace std;

void Excercise_1::test() const
{
    SP::IO::println("\nTesting GSL ...");

    double x[5] = {1.2, 1.6, 3.4, 4.51, 5.14};
    double y[5] = {1.3, 1.8, 3.3, 4.45, 5.35};
    double c0, c1, cov00, cov01, cov11, sumsq;

    gsl_fit_linear(x, 1, y, 1, 5, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);

    SP::IO::println("Best fit: Y = % + % * x", c0, c1);
    SP::IO::println("Covariance matrix:");
    SP::IO::println("[ %, % \n  %, % ]", cov00, cov01, cov01, cov11);

    SP::IO::println("\nTesting ROOT ...");

    // 在这里通过ROOT实现线性函数拟合，并与GSL的结果对比
    // ------------------------------------------








    // ------------------------------------------
}
