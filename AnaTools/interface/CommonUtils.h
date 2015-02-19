// Common utilities that may be used by multiple modules.
//
// Note that templated functions must be implemented in this header file,
// rather than in a separate implementation file, so that they can be used
// by other modules.

#ifndef COMMON_UTILS
#define COMMON_UTILS

#include <unordered_set>
#include <typeinfo>

#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

// Return whether obj is contained in vec.
#define VEC_CONTAINS(vec, obj) (find (vec.begin (), vec.end (), obj) != vec.end ())

using namespace std;

namespace anatools
{
  template <class InputCollection> bool getCollection (const edm::InputTag &, edm::Handle<InputCollection> &, const edm::Event &);

#if DATA_FORMAT == BEAN
  // Returns the type of physics object, e.g. inputObject of type BNmuon returns "muon"
  string getObjectType (const BNbxlumi&);
  string getObjectType (const BNelectron&);
  string getObjectType (const BNevent&);
  string getObjectType (const BNgenjet&);
  string getObjectType (const BNjet&);
  string getObjectType (const BNmcparticle&);
  string getObjectType (const BNmet&);
  string getObjectType (const BNmuon&);
  string getObjectType (const BNphoton&);
  string getObjectType (const BNprimaryvertex&);
  string getObjectType (const BNsupercluster&);
  string getObjectType (const BNtau&);
  string getObjectType (const BNtrack&);
  string getObjectType (const BNtrigobj&);

  // Returns class name, e.g. inputObject of type BNmuon returns "BNmuon"
  string getObjectClass (const BNbxlumi&);
  string getObjectClass (const BNelectron&);
  string getObjectClass (const BNevent&);
  string getObjectClass (const BNgenjet&);
  string getObjectClass (const BNjet&);
  string getObjectClass (const BNmcparticle&);
  string getObjectClass (const BNmet&);
  string getObjectClass (const BNmuon&);
  string getObjectClass (const BNphoton&);
  string getObjectClass (const BNprimaryvertex&);
  string getObjectClass (const BNsupercluster&);
  string getObjectClass (const BNtau&);
  string getObjectClass (const BNtrack&);
  string getObjectClass (const BNtrigobj&);

#elif DATA_FORMAT == MINI_AOD
  // Returns the type of physics object, e.g. inputObject of type pat::Muon returns "muon"
  string getObjectType (const pat::Electron&);
  string getObjectType (const reco::GenJet&);
  string getObjectType (const pat::Jet&);
  string getObjectType (const pat::PackedGenParticle&);
  string getObjectType (const pat::MET&);
  string getObjectType (const pat::Muon&);
  string getObjectType (const pat::Photon&);
  string getObjectType (const reco::Vertex&);
  string getObjectType (const reco::SuperCluster&);
  string getObjectType (const pat::Tau&);
  string getObjectType (const pat::TriggerObjectStandAlone&);
  string getObjectType (const edm::TriggerResults&);

  // Returns class name, e.g. inputObject of type pat::Muon returns "pat::Muon"
  string getObjectClass (const pat::Electron&);
  string getObjectClass (const reco::GenJet&);
  string getObjectClass (const pat::Jet&);
  string getObjectClass (const pat::PackedGenParticle&);
  string getObjectClass (const pat::MET&);
  string getObjectClass (const pat::Muon&);
  string getObjectClass (const pat::Photon&);
  string getObjectClass (const reco::Vertex&);
  string getObjectClass (const reco::SuperCluster&);
  string getObjectClass (const pat::Tau&);
  string getObjectClass (const pat::TriggerObjectStandAlone&);
  string getObjectClass (const edm::TriggerResults&);
#endif

  // user-defined cases
  string getObjectType (const VariableProducerPayload&);
  string getObjectClass (const VariableProducerPayload&);


#if DATA_FORMAT == BEAN
  // Return a (hopefully) unique hashed integer for an object
  template <class InputObject> int getObjectHash (const InputObject&);
  // Treat special cases of objects without a 3-momentum
  template<> int getObjectHash<BNevent> (const BNevent&);
  template<> int getObjectHash<BNmet> (const BNmet&);
  template<> int getObjectHash<BNprimaryvertex> (const BNprimaryvertex&);
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

  template <class InputObject> double getMember (const InputObject  &obj, const string &member);

  template<class T> T invoke (const string &returnType, Reflex::Object * const o, const string &member);
  void addDeclaringScope (const Reflex::Scope &scope, string &baseName);
}

template <class InputCollection> bool anatools::getCollection(const edm::InputTag& label, edm::Handle<InputCollection>& coll, const edm::Event &event) {
  // Get a collection with the specified type, and match the product instance name.
  // Do not use Event::getByLabel() function, since it also matches the module name.
  event.getByLabel(label, coll);
  if (!coll.isValid()) {
    vector<edm::Handle<InputCollection> > objVec;
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
      coll = objVec.at(collWithFewestParents);
    if (!coll.isValid()) {
      clog << "ERROR: could not get input collection with product instance label: " << label.instance()
           << ", but found " << objVec.size() << " collections of the specified type." << endl;
      return false;
    }
  }
  return true;
}

// version of getMember that doesn't require the "type" as an argument
template <class InputObject> double anatools::getMember(const InputObject  &obj, const string &member)
{
  string type = getObjectClass(*obj);
  return getMember(type, &(*obj), member);
}


#if DATA_FORMAT == BEAN
// generic function to calculate a hash value of any input object
// hash is based on the 3-vector where available.
// if not, the 3-position is used.
// some special cases exist as well.
template <class InputObject> int anatools::getObjectHash(const InputObject& object){
    int px_mev, py_mev, pz_mev;
    px_mev = fabs(int(1000 * object.px));
    py_mev = fabs(int(1000 * object.py));
    pz_mev = fabs(int(1000 * object.pz));
    return px_mev + py_mev + pz_mev;
}
#endif




#endif
