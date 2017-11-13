#include "KyotoScintiSD.hh"
#include "ScintillatorHit.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4VPhysicalVolume.hh"

KyotoScintiSD::KyotoScintiSD(G4String name)
 : G4VSensitiveDetector(name), fNumberOfBar(60), fScintiCollection(0)
{
  for(G4int i=0; i<fNumberOfBar; i++){
    fHitColID[i].clear();
    fTrackID[i].clear();
  }
  G4String HCname;
  collectionName.insert(HCname="KyotoScintiCollection");
}

KyotoScintiSD::~KyotoScintiSD()
{;}

void KyotoScintiSD::Initialize(G4HCofThisEvent* HCTE)
{
  fScintiCollection = new ScintillatorHitsCollection(SensitiveDetectorName,collectionName[0]);
  for(G4int i=0; i<fNumberOfBar; i++){
    fHitColID[i].clear(); fTrackID[i].clear();
  }
  static G4int HCID = -1;
  if(HCID<0) HCID = GetCollectionID(0);
  HCTE->AddHitsCollection( HCID, fScintiCollection );
}

//G4bool KyotoScintiSD::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
G4bool KyotoScintiSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  //if(!ROhist) return false;
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep<=0.) return false;
  
  const G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4ThreeVector steppos = preStepPoint->GetPosition();
  auto touchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4int copyID = touchable->GetCopyNumber(0); // scinti. ID
  
  G4ThreeVector scintiPos = touchable->GetVolume()->GetObjectTranslation();
  
  const G4Track* track = aStep->GetTrack();
  G4int trackID = track->GetTrackID();
  
  if(fHitColID[copyID].size()==0){ // first ProcessHits on this scintillator
    auto scintiHit = new ScintillatorHit(copyID,edep,scintiPos);
    scintiHit->SetTrackInformation( track );
    scintiHit->PushHitPosBuffer( steppos );
    G4int hitcolID = fScintiCollection->insert( scintiHit );
    fHitColID[copyID].push_back( hitcolID-1 );
    fTrackID[copyID].push_back( trackID );
  }
  else{ // scintillator of this copyID already has hit.
    G4bool isSameTrack = false;
    G4int  sametrackID = -1;
    for(size_t it=0; it<fTrackID[copyID].size(); it++)
      if( fTrackID[copyID].at(it) == trackID ) // there is same particle
        {isSameTrack = true; sametrackID = it;}
    
    if(isSameTrack){ // sum up edep if the track of this ProcessHits is same track that is already stored.
      (*fScintiCollection)[fHitColID[copyID].at(sametrackID)]->AddEdep(edep);
      (*fScintiCollection)[fHitColID[copyID].at(sametrackID)]->PushHitPosBuffer( steppos );
    }
    else{ // create new hit for new track
      auto scintiHit = new ScintillatorHit(copyID,edep,scintiPos);
      scintiHit->SetTrackInformation( aStep->GetTrack() );
      G4int hitcolID = fScintiCollection->insert( scintiHit );
      fHitColID[copyID].push_back( hitcolID-1 );
      fTrackID[copyID].push_back( trackID );
    }
  }

  return true;
}

void KyotoScintiSD::EndOfEvent(G4HCofThisEvent*)
{
  // get average position for each hit track.
  for(G4int i=0; i<fScintiCollection->entries(); i++)
    (*fScintiCollection)[i]->CalculateHitPosition();
}
