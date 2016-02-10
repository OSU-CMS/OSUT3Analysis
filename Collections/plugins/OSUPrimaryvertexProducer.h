#ifndef PRIMARYVERTEX_PRODUCER
#define PRIMARYVERTEX_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Primaryvertex.h"

class OSUPrimaryvertexProducer : public edm::EDProducer
{
  public:
    OSUPrimaryvertexProducer (const edm::ParameterSet &);
    ~OSUPrimaryvertexProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::EDGetTokenT<vector<TYPE(primaryvertexs)> > token_;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    auto_ptr<vector<osu::Primaryvertex> > pl_;
};

#endif
