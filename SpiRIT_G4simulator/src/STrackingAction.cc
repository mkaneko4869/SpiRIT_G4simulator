#include "STrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "STrackInformation.hh"

STrackingAction::STrackingAction()
{;}

void STrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  if(aTrack->GetParentID()==0 && aTrack->GetUserInformation()==0)
  {
    STrackInformation* anInfo = new STrackInformation(aTrack);
    G4Track* theTrack = (G4Track*)aTrack;
    theTrack->SetUserInformation(anInfo);
  }
}

void STrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  if(secondaries)
  {
    STrackInformation* info = (STrackInformation*)(aTrack->GetUserInformation());
    size_t nSeco = secondaries->size();
    if(nSeco>0)
    {
      for(size_t i=0;i<nSeco;i++)
      { 
        STrackInformation* infoNew = new STrackInformation(info);
        (*secondaries)[i]->SetUserInformation(infoNew);
      }
    }
  }
}
