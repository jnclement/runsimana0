
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>

#include <phool/recoConsts.h>

#include "/sphenix/user/decadal/dvp/testinstall/include/particleflowreco/MDCTreeMaker.h"


#include <frog/FROG.h>

R__LOAD_LIBRARY(libFROG.so)

R__LOAD_LIBRARY(libg4vertex.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmdctreemaker.so)

  int run_macro( )
{

  FROG *fr = new FROG();
    
  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libjetbackground.so");
  
  gSystem->Load("libmdctreemaker.so");
  
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity( 0 );
  // just if we set some flags somewhere in this macro
  //recoConsts *rc = recoConsts::instance();
  
  Fun4AllInputManager *in_1 = new Fun4AllDstInputManager("DSTin1");
  in_1->fileopen( fr->location("DST_TRUTH_JET_pythia8_Jet04_3MHz-0000000004-00000.root") );
  se->registerInputManager( in_1 );
  
  Fun4AllInputManager *in_2 = new Fun4AllDstInputManager("DSTin2");
  in_2->fileopen( fr->location("DST_CALO_CLUSTER_pythia8_Jet04_3MHz-0000000004-00000.root") );
  se->registerInputManager( in_2 );


  int verbosity = 0;

  MDCTreeMaker *tt = new MDCTreeMaker( "test.root"  );
  se->registerSubsystem( tt );
  
  se->run( 10 );
  
  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  
  //gSystem->Exit(0);
  
  return 0;

}
