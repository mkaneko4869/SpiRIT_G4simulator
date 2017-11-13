#ifndef SRegionInformation_h
#define SRegionInformation_h 1

#include "globals.hh"
#include "G4VUserRegionInformation.hh"

class SRegionInformation : public G4VUserRegionInformation
{
  public:
    SRegionInformation();
    virtual ~SRegionInformation();
    virtual void Print() const;

    inline void SetWorld(G4bool v=true){fIsWorld = v;}
    inline void SetSpiRITSensitive(G4bool v=true){fIsSpiRITSensitive = v;}
    inline void SetKyotoScinti(G4bool v=true){fIsKyotoScinti = v;}
    inline void SetKatanaVScinti(G4bool v=true){fIsKatanaVScinti = v;}
    inline void SetKatanaMScinti(G4bool v=true){fIsKatanaMScinti = v;}
    inline void SetVacuumChamber(G4bool v=true){fIsVC=v;}

    inline G4bool IsSpiRITSensitive() const { return fIsSpiRITSensitive; }
    inline G4bool IsWorld() const { return fIsWorld; }
    inline G4bool IsVC() const { return fIsVC; }

  private:
    G4bool fIsWorld;
    G4bool fIsSpiRITSensitive;
    G4bool fIsKyotoScinti;
    G4bool fIsKatanaVScinti;
    G4bool fIsKatanaMScinti;
    G4bool fIsVC;
};

#endif
