#include "FileParams.h"
#include "HistParams.h"
#include "drawPlotCopy.h"
#include "drawFileParams.h"
#include "crossSection.h"
#include "plotMassCutsReturnHist.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
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
#include "TChain.h"
#include "TTreeReader.h"
#include <fstream>
#include <map>

//std::map<FileParams, double> generateXSecMap();
//std::vector<double> retrieveCrossSections(const std::map<FileParams, double>& xSecMap, Helicity helicity, Interference interference, Particle particle, Lambda lambda);

double weightSum(std::string h);

//Makes a big ol' map, associating all the cross sections with their parameters
std::map<FileParams, double> generateXSecMap()
{
  std::map<FileParams, double> xSecMap;
  
  std::ifstream file("crossSections.txt");
  double crossSection;
  std::string str;
  std::vector<double> allCrossSections;

  while (std::getline(file, str))
    {
      allCrossSections.push_back(std::stod(str));
    }

  int n = 0;
  for(Helicity h : {Helicity::LL, Helicity::LR, Helicity::RR})
    {
      for(Interference i : {Interference::Con, Interference::Des})
	{
	  for(MassRange m : {MassRange::First, MassRange::Second, MassRange::Third})
	    {
	      for(Particle p : {Particle::Dielectron, Particle::Dimuon})
		{
		  for(Lambda l : {Lambda::L16000, Lambda::L22000, Lambda::L28000, Lambda::L34000})
		    {
		      if (h == Helicity::LL || h == Helicity::RR)
			{
			  FileParams fp(h, i, m, l, p);
			  xSecMap[fp] = allCrossSections[n];
			  n++;
			}
		      else
			{
			  double LR = weightSum("LR");
			  double RL = weightSum("RL");
			  double k = (2 * allCrossSections[n]) / (RL + LR);
			  
			  FileParams fpLR(Helicity::LR, i, m, l, p);
			  FileParams fpRL(Helicity::RL, i, m, l, p);

			  xSecMap[fpLR] = k * LR;
			  xSecMap[fpRL] = k * RL;
			  n++;
			}
     }}}}} 
  return xSecMap;
}

//uses the xSecMap to retrieve appropriate cross sections for given parameters
std::vector<double> retrieveCrossSections(const std::map<FileParams, double>& xSecMap, Helicity helicity, Interference interference, Particle particle, Lambda lambda)
{
  FileParams fp1(helicity, interference, MassRange::First, lambda, particle);
  FileParams fp2(helicity, interference, MassRange::Second, lambda, particle);
  FileParams fp3(helicity, interference, MassRange::Third, lambda, particle); 

  std::cout << "\nRetrieved cross sections: " << xSecMap.at(fp1) << xSecMap.at(fp2) << xSecMap.at(fp3) << std::endl;
  return {xSecMap.at(fp1), xSecMap.at(fp2), xSecMap.at(fp3)};
}

//calculates sum of weightRL, weightLR in tree
double weightSum(std::string h)
{
  static int counter = 0;
  std::cerr << "\nCalculating " << h << " weight sum!!! " << counter;
  counter++;

  auto chain = new TChain("tree");
  chain->Add("/uscms/home/jreiter/MassParameters/sample/*.root");

  double sum = 0;

  TTreeReader reader(chain);
  std::string str = "Test.calcValues.weight" + h;
  TTreeReaderValue<Double_t> variable(reader, str.c_str());
  while (reader.Next())
    sum += *variable;

  return sum;
}

