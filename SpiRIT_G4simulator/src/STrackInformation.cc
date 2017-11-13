#include "STrackInformation.hh"
#include "G4ios.hh"

G4Allocator<STrackInformation> aTrackInformationAllocator;

STrackInformation::STrackInformation()
  : G4VUserTrackInformation()
{
    fOriginalTrackID = 0;
    fParticleDefinition = 0;
    fOriginalPosition = G4ThreeVector(0.,0.,0.);
    fOriginalMomentum = G4ThreeVector(0.,0.,0.);
    fOriginalEnergy = 0.;
    fOriginalTime = 0.;
    
    fIsPrimary=false;
    fCurrentTrackID = -1;
    fCurrentDefinition = 0;
    fCurrentPosition = G4ThreeVector(0.,0.,0.);
    fCurrentMomentum = G4ThreeVector(0.,0.,0.);
    fCurrentEnergy = 0.;
    fCurrentTime = 0.;
}

STrackInformation::STrackInformation(const G4Track* aTrack)
  : G4VUserTrackInformation()
{
    fOriginalTrackID = aTrack->GetTrackID();
    fParticleDefinition = aTrack->GetDefinition();
    fOriginalPosition = aTrack->GetPosition();
    fOriginalMomentum = aTrack->GetMomentum();
    fOriginalEnergy = aTrack->GetTotalEnergy();
    fOriginalTime = aTrack->GetGlobalTime();
    
    fCurrentTrackID = -1;
    fCurrentDefinition = 0;
    fCurrentPosition = G4ThreeVector(0.,0.,0.);
    fCurrentMomentum = G4ThreeVector(0.,0.,0.);
    fCurrentEnergy = 0.;
    fCurrentTime = 0.;
}

STrackInformation::STrackInformation(const STrackInformation* aTrackInfo)
  : G4VUserTrackInformation()
{
    fOriginalTrackID    = aTrackInfo->fOriginalTrackID;
    fParticleDefinition = aTrackInfo->fParticleDefinition;
    fOriginalPosition   = aTrackInfo->fOriginalPosition;
    fOriginalMomentum   = aTrackInfo->fOriginalMomentum;
    fOriginalEnergy     = aTrackInfo->fOriginalEnergy;
    fOriginalTime       = aTrackInfo->fOriginalTime;
    fIsPrimary          = aTrackInfo->fIsPrimary;
    fCurrentTrackID     = aTrackInfo->fCurrentTrackID;
    fCurrentDefinition  = aTrackInfo->fCurrentDefinition;
    fCurrentPosition    = aTrackInfo->fCurrentPosition;
    fCurrentMomentum    = aTrackInfo->fCurrentMomentum;
    fCurrentEnergy      = aTrackInfo->fCurrentEnergy;
    fCurrentTime        = aTrackInfo->fCurrentTime;
}

STrackInformation::~STrackInformation(){;}

void STrackInformation::SetCurrentTrackInformation(const G4Track* aTrack)
{
  if(fOriginalTrackID==aTrack->GetTrackID()) fIsPrimary = true;
  else fIsPrimary = false;
  fCurrentTrackID = aTrack->GetTrackID();
  fCurrentDefinition = aTrack->GetDefinition();
  fCurrentPosition = aTrack->GetPosition();
  fCurrentMomentum = aTrack->GetMomentum();
  fCurrentEnergy = aTrack->GetTotalEnergy();
  fCurrentTime = aTrack->GetGlobalTime();
}

STrackInformation& STrackInformation::operator =(const STrackInformation& aTrackInfo)
{
    fOriginalTrackID    = aTrackInfo.fOriginalTrackID;
    fParticleDefinition = aTrackInfo.fParticleDefinition;
    fOriginalPosition   = aTrackInfo.fOriginalPosition;
    fOriginalMomentum   = aTrackInfo.fOriginalMomentum;
    fOriginalEnergy     = aTrackInfo.fOriginalEnergy;
    fOriginalTime       = aTrackInfo.fOriginalTime;
    fIsPrimary          = aTrackInfo.fIsPrimary;
    fCurrentTrackID     = aTrackInfo.fCurrentTrackID;
    fCurrentDefinition  = aTrackInfo.fCurrentDefinition;
    fCurrentPosition    = aTrackInfo.fCurrentPosition;
    fCurrentMomentum    = aTrackInfo.fCurrentMomentum;
    fCurrentEnergy      = aTrackInfo.fCurrentEnergy;
    fCurrentTime        = aTrackInfo.fCurrentTime;

    return *this;
}


void STrackInformation::Print() const
{
    G4cout 
      << "Current track ID: " << fCurrentTrackID
      << ", " << fCurrentDefinition->GetParticleName() << G4endl;
    G4cout 
     << "Original primary track ID: " << fOriginalTrackID 
     << ", " << fParticleDefinition->GetParticleName()
     << " " << fOriginalMomentum << G4endl;
}
