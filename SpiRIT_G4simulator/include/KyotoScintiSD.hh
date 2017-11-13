#ifndef KyotoScintiSD_h
#define KyotoScintiSD_h 1
 
#include "G4VSensitiveDetector.hh"
#include "ScintillatorHit.hh"
#include <vector>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class KyotoScintiSD : public G4VSensitiveDetector
{
  public:
    KyotoScintiSD(G4String name);
    virtual ~KyotoScintiSD();
    
    virtual void Initialize(G4HCofThisEvent* HCTE);
    virtual void EndOfEvent(G4HCofThisEvent* HCTE);
  
  protected:  
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  
  private:
    const int fNumberOfBar;
    
    ScintillatorHitsCollection * fScintiCollection;
    std::vector<G4int> fHitColID[60]; // hit collection ID
    std::vector<G4int> fTrackID[60]; // 
};

#endif
