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

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"


IRDetectorConstruction::IRDetectorConstruction()
  : G4VUserDetectorConstruction()
{ }


IRDetectorConstruction::~IRDetectorConstruction()
{ }


G4VPhysicalVolume* IRDetectorConstruction::Construct()
{
  double ladder_deltashi = -7. * deg;

  G4NistManager* nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  G4double world_sizeXY = 2*m;
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
  G4LogicalVolume* vol01_logic = new G4LogicalVolume(vol01_solid, nist->FindOrBuildMaterial("G4_Al"), "vol01_logic", 0, 0, 0);
  G4VPhysicalVolume* vol01_phys = new G4PVPlacement(0, G4ThreeVector(), vol01_logic, "vol01_phys", logicWorld, false, 0, checkOverlaps);

  //Beamline pipe plus flange from central chamber to NEG flange in ele d/s
  CADMesh * vol02_mesh = new CADMesh("stl/body.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol02_solid = vol02_mesh->TessellatedMesh();
  G4LogicalVolume* vol02_logic = new G4LogicalVolume(vol02_solid, nist->FindOrBuildMaterial("G4_Al"), "vol02_logic", 0, 0, 0);
  G4VPhysicalVolume* vol02_phys = new G4PVPlacement(0, G4ThreeVector(), vol02_logic, "vol02_phys", logicWorld, false, 0, checkOverlaps);

  //Flange to NEG chamber, ele u/s
  CADMesh * vol03_mesh = new CADMesh("stl/IP_NEG_CHAMBER_E-US_FLANGE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol03_solid = vol03_mesh->TessellatedMesh();
  G4LogicalVolume* vol03_logic = new G4LogicalVolume(vol03_solid, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "vol03_logic", 0, 0, 0);
  G4VPhysicalVolume* vol03_phys = new G4PVPlacement(0, G4ThreeVector(), vol03_logic, "vol03_phys", logicWorld, false, 0, checkOverlaps);

  //Innermost layer of central chamber
  CADMesh * vol04_mesh = new CADMesh("stl/IR_CHAMBER_CENTRAL_TUBE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol04_solid = vol04_mesh->TessellatedMesh();
  G4LogicalVolume* vol04_logic = new G4LogicalVolume(vol04_solid, nist->FindOrBuildMaterial("G4_Be"), "vol04_logic", 0, 0, 0);
  G4VPhysicalVolume* vol04_phys = new G4PVPlacement(0, G4ThreeVector(), vol04_logic, "vol04_phys", logicWorld, false, 0, checkOverlaps);

  //Synchrotron mask in the ele u/s, located inside volume of bodycut.stl
  CADMesh * vol05_mesh = new CADMesh("stl/IR_CHAMBER_ELECTRON_MASK.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol05_solid = vol05_mesh->TessellatedMesh();
  G4LogicalVolume* vol05_logic = new G4LogicalVolume(vol05_solid, nist->FindOrBuildMaterial("G4_Cu"), "vol05_logic", 0, 0, 0);
  G4VPhysicalVolume* vol05_phys = new G4PVPlacement(0, G4ThreeVector(), vol05_logic, "vol05_phys", logicWorld, false, 0, checkOverlaps);

  //Water cooling layer in central chamber
  CADMesh * vol06_mesh = new CADMesh("stl/IR_CHAMBER_WATERSPACE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol06_solid = vol06_mesh->TessellatedMesh();
  G4LogicalVolume* vol06_logic = new G4LogicalVolume(vol06_solid, nist->FindOrBuildMaterial("G4_WATER"), "vol06_logic", 0, 0, 0);
  G4VPhysicalVolume* vol06_phys = new G4PVPlacement(0, G4ThreeVector(), vol06_logic, "vol06_phys", logicWorld, false, 0, checkOverlaps);

  //Gold coating on interior of central chamber, 4 microns
  CADMesh * vol07_mesh = new CADMesh("stl/IR_CENTRAL_CHAMBER_GOLD_COAT.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol07_solid = vol07_mesh->TessellatedMesh();
  G4LogicalVolume* vol07_logic = new G4LogicalVolume(vol07_solid, nist->FindOrBuildMaterial("G4_Au"), "vol07_logic", 0, 0, 0);
  G4VPhysicalVolume* vol07_phys = new G4PVPlacement(0, G4ThreeVector(), vol07_logic, "vol07_phys", logicWorld, false, 0, checkOverlaps);

  // d/s ele beam pipe after grey flange, straight end
  CADMesh * vol08_mesh = new CADMesh("stl/DS_ELECTRON_BASELINE_BEAMLINE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol08_solid = vol08_mesh->TessellatedMesh();
  G4LogicalVolume* vol08_logic = new G4LogicalVolume(vol08_solid, nist->FindOrBuildMaterial("G4_Al"), "vol08_logic", 0, 0, 0);
  G4VPhysicalVolume* vol08_phys = new G4PVPlacement(0, G4ThreeVector(), vol08_logic, "vol08_phys", logicWorld, false, 0, checkOverlaps);

  // d/s ion beam pipe after grey flange, straight end
  CADMesh * vol09_mesh = new CADMesh("stl/DS_ION_BASELINE_BEAMLINE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol09_solid = vol09_mesh->TessellatedMesh();
  G4LogicalVolume* vol09_logic = new G4LogicalVolume(vol09_solid, nist->FindOrBuildMaterial("G4_Al"), "vol09_logic", 0, 0, 0);
  G4VPhysicalVolume* vol09_phys = new G4PVPlacement(0, G4ThreeVector(), vol09_logic, "vol09_phys", logicWorld, false, 0, checkOverlaps);

  // Flange to NEG chamberr, ele d/s
  CADMesh * vol10_mesh = new CADMesh("stl/NEG_CHAMBER_E-DS_FLANGE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol10_solid = vol10_mesh->TessellatedMesh();
  G4LogicalVolume* vol10_logic = new G4LogicalVolume(vol10_solid, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "vol10_logic", 0, 0, 0);
  G4VPhysicalVolume* vol10_phys = new G4PVPlacement(0, G4ThreeVector(), vol10_logic, "vol10_phys", logicWorld, false, 0, checkOverlaps);

  // u/s ele beam pipe, afer grey flange, bell-shaped end
  CADMesh * vol11_mesh = new CADMesh("stl/US_ELECTRON_BASELINE_BEAMLINE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol11_solid = vol11_mesh->TessellatedMesh();
  G4LogicalVolume* vol11_logic = new G4LogicalVolume(vol11_solid, nist->FindOrBuildMaterial("G4_Al"), "vol11_logic", 0, 0, 0);
  G4VPhysicalVolume* vol11_phys = new G4PVPlacement(0, G4ThreeVector(), vol11_logic, "vol11_phys", logicWorld, false, 0, checkOverlaps);

  // u/s ion beam pipe after grey flange, straight end
  CADMesh * vol12_mesh = new CADMesh("stl/US_ION_BASELINE_BEAMLINE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol12_solid = vol12_mesh->TessellatedMesh();
  G4LogicalVolume* vol12_logic = new G4LogicalVolume(vol12_solid, nist->FindOrBuildMaterial("G4_Al"), "vol12_logic", 0, 0, 0);
  G4VPhysicalVolume* vol12_phys = new G4PVPlacement(0, G4ThreeVector(), vol12_logic, "vol12_phys", logicWorld, false, 0, checkOverlaps);

  //Outer layer for water chamber
  CADMesh * vol13_mesh = new CADMesh("stl/IP_WATER_TUBE.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol13_solid = vol13_mesh->TessellatedMesh();
  G4LogicalVolume* vol13_logic = new G4LogicalVolume(vol13_solid, nist->FindOrBuildMaterial("G4_Be"), "vol13_logic", 0, 0, 0);
  G4VPhysicalVolume* vol13_phys = new G4PVPlacement(0, G4ThreeVector(), vol13_logic, "vol13_phys", logicWorld, false, 0, checkOverlaps);

  // Endcap 1 for water chamber
  CADMesh * vol14_mesh = new CADMesh("stl/IP_WATER_TUBE_END1.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol14_solid = vol14_mesh->TessellatedMesh();
  G4LogicalVolume* vol14_logic = new G4LogicalVolume(vol14_solid, nist->FindOrBuildMaterial("G4_Be"), "vol14_logic", 0, 0, 0);
  G4VPhysicalVolume* vol14_phys = new G4PVPlacement(0, G4ThreeVector(), vol14_logic, "vol14_phys", logicWorld, false, 0, checkOverlaps);

  // Endcap 2 for water chamber
  CADMesh * vol15_mesh = new CADMesh("stl/IP_WATER_TUBE_END2.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol15_solid = vol15_mesh->TessellatedMesh();
  G4LogicalVolume* vol15_logic = new G4LogicalVolume(vol15_solid, nist->FindOrBuildMaterial("G4_Be"), "vol15_logic", 0, 0, 0);
  G4VPhysicalVolume* vol15_phys = new G4PVPlacement(0, G4ThreeVector(), vol15_logic, "vol15_phys", logicWorld, false, 0, checkOverlaps);

  // Balloon of interior up to flange on bodycut, does not include NEG chamber flange nor ele/ion baseline beampipe
  CADMesh * vol16_mesh = new CADMesh("stl/JL0081018.stl", mm, G4ThreeVector(0, 0, 0), false);
  G4VSolid* vol16_solid = vol16_mesh->TessellatedMesh();
  G4LogicalVolume* vol16_logic = new G4LogicalVolume(vol16_solid, nist->FindOrBuildMaterial("G4_Galactic"), "vol16_logic", 0, 0, 0);
  G4VPhysicalVolume* vol16_phys = new G4PVPlacement(0, G4ThreeVector(), vol16_logic, "vol16_phys", logicWorld, false, 0, checkOverlaps);

  sensitives.insert(vol01_phys);
  sensitives.insert(vol02_phys);





  char abname[256];

  //--------------------------------------------------
  //----------vtx barrel ladder geometry--------------
  //--------------------------------------------------
  //===================================================================================

  G4RotationMatrix rm[10][40];
  // deltaphi1 = 0;

  double x, y, z;
  z = 0 * cm;
  //phi=26.*deg; x=0; y=0; z=fAbsorberZ;
  //phi=0.*deg; x=0; y=0; z=fAbsorberZ;

  int FDIV = 0;
  double dR;
  double myL;
  double phi=0;

  // Materials are defined in JLeicMaterials
  // TODO calling global stuff like that kills kittens, so think about it later!
  G4Material* cb_VTX_ladder_Material = nist->FindOrBuildMaterial("G4_Si");


  std::vector <cb_VTX_ladder_LayParam> Lays;
  cb_VTX_ladder_LayParam Lay;
  // Lay 0
  Lay.Dx=0.050 * mm;
  Lay.Dy=2*cm;
  Lay.Dz=10*cm;
  Lay.Rin=3.5 * cm;
  Lays.push_back(Lay);
  // Lay 1
  Lay.Dx=0.050 * mm;
  Lay.Dy=2*cm;
  Lay.Dz=11*cm;
  Lay.Rin=4.5 * cm;
  Lays.push_back(Lay);
  // Lay 2
  Lay.Dx=0.150 * mm;
  Lay.Dy=4*cm;
  Lay.Dz=18*cm;
  Lay.Rin=6.5 * cm;
  Lays.push_back(Lay);
  // Lay 3
  Lay.Dx=0.150 * mm;
  Lay.Dy=4*cm;
  Lay.Dz=24*cm;
  Lay.Rin=10.5 * cm;
  Lays.push_back(Lay);
  // Lay 4
  Lay.Dx=0.150 * mm;
  Lay.Dy=4*cm;
  Lay.Dz=36*cm;
  Lay.Rin=13.5 * cm;
  Lays.push_back(Lay);
  // Lay 5
  Lay.Dx=0.150 * mm;
  Lay.Dy=4*cm;
  Lay.Dz=48*cm;
  Lay.Rin=15.5 * cm;
  Lays.push_back(Lay);

  if( Lays.size()>10) {
    printf("Nlayers in VERTEX >10 !!! \n");
    exit(1);
  }


  G4LogicalVolume* cb_VTX_ladder_Logic[10];
  G4Box *cb_VTX_ladder_Solid[10];

  G4LogicalVolume *pxdSlice_log[10]; //pointer to the logical slice
  G4LogicalVolume *pxdPixel_log[10]; //pointer to the logical pixel


  for (uint lay = 0; lay < Lays.size(); lay++) {

    printf("cb_VTX_ladder:: Layer loop:: %d\n", lay);
    double cb_VTX_ladder_DZ = Lays[lay].Dz;
    double cb_VTX_ladder_DY = Lays[lay].Dy;
    double cb_VTX_ladder_Thickness = Lays[lay].Dx;
    dR =  Lays[lay].Rin;

    myL = 2*3.1415*dR;
    int NUM = myL/cb_VTX_ladder_DY;

    for(int i=0; i<2; i++) {
      double LN = cb_VTX_ladder_DY * NUM;
      double LN1 = cb_VTX_ladder_DY * (NUM+1+i);
      printf("cb_VTX_ladder:: LN= Orig NUM=%d\n",NUM);
      if (LN/LN1>0.8) NUM=NUM+1;
      printf("cb_VTX_ladder:: LN=%f, LN1=%f  delenie=%f NUM=%d \n",LN,LN1,LN/LN1,NUM);
    }

    double cb_VTX_ladder_deltaphi = 2*3.1415926/NUM  ;


    sprintf(abname, "cb_VTX_ladder_Solid_%d", lay);
    cb_VTX_ladder_Solid[lay] = new G4Box(abname, cb_VTX_ladder_Thickness / 2.,   cb_VTX_ladder_DY / 2.,cb_VTX_ladder_DZ / 2.  );

    sprintf(abname, "cb_VTX_ladder_Logic_%d", lay);
    cb_VTX_ladder_Logic[lay] = new G4LogicalVolume(cb_VTX_ladder_Solid[lay], cb_VTX_ladder_Material, abname);


    G4VisAttributes *attr_cb_VTX_ladder;
    if (lay == 0 || lay == 1) {
      attr_cb_VTX_ladder = new G4VisAttributes(G4Color(0.0, 0.2, 0.8, 2.0));
    }
    else if (lay == 2) {
      attr_cb_VTX_ladder = new G4VisAttributes(G4Color(0.0, 0.2, 0.8, 0.7));
    }
    else {
      attr_cb_VTX_ladder = new G4VisAttributes(G4Color(0.0 + 0.1 * double(lay - 3), 1., 1. - 0.1 * double(lay - 3), 1.0));
    }
    attr_cb_VTX_ladder->SetForceSolid(true);
    cb_VTX_ladder_Logic[lay]->SetVisAttributes(attr_cb_VTX_ladder);

    if( NUM>40) {
      printf("cb_VTX_ladder:: Nladders in VERTEX >40 lay=%d !!! \n",lay);
      exit(1);
    }


    for (int ia = 0; ia < NUM; ia++) {
      //for (int ia=0;ia<1;ia++) {
      printf("cb_VTX_ladder:: lay=%d  NUM=%d, dR=%f cb_VTX_ladder_deltaphi=%f %f \n",lay, NUM,  dR, cb_VTX_ladder_deltaphi,ladder_deltashi);
      printf("cb_VTX_ladder:: Module  loop:: %d\n", ia);

      phi = (ia * (cb_VTX_ladder_deltaphi));
      x = - dR * cos(phi) ;
      y = - dR * sin(phi) ;
      rm[lay][ia].rotateZ(cb_VTX_ladder_deltaphi * ia);
      rm[lay][ia].rotateZ(ladder_deltashi);

      printf("cb_VTX_ladder::  %d %d x=%f  y=%f  \n", lay, ia, x, y);
      sprintf(abname, "cb_VTX_ladder_Phys_%d_%d", lay, ia);
      G4VPhysicalVolume* cb_VTX_ladder_Phys = new G4PVPlacement(G4Transform3D(rm[lay][ia], G4ThreeVector(x, y, z)),
          abname, cb_VTX_ladder_Logic[lay], physWorld, false, 0., checkOverlaps);
      //sensitives.insert(cb_VTX_ladder_Phys);
    }
    //-------------------------------------------------------------------------
    //                          VTX  slices and pixels
    //-------------------------------------------------------------------------
    G4Box *pxdBox_slice[10];
    G4Box *pxdBox_pixel[10];
    G4double PixelDX, PixelDY;
    if (lay < 2) {
      PixelDX = cb_VTX_ladder_DZ / 10.; //2000.*um;
      PixelDY = cb_VTX_ladder_DY / 50.; //2000.*um;
    } else {
      PixelDX = cb_VTX_ladder_DZ / 50.; //2000.*um;
      PixelDY = cb_VTX_ladder_DY / 10.; //2000.*um;

    }
    //G4double PixelDX=20.*um;
    //G4double PixelDY=20.*um;
    //G4double PixelDX=24.*um;
    //G4double PixelDY=24.*um;
    G4double PixelDZ = cb_VTX_ladder_Thickness; // 0.450*mm

    if (FDIV >= 1) {
      printf("cb_VTX_ladder_pxdSlice_:: construct slices %d \n", lay);

      sprintf(abname, "cb_VTX_ladder_pxdSlice_%d", lay);
      pxdBox_slice[lay] = new G4Box(abname,
                                    PixelDX / 2,                   //gD->GetPixelDX(),
                                    cb_VTX_ladder_DY / 2., // 10.*mm,  //gD->GetHalfMPXWaferDY(),
                                    cb_VTX_ladder_Thickness / 2.);    //gD->GetHalfMPXWaferDZ());

      pxdSlice_log[lay] = new G4LogicalVolume(pxdBox_slice[lay], cb_VTX_ladder_Material, abname, 0, 0, 0);

      G4VisAttributes *pixelVisAtt = new G4VisAttributes(G4Color(0, 1, 1, 1));
      pixelVisAtt->SetLineWidth(1);
      pixelVisAtt->SetForceWireframe(true);
      pxdSlice_log[lay]->SetVisAttributes(pixelVisAtt);


      // divide in slices
      sprintf(abname, "pxdSlice_%d", lay);
      G4PVDivision *sliceDiv = new G4PVDivision(abname,
          pxdSlice_log[lay],
          cb_VTX_ladder_Logic[lay],
          kXAxis,
          PixelDX,
          0);
      printf("SetUpVertex16():: construct done\n");


      if (FDIV >= 2) {
        printf("SetUpVertex16():: construct pixels \n");
        if (lay < 2) {
          sprintf(abname, "pxdPixel");
        }
        else {
          sprintf(abname, "svdPixel");
        }

        //sprintf(abname,"pxdPixel_%d",lay);
        pxdBox_pixel[lay] = new G4Box(abname,
                                      PixelDX / 2,
                                      PixelDY / 2.,
                                      PixelDZ / 2.);
        pxdPixel_log[lay] = new G4LogicalVolume(pxdBox_pixel[lay], cb_VTX_ladder_Material, abname, 0, 0, 0);
        pxdPixel_log[lay]->SetVisAttributes(pixelVisAtt);

        // divide in pixels
        G4PVDivision *pixelDiv = new G4PVDivision(abname,
            pxdPixel_log[lay],
            pxdSlice_log[lay],
            kYAxis,
            PixelDY,
            0);
      } //-- end if pixel division
    } //-- end if slices division

    //	 };  // -- end loop over modules

  }; // --- end loop over layers


  return physWorld;
}
