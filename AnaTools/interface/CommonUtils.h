// Common utilities that may be used by multiple modules.
//
// Note that templated functions must be implemented in this header file,
// rather than in a separate implementation file, so that they can be used
// by other modules.

#ifndef COMMON_UTILS
#define COMMON_UTILS

#include <unordered_set>

#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

// Return whether obj is contained in vec.
#define VEC_CONTAINS(vec, obj) (find (vec.begin (), vec.end (), obj) != vec.end ())

using namespace std;

namespace anatools
{
  template <class InputCollection> bool getCollection(const edm::InputTag& label, edm::Handle<InputCollection>& coll, const edm::Event &event) {
    // Get a collection with the specified type, and match the product instance name.
    // Do not use Event::getByLabel() function, since it also matches the module name.
    vector< edm::Handle<InputCollection> > objVec;
    event.getManyByType(objVec);
    for (uint i=0; i<objVec.size(); i++) {
      if (label.instance() == objVec.at(i).provenance()->productInstanceName()) {
        coll = objVec.at(i);
        break;
      }
    }
    if (!coll.isValid()) {
      clog << "ERROR: could not get input collection with product instance label: " << label.instance()
           << ", but found " << objVec.size() << " collections of the specified type." << endl;
      return false;
    }
    return true;
  }

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

  void getRequiredCollections (const unordered_set<string> &, const edm::ParameterSet &, Collections &, const edm::Event &);
};

#endif
