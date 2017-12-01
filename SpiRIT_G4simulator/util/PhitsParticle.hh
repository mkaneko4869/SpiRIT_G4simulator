#include "TObject.h"

class PhitsParticle : public TObject
{
  public:
    PhitsParticle() { kf=0; }
    virtual ~PhitsParticle() {}

    Int_t kf; // kf code, see page 14 of http://phits.jaea.go.jp/manual/manualJ-phits.pdf
    Int_t q; // charge
    Int_t A; // mass number
    Double_t m; // rest mass [MeV]
    Double_t ke; // kinetic energy [MeV/c2]
    Double_t pos[3]; // position [mm]
    Double_t mom[3]; // momentum in MeV for colpart, unit vec for part

    Double_t brho; // brho
    Double_t xang; // atan(px/pz)
    Double_t yang; // atan(py/pz)

    ClassDef(PhitsParticle, 1);
};
