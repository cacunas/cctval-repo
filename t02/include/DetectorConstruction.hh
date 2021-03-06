#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include "RunAction.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:

	DetectorConstruction(RunAction*);
	~DetectorConstruction();

public:

	G4VPhysicalVolume* Construct();

	//private:
	//	void CreateScorers();

private:

	G4Box* solidWorld;
	G4LogicalVolume* logicWorld;
	G4VPhysicalVolume* physiWorld;

	G4Box* solidTarget;
	G4LogicalVolume* logicTarget;
	G4VPhysicalVolume* physiTarget;

//	G4Box* solidSensor_L;
//	G4LogicalVolume* logicSensor_L;
//	G4VPhysicalVolume* physiSensor_L;
//
//	G4Box* solidSensor_R;
//	G4LogicalVolume* logicSensor_R;
//	G4VPhysicalVolume* physiSensor_R;
	
	RunAction *runAction;
};

#endif
