#ifndef STSimTracking_hh
#define STSimTracking_hh

#include "STSimParticle.hh"
#include "TObject.h"
#include "TLorentzVector.h"
#include "TVector3.h"

class STSimTracking : public TObject
{
  public:
  STSimTracking();
  virtual ~STSimTracking();
  
  virtual void Clear(Option_t* option="");
  
  void SetPrimaryTrack(STSimParticle apart){ part = apart; }
  STSimParticle* GetPrimaryTrack(){ return &part; }
  void SetEdep(Double_t edep){ dE = edep; }
  Double_t GetEdep(){ return dE; }
  void SetTrackLength(Double_t tl){ tLength = tl; }
  Double_t GetTrackLength(){ return tLength; }
  void AddStep(Double_t edep,Double_t tl)
  {
    dE+=edep;
    tLength+=tl;
  }
    
  ClassDef(STSimTracking,1);
  
  private:
  STSimParticle part;
  Double_t dE;
  Double_t tLength;
  
};

#endif
