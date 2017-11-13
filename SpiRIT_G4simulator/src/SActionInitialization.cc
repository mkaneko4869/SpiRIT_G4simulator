#include "SActionInitialization.hh"
#include "SPrimaryGeneratorAction.hh"
#include "SRunAction.hh"
#include "SEventAction.hh"
#include "SSteppingAction.hh"
#include "STrackingAction.hh"
#include "SHistoManager.hh"

SActionInitialization::SActionInitialization()
 : G4VUserActionInitialization()
{}

SActionInitialization::~SActionInitialization()
{}

void SActionInitialization::Build() const
{
  auto histo      = new SHistoManager();
  SetUserAction(new SRunAction(histo));
  SetUserAction(new SPrimaryGeneratorAction(histo));
  SetUserAction(new SEventAction(histo));
  SetUserAction(new SSteppingAction()); 
  SetUserAction(new STrackingAction); 
}

void SActionInitialization::BuildForMaster() const
{
  auto histo      = new SHistoManager();
  SetUserAction(new SRunAction(histo));
}
