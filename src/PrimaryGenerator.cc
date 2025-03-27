#include "PrimaryGenerator.hh"

PrimaryGenerator::PrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);

    //particle postition
    G4double x = 0 * m;
    G4double y = 0 * m;
    G4double z = -1 * cm;

    G4ThreeVector pos(x, y, z);

    //Particle direction
    G4double px = 0;
    G4double py = 0;
    G4double pz = 2;

    G4ThreeVector mom(px, py, pz);

    // particle type
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("mu-");

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(1. *GeV);
    fParticleGun->SetParticleDefinition(particle);

}


PrimaryGenerator::~PrimaryGenerator()
{
    delete fParticleGun;
}


void PrimaryGenerator::GeneratePrimaries(G4Event *AnEvent)
{
    // create vertex

    fParticleGun->GeneratePrimaryVertex(AnEvent);
}