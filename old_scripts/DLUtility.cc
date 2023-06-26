#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllDstInputManager.h>

#include "TChain.h"
#include <fun4all/Fun4AllInputManager.h>
#include <phool/recoConsts.h>

#include "DLUtility.h"

#include <phool/phool.h>

#include <boost/filesystem.hpp>
#include <sstream>
#include <string>
#include "TTree.h"
#include "TFile.h"
#include <dirent.h>
#include <cstdint>  // for uintmax_t                               
#include <fstream>
#include <iostream>

/**************************************************/
void AddFileFromList(const int keep_it, const std::string filename, Fun4AllInputManager *in)
{
  std::cout<<"Looking for "<<keep_it<<std::endl;
  if (boost::filesystem::exists(filename.c_str()))
    {
      if (boost::filesystem::is_regular_file(filename.c_str()))
	{
	  uintmax_t fsize = boost::filesystem::file_size(filename.c_str());
	  if (fsize > 1000000 && !(0))
	    {
	      std::cout << "size of " << filename
			<< " is suspiciously large for a text file: "
			<< fsize << " bytes" << std::endl;
	      std::cout << "if you really want to use " << filename
			<< " as list file (it will be used as a text file containing a list of inp\
ut files), use AddListFile(\""
			<< filename << "\",1)" << std::endl;
	      return;
	    }
	}
      else
	{
	  std::cout << filename << " is not a regular file" << std::endl;
	  return;
	}
    }
  else
    {
      std::cout << PHWHERE << "Could not open " << filename << std::endl;
      return;
    }
  std::ifstream infile;
  infile.open(filename, std::ios_base::in);
  if (!infile)
    {
      std::cout << PHWHERE << "Could not open " << filename << std::endl;
      return;
    }
  std::string FullLine;
  int nfiles = 0;
  int nfiles_keep = 0;
  getline(infile, FullLine);
  while (!infile.eof())
    {
      std::cout<<"at file "<<nfiles_keep<<std::endl;
      if (nfiles == 1) break;
      if (nfiles_keep != keep_it) {
	nfiles_keep++;
	getline(infile, FullLine);
	continue;
      }
      if (FullLine.size() && FullLine[0] != '#')  // remove comments                               
	{
	  in->AddFile(FullLine);
	  std::cout<<"Adding File: "<<FullLine<<std::endl; 
	  nfiles++;
	  break;
	}
      else if (FullLine.size())
	{
	  std::cout << "Found Comment: " << FullLine << std::endl;
	  break;
	}
    }
  infile.close();
  return;

}

void MergeTreesInDir(const std::string path, const std::string output_name){
  
  TChain ch("ttree");
  std::string compare = "root";
  std::string in_file;
  std::string ftype;
  std::string dlim = ".";
  std::string token;
  size_t pos;
  struct dirent *entry = nullptr;
  DIR *dp = nullptr;
  dp = opendir(path.c_str());
  while((entry = readdir(dp))){
    in_file = entry->d_name;
    
    ftype = entry->d_name;
    while ((pos = ftype.find(dlim)) != std::string::npos){
      ftype.erase(0, pos + dlim.length());
      if (ftype.length() == 0) break;
      //      std::cout<<"Found delim: "<<ftype<<"with size "<<ftype.length()<<std::endl;
    
    }
    int b = ftype.compare("root");
    //std::cout<<"Comparing strings: "<<b<<std::endl;
    
    if (b == 0){
      //std::cout<<"adding tree"<<std::endl;
      ch.Add(in_file.c_str());

    }
  }
  //  if (tree_list->GetSize() > 0) return;
  ch.Merge("output.root");
  std::cout<<"out"<<std::endl;
  

  return;
}
