#include "OSUT3Analysis/Collections/plugins/OSUMuonProducer.h"

#if IS_VALID(muons)

#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD

#include "OSUT3Analysis/Collections/interface/Primaryvertex.h"

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUMuonProducer::OSUMuonProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  pfCandidate_ (cfg.getParameter<edm::InputTag> ("pfCandidate")),
  cfg_ (cfg)
{
  collection_         = collections_.getParameter<edm::InputTag> ("muons");
  collPrimaryvertexs_ = collections_.getParameter<edm::InputTag> ("primaryvertexs");

  produces<vector<osu::Muon> > (collection_.instance ());

  token_ = consumes<vector<TYPE(muons)> > (collection_);
  pfCandidateToken_ = consumes<vector<pat::PackedCandidate>>(pfCandidate_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  primaryvertexToken_ = consumes<vector<TYPE(primaryvertexs)> > (collPrimaryvertexs_);
}

OSUMuonProducer::~OSUMuonProducer ()
{
}

void
OSUMuonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<pat::PackedCandidate>> cands; 
  event.getByToken(pfCandidateToken_, cands);
  edm::Handle<vector<TYPE(muons)> > collection;
  edm::Handle<vector<TYPE(primaryvertexs)> > collPrimaryvertexs;
  if (!event.getByToken (token_, collection))
    return;
  if (!event.getByToken (primaryvertexToken_, collPrimaryvertexs)) {
    clog << "ERROR [OSUMuonProducer::produce]:  could not get collection: " << collPrimaryvertexs_ << endl;
    return;
  }
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

  pl_ = auto_ptr<vector<osu::Muon> > (new vector<osu::Muon> ());
  for (const auto &object : *collection)
    {
      osu::Muon muon (object, particles, cfg_);
      if (collPrimaryvertexs->size ())
        {
          const reco::Vertex &vtx = collPrimaryvertexs->at (0);
          muon.set_isTightMuonWRTVtx(muon.isTightMuon(vtx));
        }
      else
          muon.set_isTightMuonWRTVtx(false);
      double pfdBetaIsoCorr = 0;
      double chargedHadronPt = 0;
      double puPt = 0;
      int muonPVIndex = 0;
      if(cands.isValid())
        { 
          for (auto cand = cands->begin(); cand != cands->end(); cand++) 
            {
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
                  if((abs(cand->pdgId()) == 211 || abs(cand->pdgId()) == 321 || abs(cand->pdgId()) == 999211 || abs(cand->pdgId()) == 2212) && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) <= 0.4)
                    { 
                      pat::PackedCandidate thatPFCandidate = (*cand); 
                      int ivtx = cand->vertexRef().index();	
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
                 if((abs(cand->pdgId()) == 211 || abs(cand->pdgId()) == 321 || abs(cand->pdgId()) == 999211 || abs(cand->pdgId()) == 2212) && deltaR(object.eta(),object.phi(),cand->eta(),cand->phi()) <= 0.4)
                    {
                      pat::PackedCandidate thatPFCandidate = (*cand);
                      int ivtx = cand->vertexRef().index();
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
      pl_->push_back (muon);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
#elif DATA_FORMAT == AOD_CUSTOM
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
  pl_ = auto_ptr<vector<osu::Muon> > (new vector<osu::Muon> ());
  for (const auto &object : *collection)
    {
      const osu::Muon muon (object);
      pl_->push_back (muon);
    }
  event.put (pl_, collection_.instance ());
  pl_.reset ();
}
#endif

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUMuonProducer);

#endif
