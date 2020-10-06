#include "Exercise_3.h"
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

#include <iostream>

/**
 * @brief A example took from here:
 * https://indico.cern.ch/event/292887/sessions/55614/attachments/547750/754994/2014_07_25_HASCO_IvovanVulpen.pdf
 *
 * @note: when you optimise the binning / window window selection.
 * Remember, always optimise the EXPECTED significance :)
 *
 * @name Ex3
 * @author Bowen Zhang
 *
 */

using namespace std;

void DrawHZZ(int nRebin = 20);

// Count the number of events in mass window
// PLEASE FILL THIS
double nData = 0.0;
double nSig  = 0.0;
double nBkg  = 0.0;
double nMu   = 1.0;

void Exercise_3::test() const
{
    // Draw the 4-lepton invariant mass distribution
    // ---------------------------------------------
    DrawHZZ();

    // Pois likelihood p-value, significance
    // -------------------------------------
    SP::IO::println("Assume B-Only:\nObserved Pois(lambda=%, N=%) = %, p-value = %, Z = %",
                    nBkg, nData,
                    TMath::Poisson(nData, nBkg),
                    ROOT::Math::poisson_cdf_c(nData, nBkg),
                    SPEx::GetSignificance(ROOT::Math::poisson_cdf_c(nData, nBkg)));

    // Likelihood ratio p-value, significance
    // Reference: https://link.springer.com/article/10.1140/epjc/s10052-011-1554-0
    // According to Neyman - Pearson lemma, it is the optimal test statistic
    // ---------------------------------------------------------------------------

    // The test statistic as a function of n
    // -------------------------------------
    auto q0 = [&](double n)
    {
        double muHat = (n - nBkg) / nSig;  // maximise L = Pois(mu*s+b | N)
        double lambda0 = muHat < 0 ? 1.0 : TMath::Poisson(n, nBkg) / TMath::Poisson(n, nBkg + muHat * nSig);
        return -2 * TMath::Log(lambda0);
    };

    // Pseudo data generation
    // ----------------------
    TH1D* hPLR = new TH1D("Ex3_PLR", "Profile Likelihood Ratio Pseudo Data", 500, 0, 50);
    int nToys = 1000000; // 1M Toys
    SP::IO::println("Generating pseudo data...");
    for (int i = 1; i <= nToys; ++i)
    {
        if (i % (nToys / 10) == 0)
            SP::IO::println("% / %", i, nToys);
        // Those lines do the job
        // PLEASE FILL THIS
        double n = gRandom->PoissonD(nBkg);
        hPLR->Fill(q0(n));
    }

    // Draw the likelihood ratio distribution
    // --------------------------------------
    // PLEASE FILL THIS
    TCanvas* c = new TCanvas("Ex3_LR", "Likelihood Ratio", 800, 600);
    c->SetLogy();
    hPLR->Draw();
    c->SaveAs("../plots/Ex3_LR.png");

    // Think about how to calculate the p-value and significance
    // ---------------------------------------------------------
    // PLEASE FILL THIS
    double pValue = hPLR->Integral(hPLR->FindBin(nData), hPLR->GetNbinsX()) / hPLR->Integral();
    double Z = SPEx::GetSignificance(pValue);

    SP::IO::println("Assume B-Only:\nObserved LR(lambda=%, N=%) = %, p-value = %, Z = %",
                    nBkg, nData, hPLR->GetBinContent(hPLR->FindBin(nData)), pValue, Z);

    /// @todo Use Roostats example to do the calculation ...
    /// which uses asymptotic appeoach to model the PLR distribution
    ///

    delete c;
    delete hPLR;
}

// Helper functions
// ----------------

void DrawHZZ(int nRebin)
{
    gROOT->SetStyle("ATLAS");
    gStyle->SetErrorX(0.5);

    // The RES_PATH is set by tools/SetupEnv.sh
    // ----------------------------------------
    std::string filePath = std::string(getenv("RES_PATH")) + "Ex3.root";

    // ROOT I/O
    // --------
    TDirectory* dir = gDirectory;
    TFile* f = new TFile(filePath.c_str(), "READ");

    // Get Histograms
    // --------------
    dir->cd();
    TH1D *hSig  = (TH1D*) f->Get("h_m4l_Higgs125_fake")->Clone("h_sig");
    TH1D *hBkg  = (TH1D*) f->Get("h_m4l_ZZ_fake")->Clone("h_bgr");
    TH1D *hData = (TH1D*) f->Get("h_m4l_data_fake")->Clone("h_fake");
    f->Close();
    f = nullptr;

    // Rebin
    // -----

    // Can you optimise the binning
    // PLEASE FILL THIS
    hSig->Rebin(nRebin);
    hBkg->Rebin(nRebin);
    hData->Rebin(nRebin);
    SP::IO::println("Rebinning the histograms with a factor %. Binwidth is now % GeV\n",
                    nRebin, hData->GetBinWidth(1));

    // Prepare S + B histogram
    // -----------------------
    TH1D *hSPlusB = (TH1D* ) hBkg->Clone("h_m4l_Higgs125_plus_ZZ_fake");
    hSPlusB->Reset();
    for (int i = 1; i < hBkg->GetNbinsX(); i++)
        hSPlusB->SetBinContent( i, hSig->GetBinContent(i) + hBkg->GetBinContent(i));

    // print mass and number of events in a 10 GeV mass window around 125 GeV
    // ----------------------------------------------------------------------
    double mass4l = 0.;
    double nSig_i = 0.;
    double nBkg_i = 0.;
    double nData_i = 0.;
    for (int i = 1; i< hBkg->GetNbinsX(); i++) {
        mass4l = hData->GetBinCenter(i);
        nSig_i  = hSig->GetBinContent(i);
        nBkg_i  = hBkg->GetBinContent(i);
        nData_i = hData->GetBinContent(i);

        // Can you optimise the mass window
        // PLEASE FILL THIS
        double width = 10.;
        if (std::fabs(mass4l - 125.) < width) {
            SP::IO::println("Bin %: mass = % | Nsig = %  Nbgr = % and Ndata = %",
                            i, mass4l, nSig_i, nBkg_i, nData_i);
            // Count the number of events in mass window
            // PLEASE FILL THIS
            nData += nData_i;
            nBkg  += nBkg_i;
            nSig  += nSig_i;
        }
    }

    // plotting
    // --------
    TCanvas * c = new TCanvas( "c_ex3", "m_{4l}", 800, 600);

    hSig->SetFillColor(7);
    hSPlusB->SetFillColor(7);
    hSPlusB->SetAxisRange(0.,25.,"Y");
    hSPlusB->SetAxisRange(50.,400.,"X");
    hSPlusB->GetXaxis()->SetTitleOffset(1.0);
    hSPlusB->GetYaxis()->SetTitleOffset(1.0);
    hSPlusB->Draw("hist");

    hBkg->SetFillColor(2);
    hBkg->Draw("same");
    hBkg->Draw("axis same");
    hData->Draw("e same");

    hSPlusB->SetXTitle("4-lepton invariant mass [GeV]");
    hSPlusB->SetYTitle(Form("Number of events / %3.1f GeV", hBkg->GetBinWidth(1)));

    TLegend *l = new TLegend(0.65, 0.65, 0.90, 0.80);
    l->SetBorderSize(0);
    l->SetFillColor(0);
    TLegendEntry *le1 = l->AddEntry(hSig," Higgs", "f");
    TLegendEntry *le2 = l->AddEntry(hBkg," ZZ(SM)", "f");
    le1->SetTextSize(0.05);
    le2->SetTextSize(0.05);
    l->Draw();

    c->SaveAs("../plots/Ex3_m4l.png");

    delete c;
    delete l;
}
