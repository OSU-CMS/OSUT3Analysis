#ifndef VARIABLE_PRODUCER

#define VARIABLE_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"
#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
using anatools::objectHash;

using namespace std;



class VariableProducer : public edm::EDProducer
  {
    public:
      VariableProducer (const edm::ParameterSet &);
      ~VariableProducer ();

      void produce (edm::Event &, const edm::EventSetup &);

    protected:

      edm::ParameterSet collections_;
      Collections handles_;
      unordered_set<string> objectsToGet_;

      template<typename... Objects> void addUserVar (auto_ptr<VariableProducerPayload> &myVars, string varName, double value, Objects... objs);  
      void addUserVar (auto_ptr<VariableProducerPayload> &myVars, string varName, double value, vector<int>& hashes);  
      template<typename Object, typename... Objects> void addUserVar (auto_ptr<VariableProducerPayload> &myVars, string varName, double value, vector<int>& hashes, Object obj, Objects... objs);  

    private:

      // Methods

      virtual void AddVariables(const edm::Event &, auto_ptr<VariableProducerPayload> &);

  };


// Templated variadic functions are confusing!
// Try these references:
// https://www.ibm.com/developerworks/community/blogs/5894415f-be62-4bc0-81c5-3956e82276f3/entry/c_templates_what_is_a_variadic_template_function41?lang=en
// http://en.cppreference.com/w/cpp/language/parameter_pack  
// 
// This version of addUserVar(), with no argument for "hashes", 
// should be called in MyVariableProducer.cc.  
template<typename... Objects> void VariableProducer::addUserVar (auto_ptr<VariableProducerPayload> &myVars, string varName, double value, Objects... objs) { 
  vector<int> hashes;  
  addUserVar(myVars, varName, value, hashes, objs...);  // Now call the recursive function.  
}  


// This is the recursive version.  
template<typename Object, typename... Objects> void VariableProducer::addUserVar(auto_ptr<VariableProducerPayload> &myVars, string varName, double value, vector<int>& hashes, Object obj, Objects... objs) {
  hashes.push_back(objectHash(*obj));
  addUserVar(myVars, varName, value, hashes, objs...);  
  // Recursively call itself until the size of objs is 0.  
  // Then the non-recursive function will be called once.  
}

// This is the non-recursive version.  
void VariableProducer::addUserVar (auto_ptr<VariableProducerPayload> &myVars, string varName, double value, vector<int>& hashes) {
  // Now there are no more objects to put into hashes, 
  // so the new variable is created.  
  UserVar newUserVar = make_pair(hashes, value);   
  (*myVars)[varName].push_back(newUserVar);    

}  


#endif
