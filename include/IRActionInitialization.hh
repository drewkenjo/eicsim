#ifndef IRActionInitialization_h
#define IRActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class IRActionInitialization : public G4VUserActionInitialization
{
  public:
    IRActionInitialization();
    virtual ~IRActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};


#endif
