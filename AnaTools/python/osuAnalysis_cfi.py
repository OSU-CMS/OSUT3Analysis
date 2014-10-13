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


