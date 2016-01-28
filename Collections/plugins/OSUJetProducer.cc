#include "OSUT3Analysis/Collections/plugins/OSUJetProducer.h"

#if IS_VALID(jets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUJetProducer::OSUJetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("jets");

  produces<vector<osu::Jet> > (collection_.instance ());
}

OSUJetProducer::~OSUJetProducer ()
{
}

void
OSUJetProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (jets)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
  edm::Handle<vector<osu::Mcparticle> > particles;
  anatools::getCollection (edm::InputTag ("", ""), particles, event);
#endif

  pl_ = auto_ptr<vector<osu::Jet> > (new vector<osu::Jet> ());
  for (const auto &object : *collection)
    {
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD
      osu::Jet jet (object, particles, cfg_);
#elif DATA_FORMAT == AOD_CUSTOM
      const osu::Jet jet (object);
#endif
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM
      jet.set_pfCombinedInclusiveSecondaryVertexV2BJetTags(jet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
      jet.set_pfCombinedSecondaryVertexV2BJetTags(jet.bDiscriminator("pfCombinedSecondaryVertexV2BJetTags")); 
#endif     
 pl_->push_back (jet);
    }
  event.put (pl_, collection_.instance ());
  pl_.reset ();
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUJetProducer);

#endif
