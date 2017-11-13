#include "SHistoManager.hh"
#include "STSimParticle.hh"
#include "STSimTracking.hh"
#include "STSimScintillator.hh"
#include "G4PrimaryParticle.hh"
#include "TDirectory.h"

SHistoManager::SHistoManager()
 : fout(0), tree(0),
    eventID(-1), imp(-1.), npart(0), nTrack(0), nCharged(-1),
    eveMaxZ(-999), kyotoM(0), vetoMaxdE(-1.), vetoMaxZ(-999), katanaM(0),
    vertexpos(), part(), track(), kyotoHit(), katanaVHit()
{
  outname = "samurai";
}

SHistoManager::~SHistoManager()
{
  if(fout) delete fout;
}

void SHistoManager::Book()
{
  fout = new TFile(outname+".root","recreate");
  if(!fout){
    G4cout << "HistoManager::Book error!!" <<G4endl;
    return;
  }

  tree = new TTree("tree","G4 simulation");
  tree->Branch("eventID",&eventID,"eventID/I");       
  tree->Branch("imp",&imp,"imp/D");                   
  tree->Branch("phi",&phi,"phi/D");                   
  tree->Branch("npart",&npart,"npart/I");             
  tree->Branch("nTrack",&nTrack,"nTrack/I");             
  tree->Branch("nCharged",&nCharged,"nCharged/I");    
  tree->Branch("eveMaxZ",&eveMaxZ,"eveMaxZ/I");       
  tree->Branch("kyotoM",&kyotoM,"kyotoM/I");          
  tree->Branch("vetoMaxdE",&vetoMaxdE,"vetoMaxdE/D"); 
  tree->Branch("vetoMaxZ",&vetoMaxZ,"vetoMaxZ/I");    
  tree->Branch("katanaM",&katanaM,"katanaM/I");       
  tree->Branch("vertexpos",&vertexpos);
  part = new TClonesArray("STSimParticle");           
  track= new TClonesArray("STSimTracking");
  kyotoHit = new TClonesArray("STSimScintillator");   
  katanaVHit = new TClonesArray("STSimScintillator"); 
  tree->Branch("part",part);
  tree->Branch("track",track);
  tree->Branch("kyotoHit",kyotoHit);
  tree->Branch("katanaVHit",katanaVHit);


}

void SHistoManager::Save()
{
  if(fout){
    fout->Write();
    fout->Close();
    G4cout<<"SHistoManager::Save() is called, root file saved !!"<<G4endl;
  }
}

void SHistoManager::AutoSave()
{
  if(fout){
    TDirectory* savedir = gDirectory;
    fout->cd();
    tree->AutoSave();
    fout->SaveSelf();
    savedir->cd();
    G4cout<<"SHistoManager::AutoSave() is called !!"<<G4endl;
  }
    
}

void SHistoManager::InitParameter()
{
  eventID=-1;
  imp=-1.;
  npart=0;
  nTrack=0;
  nCharged=0;
  eveMaxZ=-999;
  kyotoM=0;
  vetoMaxdE=-1.;
  vetoMaxZ=-999;
  katanaM=0;
  vertexpos=TVector3(-999.,-999.,-999.);
  part->Clear();
  track->Clear();
  kyotoHit->Clear();
  katanaVHit->Clear();
}

void SHistoManager::Fill()
{
  if(tree)
    tree->Fill();


}

void SHistoManager::SetPrimaryVertex(G4PrimaryVertex* pv)
{
  //G4PrimaryParticle* pp = pv->GetPrimary();
  npart = pv->GetNumberOfParticle();
  for(G4int ipart=0; ipart<npart; ipart++){
    G4PrimaryParticle* pp = pv->GetPrimary(ipart);
    
    new((*part)[ipart]) STSimParticle;
    
    auto particle = (STSimParticle*)part->ConstructedAt(ipart);
    particle->SetPDGcode(pp->GetPDGcode());
    particle->SetAtomicCharge(pp->GetParticleDefinition()->GetPDGCharge());
    particle->SetAtomicMass(pp->GetParticleDefinition()->GetAtomicMass());
    particle->SetFourMomentum(pp->GetPx(),pp->GetPy(),pp->GetPz(),pp->GetTotalEnergy());
    particle->SetIsPrimary(kTRUE);
     
    if(pp->GetCharge()!=0) nCharged++;
    if(pp->GetCharge()>=eveMaxZ) eveMaxZ=pp->GetCharge();
  }
}

void SHistoManager::SetSpiRITTrack(TrackerHitsCollection* THC)
{
  std::vector<G4int> trackIDList; trackIDList.clear();
  for(int i=0; i<THC->entries(); i++){
    auto trackpoint = (TrackerHit*)THC->GetHit(i);
    auto info = trackpoint->GetTrackInformation();
    
    if((info->GetOriginalParticle()->GetPDGEncoding()!=111)&&!(info->GetIsPrimary())) continue; // see only primary particle but children from pizero will be tracked ( pizero -> e-e- decay?)
    
    G4bool IsNewTrack=true;
    if(trackIDList.size()!=0)
      for(size_t it=0; it<trackIDList.size(); it++)
        if(trackIDList.at(it)==trackpoint->GetTrackID())
          IsNewTrack=false;
    
    if(IsNewTrack){
      trackIDList.push_back(trackpoint->GetTrackID());
      new((*track)[nTrack]) STSimTracking;
      auto atrack = (STSimTracking*)track->ConstructedAt(nTrack);
      STSimParticle currentTrack;
      currentTrack.SetPDGcode(info->GetCurrentParticle()->GetPDGEncoding());
      currentTrack.SetAtomicCharge(info->GetCurrentParticle()->GetPDGCharge());
      currentTrack.SetAtomicMass(info->GetCurrentParticle()->GetAtomicMass());
      G4ThreeVector p = info->GetOriginalMomentum();
      currentTrack.SetFourMomentum(p.x(),p.y(),p.z(),info->GetOriginalEnergy());
      currentTrack.SetIsPrimary(info->GetIsPrimary());
      atrack->SetPrimaryTrack(currentTrack);
      atrack->AddStep(trackpoint->GetEdep(),trackpoint->GetStepLength());  
      nTrack++;
    }
    else{
      G4int iTrack=0;
      for(size_t i=0; i<trackIDList.size(); i++)
        if(trackIDList.at(i)==trackpoint->GetTrackID())
          iTrack=i;
      auto atrack = (STSimTracking*)track->ConstructedAt(iTrack);
      atrack->AddStep(trackpoint->GetEdep(),trackpoint->GetStepLength());  
    }
  }

}

void SHistoManager::SetKyotoHit(ScintillatorHitsCollection* SHC)
{
  G4bool IsHit[60];
  G4int hitID[60];
  G4int mHit[60];
  for(int i=0; i<60; i++){
    IsHit[i]=false;
    hitID[i]=-1;
    mHit[i]=0;
  }
  G4double edep[60]={};
  G4int ihit=0; // num kyoto bar
  for(int i=0; i<SHC->entries(); i++){ // get hits by each track from hit collection
    auto kyoto = (ScintillatorHit*)SHC->GetHit(i);
    G4int cellID = kyoto->GetID();
    if(!IsHit[cellID]){
      IsHit[cellID]=true;
      new((*kyotoHit)[ihit]) STSimScintillator;
      auto khit = (STSimScintillator*)kyotoHit->ConstructedAt(ihit);
      hitID[cellID]=ihit;
      khit->SetBarID(cellID);
      khit->SetBarPosition(TVector3(kyoto->GetPos().x(),0,kyoto->GetPos().z()));
       
      edep[cellID] = kyoto->GetEdep();
      auto hittrack = new STSimParticle();
      auto info = (STrackInformation*)kyoto->GetTrackInformation();
      hittrack->SetPDGcode(info->GetCurrentParticle()->GetPDGEncoding());
      hittrack->SetAtomicCharge(info->GetCurrentParticle()->GetPDGCharge());
      hittrack->SetAtomicMass(info->GetCurrentParticle()->GetAtomicMass());
      hittrack->SetIsPrimary(info->GetIsPrimary());
      if(info->GetIsPrimary())
        hittrack->SetFourMomentum(info->GetOriginalMomentum().x(),
                                  info->GetOriginalMomentum().y(),
                                  info->GetOriginalMomentum().z(),
                                  info->GetOriginalEnergy());
      G4ThreeVector xyzpos = kyoto->GetHitPosition();
      khit->PushOneTrack(kyoto->GetEdep(),TVector3(xyzpos.x(),xyzpos.y(),xyzpos.z()),hittrack);
      ihit++;
      mHit[cellID]++;
    }
    else{
      edep[cellID]+=kyoto->GetEdep();
      mHit[cellID]++;
    
      auto khit = (STSimScintillator*)kyotoHit->ConstructedAt(hitID[cellID]);
      
      auto hittrack = new STSimParticle();
      auto info = (STrackInformation*)kyoto->GetTrackInformation();
      hittrack->SetPDGcode(info->GetCurrentParticle()->GetPDGEncoding());
      hittrack->SetAtomicCharge(info->GetCurrentParticle()->GetPDGCharge());
      hittrack->SetAtomicMass(info->GetCurrentParticle()->GetAtomicMass());
      hittrack->SetIsPrimary(info->GetIsPrimary());
      if(info->GetIsPrimary())
        hittrack->SetFourMomentum(info->GetOriginalMomentum().x(),
                                  info->GetOriginalMomentum().y(),
                                  info->GetOriginalMomentum().z(),
                                  info->GetOriginalEnergy());
      G4ThreeVector xyzpos = kyoto->GetHitPosition();
      khit->PushOneTrack(kyoto->GetEdep(),TVector3(xyzpos.x(),xyzpos.y(),xyzpos.z()),hittrack);
    }
  }
  for(int i=0; i<ihit; i++){
    auto khit = (STSimScintillator*)kyotoHit->ConstructedAt(i);
    khit->SetEdep(edep[khit->GetBarID()]);
    khit->SetMhit(mHit[khit->GetBarID()]);
    if(edep[khit->GetBarID()]>=1.5)
      kyotoM++;

  }

}

void SHistoManager::SetKatanaVHit(ScintillatorHitsCollection* SHC)
{
  G4bool IsHit[3];
  G4int hitID[3];
  G4int mHit[3];
  G4double edep[3];
  for(int i=0; i<3; i++){
    IsHit[i]=false;
    hitID[i]=-1;
    mHit[i]=0;
    edep[i]=0.0;
  }
  G4int ihit=0; // num katanaV bar
  for(int i=0; i<SHC->entries(); i++){ // get hits by each track from hit collection
    auto katanaV = (ScintillatorHit*)SHC->GetHit(i);
    G4int cellID = katanaV->GetID();
    if(!IsHit[cellID]){
      IsHit[cellID]=true;
      new((*katanaVHit)[ihit]) STSimScintillator;
      auto khit = (STSimScintillator*)katanaVHit->ConstructedAt(ihit);
      hitID[cellID]=ihit;
      khit->SetBarID(cellID);
      khit->SetBarPosition(TVector3(katanaV->GetPos().x(),0,katanaV->GetPos().z()));
       
      edep[cellID] = katanaV->GetEdep();
      auto hittrack = new STSimParticle();
      auto info = (STrackInformation*)katanaV->GetTrackInformation();
      hittrack->SetPDGcode(info->GetCurrentParticle()->GetPDGEncoding());
      hittrack->SetAtomicCharge(info->GetCurrentParticle()->GetPDGCharge());
      hittrack->SetAtomicMass(info->GetCurrentParticle()->GetPDGMass());
      hittrack->SetIsPrimary(info->GetIsPrimary());
      if(info->GetIsPrimary())
        hittrack->SetFourMomentum(info->GetOriginalMomentum().x(),
                                  info->GetOriginalMomentum().y(),
                                  info->GetOriginalMomentum().z(),
                                  info->GetOriginalEnergy());
      G4ThreeVector xyzpos = katanaV->GetHitPosition();
      khit->PushOneTrack(katanaV->GetEdep(),TVector3(xyzpos.x(),xyzpos.y(),xyzpos.z()),hittrack);
      ihit++;
      mHit[cellID]++;
    }
    else{
      edep[cellID]+=katanaV->GetEdep();
      mHit[cellID]++;
      auto khit = (STSimScintillator*)katanaVHit->ConstructedAt(hitID[cellID]);
      
      auto hittrack = new STSimParticle();
      auto info = (STrackInformation*)katanaV->GetTrackInformation();
      hittrack->SetPDGcode(info->GetCurrentParticle()->GetPDGEncoding());
      hittrack->SetAtomicCharge(info->GetCurrentParticle()->GetPDGCharge());
      hittrack->SetAtomicMass(info->GetCurrentParticle()->GetPDGMass());
      hittrack->SetIsPrimary(info->GetIsPrimary());
      if(info->GetIsPrimary())
        hittrack->SetFourMomentum(info->GetOriginalMomentum().x(),
                                  info->GetOriginalMomentum().y(),
                                  info->GetOriginalMomentum().z(),
                                  info->GetOriginalEnergy());
      G4ThreeVector xyzpos = katanaV->GetHitPosition();
      khit->PushOneTrack(katanaV->GetEdep(),TVector3(xyzpos.x(),xyzpos.y(),xyzpos.z()),hittrack);
    }
  }
  for(int i=0; i<ihit; i++){
    auto khit = (STSimScintillator*)katanaVHit->ConstructedAt(i);
    khit->SetEdep(edep[khit->GetBarID()]);
    khit->SetMhit(mHit[khit->GetBarID()]);
    if(edep[khit->GetBarID()]>=vetoMaxdE)
      vetoMaxdE=edep[khit->GetBarID()];
    for(int mh=0; mh<mHit[khit->GetBarID()]; mh++)
      if(khit->GetTrackList().at(mh)->GetAtomicCharge()>=vetoMaxZ)
        vetoMaxZ=khit->GetTrackList().at(mh)->GetAtomicCharge();
  }

}
