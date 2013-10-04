#!/usr/bin/env python
intLumi = 19800

###################
# 'color' options #
###################
## 'black'
## 'red'  
## 'green'
## 'purple'
## 'blue'  
## 'yellow'
## default: cycle through list


####################
# 'marker' options #
####################
## 'circle'
## 'square'
## 'triangle'
## default: 'circle'

####################
#  'fill' options  #
####################
## 'solid'
## 'hollow'
## default: 'solid'


input_sources = [
    { 'condor_dir' : 'triggerEfficiencyTest',
      'dataset' : 'stop400_100.0mm_br50',
      'den_channel' : 'NoTrigger',
      'num_channel' : 'Mu22Photon22',
      'legend_entry' : '#epsilon of HLT_Mu22_Photon22_CaloIdL_v',
      'color' : 'red',
    },
    { 'condor_dir' : 'triggerEfficiencyTest',
      'dataset' : 'stop400_100.0mm_br50',
      'den_channel' : 'NoTrigger',
      'num_channel' : 'Mu30Ele30',
      'legend_entry' : '#epsilon of HLT_Mu30_Ele30_CaloIdL_v',
      'color' : 'green',
    },
##     { 'condor_dir' : 'triggerEfficiencyTest',
##       'dataset' : 'stop400_100.0mm_br50',
##       'den_channel' : 'NoTrigger',
##       'num_channel' : 'Mu17Ele8',
##       'legend_entry' : '#epsilon of HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
##     },
##     { 'condor_dir' : 'triggerEfficiencyTest',
##       'dataset' : 'stop400_100.0mm_br50',
##       'den_channel' : 'NoTrigger',
##       'num_channel' : 'Mu8Ele17',
##       'legend_entry' : '#epsilon of HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
##     },
    { 'condor_dir' : 'triggerEfficiencyTest',
      'dataset' : 'stop400_100.0mm_br50',
      'den_channel' : 'NoTrigger',
      'num_channel' : 'Mu17Ele8ORMu8Ele17',
      'legend_entry' : '#epsilon of Mu17Ele8 OR Mu8Ele17',
      'color' : 'purple',
    },

]
