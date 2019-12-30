#include "IRRunAction.hh"
#include "g4root.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


IRRunAction::IRRunAction()
 : G4UserRunAction()
{ 
  // set printing event number per each event
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Creating histograms
  //analysisManager->CreateH1("Edep","Edep in SVT", 100, 0., 800*MeV);
  analysisManager->CreateH2("xy0","X:Y at z=0", 100, -2,2, 100,-2,2);
  analysisManager->CreateH2("xy1","X:Y at z=-1m", 100, -2,2, 100,-2,2);
  analysisManager->CreateH2("xys","X:Y SVT, hits", 100, -20,20, 100,-20,20);
  analysisManager->CreateH2("xye","X:Y SVT, edep [GeV] weighted", 100, -20,20, 100,-20,20);
  analysisManager->CreateH1("flux","flux total energy", 2, 0,2);

  // Creating ntuple
  analysisManager->CreateNtuple("ir", "Edeps");
  analysisManager->CreateNtupleDColumn("edep");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleIColumn("detId");
  analysisManager->FinishNtuple();
}


IRRunAction::~IRRunAction()
{
  delete G4AnalysisManager::Instance();  
}


void IRRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  G4String fileName = "IR";
  analysisManager->OpenFile(fileName);
}


void IRRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  auto analysisManager = G4AnalysisManager::Instance();

  // save histograms & ntuple
  analysisManager->Write();
  analysisManager->CloseFile();
}

