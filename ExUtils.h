#ifndef EXUTILS_H
#define EXUTILS_H

/**
 * @brief this file is supposed to hold the helper functions
 * for the exercise, e.g. commonly used math calculations, etc.
 *
 * @file ExUtils.h
 * @author Bowen Zhang
 * @date 2020/10/03
 */

#include "TH1.h"
#include "TF1.h"
#include "TGraph.h"


namespace SPEx
{
    /**
     * @brief Calculate chi square value using observed data from histogram
     * and expected data from function, use root style by default, can switch
     * to the actual definition.
     *
     * definition:
     *
     *  2          (Obs_i - Exp_i)^2
     * X  = Sum of -----------------
     *               sigma(Exp_i)^2
     *
     * ROOT use a modified definiton:
     *
     *  2          (Obs_i - Exp_i)^2
     * X  = Sum of -----------------
     *               sigma(Obs_i)^2  <- != 0
     *
     *
     * Pseudo code:
     * Loop over bins, calculate per bin, get sum
     * update 1: use abs(obs_i) as sigma_obs_i^2
     * update 2: treat the obs_i = 0 case
     * update 3: prints how often obs_i = 0
     *
     */
    double GetChiSquare(TH1* h, TF1* f, bool useRootStyle = true);

    /**
     * @brief Calculate significance given p-value
     *
     *             -1
     * Z = √2 x erf  (1 - 2p)
     *
     */
    double GetSignificance(double pvalue);

    /**
     * @brief Calculate p-value given significance
     *
     *     1               Z
     * p = - x [ 1 - erf (---)]
     *     2              √2
     *
     */
    double GetPvalue(double z);


    /**
     * @brief The CountingExperiment struct
     */
    struct CountingExperiment
    {
        double nData = 0.0;
        double nSig = 0.0;
        double nBkg = 0.0;
        double nMu = 0.0;

        CountingExperiment() = default;

        CountingExperiment(double nData, double nSig, double nBkg, double nMu)
            : nData(nData), nSig(nSig), nBkg(nBkg), nMu(nMu) {}

        /**
         * @brief LHC-style test statistic for discovery of a positive signal for counting
         * experiment
         *
         * muHat is the maximum likelihood estimator of mu, in this simple case,
         * it must be (n - bkg) / sig
         *
         * for a proper fit, the nData is not a single value but a Poisson distribution maximum at n_obs
         * then the likelihood fit must be performed on this set of nData.
         *
         * @ref https://link.springer.com/article/10.1140/epjc/s10052-011-1554-0
         *
         */
        template<typename T>
        double q0(T n)
        {
            double muHat = ((double)n - nBkg) / nSig;  // maximise L = Pois(mu*s+b | N)
            if (muHat < 0.0)
                return 0.;
            else
                return -2 * TMath::Log(TMath::Poisson(n, nBkg) / TMath::Poisson(n, nBkg + muHat * nSig));
        }

        /**
         * @brief LHC-style test statistic for upper limits
         *
         * muHat is the maximum likelihood estimator of mu, in this simple case,
         * it must be (n - bkg) / sig
         *
         * @ref https://link.springer.com/article/10.1140/epjc/s10052-011-1554-0
         *
         */
        template<typename T>
        double qMu(T n)
        {
            double muHat = ((double)n - nBkg) / nSig;
            if (muHat <= nMu) {
                if (muHat < 0.0) {
                    return -2 * TMath::Log(TMath::Poisson(n, nMu*nSig+nBkg) / TMath::Poisson(n, nBkg));
                } else {
                    return -2 * TMath::Log(TMath::Poisson(n, nMu*nSig+nBkg) / TMath::Poisson(n, muHat*nSig+nBkg));
                }
            } else {
                return 0.0;
            }
        }
    };

}

#endif // EXUTILS_H
