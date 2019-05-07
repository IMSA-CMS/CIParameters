//Anisha Sharma
//takes a 3-parameter double vector, prints the parameters in a line in new text file
//LL, LR, and RR. return these using a switch statement like it is in lambdaVEvents
//muons: eta<1.2 is barrel, eta>1.2 is endcap
//electrons: eta<1.4442 is barrel, eta>1.562 is endcap

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TLeaf.h"
#include <vector>
#include <stdexcept>
#include <string>
#include "FileParams.h"
#include <iostream>
#include <fstream> //to create and write into text file


std::string returnParticle(Particle particle);
std::string returnInterference(Interference interference);
std::string returnHelicity(Helicity helicity);
std::string returnBarrelState(BarrelState bs);
std::string writeToFile(Particle particle, Interference interference, Helicity helicity, int bin, std::vector<double> parameters);



std::string returnParticle(Particle particle){
  switch(particle)
    {
    case Particle::Dimuon:
      return "dimuon";
    case Particle::Dielectron:
      return "dielectron";
    default:
      return "ERROR: Particle case not handled!";
    }
}

std::string returnInterference(Interference interference){
  switch(interference)
    {
    case Interference::Con:
      return "Con"; 
    case Interference::Des:
      return "Des";
    default:
      return "ERROR: Interference case not handled!";
    }
}

std::string returnHelicity(Helicity helicity){
 switch(helicity)
   {
   case Helicity::LL:
     return "LL";
   case Helicity::LR:
     return "LR";
   case Helicity::RR:
     return "RR";
   case Helicity::RL:
     return "RL";
   default:
     return "ERROR: Helicity case not handled!";
   }
}

std::string returnBarrelState(BarrelState bs)
{
  switch(bs)
    {
    case BarrelState::BB:
      return "BB";
    case BarrelState::BE:
      return "BE";
    default:
      return "[ Bad Barrel State :( ]";
    }
}

std::string writeToFile(Particle particle, Interference interference, Helicity helicity, int bin, std::vector<double> parameters, BarrelState bs)
{
  std::string output = returnParticle(particle) + "_IMSA2019 " + returnBarrelState(bs) + " " + returnInterference(interference) + returnHelicity(helicity) + " " + std::to_string(bin) + " nominal " + std::to_string(parameters[0]) + " " + std::to_string(parameters[1]) + " " + std::to_string(parameters[2]) + "\n";

  std::cout << "\n" + output + "\n";

  return output;
}

