#include "OSUT3Analysis/AnaTools/interface/EventVariableProducer.h"

EventVariableProducer::EventVariableProducer(const edm::ParameterSet &cfg) :
  collections_  (cfg.getParameter<edm::ParameterSet>  ("collections"))
{

  produces<vector<map<string, double> > > ("extraEventVariables");

  jets_ = collections_.getParameter<edm::InputTag> ("jets");
  muons_ = collections_.getParameter<edm::InputTag> ("muons");
  electrons_ = collections_.getParameter<edm::InputTag> ("electrons");
  taus_ = collections_.getParameter<edm::InputTag> ("taus");
  mets_ = collections_.getParameter<edm::InputTag> ("mets");
  genjets_ = collections_.getParameter<edm::InputTag> ("genjets");
  mcparticles_ = collections_.getParameter<edm::InputTag> ("mcparticles");
  primaryvertexs_ = collections_.getParameter<edm::InputTag> ("primaryvertexs");
  photons_ = collections_.getParameter<edm::InputTag> ("photons");
  triggers_ = collections_.getParameter<edm::InputTag> ("triggers");
}

EventVariableProducer::~EventVariableProducer()
{
}

void
EventVariableProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  // define structure that will be put into the event
  // string is the variable name
  // double is the value of the variable
  // it is a vector because  other collections are also vectors 
  // the vector will have size=1 for each event 
  auto_ptr<vector<map<string, double> > > extraEventVariables (new vector<map<string, double> >);  

  ////////////////////////////////////////////////////////////////////////
  map<string, double> myVars; 
  AddVariables(event, myVars);
  extraEventVariables->push_back(myVars);  

  // store all of our calculated quantities in the event  
  event.put (extraEventVariables, "extraEventVariables");
  extraEventVariables.reset();  

}

void
EventVariableProducer::AddVariables (const edm::Event &event, map<string, double> &myVars) {}


