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
    // Define a partícula (mu-)
    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(100. * MeV);

    G4double r = 10. * m;
    G4double theta = std::acos(G4RandFlat::shoot(-1.0, 1.0));  // [0, pi]
    G4double phi = G4RandFlat::shoot(0., 2. * CLHEP::pi);      // [0, 2pi]

    G4double x = r * std::sin(theta) * std::cos(phi);
    G4double y = r * std::sin(theta) * std::sin(phi);
    G4double z = r * std::cos(theta);

    G4ThreeVector pos(x, y, z);
    fParticleGun->SetParticlePosition(pos);

    // Direção apontando para o centro (0,0,0)
    G4ThreeVector mom = -pos.unit();  // vetor normalizado apontando para o centro
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
