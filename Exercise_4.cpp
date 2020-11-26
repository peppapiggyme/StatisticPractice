#include "Exercise_4.h"
#include "ExUtils.h"
#include "Utils.h"

// ROOT include(s)
#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TFitResult.h"
#include "TStyle.h"
#include "TMath.h"
#include "TRandom3.h"
#include "Math/MinimizerOptions.h"
#include "TGraphErrors.h"

// std include(s)
#include <iostream>

using namespace std;

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
    auto r1 = gr->Fit(f1,"S R +");
    r1->GetCorrelationMatrix().Print();

    // straight line fit to positive points
    f1->SetRange(0,12);
    f1->SetLineColor(kBlack);
    auto r2 = gr->Fit(f1,"S R +");
    r2->GetCorrelationMatrix().Print();

    // straight line fit to all points
    f1->SetRange(-12,12);
    f1->SetLineColor(kRed);
    auto r3 = gr->Fit(f1,"S +");
    r3->GetCorrelationMatrix().Print();

    c1->SaveAs("../plots/Ex4_fits.png");

    // clear up
    delete gr;
    delete c1;
    
}
