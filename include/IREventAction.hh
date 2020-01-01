#ifndef IREventAction_h
#define IREventAction_h 1

#include "map"
#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class IREventAction : public G4UserEventAction
{
  public:
    IREventAction();
    virtual ~IREventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);
    
    void AddEdep(G4double de);
    std::map<int, G4ThreeVector> origins;
};

#endif

