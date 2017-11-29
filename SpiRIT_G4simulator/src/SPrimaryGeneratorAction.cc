#include "SPrimaryGeneratorAction.hh"


#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4INCLGlobals.hh"
#include "G4ThreeVector.hh"

#include "SHICEvtInterface.hh"

#include <iostream>
#include <iomanip>
#include <string>

SPrimaryGeneratorAction::SPrimaryGeneratorAction(SHistoManager* histo)
  : G4VUserPrimaryGeneratorAction(),
    fHICEvt(0), fParticleGun(0), fHistoManager(histo)
{
#include "SDetectorParameterDef.icc"

   G4cout << "Constructor of SPrimaryGeneratorAction" << G4endl;

   // read input file and its header part. 
   char* datapath = std::getenv("INPFILE");
   
   fHICEvt = new SHICEvtInterface(datapath);
   fUseHICEvt = true; 
   
   std::string inname = std::string(datapath);
   inname.erase(0,inname.find_last_of('/')+1);  // remove path name -> only file name
   if(inname.find(".f14.clu") != std::string::npos){
     inname.erase(inname.find(".f14.clu"));
     inname.insert(0,"urqmd_");
   }
   else if(inname.find("phits") != std::string::npos) inname.erase(inname.find(".dat"));
   else if(inname.find("amd") != std::string::npos) inname.erase(inname.find(".dat"));
   fHistoManager->SetOutputName(inname);   

   G4int n_particle = 1;
   auto particleGun = new G4ParticleGun(n_particle);
   auto ionTable = G4IonTable::GetIonTable();
   G4ParticleDefinition* particle = ionTable->GetIon(50,132,0.0);
   particleGun->SetParticleDefinition(particle);
   particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.));
   particleGun->SetParticleEnergy(270.*132.*MeV);
   G4ThreeVector tgtpos(fTargetPos_x,fTargetPos_y,fTargetPos_z);  
   G4ThreeVector tgtsize(fTarget_x,fTarget_y,fTarget_z);  
   //randomize within target of 3x4 cm2
   //x,y : gaussian with centroid=tgt(x,y), sigma=5mm
   //z   : uniform random within target thickness 
   G4ThreeVector beampos(G4RandGauss::shoot(tgtpos.x(), 5.*mm),
                         G4RandGauss::shoot(tgtpos.y(), 5.*mm), 
                         tgtpos.z()+(G4UniformRand()-0.5)*tgtsize.z() ); 

   particleGun->SetParticlePosition(beampos);
   fParticleGun = particleGun;


}

#include "G4AutoLock.hh"
namespace {
  G4Mutex SPrimGenDestrMutex = G4MUTEX_INITIALIZER;
  G4Mutex SPrimGenMutex = G4MUTEX_INITIALIZER;
}


SPrimaryGeneratorAction::~SPrimaryGeneratorAction()
{
   G4cout << "Destructor of SPrimaryGeneratorAction" << G4endl;
   G4AutoLock lock(&SPrimGenDestrMutex);
   if(fHICEvt) {delete fHICEvt; fHICEvt=0;}
   delete fParticleGun;
}

void SPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  
  G4cout << "==== GeneratePrimaries called !! ====" << G4endl;
  
  if(fUseHICEvt){
    G4AutoLock lock(&SPrimGenMutex);
    fHICEvt->GeneratePrimaryVertex(anEvent);
    
    fHistoManager->SetImp(fHICEvt->GetImp());
    fHistoManager->SetPhiRP(fHICEvt->GetPhiRP());
    fHistoManager->SetVertexPos(fHICEvt->GetVertexPos());
  }
  else{
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
  



   G4cout << "GeneratePrimaries ends" << G4endl;
}
