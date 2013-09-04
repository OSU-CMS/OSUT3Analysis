#!/usr/bin/env python
input_histograms = [
    {
        'datasets' : ['TTbar', 'Diboson', 'DY', 'Wjets', 'QCD'],   # datasets whose normalizations we want to fit to data
        'target_dataset' : 'MuEG',                                 # the target dataset, presumably data
        'channel' : 'Isolated',
        'name' : 'electronMuonInvMass',
        'iterations' : 10,                                         # number of iterations for the fit
        'lowerLimit' : 25,                                         # lower limit on range for the fit
        'upperLimit' : 500,                                        # upper limit on range for the fit
    },
    {
        'datasets' : ['TTbar', 'Diboson', 'DY', 'Wjets', 'QCD'],
        'target_dataset' : 'MuEG',
        'channel' : 'Isolated',
        'name' : 'electronAbsD0Beamspot',
        'iterations' : 10,
                                                                   # if the limits are ommitted, the entire range is used
    },
    {
        'datasets' : ['TTbar', 'Diboson', 'DY', 'Wjets', 'QCD'],
        'target_dataset' : 'MuEG',
        'channel' : 'Isolated',
        'name' : 'muonAbsD0Beamspot',
        'iterations' : 10,
    },
]
