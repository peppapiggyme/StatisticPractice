#include "Exercise_6.h"
#include "ExUtils.h"
#include "Utils.h"

#include <iostream>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "RooStats/FrequentistCalculator.h"
#include "RooStats/ToyMCSampler.h"
#include "RooStats/ProfileLikelihoodTestStat.h"
#include "RooStats/HypoTestInverter.h"
#include "RooStats/HypoTestInverterPlot.h"
#include "RooStats/HistFactory/Measurement.h"
#include "RooStats/HistFactory/MakeModelAndMeasurementsFast.h"

/**
 * @brief Example of using Hypothesis test in RooStats
 *
 *
 * @name Ex6
 * @author Bowen Zhang
 * @date 2020/10/07
 *
 * @todo Add more comments, add plots
 */

using namespace std;
using namespace RooStats;
using namespace RooFit;
using namespace HistFactory;

void Exercise_6::test() const
{
    // ====
    //  ||
    //  ||
    // ====

    // Make workspace from histogram
    // -----------------------------

    // The histograms will be stored as input to HistFactory
    // Fill four bins with mass window, then merge into 1 bin
    // ------------------------------------------------------
    TH1D* hDataForWS = new TH1D("data", "", 4, 0, 1);
    TH1D* hBkgForWS = new TH1D("signal", "", 4, 0, 1);
    TH1D* hSigForWS = new TH1D("background", "", 4, 0, 1);

    // Counting in the mass window
    // ---------------------------

    int nRebin = 20;

    std::string sSignal = "h_m4l_Higgs200_fake";

    // The RES_PATH is set by tools/SetupEnv.sh
    // ----------------------------------------
    std::string filePath = std::string(getenv("RES_PATH")) + "Ex4.root";  // <- file from Ex4 ...

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
    int nBin = 1;
    for (int i = 1; i< hBkg->GetNbinsX(); i++) {
        mass4l_i = hData->GetBinCenter(i);
        nSig_i  = hSig->GetBinContent(i);
        nBkg_i  = hBkg->GetBinContent(i);
        nData_i = hData->GetBinContent(i);
        double width = 10.;
        if (std::fabs(mass4l_i - 200.) < width) {  // HC: mass = 200
            SP::IO::println("Bin %: mass = % | Nsig = %  Nbgr = % and Ndata = %",
                            i, mass4l_i, nSig_i, nBkg_i, nData_i);
            // Count the number of events in mass window
            // PLEASE FILL THIS
            hDataForWS->SetBinContent(nBin, nData_i);
            hDataForWS->SetBinError(nBin, hData->GetBinError(i));
            hBkgForWS->SetBinContent(nBin, nBkg_i);
            hBkgForWS->SetBinError(nBin, hBkg->GetBinError(i));
            hSigForWS->SetBinContent(nBin, nSig_i);
            hSigForWS->SetBinError(nBin, hSig->GetBinError(i));
            nBin++;
        }
    }

    hDataForWS->Rebin(4);
    hBkgForWS->Rebin(4);
    hSigForWS->Rebin(4);

    char sFileName[18] = "../plots/Ex6.root";
    f = new TFile(sFileName, "recreate");  // <- all outputs should locate in ../plots/
    f->cd();

    hDataForWS->Write();
    hBkgForWS->Write();
    hSigForWS->Write();

    f->Close();
    f = nullptr;

    // =======
    //  || ||
    //  || ||
    // =======

    // HistFactory part
    // ----------------

    // Define a measurement
    // --------------------
    Measurement meas("meas", "meas");

    meas.SetOutputFilePrefix("../plots/Ex6_meas");
    meas.SetPOI("mu");
    meas.SetBinHigh(1);
    meas.SetLumi(1);
    meas.SetExportOnly(false);

    // Define channels
    // ---------------
    Channel chan("ch1");
    chan.SetData("data", sFileName);

    // Define samples
    // --------------
    Sample signal("signal", "signal", sFileName);
    // Define signal strength (a normalisation factor on signal)
    signal.AddNormFactor("mu", 0., 0., 1.);
    // signal.SetNormalizeByTheory(false);
    // signal.SetStatError(StatError());  // <- active is false by default
    chan.AddSample(signal);

    // Define background
    Sample background("background", "background", sFileName);
    // background.SetNormalizeByTheory(false);
    // background.SetStatError(StatError());
    chan.AddSample(background);

    // Put information together
    // ------------------------
    meas.AddChannel(chan);
    meas.CollectHistograms();
    meas.PrintTree();

    MakeModelAndMeasurementFast(meas);

    // ==========
    //  || || ||
    //  || || ||
    // ==========

    // HypoInverter part
    // -----------------

    // Read the workspace generated by HistFactory
    // -------------------------------------------
    f = new TFile("../plots/Ex6_meas_combined_meas_model.root");
    RooWorkspace* ws = static_cast<RooWorkspace*>(f->Get("combined"));

    // Data
    // ----
    RooAbsData *data = ws->data("obsData");

    if (!data)
        throw std::runtime_error("No data!");

    // S+B model
    // ---------
    ModelConfig *sbModel = (ModelConfig*)ws->obj("ModelConfig");
    // no systematic uncertainties
    const RooArgSet *nuisPar = sbModel->GetNuisanceParameters();
    if (nuisPar && nuisPar->getSize() > 0)
    {
        SP::IO::println("Switching off nuisance parameters by setting them constant to their default values");
        SetAllConstant(*nuisPar);
    }

    // In this case, B model is S+B model with mu = 0
    // ----------------------------------------------
    ModelConfig *bModel = (ModelConfig*)sbModel->Clone();
    bModel->SetName(TString(sbModel->GetName()) + TString("_with_poi_0"));
    RooRealVar *var = dynamic_cast<RooRealVar *>(bModel->GetParametersOfInterest()->first());
    if (!var)
        throw std::logic_error("There must be at least 1 POI in the model!");
    /// @todo: Need to understand why SetSnapShot
    double oldval = var->getVal();
    var->setVal(0);
    bModel->SetSnapshot(RooArgSet(*var));
    var->setVal(oldval);

    // Define calculator for hypothesis test
    // Here we use FrequentistCalculator
    // One can choose from AsymptoticCalculator, HybridCalculator
    // ----------------------------------------------------------
    FrequentistCalculator fc(*data, *bModel, *sbModel);

    // Define test statistic
    // Use toy MC to generate the distribution of profiled likelihood ratio test statistic
    // -----------------------------------------------------------------------------------
    ToyMCSampler *toymcs = (ToyMCSampler*)fc.GetTestStatSampler();

    ProfileLikelihoodTestStat profll(*sbModel->GetPdf());

    profll.SetOneSided(true);
    toymcs->SetTestStatistic(&profll);
    toymcs->SetNToys(1000);

    // Define hypothesis test inverter (for upper limits)
    // --------------------------------------------------
    HypoTestInverter calc(fc);
    calc.UseCLs(false);  // use CLs by default, here just to compare with Ex4

    // Configure the POI scanning
    // --------------------------
    int npoints = 10;
    double poimin = 0.0;
    double poimax = 1.0;

    calc.SetFixedScan(npoints, poimin, poimax);

    // Calculate interval (upper limits)
    // ---------------------------------
    HypoTestInverterResult *r = calc.GetInterval();

    // Extract results
    // ---------------
    double upperLimit = r->UpperLimit();
    double expectedLimit = r->GetExpectedUpperLimit(0);

    SP::IO::println("Observed / Expected upper limits: % / %", upperLimit, expectedLimit);

    HypoTestInverterPlot *plot = new HypoTestInverterPlot("hi", "", r);

    TCanvas *c = new TCanvas("Ex6_hypoInv", "", 800, 600);
    plot->Draw();
    c->SaveAs("../plots/Ex6_hypo.png");

    f->Close();
    f = nullptr;
    delete plot;
    delete c;
}
