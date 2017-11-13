#include "SSteppingAction.hh"

//#include "SPrimaryTrack.hh"

#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "STrackInformation.hh"
#include "G4Step.hh"
#include "SRegionInformation.hh"
//#include "TMath.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


SSteppingAction::SSteppingAction()
  : G4UserSteppingAction()
{
  std::cout<<"Constructor of SSteppingAction"<<std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SSteppingAction::~SSteppingAction()
{;}

void SSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  
  
  G4Track *track = aStep->GetTrack();
  if(track->GetTrackStatus()!=fAlive)
    return;

  //std::cout<<"UserSteppingAction"<<std::endl;

  G4int charge = track->GetDefinition()->GetPDGCharge();
  
  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  G4LogicalVolume* thePreLV = thePrePoint->GetPhysicalVolume()->GetLogicalVolume();
  auto thePreRInfo = (SRegionInformation*)(thePreLV->GetRegion()->GetUserInformation());
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
  G4LogicalVolume* thePostLV = thePostPoint->GetPhysicalVolume()->GetLogicalVolume();
  auto thePostRInfo = (SRegionInformation*)(thePostLV->GetRegion()->GetUserInformation());
  
  //std::cout<<thePreLV->GetName()<<std::endl; 
  //if(thePreRInfo->IsSpiRITSensitive())std::cout<<"TPC!!"<<std::endl;
  // kill the charged particle traveling into vacuum chamber or world volume.
  if( charge!=0 && 
      ( (!(thePreRInfo->IsWorld())&&(thePostRInfo->IsWorld())) || 
        (!(thePreRInfo->IsVC())&&(thePostRInfo->IsVC())) ) )
    track->SetTrackStatus(fStopAndKill);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double SSteppingAction::LightOutput(double ene, int iz, int ia){

  const double a0  = 87.61347*219;
  const double a1  = 0.575846;
  const double a2  = 0.092112;
  const double a3  = 0.290476;
  const double a4  = 0.341147;
  const double a5  = 0.333699;
  const double a6  = 0.194667;
  const double a7  = 2.763769;



  double a   = pow(double(ia),a6);
  double z2  = pow(double(iz),a7);
  double F   = pow(ene,a4);

  //double arg = (F-a1*a-a2*a*z2*log(1.+F/(a3*a*z2)));
  double arg = (F-log(1.+F)*a1*a-a2*a*z2*log(1.+F/(a3*a*z2)));
  double S=0;
  if(arg>0) S= a0*pow(arg,1./a5);
  else      S=-a0*pow(-arg,1./a5);
  if(S<0) S=0;
  // printf(">>  %d %d %f %g\n",iz,ia,ene ,S);

  return S;
}
//______________________________________________________________________________
/////////////////////////////////////////////////////////////////////////////////
