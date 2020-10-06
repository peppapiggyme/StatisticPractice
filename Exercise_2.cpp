#include "Exercise_2.h"
#include "ExUtils.h"
#include "Utils.h"

#include <iostream>
#include <string>

#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"

/**
 * @brief
 *
 * @name Ex2
 * @author Bowen Zhang
 *
 */

using namespace std;

void Exercise_2::test() const
{
    // The RES_PATH is set by tools/SetupEnv.sh
    // ----------------------------------------
    std::string filePath = std::string(getenv("RES_PATH")) + "Ex2.root";
    // std::string filePath = std::string(getenv("RES_PATH")) + "Ex2_1k.root";

    // ROOT I/O
    // --------
    TFile* f = TFile::Open(filePath.c_str(), "READ");

    // h is created by an exponential function
    // f(x) = p0 -> a constant function
    // task: estimate parameter p0
    // ---------------------------------------
    TH1* h = (TH1*)f->Get("flat");
    
    // Set range of axises
    // -------------------
    h->GetXaxis()->SetRangeUser(0., 100.);
    h->GetYaxis()->SetRangeUser(0., 20.);
    h->SetLineColor(1);  // 1 is kBlack
    
    // Define the function (range=0~100)
    // ---------------------------------------------
    // PLESE FILL THIS
    TF1* func = new TF1("func_exp", "[0]", 0., 100.);

    // Set its parameter and calculate the chi-square
    // value (observed values from histogram)
    // ----------------------------------------------
    double p = 10;

    func->SetParameter(0, p);
    
    // Draw the histogram and the function
    // -----------------------------------
    TCanvas* c = new TCanvas("c_ex2", "Constant function", 800, 600);
    h->Draw();
    func->Draw("same");
    c->SaveAs("../plots/Ex2_flat.png");
        
    // Calculate chi-square
    // --------------------
    double x2 = SPEx::GetChiSquare(h, func);
    SP::IO::println("p0 = %, Chi2 = %", p, x2);

    // here use ROOT style Chi square
    // ------------------------------

    // Minimize chi-square
    // -------------------
    // Ideas?

    //  2          (Obs_i - Exp_i)^2
    // X  = Sum of -----------------
    //               sigma(Obs_i)^2

    // 1st-order derivative

    //   d x2
    // ------- = -2 * ( N - a * SumOf ( 1 / y_i ) )
    //   d  a

    // 2nd-order derivative

    //  d2 x2
    // ------- = 2 * SumOf ( 1 / y_i ) > 0
    //  d2  a

    // We can do it analytically :)
    // ----------------------------

    // ROOT style chi square
    auto run_chi2_root = [&]()
    {
        double SumOfInvYi = 0.0;

        for (int i = 1; i < h->GetNbinsX() + 1; ++i)
        {
            SumOfInvYi += 1.0 / h->GetBinContent(i);
        }


        p = (double)h->GetNbinsX() / SumOfInvYi;
        func->SetParameter(0, p);

        SP::IO::println("After x2 fit: (ROOT style)\np0 = %, Chi2 = %", p, SPEx::GetChiSquare(h, func));
    };

    // actual chi square
    auto run_chi2_real = [&]()
    {
        double SumOfYi2 = 0.0;  // this is used for the actual Chi square definition
        for (int i = 1; i < h->GetNbinsX() + 1; ++i)
        {
            SumOfYi2 += h->GetBinContent(i) * h->GetBinContent(i);
        }

        p = sqrt(SumOfYi2 / (double)h->GetNbinsX());
        func->SetParameter(0, p);

        SP::IO::println("After x2 fit: (Real definition)\np0 = %, Chi2 = %", p, SPEx::GetChiSquare(h, func, false));
    };


    // ROOT numerical minimisation
    // ---------------------------
    auto run_root_fit = [&]()
    {
        h->Fit(func);
    };

    // Check durations
    // ---------------
    auto dTime1 = SP::STL::FunctionRunTime(run_chi2_root);
    auto dTime2 = SP::STL::FunctionRunTime(run_chi2_real);
    auto dTime3 = SP::STL::FunctionRunTime(run_root_fit);

    SP::IO::println("\nDurations:\n - chi2root: %ms\n - chi2real: %ms\n - rootfit: %ms",
                    dTime1, dTime2, dTime3);

    // 练习：画出卡方关于参数变化的函数，使用卡方的准确定义
    // PLEASE FILL THIS

    delete func;
    delete c;
    f->Close();
}
