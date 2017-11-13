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
  
  void SetPDGcode(Long64_t apdg){ pdg = apdg; }
  Long64_t GetPDGcode(){ return pdg; }
  void SetAtomicCharge(Int_t az){ Z = az; }
  Int_t GetAtomicCharge(){ return Z; }
  void SetAtomicMass(Int_t am){ A = am; }
  Int_t GetAtomicMass(){ return A; }
  void SetFourMomentum(TLorentzVector v){ tvec = v; }
  void SetFourMomentum(TVector3 mom, Double_t E){ tvec.SetPxPyPzE(mom.X(),mom.Y(),mom.Z(),E); }
  void SetFourMomentum(Double_t px, Double_t py, Double_t pz, Double_t E){ tvec.SetPxPyPzE(px,py,pz,E); }
  TLorentzVector GetFourMomentum(){ return tvec; }
  void SetIsPrimary(Bool_t a){ IsPrimary = a; }
  Bool_t GetIsPrimary(){ return IsPrimary; }

  ClassDef(STSimParticle,1);
  
  private:
  Long64_t pdg;        // particle encode
  Int_t    Z;          // atomic charge
  Int_t    A;          // atomic mass
  TLorentzVector tvec; // 4d momentum
  Bool_t   IsPrimary;  // track from primary vertex?

};

#endif
