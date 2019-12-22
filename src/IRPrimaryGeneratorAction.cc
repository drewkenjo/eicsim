#include "IRPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


IRPrimaryGeneratorAction::IRPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(6.*MeV);
}


IRPrimaryGeneratorAction::~IRPrimaryGeneratorAction()
{
  delete fParticleGun;
}


void IRPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double size = 0.8;

  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1,0,0.1));
  fParticleGun->SetParticlePosition(G4ThreeVector(0,0,(rnd.flat()-0.5)*60*cm));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}


