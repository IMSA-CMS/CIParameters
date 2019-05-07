//Creates histograms at 7 different lambda values for 4 different components (Pt, Phi, Eta, and Mass)
//Written By: Emily Springer and Kaushal Gumpula
//Last Edited: June 14th, 2018

#include "FileParams.h"
#include "HistParams.h"
#include "drawPlotCopy.h"
#include "drawFileParams.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TLegend.h"
#include <stdexcept>
#include "TChain.h"
#include <string>
#include <climits>
#include "TCanvas.h" 
#include <vector>
#include <stdio.h>
/*
//overlays histograms of "variable" across the seven possible lambda values
void drawFileParams()
{
  //hardcoded histparams parameters. 
  //empty string parameters will be set automatically, set to dummy values for now
  HistParams hp;
  hp.name = "";
  hp.cuts = "";
  //HistParams.drawOption = "yes";
  hp.nbins = 100;
  hp.min = 0;
  hp.max = 2000;
  hp.title = "";
  hp.color = 1;
  //set just hp.maxNull to true to let root autoscale histogram x-axis with min at x=0
  hp.minNull = false;
  hp.maxNull = false;

  //Hard Coded Parameters, Lambda variable value is a dummy value as it will get changed later
  FileParams file = FileParams(Helicity::LL, Interference::Des, MassRange::Second, Lambda::L16000, Particle::Dimuon); 

  //Lambda Vector
  //std::vector<Lambda> lambdas = { Lambda::L1000, Lambda::L10000, Lambda::L16000, Lambda::L22000, Lambda::L28000, Lambda::L34000, Lambda::L100000};
std::vector<Lambda> lambdas = { Lambda::L16000, Lambda::L22000, Lambda::L28000, Lambda::L34000};

  //0 for Pt(Transverse Momentum), 1 for Phi, 2 for Eta, 3 for M(Mass)
  const int variableChoice = 0;

  //stores a vector of a vector of hists from makeFileParams() 
  std::vector<std::vector<TH1F*>> hists = makeFileParams(file, hp, lambdas);
  TCanvas *c = new TCanvas("c");
  //makes a legend
  auto legend = new TLegend(0.85,0.4,1.0,0.65);
  for(std::size_t i = 0; i < hists.size(); ++i)
  {   
      //normalizes histograms
      hists[i][variableChoice]->Scale(1 / hists[i][variableChoice]->Integral());
      //overlays each histogram
      hists[i][variableChoice]->Draw(i == 0 ? "" : "SAME"); 
      //matches each appropriate histogram line color to lambda value in the legend
      string label = "#Lambda = "+lambdaName(lambdas[i]);
      legend->AddEntry(hists[i][variableChoice], label.c_str(), "l");
  }
  legend->Draw();
  c->Update();
}*/

/*
//makes the 2d vector that contains pt, phi, eta and mass hists for the 7 lambda values
std::vector<std::vector<TH1F*>> makeFileParams(FileParams file, HistParams hp, std::vector<Lambda> lambdas)
{
  std::vector<std::vector<TH1F*>> hists; 
  //std::string name = "cool graph";
  int color = 1;

  //number of components needed for invariant mass calculation (Pt, Phi, Eta, M.
  const int fourVectorsize = 4;

  //iterates through each lambda value and then pushes back a vector to iterate through each variable: Pt(Transverse Momentum), Phi, Eta, M(Mass)
  for(std::size_t i = 0; i<lambdas.size();++i)
  {
    file.lambda = lambdas[i];
    hists.push_back(std::vector<TH1F*>());
    std::string name;
    for(std::size_t a = 0; a< fourVectorsize; ++a)
	{
	  switch(a)
	    {
	    case 0:
	      name = "Pt";
	      break;
	    case 1:
	      name = "Phi";
	      break;
	    case 2:
	      name = "Eta";
	      break;
	    case 3:
	      name = "Mass";
	      break;
	 
   }


	  
	  std::vector<std::string> variableName = makeVariableName(file);
	  //name for each histogram must be different
	  hists[i].push_back(makeOneHistogram(name+std::to_string(i)+std::to_string(a),color+i,"Events", variableName[a], file, hp));
	}
   }
  return hists;
}
*/
 /*
//makes a 1d vector that either contains pt, phi, eta or mass 
//not used in drawFileParams but a helpful function for others to call if they don't need the four vector->saves time
std::vector<TH1F*> makeFileParamsOneComponent(FileParams file, HistParams hp, std::vector<Lambda> lambdas, const int componentchoice)
{
  //Pass in 0 for Pt, 1 for Phi, 2 for Eta, and 3 for M(Mass) for the componentchoice argument
  std::vector<TH1F*> hists; 
  int color = 1;
  //iterates through each lambda value and then pushes back a vector to iterate through each variable: Pt(Transverse Momentum), Phi, Eta, M(Mass)
  for(std::size_t i = 0; i<lambdas.size();++i)
  {
    file.lambda = lambdas[i];
    std::string name;
    switch(componentchoice)
    {
     case 0:
       name = "Pt";
     case 1:
       name = "Phi";
     case 2:
       name = "Eta";
     case 3:
       name = "Mass";
    }
     std::vector<std::string> variableName = makeVariableName(file);
     //name for each histogram must be different
     hists.push_back(makeOneHistogram(name+std::to_string(i)+std::to_string(componentchoice),color+i,"Events", variableName[componentchoice], file, hp));
   }
  return hists;
}*/


/*
//creates the variable name
std::vector<string> makeVariableName(FileParams file)
{
  if(file.getParticle() == "EE")
    {
      return makeVariableTypeVector("Electrons");
    }
  else if (file.getParticle() == "MUMU")
    {
      return makeVariableTypeVector("Muons");
    }
  else
    {
      throw std::runtime_error("invalid particle input");
    }
}
*/

//makes histograms for each component (Pt, Phi, Eta, and Mass) and returns and vector of these histograms
TH1F* makeOneHistogram(std::string name, int color, std::string treeName, std::string variable, FileParams file, HistParams hp)
{
  hp.name = name;
  hp.color = color;
  hp.title = variable;
  
  auto chain = new TChain(treeName.c_str());
  chain->Add("/uscms/home/jreiter/MassParameters/sample/*.root");
  TH1F* h = drawHist(chain, variable, hp);
  
  //TH1F* h = drawHist("test.root", "tree", variable, hp);
  return h;
}

/*
//sets lambda name for the respective lambda value (Lambda::L16000 -> 16TeV)
//useful to dispalying in the hist
std::string lambdaName(Lambda lambda)
{
  switch(lambda)
    {
    case Lambda::L100000:
      return "100kTeV";
    case Lambda::L34000:
      return "34TeV"; 
    case Lambda::L28000:
      return "28TeV"; 
    case Lambda::L22000:
      return "22TeV"; 
    case Lambda::L16000:
      return "16TeV";   
    case Lambda::L10000:
      return "10TeV";
    case Lambda::L1000:
      return "1TeV";
    }
}
*/
