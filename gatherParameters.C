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
#include <iostream>
#include <climits>
#include <vector>
#include <math.h>
#include "TClass.h"
#include "TGraph.h"

//declares functions from other files
std::vector<double> retrieveCrossSections(const std::map<FileParams, double>& xSecMap, Helicity helicity, Interference interference, Particle particle, Lambda lambda);

void lambdaOverlayedMasses(Helicity helicity, Interference interference, Particle particle, BarrelState bs, std::map<FileParams, double> xSecMap);

TH1F* plotMassCutsReturnHist(Lambda lambda, Helicity helicity, Interference interference, Particle particle, std::vector<double> xSecs);

std::string writeToFile(Particle particle, Interference interference, Helicity helicity, int bin, std::vector<double> parameters, BarrelState bs);

std::vector<double> lambdaVEvents(int binNum);
std::map<FileParams, double> generateXSecMap();

void gatherParameters();
std::string returnParticle(Particle particle);
std::string returnInterference(Interference interference);
std::string returnHelicity(Helicity helicity);
std::string returnBarrelState(BarrelState bs);


////////////////////////////////////////////////////////////////
std::map<FileParams, double> xSecMap = generateXSecMap();
void gatherParameters()
{
  //these are the variables that are adjusted to give each set of parameters
  std::vector<Helicity> helicity = {Helicity::LL, Helicity::LR, Helicity::RL, Helicity::RR};
  std::vector<Interference> interference = {Interference::Con, Interference::Des};
  std::vector<Particle> particle = {Particle::Dimuon, Particle::Dielectron};
  std::vector<BarrelState> bs = {BarrelState::BB, BarrelState::BE};
  
  //here are the super bin divisions (this vector is also explicitly definied in plotMassCutsReturnHist.C, so change it there too!)
  std::vector<int> varBins = {0, 100, 200, 300, 400, 500, 700, 1100, 1900, 3500};

  //vector holds parameters
  std::vector<double> parameters;

  //string eventually gets written to text file
  std::string fileStr = "";


  //loop through every combination to get a bunch of parameters:

  for(unsigned k = 0; k < particle.size(); k++) //particle.size(); k++)
    {  
      for(unsigned b = 0; b < bs.size(); b++)
	{
	  for(unsigned j = 0; j < interference.size(); j++) //interference.size(); j++)
	    {
	      for(unsigned i = 0; i < helicity.size(); i++) //helicity.size(); i++)
		{
		  //function creates root file with these parameters
		  //currently upset with this line here:
	      
		  std::cout << "\t=================================" << std::endl;
		  std::cout << "\t|\tNOW RUNNING CASE:\t|" << std::endl;
		  std::cout << "\t|\tHelicity: " << returnHelicity(helicity[i]) << "\t\t|" << std::endl;
		  std::cout << "\t|\tInterference: " << returnInterference(interference[j]) << "\t|" << std::endl;
		  std::cout << "\t|\tParticle: " << returnParticle(particle[k]) << "\t|" << std::endl;
		  std::cout << "\t|\tBarrelState: " << returnBarrelState(bs[b]) << "\t\t|" << std::endl;
		  std::cout << "\t=================================\n" << std::endl;

		  lambdaOverlayedMasses(helicity[i], interference[j], particle[k], bs[b], xSecMap);
		  std::cout << "\nRoot File Generated!\n";

		  //loop through the different superbins
		  for(unsigned n = 3; n < varBins.size(); n++)
		    {
		      std::cout << "Handling bin for mass bin > " << varBins[n] << ": ";
		      parameters = lambdaVEvents(n);
		      fileStr += writeToFile(particle[k], interference[j], helicity[i], varBins[n], parameters, bs[b]);
		    }
		}
	    }
	}
    }
  
  ofstream aFile;
  aFile.open("paramOutput.txt");
  aFile << fileStr;
  aFile.close();
}
