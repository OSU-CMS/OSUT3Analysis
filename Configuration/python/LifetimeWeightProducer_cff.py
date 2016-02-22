import FWCore.ParameterSet.Config as cms
import OSUT3Analysis.DBTools.osusub_cfg as osusub
from OSUT3Analysis.Configuration.configurationOptions import *

srcCTau = cms.vdouble ()
dstCTau = cms.vdouble ()
pdgIds = cms.vint32 ()

if osusub.batchMode:
    if osusub.datasetLabel in types and types[osusub.datasetLabel] == "signalMC":
        if osusub.datasetLabel in srcCTau and osusub.datasetLabel in dstCTau and osusub.datasetLabel in pdgIdsForLifetimeReweighting:
            try:
                srcCTau.extend (srcCTau[osusub.datasetLabel])
            except TypeError:
                srcCTau.append (srcCTau[osusub.datasetLabel])
            try:
                dstCTau.extend (srcCTau[osusub.datasetLabel])
            except TypeError:
                dstCTau.append (srcCTau[osusub.datasetLabel])
            try:
                pdgIds.extend (srcCTau[osusub.datasetLabel])
            except TypeError:
                pdgIds.append (srcCTau[osusub.datasetLabel])

LifetimeWeightProducer = cms.EDProducer ("LifetimeWeightProducer",
    srcCTau = srcCTau,
    dstCTau = dstCTau,
    pdgIds = pdgIds,
)
