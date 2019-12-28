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
  SetUserAction(new IRRunAction);
}


void IRActionInitialization::Build() const
{
  SetUserAction(new IRPrimaryGeneratorAction(lundFilename));
  SetUserAction(new IRRunAction);
  auto eventAction = new IREventAction;
  SetUserAction(eventAction);
  SetUserAction(new IRSteppingAction(fDetConstruction,eventAction));
}  

