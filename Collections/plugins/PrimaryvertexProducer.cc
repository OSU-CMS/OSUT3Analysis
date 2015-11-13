#include "OSUT3Analysis/Collections/plugins/PrimaryvertexProducer.h"

#if IS_VALID(primaryvertexs)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

PrimaryvertexProducer::PrimaryvertexProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("primaryvertexs");

  produces<vector<osu::Primaryvertex> > (collection_.instance ());
}

PrimaryvertexProducer::~PrimaryvertexProducer ()
{
}

void
PrimaryvertexProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (primaryvertexs)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;

  pl_ = auto_ptr<vector<osu::Primaryvertex> > (new vector<osu::Primaryvertex> ());
  for (const auto &object : *collection)
    {
      const osu::Primaryvertex primaryvertex (object);
      pl_->push_back (primaryvertex);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PrimaryvertexProducer);

#endif
