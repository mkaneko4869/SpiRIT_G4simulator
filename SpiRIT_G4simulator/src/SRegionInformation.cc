#include "SRegionInformation.hh"
#include "G4ios.hh"

SRegionInformation::SRegionInformation()
  : G4VUserRegionInformation(),
  fIsWorld(false), fIsSpiRITSensitive(false), 
  fIsKyotoScinti(false), fIsKatanaVScinti(false), fIsKatanaMScinti(false), fIsVC(false)
{;}

SRegionInformation::~SRegionInformation()
{;}

void SRegionInformation::Print() const
{
  G4cout << "I'm ";
  if(fIsWorld) { G4cout << "World."; }
  else if(fIsSpiRITSensitive) { G4cout << "SpiRIT sensitive area."; }
  else if(fIsKyotoScinti) { G4cout << "Kyoto scintillator."; }
  else if(fIsKatanaVScinti) { G4cout << "Katana veto scintillator."; }
  else if(fIsKatanaMScinti) { G4cout << "Katana multiplicity scintillator."; }
  else if(fIsVC) { G4cout << "SAMURAI vacuum chamber."; }
  else { G4cout << "unknown."; }
  G4cout<<G4endl;
}
