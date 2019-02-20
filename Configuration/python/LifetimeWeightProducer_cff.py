import FWCore.ParameterSet.Config as cms
import OSUT3Analysis.DBTools.osusub_cfg as osusub
from OSUT3Analysis.Configuration.configurationOptions import *

rules = []

if osusub.batchMode:
    if osusub.datasetLabel in types and types[osusub.datasetLabel] == "signalMC":
        if osusub.datasetLabel in rulesForLifetimeReweighting:
            rules.extend(rulesForLifetimeReweighting[osusub.datasetLabel])

LifetimeWeightProducer = cms.EDFilter ("LifetimeWeightProducer",
    reweightingRules = cms.VPSet([
        cms.PSet(
            pdgIds = cms.vint32(r.pdgIds), 
            srcCTaus = cms.vdouble(r.srcCTaus), 
            dstCTaus = cms.vdouble(r.dstCTaus), 
            isDefaultRule = cms.bool(r.isDefaultRule)
        ) for r in rules])
)
