#ifndef __PMCSTRUCT__
#define __PMCSTRUCT__
#include <string>
#include <TH1.h>
#include <vector>
//#include <iostream>

struct PMCstruct
{
  TH1F* hist;
  bool isBB;
  bool isBE;
};

#endif
