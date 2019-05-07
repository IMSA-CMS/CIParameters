
#include "FileParams.h"
#include "HistParams.h"
#include "drawPlotCopy.h"
#include "drawFileParams.h"
#include "crossSection.h"
#include "plotMassCutsReturnHist.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include <stdexcept>
#include "TChain.h"
#include "TMath.h"
#include <string>
#include <climits>
#include <vector>
#include <math.h>
#include "TClass.h"
#include "TGraph.h"
#include <iostream>

std::string lambdaNameTextFile(Lambda lambda);
int lambdaNum(Lambda lam);

//for given bin number, gives number of events for each lambda
std::vector<double> lambdaVEvents(int binNum)
{
  TFile* lambdaOverlayed = TFile::Open("lambdaOverlayedMasses.root", "update");
 
  std::vector<Lambda> lambdas = {Lambda::L16000, Lambda::L22000, Lambda::L28000, Lambda::L34000};
  double tempEvents;
  std::string histName;
  std::vector<int> events;
  std::vector<int> lambdaInts;
  

  //this function is fit to the data
  //this 40 is the number of bins?
  TF1 * func = new TF1("func", "[0]+[1]/(x*x)+[2]/(x*x*x*x)", 12, 40);
  for(unsigned m = 0; m < lambdas.size(); m++)
    {
      histName = "Invariant_Mass300" + lambdaNameTextFile(lambdas[m]);

      TH1F* tempHist = dynamic_cast<TH1F*>(lambdaOverlayed->Get(histName.c_str()));

      tempEvents = tempHist->GetBinContent(binNum);
      events.push_back(tempEvents);
      lambdaInts.push_back(lambdaNum(lambdas[m]));
    }
  
  TGraph* lambdaEvents = new TGraph(events.size(),lambdaInts.data(), events.data());
  lambdaEvents->Fit("func");

  double par1 = func->GetParameter(0);
  double par2 = func->GetParameter(1);
  double par3 = func->GetParameter(2);

  std::vector<double> parameters = {par1, par2, par3};

  lambdaEvents->Write();

  return parameters;
}

int lambdaNum(Lambda lam)
{
  switch(lam)
    {
    case Lambda::L100000:
      return 100;
    case Lambda::L34000:
      return 34; 
    case Lambda::L28000:
      return 28; 
    case Lambda::L22000:
      return 22; 
    case Lambda::L16000:
      return 16;   
    case Lambda::L10000:
      return 10;
    case Lambda::L1000:
      return 1;
    }
}
