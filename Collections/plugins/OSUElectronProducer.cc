#include "OSUT3Analysis/Collections/plugins/OSUElectronProducer.h"

#if IS_VALID(electrons)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUElectronProducer::OSUElectronProducer (const edm::ParameterSet &cfg) :
  collections_  (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg),
  beamSpot_     (cfg.getParameter<edm::InputTag>  ("beamSpot")),
  conversions_  (cfg.getParameter<edm::InputTag>  ("conversions")),
  pfCandidate_  (cfg.getParameter<edm::InputTag>  ("pfCandidate")),
  rho_          (cfg.getParameter<edm::InputTag>  ("rho")),
  vertices_     (cfg.getParameter<edm::InputTag>  ("vertices"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("electrons");
  produces<vector<osu::Electron> > (collection_.instance ());

  token_ = consumes<vector<TYPE(electrons)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  beamSpotToken_ = consumes<TYPE(beamspots)> (beamSpot_);
  conversionsToken_ = consumes<vector<reco::Conversion> > (conversions_);
  pfCandidateToken_ = consumes<vector<pat::PackedCandidate>>(pfCandidate_);
  rhoToken_ = consumes<double> (rho_);
  verticesToken_ = consumes<vector<TYPE(primaryvertexs)> > (vertices_);
  metToken_ = consumes<vector<osu::Met> > (collections_.getParameter<edm::InputTag> ("mets"));
}

OSUElectronProducer::~OSUElectronProducer ()
{
}

void
OSUElectronProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM
  using namespace edm;
  using namespace reco;

  Handle<vector<pat::PackedCandidate>> cands;
  event.getByToken(pfCandidateToken_, cands);
  edm::Handle<vector<TYPE (electrons)> > collection;
  edm::Handle<TYPE(beamspots)> beamSpot;
  edm::Handle<vector<reco::Conversion> > conversions;
  edm::Handle<double> rho;
  edm::Handle<vector<TYPE(primaryvertexs)> > vertices;

  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);
  edm::Handle<vector<osu::Met> > met;
  event.getByToken (metToken_, met);
  if (!event.getByToken (token_, collection))
    return;
  pl_ = unique_ptr<vector<osu::Electron> > (new vector<osu::Electron> ());
  for (const auto &object : *collection)
    {
      pl_->emplace_back (object, particles, cfg_, met->at (0));
      osu::Electron &electron = pl_->back ();
      if(event.getByToken (rhoToken_, rho))
        electron.set_rho((float)(*rho));
      if(event.getByToken (beamSpotToken_, beamSpot) && event.getByToken (conversionsToken_, conversions) && event.getByToken (verticesToken_, vertices) && vertices->size ())
        electron.set_passesTightID_noIsolation (*beamSpot, vertices->at (0), conversions);

      float effectiveArea = 0;
      // electron effective areas from https://indico.cern.ch/event/369239/contribution/4/attachments/1134761/1623262/talk_effective_areas_25ns.pdf
      // (see slide 12)
      if(abs(object.superCluster()->eta()) >= 0.0000 && abs(object.superCluster()->eta()) < 1.0000)
        effectiveArea = 0.1752;
      if(abs(object.superCluster()->eta()) >= 1.0000 && abs(object.superCluster()->eta()) < 1.4790)
        effectiveArea = 0.1862;
      if(abs(object.superCluster()->eta()) >= 1.4790 && abs(object.superCluster()->eta()) < 2.0000)
        effectiveArea = 0.1411;
      if(abs(object.superCluster()->eta()) >= 2.0000 && abs(object.superCluster()->eta()) < 2.2000)
        effectiveArea = 0.1534;
      if(abs(object.superCluster()->eta()) >= 2.2000 && abs(object.superCluster()->eta()) < 2.3000)
        effectiveArea = 0.1903;
      if(abs(object.superCluster()->eta()) >= 2.3000 && abs(object.superCluster()->eta()) < 2.4000)
        effectiveArea = 0.2243;
      if(abs(object.superCluster()->eta()) >= 2.4000 && abs(object.superCluster()->eta()) < 5.0000)
        effectiveArea = 0.2687;
      electron.set_AEff(effectiveArea);
      double pfdRhoIsoCorr = 0;
      double chargedHadronPt = 0;
      double puPt = 0;
      int electronPVIndex = 0;
      if(cands.isValid())
        {
          for (auto cand = cands->begin(); cand != cands->end(); cand++)
            {
              if (!(abs(cand->pdgId()) == 11 && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) < 0.001))
                continue;
              else
                {
                  electronPVIndex = cand->vertexRef().index();
                  break;
                }
            }
          if(electronPVIndex == 0)
            {
              for (auto cand = cands->begin(); cand != cands->end(); cand++)
                {
                  if((abs(cand->pdgId()) == 211 || abs(cand->pdgId()) == 321 || abs(cand->pdgId()) == 999211 || abs(cand->pdgId()) == 2212) && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) <= 0.3)
                    {
                      pat::PackedCandidate thatPFCandidate = (*cand);
                      int ivtx = cand->vertexRef().index();
                      if(ivtx == electronPVIndex || ivtx == -1)
                        {
                          if(deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) > 0.0001 && cand->fromPV() >= 2)
                            chargedHadronPt = cand->pt() + chargedHadronPt;
                        }
                      else if(cand->pt() >= 0.5 && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) > 0.01)
                        puPt = cand->pt() + puPt;
                    }
                }
          pfdRhoIsoCorr = (chargedHadronPt + max(0.0,object.pfIsolationVariables().sumNeutralHadronEt + object.pfIsolationVariables().sumPhotonEt - double(effectiveArea *(float)(*rho))))/object.pt();
            }
          else
            {
              for (auto cand = cands->begin(); cand != cands->end(); cand++)
                {
                  if((abs(cand->pdgId()) == 211 || abs(cand->pdgId()) == 321 || abs(cand->pdgId()) == 999211 || abs(cand->pdgId()) == 2212) && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) <= 0.3)
                    {
                      pat::PackedCandidate thatPFCandidate = (*cand);
                      int ivtx = cand->vertexRef().index();
                      if(ivtx == electronPVIndex || ivtx == -1)
                        {
                          if(deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) > 0.0001)
                            chargedHadronPt = cand->pt() + chargedHadronPt;
                        }
                      else if(cand->pt() >= 0.5 && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) > 0.01)
                        puPt = cand->pt() + puPt;
                    }
                }
          pfdRhoIsoCorr = (chargedHadronPt + max(0.0,object.pfIsolationVariables().sumNeutralHadronEt + object.pfIsolationVariables().sumPhotonEt - double(effectiveArea *(float)(*rho))))/object.pt();
            }
       }
      electron.set_pfdRhoIsoCorr(pfdRhoIsoCorr);
      electron.set_sumChargedHadronPtCorr(chargedHadronPt);
      electron.set_sumPUPtCorr(puPt);
      electron.set_electronPVIndex(electronPVIndex);
    }
  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
#else
  edm::Handle<vector<TYPE(electrons)> > collection;
  if (!event.getByToken (token_, collection))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

  pl_ = unique_ptr<vector<osu::Electron> > (new vector<osu::Electron> ());
  for (const auto &object : *collection)
    pl_->emplace_back (object, particles, cfg_);

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
#endif
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUElectronProducer);

#endif
