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
#include "TLine.h"

#include <iostream>
#include <string>

/**
 * @brief data/MC plots, mass window cut, counting experiment, Poisson distribution,
 * Profiled likelihood ratio, p-value, significance.
 *
 * Root file took from here:
 * https://indico.cern.ch/event/292887/sessions/55614/attachments/547750/754994/2014_07_25_HASCO_IvovanVulpen.pdf
 *
 * @note: when you optimise the mass window selection.
 * Remember, always optimise the EXPECTED significance :)
 *
 * @name Ex3
 * @author Bowen Zhang
 * @date 2020/10/06
 *
 */

using namespace std;

void Exercise_3::test() const
{
    // ====
    //  ||
    //  ||
    // ====

    // Draw the 4-lepton invariant mass distribution
    // ---------------------------------------------

    // preparation
    SPEx::CountingExperiment Ex3;
    Ex3.nData = 0.0;
    Ex3.nSig  = 0.0;
    Ex3.nBkg  = 0.0;
    Ex3.nMu   = 0.0;

    int nRebin = 20;

    int HiggsMass = SP::IO::getInteger("输入Higgs信号质量: ");
    if (HiggsMass != 125 && HiggsMass != 200)
    {
        throw std::runtime_error("invalid signal Higgs mass (choose between 125 and 200)");
    }
    std::string sSignalShort = std::to_string(HiggsMass);
    std::string sSignal = "h_m4l_Higgs" + sSignalShort + "_fake";

    // The RES_PATH is set by tools/SetupEnv.sh
    // ----------------------------------------
    std::string filePath = std::string(getenv("RES_PATH")) + "Ex3.root";

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

    // print mass and number of events in a mass window around 125 GeV
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
        if (std::fabs(mass4l_i - (double)HiggsMass) < width) {
            SP::IO::println("Bin %: mass = % | Nsig = %  Nbgr = % and Ndata = %",
                            i, mass4l_i, nSig_i, nBkg_i, nData_i);
            // Count the number of events in mass window
            // PLEASE FILL THIS
            Ex3.nData += nData_i;
            Ex3.nBkg  += nBkg_i;
            Ex3.nSig  += nSig_i;
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

    c->SaveAs(std::string("../plots/Ex3_m4l_H" + sSignalShort + ".png").c_str());

    delete c;
    delete l;


    // =======
    //  || ||
    //  || ||
    // =======

    // H0: N ~ Pois(mu=0)
    // H1: N ~ Pois(mu>0)
    // ------------------

    // Use the number of events as test statistic
    // p-value, significance
    // ------------------------------------------
    SP::IO::println("Observed Pois(lambda=%, N=%) = %, p-value = %, Z = %",
                    Ex3.nBkg, Ex3.nData,
                    TMath::Poisson(Ex3.nData, Ex3.nBkg),
                    ROOT::Math::poisson_cdf_c(Ex3.nData, Ex3.nBkg),
                    SPEx::GetSignificance(ROOT::Math::poisson_cdf_c(Ex3.nData, Ex3.nBkg)));

    // The test statistic as a function of n
    // -------------------------------------
    // DEFINED IN ExUtils.h

    // Pseudo data generation
    // ----------------------
    TH1D* hPLR = new TH1D("Ex3_PLR", "Profile Likelihood Ratio Pseudo Data", 30000, 0, 30);
    int nToys = 10000000; // 10M Toys
    SP::IO::println("Generating pseudo data...");
    for (int i = 1; i <= nToys; ++i)
    {
        if (i % (nToys / 10) == 0)
            SP::IO::println("% / %", i, nToys);
        // Those lines do the job
        // PLEASE FILL THIS
        double n = gRandom->PoissonD(Ex3.nBkg);
        hPLR->Fill(Ex3.q0(n));
    }

    // Define a vertical line at q0(obs)
    // ---------------------------------
    double xObs = hPLR->GetBinCenter(hPLR->FindBin(Ex3.q0(Ex3.nData)));
    SP::IO::println("xObs = %", xObs);
    TLine* lObs = new TLine(xObs, 0., xObs, hPLR->GetMaximum());
    lObs->SetLineColor(kRed);
    lObs->SetLineWidth(2);

    // Think about how to calculate the p-value and significance
    // ---------------------------------------------------------
    // PLEASE FILL THIS
    double pValue = hPLR->Integral(hPLR->FindBin(Ex3.q0(Ex3.nData)), hPLR->GetNbinsX()) / hPLR->Integral();
    double Z = SPEx::GetSignificance(pValue);

    SP::IO::println("Observed LR(lambda=%, N=%) = %, p-value = %, Z = %",
                    Ex3.nBkg, Ex3.nData, hPLR->GetBinContent(hPLR->FindBin(Ex3.nData)), pValue, Z);

    // Expected significance?
    // PLEASE FILL THIS


    // Draw the likelihood ratio distribution
    // --------------------------------------
    // PLEASE FILL THIS
    TCanvas* c1 = new TCanvas("Ex3_LR", "Likelihood Ratio", 800, 600);
    c1->SetLogy();

    hPLR->Rebin(300);
    hPLR->Draw("HIST");
    lObs->Draw("SAME");

    // Save the image
    // --------------
    c1->SaveAs(std::string("../plots/Ex3_LR_H" + sSignalShort + ".png").c_str());

    delete c1;
    delete hPLR;
}
