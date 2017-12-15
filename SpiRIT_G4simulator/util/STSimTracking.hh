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
  
  void AddStep(Double_t edep,Double_t tl)
  {
    dE+=edep; tLength+=tl;
  }

  virtual void Clear(Option_t* option="");
  
  private:
  STSimParticle part;
  Double_t dE;
  Double_t tLength;
  

  public:
  void SetTrack(STSimParticle apart){ part = apart; }
  void SetEdep(Double_t edep){ dE = edep; }
  void SetTrackLength(Double_t tl){ tLength = tl; }
  STSimParticle* GetTrack(){ return &part; }
  Double_t GetEdep(){ return dE; }
  Double_t GetTrackLength(){ return tLength; }
    
  ClassDef(STSimTracking,1);
  
};

#endif
