#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

class SensitiveDetector : public G4VSensitiveDetector
{
public:
    SensitiveDetector(G4String);
    ~SensitiveDetector() override;

private:
    G4double fTotalEnergyDeposited;

    void Initialize(G4HCofThisEvent *) override;
    void EndOfEvent(G4HCofThisEvent *) override;
    G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
};

#endif
