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
    { 'condor_dir' : 'yieldsFromAndrew',
      'dataset' : 'QCD_MuEnriched',
      'channel' : 'Isolated',
      'legend_entry' : 'isolated',
      'marker' : 'triangle',
    },
    { 'condor_dir' : 'yieldsFromAndrew',
      'dataset' : 'QCD_MuEnriched',
      'channel' : 'AntiIsolated',
      'legend_entry' : 'anti-isolated',
      'fill' : 'hollow',
    },
]
