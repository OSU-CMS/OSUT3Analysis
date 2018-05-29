#include "OSUT3Analysis/Collections/plugins/OSUMetProducer.h"

#if IS_VALID(mets)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUMetProducer::OSUMetProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  pfCandidates_ (cfg.getParameter<edm::InputTag> ("pfCandidates")),
  firstEvent_ (true)
{
  collection_ = collections_.getParameter<edm::InputTag> ("mets");

  produces<vector<osu::Met> > (collection_.instance ());

  token_ = consumes<vector<TYPE(mets)> > (collection_);
  pfCandidatesToken_ = consumes<vector<pat::PackedCandidate> > (pfCandidates_);

  BadChCandFilterToken_ = consumes<bool>(cfg.getParameter<edm::InputTag>("BadChargedCandidateFilter"));
  BadPFMuonFilterToken_ = consumes<bool>(cfg.getParameter<edm::InputTag>("BadPFMuonFilter"));
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

  edm::Handle<bool> ifilterbadChCand;
  event.getByToken(BadChCandFilterToken_, ifilterbadChCand);

  edm::Handle<bool> ifilterbadPFMuon;
  event.getByToken(BadPFMuonFilterToken_, ifilterbadPFMuon);

  edm::Handle<vector<pat::PackedCandidate> > pfCandidates;
  event.getByToken (pfCandidatesToken_, pfCandidates);

  pl_ = unique_ptr<vector<osu::Met> > (new vector<osu::Met> ());
  for (const auto &object : *collection)
    {
      pl_->emplace_back (object, pfCandidates);
      if (ifilterbadChCand.isValid ())
        pl_->back ().setBadChargedCandidateFilter (*ifilterbadChCand);
      else if (firstEvent_)
        edm::LogWarning ("OSUMetProducer") << "Not applying \"Bad Charged Hadron Filter\"!";
      if (ifilterbadPFMuon.isValid ())
        pl_->back ().setBadPFMuonFilter (*ifilterbadPFMuon);
      else if (firstEvent_)
        edm::LogWarning ("OSUMetProducer") << "Not applying \"Bad PF Muon Filter\"!";
    }

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();

  firstEvent_ = false;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUMetProducer);

#endif
