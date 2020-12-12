#include "Exercise_7.h"
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
#include "TText.h"

#include <iostream>
#include <cmath>

#define EXIT_HERE return;

using namespace std;
using namespace SP;

void Exercise_7::test() const
{
    TFile* cData = TFile::Open("../plots/data.root");
    TFile* cBkg  = TFile::Open("../plots/bkg.root");
    TFile* cSig  = TFile::Open("../plots/Ex5_newsig_GZZ.root");
    
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
    TCanvas * c1 = new TCanvas( "c1_ex7", "m_{4l}", 800, 600);

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
    TLegendEntry *le1 = l->AddEntry(hSig, "G->ZZ", "f");
    TLegendEntry *le2 = l->AddEntry(hBkg, "Background", "f");
    le1->SetTextSize(0.05);
    le2->SetTextSize(0.05);
    l->Draw();

    c1->SaveAs(std::string("../plots/Ex7_m4l_H125.png").c_str());

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
        double width = 10.;
        if (std::fabs(m4l_i - 200.) < width) {
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
    // H0 (null hypothesis) : mu = mu0
    // H1 (alternative hypothesis) : mu < mu0
    double mu = 0.0;
    double nScan = 15;
    double muMin = 0.0;
    double muMax = 1.4;
    int nCounter = 0;
    double x[(int)nScan];
    double yObs[(int)nScan];
    double yExp[(int)nScan];
    unsigned long nToys = 1000000; // 1e6 x 0.05 = 5e4

    while (nCounter < nScan)
    {
        SP::IO::println("Scanning mu = %", mu);
        // generate toy mc
        TRandom3 r(nCounter);
        
        // number of events is our test statistic here
        // generate N's distribution
        TH1D* hEvent = new TH1D("Ex7_h", "", 150, 0, 150);
        for (unsigned long i = 0; i < nToys; i++)
        {
            double nuRandom = r.Gaus(0, 1);
            double b = nBkg * (1. + nuRandom * nBkgErr);
            double s = nSig;
            double nRandom = r.Poisson(mu * s + b);
            hEvent->Fill(nRandom);
        }
        TH1D* hEventBkgOnly = new TH1D("Ex7_h_bkgOnly", "", 150, 0, 150);
        for (unsigned long i = 0; i < nToys; i++)
        {
            double nuRandom = r.Gaus(0, 1);
            double b = nBkg * (1. + nuRandom * nBkgErr);
            double nRandom = r.Poisson(b);
            hEventBkgOnly->Fill(nRandom);
        }

        hEvent->SetTitle(";N;Event");
        hEvent->Draw();
        hEventBkgOnly->SetLineColor(4);
        hEventBkgOnly->Draw("same");
        // expected N
        double nExp{0}, q{0.5};
        hEventBkgOnly->GetQuantiles(1, &nExp, &q);
        TLine* lExp = new TLine(nExp, 0., nExp, hEvent->GetMaximum());
        lExp->SetLineColor(4);
        lExp->SetLineWidth(2);
        lExp->Draw("SAME");
        // observed N
        double nObs = nData;
        TLine* lObs = new TLine(nObs, 0., nObs, hEvent->GetMaximum());
        lObs->SetLineColor(2);
        lObs->SetLineWidth(2);
        lObs->Draw("SAME");

        TText tex(.5, .8, string("#mu = " + to_string(mu)).c_str());
        tex.SetNDC();
        tex.Draw();

        c1->SetLogy(true);
        c1->SaveAs(string("../plots/Ex7_N_" + to_string(nCounter) + ".png").c_str());

        auto pValue = [&hEvent](double n) {
            return hEvent->Integral(0, hEvent->FindBin(n)) / hEvent->Integral();
        };

        x[nCounter] = mu;
        yObs[nCounter] = pValue(nObs);
        yExp[nCounter] = pValue(nExp);

        SP::IO::println("Summary: mu=%, obs p-value=%, exp p-value=%", mu, yObs[nCounter], yExp[nCounter]);

        mu += (muMax - muMin) / (nScan - 1);
        nCounter++;

        delete hEvent;
        delete hEventBkgOnly;
        delete lExp;
        delete lObs;
    }

    TGraph* gObs = new TGraph((int)nScan, x, yObs);
    TGraph* gExp = new TGraph((int)nScan, x, yExp);
    TGraph* gObsForEval = new TGraph((int)nScan, yObs, x);
    TGraph* gExpForEval = new TGraph((int)nScan, yExp, x);
    gObs->SetMinimum(0.0);
    gObs->SetTitle(";#mu;p-value");

    // styles
    gObs->SetLineColor(kBlack);
    gObs->SetLineWidth(2);
    gObs->SetMarkerSize(0);
    gExp->SetLineColor(kBlue);
    gExp->SetLineWidth(2);
    gExp->SetMarkerSize(0);

    TLine* pValue_0p05 = new TLine(gObs->GetXaxis()->GetXmin(), 0.05, gObs->GetXaxis()->GetXmax(), 0.05);
    pValue_0p05->SetLineColor(kRed);
    pValue_0p05->SetLineWidth(2);

    TCanvas* c = new TCanvas("Ex7_pValue", "mu Scan", 800, 600);
    gObs->Draw();
    gExp->Draw("SAME");
    pValue_0p05->Draw("SAME");
    c->SaveAs("../plots/Ex7_MuScan.png");

    SP::IO::println("Upper limit = %, Expected upper limit = %", gObsForEval->Eval(0.05), gExpForEval->Eval(0.05));

    // clear up
    delete gObs;
    delete gExp;
    delete gObsForEval;
    delete gExpForEval;
    delete pValue_0p05;
    delete l;
    delete c1;

}
