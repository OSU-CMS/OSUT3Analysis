#!/usr/bin/env python

# Sample config file for use with makeBNTreePlot.py
# Usage:  makeBNTreePlot.py -l sampleBNTreePlotConfig.py -c myCondorDir  

from muonOptions import *  # Must import local options file, otherwise configurationOptions will be used, i.e., all datasets

BNTreeUseScript = False
BNTreeScript = 'BNTreeRun.C'  

BNTreeChannel = 'WToMuSimple'  
BNTreeWt = 'events_puScaleFactor * events_muonScaleFactor * events_electronScaleFactor'   # excludes lumi weight, which is included automatically 

input_histograms = [


     { 'channel'       : BNTreeChannel,  
       'varToPlot'     : '@muons_pt.size()',
       'histName'      : 'muonsSize',
       'nbins'         : 11,
       'xMin'          : -0.5,
       'xMax'          : 10.5,
       'cutString'     : BNTreeWt, 
       },

     { 'channel'       : BNTreeChannel,
       'varToPlot'     : 'muons_pt',
       'histName'      : 'muons_pt',
       'nbins'         : 100,
       'xMin'          : 0,
       'xMax'          : 500,
       'cutString'     : BNTreeWt, 
       },

    { 'channel'       : BNTreeChannel,
      'varToPlot'     : '@jets_pt.size()',
      'histName'      : 'jetsSize',
      'nbins'         : 15,
      'xMin'          : 0,
      'xMax'          : 15,
      'cutString'     : BNTreeWt,  
      },

    { 'channel'       : BNTreeChannel,
      'varToPlot'     : 'jets_pt',
      'histName'      : 'jets_pt',
      'nbins'         : 100,
      'xMin'          : 0,
      'xMax'          : 500,
      'cutString'     : BNTreeWt,  
      },

    { 'channel'       : BNTreeChannel,  
      'varToPlot'     : 'mets_pt:mets_phi', 
      'histName'      : 'mets_pt:mets_phi',  
      'nbins'         : 100, 
      'xMin'          : -3.17, 
      'xMax'          :  3.17, 
      'nbinsY'        : 100, 
      'yMin'          :   0, 
      'yMax'          : 500, 
      'cutString'     : BNTreeWt,  
      },

    { 'channel'       : BNTreeChannel,
      'varToPlot'     : '@mets_pt.size()',
      'histName'      : 'metsSize',
      'nbins'         : 11,
      'xMin'          : -0.5,
      'xMax'          : 10.5,
      'cutString'     : BNTreeWt, 
      },

    { 'channel'       : BNTreeChannel,
      'varToPlot'     : 'mets_pt',
      'histName'      : 'mets_pt',
      'nbins'         : 100,
      'xMin'          : 0,
      'xMax'          : 500,
      'cutString'     : BNTreeWt, 
      },

    { 'channel'       : BNTreeChannel,
      'varToPlot'     : 'mets_pt',
      'histName'      : 'mets_pt_cut',
      'nbins'         : 100,
      'xMin'          : 0,
      'xMax'          : 500,
      'cutString'     : '(mets_pt < 200) * ' + BNTreeWt,  
      },

    { 'channel'       : BNTreeChannel,
      'varToPlot'     : 'mets_pt',
      'histName'      : 'mets_pt_cutNJets01',
      'nbins'         : 100,
      'xMin'          : 0,
      'xMax'          : 500,
      'cutString'     : '(@jets_pt.size() >= 0 && @jets_pt.size() <= 1 ) * ' + BNTreeWt,  
      },


    { 'channel'       : BNTreeChannel,
      'varToPlot'     : 'events_puScaleFactor',  
      'histName'      : 'events_puScaleFactor', 
      'nbins'         : 100,
      'xMin'          : 0,
      'xMax'          : 5,  
      'cutString'     :'',  
      },

    { 'channel'       : BNTreeChannel,
      'varToPlot'     : 'events_muonScaleFactor',  
      'histName'      : 'events_muonScaleFactor', 
      'nbins'         : 100,
      'xMin'          : 0,
      'xMax'          : 5,  
      'cutString'     : '',  
      },

    { 'channel'       : BNTreeChannel,
      'varToPlot'     : 'events_electronScaleFactor',  
      'histName'      : 'events_electronScaleFactor', 
      'nbins'         : 100,
      'xMin'          : 0,
      'xMax'          : 5,  
      'cutString'     : '',  
      },


      ]  # end input_histograms = [

 

