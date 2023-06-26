
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fstream>
#include <string>
#include <phool/recoConsts.h>

#include "/sphenix/u/jocl/testinstall/include/particleflowreco/MDCTreeMaker.h"


#include <frog/FROG.h>

R__LOAD_LIBRARY(libFROG.so)
R__LOAD_LIBRARY(libg4jets.so)
#include <g4jets/FastJetAlgo.h>
#include <g4jets/JetReco.h>
#include <g4jets/TowerJetInput.h>
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
  std::ifstream calofile("../../dst_calo_cluster.list");
  std::ifstream jetsfile("../../dst_truth_jet.list");
  std::string calostr;
  std::string jetstr;
  //in_1->AddListFile("dst...whatever");
  JetReco *towerjetreco = new JetReco("TOWERJETRECO");
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWER));
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.2), "AntiKt_Tower_r02");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.3), "AntiKt_Tower_r03");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_Tower_r04");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Tower_r05");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.6), "AntiKt_Tower_r06");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_Tower_r07");
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.8), "AntiKt_Tower_r08");
  towerjetreco->set_algo_node("ANTIKT");
  towerjetreco->set_input_node("TOWER");
  towerjetreco->Verbosity(0);

  MDCTreeMaker *tt = new MDCTreeMaker( "test_03.root"  );
  Fun4AllInputManager *in_1 = new Fun4AllDstInputManager("DSTin1");
  Fun4AllInputManager *in_2 = new Fun4AllDstInputManager("DSTin2");
  se->registerInputManager( in_1 );
  se->registerInputManager( in_2 );
  se->registerSubsystem( tt );
  se->registerSubsystem(towerjetreco);
  while(getline(calofile, calostr) && getline(jetsfile, jetstr))
  {
    in_1->fileopen( fr->location(jetstr) );
    in_2->fileopen( fr->location(calostr) );

    int verbosity = 0;  
    se->run( 0 );
  }
  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  
  //gSystem->Exit(0);
  
  return 0;

}
