#include "IRSteppingAction.hh"
#include "IREventAction.hh"
#include "IRDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"


IRSteppingAction::IRSteppingAction(
                      const IRDetectorConstruction* detectorConstruction,
                      IREventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{}


IRSteppingAction::~IRSteppingAction()
{}


void IRSteppingAction::UserSteppingAction(const G4Step* step)
{
  // get volume of the current step
  auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();
  
  if ( fDetConstruction->IsSensitive(volume) )
    fEventAction->AddEdep(edep);
}

