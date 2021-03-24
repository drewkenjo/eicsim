#include "IRDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
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
  G4bool checkOverlaps = false;

  G4double world_sizeXY = 3*m;
  G4double world_sizeZ  = 120*m;

  double rr = 8.31446261815324;
  double roomtemp = 293*kelvin;

  double h2den = 1e-1*2.01588*g/(rr*roomtemp)/m3;
  G4Element* elH = nist->FindOrBuildElement("H");
  G4Material* H2mat = new G4Material("hydrogen",h2den,1,kStateGas,roomtemp);
  H2mat->AddElement(elH, 2);

  double coden = 1e-1*28.01*g/(rr*roomtemp)/m3;
  G4Element* elC = nist->FindOrBuildElement("C");
  G4Element* elO = nist->FindOrBuildElement("O");
  G4Material* COmat = new G4Material("co",coden,2,kStateGas,roomtemp);
  COmat->AddElement(elC, 1);
  COmat->AddElement(elO, 1);

  double vacden = 0.95*h2den + 0.05*coden;
  G4Material* vacuum = new G4Material("vacuum",vacden,2,kStateGas,roomtemp);
  vacuum->AddMaterial(H2mat, 0.95*h2den/vacden);
  vacuum->AddMaterial(COmat, 0.05*coden/vacden);

  G4Box* solidWorld = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, vacuum, "World");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);


  std::string vols[][2] = {
//    {"stl/Detector_chamber_-_beryllium_section.stl", "G4_Be"},			// 1
    {"stl/Detector_chamber_-_electron_exit_round.stl", "G4_W"},			// 2
    {"stl/Detector_chamber_-_hadron_entrance_round.stl", "G4_Al"},		// 3
    {"stl/Detector_chamber_-_hadron_forward.stl", "G4_W"},				// 4
    {"stl/Detector_chamber_-_inner_hadron_forward.stl", "G4_Al"},		// 5
    {"stl/Detector_chamber_-_inner_hadron_rear_round.stl", "G4_Al"},		// 6
    {"stl/DN120_CF_blank.stl", "G4_STAINLESS-STEEL"},					// 7
    {"stl/DN120_CF_blank001.stl", "G4_STAINLESS-STEEL"},				// 8
    {"stl/DN50_hadron_rear_flange.stl", "G4_STAINLESS-STEEL"},			// 9
    {"stl/PI-FLG-0061.stl", "G4_STAINLESS-STEEL"},					// 10
    {"stl/PI-FLG-0062-1.stl", "G4_STAINLESS-STEEL"},					// 11
    //{"stl/PI-FLG-0062-2.stl", "G4_STAINLESS-STEEL"},
    {"stl/PI-FLG-0068-01.stl", "G4_STAINLESS-STEEL"},					// 12
    {"stl/PI-FLG-0068-02.stl", "G4_STAINLESS-STEEL"},					// 13
    {"stl/PI-FLG-0400.stl", "G4_STAINLESS-STEEL"},					// 14
    {"stl/upstream_0.stl", "G4_W"},								// 15
    {"stl/upstream_1.stl", "G4_Al"},								// 16
    {"stl/upstream_2.stl", "G4_Al"}								// 17
  };

  int ivol = 0;
  for(auto vol: vols) {
    ivol++;

    char vname[100];
    sprintf(vname, "vol%02d", ivol);
    std::string vstr(vname);

    auto vol_mesh = CADMesh::TessellatedMesh::FromSTL(vol[0]);
    auto vol_sol = vol_mesh->GetSolid();
    auto vol_log = new G4LogicalVolume(vol_sol, nist->FindOrBuildMaterial(vol[1]), vstr+"_log");
    auto vol_phy = new G4PVPlacement(0, G4ThreeVector(), vol_log, vstr+"_phy", logicWorld, false, 0, checkOverlaps);

//    if(ivol==1)
//      sensitives[vol_phy] = 1;
  }


  auto endSol = new G4Tubs("endSol", 50*mm, 51*mm, 3*m, 0*deg, 360*deg);
  auto endLog = new G4LogicalVolume(endSol, nist->FindOrBuildMaterial("G4_Al"), "endLog");
  auto endPhy = new G4PVPlacement(0, G4ThreeVector(0,0,-35.5*m), endLog, "endPhy", logicWorld, false, 0, checkOverlaps);

  auto be_sol = new G4Tubs("be_sol", 31*mm, 32*mm, 734.05*mm, 0*deg, 360*deg);
  auto be_log = new G4LogicalVolume(be_sol, nist->FindOrBuildMaterial("G4_Be"), "be_log");
  auto be_phy = new G4PVPlacement(0, G4ThreeVector(0,0,64.05*mm), be_log, "be_phy", logicWorld, false, 0, checkOverlaps);
  sensitives[be_phy] = 1;


  auto au_sol = new G4Tubs("au_sol", 31*mm-2*um, 31*mm, 734.05*mm, 0*deg, 360*deg);
  auto au_log = new G4LogicalVolume(au_sol, nist->FindOrBuildMaterial("G4_Au"), "au_log");
  auto au_phy = new G4PVPlacement(0, G4ThreeVector(0,0,64.05*mm), au_log, "au_phy", logicWorld, false, 0, checkOverlaps);
  sensitives[au_phy] = 2;



/*
  sensitives[vol04_phys] = 4;
  sensitives[vol06_phys] = 6;
  sensitives[vol07_phys] = 7;
  sensitives[vol13_phys] = 13;
  sensitives[vol14_phys] = 14;
  sensitives[vol15_phys] = 15;

*/




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


  for (uint lay = 1; lay < Lays.size(); lay++) {

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
      double m0 = cb_VTX_ladder_Logic[lay]->GetMass()/g;
      printf("cb_VTX_ladder:: lay=%d  NUM=%d/%d, dR=%f, mass=%f/%f, cb_VTX_ladder_deltaphi=%f %f \n",lay, ia, NUM,  dR, m0,m0*NUM, cb_VTX_ladder_deltaphi,ladder_deltashi);
      printf("cb_VTX_ladder:: size:: %f %f %f\n", cb_VTX_ladder_Thickness, cb_VTX_ladder_DY, cb_VTX_ladder_DZ);
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
      sensitives[cb_VTX_ladder_Phys] = (lay+1)*100 + ia;
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
