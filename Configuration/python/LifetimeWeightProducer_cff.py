import FWCore.ParameterSet.Config as cms
import OSUT3Analysis.DBTools.osusub_cfg as osusub
from OSUT3Analysis.Configuration.configurationOptions import *

srcCTau = cms.vdouble ()
dstCTau = cms.vdouble ()
pdgIds = cms.vint32 ()

if osusub.batchMode:
    if osusub.datasetLabel in types and types[osusub.datasetLabel] == "signalMC":
        if osusub.datasetLabel in srcCTauForLifetimeReweighting and osusub.datasetLabel in dstCTauForLifetimeReweighting and osusub.datasetLabel in pdgIdsForLifetimeReweighting:
            try:
                srcCTau.extend (srcCTauForLifetimeReweighting[osusub.datasetLabel])
            except TypeError:
                srcCTau.append (srcCTauForLifetimeReweighting[osusub.datasetLabel])
            try:
                dstCTau.extend (dstCTauForLifetimeReweighting[osusub.datasetLabel])
            except TypeError:
                dstCTau.append (dstCTauForLifetimeReweighting[osusub.datasetLabel])
            try:
                pdgIds.extend (pdgIdsForLifetimeReweighting[osusub.datasetLabel])
            except TypeError:
                pdgIds.append (pdgIdsForLifetimeReweighting[osusub.datasetLabel])

LifetimeWeightProducer = cms.EDProducer ("LifetimeWeightProducer",
    srcCTau = srcCTau,
    dstCTau = dstCTau,
    pdgIds = pdgIds,
)
