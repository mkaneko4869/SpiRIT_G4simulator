#ifndef STSimParticle_hh
#define STSimParticle_hh

#include "TObject.h"
#include "TLorentzVector.h"
#include "TVector3.h"

class STSimParticle : public TObject
{
  public:
  STSimParticle();
  virtual ~STSimParticle();
  
  virtual void Clear(Option_t* option="");
  
  private:
  Long64_t trackID;    // track ID defined in G4
  Long64_t parentID;   // parent track ID (-1 if primary)
  Long64_t pdg;        // particle encode
  Int_t    Z;          // atomic charge
  Int_t    A;          // atomic mass
  TLorentzVector tvec; // 4d momentum at vertex.
  Bool_t   IsPrimary;  // track from primary vertex?
  
  
  public: // setter and getter
  void SetTrackID(Long64_t atrackID){ trackID = atrackID; }
  void SetParentID(Long64_t aparentID){ parentID = aparentID; }
  void SetPDGcode(Long64_t apdg){ pdg = apdg; }
  void SetAtomicCharge(Int_t az){ Z = az; }
  void SetAtomicMass(Int_t am){ A = am; }
  void SetFourMomentum(TLorentzVector v){ tvec = v; }
  void SetFourMomentum(TVector3 mom, Double_t E){ tvec.SetPxPyPzE(mom.X(),mom.Y(),mom.Z(),E); }
  void SetFourMomentum(Double_t px, Double_t py, Double_t pz, Double_t E){ tvec.SetPxPyPzE(px,py,pz,E); }
  void SetIsPrimary(Bool_t a){ IsPrimary = a; }
  Long64_t GetTrackID(){ return trackID; }
  Long64_t GetParentID(){ return parentID; }
  Long64_t GetPDGcode(){ return pdg; }
  Int_t GetAtomicCharge(){ return Z; }
  Int_t GetAtomicMass(){ return A; }
  TLorentzVector GetFourMomentum(){ return tvec; }
  Bool_t GetIsPrimary(){ return IsPrimary; }

  ClassDef(STSimParticle,1);

};

#endif
