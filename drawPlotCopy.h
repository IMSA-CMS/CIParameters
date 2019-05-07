//header file for drawPlot.C 

#ifndef drawPlot_hpp
#define drawPlot_hpp

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH1F.h"
#include <stdexcept>
#include "TChain.h"
#include <string>
#include "HistParams.h"


TFile* openFile(std::string fileName);
TTree* getTree(TFile* file, std::string treename);
TChain* getTChain(std::string path, std::string treename);
TH1F* drawHist(TTree* tree, std::string variable, HistParams params);
TH1F* drawHist(std::string filename, std::string treename, std::string variable, HistParams params);
TH1F* drawHistChain(std::string path, std::string treename, std::string variable, HistParams params);

#endif /* drawPlot_hpp */

