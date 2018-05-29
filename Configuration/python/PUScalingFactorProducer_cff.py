import FWCore.ParameterSet.Config as cms
import os

PUScalingFactorProducer = cms.EDFilter ("PUScalingFactorProducer",
    # this should be set in the user's config file if a different pileup file is being used
    PU       =  cms.string(os.environ['CMSSW_BASE'] + '/src/OSUT3Analysis/Configuration/data/pu.root'),

    # this should always be set in the user's config file
    target   =  cms.string("data"),

    # this is set by osusub.py
    dataset  =  cms.string("data2015"),
)
