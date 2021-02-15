#include "IRDetectorConstruction.hh"
#include "IRActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4PhysListFactory.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"
#include <fstream>

int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  G4String lundfile("");
  G4UIExecutive* ui = 0;
  if ( argc == 2 ) {
    lundfile = argv[1];
  }
  else if (argc==3) {
    ui = new G4UIExecutive(argc, argv);
    lundfile = argv[1];
  } else {
    return 0;
  }

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(1);
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  runManager->SetPrintProgress(0);

  // Set mandatory initialization classes
  // Detector construction
  auto detConstruction = new IRDetectorConstruction();
  runManager->SetUserInitialization(detConstruction);

  // Physics list
  G4PhysListFactory physFactory;
  //G4VModularPhysicsList* physicsList = new FTFP_BERT;
  G4VModularPhysicsList* physicsList = physFactory.GetReferencePhysList("QGSP_BERT_HP_EMZ");
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new IRActionInitialization(detConstruction, lundfile));

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    runManager->Initialize();
    std::ifstream lundf(lundfile);
    lundf.seekg(0, std::ios_base::end);
    int numevs = lundf.tellg()/24;
    runManager->BeamOn(numevs);
  }
  else {
    // interactive mode
    G4String macname(argv[2]);
    UImanager->ApplyCommand("/control/execute "+macname);
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
  return 0;
}

