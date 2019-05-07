// drawPlot.C
// 2 May 2018

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include <stdexcept>
#include "TChain.h"
#include <string>
#include "HistParams.h"
#include <iostream>

//Function to open a file
TFile* openFile(std::string fileName)
{
  const char* fileChar = fileName.c_str();
  TFile* outFile = TFile::Open(fileChar, "READ");
  if(!outFile)
    {
      throw std::runtime_error("File not found!");
    }
  return outFile;
}

//Function to get a TTree from a TFile
TTree* getTree(TFile* file, std::string treename)
{
  if (!file)
    {
      throw std::runtime_error("invalid file");
    }
  TTree *tree = dynamic_cast<TTree*>(file->Get(treename.c_str()));
  if (!tree)
    {
      throw std::runtime_error("invalid tree");
    }
  return tree;
}

//path is without the file name
TChain* getTChain(std::string path, std::string treename)
{
  TChain *chain = new TChain(treename.c_str());
  std::string str = path + ""; //in the quotes can add file names for example *.root based on preference of files
  
  chain->Add(str.c_str());
  if(!chain)
    {
      throw std::runtime_error("invalid chain");
    }
  return chain;
}

//Funtion for drawing a histogram
TH1F* drawHist(TTree* tree, std::string variable, HistParams params)
{
  std::string drawString = variable + ">>" + params.name;
  if (params.nbins > 0)
    {
      drawString += "(" + std::to_string(params.nbins);
	if (!params.minNull)
	  {
	    drawString += "," + std::to_string(params.min);
	    if(!params.maxNull)
	      {
		drawString += "," + std::to_string(params.max) + ")";
	      }
	    else 
	      {
		drawString += ")";
	      }
	  }
      }
	
  const char* drawChar = drawString.c_str();
  if(!tree)
    {
      throw std::runtime_error("Tree not found!");
    }

  

  string fullOptions = params.drawOptions + "GOFF"; //graphics off

  //std::cout << "Full Options: " << fullOptions << std::endl;
  //std::cout << "drawChar: " << drawChar << std::endl;
  //std::cout << "params.cuts: " << params.cuts << std::endl;

  tree->Draw(drawChar, params.cuts.c_str(), fullOptions.c_str());  
  
  TH1F* histOut = dynamic_cast<TH1F*>(gDirectory->Get(params.name.c_str()));
  
  if(!histOut)
    {
      throw std::runtime_error("Histogram not made");
      }
  
  histOut->SetTitle(params.title.c_str());
  histOut->SetLineColor(params.color);

  

  return histOut;
}


//draw hist with file, tree, variable name, and hist params
TH1F* drawHist(std::string filename, std::string treename, std::string variable, HistParams params)
{
  TFile* file = openFile(filename);
  TTree* tree = getTree(file,treename);
  TH1F* hist = drawHist(tree,variable,params);
  return hist;
}

//draw hist using TChain and multiple files
TH1F* drawHistChain(std::string path, std::string treename, std::string variable, HistParams params)
{
  TChain* chain = getTChain(path, treename);
  TH1F* hist = drawHist(chain,variable,params);
  return hist;
}
