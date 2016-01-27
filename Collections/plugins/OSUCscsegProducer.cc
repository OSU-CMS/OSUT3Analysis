#include "OSUT3Analysis/Collections/plugins/OSUCscsegProducer.h"

#if IS_VALID(cscsegs)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUCscsegProducer::OSUCscsegProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("cscsegs");

  produces<vector<osu::Cscseg> > (collection_.instance ());
}

OSUCscsegProducer::~OSUCscsegProducer ()
{
}

void
OSUCscsegProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (cscsegs)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;

  pl_ = auto_ptr<vector<osu::Cscseg> > (new vector<osu::Cscseg> ());
  for (const auto &object : *collection)
    {
      const osu::Cscseg cscseg (object);
      pl_->push_back (cscseg);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUCscsegProducer);

#endif
