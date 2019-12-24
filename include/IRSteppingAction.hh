#ifndef IRSteppingAction_h
#define IRSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class IRDetectorConstruction;
class IREventAction;


class IRSteppingAction : public G4UserSteppingAction
{
public:
  IRSteppingAction(const IRDetectorConstruction* detectorConstruction,
                    IREventAction* eventAction);
  virtual ~IRSteppingAction();

  virtual void UserSteppingAction(const G4Step* step);
    
private:
  const IRDetectorConstruction* fDetConstruction;
  IREventAction*  fEventAction;  
};


#endif
