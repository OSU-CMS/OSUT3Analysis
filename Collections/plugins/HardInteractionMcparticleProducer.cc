#include "OSUT3Analysis/Collections/plugins/HardInteractionMcparticleProducer.h"

#if IS_VALID(hardInteractionMcparticles)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

HardInteractionMcparticleProducer::HardInteractionMcparticleProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("hardInteractionMcparticles");

  produces<vector<osu::HardInteractionMcparticle> > (collection_.instance ());

  token_ = consumes<vector<TYPE(hardInteractionMcparticles)> > (collection_);
}

HardInteractionMcparticleProducer::~HardInteractionMcparticleProducer ()
{
}

void
HardInteractionMcparticleProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(hardInteractionMcparticles)> > collection;
  if (!event.getByToken (token_, collection))
    return;

  pl_ = unique_ptr<vector<osu::HardInteractionMcparticle> > (new vector<osu::HardInteractionMcparticle> ());
  for (const auto &object : *collection){
    pl_->emplace_back (object);
    osu::HardInteractionMcparticle &hiMcpart = pl_->back();

    const reco::Candidate *uniqueMo = uniqueMother(hiMcpart);
    if (uniqueMo) hiMcpart.set_uniqueMotherPdgId(uniqueMo->pdgId());
  }

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

const reco::Candidate *
HardInteractionMcparticleProducer::uniqueMother(const TYPE(hardInteractionMcparticles) &p) const {
  const reco::Candidate *mo = &p;
  std::unordered_set<const reco::Candidate *> dupCheck;
  while (mo && mo->pdgId() == p.pdgId()) {
    dupCheck.insert(mo);
    mo = mo->mother();
    if (dupCheck.count(mo))
      return nullptr;
  }
  return mo;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(HardInteractionMcparticleProducer);

#endif
