#ifndef SpiRITSD_h
#define SpiRITSD_h 1

#include "G4VSensitiveDetector.hh"
#include "TrackerHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class SpiRITSD : public G4VSensitiveDetector
{
  public:
    SpiRITSD(G4String name);
    virtual ~SpiRITSD();

    virtual void Initialize(G4HCofThisEvent *HCE);
    virtual void EndOfEvent(G4HCofThisEvent *HCE);

  protected:
    virtual G4bool ProcessHits(G4Step* astep, G4TouchableHistory* ROhist);

  private:
    TrackerHitsCollection* fTrackerCollection;

};

#endif
