#include "OSUT3Analysis/Collections/plugins/OSUMetProducer.h"

#if IS_VALID(mets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUMetProducer::OSUMetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  pfCandidates_ (cfg.getParameter<edm::InputTag> ("pfCandidates"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("mets");

  produces<vector<osu::Met> > (collection_.instance ());

  token_ = consumes<vector<TYPE(mets)> > (collection_);
  pfCandidatesToken_ = consumes<vector<pat::PackedCandidate> > (pfCandidates_);
}

OSUMetProducer::~OSUMetProducer ()
{
}

void
OSUMetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(mets)> > collection;
  if (!event.getByToken (token_, collection))
    return;

  edm::Handle<vector<pat::PackedCandidate> > pfCandidates;
  event.getByToken (pfCandidatesToken_, pfCandidates);

  pl_ = auto_ptr<vector<osu::Met> > (new vector<osu::Met> ());
  for (const auto &object : *collection)
    {
      const osu::Met met (object, pfCandidates);
      pl_->push_back (met);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUMetProducer);

#endif
