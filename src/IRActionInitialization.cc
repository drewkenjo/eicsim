#include "IRActionInitialization.hh"
#include "IRPrimaryGeneratorAction.hh"
#include "IRRunAction.hh"
#include "IREventAction.hh"
#include "IRSteppingAction.hh"
#include "IRDetectorConstruction.hh"


IRActionInitialization::IRActionInitialization(IRDetectorConstruction* detConstruction, G4String filename)
 : G4VUserActionInitialization(),
   fDetConstruction(detConstruction),
   lundFilename(filename)
{}


IRActionInitialization::~IRActionInitialization()
{}


void IRActionInitialization::BuildForMaster() const
{
  auto eventAction = new IREventAction;
  SetUserAction(new IRRunAction(eventAction));
}


void IRActionInitialization::Build() const
{
  auto eventAction = new IREventAction;
  SetUserAction(new IRPrimaryGeneratorAction(lundFilename, eventAction));
  SetUserAction(new IRRunAction(eventAction));
  SetUserAction(eventAction);
  SetUserAction(new IRSteppingAction(fDetConstruction,eventAction));
}  

