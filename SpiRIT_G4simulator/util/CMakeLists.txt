ROOT_GENERATE_DICTIONARY(STSim_Dict STSimParticle.hh STSimTracking.hh STSimScintillator.hh PhitsParticle.hh LINKDEF linkdef.hh)

add_library(STSim SHARED STSimParticle.cc STSimTracking.cc STSimScintillator.cc PhitsParticle.cc STSim_Dict.cxx)
target_link_libraries(STSim ${ROOT_LIBRARIES})
