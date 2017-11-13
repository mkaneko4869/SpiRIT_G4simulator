
#ifndef STrackingAction_h
#define STrackingAction_h 1

#include "G4UserTrackingAction.hh"

class STrackingAction : public G4UserTrackingAction 
{
public:
  STrackingAction();
  virtual ~STrackingAction(){};
   
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);
  
};

#endif
