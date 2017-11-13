#include "STSimScintillator.hh"

ClassImp(STSimScintillator);

STSimScintillator::STSimScintillator(){
  Clear();
}

STSimScintillator::~STSimScintillator(){
}

void STSimScintillator::Clear(Option_t*)
{
  barID=-1; dE=-1.; mhit=0; pos=TVector3(-9999,-9999,-9999);
  EdepByATrack.clear(); HitPos.clear(); part.clear();
}


void STSimScintillator::PushOneTrack(Double_t edep, TVector3 pos, STSimParticle *apart)
{
  EdepByATrack.push_back(edep);
  HitPos.push_back(pos);
  part.push_back(apart);
}

