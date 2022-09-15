#ifndef MET_PRODUCER
#define MET_PRODUCER

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Met.h"

class OSUMetProducer : public edm::stream::EDProducer<>
{
  public:
    OSUMetProducer (const edm::ParameterSet &);
    ~OSUMetProducer ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::InputTag      pfCandidates_;
    bool firstEvent_;
    edm::EDGetTokenT<vector<TYPE(mets)> > token_;
    edm::EDGetTokenT<vector<pat::PackedCandidate> > pfCandidatesToken_;
    edm::EDGetTokenT<bool> BadChCandFilterToken_;
    edm::EDGetTokenT<bool> BadPFMuonFilterToken_;
    edm::EDGetTokenT<bool> ecalBadCalibFilterUpdateToken_ ;
    ////////////////////////////////////////////////////////////////////////////

    // Payload for this EDFilter.
    unique_ptr<vector<osu::Met> > pl_;
};

#endif
