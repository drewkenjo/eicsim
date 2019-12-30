#ifndef IRDetectorConstruction_h
#define IRDetectorConstruction_h 1

#include "map"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

typedef struct {
  double Dx;
  double Dy;
  double Dz;
  double Rin;
} cb_VTX_ladder_LayParam;


class IRDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    IRDetectorConstruction();
    virtual ~IRDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    bool IsSensitive(G4VPhysicalVolume* physVol) const { return sensitives.count(physVol)>0; };

  protected:
    G4LogicalVolume*  fScoringVolume;
    std::map<G4VPhysicalVolume*, int> sensitives;
};

#endif

