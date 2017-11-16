#include "OSUT3Analysis/Collections/plugins/OSUMuonProducer.h"

#if IS_VALID(muons)

#ifndef STOPPPED_PTLS

#include "OSUT3Analysis/Collections/interface/Primaryvertex.h"
#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUMuonProducer::OSUMuonProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg),
  pfCandidate_ (cfg.getParameter<edm::InputTag> ("pfCandidate"))


{
  collection_ = collections_.getParameter<edm::InputTag> ("muons");
  produces<vector<osu::Muon> > (collection_.instance ());

  token_ = consumes<vector<TYPE(muons)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  prunedParticleToken_ = consumes<vector<reco::GenParticle> > (collections_.getParameter<edm::InputTag> ("hardInteractionMcparticles"));
  pfCandidateToken_ = consumes<vector<pat::PackedCandidate>>(pfCandidate_);
  beamspotToken_ = consumes<TYPE(beamspots)> (collections_.getParameter<edm::InputTag> ("beamspots"));
  primaryvertexToken_ = consumes<vector<TYPE(primaryvertexs)> > (collections_.getParameter<edm::InputTag> ("primaryvertexs"));
  metToken_ = consumes<vector<osu::Met> > (collections_.getParameter<edm::InputTag> ("mets"));
  triggersToken_ = consumes<edm::TriggerResults> (collections_.getParameter<edm::InputTag> ("triggers"));
  trigobjsToken_ = consumes<vector<pat::TriggerObjectStandAlone> > (collections_.getParameter<edm::InputTag> ("trigobjs"));
}

OSUMuonProducer::~OSUMuonProducer ()
{
}

void
OSUMuonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  using namespace edm;

  Handle<vector<TYPE(muons)> > collection;
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

  Handle<edm::TriggerResults> triggers;
  event.getByToken (triggersToken_, triggers);

  Handle<vector<pat::TriggerObjectStandAlone> > trigobjs;
  event.getByToken (trigobjsToken_, trigobjs);

  pl_ = unique_ptr<vector<osu::Muon> > (new vector<osu::Muon> ());
  for (const auto &object : *collection)
    {
      pl_->emplace_back (object, particles, cfg_, met->at (0));
      osu::Muon &muon = pl_->back ();

      if (vertices->size ())
        {
          const reco::Vertex &vtx = vertices->at (0);
          muon.set_isTightMuonWRTVtx(muon.isTightMuon(vtx));
        }
      else
          muon.set_isTightMuonWRTVtx(false);

      if(trigobjs.isValid())
        {
          muon.set_match_HLT_IsoMu24_v (anatools::isMatchedToTriggerObject (event, *triggers, object, *trigobjs, "hltL3MuonCandidates::HLT", "hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09"));
          muon.set_match_HLT_IsoTkMu24_v (anatools::isMatchedToTriggerObject (event, *triggers, object, *trigobjs, "hltHighPtTkMuonCands::HLT", "hltL3fL1sMu22L1f0Tkf24QL3trkIsoFiltered0p09"));
        }

      if(prunedParticles.isValid() && beamspot.isValid())
        {
          for (auto cand = prunedParticles->begin(); cand != prunedParticles->end(); cand++)
            {
              if (!(abs(cand->pdgId()) == 13 && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) < 0.001))
                continue;
	      double gen_d0 = ((-(cand->vx() - beamspot->x0())*cand->py() + (cand->vy() - beamspot->y0())*cand->px())/cand->pt());
	      muon.set_genD0(gen_d0);
	    }
	}
      double d0 = object.dB(pat::Muon::BS2D);
      double err = object.edB(pat::Muon::BS2D);
      muon.set_d0(d0);
      muon.set_d0Sig(d0/err);


      double pfdBetaIsoCorr = 0;
      double chargedHadronPt = 0;
      double puPt = 0;
      int muonPVIndex = 0;
      if(cands.isValid())
        {
          for (auto cand = cands->begin(); cand != cands->end(); cand++)
            {
              // ignore candidates with invalid vertex references, since
              // vertexRef() and fromPV() do not work in this case
              if (cand->vertexRef().isNull() || !cand->vertexRef().isAvailable())
                continue;
              if (!(abs(cand->pdgId()) == 13 && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) < 0.001))
                continue;
              else
                {
                  muonPVIndex = cand->vertexRef().index();
                  break;
                }
            }
          if(muonPVIndex == 0)
           {
             for (auto cand = cands->begin(); cand != cands->end(); cand++)
                {
                  // ignore candidates with invalid vertex references, since
                  // vertexRef() and fromPV() do not work in this case
                  if (cand->vertexRef().isNull() || !cand->vertexRef().isAvailable())
                    continue;
                  if((abs(cand->pdgId()) == 211 || abs(cand->pdgId()) == 321 || abs(cand->pdgId()) == 999211 || abs(cand->pdgId()) == 2212) && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) <= 0.4)
                    {
                      int ivtx = (cand->vertexRef().isNonnull() && cand->vertexRef().isAvailable() ? cand->vertexRef().index() : 0);
                      if(ivtx == muonPVIndex || ivtx == -1)
                        {
                          if(deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) > 0.0001 && cand->fromPV() >= 2)
                            chargedHadronPt = cand->pt() + chargedHadronPt;
                        }
                      else if(cand->pt() >= 0.5 && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) > 0.01)
                        puPt = cand->pt() + puPt;
                    }
                }
             pfdBetaIsoCorr = (chargedHadronPt + max(0.0,object.pfIsolationR04().sumNeutralHadronEt + object.pfIsolationR04().sumPhotonEt - 0.5*puPt))/object.pt();
           }
          else
            {
             for (auto cand = cands->begin(); cand != cands->end(); cand++)
               {
                  // ignore candidates with invalid vertex references, since
                  // vertexRef() and fromPV() do not work in this case
                  if (cand->vertexRef().isNull() || !cand->vertexRef().isAvailable())
                   continue;
                 if((abs(cand->pdgId()) == 211 || abs(cand->pdgId()) == 321 || abs(cand->pdgId()) == 999211 || abs(cand->pdgId()) == 2212) && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) <= 0.4)
                    {
                      int ivtx = (cand->vertexRef().isNonnull() && cand->vertexRef().isAvailable() ? cand->vertexRef().index() : 0);
                      if(ivtx == muonPVIndex || ivtx == -1)
                        {
                          if(deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) > 0.0001)
                            chargedHadronPt = cand->pt() + chargedHadronPt;
                        }
                      else if(cand->pt() >= 0.5 && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) > 0.01)
                        puPt = cand->pt() + puPt;
                    }
                }
             pfdBetaIsoCorr = (chargedHadronPt + max(0.0,object.pfIsolationR04().sumNeutralHadronEt + object.pfIsolationR04().sumPhotonEt - 0.5*puPt))/object.pt();
            }
         }
      muon.set_pfdBetaIsoCorr(pfdBetaIsoCorr);
      muon.set_sumChargedHadronPtCorr(chargedHadronPt);
      muon.set_sumPUPtCorr(puPt);
      muon.set_muonPVIndex(muonPVIndex);

      if (vertices->size ())
        {
          const reco::Vertex &vtx = vertices->at (muonPVIndex);
	  muon.set_dz(object.muonBestTrack()->dz(vtx.position()));
	}

    }

  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

#else // STOPPPED_PTLS

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUMuonProducer::OSUMuonProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_ (cfg)
{
  collection_ = collections_.getParameter<edm::InputTag> ("muons");

  produces<vector<osu::Muon> > (collection_.instance ());
}

OSUMuonProducer::~OSUMuonProducer ()
{
}

void
OSUMuonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE (muons)> > collection;
  if (!event.getByToken (token_, collection))
    return;
  pl_ = unique_ptr<vector<osu::Muon> > (new vector<osu::Muon> ());
  for (const auto &object : *collection)
    pl_->emplace_back (object);
  event.put (std::move (pl_), collection_.instance ());
  pl_.reset ();
}

#endif // STOPPPED_PTLS

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUMuonProducer);

#endif // IS_VALID(muons)
