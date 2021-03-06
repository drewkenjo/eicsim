#ifndef IRPrimaryGeneratorAction_h
#define IRPrimaryGeneratorAction_h 1

#include <fstream>
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "IREventAction.hh"
#include "Randomize.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Box;
class IREventAction;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class IRPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    IRPrimaryGeneratorAction(G4String, IREventAction* eventAction);
    virtual ~IRPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    G4Random rnd;
    inline static std::ifstream lundFile;
    IREventAction* fEventAction;
};


#endif
