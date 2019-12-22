#include "IREventAction.hh"
#include "IRRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"


IREventAction::IREventAction(IRRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction)
{}


IREventAction::~IREventAction()
{}


void IREventAction::BeginOfEventAction(const G4Event*)
{
}

void IREventAction::EndOfEventAction(const G4Event*)
{
}

void IREventAction::AddStep(G4ThreeVector pos)
{
  fRunAction->AddStep(pos);
}

