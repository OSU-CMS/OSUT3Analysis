#ifndef VARIABLE_PRODUCER

#define VARIABLE_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"
#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

class VariableProducer : public edm::EDProducer
  {
    public:
      VariableProducer (const edm::ParameterSet &);
      ~VariableProducer ();

      // Methods

      void produce (edm::Event &, const edm::EventSetup &);

    protected:

      // Variables

      edm::ParameterSet collections_;
      Collections handles_;
      unordered_set<string> objectsToGet_;
      auto_ptr<VariableProducerPayload> userVariables;

      // Methods

      template<typename... Objects> void addUserVar (string varName, double value, Objects... objs);
      void addUserVar (string varName, double value, ObjectList objects);
      template<typename Object, typename... Objects> void addUserVar (string varName, double value, ObjectList objects, Object obj, Objects... objs);

    private:

      // Methods

      virtual void AddVariables(const edm::Event &);

  };


// Templated variadic functions are confusing!
// Try these references:
// https://www.ibm.com/developerworks/community/blogs/5894415f-be62-4bc0-81c5-3956e82276f3/entry/c_templates_what_is_a_variadic_template_function41?lang=en
// http://en.cppreference.com/w/cpp/language/parameter_pack


// This version of addUserVar(), with no argument for "hashes", 
// should be called in MyVariableProducer.cc.
template<typename... Objects> void VariableProducer::addUserVar (string varName, double value, Objects... objs) { 
  ObjectList objects;
  addUserVar(varName, value, objects, objs...);  // Now call the recursive function.
}


// This is the recursive version.
template<typename Object, typename... Objects> void VariableProducer::addUserVar (string varName, double value, ObjectList objects, Object obj, Objects... objs) {
  // "obj" is the first object in the list
  // calculate it's type and hash
  // and add them into the list to save
  auto type = anatools::getObjectType(*obj);
  auto hash = anatools::getObjectHash(*obj);
  objects.insert(make_pair(type, hash));

  // Recursively call itself until the size of objs is 0.
  // Then the non-recursive function will be called once.
  addUserVar(varName, value, objects, objs...);
}


// This is the non-recursive version.
void VariableProducer::addUserVar (string varName, double value, ObjectList objects) {
  // Now there are no more objects to put into hashes, 
  // so the new variable is created.
  UserVariable userVar = {};
  userVar.value = value;
  userVar.objects = objects;
  (*userVariables)[varName].push_back(userVar);  
}


#endif
