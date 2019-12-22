#ifndef IREventAction_h
#define IREventAction_h 1

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class IRRunAction;

/// Event action class
///

class IREventAction : public G4UserEventAction
{
  public:
    IREventAction(IRRunAction* runAction);
    virtual ~IREventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddStep(G4ThreeVector pos);

  private:
    IRRunAction* fRunAction;
};


#endif

