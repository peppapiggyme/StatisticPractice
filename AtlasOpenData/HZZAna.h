#include "AtlasOpenData/HZZAnaBase.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "Utils.h"

#include <string>

enum class eSampleType : uint8_t { DATA = 0, BKG = 1, SIG = 2 };

class HZZAnaConfig
{
public:
    HZZAnaConfig(eSampleType tp, const string& sInput, const string& sOutput)
        : type(tp), inputPath(sInput), outputPath(sOutput) 
    {        
        const auto nFind_0 = inputPath.rfind(".");
        const auto nFind_1 = inputPath.rfind(".", nFind_0-1);
        channel = inputPath.substr(nFind_1+1, nFind_0 - nFind_1 - 1);

        if (type == eSampleType::DATA)
        {
            dsid = "0";
            process = "data";
        }
        else
        {
            const auto nFind_2 = inputPath.rfind(".", nFind_1-1);
            const auto nFind_3 = inputPath.rfind("_", nFind_2-1);
            dsid = inputPath.substr(nFind_3+1, nFind_2 - nFind_3 - 1);
            process = inputPath.substr(nFind_2+1, nFind_1 - nFind_2 - 1);
        }
        
        SP::IO::println("CONFIGURING [%] ...", process);        
    }

public:
    eSampleType type;
    string inputPath;
    string outputPath;
    string dsid;
    string process;
    string channel;
};

class HZZAna : public HZZAnaBase
{
public:
    explicit HZZAna(const HZZAnaConfig& cConfig);
    virtual ~HZZAna() override;
    virtual void Loop() override;
private:
    eSampleType m_eType;
    HZZAnaConfig m_cConfig;
    TFile* m_cInput;
    TFile* m_cOutput;
};
