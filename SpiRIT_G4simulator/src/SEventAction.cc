#include "SEventAction.hh"
#include "TrackerHit.hh"
#include "ScintillatorHit.hh"
#include "SRunAction.hh"
#include "SPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"

#include "STrackInformation.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>

//Constructor
SEventAction::SEventAction(SHistoManager* hist)
  : G4UserEventAction(),
  fSpiRITColID(-1), fKyotoScintiColID(-1), 
  fKatanaVScintiColID(-1), fKatanaMScintiColID(-1),
  fHistoManager(hist)
{
  G4cout << "Constructor of SEventAction" << G4endl;
}

//Destructor
SEventAction::~SEventAction(){
  G4cout << "Destructor of SEventAction" << G4endl;
}

//member func: BeginOfEventAction
void SEventAction::BeginOfEventAction(const G4Event *evt)
{
  G4cout<<"==== BeginOfEventAction !!  EventID:"<<evt->GetEventID()<<" ===="<<G4endl; 

  G4SDManager* SDMan = G4SDManager::GetSDMpointer();
  if(fSpiRITColID<0||fKyotoScintiColID<0||
    fKatanaVScintiColID<0||fKatanaMScintiColID<0){
    fSpiRITColID        = SDMan->GetCollectionID("SpiRITCollection");
    fKyotoScintiColID   = SDMan->GetCollectionID("KyotoScintiCollection");
    fKatanaVScintiColID = SDMan->GetCollectionID("KatanaVScintiCollection");
    fKatanaMScintiColID = SDMan->GetCollectionID("KatanaMScintiCollection");
  }
  
}

//member func: EndOfEventAction
void SEventAction::EndOfEventAction(const G4Event* evt)
{
  G4cout<<"==== EndOfEventAction !!  EventID:"<<evt->GetEventID()<<" ===="<<G4endl;
  
  fHistoManager->SetEventID(evt->GetEventID());
   
  G4int nvertex = evt->GetNumberOfPrimaryVertex();
  for(G4int iv=0; iv<nvertex; iv++){
    G4PrimaryVertex* pv = evt->GetPrimaryVertex(iv);
    fHistoManager->SetPrimaryVertex(pv);
    //pv->Print();
    G4PrimaryParticle* pp = pv->GetPrimary();
    while(pp){
      pp = pp->GetNext();
    }
  }
   
  // get "Hit Collection of This Event"
  G4HCofThisEvent* HCTE= evt-> GetHCofThisEvent();
  if(! HCTE){std::cout<<"No Hit Collection found !!"<<std::endl; return;}
  TrackerHitsCollection* SpiRITHC = 0;
  ScintillatorHitsCollection* KyotoHC = 0;
  ScintillatorHitsCollection* KatanaVHC = 0;
 // ScintillatorHitsCollection* KatanaMHC = 0;
  if(HCTE){
    SpiRITHC = (TrackerHitsCollection*)(HCTE->GetHC(fSpiRITColID));
    KyotoHC = (ScintillatorHitsCollection*)(HCTE->GetHC(fKyotoScintiColID));
    KatanaVHC = (ScintillatorHitsCollection*)(HCTE->GetHC(fKatanaVScintiColID));
 //   KatanaMHC = (ScintillatorHitsCollection*)(HCTE->GetHC(fKatanaMScintiColID));
  }
  
  if(SpiRITHC){
    fHistoManager->SetSpiRITTrack(SpiRITHC);
  }
  
  if(KyotoHC){
    fHistoManager->SetKyotoHit(KyotoHC);
  }
  
  if(KatanaVHC){
    fHistoManager->SetKatanaVHit(KatanaVHC);
  }


  if(fHistoManager->GetIsInputFileOK())
    fHistoManager->Fill();
  if(evt->GetEventID()%5000==0)
    fHistoManager->AutoSave();
  fHistoManager->InitParameter();
   
}
