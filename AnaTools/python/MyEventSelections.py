import FWCore.ParameterSet.Config as cms


###########################################################
##### Set up the event selections (channels) #####
###########################################################

##### List of valid input collections #####   
# jets, muons, electrons, taus, photons, mets,
# events, tracks, primaryvertexs,
# genjets, mcparticles,
# bxlumis, superclusters


ZtoMuMu = cms.PSet(
    name = cms.string("ZtoMuMu"),
    cuts = cms.VPSet (
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 2")
      ),    
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("pt > 25"),
        numberRequired = cms.string(">= 2")
      ),
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("tightID > 0"),
        numberRequired = cms.string(">= 2")
      ),
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("relPFdBetaIso < 0.12"),
        numberRequired = cms.string(">= 2")
      ),
      cms.PSet (
        inputCollection = cms.string("muon-muon pairs"),
        cutString = cms.string("invMass > 40 & invMass < 160"),
        numberRequired = cms.string(">= 1")
      ),

   )   
)

WtoMuNu = cms.PSet(
    name = cms.string("WtoMuNu"),
    cuts = cms.VPSet (
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 1")
      ),    
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("pt > 25"),
        numberRequired = cms.string(">= 1")
      ),
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("tightID > 0"),
        numberRequired = cms.string(">= 1")
      ),
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("relPFdBetaIso < 0.12"),
        numberRequired = cms.string(">= 1")
      ),
      cms.PSet (
        inputCollection = cms.string("muons"),
        cutString = cms.string("relPFdBetaIso < 0.12"),
        numberRequired = cms.string("= 1"),
        alias = cms.string("2nd muon veto")
      ),
      cms.PSet (
        inputCollection = cms.string("mets"),
        cutString = cms.string("pt > 40"),
        numberRequired = cms.string(">= 1"),
        alias = cms.string("met > 40")
      ),
   )   
)

ZtoEE = cms.PSet(
    name = cms.string("ZtoEE"),
    cuts = cms.VPSet (
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 2")
      ),    
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("pt > 25"),
        numberRequired = cms.string(">= 2")
      ),
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("mvaNonTrigV0 > 0.9"),
        numberRequired = cms.string(">= 2")
      ),
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("relPFrhoIso < 0.1"),
        numberRequired = cms.string(">= 2")
      ),
      cms.PSet (
        inputCollection = cms.string("electron-electron pairs"),
        cutString = cms.string("invMass > 40 & invMass < 160"),
        numberRequired = cms.string(">= 1")
      ),
   )   
)

WtoENu = cms.PSet(
    name = cms.string("WtoENu"),
    cuts = cms.VPSet (
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("abs(eta) < 2.5"),
        numberRequired = cms.string(">= 1")
      ),    
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("pt > 25"),
        numberRequired = cms.string(">= 1")
      ),
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("mvaNonTrigV0 > 0.9"),
        numberRequired = cms.string(">= 1")
      ),
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("relPFrhoIso < 0.1"),
        numberRequired = cms.string(">= 1")
      ),
      cms.PSet (
        inputCollection = cms.string("electrons"),
        cutString = cms.string("relPFrhoIso < 0.1"),
        numberRequired = cms.string("== 1"),
        alias = cms.string("2nd electron veto")
      ),
      cms.PSet (
        inputCollection = cms.string("mets"),
        cutString = cms.string("pt > 40"),
        numberRequired = cms.string(">= 1"),
        alias = cms.string("met > 40")
      ),
   )   
)
