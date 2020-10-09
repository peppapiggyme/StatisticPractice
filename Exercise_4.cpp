#include "Exercise_4.h"
#include "Utils.h"
#include "ExUtils.h"

// ROOT includes
#include "TCanvas.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TMath.h"
#include "Math/ProbFunc.h"
#include "TRandom.h"
#include "TLine.h"
#include "TGraph.h"

#include <iostream>

/**
 * @brief mass window cut, counting experiment, Poisson distribution,
 * Profiled likelihood ratio, p-value, upper limits, CLs
 *
 * Root file took from here:
 * https://indico.cern.ch/event/292887/sessions/55614/attachments/547750/754994/2014_07_25_HASCO_IvovanVulpen.pdf
 *
 * @note: when you optimise the mass window selection.
 * Remember, always optimise the EXPECTED upper limits :)
 *
 * @name Ex4
 * @author Bowen Zhang
 * @date 2020/10/07
 *
 */

using namespace std;

void Exercise_4::test() const
{
    // ====
    //  ||
    //  ||
    // ====

    // Counting in the mass window
    // ---------------------------

    // preparation
    SPEx::CountingExperiment Ex4(0.0, 0.0, 0.0, 0.0);

    int nRebin = 20;

    std::string sSignal = "h_m4l_Higgs200_fake";

    // The RES_PATH is set by tools/SetupEnv.sh
    // ----------------------------------------
    std::string filePath = std::string(getenv("RES_PATH")) + "Ex4.root";

    // ROOT I/O
    // --------
    TDirectory* dir = gDirectory;
    TFile* f = TFile::Open(filePath.c_str(), "READ");

    // Get Histograms
    // --------------
    dir->cd();
    TH1D *hSig  = (TH1D*) f->Get(sSignal.c_str())->Clone("h_sig");
    TH1D *hBkg  = (TH1D*) f->Get("h_m4l_ZZ_fake")->Clone("h_bgr");
    TH1D *hData = (TH1D*) f->Get("h_m4l_data_fake")->Clone("h_fake");
    f->Close();
    f = nullptr;

    // Rebin
    // -----
    hSig->Rebin(nRebin);
    hBkg->Rebin(nRebin);
    hData->Rebin(nRebin);
    SP::IO::println("Rebinning the histograms with a factor %. Binwidth is now % GeV\n",
                    nRebin, hData->GetBinWidth(1));

    // print mass and number of events in mass window around 125 GeV
    // ----------------------------------------------------------------------
    double mass4l_i = 0.;
    double nSig_i = 0.;
    double nBkg_i = 0.;
    double nData_i = 0.;
    for (int i = 1; i< hBkg->GetNbinsX(); i++) {
        mass4l_i = hData->GetBinCenter(i);
        nSig_i  = hSig->GetBinContent(i);
        nBkg_i  = hBkg->GetBinContent(i);
        nData_i = hData->GetBinContent(i);

        // Can you optimise the mass window
        // PLEASE FILL THIS
        double width = 10.;
        if (std::fabs(mass4l_i - 200.) < width) {  // HC: mass = 200
            SP::IO::println("Bin %: mass = % | Nsig = %  Nbgr = % and Ndata = %",
                            i, mass4l_i, nSig_i, nBkg_i, nData_i);
            // Count the number of events in mass window
            // PLEASE FILL THIS
            Ex4.nData += nData_i;
            Ex4.nBkg  += nBkg_i;
            Ex4.nSig  += nSig_i;
        }
    }

    // =======
    //  || ||
    //  || ||
    // =======

    // H0: if N ~ Pois(mu=mu0)
    // H1: if N ~ Pois(mu<mu0)
    // e.g. compute qmu0 exclusion p-value pmu0
    // tune mu0 until 95% CL exclusion (pmu0 = 5%) is reached
    // ------------------------------------------------------

    // HOMEWORK: use number of events as the test statistic
    // calculate the upper limit of the 200 GeV signal and
    // compare it with the profiled likelihood ratio limit.
    // PLEASE FILL THIS

    // Use profiled likelihood ration as the test statistic
    // ----------------------------------------------------

    Ex4.nMu = 0.1;
    double nScan = 11.0;
    double nMuMin = Ex4.nMu;
    double nMuMax = 0.6;
    int nCounter = 0;
    double x[(int)nScan];
    double y[(int)nScan];
    double yExp[(int)nScan];

    while (nCounter < nScan)
    {
        // Print current mu
        // ----------------
        SP::IO::println("Scanning mu = %", Ex4.nMu);

        // Generate pseudo data
        // --------------------
        TH1D* hPLR = new TH1D("Ex4_PLR", "Profile Likelihood Ratio Pseudo Data", 30000, 0, 30);
        int nToys = 1000000; // 10M Toys
        SP::IO::println("Generating pseudo data...");
        for (int i = 1; i <= nToys; ++i)
        {
            if (i % (nToys / 10) == 0)
                SP::IO::println("% / %", i, nToys);

            double n = gRandom->PoissonD(Ex4.nMu * Ex4.nSig + Ex4.nBkg);
            hPLR->Fill(Ex4.qMu(n));
        }

        // Generate pseudo data (B-only) for expected limit
        // ------------------------------------------------
        TH1D* hPLR_BOnly = new TH1D("Ex4_PLR_BOnly", "Profile Likelihood Ratio Pseudo Data", 30000, 0, 30);
        nToys = 500000; // 5M Toys
        SP::IO::println("Generating pseudo data...");
        for (int i = 1; i <= nToys; ++i)
        {
            if (i % (nToys / 10) == 0)
                SP::IO::println("% / %", i, nToys);

            double n = gRandom->PoissonD(Ex4.nBkg);
            hPLR_BOnly->Fill(Ex4.qMu(n));
        }

        // qMu(obs) and qMu(exp)
        // ---------------------
        TCanvas* c1 = new TCanvas("Ex4_LR", "Likelihood Ratio", 800, 600);
        c1->SetLogy();

        double xObs = hPLR->GetBinCenter(hPLR->FindBin(Ex4.qMu(Ex4.nData)));
        TLine* lObs = new TLine(xObs, 0., xObs, hPLR->GetMaximum());

        double xExp, q;
        q = 0.5; // for median
        hPLR_BOnly->GetQuantiles(1, &xExp, &q);
        TLine* lExp = new TLine(xExp, 0., xExp, hPLR->GetMaximum());


        SP::IO::println("xObs = %, xExp = %", xObs, xExp);

        // Calculate p-value
        // -----------------
        double pValue = hPLR->Integral(hPLR->FindBin(Ex4.qMu(Ex4.nData)), hPLR->GetNbinsX()) / hPLR->Integral();
        double pValueExp = hPLR->Integral(hPLR->FindBin(xExp), hPLR->GetNbinsX()) / hPLR->Integral();

        x[nCounter] = Ex4.nMu;
        y[nCounter] = pValue;
        yExp[nCounter] = pValueExp;

        // Summary print out
        // -----------------
        SP::IO::println("Summary: mu=%, p-value=%", Ex4.nMu, pValue);

        Ex4.nMu += (nMuMax - nMuMin) / (nScan - 1);
        nCounter++;

        // Draw the histogram (do not do re-binning before extracting the pvalue, etc.)
        // ----------------------------------------------------------------------------
        hPLR->Rebin(30);
        hPLR->SetLineColor(kBlack);
        hPLR->Draw("HIST");
        hPLR_BOnly->Rebin(30);
        hPLR_BOnly->SetLineColor(kBlue);
        //hPLR_BOnly->Draw("HIST SAME");
        lObs->SetLineColor(kRed);
        lObs->SetLineWidth(2);
        lObs->Draw("SAME");
        lExp->SetLineColor(kMagenta);
        lExp->SetLineWidth(2);
        lExp->Draw("SAME");

        // Save the image
        // --------------
        c1->SaveAs(std::string("../plots/Ex4_LR_" + std::to_string(nCounter) + ".png").c_str());

        // Clean up
        // --------
        delete hPLR;
        delete hPLR_BOnly;
        delete c1;
        delete lObs;
    }

    TGraph* g = new TGraph((int)nScan, x, y);
    TGraph* gExp = new TGraph((int)nScan, x, yExp);
    TGraph* gForEval = new TGraph((int)nScan, y, x);
    TGraph* gExpForEval = new TGraph((int)nScan, yExp, x);
    g->SetMinimum(0.0);

    // styles
    g->SetLineColor(kBlack);
    g->SetLineWidth(2);
    g->SetMarkerSize(0);
    gExp->SetLineColor(kBlue);
    gExp->SetLineWidth(2);
    gExp->SetMarkerSize(0);

    TLine* pValue_0p05 = new TLine(g->GetXaxis()->GetXmin(), 0.05, g->GetXaxis()->GetXmax(), 0.05);
    pValue_0p05->SetLineColor(kRed);
    pValue_0p05->SetLineWidth(2);

    TCanvas* c = new TCanvas("Ex4_pValue", "mu Scan", 800, 600);
    g->Draw();
    gExp->Draw("SAME");
    pValue_0p05->Draw("SAME");
    c->SaveAs("../plots/Ex4_MuScan.png");

    SP::IO::println("Upper limit = %, Expected upper limit = %", gForEval->Eval(0.05), gExpForEval->Eval(0.05));

    delete g;
    delete gExp;
    delete gForEval;
    delete gExpForEval;
    delete pValue_0p05;
    delete c;
}
