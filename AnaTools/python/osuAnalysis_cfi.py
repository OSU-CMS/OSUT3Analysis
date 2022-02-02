import FWCore.ParameterSet.Config as cms
import copy

#########################################
##### Define the datatier being used ####
#########################################

dataFormat = "MINI_AOD_2017_CUSTOM"

###########################################################
##### Set up the standard input collections in miniAOD ####
###########################################################

collectionMap = cms.PSet (
    beamspots                   =  cms.InputTag  ('offlineBeamSpot',                ''),
    electrons                   =  cms.InputTag  ('slimmedElectrons',               ''),
    generatorweights            =  cms.InputTag  ('generator',                      ''),
    genjets                     =  cms.InputTag  ('slimmedGenJets',                 ''),
    jets                        =  cms.InputTag  ('slimmedJets',                    ''),
    bjets                       =  cms.InputTag  ('slimmedJets',                    ''),
    mcparticles                 =  cms.InputTag  ('packedGenParticles',             ''),
    hardInteractionMcparticles  =  cms.InputTag  ('prunedGenParticles',             ''),
    lostTracks                  =  cms.InputTag  ('lostTracks',                     ''),
    mets                        =  cms.InputTag  ('slimmedMETs',                    ''),
    muons                       =  cms.InputTag  ('slimmedMuons',                   ''),
    photons                     =  cms.InputTag  ('slimmedPhotons',                 ''),
    pileupinfos                 =  cms.InputTag  ('slimmedAddPileupInfo',           ''),
    pfCandidates                =  cms.InputTag  ('packedPFCandidates','',          ''),
    prescales                   =  cms.InputTag  ('patTrigger',                     ''),
    primaryvertexs              =  cms.InputTag  ('offlineSlimmedPrimaryVertices',  ''),
    superclusters               =  cms.InputTag  ('reducedEgamma',                  'reducedSuperClusters'),
    taus                        =  cms.InputTag  ('slimmedTaus',                    ''),
    triggers                    =  cms.InputTag  ('TriggerResults',                 '',                       'HLT'),
    metFilters                  =  cms.InputTag  ('TriggerResults',                 '',                       'PAT'),
    trigobjs                    =  cms.InputTag  ('selectedPatTrigger',             ''),
)

#########################################################################
##### Set up the standard input collections in 2017 and 2018 miniAOD ####
#########################################################################

collectionMapMiniAOD2017 = copy.deepcopy(collectionMap)
collectionMapMiniAOD2017.tracks          = cms.InputTag ('isolatedTracks', '')
collectionMapMiniAOD2017.secondaryTracks = cms.InputTag ('isolatedTracks', '')
collectionMapMiniAOD2017.trigobjs        = cms.InputTag  ('slimmedPatTrigger') # Thanks MINIAOD for changing names!

###########################################################
##### Set up the standard input collections in AOD ####
###########################################################

collectionMapAOD = cms.PSet (
    beamspots                  =  cms.InputTag  ('offlineBeamSpot'),
    electrons                  =  cms.InputTag  ('uncleanedOnlyGsfElectrons'),
    generatorweights           =  cms.InputTag  ('generator',                   ''),
    genjets                    =  cms.InputTag  ('ak4GenJets'),
    jets                       =  cms.InputTag  ('ak4PFJets'),
    mcparticles                =  cms.InputTag  ('genParticles'),
    hardInteractionMcparticles =  cms.InputTag  ('genParticles'),
    mets                       =  cms.InputTag  ('pfMet'),
    muons                      =  cms.InputTag  ('muons'),
    taus                       =  cms.InputTag  ('hpsPFTauProducer'),
    tracks                     =  cms.InputTag  ('generalTracks'),
    triggers                   =  cms.InputTag  ('TriggerResults',              '',   'HLT'),
    photons                    =  cms.InputTag  ('photons'),
    pileupinfos                =  cms.InputTag  ('addPileupInfo'),
    primaryvertexs             =  cms.InputTag  ('offlinePrimaryVertices'),
)
