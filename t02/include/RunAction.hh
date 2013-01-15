#ifndef DetectorRunAction_h
#define DetectorRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "g4root.hh"

#include "TFile.h"
#include "TH1.h"

class G4Run;

class RunAction : public G4UserRunAction {
public:
	RunAction();
	~RunAction();

	void BeginOfRunAction(const G4Run*);
	void EndOfRunAction(const G4Run*);

private:
	TFile* file;
	TH1* H1Energy;
	TH1* H1Time;
};

#endif
