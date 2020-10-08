#ifndef ANAHYY_H
#define ANAHYY_H

#include "AtlasOpenData/AnaHyyBase.h"

#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"

enum class eSampleType : uint8_t {
    OTHER = 0,
    DATA = 1,
    MC = 2,
};

class AnaSampleConfig {
public:
    AnaSampleConfig(const eSampleType& ty, const std::string& in, const std::string& out)
        : type(ty), inFileName(in), outFileName(out)
    {
    }

public:
    eSampleType type;
    std::string inFileName;
    std::string outFileName;
};

class AnaHyy : public AnaHyyBase
{
public:
    AnaHyy(const AnaSampleConfig& asc);
    virtual ~AnaHyy() override;
    virtual void Loop() override;

private:
    eSampleType m_type;
    TFile* m_rInFile;
    TFile* m_rOutFile;
    TH1D* m_rCutflow;
};

#endif // ANAHYY_H
