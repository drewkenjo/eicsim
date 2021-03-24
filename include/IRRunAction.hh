#ifndef IRRunAction_h
#define IRRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "IREventAction.hh"

class G4Run;
class IREventAction;

class IRRunAction : public G4UserRunAction
{
  public:
    IRRunAction(IREventAction* eventAction);
    virtual ~IRRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

  private:
    IREventAction* fEventAction;
};

#endif

