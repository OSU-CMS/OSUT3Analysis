#include "OSUT3Analysis/Collections/plugins/McparticleProducer.h"

#if IS_VALID(mcparticles)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

McparticleProducer::McparticleProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("mcparticles");

  produces<vector<osu::Mcparticle> > (collection_.instance ());

  token_ = consumes<vector<TYPE(mcparticles)> > (collection_);
}

McparticleProducer::~McparticleProducer ()
{
}

void
McparticleProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(mcparticles)> > collection;
  if (!event.getByToken (token_, collection))
    return;

  pl_ = unique_ptr<vector<osu::Mcparticle> > (new vector<osu::Mcparticle> ());
  for (const auto &object : *collection) {
    pl_->emplace_back (object);
    osu::Mcparticle &mcpart = pl_->back();

    const reco::Candidate *uniqueMo = uniqueMother(mcpart);
    if (uniqueMo) mcpart.set_uniqueMotherPdgId(uniqueMo->pdgId());
  }

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

const reco::Candidate *
McparticleProducer::uniqueMother(const TYPE(mcparticles) &p) const {
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
DEFINE_FWK_MODULE(McparticleProducer);

#endif
