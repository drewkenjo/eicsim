#include "IRSteppingAction.hh"
#include "IREventAction.hh"
#include "IRDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "g4root.hh"


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

/*
  G4String prename = "--", postname = "==";
  if(step->GetPreStepPoint()->GetPhysicalVolume())
    prename = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  if(step->GetPostStepPoint()->GetPhysicalVolume())
    postname = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();
*/

  if ( fDetConstruction->IsSensitive(volume) ) {
//    G4cout<<step->GetTrack()->GetTrackID()<<" track id, kim, edep="<<edep<<" "<<volume->GetName()<<G4endl;
    fEventAction->AddEdep(edep);
  }

  if(step->GetTrack()->GetTrackID()!=1) return;

  G4ThreeVector pos0 = step->GetPreStepPoint()->GetPosition();
  G4ThreeVector pos1 = step->GetPostStepPoint()->GetPosition();

  if ( fDetConstruction->IsSensitive(volume) ) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH2(2, pos0.x()/cm, pos0.y()/cm);
    analysisManager->FillH2(3, pos0.x()/cm, pos0.y()/cm, edep/GeV);

    analysisManager->FillNtupleDColumn(0, edep/GeV);
    analysisManager->FillNtupleDColumn(1, pos0.z()/cm);
    analysisManager->FillNtupleDColumn(2, pos0.x()/cm);
    analysisManager->FillNtupleDColumn(3, pos0.y()/cm);
    analysisManager->AddNtupleRow();
  }

  if(pos0.z()>0 && pos1.z()<0) {
    G4double x0 = pos0.x() + (pos1.x()-pos0.x())*(0-pos0.z())/(pos1.z()-pos0.z());
    G4double y0 = pos0.y() + (pos1.y()-pos0.y())*(0-pos0.z())/(pos1.z()-pos0.z());
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH2(0, x0/cm, y0/cm);
  }
  if(pos0.z()>-1*m && pos1.z()<-1*m) {
    G4double x1 = pos0.x() + (pos1.x()-pos0.x())*(-1*m-pos0.z())/(pos1.z()-pos0.z());
    G4double y1 = pos0.y() + (pos1.y()-pos0.y())*(-1*m-pos0.z())/(pos1.z()-pos0.z());
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH2(1, x1/cm, y1/cm);
  }
}

