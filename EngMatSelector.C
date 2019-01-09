#include "TH2D"
#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "./TXPConfig.h"

class EngMatSelector : public TSelector
{
    public:

        TH2D* fMatEn;
        TXPConfig* fCal;

        TTreeReader fReader;
        TTreeReaderValue<int> fenergy;
        TTreeReaderValue<short> fadc;

        EngMatSelector(TTree* = 0): fMatEn(nullptr),
                    fCal(nullptr),
                    fenergy(fReader, "energy"),
                    fadc(fReader, "adc"), 
                    fmultiplicity(fReader, "multiplicity") {}
        virtual ~EngMatSelector() {}
        virtual void Init( TTree* Tree );
	    virtual void    SlaveBegin(TTree *tree);
	    virtual Bool_t  Process(Long64_t entry);
	    virtual void    Terminate();
	    virtual Int_t   Version() const { return 1; }

        ClassDef(EngMatSelector,0);
};

void TutorialSelector::Init(TTree *tree)
{
    fReader.SetTree(Tree);
    fCal = new TXPConfig("./XPConfig.txt"); // Set the calibration
}

void TutorialSelector::SlaveBegin(TTree *tree)
{
    fMatEn = new TH2D("engMat", "Energy Matrix",
            100, 0, 100,
            7000, 0, 7000);
    GetOutputList()->Add(fMatEn);
}

Bool_t TutorialSelector::Process(Long64_t entry)
{
    fReader.SetEntry(entry);
    
    if ( *fmultiplicity == 1 )
        if( *energy > 2 )
            fMatEn->Fill( *adc, fCal->GetEnergy( *energy, *adc ) );

    return kTrue;
}

void TutorialSelector::Terminate()
{
    fMatEn->Print();
}
