// Common utilities that may be used by multiple modules.
//
// Note that templated functions must be implemented in this header file,
// rather than in a separate implementation file, so that they can be used
// by other modules.

#ifndef COMMON_UTILS
#define COMMON_UTILS

#include <iostream>
#include <unordered_set>
#include <typeinfo>

#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

namespace anatools
{
  template <class T> bool getCollection (const edm::InputTag& label, edm::Handle<T>& collection, const edm::Event& event, bool verbose = true);

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
  string getObjectClass (const osu::Dtcseg &);
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
#if IS_VALID(pileupinfos)
  string getObjectType (const osu::PileUpInfo &);
  string getObjectClass (const osu::PileUpInfo &);
#endif
#if IS_VALID(trigobjs)
  string getObjectType (const osu::Trigobj &);
  string getObjectClass (const osu::Trigobj &);
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
  void getRequiredCollections (const unordered_set<string> &, const edm::ParameterSet &, Collections &, const edm::Event &);

  double getMember (const string &type, const void * const obj, const string &member);

  template <class T> double getMember (const T &obj, const string &member);

#ifdef ROOT6
  template<class T> T invoke (const string &, edm::ObjectWithDict * const, const edm::FunctionWithDict &);
#else
  const Reflex::Object * const getMember (const Reflex::Type &t, const Reflex::Object &o, const string &member, string &memberType);
  const Reflex::Object * const invoke (const string &returnType, const Reflex::Object &o, const string &member);
#endif

  double getGeneratorWeight (const TYPE(generatorweights) &);
}

/**
 * Retrieves a collection from the event, storing it in the second argument.
 *
 * First tries to get a collection with the given type and label. If that
 * fails, gets all collections with the given type and picks the one with the
 * fewest parents, provided its instance label is equal to either the specified label 
 * or "originalFormat", as is the case for a skim.  
 *
 * @param  label product instance label of collection to retrieve
 * @param  collection edm::Handle in which to store the retrieved collection
 * @param  event edm::Event from which to get the collection
 * @return boolean representing whether retrieval was successful
 */
template <class T> bool
anatools::getCollection(const edm::InputTag& label, edm::Handle<T>& collection, const edm::Event &event, bool verbose) {
  event.getByLabel(label, collection);
  if (!collection.isValid()) {
    vector<edm::Handle<T> > objVec;
    event.getManyByType(objVec);
    int collWithFewestParents = -1, fewestParents = 99;
    for (uint i=0; i<objVec.size(); i++) {
      int parents = objVec.at(i).provenance()->parents().size();
      if ((objVec.at(i).provenance()->productInstanceName() == label.instance() || 
	   objVec.at(i).provenance()->productInstanceName() == ORIGINAL_FORMAT) &&
	  parents < fewestParents) {
        collWithFewestParents = i;
        fewestParents = parents;
      }
    }
    if (collWithFewestParents != -1){
      collection = objVec.at(collWithFewestParents);
    }
    else {
      if (verbose) clog << "ERROR: did not find any collections that match input tag:  " << label 
			<< ", with type:  " << typeid(collection).name()  
			<< endl;  
      return false;
    }
    if (!collection.isValid()) {
      if (verbose) clog << "ERROR: could not get input collection with product instance label: " << label.instance()
			<< ", but found " << objVec.size() << " collections of the specified type." << endl;
      return false;
    }
  }
  return true;
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
  return getMember(type, &obj, member);
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

#endif
