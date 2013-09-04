#ifndef FILTER_ON_CHANNEL_DECISION

#define FILTER_ON_CHANNEL_DECISION

#include <iostream>
#include <map>
#include <string>

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

using namespace std;

class FilterOnChannelDecision : public edm::EDFilter
  {
    public:
      FilterOnChannelDecision (const edm::ParameterSet &);
      ~FilterOnChannelDecision ();

      bool filter (edm::Event &, const edm::EventSetup &);

    private:
      edm::InputTag src_;
      string channelName_;
  };

#endif
