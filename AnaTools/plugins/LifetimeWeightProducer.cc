#include <vector>
#include <unordered_set>
#include <sstream>

#include <iostream>

#include "OSUT3Analysis/AnaTools/plugins/LifetimeWeightProducer.h"



LifetimeWeightProducer::LifetimeWeightProducer(const edm::ParameterSet &cfg) :
  EventVariableProducer   (cfg),
  reweightingRules_       (cfg.getParameter<edm::VParameterSet> ("reweightingRules")),
  requireLastNotFirstCopy_(cfg.getParameter<bool> ("requireLastNotFirstCopy")),
  requireLastAndFirstCopy_(cfg.getParameter<bool> ("requireLastAndFirstCopy")),
  specialRHadronsForDispLeptons_(cfg.getParameter<bool> ("specialRHadronsForDispLeptons")),
  moreThanOneLLPType_(cfg.getParameter<bool> ("moreThanOneLLPType")),
  dummyPdgId_(cfg.getParameter<std::string> ("dummyPdgId")),
  multiplePdgIds_(cfg.getParameter<vector<int>> ("multiplePdgIds"))
{
  mcparticlesToken_ = consumes<vector<TYPE(hardInteractionMcparticles)> >(collections_.getParameter<edm::InputTag>("hardInteractionMcparticles"));

  dstCTau_.clear();
  srcCTau_.clear();
  pdgIds_.clear();
  isDefaultRule_.clear();

  stringstream suffix;
  for(unsigned int iRule = 0; iRule < reweightingRules_.size(); iRule++) {
    dstCTau_.push_back      (reweightingRules_[iRule].getParameter<vector<double> > ("dstCTaus"));
    srcCTau_.push_back      (reweightingRules_[iRule].getParameter<vector<double> > ("srcCTaus"));
    pdgIds_.push_back       (reweightingRules_[iRule].getParameter<vector<int> >    ("pdgIds"));
    isDefaultRule_.push_back(reweightingRules_[iRule].getParameter<bool>            ("isDefaultRule"));
    assert(srcCTau_.back().size() == dstCTau_.back().size() &&
           pdgIds_.back().size() == dstCTau_.back().size());

    suffix.str("");

    if(moreThanOneLLPType_){
      suffix << "_" << dummyPdgId_
	     << "_" << srcCTau_.back()[0]
	     << "cmTo" << dstCTau_.back()[0]
	     << "cm";
      string suffix_s = suffix.str();
      replace(suffix_s.begin(), suffix_s.end(), '.', 'p');
      suffix.str(suffix_s);
    }
    else{
      for(unsigned int iPdgId = 0; iPdgId < pdgIds_.back().size(); iPdgId++) {
	suffix << "_" << pdgIds_.back()[iPdgId]
	       << "_" << srcCTau_.back()[iPdgId]
	       << "cmTo" << dstCTau_.back()[iPdgId]
	       << "cm";
	string suffix_s = suffix.str();
	replace(suffix_s.begin(), suffix_s.end(), '.', 'p');
	suffix.str(suffix_s);
      }
    }

    weights_.push_back(1.0);
    weightNames_.push_back("lifetimeWeight" + suffix.str());
    //cout<<"weightNames_ are: lifetimeWeight"<<suffix.str()<<endl;
  }

}

LifetimeWeightProducer::~LifetimeWeightProducer() {
}

void
LifetimeWeightProducer::AddVariables(const edm::Event &event, const edm::EventSetup &setup) {

  //cout<<endl;
  //cout<<"event is: "<<event.id().event()<<endl;

#ifndef STOPPPED_PTLS
  for(unsigned int iRule = 0; iRule < weights_.size(); iRule++) weights_[iRule] = 1.0;

  edm::Handle<vector<TYPE(hardInteractionMcparticles)> > mcparticles;
  if(!event.getByToken(mcparticlesToken_, mcparticles)) {
    // Save the weights
    for(unsigned int iRule = 0; iRule < weights_.size(); iRule++) {
      (*eventvariables)[weightNames_[iRule]] = weights_[iRule];
      if(isDefaultRule_[iRule]) {
        (*eventvariables)["lifetimeWeight"] = weights_[iRule];
      }
    }
    return;
  }

  // The user supplies a VPSet of "rules", and an event weight is provided for each rule
  // Each rule is: a list of pdgIds, and (column-aligned with the pdgIds) lists of the
  // source and destination ctaus
  // Example rules:
  //     1000024: 100cm --> 90cm (produces lifetimeWeight_1000024_100cmTo90cm)
  //     1000006: 100cm --> 80cm (produces lifetimeWeight_1000006_100cmTo80cm)
  //     [1000024: 100cm --> 70cm, 1000022: 30cm --> 5cm] (produces lifetimeWeight_1000024_100cmTo70cm_1000022_30cmTo5cm)
  // and you can apply whichever of these you like in your protoConfig.

  for(unsigned int iRule = 0; iRule < dstCTau_.size(); iRule++) {

    vector<vector<double> > cTaus       (pdgIds_[iRule].size());
    vector<vector<double> > decayLengths(pdgIds_[iRule].size());
    vector<vector<double> > betaFactors (pdgIds_[iRule].size());
    vector<vector<double> > gammaFactors(pdgIds_[iRule].size());

    for(const auto &mcparticle: *mcparticles) {
      // Pythia8 first creates particles in the frame of the interaction, and only boosts them
      // for ISR recoil in later steps. So only the last copy is desired. A particle that's both
      // a last and first copy means it is a decay product created after the boost is applied,
      // e.g. a neutralino from a chargino decay, and we've already added the mother chargino.
      // For the Displaced SUSY sample, the final stops that we want (with some lifetime) are both last and first copies:
      // they are stops that appear AFTER the stop r-hadrons in the chain.
      if(requireLastNotFirstCopy_) { //require last, but not first copy (for disappearing tracks)
        if(!mcparticle.isLastCopy()) continue;
	 if(mcparticle.statusFlags().isFirstCopy()) continue;
      }
      else if(requireLastAndFirstCopy_) { //require last AND first copy (for Displaced SUSY)
        if(!mcparticle.isLastCopy()) continue;
	 if(!(mcparticle.statusFlags().isFirstCopy())) continue;
      }

      //cout<<"ctau is: "<<getCTau(mcparticle)<<endl;

      double cTau;
      for(unsigned int iPdgId = 0; iPdgId < pdgIds_[iRule].size(); iPdgId++) {

	if(moreThanOneLLPType_){
	  for(unsigned int i=0; i<multiplePdgIds_.size(); i++){
	    if(abs(mcparticle.pdgId()) == abs(multiplePdgIds_[i]) &&
	       (requireLastNotFirstCopy_ || requireLastAndFirstCopy_ || isOriginalParticle(mcparticle, mcparticle.pdgId())) &&
	       (cTau = getCTau(mcparticle)) > 0.0) {

	      //cout << "Actually using a particle!" << endl;
	      //cout << "\tpdgId = " << mcparticle.pdgId() << endl;
	      //cout << "\tmother id = " << mcparticle.motherRef()->pdgId() << endl;
	      //cout << "\tcTau = " << cTau << endl;

	      cTaus[iPdgId].push_back(cTau);
	      TVector3 x(mcparticle.vx(), mcparticle.vy(), mcparticle.vz());
	      TVector3 y = getEndVertex(mcparticle);
	      decayLengths[iPdgId].push_back((x - y).Mag());
	      betaFactors[iPdgId].push_back(mcparticle.p4().Beta());
	      gammaFactors[iPdgId].push_back(mcparticle.p4().Gamma());
	    }
	  }//end of loop over multiplePdgIds
	}//end if moreThanOneLLPType_
	else{
	  if(abs(mcparticle.pdgId()) == abs(pdgIds_[iRule][iPdgId]) &&
	     (requireLastNotFirstCopy_ || requireLastAndFirstCopy_ || isOriginalParticle(mcparticle, mcparticle.pdgId())) &&
	     (cTau = getCTau(mcparticle)) > 0.0) {

	    //cout << "Actually using a particle!" << endl;
	    //cout << "\tpdgId = " << mcparticle.pdgId() << endl;
	    //cout << "\tmother id = " << mcparticle.motherRef()->pdgId() << endl;
	    //cout << "\tcTau = " << cTau << endl;

	    cTaus[iPdgId].push_back(cTau);
	    TVector3 x(mcparticle.vx(), mcparticle.vy(), mcparticle.vz());
	    TVector3 y = getEndVertex(mcparticle);
	    decayLengths[iPdgId].push_back((x - y).Mag());
	    betaFactors[iPdgId].push_back(mcparticle.p4().Beta());
	    gammaFactors[iPdgId].push_back(mcparticle.p4().Gamma());
	  }
	}//end of else
      }//end of loop over iPdgId

    } // for mcparticles

    stringstream suffix;
    for(unsigned int iPdgId = 0; iPdgId < pdgIds_[iRule].size(); iPdgId++) {
      unsigned index = 0;
      for(unsigned int i_cTau = 0; i_cTau < cTaus[iPdgId].size(); i_cTau++) {
        double cTau = cTaus[iPdgId][i_cTau];

        // Save the cTau for every mcparticle used for reweighting, along with other information
        // If the same pdgId is used in multiple rules, this value will be
        // overwritten for every rule but the ordering/index is always the same, so it will be fine
        suffix.str("");
        suffix << "_" << abs(pdgIds_[iRule][iPdgId]) << "_" << index++;

        (*eventvariables)["cTau" + suffix.str()] = cTau;
        (*eventvariables)["decayLength" + suffix.str()] = decayLengths[iPdgId][i_cTau];
        (*eventvariables)["beta" + suffix.str()] = betaFactors[iPdgId][i_cTau];
        (*eventvariables)["gamma" + suffix.str()] = gammaFactors[iPdgId][i_cTau];

        double srcPDF = exp(-cTau / srcCTau_[iRule][iPdgId]) / srcCTau_[iRule][iPdgId];
        double dstPDF = exp(-cTau / dstCTau_[iRule][iPdgId]) / dstCTau_[iRule][iPdgId];
        weights_[iRule] *= (dstPDF / srcPDF);
      }

      // Add dummy ctau values for unused values of "index" to guarantee these eventvariables exist.
      // That is, if your sample contains a variable number of charginos, still save INVALID_VALUE
      // for 10 charginos in case there ever could be that many.
      while(index < 10) {
        suffix.str("");
        suffix << "_" << abs(pdgIds_[iRule][iPdgId]) << "_" << index++;

        (*eventvariables)["cTau" + suffix.str()] = INVALID_VALUE;
        (*eventvariables)["decayLength" + suffix.str()] = INVALID_VALUE;
        (*eventvariables)["beta" + suffix.str()] = INVALID_VALUE;
        (*eventvariables)["gamma" + suffix.str()] = INVALID_VALUE;
      }
    } // for pdgIds in this rule

  } // for rules
#endif // ifndef STOPPPED_PTLS

  // Save the weights
  for(unsigned int iRule = 0; iRule < weights_.size(); iRule++) {
    (*eventvariables)[weightNames_[iRule]] = weights_[iRule];
    //cout << "Saving weight of "<<weights_[iRule]<<" named: " << weightNames_[iRule] << endl;
    if(isDefaultRule_[iRule]) {
      (*eventvariables)["lifetimeWeight"] = weights_[iRule];
    }
  }
}

bool
LifetimeWeightProducer::isOriginalParticle(const TYPE(hardInteractionMcparticles) &mcparticle, const int pdgId) const
{
#ifndef STOPPPED_PTLS
  if(!mcparticle.numberOfMothers() || mcparticle.motherRef().isNull()) return true;
  return(mcparticle.motherRef()->pdgId() != pdgId);
#else
  return false;
#endif
}

double
LifetimeWeightProducer::getCTau(const TYPE(hardInteractionMcparticles) &mcparticle) const
{
#ifndef STOPPPED_PTLS
  math::XYZPoint v0 = mcparticle.vertex();
  math::XYZPoint v1 = v0;
  double boost = 1.0 /(mcparticle.p4().Beta() * mcparticle.p4().Gamma());

  //for Displaced SUSY, find v1 when particle mother has a different pdgid
  if(specialRHadronsForDispLeptons_ && !mcparticle.motherRef().isNull()) {
    if(mcparticle.pdgId() != mcparticle.motherRef()->pdgId()) {
      v1 = mcparticle.motherRef()->vertex();
    }
  }
  else {
    getFinalPosition(mcparticle, mcparticle.pdgId(), true, v1);
  }

  //cout<<"v0 is: "<<v0<<", v1 is: "<<v1<<endl;
  return((v1 - v0).r() * boost);
#else
  return 0.0;
#endif
}

TVector3
LifetimeWeightProducer::getEndVertex(const TYPE(hardInteractionMcparticles) &mcparticle) const
{
  TVector3 y(99999.0, 99999.0, 99999.0);

  for(const auto &daughter : mcparticle) {
    // Re-sets for every daughter, they should all have the same start vertex
    y.SetXYZ(daughter.vx(), daughter.vy(), daughter.vz());
  }

  return y;
}

// getFinalPosition: continue along the decay chain until you reach a particle with a different pdgId
//                   then take that particle's vertex
void
LifetimeWeightProducer::getFinalPosition(const reco::Candidate &mcparticle, const int pdgId, bool firstDaughter, math::XYZPoint &v1) const
{
#ifndef STOPPPED_PTLS
  if(mcparticle.pdgId() == pdgId) {
    v1 = mcparticle.vertex();
    firstDaughter = true;
  }
  else if(firstDaughter) {
    v1 = mcparticle.vertex();
    firstDaughter = false;
  }
  for(const auto &daughter : mcparticle) {
    getFinalPosition(daughter, pdgId, firstDaughter, v1);
  }
#endif
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(LifetimeWeightProducer);
