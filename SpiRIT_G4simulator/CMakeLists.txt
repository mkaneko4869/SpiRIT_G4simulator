#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(SpiRIT_G4simulator)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Find ROOT package, activating all available UI and Vis drivers by default
#
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Net)

#----------------------------------------------------------------------------
# Setup ROOT and Geant4 include directories and compile definitions
#
include(${ROOT_USE_FILE} ${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/util
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})

add_subdirectory(util)  #directory for user-class for ROOT

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")

add_executable(samurai main.cc ${sources} ${headers})
#target_link_libraries(samurai ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(samurai STSim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})


# #----------------------------------------------------------------------------
# # Copy all scripts to the build directory, i.e. the directory in which we
# # build S1. This is so that we can run the executable directly because it
# # relies on these scripts being in the current working directory.
# #
set(SAMURAI_SCRIPTS
  init.mac
  init_vis.mac
  vis.mac
  vis.mac.batch
  vis.mac.test
  )

foreach(_script ${SAMURAI_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#

# add_custom_target(smanalysis_dict.cc
# 	COMMAND rootcint -f smanalysis_dict.cc -c -p -I../include TAnalysisBulk.hh TAnalysisInterface.hh TAnalysisNeutronDetector.hh TAnalysisTools.hh TSimulationDataBeam.hh TSimulationDataScintillator.hh TSimulationParameterGlobal.hh TSimulationParameterNeutronDetector.hh smanalysis_linkdef.hh
# 	DEPENDS include/TAnalysisBulk.hh 
# 	DEPENDS include/TAnalysisInterface.hh 
# 	DEPENDS include/TAnalysisNeutronDetector.hh 
# 	DEPENDS include/TAnalysisTools.hh 
# 	DEPENDS include/TSimulationDataBeam.hh 
# 	DEPENDS include/TSimulationDataScintillator.hh 
# 	DEPENDS include/TSimulationParameterGlobal.hh 
# 	DEPENDS include/TSimulationParameterNeutronDetector.hh 
# 	DEPENDS src/smanalysis_linkdef.hh
  	#WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src
# 	COMMENT "building smanalysis_dict.cc")

add_custom_target(SpiRIT_G4simulator DEPENDS samurai)

#add_dependencies(samurai smanalysis_dict.cc)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS samurai DESTINATION bin)
