//Anisha and Emily
//Get cross section numbers: cross section, cross section error, and number of events
//Edited by Chetan Reddy

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include "crossSection.h"
#include "drawPlotCopy.h"
#include "TTree.h"
#include "TLeaf.h"
//#include "/uscms/physics_grp/lpcci2dileptons/nobackup/CI_study/Scripts/PeterScripts/rootScripts/PlotLibrary.cc"
//struct with all of the Cross Section information from text file



CrossSecInfo getInformation(std::string filename)
{
  CrossSecInfo info;
  std::ifstream reader;
  std::string myline;
  reader.open(filename);
  if(!reader)//check if the file is there
    {
      throw std::runtime_error("File not found");
    }
  
  int j = 0; //counter
  
  while (getline(reader, myline)) //go through each line of the file and put the corresponding information into the struct
    {
      if (j==0)
	{
	  info.crossSec = findNumber(myline);
	}
      if (j==1)
	{
	  info.crossSecError = findNumber(myline);
	}
      if (j==2)
	{
	  info.numOfEvents = (int)(findNumber(myline)+.5);//converts the double to an int
	  
	}
      j++;
    }
  return info;
}


CrossSecInfo getMCValue(TFile* file)
{
    const char* treeName = "Runs";
    const char* xSecBranchName
      = "GenRunInfoProduct_generator__SIM.obj.internalXSec_.value_";
    const char* xSecErrorBranchName
      = "GenRunInfoProduct_generator__SIM.obj.internalXSec_.error_";
    const char* eventTreeName = "Events";
    
    //TTree* runTree = dynamic_cast<TTree*>(file->Get(treeName.c_str()));
    TTree* runTree = getTree(file, treeName);
    double crossSec = readValue(runTree, xSecBranchName);
    double crossSecError = readValue(runTree, xSecErrorBranchName);
    TTree* eventTree = getTree(file, eventTreeName);
    //TTree* eventTree = dynamic_cast<TTree*>(file->Get(eventTreeName.c_str()));
    int entries = eventTree->GetEntries();
    std::cout<<"TempCrossSec: " << crossSec << std::endl;				  
    return {crossSec, crossSecError, entries}; 	   
					  
			     

}

CrossSecInfo getAverageCrossSection(std::vector<std::string> directory)
{
  double weightedSum = 0;
  double weightedSumSq = 0;
  int totalEvents = 0;

  for (auto filename : directory)
    {
      if (filename.find("MINIAOD") != string::npos)
	{
	  //std::cout << "Processing file " << filename << ".\n";
	  std::string str = "root://cmsxrootd.fnal.gov//" + filename;
	  TFile* file = TFile::Open(str.c_str());
	  CrossSecInfo info = getMCValue(file);
	  //std::cout<<__LINE__<<std::endl;
	  weightedSum += info.crossSec * info.numOfEvents;
	  double weightedSD = info.crossSecError * info.numOfEvents;
	  weightedSumSq += weightedSD * weightedSD;
	  totalEvents += info.numOfEvents;
	  std::cout<<__LINE__<<std::endl;
	  delete file;
	}
    }

  double mean = weightedSum / totalEvents;
  double sd = sqrt(weightedSumSq) / totalEvents;
  return {mean, sd, totalEvents};

}

double findNumber(std::string line)
{
  std::string number;
  double outputNumber;
  for (unsigned i=0; i<line.length(); ++i)
    {
      if (line[i] == '\t')//checks where the tab is 
	{
	  number = line.substr(i+1, line.length());//gets the string after the tab
	  outputNumber=std::stod(number); //converts the string from the file to a double
	  return outputNumber; 
	}
    }
  return 0;
}

double readValue(TTree* tree, const std::string& leafName)
{
  TBranch* branch = tree->GetBranch(leafName.c_str());
  branch->GetEntry(0);
  TLeaf* leaf = branch->GetLeaf(leafName.c_str());
  return leaf->GetTypedValue<double>(0);
}

//Cross section	0.0143464
//Cross section error	9.12273e-05
//Number of events	50000
