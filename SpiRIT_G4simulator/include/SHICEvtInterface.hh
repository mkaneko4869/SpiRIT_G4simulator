#ifndef SHICEvtInterface_h
#define SHICEvtInterface_h 1

#include <fstream>
#include <string>
#include "globals.hh"
#include "G4VPrimaryGenerator.hh"

class G4PrimaryVertex;
class G4Event;

class SHICEvtInterface : public G4VPrimaryGenerator
{
  public:
  SHICEvtInterface(std::string evfile);
  virtual ~SHICEvtInterface();
   
  void LoadHeader();
  void GeneratePrimaryVertex(G4Event* evt);
  
  void SetImp(G4double imp){ fImp=imp; }
  G4double GetImp(){ return fImp; }
  void SetPhiRP(G4double phi){ fPhi=phi; }
  G4double GetPhiRP(){ return fPhi; }
  void SetVertexPos(G4ThreeVector xyz){ fVertexPos = xyz; }
  G4ThreeVector GetVertexPos(){ return fVertexPos; }
  
  void SetIsFileOK(G4bool v){ isFileOK = v; }
  G4bool GetIsFileOK(){ return isFileOK; }


  private:
#include "SDetectorParameterDef.hh"
  G4String fileName;
  std::ifstream inputFile;
  G4int gentype; // type of event generator, 1:UrQMD, 2:PHITS, 3:AMD
  G4double fImp;
  G4double fPhi;
  G4ThreeVector fVertexPos;
  G4bool isFileOK;
};

#endif
