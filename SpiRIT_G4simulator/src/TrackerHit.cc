#include "TrackerHit.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<TrackerHit> * TrackerHitAllocator = 0;

TrackerHit::TrackerHit()
  : G4VHit(), fTrackID(-1), fParentID(-1), fEdep(0.0), fMom(0), fEtot(0.0), fPos(0), fStepLength(0.0), fTrackInfo()
{;}

TrackerHit::~TrackerHit()
{;}

void TrackerHit::Print()
{
  G4cout << "TrackID:" << fTrackID << ", Position:" << fPos << " : " << fEdep/keV << "[keV]" << G4endl;
  G4cout << "TrackInformation:" << G4endl;
  fTrackInfo.Print();
}
