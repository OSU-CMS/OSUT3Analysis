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

// Return whether obj is contained in vec.
#define VEC_CONTAINS(vec, obj) (find (vec.begin (), vec.end (), obj) != vec.end ())

namespace anatools
{
  template <class T> bool getCollection (const edm::InputTag &, edm::Handle<T> &, const edm::Event &);

  // Return a (hopefully) unique hashed integer for an object
  template <class T> int getObjectHash (const T &);

#if IS_VALID(beamspots)
  string getObjectType (const TYPE(beamspots) &);
  string getObjectClass (const TYPE(beamspots) &);
#endif
#if IS_VALID(bxlumis)
  string getObjectType (const TYPE(bxlumis) &);
  string getObjectClass (const TYPE(bxlumis) &);
#endif
#if IS_VALID(electrons)
  string getObjectType (const TYPE(electrons) &);
  string getObjectClass (const TYPE(electrons) &);
#endif
#if IS_VALID(events)
  string getObjectType (const TYPE(events) &);
  string getObjectClass (const TYPE(events) &);
  template<> int getObjectHash<TYPE(events)> (const TYPE(events) &);
#endif
#if IS_VALID(genjets)
  string getObjectType (const TYPE(genjets) &);
  string getObjectClass (const TYPE(genjets) &);
#endif
#if IS_VALID(jets)
  string getObjectType (const TYPE(jets) &);
  string getObjectClass (const TYPE(jets) &);
#endif
#if IS_VALID(mcparticles)
  string getObjectType (const TYPE(mcparticles) &);
  string getObjectClass (const TYPE(mcparticles) &);
#endif
#if IS_VALID(mets)
  string getObjectType (const TYPE(mets) &);
  string getObjectClass (const TYPE(mets) &);
  template<> int getObjectHash<TYPE(mets)> (const TYPE(mets) &);
#endif
#if IS_VALID(muons)
  string getObjectType (const TYPE(muons) &);
  string getObjectClass (const TYPE(muons) &);
#endif
#if IS_VALID(photons)
  string getObjectType (const TYPE(photons) &);
  string getObjectClass (const TYPE(photons) &);
#endif
#if IS_VALID(primaryvertexs)
  string getObjectType (const TYPE(primaryvertexs) &);
  string getObjectClass (const TYPE(primaryvertexs) &);
  template<> int getObjectHash<TYPE(primaryvertexs)> (const TYPE(primaryvertexs) &);
#endif
#if IS_VALID(superclusters)
  string getObjectType (const TYPE(superclusters) &);
  string getObjectClass (const TYPE(superclusters) &);
#endif
#if IS_VALID(taus)
  string getObjectType (const TYPE(taus) &);
  string getObjectClass (const TYPE(taus) &);
#endif
#if IS_VALID(tracks)
  string getObjectType (const TYPE(tracks) &);
  string getObjectClass (const TYPE(tracks) &);
#endif
#if IS_VALID(trigobjs)
  string getObjectType (const TYPE(trigobjs) &);
  string getObjectClass (const TYPE(trigobjs) &);
#endif
#if IS_VALID(uservariables)
  // user-defined cases
  string getObjectType (const VariableProducerPayload&);
  string getObjectClass (const VariableProducerPayload&);
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
  bool firstOfPairAscending (pair<size_t, string>, pair<size_t, string>);
  bool collectionIndexAscending (pair<string, tuple<unsigned, unsigned, void *> >, pair<string, tuple<unsigned, unsigned, void *> >);
  ////////////////////////////////////////////////////////////////////////////////

  // Retrieves all the collections from the event which are needed based on the
  // first argument.
  void getRequiredCollections (const unordered_set<string> &, const edm::ParameterSet &, Collections &, const edm::Event &);

  double getMember (const string &type, const void * const obj, const string &member);

  template <class T> double getMember (const T &obj, const string &member);

#ifdef ROOT6
  template<class T> T invoke (const string &, edm::ObjectWithDict * const, const edm::FunctionWithDict &);
#else
  const Reflex::Object &getMember (const Reflex::Type &t, const Reflex::Object &o, const string &member, string &memberType);
  const Reflex::Object &invoke (const string &returnType, const Reflex::Object &o, const string &member);
#endif
}

/**
 * Retrieves a collection from the event, storing it in the second argument.
 *
 * First tries to get a collection with the given type and label. If that
 * fails, gets all collections with the given type and picks the one with the
 * fewest parents.
 *
 * @param  label product instance label of collection to retrieve
 * @param  collection edm::Handle in which to store the retrieved collection
 * @param  event edm::Event from which to get the collection
 * @return boolean representing whether retrieval was successful
 */
template <class T> bool
anatools::getCollection(const edm::InputTag& label, edm::Handle<T>& collection, const edm::Event &event) {
  event.getByLabel(label, collection);
  if (!collection.isValid()) {
    vector<edm::Handle<T> > objVec;
    event.getManyByType(objVec);
    int collWithFewestParents = -1, fewestParents = -1;
    for (uint i=0; i<objVec.size(); i++) {
      int parents = objVec.at(i).provenance()->parents().size();
      if (label.instance() == objVec.at(i).provenance()->productInstanceName() && parents > fewestParents) {
        collWithFewestParents = i;
        fewestParents = parents;
      }
    }
    if (collWithFewestParents >= 0)
      collection = objVec.at(collWithFewestParents);
    else {
      clog << "ERROR: did not find any collections of the specified type." << endl;
      return false;
    }
    if (!collection.isValid()) {
      clog << "ERROR: could not get input collection with product instance label: " << label.instance()
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
