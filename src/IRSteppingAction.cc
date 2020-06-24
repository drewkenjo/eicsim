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
  int tid = step->GetTrack()->GetTrackID();
//  if(fEventAction->origins.count(tid)==0)
//    fEventAction->origins[tid] = step->GetPreStepPoint()->GetPosition();


  // get volume of the current step
  auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();

/*
  G4String prename = "--", postname = "==";
  if(step->GetPreStepPoint()->GetProcessDefinedStep())
    prename = step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName();
  if(step->GetPostStepPoint()->GetProcessDefinedStep())
    postname = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

  G4cout<<"kim: "<<step->GetTrack()->GetTrackID()<<" "
        <<step->GetTrack()->GetCurrentStepNumber()<<" "
        <<prename<<" "<<postname
        <<G4endl;
*/


/*
  G4String prename = "--", postname = "==";
  if(step->GetPreStepPoint()->GetPhysicalVolume())
    prename = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  if(step->GetPostStepPoint()->GetPhysicalVolume())
    postname = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();
*/

//  if ( fDetConstruction->IsSensitive(volume) ) {
//    G4cout<<step->GetTrack()->GetTrackID()<<" track id, kim, edep="<<edep<<" "<<volume->GetName()<<G4endl;
//    fEventAction->AddEdep(edep);
//  }

  G4ThreeVector pos0 = step->GetPreStepPoint()->GetPosition();
  G4ThreeVector pos1 = step->GetPostStepPoint()->GetPosition();

  auto vol1 = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
  if(fDetConstruction->IsSensitive(vol1))
  if(fEventAction->origins.count(tid)==0) {
    G4ThreeVector mom = step->GetPreStepPoint()->GetMomentum();
    fEventAction->origins[tid] = pos0;
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0,0, mom.x()/GeV);
    analysisManager->FillNtupleDColumn(0,1, mom.y()/GeV);
    analysisManager->FillNtupleDColumn(0,2, mom.z()/GeV);
    analysisManager->FillNtupleIColumn(0,3, fDetConstruction->GetDetID(vol1));
    analysisManager->FillNtupleDColumn(0,4, pos0.x()/cm);
    analysisManager->FillNtupleDColumn(0,5, pos0.y()/cm);
    analysisManager->FillNtupleDColumn(0,6, pos0.z()/cm);
    analysisManager->FillNtupleIColumn(0,7, step->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
    analysisManager->FillNtupleIColumn(0,8, step->GetTrack()->GetParentID());
    analysisManager->AddNtupleRow(0);
  }



  if ( fDetConstruction->IsSensitive(vol1) ) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH2(2, pos0.x()/cm, pos0.y()/cm);
    analysisManager->FillH2(3, pos0.x()/cm, pos0.y()/cm, edep/GeV);

//    if(edep>0 && step->GetPreStepPoint()->GetProcessDefinedStep() && step->GetPostStepPoint()->GetProcessDefinedStep()) G4cout<<"kim: process ="
//      <<step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName()<<" "
//      <<step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()<<G4endl;


    analysisManager->FillNtupleDColumn(1,0, edep/GeV);
    analysisManager->FillNtupleDColumn(1,1, pos0.x()/cm);
    analysisManager->FillNtupleDColumn(1,2, pos0.y()/cm);
    analysisManager->FillNtupleDColumn(1,3, pos0.z()/cm);
    analysisManager->FillNtupleDColumn(1,4, pos1.x()/cm);
    analysisManager->FillNtupleDColumn(1,5, pos1.y()/cm);
    analysisManager->FillNtupleDColumn(1,6, pos1.z()/cm);
    analysisManager->FillNtupleDColumn(1,7, step->GetPreStepPoint()->GetTotalEnergy()/GeV);
    analysisManager->FillNtupleIColumn(1,8, tid);
    analysisManager->FillNtupleIColumn(1,9, fDetConstruction->GetDetID(vol1));
    analysisManager->AddNtupleRow(1);
  }

  if(step->GetTrack()->GetTrackID()!=1) return;

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

    analysisManager->FillH1(0, 1, step->GetPreStepPoint()->GetTotalEnergy()/GeV);
  }
  if(step->GetTrack()->GetCurrentStepNumber()==1) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH1(0, 0);
  }
}

