#include "Exercise_7.h"
#include "ExUtils.h"
#include "Utils.h"
#include "AtlasOpenData/AnaHyy.h"

#include <iostream>

#include <vector>

#include "TFile.h"
#include "TTree.h"

/**
 * @brief Example use of MakeClass, Minimal TTree analysis
 *
 * @todo MC weights
 *
 * @name Ex7
 * @author Bowen Zhang
 * @date 2020/10/07
 *
 */

using namespace std;

void Exercise_7::test() const
{
    auto ExecuteFile = [](const AnaSampleConfig asc)
    {
        AnaHyyBase *a = new AnaHyy(asc);
        a->Loop();
        delete a;
    };

    vector<AnaSampleConfig> vAsc;

    vAsc.emplace_back(eSampleType::DATA, SP::STL::GetFilePath("Ex7_data_A.GamGam.root"), "../plots/Ex7_data_A.GamGam.root");
    vAsc.emplace_back(eSampleType::DATA, SP::STL::GetFilePath("Ex7_data_B.GamGam.root"), "../plots/Ex7_data_B.GamGam.root");

    for (auto& asc : vAsc)
    {
        ExecuteFile(asc);
    }

}
