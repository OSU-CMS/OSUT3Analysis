#!/usr/bin/env python


SignalSets = [
   #Select Signal Samples here.
   'stop200toBl_1mm',
   'stop200toBl_10mm',
   'stop200toBl_100mm',
   'stop300toBl_1mm',
   'stop300toBl_10mm',
   'stop300toBl_100mm',
   'stop400toBl_1mm',
   'stop400toBl_10mm',
   'stop400toBl_100mm',
   'stop500toBl_1mm',
   'stop500toBl_10mm',
   'stop500toBl_100mm',
   'stop600toBl_1mm',
   'stop600toBl_10mm',
   'stop600toBl_100mm',

   'stop200toTnu_1mm',
   'stop200toTnu_10mm',
   'stop200toTnu_100mm',
   'stop300toTnu_1mm',
   'stop300toTnu_10mm',
   'stop300toTnu_100mm',
   'stop400toTnu_1mm',
   'stop400toTnu_10mm',
   'stop400toTnu_100mm',
   'stop500toTnu_1mm',
   'stop500toTnu_10mm',
   'stop500toTnu_100mm',
   'stop600toTnu_1mm',
   'stop600toTnu_10mm',
   'stop600toTnu_100mm',
]

BackgroundSets = [
   #list of backgrounds that could be added into the datacard
   'TTbar',
   'TTbar_Had',
   'TTbar_SemiLep',
   'TTbar_Lep',
   'Diboson',
   'WW',
   'WZ',
   'ZZ',
   'DY',
   'DYToEE_20',
   'DYToMuMu_20',
   'DYToTauTau_20',
   'Wjets',
   'QCD_MuEnriched',
]

Trigger_UncertaintyApplyList = [
   #Choose what sampls should be applied a trigger uncertainty.
   'TTbar',
   'TTbar_Had',
   'TTbar_SemiLep',
   'TTbar_Lep',
   'Diboson',
   'WW',
   'WZ',
   'ZZ',
   'DY',
   'DYToEE_20',
   'DYToMuMu_20',
   'DYToTauTau_20',
   'Wjets',
   'QCD_MuEnriched',
   
   'stop200toBl_1mm',
   'stop200toBl_10mm',
   'stop200toBl_100mm',
   'stop300toBl_1mm',
   'stop300toBl_10mm',
   'stop300toBl_100mm',
   'stop400toBl_1mm',
   'stop400toBl_10mm',
   'stop400toBl_100mm',
   'stop500toBl_1mm',
   'stop500toBl_10mm',
   'stop500toBl_100mm',
   'stop600toBl_1mm',
   'stop600toBl_10mm',
   'stop600toBl_100mm',

   'stop200toTnu_1mm',
   'stop200toTnu_10mm',
   'stop200toTnu_100mm',
   'stop300toTnu_1mm',
   'stop300toTnu_10mm',
   'stop300toTnu_100mm',
   'stop400toTnu_1mm',
   'stop400toTnu_10mm',
   'stop400toTnu_100mm',
   'stop500toTnu_1mm',
   'stop500toTnu_10mm',
   'stop500toTnu_100mm',
   'stop600toTnu_1mm',
   'stop600toTnu_10mm',
]
Reco_UncertaintyApplyList = [
   #Choose what samples should be applied a reco uncertainty.
   'TTbar',
   'TTbar_Had',
   'TTbar_SemiLep',
   'TTbar_Lep',
   'Diboson',
   'WW',
   'WZ',
   'ZZ',
   'DY',
   'DYToEE_20',
   'DYToMuMu_20',
   'DYToTauTau_20',
   'Wjets',
   'QCD_MuEnriched',
   
   'stop200toBl_1mm',
   'stop200toBl_10mm',
   'stop200toBl_100mm',
   'stop300toBl_1mm',
   'stop300toBl_10mm',
   'stop300toBl_100mm',
   'stop400toBl_1mm',
   'stop400toBl_10mm',
   'stop400toBl_100mm',
   'stop500toBl_1mm',
   'stop500toBl_10mm',
   'stop500toBl_100mm',
   'stop600toBl_1mm',
   'stop600toBl_10mm',
   'stop600toBl_100mm',

   'stop200toTnu_1mm',
   'stop200toTnu_10mm',
   'stop200toTnu_100mm',
   'stop300toTnu_1mm',
   'stop300toTnu_10mm',
   'stop300toTnu_100mm',
   'stop400toTnu_1mm',
   'stop400toTnu_10mm',
   'stop400toTnu_100mm',
   'stop500toTnu_1mm',
   'stop500toTnu_10mm',
   'stop500toTnu_100mm',
   'stop600toTnu_1mm',
   'stop600toTnu_10mm',
]

MC_Global_Uncertainties= [
   #List of global Uncertainties.
   'MC_Trigger_Uncertainties',
   'MC_Reco_Uncertainties',
]


MC_Trigger_Uncertainties = {
   'type' : 'lnN',
   'value' : '0.064',
   'applylist' : Trigger_UncertaintyApplyList,
}

MC_Reco_Uncertainties = {
   'type' : 'lnN',
   'value' : '0.01',
   'applylist' : Reco_UncertaintyApplyList,
}

Norm_Uncertainties = {
    #Normalization uncertainties, 'FromHistogram' means the value would be got from a cutflow histogram. Also we could assign a number by ourselves.
    'DY' : { 
	'value' : 'FromHistogram',
        'type' : 'lnN',
    },   
    'Diboson' : {
        'value' : 'FromHistogram',
        'type' : 'lnN',
    },
    'TTbar' : {
        'value' : 'FromHistogram',
        'type' : 'lnN',
    },
    'Wjets' : {
        'value' : 1.068,
        'type' : 'lnN',
    },
}
 
Stop_Ctau = {

   'stop200toBl_1mm':   '1.00mm',
   'stop200toBl_10mm':  '10.0mm',
   'stop200toBl_100mm': '100.mm',
   'stop300toBl_1mm':   '1.00mm',
   'stop300toBl_10mm':  '10.0mm',
   'stop300toBl_100mm': '100.mm',
   'stop400toBl_1mm':   '1.00mm',
   'stop400toBl_10mm':  '10.0mm',
   'stop400toBl_100mm': '100.mm',
   'stop500toBl_1mm':   '1.00mm',
   'stop500toBl_10mm':  '10.0mm',
   'stop500toBl_100mm': '100.mm',
   'stop600toBl_1mm':   '1.00mm',
   'stop600toBl_10mm':  '10.0mm',
   'stop600toBl_100mm': '100.mm',

   'stop200toTnu_1mm':  '1.00mm',
   'stop200toTnu_10mm': '10.0mm',
   'stop200toTnu_100mm':'100.mm', 
   'stop300toTnu_1mm':  '1.00mm',
   'stop300toTnu_10mm': '10.0mm',
   'stop300toTnu_100mm':'100.mm',
   'stop400toTnu_1mm':  '1.00mm',
   'stop400toTnu_10mm': '10.0mm',
   'stop400toTnu_100mm':'100.mm',
   'stop500toTnu_1mm':  '1.00mm',
   'stop500toTnu_10mm': '10.0mm',
   'stop500toTnu_100mm':'100.mm',
   'stop600toTnu_1mm':  '1.00mm',
   'stop600toTnu_10mm': '10.0mm',
   'stop600toTnu_100mm':'100.mm',

}

Stop_Mass = {

   'stop200toBl_1mm':   '200',
   'stop200toBl_10mm':  '200',
   'stop200toBl_100mm': '200',
   'stop300toBl_1mm':   '300',
   'stop300toBl_10mm':  '300',
   'stop300toBl_100mm': '300',
   'stop400toBl_1mm':   '400',
   'stop400toBl_10mm':  '400',
   'stop400toBl_100mm': '400',
   'stop500toBl_1mm':   '500',
   'stop500toBl_10mm':  '500',
   'stop500toBl_100mm': '500',
   'stop600toBl_1mm':   '600',
   'stop600toBl_10mm':  '600',
   'stop600toBl_100mm': '600',

   'stop200toTnu_1mm':  '200',
   'stop200toTnu_10mm': '200',
   'stop200toTnu_100mm':'200', 
   'stop300toTnu_1mm':  '300',
   'stop300toTnu_10mm': '300',
   'stop300toTnu_100mm':'300',
   'stop400toTnu_1mm':  '400',
   'stop400toTnu_10mm': '400',
   'stop400toTnu_100mm':'400',
   'stop500toTnu_1mm':  '500',
   'stop500toTnu_10mm': '500',
   'stop500toTnu_100mm':'500',
   'stop600toTnu_1mm':  '600',
   'stop600toTnu_10mm': '600',
   'stop600toTnu_100mm':'600',

}

Tmp = []

for i in range(0,101,3):   #Define the step length of the branching ratios.The last digit in range()
    Tmp.append(float(i)/100)
Bl_BR = Tmp
