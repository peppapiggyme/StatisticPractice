#include "Exercise_2.h"
#include "ExUtils.h"
#include "Utils.h"

#include <iostream>
#include <string>

#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

using namespace std;

void Exercise_2::test() const
{
    // The RES_PATH is set by tools/SetupEnv.sh
    // ----------------------------------------
    char* filePath = getenv("RES_PATH");
    strcat(filePath, "Ex2.root");

    // ROOT I/O
    // --------
    TFile* f = TFile::Open(filePath);

    // h is created by an exponential function
    // f(x) = p0 * exp ( p1 * x ) + p2 
    // p0, p1, p2 are parameters
    // ---------------------------------------
    TH1* h = (TH1*)f->Get("exp");
    
    // * why you didn't see the function? (*->n iteration)
    // ----------------------------------
    h->GetXaxis()->SetRangeUser(0., 200.);
    h->GetYaxis()->SetRangeUser(0., 20.);
    h->SetLineColor(1);  // 1 is kBlack
    
    // Please define the function here (range=0~200)
    // ---------------------------------------------
    TF1* func = new TF1("func_exp", "[0] * exp([1] * x) + [2]", 0., 200.);

    // Set its parameter and calculate the chi-square
    // value (observed values from histogram)
    // ----------------------------------------------
    double p[3] = {1.0, -0.1, 2.0};

    func->SetParameter(0, p[0]);
    func->SetParameter(1, p[1]);
    func->SetParameter(2, p[2]);
    
    // Draw the histogram and the function
    // -----------------------------------
    TCanvas* c = new TCanvas("c", "plots", 800, 600);
    h->Draw();
    func->Draw("same");
    c->SaveAs("../plots/Ex2_exp.png");
        
    // Calculate chi-square
    // --------------------
    double x2 = SP::GetChiSquare(h, func);
    SP::IO::println("Parameters: %, %, % -> Chi2 = %", p[0], p[1], p[2], x2);

    p[0] = 5.; p[1] = -0.05; p[2] = 3.;
    SP::IO::println("Parameters: %, %, % -> Chi2 = %", p[0], p[1], p[2], x2);

    p[0] = 10.; p[1] = -0.01; p[2] = 5.;
    SP::IO::println("Parameters: %, %, % -> Chi2 = %", p[0], p[1], p[2], x2);    
    
    // ROOT minimisation (MIGRAD)
    // --------------------------
    h->Fit(func);

    delete func;
    delete c;
    f->Close();
}
