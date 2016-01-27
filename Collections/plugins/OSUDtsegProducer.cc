#include "OSUT3Analysis/Collections/plugins/OSUDtsegProducer.h"

#if IS_VALID(dtsegs)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUDtsegProducer::OSUDtsegProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  std::cout << "Begining " << std::endl;
  collection_ = collections_.getParameter<edm::InputTag> ("dtsegs");

  std::cout << "Middle" << std::endl; 
  produces<vector<osu::Dtseg> > (collection_.instance ());
  std::cout << collection_.instance() << std::endl;
  std::cout << "end of constructor" << std::endl;
}

OSUDtsegProducer::~OSUDtsegProducer ()
{
}

void
OSUDtsegProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  std::cout << "before we do any thing" << std::endl;
  edm::Handle<vector<TYPE (dtsegs)> > collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;

  pl_ = auto_ptr<vector<osu::Dtseg> > (new vector<osu::Dtseg> ());
  std::cout << "before push back" << std::endl;
  for (const auto &object : *collection)
    {
      const osu::Dtseg dtseg (object);
      pl_->push_back (dtseg);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUDtsegProducer);

#endif
