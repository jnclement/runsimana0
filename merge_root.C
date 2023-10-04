#include "TApplication.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TRandom2.h"
#include "TMath.h"
#include <utility>  // for std::pair
#include <cstdio>
#include <iostream>
#include "TGraph.h"
#include "TTree.h"
#include "TLegend.h"
#include "TLatex.h"
#include "stdlib.h"
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <TMinuit.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TString.h>
#include <TAxis.h>
#include <TLine.h>
#include <TFile.h>
#include <algorithm>
#include "TChain.h"

int merge_root(int nfiles)
{
  TChain ch("ttree");
  string filename;
  string base = "output/evt/events_";
  string ext = ".root";
  for(int i=0; i<nfiles; i++)
    {
      if(i%10==0) cout << i << endl;
      filename = base;
      filename += to_string(i);
      filename += ext;
      const char *alsofile = filename.c_str();
      try
	{
	  ch.Add(alsofile);
	}
      catch(...)
	{
	  continue;
	}
    }
  filename = "results/merged_dEdeta_" + to_string(nfiles) + ext;
  ch.Merge(filename.c_str());
  return 0;
}
