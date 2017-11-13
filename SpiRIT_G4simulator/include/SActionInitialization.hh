#ifndef SActionInitialization_h
#define SActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"

class SActionInitialization : public G4VUserActionInitialization
{
  public:
  SActionInitialization();
  virtual ~SActionInitialization();

  virtual void Build() const;
  virtual void BuildForMaster() const;

};

#endif
