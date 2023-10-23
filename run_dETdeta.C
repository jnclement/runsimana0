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
#include <g4bbc/BbcDigitization.h>
#include <bbc/BbcReco.h>
#include <frog/FROG.h>

using namespace std;

R__LOAD_LIBRARY(libFROG.so)

R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libg4vertex.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmdctreemaker.so)
R__LOAD_LIBRARY(libcalo_io.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libg4bbc.so)
int run_dETdeta(int nproc = 0, string tag = "", int datormc = 0, int debug = 0, int nevt = 0, int correct = 1)
{
  int verbosity = 0;
  string filename = "output/evt/events_"+tag+(tag==""?"":"_");
  string dattag = (datormc?"mc":"data");
  string cortag = (correct?"cor":"unc");
  filename += dattag + "_"+cortag+"_" + to_string(nproc);
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
  ifstream list4;
  string line4;
  if(datormc)
    {
      list1.open("/sphenix/user/jocl/projects/sandbox/run/dst_calo_nozero.list");
      list2.open("/sphenix/user/jocl/projects/sandbox/run/dst_global.list");
      list3.open("/sphenix/user/jocl/projects/sandbox/run/dst_truth.list");
      list4.open("/sphenix/user/jocl/projects/sandbox/run/g4hits.list");
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
	  getline(list4, line4);
	  getline(list2, line2);
	  getline(list3, line3);
	}
    }

  Fun4AllInputManager *in_1 = new Fun4AllDstInputManager("DSTin1");
  Fun4AllInputManager *in_2 = new Fun4AllDstInputManager("DSTin2");
  Fun4AllInputManager *in_3 = new Fun4AllDstInputManager("DSTin3");
  Fun4AllInputManager *in_4 = new Fun4AllDstInputManager("DSTin4");
  in_1->AddFile(line1);
  if(datormc)
    {
      in_2->AddFile(line2);
      in_3->AddFile(line3);
      in_4->AddFile(line4);
    }
  se->registerInputManager( in_1 );
  if(datormc)
    {
      se->registerInputManager(in_2);
      se->registerInputManager(in_3);
      se->registerInputManager(in_4);
    }
  rc->set_StringFlag("CDB_GLOBALTAG","ProdA_2023"); // this points to the global tag in the CDB
  // The calibrations have a validity range set by the beam clock which is not read out of the prdfs as of now
  rc->set_uint64Flag("TIMESTAMP",0);
  int cont = 0;
  auto bbcdigi = new BbcDigitization();
  auto bbcreco = new BbcReco();
  se->registerSubsystem(bbcdigi);
  se->registerSubsystem(bbcreco);
  MDCTreeMaker *tt = new MDCTreeMaker( filename, datormc, debug, correct );
  se->registerSubsystem( tt );
  se->Print("NODETREE");
  se->run(nevt);
  se->End();
  delete se;
  gSystem->Exit(0);
  
  return 0;

}
