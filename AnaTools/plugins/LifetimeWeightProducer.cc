#include <vector>
#include <unordered_set>
#include <sstream>

#include "OSUT3Analysis/AnaTools/plugins/LifetimeWeightProducer.h"

LifetimeWeightProducer::LifetimeWeightProducer (const edm::ParameterSet &cfg) :
  EventVariableProducer(cfg),
  srcCTau_ (cfg.getParameter<vector<double> > ("srcCTau")),
  dstCTau_ (cfg.getParameter<vector<double> > ("dstCTau")),
  pdgIds_ (cfg.getParameter<vector<int> > ("pdgIds"))
{
  mcparticlesToken_ = consumes<vector<TYPE(hardInteractionMcparticles)> > (collections_.getParameter<edm::InputTag> ("hardInteractionMcparticles"));
}

LifetimeWeightProducer::~LifetimeWeightProducer() {}

void
LifetimeWeightProducer::AddVariables (const edm::Event &event) {
  double weight = 1.0;
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == AOD
  edm::Handle<vector<TYPE(hardInteractionMcparticles)> > mcparticles;
  if (!event.getByToken (mcparticlesToken_, mcparticles))
    {
      (*eventvariables)["lifetimeWeight"] = weight;
      return;
    }

  vector<vector<double> > cTaus;
  cTaus.resize (pdgIds_.size ());
  for (const auto &mcparticle : *mcparticles)
    {
      unsigned iPdgId = 0;
      for (const auto &pdgId : pdgIds_)
        {
          double cTau;
          if (abs (mcparticle.pdgId ()) == abs (pdgId) && isOriginalParticle (mcparticle, mcparticle.pdgId ()) && (cTau = getCTau (mcparticle)) > 0.0)
            cTaus.at (iPdgId).push_back (cTau);
          iPdgId++;
        }
    }

  stringstream suffix;
  for (unsigned iPdgId = 0; iPdgId < pdgIds_.size (); iPdgId++)
    {
      unsigned index = 0;
      for (const auto &cTau : cTaus.at (iPdgId))
        {
          suffix.str ("");
          suffix << "_" << abs (pdgIds_.at (iPdgId)) << "_" << index++;
          (*eventvariables)["cTau" + suffix.str ()] = cTau;

          double srcPDF = exp (-cTau / srcCTau_.at (iPdgId)) / srcCTau_.at (iPdgId),
                 dstPDF = exp (-cTau / dstCTau_.at (iPdgId)) / dstCTau_.at (iPdgId);
          weight *= (dstPDF / srcPDF);
        }
    }
#endif
  (*eventvariables)["lifetimeWeight"] = weight;
}

bool
LifetimeWeightProducer::isOriginalParticle (const TYPE(hardInteractionMcparticles) &mcparticle, const int pdgId) const
{
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == AOD
  if (!mcparticle.numberOfMothers () || mcparticle.motherRef ().isNull ())
    return true;
  return (mcparticle.motherRef ()->pdgId () != pdgId) && isOriginalParticle ((TYPE(hardInteractionMcparticles)) *mcparticle.motherRef (), pdgId);
#else
  return false;
#endif
}

double
LifetimeWeightProducer::getCTau (const TYPE(hardInteractionMcparticles) &mcparticle) const
{
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == AOD
  math::XYZPoint v0 = mcparticle.vertex (), v1;
  double boost = 1.0 / (mcparticle.p4 ().Beta () * mcparticle.p4 ().Gamma ());

  getFinalPosition (mcparticle, mcparticle.pdgId (), true, v1);
  return ((v1 - v0).r () * boost);
#else
  return 0.0;
#endif
}

void
LifetimeWeightProducer::getFinalPosition (const reco::Candidate &mcparticle, const int pdgId, bool firstDaughter, math::XYZPoint &v1) const
{
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == AOD
  if (mcparticle.pdgId () == pdgId)
    {
      v1 = mcparticle.vertex ();
      firstDaughter = true;
    }
  else if (firstDaughter)
    {
      v1 = mcparticle.vertex ();
      firstDaughter = false;
    }
  for (const auto &daughter : mcparticle)
    getFinalPosition (daughter, pdgId, firstDaughter, v1);
#endif
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(LifetimeWeightProducer);
