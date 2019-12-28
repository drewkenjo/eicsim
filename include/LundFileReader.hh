#ifndef LundFileReader_hh
#define LundFileReader_hh

#include<fstream>
#include<string>
#include "G4String.hh"

class LundFileReader
{
  public:
    LundFileReader(G4String);
    ~LundFileReader();
    std::string getline();

  private:
    std::ifstream lundfile;
};

#endif
