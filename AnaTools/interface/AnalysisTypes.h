#ifndef ANALYSIS_TYPES
#define ANALYSIS_TYPES

#include <vector>
#include <string>
#include <map>

#include "boost/variant.hpp"

#include "DataFormats/Common/interface/Handle.h"

#include "BEAN/Collections/interface/BNbxlumi.h"
#include "BEAN/Collections/interface/BNelectron.h"
#include "BEAN/Collections/interface/BNevent.h"
#include "BEAN/Collections/interface/BNjet.h"
#include "BEAN/Collections/interface/BNmcparticle.h"
#include "BEAN/Collections/interface/BNmet.h"
#include "BEAN/Collections/interface/BNmuon.h"
#include "BEAN/Collections/interface/BNphoton.h"
#include "BEAN/Collections/interface/BNprimaryvertex.h"
#include "BEAN/Collections/interface/BNskimbits.h"
#include "BEAN/Collections/interface/BNsupercluster.h"
#include "BEAN/Collections/interface/BNtrack.h"
#include "BEAN/Collections/interface/BNtrigger.h"
#include "BEAN/Collections/interface/BNtrigobj.h"
#include "BEAN/Collections/interface/BNtau.h"
#include "BEAN/Collections/interface/BNgenjet.h"

using namespace std;

class ValueLookupTree;

typedef boost::variant<double, string> Leaf;

typedef vector<map<string, vector<pair<bool, bool> > > > FlagMap;

typedef vector<map<string, double> > VariableProducerPayload;

struct Cut
{
  ValueLookupTree  *valueLookupTree;
  bool             isVeto;
  int              numberRequired;
  string           cutString;
  string           eventComparativeOperator;
  string           inputLabel;
  string           name;
  vector<string>   inputCollections;
};

struct CutCalculatorPayload
{
  FlagMap         cumulativeObjectFlags;
  FlagMap         objectFlags;
  bool            cutDecision;
  bool            eventDecision;
  bool            isValid;
  bool            triggerDecision;
  vector<Cut>     cuts;
  vector<bool>    eventFlags;
  vector<bool>    triggerFlags;
  vector<bool>    vetoTriggerFlags;
  vector<string>  triggers;
  vector<string>  triggersToVeto;
};

struct HistoDef {
  vector<string> inputCollections;
  string inputLabel;
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

struct Node
{
  Node           *parent;
  string         type;
  string         value;
  vector<Node*>  branches;
};

struct Collections
{
  edm::Handle<BNbxlumiCollection>         bxlumis;
  edm::Handle<BNelectronCollection>       electrons;
  edm::Handle<BNeventCollection>          events;
  edm::Handle<BNgenjetCollection>         genjets;
  edm::Handle<BNjetCollection>            jets;
  edm::Handle<BNmcparticleCollection>     mcparticles;
  edm::Handle<BNmetCollection>            mets;
  edm::Handle<BNmuonCollection>           muons;
  edm::Handle<BNphotonCollection>         photons;
  edm::Handle<BNprimaryvertexCollection>  primaryvertexs;
  edm::Handle<BNsuperclusterCollection>   superclusters;
  edm::Handle<BNtauCollection>            taus;
  edm::Handle<BNtrackCollection>          tracks;
  edm::Handle<BNtriggerCollection>        triggers;
  edm::Handle<BNtrigobjCollection>        trigobjs;
  edm::Handle<VariableProducerPayload>    userVariables;
};

#endif
