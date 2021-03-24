#ifndef IREventAction_h
#define IREventAction_h 1

#include <iostream>
#include <vector>
#include <map>
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
    
    void SetPrimary(float,float,float,float,float,float);
    void AddHit(int, int, int, int, double, double, double, double, double, double, double);

    std::vector<G4int> pdgs, tids, pids, dids;
    std::vector<G4double> x0s, y0s, z0s, x1s, y1s, z1s, edeps;
};

#endif

