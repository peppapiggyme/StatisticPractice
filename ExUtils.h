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
