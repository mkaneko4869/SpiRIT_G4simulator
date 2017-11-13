#include "STSimParticle.hh"

ClassImp(STSimParticle);

STSimParticle::STSimParticle(){
  Clear();
}

STSimParticle::~STSimParticle(){
}

void STSimParticle::Clear(Option_t*)
{
  pdg=-1; Z=-9999; A=-9999; tvec=TLorentzVector(0,0,0,0); IsPrimary=kFALSE;

}
