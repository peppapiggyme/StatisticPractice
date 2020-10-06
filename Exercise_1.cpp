#include "Exercise_1.h"
#include "Utils.h"

#include <gsl/gsl_fit.h>

#include "TF1.h"
#include "TGraph.h"
#include "TMatrixD.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"

/**
 * @brief
 *
 * @name Ex1
 * @author Bowen Zhang
 *
 */

using namespace std;

void Exercise_1::test() const
{
    const int N = 5;

    SP::IO::println("\nTesting GSL ...");

    double x[N] = {1.2, 1.6, 3.4, 4.51, 5.14};
    double y[N] = {1.3, 1.8, 3.3, 4.45, 5.35};
    double c0, c1, cov00, cov01, cov11, sumsq;

    auto run_gsl = [&](){ gsl_fit_linear(x, 1, y, 1, N, &c0, &c1, &cov00, &cov01, &cov11, &sumsq); };

    auto dTime = SP::STL::FunctionRunTime(run_gsl);

    SP::IO::println("Duration: %ms", dTime);

    SP::IO::println("Best fit: Y = % + % * x", c0, c1);
    SP::IO::println("Covariance matrix:");
    SP::IO::println("[ %, % \n  %, % ]", cov00, cov01, cov01, cov11);

    SP::IO::println("\nTesting ROOT ...");

    // 在这里通过ROOT实现线性函数拟合，并与GSL的结果对比
    // ------------------------------------------

    TF1* linear_function = new TF1("linear_function", "[0] + [1] * x", 0, 6);
    TGraph* g = new TGraph(N, x, y);
    TFitResultPtr res = nullptr;

    auto run_root = [&](){ res = g->Fit(linear_function, "S"); };

    dTime = SP::STL::FunctionRunTime(run_root);

    SP::IO::println("Duration: %ms", dTime);

    TMatrixD cov = res->GetCovarianceMatrix();
    cov.Print();

    delete linear_function;
    delete g;

    // ------------------------------------------
}
