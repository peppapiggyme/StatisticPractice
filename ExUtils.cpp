#include "ExUtils.h"
#include "Utils.h"

#include "TMath.h"

double SPEx::GetChiSquare(TH1* h, TF1* f, bool bNeyman)
{
    double x2_sum = 0.0;
    int n_zeros = 0;
    int n_bins = h->GetNbinsX();
    for (int i = 1; i < n_bins + 1; ++i)
    {
        double x2_i = 0.0;
        double obs_i = h->GetBinContent(i);
        double exp_i = f->Eval(h->GetXaxis()->GetBinCenter(i));
        if (obs_i > 1e-12)  // is not zero
            if (bNeyman)
                x2_i = (obs_i - exp_i) * (obs_i - exp_i) / std::fabs(obs_i);
            else
                x2_i = (obs_i - exp_i) * (obs_i - exp_i) / std::fabs(exp_i);
        else
        { 
            x2_i += 0;
            n_zeros++;
        }
        
        x2_sum += x2_i;
    }
    if (n_zeros)
        SP::IO::println("WARN: % / % bins with n_obs = 0", n_zeros, n_bins);

    return x2_sum;
}

double SPEx::GetSignificance(double pvalue)
{
    return sqrt(2.0) * TMath::ErfInverse(1.0 - 2.0 * pvalue);
}

double SPEx::GetPvalue(double z)
{
    return 0.5 * (1.0 - TMath::Erf(z / sqrt(2.0)));
}
