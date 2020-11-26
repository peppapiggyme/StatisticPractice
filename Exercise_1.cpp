#include "Exercise_1.h"
#include "ExUtils.h"
#include "Utils.h"

// ROOT include(s)
#include "TRandom3.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "TMath.h"
#include "Math/MinimizerOptions.h"
#include "TFitResult.h"

// std include(s)
#include <iostream>
#include <vector>

using namespace std;
using namespace SP;

void Exercise_1::test() const
{
    // 数据量 default = 20
    int n = IO::getInteger("产生事例数：");
    // 真实的参数值
    double tau = 2;
    
    // 创建一个vector (for unbinned fit)
    vector<double> x(n);

    // 产生数据
    TRandom3 r(0);
    for (int i = 0; i < n; i++)
        x[i] = r.Exp(tau);

    // 后面所有图都画到这个Canvas上并保存到上级目录的plots文件夹里
    auto c1 = new TCanvas("Ex1_c1", "", 800, 600);

    // 创建一个直方图 (for binned fit)
    auto h1 = new TH1D("Ex1_h1", "Generated Exponential data", n/tau, 0, 5*tau);
    for (auto x_i : x)
        h1->Fill(x_i);
    h1->Draw();
    c1->SaveAs("../plots/Ex1_exp.png");
    
    // 计算Negative Log-Likelihood
    auto nll = [&](double* p, double*)
    {
        double sum = 0;
        double tau = p[0];
        for (auto x_i : x)
        {
            sum += std::log(1.0/tau * std::exp(-x_i/tau));
        }
        return -sum;
    };

    // NLL 作为拟合参数的函数
    auto fnll = new TF1("nll", nll, 0, 10, 0);
    fnll->SetRange(1, 5);
    fnll->SetTitle("Negative Log-likelihood function;tau;NLL");
    fnll->Draw();
    c1->SaveAs("../plots/Ex1_nll.png");

    // 计算拟和参数误差：二阶导数
    double tau_hat = fnll->GetMinimumX();
    IO::println("Estimated value of tau = %", tau_hat);

    double h = fnll->Derivative2(tau_hat);
    double error = sqrt(1./h);
    IO::println("Parabolic error on tau = %", error);
    
    // 计算拟和参数误差: deltaL = 0.5 Rule
    double nll_min = fnll->Eval(tau_hat);
    IO::println("NLL function value at minimum is = %", nll_min);
    
    double tau_low = fnll->GetX(nll_min+0.5, 1, tau_hat);
    double tau_high = fnll->GetX(nll_min+0.5, tau_hat, 10);

    IO::println("The interval for tau is : [ %, % ]", tau_low, tau_high);
    double err_low = tau_hat - tau_low;
    double err_high = tau_high - tau_hat;
    IO::println("Best estimation of tau is : % - % + %", tau_hat, err_low, err_high);

    // 画出第二种方法的拟合值以及误差
    fnll->SetRange(tau_hat - 2.*err_low, tau_hat + 2.*err_high);
    fnll->SetMinimum(nll_min);
    fnll->Draw();
    
    auto l1 = new TLine(tau_low, nll_min, tau_low, nll_min+0.5);
    auto l2 = new TLine(tau_high, nll_min, tau_high, nll_min+0.5);
    l1->SetLineColor(kBlue);
    l2->SetLineColor(kBlue);
    l1->SetLineWidth(3); 
    l2->SetLineWidth(3);
    l1->Draw("same");
    l2->Draw("same");

    c1->SaveAs("../plots/Ex1_nllmin.png");

    // Binned
    auto f1 = new TF1("Ex1_f1", "[A]* exp (-x/[tau])");
    f1->SetParameters(1, 1);
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
    auto fitResult = h1->Fit(f1, "L S");  // L -> likelihood fit, S -> return result
    fitResult->Print();
    gStyle->SetOptFit(111);
    h1->Draw("hist");
    f1->Draw("same");

    c1->SaveAs("../plots/Ex1_binned.png");

    // clear up
    delete h1;
    delete fnll;
    delete l1;
    delete l2;
    delete f1;
    delete c1;
}
