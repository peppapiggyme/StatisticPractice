#include "AnaHyy.h"
#include "Utils.h"

#include "TLorentzVector.h"

AnaHyy::AnaHyy(const AnaSampleConfig& asc)
    : AnaHyyBase(), m_type(asc.type)
{
    m_rInFile = TFile::Open(asc.inFileName.c_str(), "READ");
    if (!m_rInFile)
        throw std::runtime_error("File not exist!");

    TTree *t = (TTree*)m_rInFile->Get("mini");
    t->SetDirectory(0);
    if (!t)
        throw std::runtime_error("Tree not exist!");

    Init(t);

    m_rOutFile = TFile::Open(asc.outFileName.c_str(), "recreate");
    m_rCutflow = new TH1D("Cutflow", "Number of events after ith selection", 10, 0., 10.);
}

AnaHyy::~AnaHyy()
{
    m_rOutFile->cd();
    m_rOutFile->Close();
    m_rInFile->cd();
    m_rInFile->Close();
}

void AnaHyy::Loop()
{
    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntries();

    for (Long64_t jentry=0; jentry<nentries;jentry++)
    {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;

        // update all values
        // -----------------
        fChain->GetEntry(jentry);

        if (jentry % 50000 == 0)
            SP::IO::println("Event % / %", jentry, nentries);

        // **********************************************************************************************//
        // Begin analysis selection, largely based on: ATLAS Collaboration, PRD 98 (2018) 052005         //
        // **********************************************************************************************//

        //Scale factors
        Float_t scaleFactor = scaleFactor_PHOTON*scaleFactor_PhotonTRIGGER*scaleFactor_PILEUP;

        //MC weight
        Float_t m_mcWeight = mcWeight;

        //Total weight
        Float_t weight = scaleFactor*m_mcWeight;

        if (m_type == eSampleType::DATA) {  weight = 1.; }

        m_rCutflow->Fill(0., weight);

        // Preselection cut for photon trigger
        if (trigP)
        {
            m_rCutflow->Fill(1., weight);
            // Preselection of good photons
            int goodphoton_index[2];
            int goodphoton_n = 0;
            int photon_index =0;

            for (unsigned int i=0; i<photon_n; i++)
            {
                // photons are tight
                if ( photon_isTightID->at(i) )
                {
                    // photons with 25 GeV and excluding the transition region between the barrel and endcap calorimeters
                    if ( photon_pt->at(i) >25000. && TMath::Abs(photon_eta->at(i))<2.37 && ( TMath::Abs(photon_eta->at(i)) < 1.37 || TMath::Abs(photon_eta->at(i)) > 1.52 ) )
                    {
                        goodphoton_n = goodphoton_n + 1;
                        goodphoton_index[photon_index] = i;
                        photon_index++;
                    }
                }
            }

            //Exactly two photons
            if (goodphoton_n==2 )
            {
                m_rCutflow->Fill(2., weight);
            }
        }
    }

    // Finalise
    m_rOutFile->cd();
    m_rCutflow->SetDirectory(m_rOutFile);
    m_rCutflow->Write();
}
