#!/bin/bash

### execute this setup script by "source setup.sh" at where this script is located to reflect the environment without elimination.

MYDIR=/cache/scr/spirit/kaneko

### for using ROOT6
source /opt/rh/devtoolset-2/enable;
source /opt/rh/python27/enable;
#SpiRITROOT root version is 6.06.02!
#source $MYDIR/tool/root/6.06.06/install/bin/thisroot.sh
source $MYDIR/tool/geant/4.10.01.p03/install/bin/geant4.sh

PWD=`pwd`
SIMDIR=$MYDIR/simulation/geant
#S410SETUPURQMD
export SAMURAI_MAGFIELD_FILE=$SIMDIR/SpiRIT_G4simulator/magfieldmap/120405-bn_stage-d-51,3A-0,5T-map.bin
export G4WORKDIR=$SIMDIR/build
export G4SYSTEM="Linux-g++"
export G4INST=$MYDIR/tool/geant/4.10.01.p03/install
export G4LIB=$G4INST/lib64/Geant4-10.1.3
export G4INCLUDE=$G4INST/include/Geant4
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$G4LIB
export PATH=$PATH:$G4INCLUDE

export G4NEUTRONHPDATA=$G4INST/share/Geant4-10.1.3/data/G4NDL4.5
export G4LEDATA=$G4INST/share/Geant4-10.1.3/data/G4EMLOW6.41
export G4LEVELGAMMADATA=$G4INST/share/Geant4-10.1.3/data/PhotonEvaporation3.1
export G4RADIOACTIVEDATA=$G4INST/share/Geant4-10.1.3/data/RadioactiveDecay4.2
export G4NEUTRONXSDATA=$G4INST/share/Geant4-10.1.3/data/G4NEUTRONXS1.4
export G4PIIDATA=$G4INST/share/Geant4-10.1.3/data/G4PII1.3
export G4REALSURFACEDATA=$G4INST/share/Geant4-10.1.3/data/RealSurface1.0
export G4SAIDXSDATA=$G4INST/share/Geant4-10.1.3/data/G4SAIDDATA1.1


URQMDDIR=$MYDIR/simulation/urqmd
PHITSDIR=$MYDIR/simulation/phits
AMDDIR=$MYDIR/simulation/amd

#export INPFILE=$URQMDDIR/108sn112sn270amevb0012_0.f14.clu
#export INPFILE=$URQMDDIR/132sn124sn270amevb0012_0.f14.clu
export INPFILE=$PHITSDIR/132Sn124Sn_20171130/phits_132sn124sn281amev_0.root


#export URQMD_INP=$URQMDDIR/132sn_124sn_300amev_b00-12.f14.clu
#export URQMD_INP=$URQMDDIR/132sn124sn270amev_0.f14.clu
#export URQMD_INP=$URQMDDIR/132sn124sn270amev_1.f14.clu
#export URQMD_INP=$URQMDDIR/108sn112sn270amev_0.f14.clu
#export URQMD_INP=$URQMDDIR/108sn112sn270amev_1.f14.clu
#export URQMD_INP=$URQMDDIR/132sn124sn270amevb0012_0.f14.clu
#export PHITS_INP=$PHITSDIR/112Sn124Sn/phits_112Sn124Sn_8-12fm_dsfactor200.dat
#export PHITS_INP=$PHITSDIR/112Sn124Sn/phits_112Sn124Sn_0-6fm.dat
#export PHITS_INP=$PHITSDIR/112Sn124Sn/phits_112Sn124Sn_6-8fm_dsfactor50.dat
#export PHITS_INP=$PWD/PHITSdata/phits_112Sn124Sn_a.dat
#export PHITS_INP=$PWD/PHITSdata/phits_132Sn124Sn.dat
#export AMD_INP=$AMDDIR/amd_SLy4_b57.dat
#export AMD_INP=$AMDDIR/amd_SLy4-L10_b57.dat

#export AMD_INP=$AMDDIR/Sn132Sn124_270amev_amd_nocluster_SLy4_b01.dat
#export AMD_INP=$AMDDIR/Sn132Sn124_270amev_amd_nocluster_SLy4_b13.dat
#export AMD_INP=$AMDDIR/Sn132Sn124_270amev_amd_nocluster_SLy4_b35.dat
#export AMD_INP=$AMDDIR/Sn132Sn124_270amev_amd_nocluster_SLy4_b57.dat
#export AMD_INP=$AMDDIR/Sn132Sn124_270amev_amd_cluster_SLy4-L108_b01.dat
#export AMD_INP=$AMDDIR/Sn132Sn124_270amev_amd_cluster_SLy4-L108_b13.dat
#export AMD_INP=$AMDDIR/Sn132Sn124_270amev_amd_cluster_SLy4-L108_b35.dat
#export AMD_INP=$AMDDIR/Sn132Sn124_270amev_amd_cluster_SLy4-L108_b57.dat
#export AMD_INP=$AMDDIR/Sn132Sn124_270amev_amd_cluster_SLy4-L108.dat
#export AMD_INP=$AMDDIR/Sn132Sn124_270amev_amd_nocluster_SLy4.dat
#export AMD_INP=$AMDDIR/Sn132Sn124_270amev_amd_nocluster_SLy4-L108.dat

#export PHITS_INP=$PHITSDIR/112Sn124Sn_170427/phits_112Sn124Sn_dsfactor20.dat
#export PHITS_INP=$PHITSDIR/112Sn124Sn_170427/phits_112Sn124Sn_0-6fm_dsfactor5.dat
#export PHITS_INP=$PHITSDIR/112Sn124Sn_170427/phits_112Sn124Sn_6-8fm_dsfactor20.dat
#export PHITS_INP=$PHITSDIR/112Sn124Sn_170427/phits_112Sn124Sn_8-12fm_dsfactor50.dat
#export PHITS_INP=$PHITSDIR/132Sn124Sn_170830/phits_132Sn124Sn_0.dat
