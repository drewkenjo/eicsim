#include "IREventAction.hh"
#include "IRRunAction.hh"
#include "g4root.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>


IREventAction::IREventAction()
 : G4UserEventAction(),
   fEnergy(0.)
{}


IREventAction::~IREventAction()
{}


void IREventAction::BeginOfEventAction(const G4Event* /*event*/)
{  
  // initialisation per event
  fEnergy = 0.;
}


void IREventAction::EndOfEventAction(const G4Event* event)
{
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // fill histograms
  analysisManager->FillH1(0, fEnergy);
  
  // fill ntuple
  analysisManager->FillNtupleDColumn(0, fEnergy);
  analysisManager->AddNtupleRow();  
  
  // Print per event (modulo n)
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     

    G4cout
       << "   Absorber: total energy: " << std::setw(7)
                                        << G4BestUnit(fEnergy,"Energy")
       << G4endl;
  }
}  

