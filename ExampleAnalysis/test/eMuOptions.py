#!/usr/bin/env python3

from OSUT3Analysis.Configuration.configurationOptions import *
from OSUT3Analysis.Configuration.processingUtilities import *

InputCondorArguments = {}

config_file = "test_cfg.py"

intLumi = 19680.  # MuEG 22Jan Re-Reco

datasets = [

##########################
### background samples ###
##########################

##       'Top',
##       'EWK_WNjets',

##     'W0jets',
##     #'W1jets',
##     'W2jets',
##     'W3jets',
##     'W4jets',

#    'WW',
##     'WZ',
##     'ZZ',
##     'ZG',
##     'WG',

##     'SingleT_s',
##     'SingleTbar_s',
##     'SingleT_t',
#    'SingleTbar_t',
    #'SingleT_tW',
    #'SingleTbar_tW',

##    'WNjets',
##    'Diboson',
##    'SingleTop',
##    'TTbar',
##    'DY',
#   'QCDFromData',


#   'DY_Other',



#   'Top',
#   'EWK_WNjets_Other',
#   'QCDFromData',
#   'DYToTauTau_20',

## ####################
## ### data samples ###
## ####################

   'MuEG_22Jan2013',

    'Background'

]


## colors and labels must defined for signal samples that will be plotted

types['stopHadron500_1.0mm_br100'] = "signalMC"
types['stopHadron500_10.0mm_br100'] = "signalMC"
types['stopHadron500_100.0mm_br100'] = "signalMC"

colors['stopHadron500_1.0mm_br100'] = 632
colors['stopHadron500_10.0mm_br100'] = 600
colors['stopHadron500_100.0mm_br100'] = 1

labels['stopHadron500_1.0mm_br100'] = "#tilde{t}#tilde{t} M(500) c#tau(1mm)"
labels['stopHadron500_10.0mm_br100'] = "#tilde{t}#tilde{t} M(500) c#tau(10mm)"
labels['stopHadron500_100.0mm_br100'] = "#tilde{t}#tilde{t} M(500) c#tau(100mm)"

options = {}
options['datasets'] = datasets
options['composite_dataset_definitions'] = composite_dataset_definitions
options['dataset_names'] = dataset_names
options['nJobs'] = nJobs
options['maxEvents'] = maxEvents
options['types'] = types
options['labels'] = labels

########################################################
### add_stops function is used to add signal samples ###
########################################################

# the syntax is as follows
# add_stops (options, masses, ctaus, bottomBranchingRatios = [])
# the masses and ctaus are required
# the bottom BRs is optional and will default to just 50%

########################################################

#add_stops (options, [200,300,400,500,600,700,800,900], [0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,20.0,30.0,40.0,50.0,60.0,70.0,80.0,90.0,100.0,200.0,300.0,400.0,500.0,600.0,700.0,800.0,900.0,1000.0],[100])
add_stops (options, [500], [1.0,10.0,100.0],[100])

