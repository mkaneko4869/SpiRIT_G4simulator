#ifndef STSIMSCINTILLATOR_HH
#define STSIMSCINTILLATOR_HH

#include "TObject.h"
#include "TVector3.h"
#include "TClonesArray.h"
#include "STSimParticle.hh"
#include <vector>

class STSimScintillator : public TObject
{
  public:
  STSimScintillator();
  virtual ~STSimScintillator();

  virtual void Clear(Option_t* option="");

  void SetBarID(Int_t bar){ barID=bar; }
  Int_t GetBarID(){ return barID; }
  void SetBarPosition(TVector3 apos){ pos = apos; }
  TVector3 GetBarPosition(){ return pos; }
  void SetEdep(Double_t de){ dE = de; }
  Double_t GetEdep(){ return dE; }
  void SetMhit(Int_t amhit){ mhit = amhit; }
  Int_t GetMhit(){ return mhit; }
  
  void PushOneTrack(Double_t,TVector3,STSimParticle*);
  
  std::vector<Double_t> GetEdepByATrack(){ return EdepByATrack; }
  std::vector<TVector3> GetHitPos(){ return HitPos; }
  std::vector<STSimParticle*> GetTrackList(){ return part; }

  ClassDef(STSimScintillator,1);
  
  private:
  Int_t      barID; // bar id 
  TVector3   pos;   // bar position
  Double_t   dE;    // energy deposit
  Int_t      mhit;  // multi hit on one bar

  std::vector<Double_t> EdepByATrack;
  std::vector<TVector3> HitPos;
  std::vector<STSimParticle*> part;
};

#endif
