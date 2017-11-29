#ifndef SPrimaryGeneratorAction_h
#define SPrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4VPrimaryGenerator.hh"
#include "G4ThreeVector.hh"

#include <fstream>

#include "SHistoManager.hh"

#include "SHICEvtInterface.hh"
class G4ParticleGun;
class G4ParticleTable;
class G4ParticleDefinition;
class G4Event;

class SPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
   public:
      SPrimaryGeneratorAction(SHistoManager*);
      ~SPrimaryGeneratorAction();

      void GeneratePrimaries(G4Event* anEvent);

   private:
      SHICEvtInterface* fHICEvt;
      G4VPrimaryGenerator* fParticleGun;
      G4bool fUseHICEvt;

      SHistoManager* fHistoManager;

#include "SDetectorParameterDef.hh"

};

#endif
