#include "RunAction.hh"


RunAction::RunAction()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->CreateH1("Edep", "Energy deposited", 100, 0., 5 * GeV);
    analysisManager->CreateNtuple("Muons", "Muons");

    analysisManager->CreateNtupleIColumn("iEvent");
    analysisManager->CreateNtupleDColumn("Fx");
    analysisManager->CreateNtupleDColumn("Fy");
    analysisManager->CreateNtupleDColumn("Fz");

    analysisManager->CreateNtupleDColumn("fGlobalTime");
    analysisManager->CreateNtupleDColumn("fWlen");
    analysisManager->FinishNtuple(0);
}

RunAction::~RunAction()
{

}


void RunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;

    analysisManager->OpenFile("output" + strRunID.str() + ".root");
}

void RunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    G4int runID = run->GetRunID();

    G4cout << "Finishing out" << runID << G4endl;
    
}
