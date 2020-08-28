#include "OSUT3Analysis/Collections/plugins/OSUMuonProducer.h"

#if IS_VALID(muons)

#ifndef STOPPPED_PTLS

#include "OSUT3Analysis/Collections/interface/Primaryvertex.h"
#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUMuonProducer::OSUMuonProducer (const edm::ParameterSet &cfg) :
  collections_     (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_             (cfg),
  pfCandidate_     (cfg.getParameter<edm::InputTag>     ("pfCandidate")),
  rho_             (cfg.getParameter<edm::InputTag>     ("rho")),
  d0SmearingWidth_ (cfg.getParameter<double>            ("d0SmearingWidth")),
  genD0DR_         (cfg.getParameter<double>            ("genD0DR")),
  hltMatchingInfo_ (cfg.getParameter<vector<edm::ParameterSet> > ("hltMatchingInfo"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("muons");
  produces<vector<osu::Muon> > (collection_.instance ());

  // RNG for gaussian d0 smearing
  if (d0SmearingWidth_ >= 0) rng = new TRandom3(0);

  token_ = consumes<vector<TYPE(muons)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  prunedParticleToken_ = consumes<vector<reco::GenParticle> > (collections_.getParameter<edm::InputTag> ("hardInteractionMcparticles"));
  pfCandidateToken_ = consumes<vector<pat::PackedCandidate>>(pfCandidate_);
  rhoToken_ = consumes<double> (rho_);
  beamspotToken_ = consumes<TYPE(beamspots)> (collections_.getParameter<edm::InputTag> ("beamspots"));
  primaryvertexToken_ = consumes<vector<TYPE(primaryvertexs)> > (collections_.getParameter<edm::InputTag> ("primaryvertexs"));
  metToken_ = consumes<vector<osu::Met> > (collections_.getParameter<edm::InputTag> ("mets"));
  triggersToken_ = consumes<edm::TriggerResults> (collections_.getParameter<edm::InputTag> ("triggers"));
#if IS_VALID(trigobjs)
  trigobjsToken_ = consumes<vector<pat::TriggerObjectStandAlone> > (collections_.getParameter<edm::InputTag> ("trigobjs"));
#endif
}

OSUMuonProducer::~OSUMuonProducer ()
{
  if (d0SmearingWidth_ >= 0) delete rng;
}

void
OSUMuonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  using namespace edm;

  Handle<vector<TYPE(muons)> > collection;
  event.getByToken(token_, collection);

  Handle<vector<pat::PackedCandidate> > cands;
  event.getByToken(pfCandidateToken_, cands);

  Handle<double> rho;
  event.getByToken (rhoToken_, rho);

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

#if IS_VALID(trigobjs)
  Handle<vector<pat::TriggerObjectStandAlone> > trigobjs;
  event.getByToken (trigobjsToken_, trigobjs);
#endif

  pl_ = unique_ptr<vector<osu::Muon> > (new vector<osu::Muon> ());
  for (const auto &object : *collection)
    {
      pl_->emplace_back (object, particles, cfg_, met->at (0));
      osu::Muon &muon = pl_->back ();

      if(rho.isValid())
        muon.set_rho((float)(*rho));

#if DATA_FORMAT != AOD
      if (!vertices->empty ())
        {
          const reco::Vertex &vtx = vertices->at (0);
          muon.set_isTightMuonWRTVtx(muon.isTightMuon(vtx));
          muon.set_isSoftMuonWRTVtx(muon.isSoftMuon(vtx));
        }
      else
        {
          muon.set_isTightMuonWRTVtx(false);
          muon.set_isSoftMuonWRTVtx(false);
        }
#endif

#if IS_VALID(trigobjs)
      if(trigobjs.isValid())
        {
          for(unsigned iHLT = 0; iHLT != hltMatchingInfo_.size(); iHLT++) {
            string hltName       = hltMatchingInfo_.at(iHLT).getParameter<string>("name");
            string hltCollection = hltMatchingInfo_.at(iHLT).getParameter<string>("collection");
            string hltFilter     = hltMatchingInfo_.at(iHLT).getParameter<string>("filter");
            muon.set_hltMatch(hltName, anatools::isMatchedToTriggerObject(event, *triggers, object, *trigobjs, hltCollection, hltFilter));
          }
        }
#endif

      // generator D0 must be done with prunedGenParticles because vertex is only right in this collection, not right in packedGenParticles
      // this is a temporary solution; extending GenMatchable to use prunedGenParticles would be better
      if(prunedParticles.isValid() && beamspot.isValid())
	{
	  for (auto cand = prunedParticles->begin(); cand != prunedParticles->end(); cand++)
	    {
	      if (!(cand->status() == 1 && abs(cand->pdgId()) == 13 && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) < genD0DR_))
            continue;
	      double gen_vx = cand->vx();
	      double gen_vy = cand->vy();
	      double gen_px = cand->px();
	      double gen_py = cand->py();
	      double gen_d0 = ((-(cand->vx() - beamspot->x0())*cand->py() + (cand->vy() - beamspot->y0())*cand->px())/cand->pt());
	      muon.set_genVx(gen_vx);
	      muon.set_genVy(gen_vy);
	      muon.set_genPx(gen_px);
	      muon.set_genPy(gen_py);
	      muon.set_genD0(gen_d0);
           break;
	    }
	}

      // produce random d0 value to use in d0 smearing
      double d0SmearingVal = 0.0;
      if (!event.isRealData() && d0SmearingWidth_ >= 0) {
        d0SmearingVal = rng->Gaus(0, d0SmearingWidth_);
      }
      muon.set_d0SmearingVal(d0SmearingVal);

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
