#include "SensitiveDetector.hh"
#include "RunAction.hh"

#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"
#include "G4HCofThisEvent.hh"
#include "G4OpticalPhoton.hh"

#include "TH1.h"

SensitiveDetector::SensitiveDetector(const G4String& name,
        RunAction* runaction)
: G4VSensitiveDetector(name),
runAction(runaction),
energy(0.),
time(0.),
pos(G4ThreeVector(0., 0., 0.)),
mom(G4ThreeVector(0., 0., 0.)),
point(0)
{
}

SensitiveDetector::~SensitiveDetector()
{
}

void SensitiveDetector::Initialize(G4HCofThisEvent*)
{
}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{

    if (step->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhoton()) {

        //Photons Entering The Sensitive Detector
        point = step->GetPreStepPoint();
        pos = point->GetPosition();
        mom = point->GetMomentumDirection();
        energy = point->GetKineticEnergy();
        time = point->GetGlobalTime();
        runAction->H1Energy->Fill(energy / eV);
        runAction->H1Time->Fill(time / ns);

    }
    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
}
