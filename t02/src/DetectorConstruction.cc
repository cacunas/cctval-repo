#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4NistManager.hh"

#include "G4SDManager.hh"

#include "G4UserLimits.hh"
#include "G4LogicalVolumeStore.hh"

// Constructor 
DetectorConstruction::DetectorConstruction(RunAction* aRunAction)
: solidWorld(NULL), logicWorld(NULL), physiWorld(NULL),
solidTarget(NULL), logicTarget(NULL), physiTarget(NULL),
//solidSensor_L(NULL), logicSensor_L(NULL), physiSensor_L(NULL),
//solidSensor_R(NULL), logicSensor_R(NULL), physiSensor_R(NULL),
runAction(aRunAction)
{
}

//Destructor

DetectorConstruction::~DetectorConstruction()
{
}

//Construcción de los volumenes y sus materiales 

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    //Get Elements From Material Data Base
    G4NistManager* manager = G4NistManager::Instance();

    G4Element* Oxigen = manager->FindOrBuildElement("O");
    G4Element* Lutetium = manager->FindOrBuildElement("Lu");
    G4Element* Yttrium = manager->FindOrBuildElement("Y");

    //Air
    G4Material* Air = manager->FindOrBuildMaterial("G4_AIR");

    //Lutetium Oxide Lu2O3
    G4Material* LutetiumOxide =
            new G4Material("LutetiumOxide", 9.41 * g / cm3, 2);
    LutetiumOxide->AddElement(Lutetium, 2);
    LutetiumOxide->AddElement(Oxigen, 3);

    //Silicon Dioxide SiO2
    G4Material* SiliconDioxide =
            manager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

    //Yttrium Oxide Y2O3
    G4Material* YttriumOxide =
            new G4Material("YttriumOxide", 5.01 * g / cm3, 2);
    YttriumOxide->AddElement(Yttrium, 2);
    YttriumOxide->AddElement(Oxigen, 3);

    //Build LYSO Material
    G4Material* LYSO = new G4Material("LYSO", 7.1 * g / cm3, 3);
    LYSO->AddMaterial(LutetiumOxide, 81 * perCent);
    LYSO->AddMaterial(SiliconDioxide, 14 * perCent);
    LYSO->AddMaterial(YttriumOxide, 5 * perCent);

    const G4int num = 20;
    G4double ene[num] = {1.79 * eV, 1.85 * eV, 1.91 * eV, 1.97 * eV,
        2.04 * eV, 2.11 * eV, 2.19 * eV, 2.27 * eV,
        2.36 * eV, 2.45 * eV, 2.56 * eV, 2.67 * eV,
        2.80 * eV, 2.94 * eV, 3.09 * eV, 3.25 * eV,
        3.44 * eV, 3.65 * eV, 3.89 * eV, 4.16 * eV};

    G4double fast[num] = {0.01, 0.10, 0.20, 0.50,
        0.90, 1.70, 2.90, 5.00,
        8.30, 12.5, 17.0, 22.9,
        26.4, 25.6, 16.8, 4.20,
        0.30, 0.20, 0.10, 0.01};

    G4double rLyso[num] = {1.81, 1.81, 1.81, 1.81,
        1.81, 1.81, 1.81, 1.81,
        1.81, 1.81, 1.81, 1.81,
        1.81, 1.81, 1.81, 1.81,
        1.81, 1.81, 1.81, 1.81};

    G4double rAir[num] = {1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00};

    G4double abs[num] = {3.5 * m, 3.5 * m, 3.5 * m, 3.5 * m,
        3.5 * m, 3.5 * m, 3.5 * m, 3.5 * m,
        3.5 * m, 3.5 * m, 3.5 * m, 3.5 * m,
        3.5 * m, 3.5 * m, 3.5 * m, 3.5 * m,
        3.5 * m, 3.5 * m, 3.5 * m, 3.5 * m};
    
// Constraints for minimum energy solution (test) ------------------------------
    G4LogicalVolumeStore* lvStore=G4LogicalVolumeStore::GetInstance();
    G4UserLimits *MyLimits = new G4UserLimits();
    MyLimits->SetUserMinEkine(3.542405*eV);
    for (size_t i=0; i<lvStore->size(); i++) {
        (*lvStore)[i]->SetUserLimits(MyLimits);
    }
//------------------------------------------------------------------------------

    G4MaterialPropertiesTable* mpt1 = new G4MaterialPropertiesTable();
    G4MaterialPropertiesTable* mpt2 = new G4MaterialPropertiesTable();

    mpt1->AddProperty("FASTCOMPONENT", ene, fast, num);
    mpt1->AddProperty("RINDEX", ene, rLyso, num);
    mpt1->AddProperty("ABSLENGTH", ene, abs, num);
    mpt1->AddConstProperty("SCINTILLATIONYIELD", 32 / keV);
    mpt1->AddConstProperty("RESOLUTIONSCALE", 1);
    mpt1->AddConstProperty("FASTTIMECONSTANT", 41 * ns);

    mpt2->AddProperty("RINDEX", ene, rAir, num);

    LYSO->SetMaterialPropertiesTable(mpt1);
    Air ->SetMaterialPropertiesTable(mpt2);

// Vacuum definition -----------------------------------------------------------
    G4int density = universe_mean_density; //from PhysicalConstants.h
    G4double pressure = 1.e-19 * pascal;
    G4double temperature = 0.1 * kelvin;
    G4Material* Vacuum = new G4Material("Vacuum", 1.,
        1.01 * g / mole, density, kStateGas, temperature, pressure);
//------------------------------------------------------------------------------


    //Definición de "World Volume"
    solidWorld = new G4Box("world", 10 * cm, 10 * cm, 10 * cm);
    logicWorld = new G4LogicalVolume(solidWorld, Vacuum, "World", 0, 0, 0);
    physiWorld = new G4PVPlacement(NULL, G4ThreeVector(), logicWorld, "World",
            NULL, false, 0);

    //Definición de "Target Volume"
    solidTarget = new G4Box("target", 2 * mm, 22.5 * mm, 2 * mm);
    logicTarget = new G4LogicalVolume(solidTarget, LYSO, "Target", 0, 0, 0);
    physiTarget = new G4PVPlacement(NULL, G4ThreeVector(0., 0., 0.),
            logicTarget, "Target", logicWorld, false, 0);

    // Definition of "left sensor"
//    solidSensor_L = new G4Box("sensor_l", 2 * mm, 2 * mm, 2 * mm);
//    logicSensor_L =
//            new G4LogicalVolume(solidSensor_L, Air, "Sensor_L", 0, 0, 0);
//    physiSensor_L = new G4PVPlacement(NULL, G4ThreeVector(0., -24.5 * mm, 0.),
//            logicSensor_L, "Sensor_L", logicWorld, false, 0);

    // Definition of "right sensor"
//    solidSensor_R = new G4Box("sensor_r", 2 * mm, 2 * mm, 2 * mm);
//    logicSensor_R =
//            new G4LogicalVolume(solidSensor_R, Air, "Sensor_R", 0, 0, 0);
//    physiSensor_R = new G4PVPlacement(0, G4ThreeVector(0., 24.5 * mm, 0.),
//            logicSensor_L, "Sensor_R", logicWorld, false, 0);

    SensitiveDetector* sd =
            new SensitiveDetector("SensitiveDetector", runAction);
    G4SDManager* SDman = G4SDManager::GetSDMpointer();

//    logicSensor_L->SetSensitiveDetector(sd);
//    logicSensor_R->SetSensitiveDetector(sd);
    
    logicTarget->SetSensitiveDetector(sd);

    SDman->AddNewDetector(sd);

    return physiWorld;
}