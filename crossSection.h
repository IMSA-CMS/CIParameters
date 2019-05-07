

#ifndef crossSection_hpp
#define crossSection_hpp

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include "drawPlot.h"
#include "TTree.h"
#include "TLeaf.h"


struct CrossSecInfo
{
  double crossSec;
  double crossSecError;
  int numOfEvents;
};


CrossSecInfo getInformation(std::string filename);
double findNumber(std::string line);
double readValue(TTree* tree, const std::string& leafName);
CrossSecInfo getAverageCrossSection(std::vector<std::string> directory);
CrossSecInfo getMCValue(TFile*);


#endif
