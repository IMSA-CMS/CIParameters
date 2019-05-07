#ifndef drawFileParams_hpp
#define drawFileParams_hpp

#include <string>
#include <vector>
#include <stdexcept>
#include <stdio.h>

class TH1F;

std::vector<string> makeVariableVector(string particle);
std::vector<string> makeVariableName(FileParams file);
TH1F* makeOneHistogram(std::string name, int color, std::string treeName, std::string variable, FileParams file, HistParams hp);
std::vector<std::vector<TH1F*>> makeFileParams(FileParams file, HistParams hp, std::vector<Lambda> lambdas);
std::vector<TH1F*> makeFileParamsOneComponent(FileParams file, HistParams hp, std::vector<Lambda> lambdas, const int componentchoice);
std::string lambdaName(Lambda lambda);

#endif /* drawFileParams_hpp */
