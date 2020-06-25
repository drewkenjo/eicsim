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
#include "G4AutoLock.hh"
namespace { G4Mutex myMutex = G4MUTEX_INITIALIZER; }

LundFileReader* IRPrimaryGeneratorAction::lundFile = 0;

IRPrimaryGeneratorAction::IRPrimaryGeneratorAction(G4String filename)
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(22);
  fParticleGun->SetParticleDefinition(particle);

  G4AutoLock lock(&myMutex);
  if(!lundFile) lundFile = new LundFileReader(filename);
}


IRPrimaryGeneratorAction::~IRPrimaryGeneratorAction()
{
  G4AutoLock lock(&myMutex);
  if(lundFile) {
    delete lundFile;
    lundFile = 0;
  }
  delete fParticleGun;
}


void IRPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(lundFile)
  {
    G4AutoLock lock(&myMutex);
    std::stringstream ss(lundFile->getline());
    double vx,vy,vz;
    double px,py,pz,e;
    ss>>px>>py>>pz>>e>>vx>>vy>>vz;

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px*GeV,py*GeV,-pz*GeV).unit());
    fParticleGun->SetParticleEnergy(e*GeV);
    fParticleGun->SetParticlePosition(G4ThreeVector(vx*cm,vy*cm,-vz*cm));
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
}


