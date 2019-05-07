/*
 * This is the Source file for FileParams, which will be used to find files
 * Written by: Jay Reiter, Matt Hokinson, and John Woods
 * Last Edited: May 9th, 2018
 * Edits to merge fileLocator to FileParams by Kaushal Gumpula and Emily Springer
 * Last Edited: June 11th, 2018
 */

#include "TROOT.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TH1.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "TChain.h"
#include "drawPlot.h"
#include "HistParams.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "FileParams.h"


using std::map;
using std::cout;
using std::string;

//Declaring the Maps
const map<Helicity, string> FileParams::helicityMap = { { Helicity::LL, "LL" },{ Helicity::LR, "LR" },{ Helicity::RL, "RL" },{ Helicity::RR, "RR" } };

const map<Interference, string> FileParams::interferenceMap = { { Interference::Con, "Con" },{ Interference::Des, "Des" } };

const map<MassRange, string>  FileParams::massRangeMap = { { MassRange::First, "300to800" },{ MassRange::Second, "800to1300" },{ MassRange::Third, "1300to2000" },{ MassRange::Fourth, "2000toInf" } };

const map<Lambda, string> FileParams::lambdaMap = { { Lambda::L1000, "1000" },{ Lambda::L10000, "10000" },{ Lambda::L16000, "16000"},{ Lambda::L22000, "22000" },{ Lambda::L28000, "28000" },{ Lambda::L34000, "34000" },{ Lambda::L100000, "100000" }};

const map<Particle, string> FileParams::particleMap = { { Particle::Dielectron, "EE" },{ Particle::Dimuon, "MUMU" } };

//constructor for fileParams
FileParams::FileParams(Helicity heli, Interference inter, MassRange mrange, Lambda L, Particle part) :
helicity(heli),
interference(inter),
massRange(mrange),
lambda(L),
particle(part)
{}

std::ostream& operator<<(std::ostream& stream, FileParams params)
{
    stream << "Your Parameters: " << "\n";
    stream << "Helicity: " << params.getHelicity() << "\n";
    stream << "Interference: " << params.getInterference() << "\n";
    stream << "Mass Range: " << params.getMassRange() << "\n";
    stream << "Lambda: " << params.getLambda() << "\n";
    stream << "Particle: " << params.getParticle() << "\n";
    stream << "\n";
    
    return stream;
}

int FileParams::fpToInt() const
{
  int n = 0;

  if (this->helicity == Helicity::LL)
    n += 1;
  else if (this->helicity == Helicity::LR)
    n += 2;
  else if (this->helicity == Helicity::RL)
    n += 3;
  else if (this->helicity == Helicity::RR)
    n += 4;
  n *= 10;

  if (this->interference == Interference::Con)
    n += 1;
  else if (this->interference == Interference::Des)
    n += 2;
  n *= 10;

  if (this->massRange == MassRange::First)
    n += 1;
  else if (this->massRange == MassRange::Second)
    n += 2;
  else if (this->massRange == MassRange::Third)
    n += 3;
  else if (this->massRange == MassRange:: Fourth)
    n += 4;
  n *= 10;

  switch(this->lambda)
    {
    case Lambda::L1000:
      n += 1; break;
    case Lambda::L10000:
      n += 2; break;
    case Lambda::L16000:
      n += 3; break;
    case Lambda::L22000:
      n += 4; break;
    case Lambda::L28000:
      n += 5; break;
    case Lambda::L34000:
      n += 6; break;
    case Lambda::L100000:
      n += 7;
    }
  return n;
}


//parses through the vector of root files and creates a TChain
TChain* FileParams::createTChainTextFile(FileParams params, std::string treename)
{
  std::vector<std::string>rootfileVector = fileVector(params);
  TChain *chain = new TChain(treename.c_str());
  for(std::size_t i = 0; i < rootfileVector.size(); ++i)
    {
      std::string str = "root://cmsxrootd.fnal.gov//"+rootfileVector[i];
      if(!chain)
      {
	throw std::runtime_error("invalid chain");
      }
      chain->Add(str.c_str());
    }
  return chain;
}

//parses through the vector of root files and creates a TChain
TChain* FileParams::TESTcreateTChainTextFile(FileParams params, std::string treename)
{
  std::vector<std::string>rootfileVector = fileVector(params);
  TChain *chain = new TChain(treename.c_str());
  
  for(std::size_t i = 0; i < 1; ++i)
    {
      std::string str = "root://cmsxrootd.fnal.gov//"+rootfileVector[i];
      if(!chain)
      {
	throw std::runtime_error("invalid chain");
      }
      chain->Add(str.c_str());
    }
  return chain;
}


//creates a vector of root files through parsing through the text file given by the path
 std::vector<std::string> FileParams::fileVector(FileParams params)
{
  string textfilepath = params.locateTextFile();
  std::cout <<textfilepath<<std::endl;
  std::vector<std::string> rootfileVector;
  string line;
  ifstream myfile (textfilepath);
  if (myfile.is_open())
    {
      while(getline(myfile,line))
	{
	  rootfileVector.push_back(line);
	}
    }
  return rootfileVector;

}

//finds text file path
string FileParams::locateTextFile()
{
  string directory = "/uscms/home/pdong/work/IMSACMS/scripts/newData/fileQueryTest";
  directory += getParticlePathTextFile();
  directory += getMassRangePathTextFile();
  directory += getLambdaPathTextFile();
  directory += getInterferencePathTextFile();
  directory += getHelicityPath();
  directory += ".txt";
  return directory;
}


//finds text file with designated particles
string FileParams::getParticlePathTextFile()
{
  switch(particle)
    {
    case Particle::Dielectron:
      return "E"; 
       
    case Particle::Dimuon:
      return "Mu";
       
    }
}

//finds text file with designated mass range
string FileParams::getMassRangePathTextFile()
{
  switch(massRange)
    {
    case MassRange::First: 
      return "M300"; 
       
    case MassRange::Second:
      return "M800"; 
       
    case MassRange::Third: 
      return "M1300";
       
    case MassRange::Fourth: 
      throw std::runtime_error("Not a valid mass range for Text File"); 
    }
}

//finds text file with designated energy scale
string FileParams::getLambdaPathTextFile() 
{
  switch(lambda)
    {
    case Lambda::L100000:
      throw std::runtime_error("Not a valid Lambda Value for Text File");
    case Lambda::L34000:
      return "L34"; 
    case Lambda::L28000:
      return "L28"; 
    case Lambda::L22000:
      return "L22"; 
    case Lambda::L16000:
      return "L16";   
    case Lambda::L10000:
      throw std::runtime_error("Not a valid Lambda Value for Text File");
    case Lambda::L1000:
      return "L1";
    }
}

//finds text file with designated interference
string FileParams::getInterferencePathTextFile()
{
  switch(interference)
    {
    case Interference::Con:
      return "Con"; 
       
    case Interference::Des:
      return "Des"; 
       
    }
}

string FileParams::locateFile()
{
  string directory = "/uscms/physics_grp/lpcci2dileptons/nobackup/CI_study/Samples/";  
  directory += getLambdaPath(); 
  directory += getParticlePath();
  directory += getHelicityPath(); 
  directory += getInterferencePath(); 
  directory += getMassRangePath(); 
  return directory;  
}

//finds file with designated energy scale
string FileParams::getLambdaPath() 
{
  switch(lambda)
    {
    case Lambda::L100000:
      return "Lambda_100kTeV/"; 
    case Lambda::L34000:
      return "Lambda_34TeV/"; 
    case Lambda::L28000:
      return "Lambda_28TeV/"; 
    case Lambda::L22000:
      return "Lambda_22TeV/"; 
    case Lambda::L16000:
      return "Lambda_16TeV/"; 
    case Lambda::L10000:
      return "Lambda_10TeV/"; 
    case Lambda::L1000:
      return "Lambda_1TeV/"; 
       
    }
}

//finds file with designated particles
string FileParams::getParticlePath()
{
  switch(particle)
    {
    case Particle::Dimuon:
      return "CITo2Mu/"; 
       
    case Particle::Dielectron:
      return "CITo2E/";
       
    }
}

//finds file with designated helicity model
string FileParams::getHelicityPath()
{
  switch(helicity)
    {
    case Helicity::LL:
      return "LL";
       
    case Helicity::LR:
      return "LR";
       
    case Helicity::RR:
      return "RR";
       
    case Helicity::RL:
      throw std::runtime_error("Use LR instead of RL"); 
       
    }
}

//finds file with designated interference
string FileParams::getInterferencePath()
{
  switch(interference)
    {
    case Interference::Con:
      return "Con/"; 
       
    case Interference::Des:
      return "Des/"; 
       
    }
}

//finds file with designated mass range
string FileParams::getMassRangePath()
{
  switch(massRange)
    {
    case MassRange::First: 
      return "M300/"; 
       
    case MassRange::Second:
      return "M800/"; 
       
    case MassRange::Third: 
      return "M1300/";
       
    case MassRange::Fourth: 
      return "M2000/"; 
       
    }
}
