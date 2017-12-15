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

  private:
    G4int fTrackID;
    G4int fParentID;
    G4double fEdep;
    G4double fTime;
    G4double fEtot;
    G4ThreeVector fMom;
    G4ThreeVector fPos;
    G4double fStepLength;
    STrackInformation fTrackInfo;
  
  public:
    inline void SetTrackID(G4int id){ fTrackID = id;}
    inline void SetParentID(G4int id){ fParentID = id;}
    inline void SetEdep(G4double edep){ fEdep = edep; }
    inline void SetTime(G4double time){ fTime = time; }
    inline void SetTotalEnergy(G4double etot){ fEtot = etot; }
    inline void SetMomentum(G4ThreeVector mom){ fMom = mom; }
    inline void SetStepLength(G4double slen){ fStepLength = slen; }
    inline void SetPos(G4ThreeVector xyz){ fPos = xyz; }
    inline void SetTrackInformation(const G4Track* aTrack)
    {
      STrackInformation* info = (STrackInformation*)(aTrack->GetUserInformation());
      info->SetCurrentTrackInformation(aTrack);
      fTrackInfo = *info;
    }
    
    inline G4double GetEdep() const { return fEdep;}
    inline G4double GetStepLength() const { return fStepLength;}
    inline G4double GetTime() const { return fTime;}
    inline G4double GetTotalEnergy() const { return fEtot;}
    inline G4ThreeVector GetPos(){ return fPos; }
    inline G4ThreeVector GetMomentum(){ return fMom; }
    inline G4int GetTrackID() const { return fTrackID; }
    inline G4int GetParentID() const { return fParentID; }
    inline STrackInformation* GetTrackInformation()
    { return &fTrackInfo; }

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
