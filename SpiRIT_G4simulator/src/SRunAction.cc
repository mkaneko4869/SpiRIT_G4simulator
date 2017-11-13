#include "SRunAction.hh"
#include "G4Run.hh"
#include "SHistoManager.hh"

SRunAction::SRunAction(SHistoManager* histo)
 : G4UserRunAction(), fHistoManager(histo)
{
  G4cout << "Constructor of SRunAction" << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SRunAction::~SRunAction()
{
  G4cout << "Destructor of SRunAction" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "==== BeginOfRunAction !!  RunID: " << aRun->GetRunID() << " ===="<<G4endl;
  
  fHistoManager->Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SRunAction::EndOfRunAction(const G4Run* aRun)
{ 
  G4cout << "==== EndOfRunAction !!  RunID: " << aRun->GetRunID() << " ===="<<G4endl;
  
  fHistoManager->Save();
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
