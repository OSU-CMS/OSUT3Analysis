#ifndef ANALYSIS_TYPES
#define ANALYSIS_TYPES

#include <vector>
#include <string>
#include <map>

using namespace std;

typedef map<string, vector<vector<bool> > > flagMap;

class ValueLookupTree;

struct BadCSC {
  double etaCSC;
  double phiCSC;
};

struct DeadEcal {
  double etaEcal;
  double phiEcal;
};

struct cut
{
  bool            isVeto;
  int             numberRequired;
  string          eventComparativeOperator;
  string          inputCollection;
  string          name;
  string          cutString;
  ValueLookupTree *valueLookupTree;
};

struct histoDef {
  string inputCollection;
  string directory;
  string name;
  string title; // contains axis labels
  vector<double> binsX;
  vector<double> binsY;
  bool hasVariableBinsX;
  bool hasVariableBinsY;
  vector<string> inputVariables;
  vector<ValueLookupTree *> valueLookupTrees;
  int dimensions;
};

struct node
{
  string value;
  vector<node *> branches;
};

#endif
