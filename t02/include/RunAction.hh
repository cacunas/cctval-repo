#ifndef DetectorRunAction_h
#define DetectorRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "TFile.h"
#include "TH1.h"

class G4Run;

class RunAction : public G4UserRunAction {
public:
	RunAction();
	~RunAction();

	void BeginOfRunAction(const G4Run*);
	void EndOfRunAction(const G4Run*);

//private:
	TH1D* H1Energy;
	TH1D* H1Time;
	TFile* file;
};

#endif
