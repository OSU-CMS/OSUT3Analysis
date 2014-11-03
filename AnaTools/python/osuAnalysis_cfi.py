import FWCore.ParameterSet.Config as cms

###########################################################
##### Set up the standard input collections in miniAOD ####
###########################################################

collectionMap = cms.PSet ( jets = cms.InputTag ('PAT', 'slimmedJets'),
    muons = cms.InputTag ('PAT', 'slimmedMuons'),
    electrons = cms.InputTag ('PAT', 'slimmedElectrons'),
    taus = cms.InputTag ('PAT', 'slimmedTaus'),
    mets = cms.InputTag ('PAT', 'slimmedMETs'),
    genjets = cms.InputTag ('PAT', 'slimmedGenJets'),
    mcparticles = cms.InputTag ('PAT', 'prunedGenParticles'),
    primaryvertexs = cms.InputTag ('PAT', 'offlineSlimmedPrimaryVertices'),
    photons = cms.InputTag ('PAT', 'slimmedPhotons'),
)


collectionMapBEANs = cms.PSet (
    # Copy from https://github.com/OSU-CMS/OSUT3Analysis/blob/master/AnaTools/python/osuAnalysis_cfi.py
    jets = cms.InputTag ('BNproducer', 'selectedPatJetsPFlow'),
    muons = cms.InputTag ('BNproducer', 'selectedPatMuonsLoosePFlow'),
    secMuons = cms.InputTag ('BNproducer', 'selectedPatMuonsLoosePFlow'),
    electrons = cms.InputTag ('BNproducer', 'selectedPatElectronsLoosePFlow'),
    events = cms.InputTag ('BNproducer', ''),
    taus = cms.InputTag ('BNproducer', 'selectedPatTaus'),
    mets = cms.InputTag ('BNproducer', 'patMETsPFlow'),
    tracks = cms.InputTag ('BNproducer', 'generalTracks'),
    genjets = cms.InputTag ('BNproducer', 'ak5GenJets'),
    mcparticles = cms.InputTag ('BNproducer', 'MCstatus3'),
    stops = cms.InputTag ('BNproducer', 'MCstop'),
    primaryvertexs = cms.InputTag ('BNproducer', 'offlinePrimaryVertices'),
    bxlumis = cms.InputTag ('BNproducer', 'BXlumi'),
    photons = cms.InputTag ('BNproducer', 'none'),
    superclusters = cms.InputTag ('BNproducer', 'corHybridSCandMulti5x5WithPreshower'),
    triggers = cms.InputTag('BNproducer','HLT'),
    trigobjs = cms.InputTag('BNproducer','HLT'),
    )




