#include "Exercise_6.h"
#include "ExUtils.h"
#include "Utils.h"

#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TStyle.h"
#include "TRandom3.h"
#include "TLine.h"

#include <iostream>
#include <cmath>

#define EXIT_HERE return;

using namespace std;
using namespace SP;

void Exercise_6::test() const
{
    TFile* cData = TFile::Open("../plots/data.root");
    TFile* cBkg  = TFile::Open("../plots/bkg.root");
    TFile* cSig  = TFile::Open("../plots/sig.root");
    
    TH1D* hData = (TH1D*)cData->Get("m4l")->Clone("data");
    hData->SetDirectory(0);
    TH1D* hBkg = (TH1D*)cBkg->Get("m4l")->Clone("bkg");
    hBkg->SetDirectory(0);
    TH1D* hSig = (TH1D*)cSig->Get("m4l")->Clone("sig");
    hSig->SetDirectory(0);

    // Prepare S + B histogram
    TH1D *hSPlusB = (TH1D*)hBkg->Clone("m4l_splusb");
    hSPlusB->Reset();
    for (int i = 1; i < hBkg->GetNbinsX(); i++)
        hSPlusB->SetBinContent( i, hSig->GetBinContent(i) + hBkg->GetBinContent(i));

    // plotting
    TCanvas * c1 = new TCanvas( "c1_ex6", "m_{4l}", 800, 600);

    hSig->SetFillColor(2);
    hSPlusB->SetFillColor(2);
    hSPlusB->SetAxisRange(0.,25.,"Y");
    hSPlusB->SetAxisRange(50.,400.,"X");
    hSPlusB->GetXaxis()->SetTitleOffset(1.0);
    hSPlusB->GetYaxis()->SetTitleOffset(1.0);
    hSPlusB->Draw("hist");

    hBkg->SetFillColor(7);
    hBkg->Draw("hist same");
    hBkg->Draw("axis same");
    hData->Draw("e same");

    hSPlusB->SetXTitle("4-lepton invariant mass [GeV]");
    hSPlusB->SetYTitle(Form("Number of events / %3.1f GeV", hData->GetBinWidth(1)));

    TLegend *l = new TLegend(0.65, 0.65, 0.90, 0.80);
    l->SetBorderSize(0);
    l->SetFillColor(0);
    TLegendEntry *le1 = l->AddEntry(hSig, "Higgs", "f");
    TLegendEntry *le2 = l->AddEntry(hBkg, "Background", "f");
    le1->SetTextSize(0.05);
    le2->SetTextSize(0.05);
    l->Draw();

    c1->SaveAs(std::string("../plots/Ex6_m4l_H125.png").c_str());

    // total number of events in a mass window
    double nData{0};
    double nBkg{0};
    double nSig{0};

    // number of events in the i^th bin
    double nData_i{0};
    double nBkg_i{0};
    double nSig_i{0};

    // the center of the i^th bin in the 4l mass distribution
    double m4l_i;

    for (unsigned i = 1; i < (unsigned)hBkg->GetNbinsX(); i++) {
        m4l_i = hData->GetBinCenter(i);
        nData_i = hData->GetBinContent(i);
        nBkg_i = hBkg->GetBinContent(i);
        nSig_i = hSig->GetBinContent(i);

        // Can you optimise the mass window selection
        // [TASK] modify this
        double width = 5.;
        if (std::fabs(m4l_i - 125.) < width) {
            IO::println("Bin %: mass = % | data = %, bkg = %, sig = %",
                        i, m4l_i, nData_i, nBkg_i, nSig_i);
            nData += nData_i;
            nBkg  += nBkg_i;
            nSig  += nSig_i;
        }
    }
    
    // assume we know nBkg from a previous experiment
    // i.e. we have measured b to have a value b0 with an uncertainty nBkgErr
    // [TASK] modify this
    double nBkgErr = 0.2;

    // Hypothesis test
    // H0 (null hypothesis) : no Higgs (background only)
    // H1 (alternative hypothesis) : Higgs exists
    
    // nData is another measurement
    // Revise: Use ML to get the best fit value of nuisance parameter
    double mu = 0; // H0
    // remove constant terms (w.r.t. nu)
    auto nll = [&](double* p, double* )
    {
        double nu = p[0];
        double b = nBkg * (1. + nu * nBkgErr);
        double s = nSig;
        double N = nData;
        return b + 2. * nu * nu - N * TMath::Log(mu * s + b);
    };
    
    auto fnll = new TF1("nll", nll, -5, 5, 0);
    fnll->SetTitle("Negative Log-likelihood function;nu;NLL");
    fnll->Draw();
    c1->SaveAs("../plots/Ex6_nll.png");

    // generate toy mc
    TRandom3 r(0);
    unsigned long nToys = 100000000; // 1e8 x 2.7e-7 = 27
    // number of events is our test statistic here
    // generate N's distribution
    TH1D* hEvent = new TH1D("Ex6_h", "", 100, 0, 100);
    for (unsigned long i = 0; i < nToys; i++)
    {
        double nuRandom = r.Gaus(0, 1);
        double b = nBkg * (1. + nuRandom * nBkgErr);
        double s = nSig;
        double nRandom = r.Poisson(mu * s + b);
        hEvent->Fill(nRandom);
        if ((i + 1) % 10000000 == 0)
            IO::println("% / % generated", i + 1, nToys);
    }
    TH1D* hEventMu = new TH1D("Ex6_h_mu", "", 100, 0, 100);
    mu = 1.; // H1
    for (unsigned long i = 0; i < nToys; i++)
    {
        double nuRandom = r.Gaus(0, 1);
        double b = nBkg * (1. + nuRandom * nBkgErr);
        double s = nSig;
        double nRandom = r.Poisson(mu * s + b);
        hEventMu->Fill(nRandom);
        if ((i + 1) % 10000000 == 0)
            IO::println("% / % generated", i + 1, nToys);
    }

    hEvent->SetTitle(";N;Event");
    hEvent->Draw();
    hEventMu->SetLineColor(3);
    hEventMu->Draw("same");
    // expected N
    double nExp{0}, q{0.5};
    hEventMu->GetQuantiles(1, &nExp, &q);
    TLine* lExp = new TLine(nExp, 0., nExp, hEvent->GetMaximum());
    lExp->SetLineColor(3);
    lExp->SetLineWidth(2);
    lExp->Draw("SAME");
    c1->SetLogy(true);
    c1->SaveAs("../plots/Ex6_Nexp.png");
    
    auto pValue = [&hEvent](double n) {
        return hEvent->Integral(hEvent->FindBin(round(n)), hEvent->GetNbinsX()) / hEvent->Integral();
    };

    double nObs = nData;
    double pValueObs = pValue(nObs);
    double pValueExp = pValue(nExp);
    double zObs = SPEx::GetSignificance(pValueObs);
    double zExp = SPEx::GetSignificance(pValueExp);

    SP::IO::println("Observed number of events = %, p-value = %, Z = %\n"
                    "Expected number of events = %, p-value = %, Z = %\n", 
                    nObs, pValueObs, zObs, round(nExp), pValueExp, zExp);
    
    hEvent->SetTitle(";N;Event");
    hEvent->Draw();
    c1->SetLogy(true);
    TLine* lObs = new TLine(nObs, 0., nObs, hEvent->GetMaximum());
    lObs->SetLineColor(kRed);
    lObs->SetLineWidth(2);
    lObs->Draw("SAME");
    c1->SaveAs("../plots/Ex6_Nobs.png");    

    // clear up 
    delete l;
    delete fnll;
    delete hEvent;
    delete hEventMu;
    delete lExp;
    delete lObs;
    delete c1;
}
