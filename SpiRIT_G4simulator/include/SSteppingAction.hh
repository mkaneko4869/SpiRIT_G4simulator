#ifndef SSteppingAction_h
#define SSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SSteppingAction : public G4UserSteppingAction
{
  public:
    SSteppingAction();
    virtual ~SSteppingAction();

    void UserSteppingAction(const G4Step*);

    double LightOutput(double ene, int iz, int ia);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
