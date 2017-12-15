#include "SpiRITSD.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
SpiRITSD::SpiRITSD(G4String name)
  : G4VSensitiveDetector(name), fTrackerCollection(0)
{
  G4String HCname;
  collectionName.insert(HCname="SpiRITCollection");
}

SpiRITSD::~SpiRITSD(){;}

void SpiRITSD::Initialize(G4HCofThisEvent* HCTE)
{
  static int HCID = -1;
  fTrackerCollection = new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]);
  if(HCID<0)
  { HCID = GetCollectionID(0); }
  HCTE->AddHitsCollection(HCID,fTrackerCollection);
}

G4bool SpiRITSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep<=0.0) return false;

  TrackerHit* newHit = new TrackerHit();
  newHit->SetEdep( edep );
  newHit->SetStepLength( aStep->GetStepLength() );
  newHit->SetPos( aStep->GetPreStepPoint()->GetPosition() );
  newHit->SetTrackInformation( aStep->GetTrack() );
  newHit->SetTrackID( aStep->GetTrack()->GetTrackID() );
  newHit->SetParentID( aStep->GetTrack()->GetParentID() );
  newHit->SetMomentum( aStep->GetTrack()->GetMomentum() );
  newHit->SetTime( aStep->GetPreStepPoint()->GetGlobalTime() );
  newHit->SetTotalEnergy( aStep->GetPreStepPoint()->GetTotalEnergy() );
  fTrackerCollection->insert( newHit );

  return true;

}

void SpiRITSD::EndOfEvent(G4HCofThisEvent*){;}
