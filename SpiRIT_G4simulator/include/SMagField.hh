#ifndef SMAGFIELD_HH
#define SMAGFIELD_HH

#include "G4MagneticField.hh"

class SMagField : public G4MagneticField
{
public:  // with description
                       
  SMagField();
  ~SMagField();

  void GetFieldValue(const G4double Pos[4], G4double *B) const;
  void LoadMagneticField();

private:
  G4bool IsLoaded;
  G4double Bx[301][81][301];
  G4double By[301][81][301];
  G4double Bz[301][81][301];

};

#endif
