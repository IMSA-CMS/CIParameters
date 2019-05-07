#ifndef __HISTPARAMS__
#define __HISTPARAMS__
#include <string>
//#include <iostream>

struct HistParams
{
  std::string name;
  std::string cuts;
  std::string drawOptions;
  int nbins;
  double min;
  double max;
  double * varBins;
  std::string title;
  int color;
  bool minNull;
  bool maxNull;
  
};

//std::ostream& operator<<(std::ostream s, HistParams hp)
//{
//s << "\n\nPrinting HistParams Opbject: \nName: " << hp.name << "\nCuts: " << hp.cuts << "\nDrawOptions: " << hp.drawOptions << "\nnBins: " << hp.nbins << "\nMin: " << hp.min << "\nMax: " << hp.max << "\nvarBins: " << hp.varBins << "\nTitle: " << hp.title << "\nColor: " << hp.color;
//return s;
//}

#endif
