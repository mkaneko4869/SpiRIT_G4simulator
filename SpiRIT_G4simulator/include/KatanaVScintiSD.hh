#ifndef KatanaVScintiSD_h
#define KatanaVScintiSD_h 1
 
#include "G4VSensitiveDetector.hh"
#include "ScintillatorHit.hh"
#include <vector>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class KatanaVScintiSD : public G4VSensitiveDetector
{
  public:
    KatanaVScintiSD(G4String name);
    virtual ~KatanaVScintiSD();
    
    virtual void Initialize(G4HCofThisEvent* HCTE);
    virtual void EndOfEvent(G4HCofThisEvent* HCTE);
  
  protected:  
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  
  private:
    const int fNumberOfBar;
    ScintillatorHitsCollection * fScintiCollection;
    std::vector<G4int> fHitColID[3]; // hit collection ID
    std::vector<G4int> fTrackID[3]; // 
};

#endif
