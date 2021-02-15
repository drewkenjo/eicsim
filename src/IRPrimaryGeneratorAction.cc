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
  if(!lundFile.is_open()) lundFile.open(filename, std::ios::binary);
}


IRPrimaryGeneratorAction::~IRPrimaryGeneratorAction()
{
  G4AutoLock lock(&myMutex);
  if(lundFile.is_open()) {
    lundFile.close();
  }
  delete fParticleGun;
}


void IRPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(lundFile.is_open())
  {
    G4AutoLock lock(&myMutex);
    float px,py,pz;
    float vx,vy,vz;
    lundFile.read(reinterpret_cast<char*>(&px), sizeof(float));
    lundFile.read(reinterpret_cast<char*>(&py), sizeof(float));
    lundFile.read(reinterpret_cast<char*>(&pz), sizeof(float));
    lundFile.read(reinterpret_cast<char*>(&vx), sizeof(float));
    lundFile.read(reinterpret_cast<char*>(&vy), sizeof(float));
    lundFile.read(reinterpret_cast<char*>(&vz), sizeof(float));

    if(lundFile.good()) {
//std::cout<<px<<" "<<py<<" "<<pz<<" "<<vx<<" "<<vy<<" "<<vz<<std::endl;
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz).unit());
      fParticleGun->SetParticleEnergy(sqrt(px*px + py*py + pz*pz)*GeV);
      fParticleGun->SetParticlePosition(G4ThreeVector(vx*cm,vy*cm,vz*cm));
      fParticleGun->GeneratePrimaryVertex(anEvent);
    }
  }
}


