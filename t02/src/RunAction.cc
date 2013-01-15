#include "RunAction.hh"
#include "g4root.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

RunAction::RunAction()
: G4UserRunAction(), H1Energy(NULL), H1Time(NULL), file(NULL)
{
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
    if (file = TFile::Open("LYSO.root") == NULL)
        file = new TFile("LYSO.root", "RECREATE");

    G4int nBin = 1000;
    G4double eneMin = 1.5;
    G4double eneMax = 4.5;
    H1Energy = new TH1D("Energy", "Photon Energy Distribution ", nBin,
            eneMin, eneMax);
    H1Time = new TH1D("Time", "Photon Time Distribution ", nBin, -50, 350);

}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
    G4cout << "Run " << run->GetRunID() << " done..." << G4endl;
    G4cout << "Photons reaching one single end: ";
    G4cout << (Int_t)(H1Time->GetIntegral())[H1Time->GetNbinsX()+1]/2 << G4endl;
    H1Energy->Write();
    H1Time ->Write();
    file->Close();
}
