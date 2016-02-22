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
    pileupinfos                 =  cms.InputTag  ('addPileupInfo',                  ''),
    prescales                   =  cms.InputTag  ('patTrigger',                     ''),
    primaryvertexs              =  cms.InputTag  ('offlineSlimmedPrimaryVertices',  ''),
    superclusters               =  cms.InputTag  ('reducedEgamma',                  'reducedSuperClusters'),
    taus                        =  cms.InputTag  ('slimmedTaus',                    ''),
    triggers                    =  cms.InputTag  ('TriggerResults',                 '',                       'HLT'),
    trigobjs                    =  cms.InputTag  ('selectedPatTrigger',             ''),
)


collectionMapBEANs = cms.PSet (
    # Copy from https://github.com/OSU-CMS/OSUT3Analysis/blob/master/AnaTools/python/osuAnalysis_cfi.py
    jets            =  cms.InputTag  ('BNproducer',         'selectedPatJetsPFlow'),
    muons           =  cms.InputTag  ('BNproducer',         'selectedPatMuonsLoosePFlow'),
    secMuons        =  cms.InputTag  ('BNproducer',         'selectedPatMuonsLoosePFlow'),
    electrons       =  cms.InputTag  ('BNproducer',         'selectedPatElectronsLoosePFlow'),
    events          =  cms.InputTag  ('BNproducer',         ''),
    taus            =  cms.InputTag  ('BNproducer',         'selectedPatTaus'),
    mets            =  cms.InputTag  ('BNproducer',         'patMETsPFlow'),
    tracks          =  cms.InputTag  ('BNproducer',         'generalTracks'),
    genjets         =  cms.InputTag  ('BNproducer',         'ak5GenJets'),
    mcparticles     =  cms.InputTag  ('BNproducer',         'MCstatus3'),
    stops           =  cms.InputTag  ('BNproducer',         'MCstop'),
    primaryvertexs  =  cms.InputTag  ('BNproducer',         'offlinePrimaryVertices'),
    bxlumis         =  cms.InputTag  ('BNproducer',         'BXlumi'),
    photons         =  cms.InputTag  ('BNproducer',         'none'),
    superclusters   =  cms.InputTag  ('BNproducer',         'corHybridSCandMulti5x5WithPreshower'),
    triggers        =  cms.InputTag  ('BNproducer','HLT'),
    trigobjs        =  cms.InputTag  ('BNproducer','HLT'),
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


