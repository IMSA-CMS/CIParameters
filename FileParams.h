#ifndef Testing_hpp
#define Testing_hpp

#include <stdio.h>

/*
 * This is a header file for the fileParams struct
 * Written by Jay Reiter, John Woods, and Matt Hokinson
 * Last Edited: May 9th, 2018
 * Edits to merge fileLocator to FileParams by Kaushal Gumpula and Emily Springer
 * Last Edited: June 11th, 2018
 */

#include <string>
#include <iostream>
#include <map>

enum class Helicity { LL, LR, RL, RR };
enum class Interference { Con, Des };
enum class MassRange { First, Second, Third, Fourth };
enum class Lambda { L1000, L10000, L16000, L22000, L28000, L34000, L100000 };
enum class Particle { Dielectron, Dimuon };
enum class BarrelState { BB, BE, neither };

class TChain;

struct FileParams
{  
  //constructor declaration
  FileParams(Helicity heli, Interference inter, MassRange mrange, Lambda L, Particle part);
    
public: 
  //getters
  std::string getHelicity() { return helicityMap.at(helicity); }
  std::string getInterference() { return interferenceMap.at(interference); }
  std::string getMassRange() { return massRangeMap.at(massRange); }
  std::string getLambda() { return lambdaMap.at(lambda); }
  std::string getParticle() { return particleMap.at(particle); }

  //file parameters
  Helicity helicity;
  Interference interference;
  MassRange massRange;
  Lambda lambda;
  Particle particle;
   
  //creates path
  std::string locateFile();

  //function for parsing and locating textfiles
  TChain* createTChainTextFile(FileParams params, std::string treename);
  TChain* TESTcreateTChainTextFile(FileParams params, std::string treename);
  std::vector<std::string> fileVector(FileParams params);
  std::string locateTextFile();

  int fpToInt() const;     

  //< operator defined for making maps
  //checks if lhs < rhs
  bool operator<(const FileParams& rhs) const { return this->fpToInt() < rhs.fpToInt(); }

private:
  //Hash maps to put to string
  static const std::map<Helicity, string> helicityMap;
  static const std::map<Interference, string> interferenceMap;
  static const std::map<MassRange, string>  massRangeMap;
  static const std::map<Lambda, string> lambdaMap;
  static const std::map<Particle, string> particleMap;

  //support functions for locateFile
  std::string getLambdaPath(); 
  std::string getParticlePath(); 
  std::string getHelicityPath(); 
  std::string getInterferencePath(); 
  std::string getMassRangePath(); 
    
  //support functions for locateTextFile
  //getHelicityPath() isn't translated because it follows the conventions of the text file name
  std::string getParticlePathTextFile();
  std::string getMassRangePathTextFile();
  std::string getLambdaPathTextFile();
  std::string getInterferencePathTextFile();
};

#endif /* Testing_hpp */
