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
  pdgs.clear();
  tids.clear();
  pids.clear();
  dids.clear();
  x0s.clear();
  y0s.clear();
  z0s.clear();
  x1s.clear();
  y1s.clear();
  z1s.clear();
  edeps.clear();
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

  if(pdgs.size()>0) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->AddNtupleRow();
  }
}

void IREventAction::SetPrimary(float vx, float vy, float vz, float px, float py, float pz)
{
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->FillNtupleFColumn(0, 0, vx);
  analysisManager->FillNtupleFColumn(0, 1, vy);
  analysisManager->FillNtupleFColumn(0, 2, vz);

  analysisManager->FillNtupleFColumn(0, 3, px);
  analysisManager->FillNtupleFColumn(0, 4, py);
  analysisManager->FillNtupleFColumn(0, 5, pz);
}


void IREventAction::AddHit(int pdg, int tid, int pid, int detId, double x0, double y0, double z0, double x1, double y1, double z1, double edep)
{
  pdgs.push_back(pdg);
  tids.push_back(tid);
  pids.push_back(pid);
  dids.push_back(detId);
  x0s.push_back(x0);
  y0s.push_back(y0);
  z0s.push_back(z0);
  x1s.push_back(x1);
  y1s.push_back(y1);
  z1s.push_back(z1);
  edeps.push_back(edep);
}
