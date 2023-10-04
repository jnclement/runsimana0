#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/SubsysReco.h>
#include <jetbase/FastJetAlgo.h>
#include <jetbase/JetReco.h>
#include <jetbase/TowerJetInput.h>
#include <g4jets/TruthJetInput.h>
#include <fstream>
#include <phool/recoConsts.h>
#include <TSystem.h>
#include "mdctreemaker/MDCTreeMaker.h"
#include <caloreco/CaloTowerCalib.h>

#include <frog/FROG.h>

using namespace std;

R__LOAD_LIBRARY(libFROG.so)

R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libg4vertex.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmdctreemaker.so)
R__LOAD_LIBRARY(libcalo_io.so)
R__LOAD_LIBRARY(libcalo_reco.so)
int run_dETdeta(int nproc = 0, string tag = "", int datormc = 0, int debug = 0)
{
  int verbosity = 0;
  string filename = "events_"+tag+(tag==""?"":"_");
  string dattag = (datormc?"mc":"data");
  filename += dattag + "_" + to_string(nproc);
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
  string line1;
  ifstream list2;
  string line2;
  ifstream list3;
  string line3;
  if(datormc)
    {
      list1.open("/sphenix/user/jocl/projects/sandbox/run/dst_calo_cluster.list");
      list2.open("/sphenix/user/jocl/projects/sandbox/run/dst_global.list");
      list3.open("/sphenix/user/jocl/projects/sandbox/run/dst_truth.list");
    }
  else
    {
      list1.open("/sphenix/user/jocl/projects/sandbox/run/dsts_dEdeta_study", ifstream::in);
    }
  for(int i=0; i<nproc+1; i++)
    {
      getline(list1, line1);
      if(datormc)
	{
	  getline(list2, line2);
	  getline(list3, line3);
	}
    }

  Fun4AllInputManager *in_1 = new Fun4AllDstInputManager("DSTin1");
  Fun4AllInputManager *in_2 = new Fun4AllDstInputManager("DSTin2");
  Fun4AllInputManager *in_3 = new Fun4AllDstInputManager("DSTin3");
  in_1->AddFile(line1);
  if(datormc)
    {
      in_2->AddFile(line2);
      in_3->AddFile(line3);
    }
  se->registerInputManager( in_1 );
  if(datormc)
    {
      se->registerInputManager(in_2);
      se->registerInputManager(in_3);
    }
  rc->set_StringFlag("CDB_GLOBALTAG","ProdA_2023"); // this points to the global tag in the CDB
  // The calibrations have a validity range set by the beam clock which is not read out of the prdfs as of now
  rc->set_uint64Flag("TIMESTAMP",0);
  int cont = 0;
  MDCTreeMaker *tt = new MDCTreeMaker( filename, datormc, debug );
  se->registerSubsystem( tt );
  se->Print("NODETREE");
  se->run();
  se->End();
  delete se;
  gSystem->Exit(0);
  
  return 0;

}
