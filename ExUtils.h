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
}

#endif // EXUTILS_H
