#include "Excercise_1.h"
#include "Utils.h"

#include <gsl/gsl_fit.h>
#include <chrono>

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

void Excercise_1::test() const
{
    const int N = 5;

    SP::IO::println("\nTesting GSL ...");

    double x[N] = {1.2, 1.6, 3.4, 4.51, 5.14};
    double y[N] = {1.3, 1.8, 3.3, 4.45, 5.35};
    double c0, c1, cov00, cov01, cov11, sumsq;

    auto t1 = chrono::system_clock::now();
    gsl_fit_linear(x, 1, y, 1, N, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
    auto t2 = chrono::system_clock::now();

    SP::IO::println("Duration: %ms", chrono::duration_cast<chrono::milliseconds>(t2-t1).count());
    SP::IO::println("Best fit: Y = % + % * x", c0, c1);
    SP::IO::println("Covariance matrix:");
    SP::IO::println("[ %, % \n  %, % ]", cov00, cov01, cov01, cov11);

    SP::IO::println("\nTesting ROOT ...");

    // 在这里通过ROOT实现线性函数拟合，并与GSL的结果对比
    // ------------------------------------------

    TF1* linear_function = new TF1("linear_function", "[0] + [1] * x", 0, 6);
    TGraph* g = new TGraph(N, x, y);

    t1 = chrono::system_clock::now();
    TFitResultPtr res = g->Fit(linear_function, "S");
    t2 = chrono::system_clock::now();

    SP::IO::println("Duration: %ms", chrono::duration_cast<chrono::milliseconds>(t2-t1).count());
    TMatrixD cov = res->GetCovarianceMatrix();
    cov.Print();

    delete linear_function;
    delete g;

    // ------------------------------------------
}
