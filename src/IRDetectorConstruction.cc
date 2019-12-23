#include "IRDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "CADMesh.hh"

IRDetectorConstruction::IRDetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }


IRDetectorConstruction::~IRDetectorConstruction()
{ }


G4VPhysicalVolume* IRDetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  G4double world_sizeXY = 50*m;
  G4double world_sizeZ  = 50*m;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidWorld = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);


/*
CADMesh * mesh = new CADMesh("beamlines.stl", mm, G4ThreeVector(0, 0, 0), false);
G4VSolid* cad_solid = mesh->TessellatedMesh();
G4LogicalVolume* cad_logical = new G4LogicalVolume(cad_solid, nist->FindOrBuildMaterial("G4_WATER"), "cad_logical", 0, 0, 0);
G4VPhysicalVolume* cad_physical = new G4PVPlacement(0, G4ThreeVector(), cad_logical, "cad_physical", logicWorld, false, 0);
*/

  //Beamline pipe plus flange from central chamber to NEG flange in ele u/s, holds SR mask
  CADMesh * vol01_mesh = new CADMesh("stl/bodycut.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol01_solid = vol01_mesh->TessellatedMesh();
  G4LogicalVolume* vol01_logic = new G4LogicalVolume(vol01_solid, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "vol01_logic", 0, 0, 0);
  G4VPhysicalVolume* vol01_phys = new G4PVPlacement(0, G4ThreeVector(), vol01_logic, "vol01_phys", logicWorld, false, 0);

  //Beamline pipe plus flange from central chamber to NEG flange in ele d/s
  CADMesh * vol02_mesh = new CADMesh("stl/body.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol02_solid = vol02_mesh->TessellatedMesh();
  G4LogicalVolume* vol02_logic = new G4LogicalVolume(vol02_solid, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "vol02_logic", 0, 0, 0);
  G4VPhysicalVolume* vol02_phys = new G4PVPlacement(0, G4ThreeVector(), vol02_logic, "vol02_phys", logicWorld, false, 0);

  //Flange to NEG chamber, ele u/s
  CADMesh * vol03_mesh = new CADMesh("stl/IP_NEG_CHAMBER_E-US_FLANGE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol03_solid = vol03_mesh->TessellatedMesh();
  G4LogicalVolume* vol03_logic = new G4LogicalVolume(vol03_solid, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "vol03_logic", 0, 0, 0);
  G4VPhysicalVolume* vol03_phys = new G4PVPlacement(0, G4ThreeVector(), vol03_logic, "vol03_phys", logicWorld, false, 0);

  //Innermost layer of central chamber
  CADMesh * vol04_mesh = new CADMesh("stl/IR_CHAMBER_CENTRAL_TUBE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol04_solid = vol04_mesh->TessellatedMesh();
  G4LogicalVolume* vol04_logic = new G4LogicalVolume(vol04_solid, nist->FindOrBuildMaterial("G4_Be"), "vol04_logic", 0, 0, 0);
  G4VPhysicalVolume* vol04_phys = new G4PVPlacement(0, G4ThreeVector(), vol04_logic, "vol04_phys", logicWorld, false, 0);

  //Synchrotron mask in the ele u/s, located inside volume of bodycut.stl
  CADMesh * vol05_mesh = new CADMesh("stl/IR_CHAMBER_ELECTRON_MASK.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol05_solid = vol05_mesh->TessellatedMesh();
  G4LogicalVolume* vol05_logic = new G4LogicalVolume(vol05_solid, nist->FindOrBuildMaterial("G4_Cu"), "vol05_logic", 0, 0, 0);
  G4VPhysicalVolume* vol05_phys = new G4PVPlacement(0, G4ThreeVector(), vol05_logic, "vol05_phys", logicWorld, false, 0);

  //Water cooling layer in central chamber
  CADMesh * vol06_mesh = new CADMesh("stl/IR_CHAMBER_WATERSPACE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol06_solid = vol06_mesh->TessellatedMesh();
  G4LogicalVolume* vol06_logic = new G4LogicalVolume(vol06_solid, nist->FindOrBuildMaterial("G4_WATER"), "vol06_logic", 0, 0, 0);
  G4VPhysicalVolume* vol06_phys = new G4PVPlacement(0, G4ThreeVector(), vol06_logic, "vol06_phys", logicWorld, false, 0);

  //Gold coating on interior of central chamber, 4 microns
  CADMesh * vol07_mesh = new CADMesh("stl/IR_CENTRAL_CHAMBER_GOLD_COAT.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol07_solid = vol07_mesh->TessellatedMesh();
  G4LogicalVolume* vol07_logic = new G4LogicalVolume(vol07_solid, nist->FindOrBuildMaterial("G4_Au"), "vol07_logic", 0, 0, 0);
  G4VPhysicalVolume* vol07_phys = new G4PVPlacement(0, G4ThreeVector(), vol07_logic, "vol07_phys", logicWorld, false, 0);

  // d/s ele beam pipe after grey flange, straight end
  CADMesh * vol08_mesh = new CADMesh("stl/DS_ELECTRON_BASELINE_BEAMLINE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol08_solid = vol08_mesh->TessellatedMesh();
  G4LogicalVolume* vol08_logic = new G4LogicalVolume(vol08_solid, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "vol08_logic", 0, 0, 0);
  G4VPhysicalVolume* vol08_phys = new G4PVPlacement(0, G4ThreeVector(), vol08_logic, "vol08_phys", logicWorld, false, 0);

  // d/s ion beam pipe after grey flange, straight end
  CADMesh * vol09_mesh = new CADMesh("stl/DS_ION_BASELINE_BEAMLINE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol09_solid = vol09_mesh->TessellatedMesh();
  G4LogicalVolume* vol09_logic = new G4LogicalVolume(vol09_solid, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "vol09_logic", 0, 0, 0);
  G4VPhysicalVolume* vol09_phys = new G4PVPlacement(0, G4ThreeVector(), vol09_logic, "vol09_phys", logicWorld, false, 0);
 
  // Flange to NEG chamberr, ele d/s
  CADMesh * vol10_mesh = new CADMesh("stl/NEG_CHAMBER_E-DS_FLANGE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol10_solid = vol10_mesh->TessellatedMesh();
  G4LogicalVolume* vol10_logic = new G4LogicalVolume(vol10_solid, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "vol10_logic", 0, 0, 0);
  G4VPhysicalVolume* vol10_phys = new G4PVPlacement(0, G4ThreeVector(), vol10_logic, "vol10_phys", logicWorld, false, 0);

  // u/s ele beam pipe, afer grey flange, bell-shaped end
  CADMesh * vol11_mesh = new CADMesh("stl/US_ELECTRON_BASELINE_BEAMLINE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol11_solid = vol11_mesh->TessellatedMesh();
  G4LogicalVolume* vol11_logic = new G4LogicalVolume(vol11_solid, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "vol11_logic", 0, 0, 0);
  G4VPhysicalVolume* vol11_phys = new G4PVPlacement(0, G4ThreeVector(), vol11_logic, "vol11_phys", logicWorld, false, 0);

  // u/s ion beam pipe after grey flange, straight end
  CADMesh * vol12_mesh = new CADMesh("stl/US_ION_BASELINE_BEAMLINE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol12_solid = vol12_mesh->TessellatedMesh();
  G4LogicalVolume* vol12_logic = new G4LogicalVolume(vol12_solid, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "vol12_logic", 0, 0, 0);
  G4VPhysicalVolume* vol12_phys = new G4PVPlacement(0, G4ThreeVector(), vol12_logic, "vol12_phys", logicWorld, false, 0);

  //Outer layer for water chamber 
  CADMesh * vol13_mesh = new CADMesh("stl/IP_WATER_TUBE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol13_solid = vol13_mesh->TessellatedMesh();
  G4LogicalVolume* vol13_logic = new G4LogicalVolume(vol13_solid, nist->FindOrBuildMaterial("G4_Be"), "vol13_logic", 0, 0, 0);
  G4VPhysicalVolume* vol13_phys = new G4PVPlacement(0, G4ThreeVector(), vol13_logic, "vol13_phys", logicWorld, false, 0);

  // Endcap 1 for water chamber
  CADMesh * vol14_mesh = new CADMesh("stl/IP_WATER_TUBE_END1.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol14_solid = vol14_mesh->TessellatedMesh();
  G4LogicalVolume* vol14_logic = new G4LogicalVolume(vol14_solid, nist->FindOrBuildMaterial("G4_Be"), "vol14_logic", 0, 0, 0);
  G4VPhysicalVolume* vol14_phys = new G4PVPlacement(0, G4ThreeVector(), vol14_logic, "vol14_phys", logicWorld, false, 0);

  // Endcap 2 for water chamber
  CADMesh * vol15_mesh = new CADMesh("stl/IP_WATER_TUBE_END2.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol15_solid = vol15_mesh->TessellatedMesh();
  G4LogicalVolume* vol15_logic = new G4LogicalVolume(vol15_solid, nist->FindOrBuildMaterial("G4_Be"), "vol15_logic", 0, 0, 0);
  G4VPhysicalVolume* vol15_phys = new G4PVPlacement(0, G4ThreeVector(), vol15_logic, "vol15_phys", logicWorld, false, 0);

  // Balloon of interior up to flange on bodycut, does not include NEG chamber flange nor ele/ion baseline beampipe
  CADMesh * vol16_mesh = new CADMesh("stl/JL0081018.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol16_solid = vol16_mesh->TessellatedMesh();
  G4LogicalVolume* vol16_logic = new G4LogicalVolume(vol16_solid, nist->FindOrBuildMaterial("G4_GALACTIC"), "vol16_logic", 0, 0, 0);
  G4VPhysicalVolume* vol16_phys = new G4PVPlacement(0, G4ThreeVector(), vol16_logic, "vol16_phys", logicWorld, false, 0);

  fScoringVolume = logicWorld;

  return physWorld;
}

