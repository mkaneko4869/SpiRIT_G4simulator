#include "STSimTracking.hh"

ClassImp(STSimTracking);

STSimTracking::STSimTracking(){
  Clear();
}

STSimTracking::~STSimTracking(){
}

void STSimTracking::Clear(Option_t*)
{
  part.Clear(); tLength=0.; dE=0.; 
}
