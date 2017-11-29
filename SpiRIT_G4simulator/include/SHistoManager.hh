#ifndef SHistoManager_h
#define SHistoManager_h 1

#include "globals.hh"

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TVector3.h"

#include "G4PrimaryVertex.hh"
#include "ScintillatorHit.hh"
#include "TrackerHit.hh"

class SHistoManager
{
  public:
  SHistoManager();
  virtual ~SHistoManager();
  
  void Book();
  void InitParameter();
  void Fill();
  void Save();
  void AutoSave();
  
  void SetOutputName(TString name){ outname = name; }
  void SetEventID(G4int id){ eventID = id; }
  void SetImp(G4double aimp){ imp = aimp; }
  void SetPhiRP(G4double aphi){ phi = aphi; }
  void SetVertexPos(G4ThreeVector xyz)
  { 
    TVector3 pos(xyz.x(),xyz.y(),xyz.z());
    vertexpos = pos;
  }

  void SetPrimaryVertex(G4PrimaryVertex* );
  void SetSpiRITTrack(TrackerHitsCollection* );
  void SetKyotoHit(ScintillatorHitsCollection* );
  void SetKatanaVHit(ScintillatorHitsCollection* );
  
  void SetNFragment(G4int nfrag){ npart = nfrag; }
  void SetNCharged(G4int nchg){ nCharged = nchg; }
  void SetEveMaxZ(G4int az){ eveMaxZ = az; }
  void SetNKyoto(G4int nk){ kyotoM = nk; }
  void SetVMaxZ(G4int az){ vetoMaxZ = az; }
  void SetVMaxdE(G4double de){ vetoMaxdE = de; }
  void SetNKatanaM(G4int nk){ katanaM = nk; }
  
  void SetIsInputFileOK(bool v){ isInputFileOK = v; }
  G4bool GetIsInputFileOK(){ return isInputFileOK; };
   
  private:
  TFile* fout;
  TString outname;
  TTree* tree;

  Int_t     eventID;        // event ID
  Double_t  imp;            // impact parameter
  Double_t  phi;            // reaction plane
  Int_t     npart;          // number of fragment from the collision
  Int_t     nTrack;         // number of track passed through the TPC
  Int_t     nCharged;       // number of charged particle  
  Int_t     eveMaxZ;        // maximum Z value of the event
  Int_t     kyotoM;         // kyoto multiplicity          
  Double_t  vetoMaxdE;      // maximum energy deposit in veto paddles
  Int_t     vetoMaxZ;       // maximum Z which passed throuhg the veto paddles
  Int_t     katanaM;        // katana multiplicity
  TVector3  vertexpos;      // vertex position
  TClonesArray* part;       // fragment from collision
  TClonesArray* track;      // information of track passed through TPC sensitive area.
  TClonesArray* kyotoHit;   // kyoto Hit information
  TClonesArray* katanaVHit; // katana veto Hit information
  
  G4bool isInputFileOK;

};
#endif
