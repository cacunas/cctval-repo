#include "G4RunManager.hh"
#include "G4UImanager.hh"
#ifdef G4UI_USE
#include "G4VisExecutive.hh"
#endif
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"

int
main(int argc, char** argv)
{
    // construct the default run manager
    G4RunManager* runManager = new G4RunManager;

    //set mandatory initialization aclasses
    runManager->SetUserInitialization(new DetectorConstruction);
    runManager->SetUserInitialization(new PhysicsList);

    runManager->SetUserAction(new PrimaryGeneratorAction);

    //set event action
    EventAction* eventAction = new EventAction;
    runManager->SetUserAction(eventAction);

    // initialize G4 kernel
    runManager->Initialize();

#ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (argc != 1) {
        G4String command = ("/control/execute ");
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {

#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
        UImanager->ApplyCommand("/control/execute vis.mac");
#endif
        ui->SessionStart();
        delete ui;
#endif

#ifdef G4VIS_USE
        delete visManager;
#endif

    }

    delete runManager;

    return 0;
}
