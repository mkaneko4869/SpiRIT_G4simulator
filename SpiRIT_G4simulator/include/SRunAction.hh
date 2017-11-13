#ifndef SRunAction_h
#define SRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "SHistoManager.hh"
class G4Run;

class SRunAction : public G4UserRunAction
{
public:
  SRunAction(SHistoManager*);
  virtual ~SRunAction();

  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
  

private:
   SHistoManager* fHistoManager;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
