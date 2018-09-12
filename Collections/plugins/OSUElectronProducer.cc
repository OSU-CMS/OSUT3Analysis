#include "OSUT3Analysis/Collections/plugins/OSUElectronProducer.h"

#if IS_VALID(electrons)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUElectronProducer::OSUElectronProducer (const edm::ParameterSet &cfg) :
  collections_     (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_             (cfg),
  pfCandidate_     (cfg.getParameter<edm::InputTag>     ("pfCandidate")),
  conversions_     (cfg.getParameter<edm::InputTag>     ("conversions")),
  rho_             (cfg.getParameter<edm::InputTag>     ("rho")),
  vidVetoIdMap_    (cfg.getParameter<edm::InputTag>     ("vidVetoIdMap")),
  vidLooseIdMap_   (cfg.getParameter<edm::InputTag>     ("vidLooseIdMap")),
  vidMediumIdMap_  (cfg.getParameter<edm::InputTag>     ("vidMediumIdMap")),
  vidTightIdMap_   (cfg.getParameter<edm::InputTag>     ("vidTightIdMap")),
  effectiveAreas_  ((cfg.getParameter<edm::FileInPath>  ("effAreasPayload")).fullPath()),
  d0SmearingWidth_ (cfg.getParameter<double>            ("d0SmearingWidth"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("electrons");
  produces<vector<osu::Electron> > (collection_.instance ());

  // RNG for gaussian d0 smearing
  rng = new TRandom3(0);

  token_ = consumes<edm::View<TYPE(electrons)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  prunedParticleToken_ = consumes<vector<reco::GenParticle> > (collections_.getParameter<edm::InputTag> ("hardInteractionMcparticles"));
  pfCandidateToken_ = consumes<vector<pat::PackedCandidate>>(pfCandidate_);
  beamspotToken_ = consumes<TYPE(beamspots)> (collections_.getParameter<edm::InputTag> ("beamspots"));
  primaryvertexToken_ = consumes<vector<TYPE(primaryvertexs)> > (collections_.getParameter<edm::InputTag> ("primaryvertexs"));
  metToken_ = consumes<vector<osu::Met> > (collections_.getParameter<edm::InputTag> ("mets"));
  conversionsToken_ = consumes<vector<reco::Conversion> > (conversions_);
  rhoToken_ = consumes<double> (rho_);
  triggersToken_ = consumes<edm::TriggerResults> (collections_.getParameter<edm::InputTag> ("triggers"));
  trigobjsToken_ = consumes<vector<pat::TriggerObjectStandAlone> > (collections_.getParameter<edm::InputTag> ("trigobjs"));

  vidVetoIdMapToken_   = consumes<edm::ValueMap<bool> > (vidVetoIdMap_);
  vidLooseIdMapToken_  = consumes<edm::ValueMap<bool> > (vidLooseIdMap_);
  vidMediumIdMapToken_ = consumes<edm::ValueMap<bool> > (vidMediumIdMap_);
  vidTightIdMapToken_  = consumes<edm::ValueMap<bool> > (vidTightIdMap_);
}

OSUElectronProducer::~OSUElectronProducer ()
{
  delete rng;
}

void
OSUElectronProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
#if DATA_FORMAT_FROM_MINIAOD
  using namespace edm;
  using namespace reco;

  Handle<edm::View<TYPE(electrons)> > collection;
  event.getByToken(token_, collection);

  Handle<vector<pat::PackedCandidate> > cands;
  event.getByToken(pfCandidateToken_, cands);

  Handle<vector<TYPE(primaryvertexs)> > vertices;
  event.getByToken (primaryvertexToken_, vertices);

  Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

  Handle<vector<reco::GenParticle> > prunedParticles;
  event.getByToken (prunedParticleToken_, prunedParticles);

  Handle<TYPE(beamspots)> beamspot;
  event.getByToken (beamspotToken_, beamspot);

  Handle<vector<osu::Met> > met;
  event.getByToken (metToken_, met);

  Handle<vector<reco::Conversion> > conversions;
  event.getByToken (conversionsToken_, conversions);

  Handle<double> rho;
  event.getByToken (rhoToken_, rho);

  Handle<edm::TriggerResults> triggers;
  event.getByToken (triggersToken_, triggers);

  Handle<vector<pat::TriggerObjectStandAlone> > trigobjs;
  event.getByToken (trigobjsToken_, trigobjs);

  edm::Handle<edm::ValueMap<bool> > vidVetoIdMap;
  event.getByToken(vidVetoIdMapToken_, vidVetoIdMap);

  edm::Handle<edm::ValueMap<bool> > vidLooseIdMap;
  event.getByToken(vidLooseIdMapToken_, vidLooseIdMap);

  edm::Handle<edm::ValueMap<bool> > vidMediumIdMap;
  event.getByToken(vidMediumIdMapToken_, vidMediumIdMap);

  edm::Handle<edm::ValueMap<bool> > vidTightIdMap;
  event.getByToken(vidTightIdMapToken_, vidTightIdMap);

  pl_ = unique_ptr<vector<osu::Electron> > (new vector<osu::Electron> ());

  unsigned iEle = -1;

  for (const auto &object : *collection)
    {
      ++iEle;

      pl_->emplace_back (object, particles, cfg_, met->at (0));
      osu::Electron &electron = pl_->back ();

      if(rho.isValid())
        electron.set_rho((float)(*rho));

      if(beamspot.isValid() && conversions.isValid() && vertices.isValid() && !vertices->empty ())
        electron.set_passesTightID_noIsolation (*beamspot, vertices->at (0), conversions);

      if(vidVetoIdMap.isValid())
        electron.set_passesVID_vetoID ( (*vidVetoIdMap)[(*collection).refAt(iEle)] );

      if(vidLooseIdMap.isValid())
        electron.set_passesVID_looseID ( (*vidLooseIdMap)[(*collection).refAt(iEle)] );

      if(vidMediumIdMap.isValid())
        electron.set_passesVID_mediumID ( (*vidMediumIdMap)[(*collection).refAt(iEle)] );

      if(vidTightIdMap.isValid())
        electron.set_passesVID_tightID ( (*vidTightIdMap)[(*collection).refAt(iEle)] );

      if(trigobjs.isValid())
        {
          electron.set_match_HLT_Ele25_eta2p1_WPTight_Gsf_v (anatools::isMatchedToTriggerObject (event, *triggers, object, *trigobjs, "hltEgammaCandidates::HLT", "hltEle25erWPTightGsfTrackIsoFilter"));
          electron.set_match_HLT_Ele22_eta2p1_WPLoose_Gsf_v (anatools::isMatchedToTriggerObject (event, *triggers, object, *trigobjs, "hltEgammaCandidates::HLT", "hltSingleEle22WPLooseGsfTrackIsoFilter"));
          electron.set_match_HLT_Ele35_WPTight_Gsf_v        (anatools::isMatchedToTriggerObject (event, *triggers, object, *trigobjs, "hltEgammaCandidates::HLT", "hltEle35noerWPTightGsfTrackIsoFilter"));
        }

      float effectiveArea = 0;
      effectiveArea = effectiveAreas_.getEffectiveArea(fabs(object.superCluster()->eta()));
      electron.set_AEff(effectiveArea);

      //generator D0 must be done with prunedGenParticles because vertex is only right in this collection, not right in packedGenParticles
      if(prunedParticles.isValid() && beamspot.isValid())
	{
	  for (auto cand = prunedParticles->begin(); cand != prunedParticles->end(); cand++)
	    {
	      if (!(abs(cand->pdgId()) == 11 && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) < 0.1))
		continue;
	      double gen_d0 = ((-(cand->vx() - beamspot->x0())*cand->py() + (cand->vy() - beamspot->y0())*cand->px())/cand->pt());
	      electron.set_genD0(gen_d0);
	    }
	}

      // produce random d0 value to use in d0 smearing
      double d0SmearingVal = 0.0;
      if (!event.isRealData() && d0SmearingWidth_ >= 0) {
        d0SmearingVal = rng->Gaus(0, d0SmearingWidth_);
      }
      electron.set_d0SmearingVal(d0SmearingVal);

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
  edm::Handle<edm::View<TYPE(electrons)> > collection;
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
