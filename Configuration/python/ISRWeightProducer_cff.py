import FWCore.ParameterSet.Config as cms
import OSUT3Analysis.DBTools.osusub_cfg as osusub
from OSUT3Analysis.Configuration.configurationOptions import *

srcMomentumPdgId = cms.vint32 ()

if osusub.batchMode:
    if osusub.datasetLabel in types and types[osusub.datasetLabel] == "signalMC":
        if osusub.datasetLabel in pdgIdsForISRReweighting:
            try:
                srcMomentumPdgId.extend (pdgIdsForISRReweighting[osusub.datasetLabel])
            except TypeError:
                srcMomentumPdgId.extend (pdgIdsForISRReweighting[osusub.datasetLabel])

ISRWeightProducer = cms.EDProducer ("ISRWeightProducer",
    pdgIds = srcMomentumPdgId,
    #weightFile = cms.string(os.environ['CMSSW_BASE'] + '/src/DisappTrks/StandardAnalysis/data/isrWeight_disappTrks_run2.root')
    #weightHist = cms.string('SingleMu_2015D')
)
