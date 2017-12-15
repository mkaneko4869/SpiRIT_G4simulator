#include "STSimParticle.hh"

ClassImp(STSimParticle);

STSimParticle::STSimParticle(){
  Clear();
}

STSimParticle::~STSimParticle(){
}

void STSimParticle::Clear(Option_t*)
{
 trackID=-1; parentID=-1; pdg=-1; Z=-9999; A=-9999; tvec=TLorentzVector(0,0,0,0); IsPrimary=kFALSE;
}
