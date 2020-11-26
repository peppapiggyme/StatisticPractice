#include "Exercise_3.h"
#include "ExUtils.h"
#include "Utils.h"

// ROOT include(s)
#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TFitResult.h"
#include "TStyle.h"

// std include(s)
#include <iostream>

using namespace std;
using namespace SP;

#define PRINT_NDF(res, str) \
    IO::println("Number of degree of freedom for % chi2 distribution is % +/- %", \
                str, res->Parameter(1), res->Error(1));

void Exercise_3::test() const
{
    int nbin = 100;
    int xmin = 0;
    int xmax = 10;
    
    // Toy实验的数量
    int nexp = 1000;
    // 每个实验的事例数
    int n = 200;

    // 用下面的PDF抽样填入这个直方图 
    auto hist = new TH1D("Ex3_hist", "Data Sample distribution", nbin, xmin, xmax);
    // chi2 值的分布
    auto hChi2N = new TH1D("Ex3_hChi2N", "Neyman chi-squared distribution", nbin, 0, 2*nbin);
    auto hChi2P = new TH1D("Ex3_hChi2P", "Pearson chi-squared distribution", nbin, 0, 2*nbin);
    auto hChi2L = new TH1D("Ex3_hChi2L", "Baker-Cousins chi-square distribution", nbin, 0, 2*nbin);
    // 拟合参数的分布
    auto hTauN = new TH1D("Ex3_hTauN","Distribution of fitted parameter tau for Neyman Chi-squared fits", 50, 0, 10);
    auto hTauP = new TH1D("Ex3_hTauP","Distribution of fitted parameter tau for Pearson Chi-squared fits", 50, 0, 10);
    auto hTauL = new TH1D("Ex3_hTauL","Distribution of fitted parameter tau for Log-Likelihood fits", 50, 0, 10);
    // PDF
    TF1 f1("Ex3_f1", "[A]*exp(-x/[tau])", xmin, xmax);
    double trueParams[] = {1, 5}; // 真实值
    double initParams[] = {1, 1}; // 初始值 -> 每次拟合之前设回初始值

    // 产生nexp个Toy实验
    hChi2N->Reset();
    hChi2P->Reset();
    hChi2L->Reset();
    hTauN->Reset();
    hTauP->Reset();
    hTauL->Reset();

    for (int iexp = 0; iexp < nexp; ++iexp) 
    {
        f1.SetParameters(trueParams);  // 设成真实值用来产生数据
        // 清空直方图
        hist->Reset();
        for (int i = 0; i < n; ++i) 
        {
            hist->Fill(f1.GetRandom());
        }

        // Fit option 说明：
        // S -> return result pointer
        // Q -> quiet print outs
        // P -> Pearson chi2 fit
        // L -> Maximum Likelihood fit
        // + -> Add the new fitted funtion to the list of fitted functions

        // Neyman chi2 拟合
        f1.SetParameters(initParams);
        f1.SetLineColor(kRed);
        f1.SetTitle("Neyman Fit");
        auto rN = hist->Fit(&f1, "S Q"); 

        // Pearson chi2 拟合
        f1.SetParameters(initParams);
        f1.SetLineColor(kBlack);
        f1.SetTitle("Pearson Fit");
        auto rP = hist->Fit(&f1, "S P Q +");

        // Baker-Cousins log-likelihood 拟合
        f1.SetParameters(initParams);
        f1.SetLineColor(kBlue);
        f1.SetTitle("Likelihood Fit");
        auto rL = hist->Fit(&f1, "S Q L +");

        // 从拟合结果得到需要的统计量并填入相应的直方图
        hChi2N->Fill(rN->Chi2());
        hChi2P->Fill(rP->Chi2());
        hChi2L->Fill(2.*rL->MinFcnValue());
        
        hTauN->Fill(rN->Parameter(1));
        hTauP->Fill(rP->Parameter(1));
        hTauL->Fill(rL->Parameter(1));
    }
    // 画出最后一个Toy的数据分布和拟合函数
    auto c1 = new TCanvas("Ex3_c1", "", 800, 600);
    hist->Draw();
    auto legend = new TLegend(0.6, 0.6, 0.88, 0.88);
    auto functions = hist->GetListOfFunctions();
    for (auto f : *functions)
    { 
        if (f->IsA() == TF1::Class())
            legend->AddEntry(f, f->GetTitle(), "L");
    }

    legend->Draw();
    c1->SaveAs("../plots/Ex3_fits.png");

    // 画出三种方法的chi2或2x最小NLL的分布
    auto c2 = new TCanvas("Ex3_c2", "", 1000, 400);
    c2->Divide(3, 1);
    c2->cd(1); hChi2N->Draw();
    c2->cd(2); hChi2P->Draw();
    c2->cd(3); hChi2L->Draw();
    
    // 分别拟合出拟合自由度
    gStyle->SetOptFit(1111);
    auto fChi2 = new TF1("Ex3_fChi2", "[C] * ROOT::Math::chisquared_pdf(x, [ndf])", 0, 100);

    fChi2->SetParameters(hChi2N->GetEntries() * hChi2N->GetBinWidth(1), hChi2N->GetMean());
    auto rChi2N = hChi2N->Fit(fChi2, "LS");

    fChi2->SetParameters(hChi2P->GetEntries() * hChi2P->GetBinWidth(1), hChi2P->GetMean());
    auto rChi2P = hChi2P->Fit(fChi2, "LS");

    fChi2->SetParameters(hChi2L->GetEntries() * hChi2L->GetBinWidth(1), hChi2L->GetMean());
    auto rChi2L = hChi2L->Fit(fChi2, "LS");

    c2->SaveAs("../plots/Ex3_chi2dist.png");

    PRINT_NDF(rChi2N, "Neyman");
    PRINT_NDF(rChi2P, "Pearson");
    PRINT_NDF(rChi2L, "Baker-Cousins");

    // 顺便看一下拟合值的分布
    c2->cd(1); hTauN->Draw(); hTauN->Fit("gaus", "L");
    c2->cd(2); hTauP->Draw(); hTauP->Fit("gaus", "L");
    c2->cd(3); hTauL->Draw(); hTauL->Fit("gaus", "L");
    c2->SaveAs("../plots/Ex3_taus.png");
    
    // clear up
    delete hist;
    delete hChi2N;
    delete hChi2P;
    delete hChi2L;
    delete hTauN;
    delete hTauP;
    delete hTauL;
    delete c1;
    delete c2;
}
