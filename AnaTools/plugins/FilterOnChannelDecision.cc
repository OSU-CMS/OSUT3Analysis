#include "OSUT3Analysis/AnaTools/plugins/FilterOnChannelDecision.h"

FilterOnChannelDecision::FilterOnChannelDecision (const edm::ParameterSet &cfg) :
  src_ (cfg.getParameter<edm::InputTag> ("src")),
  channelName_ (cfg.getParameter<string> ("channelName"))
{
}

FilterOnChannelDecision::~FilterOnChannelDecision ()
{
}

bool
FilterOnChannelDecision::filter (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<map<string, bool> > channelDecisions;
  event.getByLabel (src_, channelDecisions);
  //map<string, bool> channelDecisionsProduct (*channelDecisions.product ());

  //return channelDecisionsProduct[channelName_];
  return channelDecisions->at (channelName_);
}

DEFINE_FWK_MODULE(FilterOnChannelDecision);
