#include "SDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4Para.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"
#include "G4NistManager.hh"
#include "SMagField.hh"
#include "globals.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4SDManager.hh"
#include "KyotoScintiSD.hh"
#include "KatanaVScintiSD.hh"
#include "KatanaMScintiSD.hh"
#include "SpiRITSD.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "SRegionInformation.hh"

SDetectorConstruction::SDetectorConstruction()
  : G4VUserDetectorConstruction(),   // initialize member variable
fHall_Log(0), fYoke_Log(0), fCramp_Log(0), fVC_Log(0), fVCInside_Log(0), 
fTPCEnc_Log(0), fTPCInside_Log(0), fFC_Log(0), fTarget_Log(0),
fKapton_Log(0), fMylar_Log(0), fTPCSensitive_Log(0), 
fKyotoPla_Log(0), fKatanaVeto_Log(0), fKatanaM_Log(0) 
{
#include "SDetectorParameterDef.icc"
  G4cout << "Constructor of SDetectorConstruction." << G4endl;
  DefineMaterials();
}

SDetectorConstruction::~SDetectorConstruction()
{

  G4cout << "Destructor of SDetectorConstruction." << G4endl;
  ClearMaterials();
}

G4VPhysicalVolume* SDetectorConstruction::Construct()
{
  // construct experimental hall with geometrical setups(detector, spectrometer, etc.) 
  std::cout<<"called SDetectorConstrucion::Construct()"<<std::endl;

  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //---- experimental hall (world volume)
  //---- beam line along z axis

  auto Hall_Box = new G4Box("Hall_Box",fExpHall_x/2.,fExpHall_y/2.,fExpHall_z/2.);
  fHall_Log = new G4LogicalVolume(Hall_Box,fVacuum,"Hall_Log");
  //fHall_Log = new G4LogicalVolume(Hall_Box,fAirMaterial,"Hall_Log");
  auto Hall_Phys = new G4PVPlacement(0,G4ThreeVector(),
                                      fHall_Log,"Hall_Phys",
                                      0,false,0);
  G4Region* defaultRegion = (*(G4RegionStore::GetInstance()))[0];
  auto defaultRInfo = new SRegionInformation();
  defaultRInfo->SetWorld();
  defaultRInfo->Print();
  defaultRegion->SetUserInformation(defaultRInfo);

  
  ConstructSAMURAI();
  ConstructVacuumChamber();
  ConstructTPCenc();
  ConstructFieldCage();
  ConstructTarget();
  ConstructKyotoArray();
  ConstructKATANA();
  
  // change visualization (Only invisible or not)
  fYoke_Log->SetVisAttributes(G4VisAttributes::Invisible);
  fCramp_Log->SetVisAttributes(G4VisAttributes::Invisible);
  fVC_Log->SetVisAttributes(G4VisAttributes::Invisible);
  //fTPCenc_Log->SetVisAttributes(G4VisAttributes::Invisible);
  //fFC_Log->SetVisAttributes(G4VisAttributes::Invisible);
  //fTarget_Log->SetVisAttributes(G4VisAttributes::Invisible);
  //fMylar_Log->SetVisAttributes(G4VisAttributes::Invisible);
  //fKapton_Log->SetVisAttributes(G4VisAttributes::Invisible);
  //fKyotoPla_Log->SetVisAttributes(G4VisAttributes::Invisible);
  //fKatanaVeto_Log->SetVisAttributes(G4VisAttributes::Invisible);
  //fKatanaM_Log->SetVisAttributes(G4VisAttributes::Invisible);
  
  
  G4SDManager* SDmanager = G4SDManager::GetSDMpointer();
  auto kyotoSD   = new KyotoScintiSD("/det/Kyoto");
  auto katanavSD = new KatanaVScintiSD("/det/KatanaV");
  auto katanamSD = new KatanaMScintiSD("/det/KatanaM");
  SDmanager->AddNewDetector(kyotoSD);
  SDmanager->AddNewDetector(katanavSD);
  SDmanager->AddNewDetector(katanamSD);
  fKatanaVeto_Log->SetSensitiveDetector(katanavSD);
  fKatanaM_Log->   SetSensitiveDetector(katanamSD);
  fKyotoPla_Log->  SetSensitiveDetector(kyotoSD);
  
  //TPCSD* tpcsd = new TPCSD("/det/TPC");
  auto spiritSD = new SpiRITSD("/det/SpiRIT");
  SDmanager->AddNewDetector(spiritSD);
  fTPCSensitive_Log->SetSensitiveDetector(spiritSD);
    
  SetMagField();
  
  /*
  // limit for step visualization //

  G4double maxStep = 1*cm;
  G4UserLimits* stepLimit = new G4UserLimits(maxStep);
  //  Hall_Log->SetUserLimits(stepLimit);
  //  vchamber_log->SetUserLimits(stepLimit);
*/

  return Hall_Phys;
}

#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4TransportationManager.hh"

void SDetectorConstruction::SetMagField()
{
  G4FieldManager* fieldMgr = 
    G4TransportationManager::GetTransportationManager()->GetFieldManager();

  //  if(magField) delete magField;         //delete the existing magn field
  auto magField = new SMagField();
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  fieldMgr->GetChordFinder()->SetDeltaChord(0.001*mm); // 10 micron m order accuracy
}

void SDetectorConstruction::DefineMaterials(){

  G4NistManager* nistMan = G4NistManager::Instance();
  
  // vacuum
  fVacuum = nistMan->FindOrBuildMaterial("G4_Galactic");
  //fVacuum = new G4Material("fVacuum", 1., 1.08*g/mole, universe_mean_density, kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // P10 
  fP10gas = new G4Material("fP10gas", 1.56*mg/cm3, 2);
  G4Material* fCH4  = nistMan->FindOrBuildMaterial("G4_METHANE");
  G4Material* fAr   = nistMan->FindOrBuildMaterial("G4_Ar");
  fP10gas->AddMaterial(fAr,0.9);
  fP10gas->AddMaterial(fCH4,0.1);

  // Kapton
  fKapton = nistMan->FindOrBuildMaterial("G4_KAPTON");
  
  // G10 for fieldcage 
  fG10 = new G4Material("fG10",1.70*g/cm3,4);
  G4Element* H   = nistMan->FindOrBuildElement(1);
  G4Element* C   = nistMan->FindOrBuildElement(6);
  G4Element* O   = nistMan->FindOrBuildElement(8);
  G4Element* Si  = nistMan->FindOrBuildElement(14);
  fG10->AddElement(H,3);
  fG10->AddElement(C,3);
  fG10->AddElement(O,2);
  fG10->AddElement(Si,1);

  //Mylar
  fMylar = nistMan->FindOrBuildMaterial("G4_MYLAR");
  /*
  fMylar = new G4Material("fMylar", 1.39*g/cm3,3);
  fMylar->AddElement(H,4);
  fMylar->AddElement(C,5);
  fMylar->AddElement(O,2);
*/
  // Kyoto extruded scintillator
  G4Material* polystyrene = nistMan->FindOrBuildMaterial("G4_POLYSTYRENE");
  G4Material* ppo = new G4Material("ppo",1.08*g/cm3,3);
  ppo->AddElement(H,8);
  ppo->AddElement(C,8);
  ppo->AddElement(O,1);
  fKyotoPlaMaterial = new G4Material("fKyotoPlaMaterial",1.05*g/cm3,2);
  fKyotoPlaMaterial->AddMaterial(polystyrene,0.99);
  fKyotoPlaMaterial->AddMaterial(ppo,0.01);

  fAirMaterial       = nistMan->FindOrBuildMaterial("G4_AIR");
  fPlasticMaterial   = nistMan->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  fFe = nistMan->FindOrBuildMaterial("G4_Fe");
  fAl = nistMan->FindOrBuildMaterial("G4_Al");
  fSn = nistMan->FindOrBuildMaterial("G4_Sn");
}

void SDetectorConstruction::ClearMaterials()
{
  delete fVacuum;
  delete fP10gas;
  delete fKapton;
  delete fMylar;
  delete fG10;
  delete fAirMaterial;
  delete fPlasticMaterial;
  delete fKyotoPlaMaterial;
  delete fFe;
  delete fAl;
  delete fSn;
}

void SDetectorConstruction::ConstructSAMURAI(){

  // SAMURAI yoke
  auto Yoke_Box = new G4Box("Yoke_Box",
                             fSmYoke_x/2.,fSmYoke_y/2.,fSmYoke_z/2.);

  // empty space for vacuum chamber in SAMURAI
  auto YokeInside_Box = new G4Box("YokeInside_Box",
                                   fSmYokeIn_x/2.,fSmYokeIn_y/2.,fSmYokeIn_z/2.);
  // empty space entrance part
  auto YokeInsideEnt_Trd = new G4Trd("YokeInsideEnt_Trd",  
                                     fSmYokeInTrd_x/2., fSmYokeIn_x/2., 
                                     fSmYokeIn_y/2., fSmYokeIn_y/2.,
                                     fSmYokeInTrd_z/2.);
  G4ThreeVector YokeInsideEntTrdPos(0.,0.,-0.6*m-fSmYokeInTrd_z/2.);
  auto YokeInside_Uni = new G4UnionSolid("YokeInside_Uni",
                                          YokeInside_Box,YokeInsideEnt_Trd,
                                          0, YokeInsideEntTrdPos);
  
  auto YokeInsideExit_Trd = new G4Trd("YokeInsideExit_Trd", 
                                       fSmYokeIn_x/2., fSmYokeInTrd_x/2., 
                                       fSmYokeIn_y/2., fSmYokeIn_y/2.,
                                       fSmYokeInTrd_z/2.);
  G4ThreeVector YokeInsideExitTrdPos(0.,0.,0.6*m+fSmYokeInTrd_z/2.);
  YokeInside_Uni = new G4UnionSolid("YokeInside_Uni",
                                      YokeInside_Uni,YokeInsideExit_Trd,
                                      0, YokeInsideExitTrdPos);
  // side hole
  auto SideHole_Box = new G4Box("SideHole_Box",
                                 fSmSideHole_x/2.,fSmSideHole_y/2.,fSmSideHole_z/2.);
  YokeInside_Uni = new G4UnionSolid("YokeInside_Uni",YokeInside_Uni,SideHole_Box);
  // finish to make the empty space which is going to be gotten rid of.
  
  // get empty space for vacuum chamber (each local origin is same)
  auto Yoke_Sub = new G4SubtractionSolid("Yoke_Sub",Yoke_Box,YokeInside_Uni);
  // set "Yoke_Sub" as a logical volume of SAMURAI yoke.
  fYoke_Log = new G4LogicalVolume(Yoke_Sub,fFe,"Yoke_Log");
  // set the SAMURAI at the center of the experimental hall
  new G4PVPlacement(0,G4ThreeVector(),fYoke_Log,"Yoke_Phys",fHall_Log,false,0);

  // cramp for neutron windows(neutron windows are removed at TPC exp.)
  auto Cramp_Box = new G4Box("Cramp_Box",
                              fCramp_x/2.,fCramp_y/2.,fCramp_z/2.);

  fCramp_Log = new G4LogicalVolume(Cramp_Box,fAl,"Cramp_Box");

  G4ThreeVector Cramp_Pos(0, fSmYokeIn_y/2.+fCramp_y/2., fSmYoke_z/2.+fCramp_z/2.);
  G4int icramp=0; 
  new G4PVPlacement(0,G4ThreeVector(0,Cramp_Pos.y(),Cramp_Pos.z()),
                    fCramp_Log,"Cramp_Phys",fHall_Log,false,icramp++); // downstream up
  new G4PVPlacement(0,G4ThreeVector(0,-Cramp_Pos.y(),Cramp_Pos.z()),
                    fCramp_Log,"Cramp_Phys",fHall_Log,false,icramp++); // downstream bottom
  new G4PVPlacement(0,G4ThreeVector(0,Cramp_Pos.y(),-Cramp_Pos.z()),
                    fCramp_Log,"Cramp_Phys",fHall_Log,false,icramp++); // upstream up
  new G4PVPlacement(0,G4ThreeVector(0,-Cramp_Pos.y(),-Cramp_Pos.z()),
                    fCramp_Log,"Cramp_Phys",fHall_Log,false,icramp++); // upstream bottom


  //  ------- SAMURAI magnet and cramps are finished to be placed. --------------- 

  auto Yoke_VisAtt = new G4VisAttributes(G4Colour(0,0,1,0.5));
  //Yoke_VisAtt->SetForceWireframe(true);
  Yoke_VisAtt->SetForceSolid(true);
  fYoke_Log->SetVisAttributes(Yoke_VisAtt);
  auto Cramp_VisAtt = new G4VisAttributes(G4Colour(0.3,0.3,0.3,0.3));
  //Cramp_VisAtt->SetForceWireframe(true);
  Cramp_VisAtt->SetForceSolid(true);
  fCramp_Log->SetVisAttributes(Cramp_VisAtt);
}

void SDetectorConstruction::ConstructVacuumChamber(){

  // inside box part
  // enclosure part of vchamber (box)
  auto VCEnc_Box = new G4Box("VCEnc_Box",  
                              fVC_x/2.,fVC_y/2.,fVC_z/2.);
  auto VCEnc_Trd = new G4Trd("VCEnc_Trd",
                             fVC_x/2., fVCTrd_x/2.,
                             fVC_y/2., fVC_y/2.,
                             fVCTrd_z/2.);
  // VCEnc_Trd position related to VCEnc_Box
  G4ThreeVector VCEncTrdPos(0.,0.,0.6*m+fVCTrd_z/2.); 
  auto VCEnc_Uni = new G4UnionSolid("VCEnc_Uni",
                                    VCEnc_Box, VCEnc_Trd,
                                    0,VCEncTrdPos);
  
  auto VCHollow_Box = new G4Box("VCHollow_Box",  // inside hollow part
                                 fVCIn_x/2., fVCIn_y/2., fVC_z/2.+0.5*m); // *0.5*m: not to overlap the plane
  auto VCHollow_Trd = new G4Trd("VCHollow_Trd",
                                fVCIn_x/2., fVCTrd_x/2.,
                                fVCIn_y/2., fVCIn_y/.2,
                                fVCTrd_z/2.);
  
  auto VC_Sub = new G4SubtractionSolid("VC_Sub",VCEnc_Uni,VCHollow_Box);
  G4ThreeVector VCHollowTrdPos(0.,0.,0.6*m+fVCTrd_z/2.+6.49*cm); // 6.49cm: shift for making 4cm thickness of side of VC trapezoid part.
  VC_Sub = new G4SubtractionSolid("VC_Sub",VC_Sub,VCHollow_Trd,
                                  0,VCHollowTrdPos);

  auto VCForwardPlate_Box = new G4Box("VCForwardPlate_Box",
                                       fVC_x/2.,fVC_y/2.,fVC_thickness/2.);
  auto VC_Uni = new G4UnionSolid("VC_Uni",
                                  VC_Sub,VCForwardPlate_Box,
                                  0,G4ThreeVector(0,0,-fVC_z/2.+fVC_thickness/2.));
 
  fVC_Log = new G4LogicalVolume(VC_Uni,fAl,"VC_Log");
  // set vacuum chamber
  new G4PVPlacement(0,G4ThreeVector(0,0,0),fVC_Log,"VC_Phys",fHall_Log,false,0);
  
  auto VC_VisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.1));
  VC_VisAtt->SetForceSolid(true);
  //VC_VisAtt->SetForceWireframe(true);
  fVC_Log->SetVisAttributes(VC_VisAtt);
  
  auto vacuumchamberRegion = new G4Region("VacuumChamberRegion");
  auto vacuumchamberRInfo  = new SRegionInformation();
  vacuumchamberRInfo->SetVacuumChamber();
  vacuumchamberRegion->SetUserInformation(vacuumchamberRInfo);
  fVC_Log->SetRegion(vacuumchamberRegion);
  vacuumchamberRegion->AddRootLogicalVolume(fVC_Log);

/*  
  VCBox_Log = new G4LogicalVolume(VCBox_Uni,fAl,"VCBox_Log");
  VCBox_Phys = new G4PVPlacement(0,G4ThreeVector(0,0,-0.2*m),
                                  VCBox_Log,"VCBox_Phys",Hall_Log,false,0);


  // vacuum chamber exit part
  G4Trd* VCExitTBPlate_Trd = new G4Trd("VCExitTBPlate_Trd", // vacuum chamber exit part: top&bottom plates
                                         VCEncBox_Size.x()/2., VCOut_x/2.,
                                         4./2.*cm, 4./2.*cm,
                                         VCTrd_z/2.);
  VCExitTBPlate_Log = new G4LogicalVolume(VCExitTBPlate_Trd,fAl,"VCExitTBPlate_Log");
  VCExitTopPlate_Phys = new G4PVPlacement(0,G4ThreeVector(0,0.4*m+4./2.*cm,0.6*m+VCTrd_z/2.),
                                          VCExitTBPlate_Log,"VCExitTopPlate_Phys",
                                          Hall_Log,false,0);
  VCExitBottomPlate_Phys = new G4PVPlacement(0,G4ThreeVector(0,-0.4*m-4./2.*cm,0.6*m+VCTrd_z/2.),
                                                    VCExitTBPlate_Log,"VCExitBottomPlate_Phys",
                                                    Hall_Log,false,0);

  G4Para* VCExitSidePlate_Para = new G4Para("VCExitSidePlate_Para", // side plates
                                              4./2.*cm,88./2.*cm,VCTrd_z/2.,
                                              0,std::atan(730./900.),0);
  
  // define logical volume & physicalvolume
  VCExitSidePlate_Log = new G4LogicalVolume(VCExitSidePlate_Para,fAl,"VCExitSidePlate_Log");
  VCExitLeftPlate_Phys = new G4PVPlacement(0,G4ThreeVector((1700.+2600.)/2.*mm,0,0.6*m+VCTrd_z/2.),
                                             VCExitSidePlate_Log,"VCExitLeftPlate_Phys",
                                             Hall_Log,false,0);
  G4RotationMatrix exitside_rm; exitside_rm.rotateZ(180.*deg);
  VCExitRightPlate_Phys = 
    new G4PVPlacement(G4Transform3D(exitside_rm,G4ThreeVector(-(1700.+2600.)/2.*mm,0,0.6*m+VCTrd_z/2.)),
                      VCExitSidePlate_Log,"VCExitRightPlate_Phys",
                      Hall_Log,false,0);



  // vacuum chamber exit window part 
  G4ThreeVector VCExitWindow_Size(5.76*m, (0.88+0.56)*m, 0.16*m);
  G4Box* VCExitWindow_Box = new G4Box("VCExitWindow_Box",
                                        VCExitWindow_Size.x()/2.,
                                        VCExitWindow_Size.y()/2.,
                                        VCExitWindow_Size.z()/2.);
  G4Box* VCExitWindowEmp_Box = new G4Box("VCExitWindowEmp_Box",
                                           VCOut_x/2.-0.08/2.*m,
                                           VCInsideBox_Size.y()/2.,
                                           VCExitWindow_Size.z()/2.+1.*m);
  G4SubtractionSolid* VCExitWindow_Sub = new G4SubtractionSolid("VCExitWindow_Sub",
                                                                 VCExitWindow_Box,
                                                                 VCExitWindowEmp_Box);

  VCExitWindow_Log = new G4LogicalVolume(VCExitWindow_Sub,fAl,"VCExitWindow_Log");
  VCExitWindow_Phys = new G4PVPlacement(0,G4ThreeVector(0,0,0.6*m+VCTrd_z+VCExitWindow_Size.z()/2.),
                                         VCExitWindow_Log,"VCExitWindow_Phys",
                                         Hall_Log,false,0); 
  
  VC_VisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.1));
  VC_VisAtt->SetForceSolid(true);
  //VC_VisAtt->SetForceWireframe(true);
  VCBox_Log->SetVisAttributes(VC_VisAtt);
  VCExitTBPlate_Log->SetVisAttributes(VC_VisAtt);
  VCExitSidePlate_Log->SetVisAttributes(VC_VisAtt);
  VCExitWindow_Log->SetVisAttributes(VC_VisAtt);
*/

  // inside of Vacuum chamber (mother volume for TPC etc.)
  // local origin is same with Hall.
  auto VCInside_Box = new G4Box("VCInside_Box",
                                 fVCIn_x/2.,fVCIn_y/2.,(fVC_z-fVC_thickness*2.)/2.); 
  G4Trd* VCInside_Trd = VCHollow_Trd;
  /*G4Trd* VCInside_Trd = new G4Trd("VCInside_Trd",
                                    VCIn_x/2., fVCTrd_x/2.,
                                    VCIn_y/2., VCIn_y/2.,
                                    VCTrd_z/2.);
  */
  auto VCInside_Uni = new G4UnionSolid("VCInside_Uni",
                                        VCInside_Box,VCInside_Trd,
                                        0,VCHollowTrdPos);
  //fVCInside_Log = new G4LogicalVolume(VCInside_Uni,fVacuum,"VCInside_Log");
  fVCInside_Log = new G4LogicalVolume(VCInside_Uni,fAirMaterial,"VCInside_Log");
  new G4PVPlacement(0,G4ThreeVector(),
                    fVCInside_Log,"VCInside_Phys",
                    fHall_Log,false,0);
  fVCInside_Log->SetVisAttributes(G4VisAttributes::Invisible);

}



void SDetectorConstruction::ConstructTPCenc(){
  // TPC enclosure
  auto TPCEnc_Box = new G4Box("TPCEnc_Box",
                               fTPCEnc_x/2.,fTPCEnc_y/2.,fTPCEnc_z/2.);
  G4ThreeVector TPCInsideSize(fTPCEnc_x-fTPCEnc_thickness*2.,
                              fTPCEnc_y-fTPCEnc_thickness*2.,
                              fTPCEnc_z-fTPCEnc_thickness*2.);
  auto TPCInside_Box = new G4Box("TPCInside_Box",
                                  TPCInsideSize.x()/2.,
                                  TPCInsideSize.y()/2.,
                                  TPCInsideSize.z()/2.);
  auto TPCEnc_Sub = new G4SubtractionSolid("TPCEnc_Sub",TPCEnc_Box,TPCInside_Box);

  // for the thin windows
  // side window part
  auto TPCSWindowEmp_Box = new G4Box("TPCSWindowEmp_Box",
                                       1600./2.*mm,
                                       410.97/2.*mm,
                                       1461.34/2.*mm);     //y&z: from INVENTOR information (Mizuki-san)
  // down stream part
  G4Box* TPCDSWindowEmp_Box = new G4Box("TPCDSWindowEmp_Box",
                                        1273.05/2.*mm,
                                        410.97/2.*mm,
                                        500./2.*mm);     // x&y: from INVENTOR information
  
  TPCEnc_Sub = new G4SubtractionSolid("TPCEnc_Sub",TPCEnc_Sub,TPCSWindowEmp_Box,
                                         0,G4ThreeVector(0,0,200.*mm));
  TPCEnc_Sub = new G4SubtractionSolid("TPCEnc_Sub",TPCEnc_Sub,TPCDSWindowEmp_Box,
                                         0,G4ThreeVector(0,0,1000.*mm));
  
  G4Box* TPCSWindow_Box = new G4Box("TPCSWindow_Box",
                                    0.51/2.*mm, 420./2.*mm, 1480./2.*mm);
  G4Box* TPCDSWindow_Box = new G4Box("TPCDSWindow_Box",
                                      1280./2.*mm, 420./2.*mm, 0.51/2.*mm);
  G4ThreeVector TPCLeftThinWindowPos(TPCInsideSize.x()/2.+1.*mm,0,200*mm);
  G4ThreeVector TPCRightThinWindowPos(-TPCInsideSize.x()/2.-1.*mm,0,200*mm);
  auto TPCEnc_Uni = new G4UnionSolid("TPCenc_Uni",TPCEnc_Sub,TPCSWindow_Box,
                                      0,TPCLeftThinWindowPos);
  TPCEnc_Uni = new G4UnionSolid("TPCEnc_Uni",TPCEnc_Uni,TPCSWindow_Box,
                                 0,TPCRightThinWindowPos);
  G4ThreeVector TPCDSThinWindowPos(0,0,TPCInsideSize.z()/2.+1.*mm);
  TPCEnc_Uni = new G4UnionSolid("TPCEnc_Uni",TPCEnc_Uni,TPCDSWindow_Box,
                                0,TPCDSThinWindowPos);
  
  fTPCEnc_Log = new G4LogicalVolume(TPCEnc_Uni,fAl,"TPCEnc_Log");
  auto TPCEnc_VisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.6));
  TPCEnc_VisAtt->SetForceSolid(true);
  //TPCEnc_VisAtt->SetForceWireframe(true);
  fTPCEnc_Log->SetVisAttributes(TPCEnc_VisAtt);

  G4double tpc_centerpos_x = 0./2.*mm;
  G4double offset_beamline_tpctop = 226.*mm;  // beam-pad plane distance: from the data analysis
  G4double tpc_centerpos_y = offset_beamline_tpctop-TPCInsideSize.y()/2.;   // -52 mm
  G4double offset_z0_tpcfor = 853.183*mm;
  G4double tpc_centerpos_z = -offset_z0_tpcfor+fTPCEnc_z/2.;    // -853.183+1004.9= 151.717 mm
  G4ThreeVector TPCEncPos(tpc_centerpos_x,tpc_centerpos_y,tpc_centerpos_z);
  // set TPC enclosure 
  new G4PVPlacement(0, TPCEncPos,fTPCEnc_Log, "TPCEnc_Phys",
                      fVCInside_Log, false, 0);

  // mother volume for FC and target. 
  //fTPCInside_Log = new G4LogicalVolume(TPCInside_Box,Vacuum,"TPCInside_Log") ; 
  fTPCInside_Log = new G4LogicalVolume(TPCInside_Box,fP10gas,"TPCInside_Log") ; 
  new G4PVPlacement(0,TPCEncPos,fTPCInside_Log, "TPCInside_Phys", 
                      fVCInside_Log, false, 0); 

}



void SDetectorConstruction::ConstructFieldCage(){
  // TPC field cage
  G4ThreeVector FC_Size(1000.*mm, 530.*mm, 1490.*mm);
  G4Box* FC_Box = new G4Box("FC_Box",fFC_x/2.,fFC_y/2.,fFC_z/2.);
  
  G4ThreeVector FCInsideSize(fFC_x-2.*fFC_thickness,532.*mm,fFC_z-2.*fFC_thickness); 
  G4Box* FCInside_Box = new G4Box("FCInside_Box",
                                    FCInsideSize.x()/2.,
                                    FCInsideSize.y()/2.,
                                    FCInsideSize.z()/2.);

  G4Box* FCEnt_Box = new G4Box("FCEnt_Box", // for mylar entrance window
                                 100./2.*mm,
                                 150./2.*mm,
                                 1.*cm);
  G4Box* FCExit_Box = new G4Box("FCExit_Box", // for kapton exit window
                                 (FC_Size.x()-50.)/2.*mm,
                                 (FC_Size.y()-50.)/2.*mm,
                                 1.*cm);
  G4ThreeVector FCEntBoxPos(0., 70.*mm,-FCInsideSize.z()/2.-fFC_thickness/2.);
  auto FCInside_Uni = new G4UnionSolid("FCInside_Uni",
                                        FCInside_Box, FCEnt_Box,
                                        0,FCEntBoxPos);
  G4ThreeVector FCExitBoxPos(0., 0.,FCInsideSize.z()/2.+fFC_thickness/2.);
  FCInside_Uni = new G4UnionSolid("FCInside_Uni",
                                   FCInside_Uni, FCExit_Box,
                                   0,FCExitBoxPos);

  auto FC_Sub = new G4SubtractionSolid("FC_Sub",FC_Box,FCInside_Uni);

  fFC_Log = new G4LogicalVolume(FC_Sub,fG10,"FiledCage_Log");
  auto FC_VisAtt = new G4VisAttributes(G4Colour(1.0,0.5,0.0));
  FC_VisAtt->SetForceSolid(true);
  fFC_Log->SetVisAttributes(FC_VisAtt);

  G4double offset_z0_tpcfor = 853.183*mm; // distance between z=0 and TPCenc forward
  G4double offset_z0_fcfor = 553.063*mm;  // distance between z=0 and FC forward
  G4double offset_tpcz0_fcfor = fTPCEnc_z/2. - offset_z0_tpcfor + offset_z0_fcfor; // 704.78 mm
  G4double fc_centerpos_z = -offset_tpcz0_fcfor + fFC_z/2.;   // 40.22 mm
  new G4PVPlacement(0, G4ThreeVector(0, 0, fc_centerpos_z), // relative coordinate to tpc enclosure frame
                     fFC_Log, "FC_Phys", fTPCInside_Log, false, 0); 
    
  //Mylar entering window & Kapton exit window
  G4Box* Mylar_Box = new G4Box("Mylar_Box",100./2.*mm,150./2.*mm,0.025/2.*mm);
  G4Box* Kapton_Box = new G4Box("Kapton_Box",(1000.-50.)/2.*mm,(530.-50.)/2.*mm,0.125/2.*mm);
  fMylar_Log = new G4LogicalVolume(Mylar_Box,fMylar,"Mylar_Log");
  fKapton_Log = new G4LogicalVolume(Kapton_Box,fKapton,"Kapton_Log");

  auto Mylar_VisAtt = new G4VisAttributes(G4Colour(1.0,0.5,0.4,0.2));
  auto Kapton_VisAtt = new G4VisAttributes(G4Colour(1.0,0.5,0.4,0.2));
  Mylar_VisAtt->SetForceSolid(true);
  Kapton_VisAtt->SetForceSolid(true);
  fMylar_Log->SetVisAttributes(Mylar_VisAtt);
  fKapton_Log->SetVisAttributes(Kapton_VisAtt);
  
  G4ThreeVector MylarPos(0.,70.*mm,-FCInsideSize.z()/2.-fFC_thickness/2.+fc_centerpos_z); // mylar position relative to the TPC_Inside
  new G4PVPlacement(0,MylarPos,
                    fMylar_Log,"Mylar_Phys",
                    fTPCInside_Log,false,0);
 
  G4ThreeVector KaptonPos(0.,0.,FCInsideSize.z()/2.+fFC_thickness/2.+fc_centerpos_z); // kapton position relative to the TPC_Inside
    new G4PVPlacement(0, KaptonPos,
                      fKapton_Log,"Kapton_Phys",
                      fTPCInside_Log,false,0);


  // for the sensitive area volume.
  G4Box* Sensitive_Box = new G4Box("Sensitive_Box",
                                  fPadPlane_x/2., fArGas_y/2., fPadPlane_z/2.);
  fTPCSensitive_Log = new G4LogicalVolume(Sensitive_Box,fP10gas,"TPCSensitive_Log");
  G4ThreeVector SensitivePos(0.,0.,fc_centerpos_z-FCInsideSize.z()/2.+fPadPlane_z/2.);
  new G4PVPlacement(0, SensitivePos,
                    fTPCSensitive_Log, "TPCSensitive_Phys", 
                    fTPCInside_Log, false, 0); 
    
}


void SDetectorConstruction::ConstructTarget(){

  G4double offset_beamline_tpctop = 226.*mm;  // from the data analysis
  G4double tpc_centerpos_y = offset_beamline_tpctop+2.*mm-fTPCEnc_y/2.;   // -52 mm
  G4double offset_z0_tpcfor = 853.183*mm;
  G4double offset_z0_fcfor = 553.063*mm;
  G4double offset_tpcz0_fcfor = fTPCEnc_z/2. - offset_z0_tpcfor + offset_z0_fcfor; // 704.78 mm
  //G4double tpc_centerpos_x = 0.*mm;
  //G4double tpc_centerpos_z = -offset_z0_tpcfor+fTPCEnc_z/2.;
  
  // Sn target
  // target position in TPCInside_Log coordinate.
  G4double tgtPos_x =   0.00*cm;
  G4double tgtPos_y =   -tpc_centerpos_y;
  G4double tgtPos_z =   -offset_tpcz0_fcfor-8.9*mm-fTarget_z/2.; // relative to z=0 of TPC frame
  //fTargetPos.set(tgtPos_x+tpc_centerpos_x, tgtPos_y+tpc_centerpos_y, tgtPos_z+tpc_centerpos_z);
  G4Box* Target_Box = new G4Box("Target_Box",
                                  fTarget_x/2.,fTarget_y/2.,fTarget_z/2.);
  fTarget_Log  = new G4LogicalVolume(Target_Box,fSn,"Target_Log");
  //fTarget_Log  = new G4LogicalVolume(Target_Box,fVacuum,"Target_Log");
  
  new G4PVPlacement(0,G4ThreeVector(tgtPos_x,tgtPos_y,tgtPos_z), 
                    fTarget_Log, "Target_Phys", fTPCInside_Log, false, 0);
  G4VisAttributes* Target_VisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  fTarget_Log->SetVisAttributes(Target_VisAtt);

}


void SDetectorConstruction::ConstructKyotoArray(){
  // Kyotoarray

  G4Box* KyotoPla_Box = new G4Box("KyotoPla_Box",
                                    fKyoto_thick/2.,fKyoto_length/2.,fKyoto_width/2.);
  fKyotoPla_Log = new G4LogicalVolume(KyotoPla_Box,fKyotoPlaMaterial,"KyotoPla_Log");
  
  
  G4double offset_z0_fcfor = 553.063*mm;

  G4double kyoto_xpos = 758.*mm;
  G4double kyoto_ypos = 226.-51.-fKyoto_length/2.;  // relative to vchamber coodinate (y=0: beamline)
  G4double kyoto_zpos = -offset_z0_fcfor+168.+fKyoto_width/2.; // relative to z=0

  int ikyoto_plastic =0;
  for(int i=0; i<30; i++){
    new G4PVPlacement(0,G4ThreeVector( kyoto_xpos, kyoto_ypos, kyoto_zpos),
        fKyotoPla_Log,"KyotoPla_Phys",fVCInside_Log,false,ikyoto_plastic++);    // beam left side 
    new G4PVPlacement(0,G4ThreeVector( -kyoto_xpos, kyoto_ypos, kyoto_zpos),
        fKyotoPla_Log,"KyotoPla_Phys",fVCInside_Log,false,ikyoto_plastic++);    // beam right side
    kyoto_zpos += 50.5*mm; 
  }

  printf("Num of Kyotoarray bars: %d\n",ikyoto_plastic);
  G4VisAttributes* KyotoPla_VisAtt = new G4VisAttributes(G4Colour(0.,0.,1.,0.7));
  KyotoPla_VisAtt->SetForceSolid(true);
  fKyotoPla_Log->SetVisAttributes(KyotoPla_VisAtt);
}

void SDetectorConstruction::ConstructKATANA(){
  // KATANAveto
  G4Box* KatanaVeto_Box = new G4Box("KatanaVeto_Box",
                                     fKatana_width/2.,
                                     fKatana_length/2.,
                                     fKatanaV_thick/2.);
  fKatanaVeto_Log = new G4LogicalVolume(KatanaVeto_Box,fPlasticMaterial,"KatanaVeto_Log");

  G4double offset_z0_fcfor = 553.063*mm;
  
  //G4double katanavetocenter_xpos = -246.*mm; // for 108Sn beam exp.
  G4double katanavetocenter_xpos = -215.*mm; // for 132Sn beam exp.
  G4double katana_ypos = 226.*mm-40.*mm-fKatana_length/2.;  // relative to vchamber coodinate (y=0: beamline)
  G4double katana_zpos = -offset_z0_fcfor+1867.*mm+fKatanaV_thick/2.; // relative to z=0
  G4double katanaveto_xoverlap = 12.*mm;
  G4double katanavetoright_xpos = katanavetocenter_xpos-fKatana_width+katanaveto_xoverlap;
  G4double katanavetoleft_xpos = katanavetocenter_xpos+fKatana_width-katanaveto_xoverlap;

  int ikatana_plastic =0;
  //center veto
  new G4PVPlacement(0,G4ThreeVector( katanavetocenter_xpos, katana_ypos, katana_zpos),
                    fKatanaVeto_Log,"KatanaVeto_Phys",
                    fVCInside_Log,false,ikatana_plastic++);
  // beam right
  new G4PVPlacement(0,G4ThreeVector( katanavetoright_xpos, katana_ypos, katana_zpos-10.*mm),
                    fKatanaVeto_Log,"KatanaVeto_Phys",
                    fVCInside_Log,false,ikatana_plastic++);
  // beam left
  new G4PVPlacement(0,G4ThreeVector( katanavetoleft_xpos, katana_ypos, katana_zpos-10.*mm),
                    fKatanaVeto_Log,"KatanaVeto_Phys",
                    fVCInside_Log,false,ikatana_plastic++);

  printf("Num of KATANAveto bars: %d\n",ikatana_plastic);
  G4VisAttributes* KatanaVeto_VisAtt = new G4VisAttributes(G4Colour(1.,0.,0.,0.7));
  KatanaVeto_VisAtt->SetForceSolid(true);
  fKatanaVeto_Log->SetVisAttributes(KatanaVeto_VisAtt);

  // KATANA Multiplicity
  G4Box* KatanaM_Box = new G4Box("KatanaM_box",
                                  fKatana_width/2.,
                                  fKatana_length/2.,
                                  fKatanaM_thick/2.);
  fKatanaM_Log = new G4LogicalVolume(KatanaM_Box,fPlasticMaterial,"KatanaM_Log");

  G4double katanaM_xpos = katanavetocenter_xpos-10.*mm-100.1*7.*mm; // M1 position
  G4double katanaM_ypos = katana_ypos;  // relative to vchamber coodinate (y=0: beamline)
  G4double katanaM_zpos = -offset_z0_fcfor+1867.*mm+fKatanaM_thick/2.; // relative to z=0

  int ikatanaM_plastic =0;
  for(int i=0; i<7; i++){
    new G4PVPlacement(0,G4ThreeVector( katanaM_xpos+(i*100.1)*mm, katanaM_ypos, katanaM_zpos),
        fKatanaM_Log,"KatanaM_Phys",
        fVCInside_Log,false,ikatanaM_plastic++);    // M1, M2, ... M7
  }
  for(int i=0; i<5; i++){
    new G4PVPlacement(0,G4ThreeVector( katanavetocenter_xpos+110.1*mm+(i*100.1)*mm, katanaM_ypos, katanaM_zpos),
        fKatanaM_Log,"KatanaM_Phys",
        fVCInside_Log,false,ikatanaM_plastic++);    // M8~M12
  }

  G4VisAttributes* KatanaM_VisAtt = new G4VisAttributes(G4Colour(0.,1.,0.,0.4));
  KatanaM_VisAtt->SetForceSolid(true);
  fKatanaM_Log->SetVisAttributes(KatanaM_VisAtt);

}
