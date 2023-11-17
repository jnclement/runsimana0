#ifndef MACRO_RUNDUMP_C
#define MACRO_RUNDUMP_C

#include <phool/recoConsts.h>
#include <nodedump/Dumper.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <ffamodules/CDBInterface.h>
// cppcheck-suppress unknownMacro
R__LOAD_LIBRARY(libfun4all.so)
// cppcheck-suppress unknownMacro
R__LOAD_LIBRARY(libphnodedump.so)
R__LOAD_LIBRARY(libffamodules.so)
void run_dump(const std::string &infile, const int evts=100)
{
  gSystem->Load("libg4dst.so");
  Fun4AllServer* se = Fun4AllServer::instance();
  recoConsts *rc =  recoConsts::instance();
  rc->set_uint64Flag("TIMESTAMP",21615);
  Fun4AllInputManager *intrue2 = new Fun4AllRunNodeInputManager("DST_GEO");
  rc->set_StringFlag("CDB_GLOBALTAG","2023p003");//"ProdA_2023");
  CDBInterface *cdb = CDBInterface::instance();
  std::string geoLocation = cdb->getUrl("calo_geo");
  intrue2->AddFile(geoLocation);
  se->registerInputManager(intrue2);

  Dumper *dmp = new Dumper();
  gSystem->Exec("mkdir dump");
  dmp->SetOutDir("./dump");

  se->registerSubsystem(dmp);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  se->registerInputManager(in);
  se->fileopen("DSTin",infile);
  se->run(evts);
  se->End();
  delete se;
}

#endif
