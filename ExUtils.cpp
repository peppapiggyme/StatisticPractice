#include "ExUtils.h"
#include "Utils.h"

using namespace SP;

double SP::GetChiSquare(TH1* h, TF1* f)
{
    // definition:
    //
    //  2          (Obs_i - Exp_i)^2
    // X  = Sum of -----------------
    //               sigma(Exp_i)^2

    // ROOT use a modified definiton: 
    // 
    //  2          (Obs_i - Exp_i)^2
    // X  = Sum of -----------------
    //               sigma(Obs_i)^2  <- != 0


    // Pseudo code:
    // Loop over bins, calculate per bin, get sum
    // update 1: use abs(obs_i) as sigma_obs_i^2
    // update 2: treat the obs_i = 0 case
    // update 3: prints how often obs_i = 0
    // ------------------------------------------

    double x2_sum = 0.0;
    int n_zeros = 0;
    int n_bins = h->GetNbinsX();
    for (int i = 1; i < n_bins + 1; ++i)
    {
        double x2_i = 0.0;
        double obs_i = h->GetBinContent(i);
        double exp_i = f->Eval(h->GetXaxis()->GetBinCenter(i));
        if (obs_i != 0)
            x2_i = (obs_i - exp_i) * (obs_i - exp_i) / std::fabs(obs_i);
        else
        { 
            x2_i = 0;
            n_zeros++;
        }
        
        x2_sum += x2_i;
    }
    if (n_zeros)
        SP::IO::println("WARN: % / % bins with n_obs = 0", n_zeros, n_bins);

    return x2_sum;
}

