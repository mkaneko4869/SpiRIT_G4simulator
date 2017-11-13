#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#include "SDetectorConstruction.hh"
#include "SPrimaryGeneratorAction.hh"
#include "SPhysicsList.hh"
#include "SRunAction.hh"
#include "SEventAction.hh"
#include "STrackingAction.hh"
#include "SSteppingAction.hh"

#include "SActionInitialization.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#include "G4TrajectoryDrawByParticleID.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

#ifdef G4MULTITHREADED
  G4int nThreads = 2;
  auto runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(nThreads);
#else
  auto runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  runManager->SetUserInitialization(new SDetectorConstruction);
  runManager->SetUserInitialization(new SPhysicsList);
  runManager->Initialize();
  runManager->SetUserInitialization(new SActionInitialization);


  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();

  if (argc!=1) {  // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { // interactive mode : define UI session
    
#ifdef G4VIS_USE
    auto visManager = new G4VisExecutive;
    visManager->Initialize();

    auto tcol = new G4TrajectoryDrawByParticleID;
    tcol->SetDefault(G4Colour(0.5,0.5,0.5));
    tcol->Set("proton",G4Colour(1,0.,0.,0.9));
    tcol->Set("neutron",G4Colour(0.1,0.1,0.8,0.2));
    tcol->Set("pi-","green");
    tcol->Set("pi+","blue");
    tcol->Set("pi0","yellow");
    tcol->Set("deuteron","magenta");
    tcol->Set("triton","brown");
    tcol->Set("alpha","cyan");
    tcol->Set("e-",G4Colour(0.5,0.8,0.2,0.3));
    tcol->Set("e+",G4Colour(0.8,0.2,0.5,0.3));
    visManager->RegisterModel(tcol);
    visManager->SelectTrajectoryModel(tcol->Name());

#ifdef G4UI_USE
    auto ui = new G4UIExecutive(argc, argv,"qt");
#endif
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
    UImanager->ApplyCommand("/control/execute init.mac"); 
#endif

#ifdef G4UI_USE
    ui->SessionStart();
    delete ui;
#endif

#ifdef G4VIS_USE
    delete visManager;
#endif 
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
  delete runManager;

  return 0;
}
