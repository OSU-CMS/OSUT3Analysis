#include "OSUT3Analysis/Collections/plugins/OSUPhotonProducer.h"

#if IS_VALID(photons)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

OSUPhotonProducer::OSUPhotonProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections")),
  cfg_         (cfg),
  rho_         (cfg.getParameter<edm::InputTag>  ("rho"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("photons");

  produces<vector<osu::Photon> > (collection_.instance ());

  token_ = consumes<vector<TYPE(photons)> > (collection_);
  mcparticleToken_ = consumes<vector<osu::Mcparticle> > (collections_.getParameter<edm::InputTag> ("mcparticles"));
  rhoToken_ = consumes<double>(rho_);
}

OSUPhotonProducer::~OSUPhotonProducer ()
{
}

void
OSUPhotonProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<vector<TYPE(photons)> > collection;
  if (!event.getByToken (token_, collection))
    return;
  edm::Handle<vector<osu::Mcparticle> > particles;
  event.getByToken (mcparticleToken_, particles);

  edm::Handle<double> rho;

  pl_ = auto_ptr<vector<osu::Photon> > (new vector<osu::Photon> ());
  for (const auto &object : *collection)
    {
      pl_->emplace_back (object, particles, cfg_);
      osu::Photon &photon = pl_->back ();

      if(event.getByToken(rhoToken_, rho)) photon.set_rho((float)(*rho));

      float Aeff_neutralHadron = 0.;
      float Aeff_chargedHadron = 0.;
      float Aeff_photon = 0.;
      float Aeff_eta = abs(object.superCluster()->eta());

      // https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2#Selection_implementation_details

      if(Aeff_eta < 1.0) {
        Aeff_neutralHadron = 0.0599; // pm 0.001
        Aeff_photon = 0.1271; // pm 0.001
      }
      else if(Aeff_eta < 1.479) {
        Aeff_neutralHadron = 0.0819; // pm 0.001
        Aeff_photon = 0.1101; // pm 0.003
      }
      else if(Aeff_eta < 2.0) {
        Aeff_neutralHadron = 0.0696; // pm 0.001
        Aeff_photon = 0.0756; // pm 0.002
      }
      else if(Aeff_eta < 2.2) {
        Aeff_neutralHadron = 0.0360; // pm 0.001
        Aeff_photon = 0.1175; // pm 0.002
      }
      else if(Aeff_eta < 2.3) {
        Aeff_neutralHadron = 0.0360; // pm 0.002
        Aeff_photon = 0.1498; // pm 0.00001
      }
      else if(Aeff_eta < 2.4) {
        Aeff_neutralHadron = 0.0462; // pm 0.001
        Aeff_photon = 0.1857; // pm 0.005
      }
      else {
        Aeff_neutralHadron = 0.0656; // pm 0.005
        Aeff_photon = 0.2183; // pm 0.003
      }

      photon.set_AEff_neutralHadron(Aeff_neutralHadron);
      photon.set_Aeff_chargedHadron(Aeff_chargedHadron);
      photon.set_Aeff_photon(Aeff_photon);
    }

  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(OSUPhotonProducer);

#endif
