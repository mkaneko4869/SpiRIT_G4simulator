#include "ScintillatorHit.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<ScintillatorHit> * ScintillatorHitAllocator = 0;

ScintillatorHit::ScintillatorHit()
 : G4VHit(), fCellID(-1), fEdep(0.0), fPos(0), fTrackInfo(), fHitPos(0)
{;}

ScintillatorHit::ScintillatorHit(G4int id)
 : G4VHit(), fCellID(id), fEdep(0.0), fPos(0), fTrackInfo(), fHitPos(0)
{;}

ScintillatorHit::ScintillatorHit(G4int id, G4double edep, G4ThreeVector pos)
 : G4VHit(), fCellID(id), fEdep(edep), fPos(pos), fTrackInfo(), fHitPos(0)
{;}

ScintillatorHit::ScintillatorHit(G4int id, G4double edep, G4ThreeVector pos, STrackInformation info)
 : G4VHit(), fCellID(id), fEdep(edep), fPos(pos), fTrackInfo(info), fHitPos(0)
{;}

ScintillatorHit::~ScintillatorHit()
{;}

void ScintillatorHit::Print()
{
  G4cout << "ScintillatorHit: Cell[" << fCellID << "] " << fEdep/MeV << " MeV" << G4endl;
  G4cout << "TrackInformation: " << G4endl;
  fTrackInfo.Print();
}
