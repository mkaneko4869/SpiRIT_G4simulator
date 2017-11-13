#ifndef ScintillatorHit_h
#define ScintillatorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "STrackInformation.hh"
#include "G4Types.hh"
#include <vector>

class ScintillatorHit : public G4VHit
{
  public:
    ScintillatorHit();
    ScintillatorHit(G4int id);
    ScintillatorHit(G4int id, G4double edep, G4ThreeVector pos);
    ScintillatorHit(G4int id, G4double edep, G4ThreeVector pos, STrackInformation info);
    virtual ~ScintillatorHit();

    virtual void Print();

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    inline void SetID(G4int id){ fCellID = id; }
    inline G4int GetID(){ return fCellID; }
    inline void SetEdep(G4double edep){ fEdep = edep; }
    inline void AddEdep(G4double edep){ fEdep += edep; }
    inline G4double GetEdep(){ return fEdep; }
    inline void SetPos(G4ThreeVector pos){ fPos = pos; }
    inline G4ThreeVector GetPos(){ return fPos; }

    inline void SetTrackInformation(const G4Track* aTrack)
    {
      STrackInformation* info = (STrackInformation*)(aTrack->GetUserInformation());
      info->SetCurrentTrackInformation(aTrack);
      fTrackInfo = *info ;
    }
    inline STrackInformation* GetTrackInformation()
    { return &fTrackInfo; }
    
    inline G4int GetTrackID()
    { return fTrackInfo.GetCurrentTrackID(); }

    inline void SetHitPosition(G4ThreeVector xyz)
    { fHitPos = xyz; }
    inline void CalculateHitPosition()
    { 
      for(size_t i=0; i<fHitPosBuffer.size(); i++)
        fHitPos += fHitPosBuffer.at(i); 
      fHitPos /= (double)fHitPosBuffer.size();
    }
    inline G4ThreeVector GetHitPosition()
    { return fHitPos; }
    
    inline void PushHitPosBuffer(G4ThreeVector xyz)
    { fHitPosBuffer.push_back( xyz ); }
    inline void ClearHitPosBuffer()
    { fHitPosBuffer.clear(); }


  private:
    G4int fCellID;  // scintillator ID
    G4double fEdep; // total energy deposit
    G4ThreeVector fPos; // position of the scintillator
    STrackInformation fTrackInfo; // track information which passed scintillator.
    G4ThreeVector fHitPos;   // position of passed track (average)
    std::vector<G4ThreeVector> fHitPosBuffer; // buffer for calculate the average position of track path.

};

typedef G4THitsCollection<ScintillatorHit> ScintillatorHitsCollection;

extern G4ThreadLocal G4Allocator<ScintillatorHit> * ScintillatorHitAllocator;

inline void* ScintillatorHit::operator new(size_t)
{
  if(!ScintillatorHitAllocator)
    ScintillatorHitAllocator = new G4Allocator<ScintillatorHit>;
  return (void *) ScintillatorHitAllocator->MallocSingle();
}

inline void ScintillatorHit::operator delete(void *aHit)
{
  ScintillatorHitAllocator->FreeSingle((ScintillatorHit*) aHit);
}

#endif
