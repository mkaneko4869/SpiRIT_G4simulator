#ifndef STrackInformation_h
#define STrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class STrackInformation : public G4VUserTrackInformation 
{
  public:
    STrackInformation();
    STrackInformation(const G4Track* aTrack);
    STrackInformation(const STrackInformation* aTrackInfo);
    virtual ~STrackInformation();

    inline void *operator new(size_t);
    inline void operator delete(void *aTrackInfo);

    inline int operator ==(const STrackInformation& right) const
    {return (this==&right);}
    STrackInformation& operator =(const STrackInformation& right);

    virtual void Print() const;

    void SetCurrentTrackInformation(const G4Track* aTrack);
    inline void SetIsPrimary(G4bool f) {fIsPrimary = f;}
    inline G4bool GetIsPrimary() const {return fIsPrimary;}

  private:
    // Information of the primary track from the primary vertex
    G4int                 fOriginalTrackID;
    G4ParticleDefinition* fParticleDefinition;
    G4ThreeVector         fOriginalPosition;
    G4ThreeVector         fOriginalMomentum;
    G4double              fOriginalEnergy;
    G4double              fOriginalTime;

    G4bool                fIsPrimary;
    // Information of the current track.
    G4int                 fCurrentTrackID;
    G4ParticleDefinition* fCurrentDefinition;
    G4ThreeVector         fCurrentPosition;
    G4ThreeVector         fCurrentMomentum;
    G4double              fCurrentEnergy;
    G4double              fCurrentTime;

  public:
    inline G4int GetOriginalTrackID() const {return fOriginalTrackID;}
    inline G4ParticleDefinition* GetOriginalParticle() const {return fParticleDefinition;}
    inline G4ThreeVector GetOriginalPosition() const {return fOriginalPosition;}
    inline G4ThreeVector GetOriginalMomentum() const {return fOriginalMomentum;}
    inline G4double GetOriginalEnergy() const {return fOriginalEnergy;}
    inline G4double GetOriginalTime() const {return fOriginalTime;}
    inline G4int GetCurrentTrackID() const {return fCurrentTrackID;}
    inline G4ParticleDefinition* GetCurrentParticle() const {return fCurrentDefinition;}
    inline G4ThreeVector GetCurrentPosition() const {return fCurrentPosition;}
    inline G4ThreeVector GetCurrentMomentum() const {return fCurrentMomentum;}
    inline G4double GetCunnrentEnergy() const {return fCurrentEnergy;}
    inline G4double GetCurrentTime() const {return fCurrentTime;}
};

extern G4Allocator<STrackInformation> aTrackInformationAllocator;

inline void* STrackInformation::operator new(size_t)
{ void* aTrackInfo;
  aTrackInfo = (void*)aTrackInformationAllocator.MallocSingle();
  return aTrackInfo;
}

inline void STrackInformation::operator delete(void *aTrackInfo)
{ aTrackInformationAllocator.FreeSingle((STrackInformation*)aTrackInfo);}

#endif
