#ifndef SHICEvtInterface_h
#define SHICEvtInterface_h 1

#include <fstream>
#include <string>
#include "globals.hh"
#include "G4VPrimaryGenerator.hh"
#include "G4PrimaryVertex.hh"

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

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
  void SetVertexPos(G4ThreeVector xyz){ fVertexPosition = xyz; }
  G4ThreeVector GetVertexPos(){ return fVertexPosition; }
  
  void SetIsFileOK(G4bool v){ isFileOK = v; }
  G4bool GetIsFileOK(){ return isFileOK; }
  

  private:
#include "SDetectorParameterDef.hh"
  
  G4bool ReadUrqmdEvent(G4PrimaryVertex* v);
  G4bool ReadPhitsEvent(G4PrimaryVertex* v);
  G4bool ReadAMDEvent(G4PrimaryVertex* v);
  
  G4PrimaryVertex* vertex;

  G4String fileName;
  std::ifstream inputFile;
  G4int gentype; // type of event generator, 1:UrQMD, 2:PHITS, 3:AMD
  double fImp;
  double fPhi;
  G4ThreeVector fVertexPosition;
  G4bool isFileOK;

  TFile* phitsFile;
  TTree* phitsTree;
  TClonesArray* phitsParticleArray;
  G4int phitsEventID;
  G4int phitsTotalEvent;
  
  

};


#endif
