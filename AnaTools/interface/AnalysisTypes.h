#ifndef ANALYSIS_TYPES
#define ANALYSIS_TYPES

#include "boost/variant.hpp"

#include "DataFormats/Common/interface/Handle.h"

#include "OSUT3Analysis/Collections/interface/Beamspot.h"
#include "OSUT3Analysis/Collections/interface/Bxlumi.h"
#include "OSUT3Analysis/Collections/interface/Cschit.h"
#include "OSUT3Analysis/Collections/interface/Cscseg.h"
#include "OSUT3Analysis/Collections/interface/Dtseg.h"
#include "OSUT3Analysis/Collections/interface/Electron.h"
#include "OSUT3Analysis/Collections/interface/Event.h"
#include "OSUT3Analysis/Collections/interface/Eventvariable.h"
#include "OSUT3Analysis/Collections/interface/Genjet.h"
#include "OSUT3Analysis/Collections/interface/Jet.h"
#include "OSUT3Analysis/Collections/interface/Mcparticle.h"
#include "OSUT3Analysis/Collections/interface/Met.h"
#include "OSUT3Analysis/Collections/interface/Muon.h"
#include "OSUT3Analysis/Collections/interface/Photon.h"
#include "OSUT3Analysis/Collections/interface/Primaryvertex.h"
#include "OSUT3Analysis/Collections/interface/Rpchit.h"
#include "OSUT3Analysis/Collections/interface/Supercluster.h"
#include "OSUT3Analysis/Collections/interface/Tau.h"
#include "OSUT3Analysis/Collections/interface/Track.h"
#include "OSUT3Analysis/Collections/interface/Trigobj.h"
#include "OSUT3Analysis/Collections/interface/Uservariable.h"
#include "OSUT3Analysis/Collections/interface/PileUpInfo.h"


class ValueLookupTree;

typedef boost::variant<double, string> Leaf;

typedef vector<map<string, vector<pair<bool, bool> > > > FlagMap;

struct Cut
{
  ValueLookupTree  *valueLookupTree;
  ValueLookupTree  *arbitrationTree;
  bool             isVeto;
  int              numberRequired;
  string           cutString;
  string           eventComparativeOperator;
  string           inputLabel;
  string           name;
  vector<string>   inputCollections;
  string           arbitration;
};

typedef vector<Cut> Cuts;

struct CutCalculatorPayload
{
  FlagMap         cumulativeObjectFlags;
  FlagMap         individualObjectFlags;
  bool            cutDecision;         // whether event passes current cut (independant from other cuts)
  bool            cutsDecision;        // whether event passes all cuts, without trigger
  bool            eventDecision;       // whether event passes all cuts and the trigger
  bool            isValid;
  bool            triggerDecision;
  bool            triggerFilterDecision;
  bool            metFilterDecision;
  Cuts            cuts;
  vector<bool>    cumulativeEventFlags;
  vector<bool>    individualEventFlags;
  vector<bool>    triggerFlags;
  vector<bool>    vetoTriggerFlags;
  vector<bool>    triggerFilterFlags;
  vector<bool>    triggerInMenuFlags;
  vector<bool>    metFilterFlags;
  vector<string>  triggers;
  vector<string>  triggersToVeto;
  vector<string>  triggerFilters;
  vector<string>  triggersInMenu;
  vector<string>  metFilters;
};

struct HistoDef {
  vector<string> inputCollections;
  string inputLabel;
  string directory;
  string name;
  string title; // contains axis labels
  vector<double> binsX;
  vector<double> binsY;
  vector<double> binsZ;
  int indexX;
  int indexY;
  int indexZ;
  bool hasVariableBinsX;
  bool hasVariableBinsY;
  bool hasVariableBinsZ;
  vector<string> inputVariables;
  vector<ValueLookupTree *> valueLookupTrees;
  int dimensions;
  bool weight;
};

struct BranchDef {
  vector<string> inputCollections;
  string branchName;
  int index;
  vector<string> inputVariables;
  vector<ValueLookupTree *> valueLookupTrees;
  double value;
};

struct Weight
{
  vector<string> inputCollections;
  string inputVariable;
  ValueLookupTree *valueLookupTree;
  double product;
};

struct ScaleFactor
{
  string inputCollection;
  string sfType;
  string version;
  string wp;
  double additionalSystematic;
  double additionalSystematicBelow20GeV;
  double additionalSystematicAbove80GeV;
  vector<string> inputPlots;
  vector<double> inputLumis;
  string outputVariable;
};

struct Node
{
  Node            *parent;
  string          value;
  vector<Node *>  branches;
};

struct Collections
{
  edm::Handle<osu::Beamspot>                beamspots;
  edm::Handle<vector<osu::Bxlumi> >         bxlumis;
  edm::Handle<vector<osu::Cschit> >         cschits;
  edm::Handle<vector<osu::Cscseg> >         cscsegs;
  edm::Handle<vector<osu::Dtseg> >          dtsegs;
  edm::Handle<vector<osu::Electron> >       electrons;
  edm::Handle<vector<osu::Event> >          events;
  edm::Handle<vector<osu::Genjet> >         genjets;
  edm::Handle<vector<osu::Jet> >            jets;
  edm::Handle<vector<osu::Bjet> >           bjets;
  edm::Handle<vector<osu::Basicjet> >       basicjets;
  edm::Handle<vector<osu::Mcparticle> >     mcparticles;
  edm::Handle<vector<osu::Met> >            mets;
  edm::Handle<vector<osu::Muon> >           muons;
  edm::Handle<vector<osu::Photon> >         photons;
  edm::Handle<vector<osu::Primaryvertex> >  primaryvertexs;
  edm::Handle<vector<osu::Rpchit> >         rpchits;
  edm::Handle<vector<osu::Supercluster> >   superclusters;
  edm::Handle<vector<osu::Tau> >            taus;
  edm::Handle<vector<osu::Track> >          tracks;
  edm::Handle<vector<osu::SecondaryTrack> > secondaryTracks;
  edm::Handle<vector<osu::PileUpInfo> >     pileupinfos;
  vector<edm::Handle<osu::Uservariable> >   uservariables;
  vector<edm::Handle<osu::Eventvariable> >  eventvariables;

  edm::Handle<TYPE(triggers)>                 triggers;
  edm::Handle<vector<TYPE(trigobjs)> >        trigobjs;
  edm::Handle<TYPE(prescales)>                prescales;
  edm::Handle<TYPE(generatorweights)>         generatorweights;
  edm::Handle<TYPE(triggers)>                 metFilters;
};

struct ValueToPrint
{
  ValueLookupTree  *valueLookupTree;
  string           inputLabel;
  string           valueToPrint;
  vector<string>   inputCollections;
};

typedef vector<ValueToPrint> ValuesToPrint;

struct DressedObject
{
  unsigned collectionIndex;
  unsigned localIndex;
  void *addr;
};

#endif
