// Common utilities that may be used by multiple modules.
//
// Note that templated functions must be implemented in this header file, 
// rather than in a separate implementation file, so that they can be used 
// by other modules.  

#ifndef COMMON_UTILS
#define COMMON_UTILS

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

#include <vector>

using namespace std;


template <class VecType> bool vecContains(vector< VecType > vec, VecType obj) {
  // Return whether obj is contained in vec.  
  if (find(vec.begin(), vec.end(), obj) != vec.end()) return true;
  else return false;
}


template <class InputCollection> void getCollection(const edm::InputTag& label, edm::Handle<InputCollection>& coll, const edm::Event &event) {                                                         
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
  if (!coll.product()) clog << "ERROR: could not get input collection with product instance label: " << label.instance()                                                                               
                         << ", but found " << objVec.size() << " collections of the specified type." << endl;                                                                                          
}                                                                                                                                                                          


#endif

