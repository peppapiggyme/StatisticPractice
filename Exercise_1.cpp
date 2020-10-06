#include "Exercise_1.h"
#include "Utils.h"

#include "TH1D.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TRandom.h"
#include "TF1.h"
#include "TMath.h"
#include "TMinuitMinimizer.h"
#include "TMatrixD.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"

/**
 * @brief 回顾之前学到的 ROOT Fit 功能
 *
 * reference:
 * https://github.com/root-project/training/tree/master/BasicCourse/Exercises/Fitting
 *
 * @name Ex1
 * @author Bowen Zhang
 *
 */

using namespace std;

void Exercise_1::test() const
{
    // --------------------------------------------------------------------------
    // Preparation of the histogram to fit

    // Create First an empty histogram with 50 bins with range [-10,10]
    TH1D h1("h1", "h1", 50, -10, 10);

    // Fill the histogram with 10000 Gaussian Random number with mean=1 and
    // sigma=2

    // PLEASE FILL THIS
    for (int i = 0; i < 10000; ++i) {
      h1.Fill(gRandom->Gaus(1, 2));
    }

    // Let's now draw the histogram and save the image as .png file

    // PLEASE FILL THIS
    TCanvas c("c", "Ex1_c", 800, 600);
    h1.Draw();

    // --------------------------------------------------------------------------
    // Before Fitting we need to create the fitting function and set its initial
    // parameter values.

    auto f1 = new TF1("f1", "gaus", -10,
                      10); // We use new to have this surviving the scope
    f1->SetParameters(100, 0, 1);

    // We fit now the histogram using the Fit method in ROOT. By default the
    // least-square method is used. For likelihood fits we need to use the option
    // "L". The option "S" is used to create a TFitResult object that is returned
    // to the user. If we want to compute the error using MINOS, we use the "E"
    // option We want to change also the default Minimization engine. We will use
    // Minuit2

    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");

    auto res = h1.Fit(f1, "L S E");

    // Print the result

    // PLEASE FILL THIS
    res->Print();

    // We now get the correlation matrix of the fit from the TFitResult class

    // PLEASE FILL THIS
    auto corMatrix = res->GetCorrelationMatrix();
    auto covMatrix = res->GetCovarianceMatrix();
    corMatrix.Print();
    covMatrix.Print();
    SP::IO::println("Gaussian sigma = % +/- %",
                    f1->GetParameter("Sigma"),
                    f1->GetParError(f1->GetParNumber("Sigma")));

    c.SaveAs("../plots/Ex1_gaus.png");
}
