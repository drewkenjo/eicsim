#include "IRSteppingAction.hh"
#include "IREventAction.hh"
#include "IRDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"


IRSteppingAction::IRSteppingAction(IREventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}


IRSteppingAction::~IRSteppingAction()
{}


void IRSteppingAction::UserSteppingAction(const G4Step* step)
{
  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();

  G4VPhysicalVolume* vol0 = step->GetPreStepPoint()->GetPhysicalVolume();
  G4VPhysicalVolume* vol1 = step->GetPostStepPoint()->GetPhysicalVolume();
  if(vol0 && vol1 && vol0 != vol1){
    fEventAction->AddStep(step->GetPostStepPoint()->GetPosition());
    G4cout<<"kim "<<step->GetPostStepPoint()->GetPosition()<<G4endl;
  }
}

