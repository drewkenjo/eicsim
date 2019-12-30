#include "IREventAction.hh"
#include "IRRunAction.hh"
#include "g4root.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>


IREventAction::IREventAction()
 : G4UserEventAction()
{}


IREventAction::~IREventAction()
{}


void IREventAction::BeginOfEventAction(const G4Event* /*event*/)
{  
}


void IREventAction::EndOfEventAction(const G4Event* event)
{
  // get analysis manager
  // auto analysisManager = G4AnalysisManager::Instance();

  // fill histograms
  //analysisManager->FillH1(0, fEnergy/GeV);
  
  // fill ntuple
  //analysisManager->FillNtupleDColumn(0, fEnergy/GeV);
  //analysisManager->AddNtupleRow(0);
  
  // Print per event (modulo n)
  // auto eventID = event->GetEventID();

/*
  if ( eventID % 1000 == 0 ) {
    G4cout << "---> End of event: " << eventID << G4endl;     

    G4cout
       << "   Absorber: total energy: " << std::setw(7)
                                        << G4BestUnit(fEnergy/GeV,"Energy")
       << G4endl;
  }
*/
}  

