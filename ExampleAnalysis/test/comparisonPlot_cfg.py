#! /usr/bn/env python
intLumi = 16146.2 # 2016G,H only 

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

cutName = 'trigger'

input_sources = [
        {
          'condor_dir'   : 'EMu_Preselection_CorrelationTest_18_05_02',
          'dataset'      : 'Diboson',
          'channel'      : 'PreselectionCorrelatedD0',
          'legend_entry' : 'Correlated |d0|',
          'color'        : 'black',
        },
        {
          'condor_dir'   : 'EMu_Preselection_CorrelationTest_18_05_02',
          'dataset'      : 'Diboson',
          'channel'      : 'PreselectionUncorrelatedD0',
          'legend_entry' : 'Uncorrelated |d0|',
          'color'        : 'red',
        },
        {
          'condor_dir'   : 'EMu_Preselection_CorrelationTest_18_05_02',
          'dataset'      : 'Diboson',
          'channel'      : 'PreselectionCorrelatedGenD0',
          'legend_entry' : 'Correlated |genD0|',
          'color'        : 'blue',
        },
        {
          'condor_dir'   : 'EMu_Preselection_CorrelationTest_18_05_02',
          'dataset'      : 'Diboson',
          'channel'      : 'PreselectionUncorrelatedGenD0',
          'legend_entry' : 'Uncorrelated |genD0|',
          'color'        : 'purple',
        },
]
