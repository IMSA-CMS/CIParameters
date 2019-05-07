

#ifndef plotMassCutsReturnHist_hpp
#define plotMassCutsReturnHist_hpp

#include "FileParams.h"
#include "HistParams.h"
#include "drawPlot.h"
#include "drawFileParams.h"
#include "crossSection.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include <stdexcept>
#include "TChain.h"
#include "TMath.h"
#include <string>
#include <climits>
#include <vector>
#include <math.h>
#include "TClass.h"



TH1F* plotMassCutsReturnHist(Lambda lambda, Helicity helicity, Interference interference, Particle particle, BarrelState bs, std::vector<double> xSecs);

std::string makeCuts(std::string cut, std::string variable);
std::string lambdaNameTextFile(Lambda lambda);

#endif

