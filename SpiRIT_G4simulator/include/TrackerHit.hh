#ifndef TrackerHit_h
#define TrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "G4Track.hh"
#include "STrackInformation.hh"

class TrackerHit : public G4VHit
{
  public:
    TrackerHit();
    virtual ~TrackerHit();

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    virtual void Print();

    inline void SetEdep(G4double edep){ fEdep = edep; }
    inline G4double GetEdep() const { return fEdep;}
    inline void SetStepLength(G4double slen){ fStepLength = slen; }
    inline G4double GetStepLength() const { return fStepLength;}
    inline void SetPos(G4ThreeVector xyz){ fPos = xyz; }
    inline G4ThreeVector Getpos(){ return fPos; }
    inline void SetTrackID(G4int id){ fTrackID = id;}
    inline G4int GetTrackID() const { return fTrackID; }
    inline void SetTrackInformation(const G4Track* aTrack)
    {
      STrackInformation* info = (STrackInformation*)(aTrack->GetUserInformation());
      info->SetCurrentTrackInformation(aTrack);
      fTrackInfo = *info;
    }
    inline STrackInformation* GetTrackInformation()
    { return &fTrackInfo; }
  private:
    G4int fTrackID;
    G4double fEdep;
    G4ThreeVector fPos;
    G4double fStepLength;
    STrackInformation fTrackInfo;

};

typedef G4THitsCollection<TrackerHit> TrackerHitsCollection;

extern G4ThreadLocal G4Allocator<TrackerHit> * TrackerHitAllocator;

inline void* TrackerHit::operator new(size_t)
{
  if(!TrackerHitAllocator)
    TrackerHitAllocator = new G4Allocator<TrackerHit>;
  return (void *) TrackerHitAllocator->MallocSingle();
}

inline void TrackerHit::operator delete(void *aHit)
{
  TrackerHitAllocator->FreeSingle((TrackerHit*) aHit);
}

#endif
