#include "OSUT3Analysis/Collections/plugins/OSUElectronProducer.h"

#if IS_VALID(electrons)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUElectronProducer::OSUElectronProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg),
  pfCandidate_ (cfg.getParameter<edm::InputTag> ("pfCandidate")),
  conversions_ (cfg.getParameter<edm::InputTag> ("conversions")),
  rho_         (cfg.getParameter<edm::InputTag> ("rho"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("electrons");
  produces<vector<osu::Electron> > (collection_.instance ());

  token_ = consumes<vector<TYPE(electrons)> > (collection_);
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
}

OSUElectronProducer::~OSUElectronProducer ()
{
}

void
OSUElectronProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
#if DATA_FORMAT_FROM_MINIAOD
  using namespace edm;
  using namespace reco;

  Handle<vector<TYPE(electrons)> > collection;
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

  pl_ = unique_ptr<vector<osu::Electron> > (new vector<osu::Electron> ());
  for (const auto &object : *collection)
    {
      pl_->emplace_back (object, particles, cfg_, met->at (0));
      osu::Electron &electron = pl_->back ();

      if(rho.isValid())
        electron.set_rho((float)(*rho));
      if(beamspot.isValid() && conversions.isValid() && vertices.isValid() && !vertices->empty ())
        electron.set_passesTightID_noIsolation (*beamspot, vertices->at (0), conversions);
      if(trigobjs.isValid())
        {
          electron.set_match_HLT_Ele25_eta2p1_WPTight_Gsf_v (anatools::isMatchedToTriggerObject (event, *triggers, object, *trigobjs, "hltEgammaCandidates::HLT", "hltEle25erWPTightGsfTrackIsoFilter"));
          electron.set_match_HLT_Ele22_eta2p1_WPLoose_Gsf_v (anatools::isMatchedToTriggerObject (event, *triggers, object, *trigobjs, "hltEgammaCandidates::HLT", "hltSingleEle22WPLooseGsfTrackIsoFilter"));
        }

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

      if(prunedParticles.isValid() && beamspot.isValid())
        {
          for (auto cand = prunedParticles->begin(); cand != prunedParticles->end(); cand++)
            {
              if (!(abs(cand->pdgId()) == 11 && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) < 0.001))
                continue;
              double gen_d0 = ((-(cand->vx() - beamspot->x0())*cand->py() + (cand->vy() - beamspot->y0())*cand->px())/cand->pt());
              electron.set_genD0(gen_d0);
              electron.set_genPt(cand->pt());
              electron.set_genEta(cand->eta());
              electron.set_genPhi(cand->phi());
            }
        }
      ///////////////////////////////////////////////////////////
      // THIS APPEARS TO BE BROKEN -
      // IT LOOKS LIKE IT JUST RETURNS D0 WRT THE CMS ORIGIN
      // WILL USE MANUAL CALCULATION INSTEAD
      // double d0 = object.dB(pat::Electron::BS2D);
      // double err = object.edB(pat::Electron::BS2D);
      ///////////////////////////////////////////////////////////

      double d0 = ((-(object.vx() - beamspot->x0())*object.py() + (object.vy() - beamspot->y0())*object.px())/object.pt());
      double err = hypot(object.gsfTrack()->d0Error(), hypot(beamspot->x0Error(), beamspot->y0Error()));
      electron.set_d0(d0);
      electron.set_d0Sig(d0/err);


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


      if (!vertices->empty ())
        {
          const reco::Vertex &vtx = vertices->at (electronPVIndex);
          electron.set_dz(object.gsfTrack()->dz(vtx.position()));
        }
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
