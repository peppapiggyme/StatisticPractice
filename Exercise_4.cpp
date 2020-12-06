#include "Exercise_4.h"
#include "ExUtils.h"
#include "Utils.h"

// ROOT include(s)
#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TFitResult.h"
#include "TRandom3.h"
#include "Math/MinimizerOptions.h"
#include "TGraphErrors.h"
#include "TMatrixD.h"
#include "TVectorD.h"

// std include(s)
#include <iostream>

using namespace std;
using namespace SP;

void Exercise_4::test() const
{
    double xpoints[] = { -11, -10, -9, 9, 10, 11 };
    auto f1 = new TF1("Ex4_f1", "[A] * x + [B]");
    f1->SetParameter("A", 0.5);
    f1->SetParameter("B", 0.);
    
    // Generate some random y points with an error of 1
    double ypoints[6];
    double sigma = 1;
    TRandom3 r(0);
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
    
    for (int i = 0; i < 6; ++i)
        ypoints[i] = f1->Eval(xpoints[i]) + r.Gaus(sigma);

    // 
    auto c1 = new TCanvas("Ex4_c1", "", 800, 600);

    auto gr = new TGraphErrors(6, xpoints, ypoints);
    for (int i = 0; i < 6; ++i)
        gr->SetPointError(i, 0, 1);

    gr->SetMarkerStyle(20);
    gr->Draw("AP");
    
    // straight line fit to negative points
    f1->SetRange(-12, 0);
    f1->SetLineColor(kBlue);
    auto r1 = gr->Fit(f1, "S R +");
    r1->GetCorrelationMatrix().Print();

    // straight line fit to positive points
    f1->SetRange(0,12);
    f1->SetLineColor(kBlack);
    auto r2 = gr->Fit(f1, "S R +");
    r2->GetCorrelationMatrix().Print();

    // straight line fit to all points
    f1->SetRange(-12,12);
    f1->SetLineColor(kRed);
    auto r3 = gr->Fit(f1, "S +");
    r3->GetCorrelationMatrix().Print();

    c1->SaveAs("../plots/Ex4_fits.png");

    // Ignoring correlations
    int ipar = 0;
    double v1 = r1->Parameter(ipar);
    double err1 = r1->Error(ipar);
    double v2 = r2->Parameter(ipar);
    double err2 = r2->Error(ipar);
    
    // weight = ...
    double w1 = 1. / (err1 * err1);
    double w2 = 1. / (err2 * err2);
    double wtot = w1 + w2;
    double value_comb = (w1*v1 + w2*v2)/wtot;
    double err_comb = std::sqrt(1. / wtot);
    
    // this result does not make sense (?)
    IO::println("The combined value for paramter % is : % +- %", 
                r1->ParName(ipar), value_comb, err_comb);

    // Consider correlations
    TMatrixD M1 = r1->GetCovarianceMatrix();
    TMatrixD M2 = r2->GetCovarianceMatrix();
    TMatrixD W1 = M1.Invert();
    TMatrixD W2 = M2.Invert();

    // Combined error
    auto Wcomb = W1 + W2;
    auto Mcomb = Wcomb.Invert();
    Wcomb.Print();
    IO::println("Combined error on A = %", sqrt(Mcomb(0, 0)));
    IO::println("Combined error on B = %", sqrt(Mcomb(1, 1)));
    
    // Combined value
    TVectorD Vec1(2, r1->GetParams());
    TVectorD Vec2(2, r2->GetParams());

    TVectorD Vec_comb(2);
    Vec_comb = W1*Vec1 + W2*Vec2;
    Vec_comb = Mcomb * Vec_comb; 
    Vec_comb.Print();

    IO::println("Computed combined values are:");
    IO::println("A = % +/- %", Vec_comb(0), sqrt(Mcomb(0, 0)));
    IO::println("B = % +/- %", Vec_comb(1), sqrt(Mcomb(1, 1)));

    // clear up
    delete f1;
    delete gr;
    delete c1;
}
