#ifndef IRRunAction_h
#define IRRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class IRRunAction : public G4UserRunAction
{
  public:
    IRRunAction();
    virtual ~IRRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
};

#endif

