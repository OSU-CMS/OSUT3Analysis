import FWCore.ParameterSet.Config as cms
import os

# Define the CMSSW producer

if os.environ["CMSSW_VERSION"].startswith("CMSSW_10_2_") and int(os.environ["CMSSW_VERSION"].split("_")[3]) >= 10:
	from PhysicsTools.PatUtils.l1ECALPrefiringWeightProducer_cfi import l1ECALPrefiringWeightProducer
	prefiringweight = l1ECALPrefiringWeightProducer.clone(
    	DataEra = cms.string("replaceMe"),
    	UseJetEMPt = cms.bool(False),
    	PrefiringRateSystematicUncty = cms.double(0.2),
    	SkipWarnings = False
    )
else:
	prefiringweight = cms.EDProducer("L1ECALPrefiringWeightProducer",
		ThePhotons = cms.InputTag("slimmedPhotons"),
		TheJets = cms.InputTag("slimmedJets"),
		L1Maps = cms.string(os.environ["CMSSW_BASE"] + "/src/L1Prefiring/EventWeightProducer/files/L1PrefiringMaps_new.root"),
		DataEra = cms.string("replaceMe"),
		UseJetEMPt = cms.bool(False), #can be set to true to use jet prefiring maps parametrized vs pt(em) instead of pt
		PrefiringRateSystematicUncty = cms.double(0.2) #Minimum relative prefiring uncty per object
	)

# Call: variableProducers.extend(l1PrefiringWeightVariableProducers)
# *order matters*
l1PrefiringWeightVariableProducers = ['prefiringweight', 'L1PrefiringWeightProducer']
