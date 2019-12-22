#ifndef IRRunAction_h
#define IRRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <vector>

class G4Run;

class IRRunAction : public G4UserRunAction
{
  public:
    IRRunAction();
    virtual ~IRRunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void AddStep (G4ThreeVector pos);

  private:
    std::vector<G4double> fX;
    std::vector<G4double> fY;
    std::vector<G4double> fZ;
};

#endif

