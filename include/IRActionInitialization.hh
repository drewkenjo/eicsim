#ifndef IRActionInitialization_h
#define IRActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"

class IRDetectorConstruction;

class IRActionInitialization : public G4VUserActionInitialization
{
  public:
    IRActionInitialization(IRDetectorConstruction*, G4String);
    virtual ~IRActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    IRDetectorConstruction* fDetConstruction;
    G4String lundFilename;
};

#endif

    
