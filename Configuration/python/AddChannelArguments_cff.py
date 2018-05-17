import FWCore.ParameterSet.Config as cms

class AddChannelArguments:
  pass

addChannelArguments = AddChannelArguments ()

addChannelArguments.variableProducers = []
addChannelArguments.weights = cms.VPSet()
addChannelArguments.scalingfactorproducers = []
addChannelArguments.standAloneAnalyzers = []
addChannelArguments.channels = []
addChannelArguments.histogramSets = cms.VPSet()
addChannelArguments.collections = cms.PSet()

