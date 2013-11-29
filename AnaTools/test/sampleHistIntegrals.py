# sampleHistIntegrals.py
#
# Specify the condor directory, dataset, channel, histogram name, and range for integrals.
# 
# Usage:  getHistIntegrals.py -l sampleHistIntegrals.py 

condor_dir = 'condor_2013_11_20_FakeTrkBkgdEst' 
channel =  'FullSelectionInvD0InvDZInvNHits' 
histName = "nHitsMissingOuter" 

input_hists = [    

    { 'condor_dir' : condor_dir, 
      'dataset' : 'WjetsHighPt',
      'channel' : channel, 
      'histName' : histName,  
      'xlo' : 3, 
      'xhi' : 15, 
      },


    { 'condor_dir' : condor_dir, 
      'dataset' : 'WjetsHighPt',
      'channel' : channel, 
      'histName' : histName, 
      'xlo' : 0, 
      'xhi' : 15, 
      },


    ]


