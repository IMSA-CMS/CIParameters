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
#include "TImage.h"
#include <fstream>
#include <iostream>
#include <map>

std::map<FileParams, double> generateXSecMap();
std::vector<double> retrieveCrossSections(const std::map<FileParams, double>& xSecMap, Helicity helicity, Interference interference, Particle particle, Lambda lambda);

std::string returnParticle(Particle particle);
std::string returnInterference(Interference interference);
std::string returnHelicity(Helicity helicity);
std::string returnBarrelState(BarrelState bs);


//creates 4 invariant mass graphs and overlays them on one hist
void lambdaOverlayedMasses(Helicity helicity, Interference interference, Particle particle, BarrelState bs, std::map<FileParams, double> xSecMap)
{
  //target file for overlayed masses (gets opened, analyzed in lambdaVEvents)
  std::cerr << "Replacing AAA lambdaOverlayedMasses.root file...";
  TFile* of = new TFile("lambdaOverlayedMasses.root", "recreate");
  
  std::vector<Lambda> lambdas = {Lambda::L16000, Lambda::L22000, Lambda::L28000, Lambda::L34000};
  
  std::vector<TH1F*> hists = {};

  of->cd();

  for(unsigned i = 0; i < lambdas.size(); i++)
    {
      //get the appropriate cross sections      
      std::vector<double> xSecs = retrieveCrossSections(xSecMap, helicity, interference, particle, lambdas[i]);

      TH1F* h = plotMassCutsReturnHist(lambdas[i], helicity, interference, particle, bs, xSecs);

      //add hist to the end of hists vector
      hists.push_back(h);
      hists[i]->SetLineColor(i+1);
    }
  
  //Draw overlayed hist of data
  TCanvas * c1 = new TCanvas("c1", "Lambdas");
  
  hists[0]->Draw("HIST");
  
  for(unsigned m = 1; m < hists.size(); m++)
    hists[m]->Draw("HIST SAME");

  c1->SetTitle("Lambdas");
  c1->Update();
  of->cd();
  c1->Write();
  for(unsigned n = 0; n < hists.size(); n++)
    {
      hists[n]->Write();
    }

  //put this back in maybe
  //TImage *img = TImage::Create();
  //img->FromPad(c1);
  //img->WriteImage(returnParticle(particle) + returnHelicity(helicity) + returnInterference(interference) + returnBarrelState(bs) + ".png");

  of->Close();
}
