#include "SMagField.hh"
#include "globals.hh"

//#include "TString.h"
#include "G4SystemOfUnits.hh"

#include <string>
#include <fstream>
#include <iostream>

SMagField::SMagField()
  :IsLoaded(0)
{
  LoadMagneticField();
}

////////////////////////////////////////////////////////////////////////

SMagField::~SMagField()
{
}

////////////////////////////////////////////////////////////////////////

void SMagField::LoadMagneticField()
{
//  char buffer[256];
  //std::string magfile = "/home/kaneko/GEANT4/G4simulations/120405-bn_stage-d-51,3A-0,5T-map.bin";
  const char* magfilepath = std::getenv("SAMURAI_MAGFIELD_FILE");
  std::string magfile = std::string(magfilepath);

  //if(magfile.EndsWith("bin")){ // if it is binary.
    std::ifstream magin(magfile.data(),std::ios::in|std::ios::binary);
    if(magin.is_open()){
      std::cout << "succeed to get magnetic field data from: " << magfile.data() << std::endl;
      magin.read((char *)Bx,sizeof(Bx));
      magin.read((char *)By,sizeof(By));
      magin.read((char *)Bz,sizeof(Bz));
      magin.close();
    }
    else{
      std::cout << "fail to get magnetic field data from: " << magfile.data() << std::endl;
      magin.close();
    }
 // }
 // else{
 //   std::cout<< "Unknown type of magnetic field file!!" <<std::endl;
 // }
/* 
  else if(magfile.EndsWith("table")){ // supposed to be usual ascii file

    std::cout << "opening: " << magfile.data() << std::endl;
    std::ifstream magfin(magfile.data()); 
    for(int i=0;i<8;i++)  magfin.getline(buffer,256);

    G4double v[7];
    
    while(magfin>>v[0]>>v[1]>>v[2]>>v[3]>>v[4]>>v[5]){
      v[1] += 400;
      //v[1] = 400 - v[1];
      //      G4cout << " x " << (v[0]/10.) << " y " << (v[1]/10.) << " z " << (v[2]/10.) << G4endl;
      Bx[(int)(v[0]/10.)][(int)(v[1]/10.)][(int)(v[2]/10.)] = fabs(v[3]) > 0.0001 ? v[3]*tesla : 0;
      By[(int)(v[0]/10.)][(int)(v[1]/10.)][(int)(v[2]/10.)] = fabs(v[4]) > 0.0001 ? v[4]*tesla : 0;
      Bz[(int)(v[0]/10.)][(int)(v[1]/10.)][(int)(v[2]/10.)] = fabs(v[5]) > 0.0001 ? v[5]*tesla : 0;
    }
 else {
    std::cout << "can not identify file type: " << magfile.data() << std::endl;
  }
   
 magfin.close();

    magfile.Remove(magfile.Length()-2);
    magfile.Replace(magfile.Length()-3,3,"bin");
    std::cout << "making new binary file: " << magfile.Data() << std::endl;
    std::ofstream magout(magfile.Data(),std::ios::out|std::ios::binary);
    magout.write((char *)Bx,sizeof(Bx));
    magout.write((char *)By,sizeof(By));
    magout.write((char *)Bz,sizeof(Bz));
    magout.close();

  }
*/

  IsLoaded = true;
  return;

}

///////////////////////////////////////////////////////////////////////

void SMagField::GetFieldValue( const G4double Pos[4], G4double *B ) const 
{

  if(!IsLoaded) G4cout << "load the magnetic field map!!" << G4endl;
  // af[*] is correction for symmetric axis. 

  G4double x[3], d[3], af[3], dsqt[3], mdsq[3];
  G4int n[3];
  for(int i=0;i<3;i++){
    x[i] = fabs(Pos[i]);
    af[i] = Pos[i] < 0 ? -1 : 1;
  }

  if( x[0]>=300*cm || x[1]>=40*cm || x[2]>=300*cm ){ // out of magnetic field
    B[0]=0; B[1]=0; B[2]=0; return;
  }

  for(int i=0;i<3;i++){
    n[i] = i == 1 ? (int)((Pos[i]+40*cm)/cm) : (int)(x[i]/cm);
    B[i]=0;
    d[i] = i == 1 ? (Pos[i]+40*cm)/cm - n[i]: x[i]/cm - n[i];
    //    dsqt[i] = d[i] * d[i];
    //    mdsq[i] = 1 - d[i] * d[i];
    if(0 == d[i]){
      mdsq[i] = 1;
      dsqt[i] = 0;
    }
    else if(1 == d[i]){
      mdsq[i] = 0;
      dsqt[i] = 1;
    }
    else{
      double sum = 1/d[i] + 1/(1.-d[i]);
      mdsq[i] = 1/d[i]/sum;
      dsqt[i] = 1 - mdsq[i];
    }

  }

  B[0] = mdsq[0]*mdsq[1]*mdsq[2]*Bx[n[0]  ][n[1]  ][n[2]  ] +
         dsqt[0]*mdsq[1]*mdsq[2]*Bx[n[0]+1][n[1]  ][n[2]  ] +
         mdsq[0]*dsqt[1]*mdsq[2]*Bx[n[0]  ][n[1]+1][n[2]  ] +
         mdsq[0]*mdsq[1]*dsqt[2]*Bx[n[0]  ][n[1]  ][n[2]+1] +
         dsqt[0]*dsqt[1]*mdsq[2]*Bx[n[0]+1][n[1]+1][n[2]  ] +
         dsqt[0]*mdsq[1]*dsqt[2]*Bx[n[0]+1][n[1]  ][n[2]+1] +
         mdsq[0]*dsqt[1]*dsqt[2]*Bx[n[0]  ][n[1]+1][n[2]+1] +
         dsqt[0]*dsqt[1]*dsqt[2]*Bx[n[0]+1][n[1]+1][n[2]+1];

  B[1] = mdsq[0]*mdsq[1]*mdsq[2]*By[n[0]  ][n[1]  ][n[2]  ] +
         dsqt[0]*mdsq[1]*mdsq[2]*By[n[0]+1][n[1]  ][n[2]  ] +
         mdsq[0]*dsqt[1]*mdsq[2]*By[n[0]  ][n[1]+1][n[2]  ] +
         mdsq[0]*mdsq[1]*dsqt[2]*By[n[0]  ][n[1]  ][n[2]+1] +
         dsqt[0]*dsqt[1]*mdsq[2]*By[n[0]+1][n[1]+1][n[2]  ] +
         dsqt[0]*mdsq[1]*dsqt[2]*By[n[0]+1][n[1]  ][n[2]+1] +
         mdsq[0]*dsqt[1]*dsqt[2]*By[n[0]  ][n[1]+1][n[2]+1] +
         dsqt[0]*dsqt[1]*dsqt[2]*By[n[0]+1][n[1]+1][n[2]+1];

  B[2] = mdsq[0]*mdsq[1]*mdsq[2]*Bz[n[0]  ][n[1]  ][n[2]  ] +
         dsqt[0]*mdsq[1]*mdsq[2]*Bz[n[0]+1][n[1]  ][n[2]  ] +
         mdsq[0]*dsqt[1]*mdsq[2]*Bz[n[0]  ][n[1]+1][n[2]  ] +
         mdsq[0]*mdsq[1]*dsqt[2]*Bz[n[0]  ][n[1]  ][n[2]+1] +
         dsqt[0]*dsqt[1]*mdsq[2]*Bz[n[0]+1][n[1]+1][n[2]  ] +
         dsqt[0]*mdsq[1]*dsqt[2]*Bz[n[0]+1][n[1]  ][n[2]+1] +
         mdsq[0]*dsqt[1]*dsqt[2]*Bz[n[0]  ][n[1]+1][n[2]+1] +
         dsqt[0]*dsqt[1]*dsqt[2]*Bz[n[0]+1][n[1]+1][n[2]+1];

  B[0] *= af[0];// * af[1]; y-axis is not symmetric now
  B[2] *= af[2];// * af[1]; y-axis is not symmetric now

  return ;
}

// -----------------------------------------------------------------
