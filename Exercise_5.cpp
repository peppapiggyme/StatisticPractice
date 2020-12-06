#include "Exercise_5.h"
#include "ExUtils.h"
#include "Utils.h"

#include "AtlasOpenData/HZZAna.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;
using SP::STL;
using std::chrono::seconds;

/**
 * This might be the only Exercise cpp file that you don't need to modify, 
 * This cpp file only serves as a `main` function that steers the executing of loops
 * All modification should be made in `AtlasOpenData/HZZAna.cpp`
 * Do your experiments in the `Loop()` function :)
 */ 

void Exercise_5::test() const
{
    HZZAnaBase::getLumiWeights(STL::GetFilePath("HZZ_info.txt"), 10000.0f /* pb-1 */);

    mutex m;
    
    auto ExecuteFile = [&](const HZZAnaConfig& cfg)
    {
        m.lock();
        HZZAnaBase *a = new HZZAna(cfg);
        a->Loop();
        delete a;
        m.unlock();
    };

    vector<HZZAnaConfig> vCfg;
    vector<thread> vThr;

    vCfg.emplace_back(eSampleType::DATA, STL::GetFilePath("data_A.4lep.root"), "../plots/Ex7_data_A.root");
    vCfg.emplace_back(eSampleType::DATA, STL::GetFilePath("data_B.4lep.root"), "../plots/Ex7_data_B.root");
    vCfg.emplace_back(eSampleType::DATA, STL::GetFilePath("data_C.4lep.root"), "../plots/Ex7_data_C.root");
    vCfg.emplace_back(eSampleType::DATA, STL::GetFilePath("data_D.4lep.root"), "../plots/Ex7_data_D.root");

    vCfg.emplace_back(eSampleType::SIG, STL::GetFilePath("mc_341947.ZH125_ZZ4lep.4lep.root"), "../plots/Ex7_sig_ZH.root");
    vCfg.emplace_back(eSampleType::SIG, STL::GetFilePath("mc_341964.WH125_ZZ4lep.4lep.root"), "../plots/Ex7_sig_WH.root");
    vCfg.emplace_back(eSampleType::SIG, STL::GetFilePath("mc_344235.VBFH125_ZZ4lep.4lep.root"), "../plots/Ex7_sig_VBFH.root");
    vCfg.emplace_back(eSampleType::SIG, STL::GetFilePath("mc_345060.ggH125_ZZ4lep.4lep.root"), "../plots/Ex7_sig_ggH.root");

    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_361100.Wplusenu.2lep.root"), "../plots/Ex7_bkg_Wplusenu.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_361101.Wplusmunu.2lep.root"), "../plots/Ex7_bkg_Wplusmunu.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_361102.Wplustaunu.2lep.root"), "../plots/Ex7_bkg_Wplustaunu.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_361103.Wminusenu.2lep.root"), "../plots/Ex7_bkg_Wminusenu.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_361104.Wminusmunu.2lep.root"), "../plots/Ex7_bkg_Wminusmunu.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_361105.Wminustaunu.2lep.root"), "../plots/Ex7_bkg_Wminustaunu.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_361106.Zee.4lep.root"), "../plots/Ex7_bkg_Zee.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_361107.Zmumu.4lep.root"), "../plots/Ex7_bkg_Zmumu.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_361108.Ztautau.4lep.root"), "../plots/Ex7_bkg_Ztautau.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_363356.ZqqZll.4lep.root"), "../plots/Ex7_bkg_ZqqZll.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_363358.WqqZll.2lep.root"), "../plots/Ex7_bkg_WqqZll.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_363359.WpqqWmlv.2lep.root"), "../plots/Ex7_bkg_WpqqWmlv.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_363360.WplvWmqq.2lep.root"), "../plots/Ex7_bkg_WplvWmqq.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_363489.WlvZqq.2lep.root"), "../plots/Ex7_bkg_WlvZqq.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_363490.llll.4lep.root"), "../plots/Ex7_bkg_llll.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_363491.lllv.4lep.root"), "../plots/Ex7_bkg_lllv.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_363492.llvv.4lep.root"), "../plots/Ex7_bkg_llvv.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_363493.lvvv.2lep.root"), "../plots/Ex7_bkg_lvvv.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_410000.ttbar_lep.4lep.root"), "../plots/Ex7_bkg_ttbar_lep.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_410011.single_top_tchan.4lep.root"), "../plots/Ex7_bkg_single_top_tchan.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_410012.single_antitop_tchan.4lep.root"), "../plots/Ex7_bkg_single_antitop_tchan.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_410013.single_top_wtchan.4lep.root"), "../plots/Ex7_bkg_single_top_wtchan.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_410014.single_antitop_wtchan.4lep.root"), "../plots/Ex7_bkg_single_antitop_wtchan.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_410025.single_top_schan.4lep.root"), "../plots/Ex7_bkg_single_top_schan.root");
    vCfg.emplace_back(eSampleType::BKG, STL::GetFilePath("mc_410026.single_antitop_schan.4lep.root"), "../plots/Ex7_bkg_single_antitop_schan.root");

    // multi-thread executing
    for (auto& cfg : vCfg)
        vThr.emplace_back(ExecuteFile, cfg);
    
    for_each(vThr.begin(), vThr.end(), [](thread& t){ t.join(); });

    // one-by-one executing
    // for (auto& cfg : vCfg)
    //     ExecuteFile(cfg);

    // run one sample
    // ExecuteFile(HZZAnaConfig(eSampleType::DATA, STL::GetFilePath("data_A.4lep.root"), "../plots/Ex7_data_A.root"));
}
