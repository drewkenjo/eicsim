#ifndef IRSteppingAction_h
#define IRSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class IREventAction;

class G4LogicalVolume;

class IRSteppingAction : public G4UserSteppingAction
{
  public:
    IRSteppingAction(IREventAction* eventAction);
    virtual ~IRSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    IREventAction*  fEventAction;
};


#endif
