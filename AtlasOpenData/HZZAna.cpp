#include "AtlasOpenData/HZZAna.h"
#include "Utils.h"

#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TMath.h>
#include <TLorentzVector.h>

using namespace std;
using namespace SP;

// help to make the code in Loop() more readable
#define APPLY_CUT(criteria) \
    if (!(criteria)) continue;

// In priciple this should be lying in HZZAnaBase.cpp but
// I think it's fine.
unordered_map<string, float> HZZAnaBase::mapLumiWeights{};

// the only input is the "Config" that we defined
HZZAna::HZZAna(const HZZAnaConfig& cConfig)
    : HZZAnaBase(), m_eType(cConfig.type), m_cConfig(cConfig)
{
    m_cInput = TFile::Open(cConfig.inputPath.c_str(), "read");
    m_cOutput = TFile::Open(cConfig.outputPath.c_str(), "recreate");
    auto tree = (TTree*)m_cInput->Get("mini");
    Init(tree);
}

// file closing
HZZAna::~HZZAna()
{
    m_cInput->Close();
    m_cOutput->Close();
}

// main logic is here
// selections, histogram filling, etc
void HZZAna::Loop()
{
    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntries();
    
    // Here you book histograms, etc
    TH1D* h1_InvMass4l = new TH1D("m4l", "", 400, 0, 400);
    h1_InvMass4l->SetDirectory(m_cOutput);

    // 
    // Loop starts here
    //
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        // update all values
        // -----------------
        fChain->GetEntry(jentry);

        // for debugging 
        // if (jentry % 10000 == 0)
        //     IO::println("Event %", jentry);

        // // early stop (for testing)
        // if (jentry > 10)
        //     break;

        // Scale factors
        Float_t fSF = scaleFactor_ELE * scaleFactor_MUON * scaleFactor_LepTRIGGER * scaleFactor_PILEUP;

        string process = m_cConfig.process;
      
        // Lumi weight
        Float_t fLumiWeight = HZZAnaBase::mapLumiWeights[process];
        
        // MC weight
        Float_t fMCWeight = mcWeight;
      
        // read input option
        // ??? set -1 or 1 for stop
        if (m_eType == eSampleType::BKG && process.find("single") != string::npos) 
        {
            fMCWeight = (fMCWeight / TMath::Abs(fMCWeight));
        } 
        
        // IO::println("mc %, sf %, lumi %", fMCWeight, fSF, fLumiWeight);
        
        //Total weight
        Float_t weight = fSF * fMCWeight * fLumiWeight;

        // Make difference between data and MC
        if (m_eType == eSampleType::DATA) { weight = 1.; }
        
        // Cut 1 : electron or muon triggers
        APPLY_CUT(trigE || trigM);

        // Preselection of good leptons
        int goodlep_index[lep_n];
        int goodlep_n = 0;
        int lep_index = 0;

        // Preselection of good leptons
        for (unsigned int i = 0; i < lep_n; i++)
        {
            TLorentzVector leptemp;  
            leptemp.SetPtEtaPhiE(lep_pt->at(i)/1000., lep_eta->at(i), lep_phi->at(i), lep_E->at(i)/1000.);
                      
            // loosely isolated and very soft 
            if (lep_pt->at(i) > 5000. 
                && TMath::Abs(lep_eta->at(i)) < 2.5 
                && (lep_ptcone30->at(i) / lep_pt->at(i)) < 0.3
                && (lep_etcone20->at(i) / lep_pt->at(i)) < 0.3) 
            {
                // electron
                if (lep_type->at(i) == 11 
                    && lep_pt->at(i) > 7000. 
                    && TMath::Abs(lep_eta->at(i)) < 2.47 ) 
                {
                    // ??? track selection
                    if (TMath::Abs(lep_trackd0pvunbiased->at(i)) / lep_tracksigd0pvunbiased->at(i) < 5 
                        && TMath::Abs(lep_z0->at(i) * TMath::Sin(leptemp.Theta())) < 0.5) 
                    {
                        goodlep_n = goodlep_n + 1;
                        goodlep_index[lep_index] = i;
                        lep_index++;
                    }
                }
                //muon
                if ( lep_type->at(i) == 13) 
                {
                    if (TMath::Abs(lep_trackd0pvunbiased->at(i)) / lep_tracksigd0pvunbiased->at(i) < 3 
                       && TMath::Abs(lep_z0->at(i) * TMath::Sin(leptemp.Theta())) < 0.5) 
                    {
                        goodlep_n = goodlep_n + 1;
                        goodlep_index[lep_index] = i;
                        lep_index++;
                    }
                }
            }
        }
        
        // Cut 2: four good leptons
        APPLY_CUT(goodlep_n == 4);

        int goodlep1_index = goodlep_index[0];
        int goodlep2_index = goodlep_index[1];
        int goodlep3_index = goodlep_index[2];
        int goodlep4_index = goodlep_index[3];

        // Cut 3: first lepton pT > 25 GeV, second > 15 GeV and third > 10 GeV
        APPLY_CUT(lep_pt->at(goodlep1_index) > 25000. 
                  && lep_pt->at(goodlep2_index) > 15000. 
                  && lep_pt->at(goodlep3_index) > 10000.);

        TLorentzVector Lepton_1  = TLorentzVector();
        TLorentzVector Lepton_2  = TLorentzVector();
        TLorentzVector Lepton_3  = TLorentzVector();
        TLorentzVector Lepton_4  = TLorentzVector();

        Lepton_1.SetPtEtaPhiE(lep_pt->at(goodlep1_index), 
                              lep_eta->at(goodlep1_index), 
                              lep_phi->at(goodlep1_index),
                              lep_E->at(goodlep1_index));
        Lepton_2.SetPtEtaPhiE(lep_pt->at(goodlep2_index), 
                              lep_eta->at(goodlep2_index), 
                              lep_phi->at(goodlep2_index),
                              lep_E->at(goodlep2_index));
        Lepton_3.SetPtEtaPhiE(lep_pt->at(goodlep3_index), 
                              lep_eta->at(goodlep3_index), 
                              lep_phi->at(goodlep3_index),
                              lep_E->at(goodlep3_index));
        Lepton_4.SetPtEtaPhiE(lep_pt->at(goodlep4_index), 
                              lep_eta->at(goodlep4_index), 
                              lep_phi->at(goodlep4_index),
                              lep_E->at(goodlep4_index));
        
        // minimisation of difference from the Z mass
        float delta_Z1=0; 
        float delta_Z2=0; 
        float InvMassZ1=0; 
        float InvMassZ2=0;
        float delta_Z1_1=0; float delta_Z1_2=0; float delta_Z1_3=0;
        float delta_Z2_1=0; float delta_Z2_2=0; float delta_Z2_3=0;
        float InvMassZ1_1=0; float InvMassZ1_2=0; float InvMassZ1_3=0;
        float InvMassZ2_1=0; float InvMassZ2_2=0; float InvMassZ2_3=0;
        float sum_ZZ1=0; float sum_ZZ2=0; float sum_ZZ3=0;
          
        // final values
        float InvMassZ1_min=0; float InvMassZ2_min=0; float sum_ZZ_fin=0;

        float sum_charges = lep_charge->at(goodlep1_index) + lep_charge->at(goodlep2_index) + 
            lep_charge->at(goodlep3_index) + lep_charge->at(goodlep4_index);

        // sum of types -> eeee = 44, eemm = 48, mmmm = 52
        int sum_types = lep_type->at(goodlep1_index) + lep_type->at(goodlep2_index) + 
            lep_type->at(goodlep3_index) + lep_type->at(goodlep4_index);

        // Cut 4: Sum of charge = 0 and Reasonable sum of types (Same flavour opposite sign, SFOS)
        APPLY_CUT(sum_charges == 0 && (sum_types == 44 || sum_types == 52 || sum_types == 48));
        
        // begin case e+e-e+e- or mu+mu-mu+mu-
        if (sum_types == 44 || sum_types == 52)
        {
            if (lep_type->at(goodlep1_index) == lep_type->at(goodlep2_index) 
                && ((lep_charge->at(goodlep1_index) * lep_charge->at(goodlep2_index)) < 0))
            {
                InvMassZ1_1 = (Lepton_1 + Lepton_2).Mag() / 1000.;
                InvMassZ2_1 = (Lepton_3 + Lepton_4).Mag() / 1000.;
                delta_Z1_1 =  TMath::Abs(InvMassZ1_1 - 91.18); 
                delta_Z2_1 =  TMath::Abs(InvMassZ2_1 - 91.18);
            }
            if (lep_type->at(goodlep1_index) == lep_type->at(goodlep3_index)  
                && ((lep_charge->at(goodlep1_index) * lep_charge->at(goodlep3_index)) < 0))
            {
                InvMassZ1_2 = (Lepton_1 + Lepton_3).Mag() / 1000.;
                InvMassZ2_2 = (Lepton_2 + Lepton_4).Mag() / 1000.;
                delta_Z1_2 =  TMath::Abs(InvMassZ1_2 - 91.18); 
                delta_Z2_2 =  TMath::Abs(InvMassZ2_2 - 91.18);
            }
            if (lep_type->at(goodlep1_index) == lep_type->at(goodlep4_index)  
                 && ((lep_charge->at(goodlep1_index) * lep_charge->at(goodlep4_index)) < 0))
            {
                InvMassZ1_3 = (Lepton_1 + Lepton_4).Mag() / 1000.;
                InvMassZ2_3 = (Lepton_2 + Lepton_3).Mag() / 1000.;
                delta_Z1_3 =  TMath::Abs(InvMassZ1_3 - 91.18); 
                delta_Z2_3 =  TMath::Abs(InvMassZ2_3 - 91.18);
            }
            
            // ??? overwrite ???
            if(delta_Z1_1 < delta_Z2_1) { InvMassZ1_min = InvMassZ1_1; InvMassZ2_min = InvMassZ2_1; }
            if(delta_Z2_1 < delta_Z1_1) { InvMassZ1_min = InvMassZ2_1; InvMassZ2_min = InvMassZ1_1; }

            if(delta_Z1_2 < delta_Z2_2) { InvMassZ1_min = InvMassZ1_2; InvMassZ2_min = InvMassZ2_2; }
            if(delta_Z2_2 < delta_Z1_2) { InvMassZ1_min = InvMassZ2_2; InvMassZ2_min = InvMassZ1_2; }

            if(delta_Z1_3 < delta_Z2_3) { InvMassZ1_min = InvMassZ1_3; InvMassZ2_min = InvMassZ2_3; }
            if(delta_Z2_3 < delta_Z1_3) { InvMassZ1_min = InvMassZ2_3; InvMassZ2_min = InvMassZ1_3; }
        }

        // case eemm
        if (sum_types == 48)
        {
            if (lep_type->at(goodlep1_index) == lep_type->at(goodlep2_index)  
                && ((lep_charge->at(goodlep1_index) * lep_charge->at(goodlep2_index)) < 0))
            {
                InvMassZ1 = (Lepton_1 + Lepton_2).Mag() / 1000.;
                InvMassZ2 = (Lepton_3 + Lepton_4).Mag() / 1000.;
                delta_Z1 =  TMath::Abs(InvMassZ1 - 91.18); 
                delta_Z2 =  TMath::Abs(InvMassZ2 - 91.18);
            }
            if (lep_type->at(goodlep1_index) == lep_type->at(goodlep3_index)  
                && ((lep_charge->at(goodlep1_index) * lep_charge->at(goodlep3_index)) < 0))
            {
                InvMassZ1 = (Lepton_1 + Lepton_3).Mag() / 1000.;
                InvMassZ2 = (Lepton_2 + Lepton_4).Mag() / 1000.;
                delta_Z1 =  TMath::Abs(InvMassZ1 - 91.18); 
                delta_Z2 =  TMath::Abs(InvMassZ2 - 91.18);
            }
            if (lep_type->at(goodlep1_index) == lep_type->at(goodlep4_index)  
                && ((lep_charge->at(goodlep1_index) * lep_charge->at(goodlep4_index)) < 0))
            {
                InvMassZ1 = (Lepton_1 + Lepton_4).Mag() / 1000.;
                InvMassZ2 = (Lepton_2 + Lepton_3).Mag() / 1000.;
                delta_Z1 =  TMath::Abs(InvMassZ1 - 91.18); 
                delta_Z2 =  TMath::Abs(InvMassZ2 - 91.18);
            }
                  
            if(delta_Z1 < delta_Z2) { InvMassZ1_min = InvMassZ1; InvMassZ2_min = InvMassZ2; }
            if(delta_Z2 < delta_Z1) { InvMassZ1_min = InvMassZ2; InvMassZ2_min = InvMassZ1; }
        }
        
        TLorentzVector FourLepSystem = TLorentzVector();
        FourLepSystem = Lepton_1 + Lepton_2 + Lepton_3 + Lepton_4;
        float FourLepSystem_M = FourLepSystem.M() / 1000.;
        float FourLepSystem_pt = FourLepSystem.Pt() / 1000.;
        float FourLepSystem_y = FourLepSystem.Rapidity();
        
        //Preselection of good jets
        int goodjet_n = 0;
          
        if (jet_n > 0)
        {
            for(unsigned int i = 0; i < (unsigned int)jet_n; i++)
            {
                if(jet_pt->at(i) > 30000. && TMath::Abs(jet_eta->at(i)) < 4.4)
                {
                    goodjet_n++;
                }
            }
        }
        // IO::println("m4l %", FourLepSystem_M);
        // Fill histograms
        h1_InvMass4l->Fill(FourLepSystem_M, weight);
    } // end of loop

    // Here you save histograms, etc to file
    h1_InvMass4l->Write();

    IO::println("PROCESS [%] DONE!", m_cConfig.process);
}
