#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class PhysicsList : public G4VUserPhysicsList {
public:

	PhysicsList();
	~PhysicsList();

private:

	void ConstructParticle();
	void ConstructProcess();
	void SetCuts();

	void ConstructEM();
	void ConstructOp();
	void ConstructScintillation();

};

#endif


