#include "OSUT3Analysis/Collections/plugins/OSUTauProducer.h"

#if IS_VALID(taus)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUTauProducer::OSUTauProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("taus");

  produces<vector<osu::Tau> > (collection_.instance ());

  token_ = consumes<vector<TYPE(taus)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  metToken_ = consumes<vector<osu::Met> > (collections_.getParameter<edm::InputTag> ("mets"));
  triggersToken_ = consumes<edm::TriggerResults> (collections_.getParameter<edm::InputTag> ("triggers"));
  trigobjsToken_ = consumes<vector<pat::TriggerObjectStandAlone> > (collections_.getParameter<edm::InputTag> ("trigobjs"));
}

OSUTauProducer::~OSUTauProducer ()
{
}

void
OSUTauProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(taus)> > collection;
  if (!event.getByToken (token_, collection))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);
  edm::Handle<vector<osu::Met> > met;
  event.getByToken (metToken_, met);
  edm::Handle<edm::TriggerResults> triggers;
  event.getByToken (triggersToken_, triggers);
  edm::Handle<vector<pat::TriggerObjectStandAlone> > trigobjs;
  event.getByToken (trigobjsToken_, trigobjs);

  pl_ = unique_ptr<vector<osu::Tau> > (new vector<osu::Tau> ());
  for (const auto &object : *collection)
    {
      pl_->emplace_back (object, particles, cfg_, met->at (0));
      osu::Tau &tau = pl_->back ();

      if(trigobjs.isValid())
        tau.set_match_HLT_LooseIsoPFTau50_Trk30_eta2p1_v (anatools::isMatchedToTriggerObject (event, *triggers, object, *trigobjs, "hltSelectedPFTausTrackPt30AbsOrRelIsolation::HLT", "hltPFTau50TrackPt30LooseAbsOrRelIso"));
    }

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUTauProducer);

#endif
