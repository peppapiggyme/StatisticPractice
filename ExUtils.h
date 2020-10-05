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


namespace SP
{
    double GetChiSquare(TH1* h, TF1* f);


}

#endif // EXUTILS_H
