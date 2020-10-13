#include "Exercise_7.h"
#include "ExUtils.h"
#include "Utils.h"
#include "AtlasOpenData/AnaHyy.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <vector>
#include <chrono>

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
using chrono::seconds;

void Exercise_7::test() const
{
    mutex m;

    auto ExecuteFile = [&m](const AnaSampleConfig asc)
    {
        this_thread::sleep_for(seconds(5));  // for developer (check MT is on)
        m.lock();
        AnaHyyBase *a = new AnaHyy(asc);
        a->Loop();
        delete a;
        m.unlock();
    };

    vector<AnaSampleConfig> vAsc;
    vector<thread> vThr;

    vAsc.emplace_back(eSampleType::DATA, SP::STL::GetFilePath("Ex7_data_A.GamGam.root"), "../plots/Ex7_data_A.GamGam.root");
    vAsc.emplace_back(eSampleType::DATA, SP::STL::GetFilePath("Ex7_data_B.GamGam.root"), "../plots/Ex7_data_B.GamGam.root");

    for (auto& asc : vAsc)
    {
        vThr.emplace_back(ExecuteFile, asc);
    }

    for_each(vThr.begin(), vThr.end(), [](thread& t){ t.join(); });
}
