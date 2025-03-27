#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "DetectorConstruction.hh"
#include "QGSP_BERT.hh" 
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv) {
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    G4RunManager* runManager = new G4RunManager();

    // detector construction
    runManager->SetUserInitialization(new DetectorConstruction());

    runManager->SetUserInitialization(new QGSP_BERT()); 

    //phys list
    runManager->SetUserInitialization(new PhysicsList());

     //Action initialization
     runManager->SetUserInitialization(new ActionInitialization());

    runManager->Initialize();
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();
    G4UImanager::GetUIpointer()->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();

    delete visManager;
    delete runManager;
    delete ui;
    return 0;
}