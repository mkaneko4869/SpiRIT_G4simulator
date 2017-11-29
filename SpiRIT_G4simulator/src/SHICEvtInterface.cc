#include "SHICEvtInterface.hh"
#include "globals.hh"
#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4INCLGlobals.hh"
#include "G4SystemOfUnits.hh"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>


SHICEvtInterface::SHICEvtInterface(std::string evfile)
  : fileName(), inputFile(0), gentype(0), fImp(-1.), fPhi(-999.)
{
#include "SDetectorParameterDef.icc"
  inputFile.open(evfile.c_str(),std::ios::in);
  if(inputFile.is_open()){
    fileName = evfile;
    if( fileName.find(".f14.clu") != std::string::npos ){
      std::cout<<"SHICEvtInterface - "<<fileName<<" is open."<<std::endl;
      gentype = 1;
    }else if( fileName.find("phits") != std::string::npos ){
      std::cout<<"SHICEvtInterface - "<<fileName<<" is open."<<std::endl;
      gentype = 2;
    }else if( fileName.find("amd") != std::string::npos ){
      std::cout<<"SHICEvtInterface - "<<fileName<<" is open."<<std::endl;
      gentype = 3;
    }else{
      std::cout<<"SHICEvtInterface - Error!!"<<std::endl;
      std::cout<<"Unknown input data file!!"<<std::endl;
    }

    LoadHeader();

  }else{
    std::cout<<"SHICEvtInterface - Error!!"<<std::endl;
    std::cout<<"File is not readable!!"<<std::endl;
  }
  
  G4ThreeVector zero;
  particle_position = zero; // defined in the G4VPrimaryGenerator
  particle_time = 0.0;

}

SHICEvtInterface::~SHICEvtInterface()
{
  std::cout<<"Destructor of SHICEvtInterface"<<std::endl;

}


void SHICEvtInterface::LoadHeader()
{
  std::string line;
  int Abeam, Zbeam, Atgt, Ztgt;
  float Ebeam;
  if(gentype==0) std::cout<<"File is unknown."<<std::endl;
  else if(gentype==1){ // read UrQMD file
    char buffer[100];
    float bmin, bmax;
    std::getline(inputFile,line); 
    sscanf(line.c_str(),"%17c%d%d%d%d",buffer,&Abeam,&Zbeam,&Atgt,&Ztgt);
    std::getline(inputFile,line); 
    sscanf(line.c_str(),"%17c%f%f%f",buffer,&Ebeam,&bmin,&bmax);
    for(int i=0;i<7;i++) getline(inputFile,line);
    std::cout<<"======== URQMD INPUT DATA INFORMATION ========"<<std::endl;
  }
  else if(gentype==2){ // read PHITS file
    std::getline(inputFile,line);
    sscanf(line.data(),"%d%d%d%d%f",&Abeam,&Zbeam,&Atgt,&Ztgt,&Ebeam);
    std::cout<<"======== PHITS INPUT DATA INFORMATION ========"<<std::endl;
  }
  else if(gentype==3){ // read AMD file
    std::getline(inputFile,line);
    sscanf(line.data(),"%d%d%d%d%f",&Abeam,&Zbeam,&Atgt,&Ztgt,&Ebeam);
    std::cout<<"======== AMD INPUT DATA INFORMATION ========"<<std::endl;
  }

  std::cout<<" Abeam, Zbeam, Atarget, Ztarget:"<<Abeam<<", "<<Zbeam<<", "<<Atgt<<", "<<Ztgt<<std::endl;
  std::cout<<" Ebeam= "<<Ebeam<<" MeV/A"<<std::endl;
  std::cout<<"=============================================="<<std::endl;

}

void SHICEvtInterface::GeneratePrimaryVertex(G4Event* evt)
{
   std::string event_info;
   if(inputFile.is_open())
     std::getline(inputFile,event_info);
   else 
     G4Exception("SHICEvtInterface::GeneratePrimaryVertex","Code",FatalException,"SHICEvtInterface:: cannot open file.");

   if(inputFile.eof()){ // End Of File
     G4Exception("SHICEvtInterface::GeneratePrimaryVertex","Code",JustWarning,"End-Of-File: SHICEvt input file");
     inputFile.close();
     return;
   }
  
   // Event Reader
   int eventNo, npbloc, ncoll, ncoll0, npart;
   float reac_time, etrans;
   double imp;
   if(gentype==1) 
     sscanf(event_info.data(),
         "%f%d%d%lf%f%d%d%d",
         &reac_time,&eventNo,&npart,&imp,&etrans,&npbloc,&ncoll,&ncoll0);
   else if(gentype==2||gentype==3) 
     sscanf(event_info.data(),"%lf %d",&imp,&npart);
   

   // Read Fragment data
   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
   G4IonTable* ionTable = G4IonTable::GetIonTable();
   G4ParticleDefinition* particle = NULL;
   std::vector<G4PrimaryParticle*> partlist;
   G4double rand_phi = G4UniformRand()*G4INCL::Math::twoPi; // rotate reaction plane randomly.
   for(int ipart=0; ipart<npart; ipart++){
     int Apart, Zpart, kf=-999;
     double x[3], p[3];
     std::string part_info = "";
     std::getline(inputFile,part_info);
     if(gentype==1){ // for UrQMD data
       sscanf(part_info.data(),"%d%d%lf%lf%lf%lf%lf%lf",
               &Apart,&Zpart,&p[0],&p[1],&p[2],&x[0],&x[1],&x[2]);
       // set PDG code for baryon, meson.
       if(Apart==1&&Zpart==1) kf=2212; //proton
       else if(Apart==1&&Zpart==0) kf=2112; //neutron
       else if(Apart==-1&&Zpart==-1) kf=-211; //pi minus
       else if(Apart==-1&&Zpart==1) kf=211; //pi plus
       else if(Apart==-1&&Zpart==0) kf=111; //pi zero
     
     }else if(gentype==2){ // for PHITS data.
        double mass, Ekin;
        sscanf(part_info.data(),
               "%d%d%d%lf%lf%lf%lf%lf%lf%lf%lf",
               &kf,&Zpart,&Apart,&mass,&Ekin,&p[0],&p[1],&p[2],&x[0],&x[1],&x[2]);
        G4double E = Ekin + mass;
        G4double P = std::sqrt( E*E - mass*mass );
        p[0]*=P; p[1]*=P; p[2]*=P; // PHITS output has momentum unit vector.
         
     }else if(gentype==3){ // for AMD data.
        int Npart; double Ekin;
        sscanf(part_info.data(),
                "%d%d%d%lf%lf%lf%lf",
                &kf,&Zpart,&Npart,&Ekin,&p[0],&p[1],&p[2]);
        Apart = Zpart+Npart;
     }
        
     if(particleTable->FindParticle(kf))
       particle = particleTable->FindParticle(kf);
     else if(ionTable->GetIon(Zpart,Apart,0.0)) 
       particle = ionTable->GetIon(Zpart,Apart,0.0);
     else{
       particle = particleTable->FindParticle(0); // set geantino
       std::cout<<"Set Geantino!! Unknown particle!!"<<std::endl;
       std::cout<<part_info.data()<<std::endl;
     }
     
     G4ThreeVector P(p[0],p[1],p[2]);
     P.rotateZ(rand_phi);
     P.rotateY(-0.06); // beam is rotated about 0.06 radian toward beam right side 
                         // by the magnet at the moment of hitting the target.
     G4double M = particle->GetPDGMass();
     G4double E = std::sqrt( P.mag2() + M*M );
     auto part = new G4PrimaryParticle(particle,P.x(),P.y(),P.z(),E);
     partlist.push_back(part);

   }
   
   // at least one fragment.
   if(partlist.size()==0) return;

   // set vertex in the event.
   G4ThreeVector tgtPos(fTargetPos_x,fTargetPos_y,fTargetPos_z);  
   G4ThreeVector tgtSize(fTarget_x,fTarget_y,fTarget_z);  

   particle_position.set( G4RandGauss::shoot(tgtPos.x(),5.*mm),
                           G4RandGauss::shoot(tgtPos.y(),5.*mm),
                           tgtPos.z()+(G4UniformRand()-0.5)*tgtSize.z() );

   auto vertex = new G4PrimaryVertex(particle_position,particle_time);
   for(size_t ipart=0; ipart<partlist.size(); ipart++){
      vertex->SetPrimary(partlist.at(ipart));
      //partlist.at(ipart)->Print();
   }
  
  /* 
   for(size_t ipart=0; ipart<partlist.size(); ipart++){
     std::cout<< ipart << std::endl;
      delete partlist[ipart];
   }*/
   partlist.clear();

   evt->AddPrimaryVertex( vertex );
   
   SetImp(imp);
   SetPhiRP(rand_phi);
   SetVertexPos(particle_position);
}
