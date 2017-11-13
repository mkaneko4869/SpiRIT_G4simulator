#ifndef SDetectorConstruction_H
#define SDetectorConstruction_H 1

class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VisAttributes;
class SMagField; 

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class SDetectorConstruction : public G4VUserDetectorConstruction
{
public:

  SDetectorConstruction();
  ~SDetectorConstruction();

  G4VPhysicalVolume* Construct();
  

private:
#include "SDetectorParameterDef.hh"
  void DefineMaterials(); 
  void ClearMaterials(); 
  void ConstructSAMURAI();
  void ConstructVacuumChamber();
  void ConstructTPCenc();
  void ConstructFieldCage();
  void ConstructTarget();
  void ConstructKyotoArray();
  void ConstructKATANA();
  
  void SetMagField();
  
  // materials
  G4Material* fVacuum;
  G4Material* fP10gas;
  G4Material* fKapton; // for window
  G4Material* fMylar; // for window
  G4Material* fG10;   // for field cage
  G4Material* fAirMaterial;
  G4Material* fKyotoPlaMaterial; // for Kyoto
  G4Material* fPlasticMaterial; // for katana
  G4Material* fFe;  // for SAMURAI
  G4Material* fAl;  // for vacuum chamber
  G4Material* fSn;  // for target
     
  // fVacuum  
  G4LogicalVolume*   fHall_Log;
  // SAMURAI 
  G4LogicalVolume*   fYoke_Log;
  G4LogicalVolume*   fCramp_Log;
  //Vacuum chamber
  G4LogicalVolume*   fVC_Log;
  // fAir 
  G4LogicalVolume*   fVCInside_Log;
  //TPC enclosure
  G4LogicalVolume*   fTPCEnc_Log;
  // fP10gas
  G4LogicalVolume*   fTPCInside_Log;
  
  G4LogicalVolume*   fFC_Log;
  G4LogicalVolume*   fTarget_Log;
  G4LogicalVolume*   fKapton_Log;
  G4LogicalVolume*   fMylar_Log;
  
  // TPC sensitive area(P10)
  G4LogicalVolume*   fTPCSensitive_Log;
     
  // ancillary arrays
  G4LogicalVolume*   fKyotoPla_Log;
  G4LogicalVolume*   fKatanaVeto_Log;
  G4LogicalVolume*   fKatanaM_Log;

};

#endif
