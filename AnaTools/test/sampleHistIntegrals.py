# sampleHistIntegrals.py
#
# Specify the condor directory, dataset, channel, histogram name, and range for integrals.
# 
# Usage:  getHistIntegrals.py -l sampleHistIntegrals.py 

condor_dir = 'condor_2013_11_20_FakeTrkBkgdEst' 
channel =  'FullSelectionInvD0InvDZInvNHits' 
histName = "nHitsMissingOuter" 

# A dictionary you define to calculate the ratios between some of the integrals you get.

RatioDic = {
              'Name of the ratio you define' : {
                                                'Numerator'   : {'value' : 0.0, 'error' : 0.0 },
                                                'Denominator' : {'value' : 0.0, 'error' : 0.0 }
                                         }
           }

#If yo do not want to calculate the ratio, comment out the 'role in ratioDic' lines below. 
input_hists = [    

    { 'condor_dir' : condor_dir, 
      'dataset' : 'WjetsHighPt',
      'channel' : channel, 
      'histName' : histName,  
      'xlo' : 3, 
      'xhi' : 15, 
      'role in ratioDic' : ['Name of the ratio you define','Numerator'],#Add this line if you want to use it as an input to the ratio calculation, the name shuold be the same as one of them defined above. 
      },


    { 'condor_dir' : condor_dir, 
      'dataset' : 'WjetsHighPt',
      'channel' : channel, 
      'histName' : histName, 
      'xlo' : 0, 
      'xhi' : 15, 
      'role in ratioDic' : ['Name of the ratio you define','Denominator'],#Add this line if you want to use it as an input to the ratio calculation, the name shuold be the same as one of them defined above.
      },


    ]


