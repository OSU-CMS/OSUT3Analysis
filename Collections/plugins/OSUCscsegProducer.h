#ifndef CSCSEG_PRODUCER
#define CSCSEG_PRODUCER

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Cscseg.h"

class OSUCscsegProducer : public edm::stream::EDProducer<>
{
  public:
    OSUCscsegProducer (const edm::ParameterSet &);
    ~OSUCscsegProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::EDGetTokenT<vector<TYPE(cscsegs)> > token_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    unique_ptr<vector<osu::Cscseg> > pl_;
};

#endif
