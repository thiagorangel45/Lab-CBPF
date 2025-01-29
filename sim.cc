#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "DetectorConstruction.hh"
#include "QGSP_BERT.hh" // Include a pre-defined physics list

int main(int argc, char** argv) {
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    G4RunManager* runManager = new G4RunManager();

    // Set mandatory initialization classes
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new QGSP_BERT()); // Use a pre-defined physics list

    // Initialize the run manager
    runManager->Initialize();

    // Visualization
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    // Apply visualization commands
    G4UImanager::GetUIpointer()->ApplyCommand("/control/execute vis.mac");

    // Start the UI session
    ui->SessionStart();

    // Cleanup
    delete visManager;
    delete runManager;
    delete ui;
    return 0;
}