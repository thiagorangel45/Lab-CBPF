#include "PMPrimaryGenerator.hh"
#include "Randomize.hh"  // Biblioteca do Geant4 para geração de números aleatórios

PMPrimaryGenerator::PMPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);

    // Particle position (fixa)
    G4double x = 0. * m;
    G4double y = 0. * m;
    G4double z = 0.5 * m;
    G4ThreeVector pos(x, y, z);

    fParticleGun->SetParticlePosition(pos);
}

PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleGun;
}

void PMPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // Partícula
    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    fParticleGun->SetParticleDefinition(particle);

    // Energia fixa
    fParticleGun->SetParticleEnergy(100. * MeV);

    // Gera direção de momento aleatória normalizada
    G4double px = G4RandFlat::shoot(-1.0, 1.0);
    G4double py = G4RandFlat::shoot(-1.0, 1.0);
    G4double pz = G4RandFlat::shoot(-1.0, 1.0);

    G4ThreeVector mom(px, py, pz);
    mom = mom.unit();  // Normaliza o vetor para ter módulo 1

    fParticleGun->SetParticleMomentumDirection(mom);

    // Gera o vértice primário
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
