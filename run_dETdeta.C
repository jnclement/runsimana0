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
#include <g4mbd/MbdDigitization.h>
#include <mbd/MbdReco.h>
#include <frog/FROG.h>
#include <centrality/DansSpecialVertex.h>
#include <ffamodules/CDBInterface.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <g4centrality/PHG4CentralityReco.h>
#include <G4Setup_sPHENIX.C>
#include </sphenix/user/jocl/projects/sandbox/HIJINGcorrection/source/EnergyCorrection.h>
using namespace std;

R__LOAD_LIBRARY(libg4centrality.so)
R__LOAD_LIBRARY(libEnergyCorrection.so)
R__LOAD_LIBRARY(libFROG.so)
R__LOAD_LIBRARY(libdansspecialvertex.so)
R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libg4vertex.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libmdctreemaker.so)
R__LOAD_LIBRARY(libcalo_io.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libg4mbd.so)
R__LOAD_LIBRARY(libmbd_io.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libffamodules.so)
bool file_exists(const char* filename)
{
  std::ifstream infile(filename);
  return infile.good();
}
int run_dETdeta(int nproc = 0, string tag = "", int datormc = 0, int debug = 0, int nevt = 0, int correct = 1, int zs = 0, int upweightb = 0, int doupweight = 0)
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
  gSystem->Load("libg4dst.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity( verbosity );
  // just if we set some flags somewhere in this macro
  recoConsts *rc =  recoConsts::instance();
  rc->set_uint64Flag("TIMESTAMP",21615);
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
      if(!zs) list4.open("/sphenix/user/jocl/projects/sandbox/run/dst_calo_nozero.list");
      else list4.open("/sphenix/user/jocl/projects/sandbox/run/dst_calo_cluster.list");
      list2.open("/sphenix/user/jocl/projects/sandbox/run/dst_global.list");
      list3.open("/sphenix/user/jocl/projects/sandbox/run/dst_truth.list");
      list1.open("/sphenix/user/jocl/projects/sandbox/run/g4hits.list");
    }
  else
    {
      list1.open("/sphenix/user/jocl/projects/sandbox/run/dsts_production", ifstream::in);
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
  if(datormc && !upweightb)
    {
      se->registerInputManager(in_2);
      se->registerInputManager(in_3);
      se->registerInputManager(in_4);
    }


  EnergyCorrection *energycorrect;
  if(upweightb)
    {
      energycorrect = new EnergyCorrection();
      energycorrect->SetHitNodeName("G4HIT_CEMC");
      //only want to upweight the truth once
      energycorrect->SetUpWeightTruth((doupweight?true:false));
      if(upweightb && datormc) se->registerSubsystem(energycorrect);
      energycorrect = new EnergyCorrection();
      energycorrect->SetHitNodeName("G4HIT_HCALIN");
      if(upweightb && datormc) se->registerSubsystem(energycorrect);
      energycorrect->SetUpWeightTruth(false);
      energycorrect = new EnergyCorrection();
      energycorrect->SetHitNodeName("G4HIT_HCALOUT");
      energycorrect->SetUpWeightTruth(false);
      if(upweightb && datormc) se->registerSubsystem(energycorrect);
    }
  /*
    PHG4CylinderCellReco *cemc_cells =
        new PHG4CylinderCellReco("CEMCCYLCELLRECO");
    cemc_cells->Detector("CEMC");
    for (int i = G4CEMC::Min_cemc_layer; i <= G4CEMC::Max_cemc_layer; i++) {
      const double radius = 95;
      cemc_cells->cellsize(i, 2 * M_PI / 256. * radius, 2 * M_PI / 256. *
    radius);
    }
    cemc_cells->Verbosity(1);
    se->registerSubsystem(cemc_cells);
  */

  PHG4FullProjSpacalCellReco *cemc_cells;
  if(upweightb)
    {
      cemc_cells = new PHG4FullProjSpacalCellReco("CEMCCYLCELLRECO");
      cemc_cells->Detector("CEMC");
      cemc_cells->Verbosity(0);
      cemc_cells->get_light_collection_model().load_data_file(
							      string(getenv("CALIBRATIONROOT")) +
							      string("/CEMC/LightCollection/Prototype3Module.xml"),
							      "data_grid_light_guide_efficiency", "data_grid_fiber_trans");
      if(upweightb && datormc) se->registerSubsystem(cemc_cells);
    }

  PHG4HcalCellReco *ohc;
  PHG4HcalCellReco *ihc;
  if(upweightb)
    {
      ihc = new PHG4HcalCellReco("HCALIN_CELLRECO");
      ohc = new PHG4HcalCellReco("HCALOUT_CELLRECO");
      ihc->Detector("HCALIN");
      ihc->Verbosity(0);
      if(upweightb && datormc) se->registerSubsystem(ihc);


      ohc->Detector("HCALOUT");
      ohc->Verbosity(1);
      if(upweightb && datormc) se->registerSubsystem(ohc);
    }
  HcalRawTowerBuilder *OTowerBuilder;
  RawTowerBuilder *ETowerBuilder;
  HcalRawTowerBuilder *ITowerBuilder;
  if(upweightb)
    {
      ETowerBuilder = new RawTowerBuilder("EmcRawTowerBuilder");
      ETowerBuilder->Detector("CEMC");
      ETowerBuilder->set_sim_tower_node_prefix("SIM");
      ETowerBuilder->Verbosity(0);
      if(upweightb && datormc) se->registerSubsystem(ETowerBuilder);
      ITowerBuilder = new HcalRawTowerBuilder("HcalInRawTowerBuilder");
      ITowerBuilder->Detector("HCALIN");
      ITowerBuilder->set_sim_tower_node_prefix("SIM");
      if (!isfinite(G4HCALIN::phistart)) {
	G4HCALIN::phistart =
	  0.0445549893; // offet in phi (from zero) extracted from geantinos
      }
      ITowerBuilder->set_double_param("phistart", G4HCALIN::phistart);
      if (isfinite(G4HCALIN::tower_emin)) {
	ITowerBuilder->set_double_param("emin", G4HCALIN::tower_emin);
      }
      if (G4HCALIN::tower_energy_source >= 0) {
	ITowerBuilder->set_int_param("tower_energy_source",
				     G4HCALIN::tower_energy_source);
      }
      ITowerBuilder->Verbosity(0);
      if(upweightb && datormc) se->registerSubsystem(ITowerBuilder);
      
      OTowerBuilder = new HcalRawTowerBuilder("HcalOutRawTowerBuilder");
      OTowerBuilder->Detector("HCALOUT");
      OTowerBuilder->set_sim_tower_node_prefix("SIM");
      if (!isfinite(G4HCALOUT::phistart)) {
	G4HCALOUT::phistart =
	  0.0240615415; // offet in phi (from zero) extracted from geantinos
      }
      OTowerBuilder->set_double_param("phistart", G4HCALOUT::phistart);
      if (isfinite(G4HCALOUT::tower_emin)) {
	OTowerBuilder->set_double_param("emin", G4HCALOUT::tower_emin);
      }
      if (G4HCALOUT::tower_energy_source >= 0) {
	OTowerBuilder->set_int_param("tower_energy_source",
				     G4HCALOUT::tower_energy_source);
      }
      OTowerBuilder->Verbosity(0);
      if(upweightb && datormc) se->registerSubsystem(OTowerBuilder);
    }

  // this points to the global tag in the CDB
  rc->set_StringFlag("CDB_GLOBALTAG","2023p004");//"ProdA_2023");                                     
// The calibrations have a validity range set by the beam clock which is not read out of the prdfs as of now
  //rc->set_IntFlag("RANDOMSEED",158804);
  int cont = 0;
  /*
  MbdDigitization* mbddigi;
  MbdReco* mbdreco;
  if(datormc)
    {
      mbdreco = new MbdReco();
      mbddigi = new MbdDigitization();
      se->registerSubsystem(mbddigi);
      se->registerSubsystem(mbdreco);
    }
  */
  int runnumber = 21615;
  /*
  DansSpecialVertex *dsv;
  if(!datormc)
    {
      dsv = new DansSpecialVertex("DansSpecialVertex", "dump.root");
      dsv->SetRunNumber(runnumber);
      dsv->Verbosity(0);
      se->registerSubsystem(dsv);
    }
  */
  //Fun4AllInputManager *intrue2 = new Fun4AllRunNodeInputManager("DST_GEO");

  //CDBInterface *cdb = CDBInterface::instance();
  //std::string geoLocation = cdb->getUrl("calo_geo");
  //intrue2->AddFile(geoLocation);
  //se->registerInputManager(intrue2);

  MDCTreeMaker *tt = new MDCTreeMaker( filename, datormc, debug, correct );
  se->registerSubsystem( tt );
  se->Print("NODETREE");
  se->run(nevt);
  cout << "Ran all events" << endl;
  se->End();
  cout << "Ended server" << endl;
  delete se;
  cout << "Deleted server" << endl;
  gSystem->Exit(0);
  cout << "Exited gSystem" << endl;
  return 0;

}
