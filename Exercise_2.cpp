#include "Exercise_2.h"
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
#include "TGraphErrors.h" 

// std include(s)
#include <iostream>

using namespace std;

void Exercise_2::test() const
{
    // 按照均匀分布产生数据
    auto h1 = new TH1D("Ex2_h1", "A Constant Distribution", 100, 0, 100);
    
    // 数据量
    int n = 1000;
    TRandom3 r(0);
    for (int i = 0; i < n; ++i)
    {
        h1->Fill(r.Uniform(0, 100));
    }

    // 后面所有图都画到这个Canvas上并保存到上级目录的plots文件夹里
    auto c1 = new TCanvas("Ex2_c1", "", 800, 600);
    
    h1->Draw("e1");
    
    // 定义函数
    auto f1 = new TF1("Ex2_f1", "[A]");

    // 参数真实值
    double trueValue = double(n)/h1->GetNbinsX();
    
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2"); 
    TFitResultPtr result_neyman, result_pearson, result_likelihood;
    
    // Neyman chi-square
    result_neyman = h1->Fit(f1, "S");
    c1->SaveAs("../plots/Ex2_neyman.png");

    // Pearson chi-square
    result_pearson = h1->Fit(f1, "S P");
    c1->SaveAs("../plots/Ex2_pearson.png");

    // binned likelihood fit
    result_likelihood = h1->Fit(f1, "S L");
    c1->SaveAs("../plots/Ex2_likelihood.png");

    // 解析结果
    auto f_result_pearson = [&]()
    {
        double SumOfYi2 = 0.0;
        for (int i = 1; i < h1->GetNbinsX() + 1; ++i)
        {
            SumOfYi2 += h1->GetBinContent(i) * h1->GetBinContent(i);
        }
        return std::sqrt(SumOfYi2 / (double)h1->GetNbinsX());
    };

    // 
    auto g = new TGraphErrors(3);
    g->SetMarkerStyle(20);

    g->SetPoint(0, 1, result_neyman->Value(0));
    g->SetPointError(0, 0, result_neyman->Error(0));

    g->SetPoint(1, 2, result_pearson->Value(0));
    g->SetPointError(1, 0, result_pearson->Error(0));

    g->SetPoint(2, 3, result_likelihood->Value(0));
    g->SetPointError(2, 0, result_likelihood->Error(0));

    g->SetPoint(3, 4, f_result_pearson());
    g->SetPointError(3, 0, 0);

    g->Draw("A EP");

    auto line = new TLine(0.8, trueValue, 4.2, trueValue);
    line->SetLineColor(kRed);
    line->Draw("same");

    c1->SaveAs("../plots/Ex2_fitresults.png");
    
    // clear up
    delete f1;
    delete h1;
    delete g;
    delete line;
    delete c1;
}
