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
 
  private:
  Int_t      barID; // bar id 
  TVector3   pos;   // bar position
  Double_t   dE;    // energy deposit
  Int_t      mhit;  // multi hit on one bar
  std::vector<Double_t> EdepByATrack;   // edep for each track
  std::vector<TVector3> HitPos;         // averaged hit position for each track
  std::vector<STSimParticle*> part;     // each hit track
  
  public:
  void SetBarID(Int_t bar){ barID=bar; }
  void SetBarPosition(TVector3 apos){ pos = apos; }
  void SetEdep(Double_t de){ dE = de; }
  void SetMhit(Int_t amhit){ mhit = amhit; }
  Int_t GetBarID(){ return barID; }
  TVector3 GetBarPosition(){ return pos; }
  Double_t GetEdep(){ return dE; }
  Int_t GetMhit(){ return mhit; }
  
  void PushOneTrack(Double_t,TVector3,STSimParticle*);
   
  std::vector<Double_t> GetEdepByATrack(){ return EdepByATrack; }
  std::vector<TVector3> GetHitPos(){ return HitPos; }
  std::vector<STSimParticle*> GetTrackList(){ return part; }

  ClassDef(STSimScintillator,1);
};

#endif
