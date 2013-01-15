#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    G4int n_particle = 1;
    particleGun = new G4ParticleGun(n_particle);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    particleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
    particleGun->SetParticleEnergy(511 * keV);
    particleGun->SetParticlePosition(G4ThreeVector(-5 * cm, 0.0, 0.0));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    G4ThreeVector v(1.0, 0.0, 0.0);
    particleGun->SetParticleMomentumDirection(v);
    particleGun->GeneratePrimaryVertex(anEvent);
}
