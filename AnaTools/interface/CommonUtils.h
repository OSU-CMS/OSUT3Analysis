// Common utilities that may be used by multiple modules.
//
// Note that templated functions must be implemented in this header file,
// rather than in a separate implementation file, so that they can be used
// by other modules.

#ifndef COMMON_UTILS
#define COMMON_UTILS

#include <iostream>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <typeinfo>

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/BaseWithDict.h"
#include "OSUT3Analysis/AnaTools/interface/FunctionWithDict.h"
#include "OSUT3Analysis/AnaTools/interface/ObjectWithDict.h"
#include "OSUT3Analysis/AnaTools/interface/IterWithDict.h"
#include "OSUT3Analysis/AnaTools/interface/MemberWithDict.h"
#include "OSUT3Analysis/AnaTools/interface/TypeWithDict.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

struct Tokens
{
  edm::EDGetTokenT<vector<osu::Bxlumi> > bxlumis;
  edm::EDGetTokenT<vector<osu::Cschit> > cschits;
  edm::EDGetTokenT<vector<osu::Cscseg> > cscsegs;
  edm::EDGetTokenT<vector<osu::Dtseg> > dtsegs;
  edm::EDGetTokenT<vector<osu::Electron> > electrons;
  edm::EDGetTokenT<vector<osu::Event> > events;
  edm::EDGetTokenT<vector<osu::Genjet> > genjets;
  edm::EDGetTokenT<vector<osu::Jet> > jets;
  edm::EDGetTokenT<vector<osu::Bjet> > bjets;
  edm::EDGetTokenT<vector<osu::Basicjet> > basicjets;
  edm::EDGetTokenT<vector<osu::Mcparticle> > mcparticles;
  edm::EDGetTokenT<vector<osu::Met> > mets;
  edm::EDGetTokenT<vector<osu::Muon> > muons;
  edm::EDGetTokenT<vector<osu::Photon> > photons;
  edm::EDGetTokenT<vector<osu::Primaryvertex> > primaryvertexs;
  edm::EDGetTokenT<vector<osu::Rpchit> > rpchits;
  edm::EDGetTokenT<vector<osu::Supercluster> > superclusters;
  edm::EDGetTokenT<vector<osu::Tau> > taus;
  edm::EDGetTokenT<vector<osu::Track> > tracks;
  edm::EDGetTokenT<vector<osu::SecondaryTrack> > secondaryTracks;
  edm::EDGetTokenT<vector<osu::PileUpInfo> > pileupinfos;

  edm::EDGetTokenT<osu::Beamspot> beamspots;
  edm::EDGetTokenT<TYPE(generatorweights)> generatorweights;
  edm::EDGetTokenT<TYPE(prescales)> prescales;
  edm::EDGetTokenT<TYPE(triggers)> triggers;
  edm::EDGetTokenT<vector<TYPE(trigobjs)> > trigobjs;
  edm::EDGetTokenT<TYPE(triggers)> metFilters;

  vector<edm::EDGetTokenT<osu::Uservariable> > uservariables;
  vector<edm::EDGetTokenT<osu::Eventvariable> > eventvariables;
};

namespace anatools
{
  // Return a (hopefully) unique hashed integer for an object
  template <class T> int getObjectHash (const T &);

#if IS_VALID(beamspots)
  string getObjectType (const osu::Beamspot &);
  string getObjectClass (const osu::Beamspot &);
#endif
#if IS_VALID(bxlumis)
  string getObjectType (const osu::Bxlumi &);
  string getObjectClass (const osu::Bxlumi &);
#endif
#if IS_VALID(cschits)
  string getObjectType (const osu::Cschit &);
  string getObjectClass (const osu::Cschit &);
#endif
#if IS_VALID(cscsegs)
  string getObjectType (const osu::Cscseg &);
  string getObjectClass (const osu::Cscseg &);
#endif
#if IS_VALID(dtsegs)
  string getObjectType (const osu::Dtseg &);
  string getObjectClass (const osu::Dtseg &);
#endif
#if IS_VALID(electrons)
  string getObjectType (const osu::Electron &);
  string getObjectClass (const osu::Electron &);
#endif
#if IS_VALID(events)
  string getObjectType (const osu::Event &);
  string getObjectClass (const osu::Event &);
  template<> int getObjectHash<osu::Event> (const osu::Event &);
#endif
#if IS_VALID(genjets)
  string getObjectType (const osu::Genjet &);
  string getObjectClass (const osu::Genjet &);
#endif
#if IS_VALID(basicjets)
  string getObjectType  (const osu::Basicjet &);
  string getObjectClass (const osu::Basicjet &);
#endif
#if IS_VALID(jets)
  string getObjectType (const osu::Jet &);
  string getObjectClass (const osu::Jet &);
#endif
#if IS_VALID(bjets)
  string getObjectType (const osu::Bjet &);
  string getObjectClass (const osu::Bjet &);
#endif
#if IS_VALID(mcparticles)
  string getObjectType (const osu::Mcparticle &);
  string getObjectClass (const osu::Mcparticle &);
#endif
#if IS_VALID(mets)
  string getObjectType (const osu::Met &);
  string getObjectClass (const osu::Met &);
  template<> int getObjectHash<osu::Met> (const osu::Met &);
#endif
#if IS_VALID(muons)
  string getObjectType (const osu::Muon &);
  string getObjectClass (const osu::Muon &);
#endif
#if IS_VALID(photons)
  string getObjectType (const osu::Photon &);
  string getObjectClass (const osu::Photon &);
#endif
#if IS_VALID(primaryvertexs)
  string getObjectType (const osu::Primaryvertex &);
  string getObjectClass (const osu::Primaryvertex &);
  template<> int getObjectHash<osu::Primaryvertex> (const osu::Primaryvertex &);
#endif
#if IS_VALID(rpchits)
  string getObjectType (const osu::Rpchit &);
  string getObjectClass (const osu::Rpchit &);
#endif
#if IS_VALID(superclusters)
  string getObjectType (const osu::Supercluster &);
  string getObjectClass (const osu::Supercluster &);
#endif
#if IS_VALID(taus)
  string getObjectType (const osu::Tau &);
  string getObjectClass (const osu::Tau &);
#endif
#if IS_VALID(tracks)
  string getObjectType (const osu::Track &);
  string getObjectClass (const osu::Track &);
#endif
#if IS_VALID(secondaryTracks)
  string getObjectType (const osu::SecondaryTrack &);
  string getObjectClass (const osu::SecondaryTrack &);
#endif
#if IS_VALID(pileupinfos)
  string getObjectType (const osu::PileUpInfo &);
  string getObjectClass (const osu::PileUpInfo &);
#endif
#if IS_VALID(uservariables)
  // user-defined cases
  string getObjectType (const VariableProducerPayload&);
  string getObjectClass (const VariableProducerPayload&);
#endif
#if IS_VALID(eventvariables)
  // user-defined cases
  string getObjectType (const EventVariableProducerPayload&);
  string getObjectClass (const EventVariableProducerPayload&);
#endif
  // Extracts the constituent collections from a composite collection name.
  vector<string> getSingleObjects (string);

  // Concatenates collection names with hyphens between.
  string concatenateInputCollection (const vector<string> &);

  // Capitalizes a string.
  string capitalize (string);

  // Removes the trailing 's' from a string.
  string singular (string);

  // Adds an 's' to the end of a string.
  string plural (string);

  // Removes whitespace from the left side of a string.
  string &ltrim (string &);

  // Removes whitespace from the right side of a string.
  string &rtrim (string &);

  // Removes whitespace from both sides of a string.
  string &trim (string &);

  ////////////////////////////////////////////////////////////////////////////////
  // Comparison functions for sorting.
  ////////////////////////////////////////////////////////////////////////////////
  bool firstOfTupleAscending (tuple<size_t, size_t, string>, tuple<size_t, size_t, string>);
  bool firstOfTupleDescending (tuple<size_t, size_t, string>, tuple<size_t, size_t, string>);
  bool collectionIndexAscending (pair<string, DressedObject>, pair<string, DressedObject>);
  ////////////////////////////////////////////////////////////////////////////////

  // Retrieves all the collections from the event which are needed based on the
  // first argument.
  void getRequiredCollections (const unordered_set<string> &, Collections &, const edm::Event &, const Tokens &);

  double getMember (const string &type, void *obj, const string &member, map<pair<string, string>, pair<string, void (*) (void *, int, void **, void *)> > * = NULL);

  template <class T> double getMember (const T &obj, const string &member);

#ifdef ROOT6
  anatools::ObjectWithDict * getMember (const anatools::TypeWithDict &tDerived, const anatools::TypeWithDict &t, const anatools::ObjectWithDict &o, const string &member, string &memberType, map<pair<string, string>, pair<string, void (*) (void *, int, void **, void *)> > *);
  anatools::ObjectWithDict * invoke (const string &returnType, const anatools::ObjectWithDict &o, const anatools::FunctionWithDict &f);
#else
  const Reflex::Object * const getMember (const Reflex::Type &t, const Reflex::Object &o, const string &member, string &memberType);
  const Reflex::Object * const invoke (const string &returnType, const Reflex::Object &o, const string &member);
#endif

  double getGeneratorWeight (const TYPE(generatorweights) &);

  void getAllTokens (const edm::ParameterSet &, edm::ConsumesCollector &&, Tokens &);

  template<class T> bool jetPassesTightLepVeto (const T &);

  template<class T> bool isMatchedToTriggerObject (const edm::Event &, const edm::TriggerResults &, const T &, const vector<pat::TriggerObjectStandAlone> &, const string &, const string &, const double dR = 0.1);

  void logSpace (const unsigned, const double, const double, vector<double> &);
  void linSpace (const unsigned, const double, const double, vector<double> &);
}

/**
 * Returns the value of a member of an object.
 *
 * @param  obj object whose member will be evaluated
 * @param  member string giving the member, data or function, to evaluate
 * @return value of the member of the given object
 */
template <class T> double
anatools::getMember(const T &obj, const string &member)
{
  string type = getObjectClass(obj);
  return getMember(type, (void *) &obj, member);
}

/**
 * Returns a hash value representing the given object.
 *
 * This version applies to objects with a momentum three-vector, and simply
 * returns the sum of the Cartesian coordinates of this vector.
 *
 * @param  object object whose hash value will be calculated
 * @return hash value corresponding to the object
 */
template <class T> int
anatools::getObjectHash(const T& object){
    int px_mev, py_mev, pz_mev;
    px_mev = abs(int(1000 * getMember (object, "px")));
    py_mev = abs(int(1000 * getMember (object, "py")));
    pz_mev = abs(int(1000 * getMember (object, "pz")));
    return px_mev + py_mev + pz_mev;
}

template<class T> bool
anatools::jetPassesTightLepVeto (const T &jet)
{
  // Taken from recommendations from the JetMET group for 13 TeV:
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID#Recommendations_for_13_TeV_data
  //
  return (((jet.neutralHadronEnergyFraction()<0.90 && jet.neutralEmEnergyFraction()<0.90 && (jet.chargedMultiplicity() + jet.neutralMultiplicity())>1 && jet.muonEnergyFraction()<0.8) && ((fabs(jet.eta())<=2.4 && jet.chargedHadronEnergyFraction()>0 && jet.chargedMultiplicity()>0 && jet.chargedEmEnergyFraction()<0.90) || fabs(jet.eta())>2.4) && fabs(jet.eta())<=3.0)
    || (jet.neutralEmEnergyFraction()<0.90 && jet.neutralMultiplicity()>10 && fabs(jet.eta())>3.0));
}

template<class T> bool
anatools::isMatchedToTriggerObject (const edm::Event &event, const edm::TriggerResults &triggers, const T &obj, const vector<pat::TriggerObjectStandAlone> &trigObjs, const string &collection, const string &filter, const double dR)
{
  for (auto trigObj : trigObjs)
    {
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,2,0)
      trigObj.unpackNamesAndLabels(event, triggers);
#else
      trigObj.unpackPathNames(event.triggerNames(triggers));
#endif
      if (trigObj.collection () != collection)
        continue;
      bool flag = false;
      for (const auto &filterLabel : trigObj.filterLabels ())
        if (filterLabel == filter)
          {
            flag = true;
            break;
          }
      if (!flag)
        continue;
      if (deltaR (obj, trigObj) > dR)
        continue;

      return true;
    }
  return false;
}

#endif
