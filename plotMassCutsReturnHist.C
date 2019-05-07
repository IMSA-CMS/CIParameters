#include "FileParams.h"
#include "HistParams.h"
#include "drawPlotCopy.h"
#include "drawFileParams.h"
#include "plotMassCutsReturnHist.h"
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
#include "TLegend.h"
#include <iostream>
#include <algorithm>

std::string makeCuts(std::string cut, std::string variable);
std::string lambdaNameTextFile(Lambda lambda);
std::string returnHelicity(Helicity h);
std::string returnParticle(Particle p);
std::string returnInterference(Interference i);
std::string returnBarrelState(BarrelState bs);

//makes hists
TH1F* plotMassCutsReturnHist(Lambda lambda, Helicity helicity, Interference interference, Particle particle, BarrelState bs, std::vector<double> xSecs)
{   
  std::cout << "\n=================================================" << std::endl;
  std::cout << "Making a Histogram for " << "\n\tLambda: " << lambdaNameTextFile(lambda) << std::endl;
  std::cout << "\n=================================================" << std::endl;
  
  std::string variable = "Test.calcValues.invariantMass";
  
  //filled with dummy values
  FileParams *file = new FileParams(Helicity::RR, Interference::Con, MassRange::First, Lambda::L16000, Particle::Dielectron);
  HistParams hp;
  hp.cuts = "";
  //hp.cuts = "3";
  //HistParams.drawOption = "yes";
  hp.nbins = 40;
  hp.min = 0;
  hp.max = 4000; //can make larger to see tail off
  hp.title = "Invariant Mass with Masscuts";
  hp.color = 1;
  hp.minNull = false;
  hp.maxNull = false;

  //these are the super bin increments being used here; start with bin 4 (400 to 500);
  std::vector<double> varBins = {0, 100, 200, 300, 400, 500, 700, 1100, 1900, 3500, 5000};
  std::vector<std::string> directory = {};
  
  TH1F* hist = nullptr;
  TH1F* hist1 = nullptr;
  std::vector<TH1F*> hists;
  std::string treename = "Events";
  std::string filename;
  std::vector<std::string> cuts = {"300","800","1300"}; //vector of the mass cuts, possibly include 2000

  //Luminosity; subject to change!
  const double lum = 36.3;
  //scaleFactor:
  double f1;
  double crossSec;

  std::vector<MassRange> massranges = {MassRange::First, MassRange::Second, MassRange::Third, MassRange::Fourth};

  for (unsigned i = 0; i < cuts.size(); i++)
    {
      //switch this to make2016Cuts(cuts[i], variable); to use includive cuts
      std::string cutsTemp = makeCuts(cuts[i], variable);
      if (bs == BarrelState::BB || bs == BarrelState::BE)
	cutsTemp += "&&Test.calcValues.is" + returnBarrelState(bs);
      
      //why do we do this part again?
      if (helicity == Helicity::LR)
	hp.cuts = "(" + cutsTemp + ")*Test.calcValues.weightLR";
      else if (helicity == Helicity::RL)
	hp.cuts = "(" + cutsTemp + ")*Test.calcValues.weightRL";
      else
	hp.cuts = cutsTemp;

      hp.name = cuts[i];

      if (helicity == Helicity::RL)
	*file = FileParams(Helicity::LR, interference, massranges[i], lambda, particle);
      else
	*file = FileParams(helicity, interference, massranges[i], lambda, particle);

      directory = file->fileVector(*file);

      //Gather cross section and calculate hist scale factor, f1
      crossSec = xSecs[i];
      f1 = (lum*crossSec*1000)/50000;

      if(i == 0)
	{
	  hist = makeOneHistogram("Invariant_Mass"+cuts[i]+lambdaNameTextFile(lambda), i+1, "tree", variable, *file, hp);
	  hist->Scale(f1);
	  hist->GetXaxis()->Set(9, varBins.data());

	  hist->SetLineStyle(1);
	  hist->Sumw2();
	  hist->SetOption("HIST");
	}
      else
	{
	  hist1 = makeOneHistogram("Invariant_Mass"+cuts[i]+lambdaNameTextFile(lambda), i+1, "tree", variable, *file, hp);
	  hist1->Scale(f1);
	  hist1->GetXaxis()->Set(9, varBins.data());

	  hist1->Sumw2();
	  hist1->SetOption("HIST");
	  hist->Add(hist1);
	}
    }

  return hist;
  //hist->Draw();
}


//makes cuts inclusive to accommodate 2016 Monte Carlo data
std::string make2016Cuts(std::string cut, std::string variable)
{ 
  std::string cutName = variable + ">" + cut;
  cutName += "&&!Test.calcValues.isEmpty";
  return cutName;
}


//makes cuts exclusive to accomotade 2017 Monte Carlo data
std::string makeCuts(std::string cut, std::string variable)
{
  std::string cutName;
  if (cut=="300")
    {
      cutName = variable + ">300 &&" + variable + "<800";
    }
  else if (cut=="800")
    {
      cutName = variable+">800 &&" + variable + "<1300";
    }
  else if (cut=="1300")
    {
      cutName = variable+">1300 &&" + variable + "<2000";
    }
  else if (cut=="2000")
    {
      cutName = variable+">2000";
    }

  cutName += "&&!Test.calcValues.isEmpty";
  return cutName;
}


std::string lambdaNameTextFile(Lambda lambda)
{
  switch(lambda)
    {
    case Lambda::L100000:
      throw std::runtime_error("Not a valid Lambda Value for Text File");
    case Lambda::L34000:
      return "34TeV"; 
    case Lambda::L28000:
      return "28TeV"; 
    case Lambda::L22000:
      return "22TeV"; 
    case Lambda::L16000:
      return "16TeV";   
    case Lambda::L10000:
      throw std::runtime_error("Not a valid Lambda value for Text File");
    case Lambda::L1000:
      return "1TeV";
    }
}
