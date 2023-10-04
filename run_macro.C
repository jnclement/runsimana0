#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>

#include <jetbase/FastJetAlgo.h>
#include <jetbase/JetReco.h>
#include <jetbase/TowerJetInput.h>
#include <g4jets/TruthJetInput.h>
#include <fstream>
#include <phool/recoConsts.h>
#include <TSystem.h>
#include "mdctreemaker/MDCTreeMaker.h"


#include <frog/FROG.h>

using namespace std;

R__LOAD_LIBRARY(libFROG.so)

R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libg4vertex.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmdctreemaker.so)
R__LOAD_LIBRARY(libcalo_io.so)

int run_macro(int nproc = 0)
{
  int verbosity = 0;
  string filename = "events_";
  filename += to_string(nproc);
  filename += ".root";
  FROG *fr = new FROG();
    
  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libjetbackground.so");
  gSystem->Load("libcalo_io.so");
  gSystem->Load("libmdctreemaker.so");
  
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity( verbosity );
  // just if we set some flags somewhere in this macro
  recoConsts *rc = recoConsts::instance();
  ifstream list1;
  ifstream list2;
  //ifstream list3;
  //ifstream list4;
  string line1;
  string line2;
  //string line3;
  //string line4;
  //list1.open("/sphenix/user/jocl/projects/sandbox/run/dsts_dEdeta_study");
  list1.open("/sphenix/user/jocl/projects/sandbox/run/dst_global.list", ifstream::in);
  list2.open("/sphenix/user/jocl/projects/sandbox/run/dst_calo_cluster.list", ifstream::in);
  //list3.open("/sphenix/user/jocl/projects/sandbox/run/dst_calo_cluster.list", ifstream::in);
  //list4.open("/sphenix/user/jocl/projects/sandbox/run/dst_truth_jet.list", ifstream::in);
  for(int i=0; i<nproc+1; i++)
    {
      getline(list1, line1);
      getline(list2, line2);
      //getline(list3, line3);
      //getline(list4, line4);
    }
  cout <<"Filename: "<< line1 << endl;

  Fun4AllInputManager *in_1 = new Fun4AllDstInputManager("DSTin1");
  in_1->AddFile(line1);
  se->registerInputManager( in_1 );
  
  Fun4AllInputManager *in_2 = new Fun4AllDstInputManager("DSTin2");
  in_2->AddFile(line2);
  se->registerInputManager( in_2 );
  /*
  Fun4AllInputManager *in_3 = new Fun4AllDstInputManager("DSTin3");
  in_3->AddFile(line3);//"/sphenix/user/jocl/projects/sandbox/dst_truth.list");
  se->registerInputManager( in_3 );

  Fun4AllInputManager *in_4 = new Fun4AllDstInputManager("DSTin4");
  in_4->AddFile(line4);
  se->registerInputManager( in_4 );
  */
  //cout << "test1" << endl;
  // tower jets
  /*
  JetReco *towerjetreco = new JetReco("TOWERJETRECO");
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWER));
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_Tower_r04");
  towerjetreco->set_algo_node("ANTIKT");
  towerjetreco->set_input_node("TOWER");
  towerjetreco->Verbosity( verbosity );
  se->registerSubsystem(towerjetreco);

  JetReco *truthjetreco = new JetReco("TRUTHJETRECO");
  truthjetreco->add_input(new TruthJetInput(Jet::PARTICLE));
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_Truth_r04");
  truthjetreco->set_algo_node("ANTIKT");
  truthjetreco->set_input_node("TRUTH");
  se->registerSubsystem(truthjetreco);
  se->Print("NODETREE");
  int cont = 0;
  */
  // cin >> cont;
  cout << "test2" << endl;
  MDCTreeMaker *tt = new MDCTreeMaker( filename );
  //cout << "test3" << endl;
  se->registerSubsystem( tt );
  cout << "test4" << endl;
  se->Print("NODETREE");
  se->run();
  cout << "test5" << endl;
  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  //cout << "test6" << endl;
  gSystem->Exit(0);
  
  return 0;

}
