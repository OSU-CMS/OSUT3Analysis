import FWCore.ParameterSet.Config as cms

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
    mets                        =  cms.InputTag  ('slimmedMETs',                    ''),
    muons                       =  cms.InputTag  ('slimmedMuons',                   ''),
    photons                     =  cms.InputTag  ('slimmedPhotons',                 ''),
#    pileupinfos                 =  cms.InputTag  ('addPileupInfo',                  ''),
    pileupinfos                 =  cms.InputTag  ('slimmedAddPileupInfo',           ''),
    prescales                   =  cms.InputTag  ('patTrigger',                     ''),
    primaryvertexs              =  cms.InputTag  ('offlineSlimmedPrimaryVertices',  ''),
    superclusters               =  cms.InputTag  ('reducedEgamma',                  'reducedSuperClusters'),
    taus                        =  cms.InputTag  ('slimmedTaus',                    ''),
    triggers                    =  cms.InputTag  ('TriggerResults',                 '',                       'HLT'),
    trigobjs                    =  cms.InputTag  ('selectedPatTrigger',             ''),
)

collectionMapAOD = cms.PSet (
    electrons         =  cms.InputTag  ('uncleanedOnlyGsfElectrons'),
    generatorweights  =  cms.InputTag  ('generator',                   ''),
    genjets           =  cms.InputTag  ('ak4GenJets'),
    jets              =  cms.InputTag  ('ak4PFJets'),
    mcparticles       =  cms.InputTag  ('genParticles'),
    mets              =  cms.InputTag  ('pfMet'),
    muons             =  cms.InputTag  ('muons'),
    taus              =  cms.InputTag  ('hpsPFTauProducer'),
    tracks            =  cms.InputTag  ('generalTracks'),
    triggers          =  cms.InputTag  ('TriggerResults',              '',   'HLT'),
    photons           =  cms.InputTag  ('photons'),
    primaryvertexs    =  cms.InputTag  ('offlinePrimaryVertices'),
)
