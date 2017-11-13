#ifndef SEventAction_h
#define SEventAction_h 1

#include "G4UserEventAction.hh"
#include "SHistoManager.hh"
class G4Event;

class SEventAction : public G4UserEventAction
{
public:
  SEventAction(SHistoManager*);
  virtual ~SEventAction();

  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

private: 
  
  G4int fSpiRITColID;
  G4int fKyotoScintiColID;
  G4int fKatanaVScintiColID;
  G4int fKatanaMScintiColID;

  SHistoManager* fHistoManager;
};


#endif
