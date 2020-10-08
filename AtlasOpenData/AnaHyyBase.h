#ifndef AtlasOpenData_AnaHyyBase_H
#define AtlasOpenData_AnaHyyBase_H

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"

#include <vector>

using std::vector;

class AnaHyyBase {
public :
    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain

    // Fixed size dimensions of array or collections stored in the TTree if any.

    // Declaration of leaf types
    Int_t           runNumber = 0;
    Int_t           eventNumber = 0;
    Int_t           channelNumber = 0;
    Float_t         mcWeight = 0.0;
    Float_t         scaleFactor_PILEUP = 0.0;
    Float_t         scaleFactor_ELE = 0.0;
    Float_t         scaleFactor_MUON = 0.0;
    Float_t         scaleFactor_PHOTON = 0.0;
    Float_t         scaleFactor_TAU = 0.0;
    Float_t         scaleFactor_BTAG = 0.0;
    Float_t         scaleFactor_LepTRIGGER = 0.0;
    Float_t         scaleFactor_PhotonTRIGGER = 0.0;
    Bool_t          trigE = false;
    Bool_t          trigM = false;
    Bool_t          trigP = false;
    UInt_t          lep_n = 0;
    vector<bool>    *lep_truthMatched = nullptr;
    vector<bool>    *lep_trigMatched = nullptr;
    vector<float>   *lep_pt = nullptr;
    vector<float>   *lep_eta = nullptr;
    vector<float>   *lep_phi = nullptr;
    vector<float>   *lep_E = nullptr;
    vector<float>   *lep_z0 = nullptr;
    vector<int>     *lep_charge = nullptr;
    vector<unsigned int> *lep_type = nullptr;
    vector<bool>    *lep_isTightID = nullptr;
    vector<float>   *lep_ptcone30 = nullptr;
    vector<float>   *lep_etcone20 = nullptr;
    vector<float>   *lep_trackd0pvunbiased = nullptr;
    vector<float>   *lep_tracksigd0pvunbiased = nullptr;
    Float_t         met_et = 0.0;
    Float_t         met_phi = 0.0;
    UInt_t          jet_n = 0;
    vector<float>   *jet_pt = nullptr;
    vector<float>   *jet_eta = nullptr;
    vector<float>   *jet_phi = nullptr;
    vector<float>   *jet_E = nullptr;
    vector<float>   *jet_jvt = nullptr;
    vector<int>     *jet_trueflav = nullptr;
    vector<bool>    *jet_truthMatched = nullptr;
    vector<float>   *jet_MV2c10 = nullptr;
    UInt_t          photon_n = 0;
    vector<bool>    *photon_truthMatched = nullptr;
    vector<bool>    *photon_trigMatched = nullptr;
    vector<float>   *photon_pt = nullptr;
    vector<float>   *photon_eta = nullptr;
    vector<float>   *photon_phi = nullptr;
    vector<float>   *photon_E = nullptr;
    vector<bool>    *photon_isTightID = nullptr;
    vector<float>   *photon_ptcone30 = nullptr;
    vector<float>   *photon_etcone20 = nullptr;
    vector<int>     *photon_convType = nullptr;
    UInt_t          tau_n = 0;
    vector<float>   *tau_pt = nullptr;
    vector<float>   *tau_eta = nullptr;
    vector<float>   *tau_phi = nullptr;
    vector<float>   *tau_E = nullptr;
    vector<bool>    *tau_isTightID = nullptr;
    vector<bool>    *tau_truthMatched = nullptr;
    vector<bool>    *tau_trigMatched = nullptr;
    vector<int>     *tau_nTracks = nullptr;
    vector<float>   *tau_BDTid = nullptr;
    Float_t         ditau_m = 0.0;
    vector<float>   *lep_pt_syst = nullptr;
    Float_t         met_et_syst = 0.0;
    vector<float>   *jet_pt_syst = nullptr;
    vector<float>   *photon_pt_syst = nullptr;
    vector<float>   *tau_pt_syst = nullptr;
    Float_t         XSection = 0.0;
    Float_t         SumWeights = 0.0;
    UInt_t          largeRjet_n = 0;
    vector<float>   *largeRjet_pt = nullptr;
    vector<float>   *largeRjet_eta = nullptr;
    vector<float>   *largeRjet_phi = nullptr;
    vector<float>   *largeRjet_E = nullptr;
    vector<float>   *largeRjet_m = nullptr;
    vector<float>   *largeRjet_truthMatched = nullptr;
    vector<float>   *largeRjet_D2 = nullptr;
    vector<float>   *largeRjet_tau32 = nullptr;
    vector<float>   *largeRjet_pt_syst = nullptr;
    vector<int>     *tau_charge = nullptr;

    // List of branches
    TBranch        *b_runNumber;   //!
    TBranch        *b_eventNumber;   //!
    TBranch        *b_channelNumber;   //!
    TBranch        *b_mcWeight;   //!
    TBranch        *b_scaleFactor_PILEUP;   //!
    TBranch        *b_scaleFactor_ELE;   //!
    TBranch        *b_scaleFactor_MUON;   //!
    TBranch        *b_scaleFactor_PHOTON;   //!
    TBranch        *b_scaleFactor_TAU;   //!
    TBranch        *b_scaleFactor_BTAG;   //!
    TBranch        *b_scaleFactor_LepTRIGGER;   //!
    TBranch        *b_scaleFactor_PhotonTRIGGER;   //!
    TBranch        *b_trigE;   //!
    TBranch        *b_trigM;   //!
    TBranch        *b_trigP;   //!
    TBranch        *b_lep_n;   //!
    TBranch        *b_lep_truthMatched;   //!
    TBranch        *b_lep_trigMatched;   //!
    TBranch        *b_lep_pt;   //!
    TBranch        *b_lep_eta;   //!
    TBranch        *b_lep_phi;   //!
    TBranch        *b_lep_E;   //!
    TBranch        *b_lep_z0;   //!
    TBranch        *b_lep_charge;   //!
    TBranch        *b_lep_type;   //!
    TBranch        *b_lep_isTightID;   //!
    TBranch        *b_lep_ptcone30;   //!
    TBranch        *b_lep_etcone20;   //!
    TBranch        *b_lep_trackd0pvunbiased;   //!
    TBranch        *b_lep_tracksigd0pvunbiased;   //!
    TBranch        *b_met_et;   //!
    TBranch        *b_met_phi;   //!
    TBranch        *b_jet_n;   //!
    TBranch        *b_jet_pt;   //!
    TBranch        *b_jet_eta;   //!
    TBranch        *b_jet_phi;   //!
    TBranch        *b_jet_E;   //!
    TBranch        *b_jet_jvt;   //!
    TBranch        *b_jet_trueflav;   //!
    TBranch        *b_jet_truthMatched;   //!
    TBranch        *b_jet_MV2c10;   //!
    TBranch        *b_photon_n;   //!
    TBranch        *b_photon_truthMatched;   //!
    TBranch        *b_photon_trigMatched;   //!
    TBranch        *b_photon_pt;   //!
    TBranch        *b_photon_eta;   //!
    TBranch        *b_photon_phi;   //!
    TBranch        *b_photon_E;   //!
    TBranch        *b_photon_isTightID;   //!
    TBranch        *b_photon_ptcone30;   //!
    TBranch        *b_photon_etcone20;   //!
    TBranch        *b_photon_convType;   //!
    TBranch        *b_tau_n;   //!
    TBranch        *b_tau_pt;   //!
    TBranch        *b_tau_eta;   //!
    TBranch        *b_tau_phi;   //!
    TBranch        *b_tau_E;   //!
    TBranch        *b_tau_isTightID;   //!
    TBranch        *b_tau_truthMatched;   //!
    TBranch        *b_tau_trigMatched;   //!
    TBranch        *b_tau_nTracks;   //!
    TBranch        *b_tau_BDTid;   //!
    TBranch        *b_ditau_m;   //!
    TBranch        *b_lep_pt_syst;   //!
    TBranch        *b_met_et_syst;   //!
    TBranch        *b_jet_pt_syst;   //!
    TBranch        *b_photon_pt_syst;   //!
    TBranch        *b_tau_pt_syst;   //!
    TBranch        *b_XSection;   //!
    TBranch        *b_SumWeights;   //!
    TBranch        *b_largeRjet_n;   //!
    TBranch        *b_largeRjet_pt;   //!
    TBranch        *b_largeRjet_eta;   //!
    TBranch        *b_largeRjet_phi;   //!
    TBranch        *b_largeRjet_E;   //!
    TBranch        *b_largeRjet_m;   //!
    TBranch        *b_largeRjet_truthMatched;   //!
    TBranch        *b_largeRjet_D2;   //!
    TBranch        *b_largeRjet_tau32;   //!
    TBranch        *b_largeRjet_pt_syst;   //!
    TBranch        *b_tau_charge;   //!

    AnaHyyBase() : fChain(0) {}

    virtual ~AnaHyyBase()
    {
    }

    virtual Int_t Cut(Long64_t entry)
    {
        return entry != 0;
    }

    virtual Int_t GetEntry(Long64_t entry)
    {
        // Read contents of entry.
        if (!fChain) return 0;
        return fChain->GetEntry(entry);
    }

    virtual Long64_t LoadTree(Long64_t entry)
    {
        // Set the environment to read one entry
        if (!fChain) return -5;
        Long64_t centry = fChain->LoadTree(entry);
        if (centry < 0) return centry;
        if (fChain->GetTreeNumber() != fCurrent) {
            fCurrent = fChain->GetTreeNumber();
            Notify();
        }
        return centry;
    }

    virtual void Init(TTree *tree)
    {
        // Set branch addresses and branch pointers
        if (!tree) return;
        fChain = tree;
        fCurrent = -1;
        fChain->SetMakeClass(1);

        fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
        fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
        fChain->SetBranchAddress("channelNumber", &channelNumber, &b_channelNumber);
        fChain->SetBranchAddress("mcWeight", &mcWeight, &b_mcWeight);
        fChain->SetBranchAddress("scaleFactor_PILEUP", &scaleFactor_PILEUP, &b_scaleFactor_PILEUP);
        fChain->SetBranchAddress("scaleFactor_ELE", &scaleFactor_ELE, &b_scaleFactor_ELE);
        fChain->SetBranchAddress("scaleFactor_MUON", &scaleFactor_MUON, &b_scaleFactor_MUON);
        fChain->SetBranchAddress("scaleFactor_PHOTON", &scaleFactor_PHOTON, &b_scaleFactor_PHOTON);
        fChain->SetBranchAddress("scaleFactor_TAU", &scaleFactor_TAU, &b_scaleFactor_TAU);
        fChain->SetBranchAddress("scaleFactor_BTAG", &scaleFactor_BTAG, &b_scaleFactor_BTAG);
        fChain->SetBranchAddress("scaleFactor_LepTRIGGER", &scaleFactor_LepTRIGGER, &b_scaleFactor_LepTRIGGER);
        fChain->SetBranchAddress("scaleFactor_PhotonTRIGGER", &scaleFactor_PhotonTRIGGER, &b_scaleFactor_PhotonTRIGGER);
        fChain->SetBranchAddress("trigE", &trigE, &b_trigE);
        fChain->SetBranchAddress("trigM", &trigM, &b_trigM);
        fChain->SetBranchAddress("trigP", &trigP, &b_trigP);
        fChain->SetBranchAddress("lep_n", &lep_n, &b_lep_n);
        fChain->SetBranchAddress("lep_truthMatched", &lep_truthMatched, &b_lep_truthMatched);
        fChain->SetBranchAddress("lep_trigMatched", &lep_trigMatched, &b_lep_trigMatched);
        fChain->SetBranchAddress("lep_pt", &lep_pt, &b_lep_pt);
        fChain->SetBranchAddress("lep_eta", &lep_eta, &b_lep_eta);
        fChain->SetBranchAddress("lep_phi", &lep_phi, &b_lep_phi);
        fChain->SetBranchAddress("lep_E", &lep_E, &b_lep_E);
        fChain->SetBranchAddress("lep_z0", &lep_z0, &b_lep_z0);
        fChain->SetBranchAddress("lep_charge", &lep_charge, &b_lep_charge);
        fChain->SetBranchAddress("lep_type", &lep_type, &b_lep_type);
        fChain->SetBranchAddress("lep_isTightID", &lep_isTightID, &b_lep_isTightID);
        fChain->SetBranchAddress("lep_ptcone30", &lep_ptcone30, &b_lep_ptcone30);
        fChain->SetBranchAddress("lep_etcone20", &lep_etcone20, &b_lep_etcone20);
        fChain->SetBranchAddress("lep_trackd0pvunbiased", &lep_trackd0pvunbiased, &b_lep_trackd0pvunbiased);
        fChain->SetBranchAddress("lep_tracksigd0pvunbiased", &lep_tracksigd0pvunbiased, &b_lep_tracksigd0pvunbiased);
        fChain->SetBranchAddress("met_et", &met_et, &b_met_et);
        fChain->SetBranchAddress("met_phi", &met_phi, &b_met_phi);
        fChain->SetBranchAddress("jet_n", &jet_n, &b_jet_n);
        fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
        fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
        fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
        fChain->SetBranchAddress("jet_E", &jet_E, &b_jet_E);
        fChain->SetBranchAddress("jet_jvt", &jet_jvt, &b_jet_jvt);
        fChain->SetBranchAddress("jet_trueflav", &jet_trueflav, &b_jet_trueflav);
        fChain->SetBranchAddress("jet_truthMatched", &jet_truthMatched, &b_jet_truthMatched);
        fChain->SetBranchAddress("jet_MV2c10", &jet_MV2c10, &b_jet_MV2c10);
        fChain->SetBranchAddress("photon_n", &photon_n, &b_photon_n);
        fChain->SetBranchAddress("photon_truthMatched", &photon_truthMatched, &b_photon_truthMatched);
        fChain->SetBranchAddress("photon_trigMatched", &photon_trigMatched, &b_photon_trigMatched);
        fChain->SetBranchAddress("photon_pt", &photon_pt, &b_photon_pt);
        fChain->SetBranchAddress("photon_eta", &photon_eta, &b_photon_eta);
        fChain->SetBranchAddress("photon_phi", &photon_phi, &b_photon_phi);
        fChain->SetBranchAddress("photon_E", &photon_E, &b_photon_E);
        fChain->SetBranchAddress("photon_isTightID", &photon_isTightID, &b_photon_isTightID);
        fChain->SetBranchAddress("photon_ptcone30", &photon_ptcone30, &b_photon_ptcone30);
        fChain->SetBranchAddress("photon_etcone20", &photon_etcone20, &b_photon_etcone20);
        fChain->SetBranchAddress("photon_convType", &photon_convType, &b_photon_convType);
        fChain->SetBranchAddress("tau_n", &tau_n, &b_tau_n);
        fChain->SetBranchAddress("tau_pt", &tau_pt, &b_tau_pt);
        fChain->SetBranchAddress("tau_eta", &tau_eta, &b_tau_eta);
        fChain->SetBranchAddress("tau_phi", &tau_phi, &b_tau_phi);
        fChain->SetBranchAddress("tau_E", &tau_E, &b_tau_E);
        fChain->SetBranchAddress("tau_isTightID", &tau_isTightID, &b_tau_isTightID);
        fChain->SetBranchAddress("tau_truthMatched", &tau_truthMatched, &b_tau_truthMatched);
        fChain->SetBranchAddress("tau_trigMatched", &tau_trigMatched, &b_tau_trigMatched);
        fChain->SetBranchAddress("tau_nTracks", &tau_nTracks, &b_tau_nTracks);
        fChain->SetBranchAddress("tau_BDTid", &tau_BDTid, &b_tau_BDTid);
        fChain->SetBranchAddress("ditau_m", &ditau_m, &b_ditau_m);
        fChain->SetBranchAddress("lep_pt_syst", &lep_pt_syst, &b_lep_pt_syst);
        fChain->SetBranchAddress("met_et_syst", &met_et_syst, &b_met_et_syst);
        fChain->SetBranchAddress("jet_pt_syst", &jet_pt_syst, &b_jet_pt_syst);
        fChain->SetBranchAddress("photon_pt_syst", &photon_pt_syst, &b_photon_pt_syst);
        fChain->SetBranchAddress("tau_pt_syst", &tau_pt_syst, &b_tau_pt_syst);
        fChain->SetBranchAddress("XSection", &XSection, &b_XSection);
        fChain->SetBranchAddress("SumWeights", &SumWeights, &b_SumWeights);
        fChain->SetBranchAddress("largeRjet_n", &largeRjet_n, &b_largeRjet_n);
        fChain->SetBranchAddress("largeRjet_pt", &largeRjet_pt, &b_largeRjet_pt);
        fChain->SetBranchAddress("largeRjet_eta", &largeRjet_eta, &b_largeRjet_eta);
        fChain->SetBranchAddress("largeRjet_phi", &largeRjet_phi, &b_largeRjet_phi);
        fChain->SetBranchAddress("largeRjet_E", &largeRjet_E, &b_largeRjet_E);
        fChain->SetBranchAddress("largeRjet_m", &largeRjet_m, &b_largeRjet_m);
        fChain->SetBranchAddress("largeRjet_truthMatched", &largeRjet_truthMatched, &b_largeRjet_truthMatched);
        fChain->SetBranchAddress("largeRjet_D2", &largeRjet_D2, &b_largeRjet_D2);
        fChain->SetBranchAddress("largeRjet_tau32", &largeRjet_tau32, &b_largeRjet_tau32);
        fChain->SetBranchAddress("largeRjet_pt_syst", &largeRjet_pt_syst, &b_largeRjet_pt_syst);
        fChain->SetBranchAddress("tau_charge", &tau_charge, &b_tau_charge);
        Notify();
    }

    virtual void Loop()
    {

    }

    virtual Bool_t Notify()
    {
        return kTRUE;
    }

    virtual void Show(Long64_t entry = -1)
    {
        if (!fChain) return;
        fChain->Show(entry);
    }
};

#endif
