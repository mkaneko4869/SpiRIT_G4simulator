#ifndef KatanaMScintiSD_h
#define KatanaMScintiSD_h 1
 
#include "G4VSensitiveDetector.hh"
#include "ScintillatorHit.hh"
#include <vector>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class KatanaMScintiSD : public G4VSensitiveDetector
{
  public:
    KatanaMScintiSD(G4String name);
    virtual ~KatanaMScintiSD();
    
    virtual void Initialize(G4HCofThisEvent* HCTE);
    virtual void EndOfEvent(G4HCofThisEvent* HCTE);
  
  protected:  
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  
  private:
    const int fNumberOfBar;
    
    ScintillatorHitsCollection * fScintiCollection;
    std::vector<G4int> fHitColID[12]; // hit collection ID
    std::vector<G4int> fTrackID[12]; // 
};

#endif
