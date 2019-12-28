#include "LundFileReader.hh"

LundFileReader::LundFileReader(G4String filename)
{
  lundfile.open(filename.data());
}

LundFileReader::~LundFileReader()
{
  lundfile.close();
}

std::string LundFileReader::getline()
{
  std::string line;
  std::getline(lundfile,line);
  return line;
}

