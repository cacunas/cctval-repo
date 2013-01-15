#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class PhysicsList : public G4VUserPhysicsList {
public:
	PhysicsList();
	~PhysicsList();

protected:
	//Construcción de partículas y procesos
	void ConstructParticle();
	void ConstructProcess();
	void SetCuts();


protected:
	//Métodos para construir partículas 
	void ConstructBosons();
	void ConstructLeptons();
	void ConstructMesons();
	void ConstructBaryons();

protected:
	//Métodos para construir la física de los procesos y registrarlos
	void ConstructGeneral();
	void AddStepMax();
	void ConstructEM();
	void ConstructOp();
	void ConstructScintillation();
};

#endif


