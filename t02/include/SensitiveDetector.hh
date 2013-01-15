#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "globals.hh"
#include "G4VSensitiveDetector.hh"
#include "RunAction.hh"

#include "G4VSensitiveDetector.hh"
#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"
#include "G4HCofThisEvent.hh"

class SensitiveDetector : public G4VSensitiveDetector {
public:
	SensitiveDetector(const G4String&, RunAction*);
	~SensitiveDetector();

	void Initialize(G4HCofThisEvent*);
	G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	void EndOfEvent(G4HCofThisEvent*);

private:
	RunAction* runAction;
	G4double energy;
	G4double time;
	G4ThreeVector pos;
	G4ThreeVector mom;
	G4StepPoint* point;

};

#endif
