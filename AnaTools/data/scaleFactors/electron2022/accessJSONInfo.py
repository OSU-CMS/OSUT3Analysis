## example how to read the muon format v2
## (Adapted from JMAR and EGM examples)
from correctionlib import _core
import json
from array import array
from ROOT import TFile, TH2D, TCanvas

#######################################################################################################
## This script contains functions to:
##     1. create root file SFs from json files for electron ID and RECO SFs
##     2. combine RECO SF histograms into one histogram
##
## For 2022 data the root files are available on EOS so (1) is not necessary
## Original JSON files can be downloaded from https://gitlab.cern.ch/cms-nanoAOD/jsonpog-integration/-/tree/master/POG/EGM/2022_Summer22EE
######################################################################################################


fname = "electron.json.gz"
if fname.endswith(".json.gz"):
    import gzip
    with gzip.open(fname,'rt') as file:
        data = file.read().strip()
        evaluator = _core.CorrectionSet.from_string(data)
else:
    evaluator = _core.CorrectionSet.from_file(fname)

myevaluator = evaluator['Electron-ID-SF']

ptRange = {}
ptRange[15.0] = "pt:[10.00,20.00]"
ptRange[27.0] = "pt:[20.00,35.00]"
ptRange[42.0] = "pt:[35.00,50.00]"
ptRange[75.0] = "pt:[50.00,100.00]"
ptRange[200.0] = "pt:[100.00,Inf]"

etaRange = {}
etaRange[-3.] = "eta:[-Inf,-2.]"
etaRange[-1.8] = "eta:[-2.,-1.566]"
etaRange[-1.5] = "eta:[-1.566,-1.444]"
etaRange[-1.2] = "eta:[-1.444,-1.0]"
etaRange[-0.5] = "eta:[-1.0,0.0]"
etaRange[0.5] = "eta:[0.0,1.0]"
etaRange[1.2] = "eta:[1.0,1.444]"
etaRange[1.5] = "eta:[1.444,1.566]"
etaRange[1.8] = "eta:[1.566,2.0]"
etaRange[3.] = "eta:[2.0,Inf]"

namesId = ['Veto', 'Loose', 'Medium', 'Tight']

##############
# Electron ID
##############

def electronID_sf():
    new = {}
    for name in namesId:
        typeSF = {}
        eta_pt = {}
        for eta in etaRange.keys():
            etaDict = {}
            for pt in ptRange.keys():
                ptDict = {}
                valsf = myevaluator.evaluate("2022Re-recoE+PromptFG","sf",name, eta, pt)
                val_up = myevaluator.evaluate("2022Re-recoE+PromptFG","sfup",name, eta, pt)
                val_down = myevaluator.evaluate("2022Re-recoE+PromptFG","sfdown",name, eta, pt)

                print("Eta {}, Pt {}, SF {} +{}-{}".format(eta, pt, valsf, val_up, val_down))
                ptDict["value"] = valsf
                ptDict["error_up"] = val_up
                ptDict['error_down'] = val_down
                etaDict[ptRange[pt]] = ptDict
            eta_pt[etaRange[eta]] = etaDict
        typeSF["eta_pt"] = eta_pt
        new[name] = typeSF


    fout = 'RunFG_SF_ID.json'
    with open(fout, 'w+') as f:
        json.dump(new, f)

    files = ['RunFG_SF_ID.json']

    for file in files:
        output = TFile(file.strip('.json') + '.root', 'RECREATE')

        era = file.split('_')[0][3:]
        sfType = file.split('_')[-1][:-5]
        if sfType == 'ISO':
            sfType = sfType.title() # 'Iso'

        print(file, era, sfType)

        f = open(file, 'r')
        results = json.load(f)

        for r in results.keys():
            xbinNames = [] # pt
            ybinNames = [] # eta
            values = {}
            errors_up = {}
            errors_down = {}

            etaLabel = 'eta_pt' if 'eta_pt' in results[r] else 'eta_pair_newTuneP_probe_pt'
            yLabelPosition = int(4) if 'eta_pt' in results[r] else int(24)

            for xbin in results[r][etaLabel]:
                xbinNames.append(xbin)
                for ybin in results[r][etaLabel][xbin]:
                    ybinNames.append(ybin)
                    values[ybin, xbin] = results[r][etaLabel][xbin][ybin]['value']
                    errors_up[ybin, xbin] = results[r][etaLabel][xbin][ybin]['error_up']
                    errors_down[ybin, xbin] = results[r][etaLabel][xbin][ybin]['error_down']


            # bins are stored as strings like 'pt:[20.00,25.00]', so figure out real values here

            xbinEdges = []
            for x in xbinNames:
                range_ = x.split(':')[-1].strip('[]').split(',')
                edge1 = float(range_[0])
                edge2 = float(range_[1])
                if edge1 == float('-Inf'):
                    edge1 = float(-2.5)
                if edge2 == float('Inf'):
                    edge2 = float(2.5)
                if not edge1 in xbinEdges:
                    xbinEdges.append(edge1)
                if not edge2 in xbinEdges:
                    xbinEdges.append(edge2)
            xbinEdges.sort()

            ybinEdges = []
            for y in ybinNames:
                range_ = y.split(':')[-1].strip('[]').split(',')
                edge1 = float(range_[0])
                edge2 = float(range_[1])
                if edge2 == float('Inf'):
                    edge2 = float(500.)
                if not edge1 in ybinEdges:
                    ybinEdges.append(edge1)
                if not edge2 in ybinEdges:
                    ybinEdges.append(edge2)
            ybinEdges.sort()

            print("r", r)

            h = TH2D(r, r, len(xbinEdges)-1, array('d', xbinEdges), len(ybinEdges)-1, array('d', ybinEdges))

            values = {}
            errors = {}

            for xbin in results[r][etaLabel]:
                #print('ybin', ybin)
                range_ = xbin.split(':')[-1].strip('[]').split(',')
                xval = float(range_[0])
                if xval == float("-Inf"):
                    xval = -2.5
                for ybin in results[r][etaLabel][xbin]:
                    #print("xval {}, yval {}".format(xval, yval))
                    yval = float(ybin.split(',')[0][yLabelPosition:])

                    #print("xbin", xbin)
                    #print("xval {}, yval {}, bin {}".format(xval+0.01, yval+1, h.FindBin(xval+0.01, yval+1)))
                    ibin = h.FindBin(xval + 0.01, yval + 1)
                    h.SetBinContent(ibin, results[r][etaLabel][xbin][ybin]['value'])
                    h.SetBinError(ibin, results[r][etaLabel][xbin][ybin]['error_up'])
                    h.SetBinError(ibin, -results[r][etaLabel][xbin][ybin]['error_down'])
                    print('x {}, y {}, bin {}, val {}, +{}-{}'.format(xval, yval, ibin, results[r][etaLabel][xbin][ybin]['value'], results[r][etaLabel][xbin][ybin]['error_up'], -results[r][etaLabel][xbin][ybin]['error_down']))


            #h.GetZaxis().SetRangeUser(0.7, 1.1)
            h.GetYaxis().SetTitle('muon pt [GeV]')
            h.GetXaxis().SetTitle('|#eta|')

            output.cd()
            h.Write()

        output.Close()

#################
# Electron RECO
#################

def electronReco_sf():

    namesReco = {'RecoBelow20':[10, 20], 'Reco20to75':[20.0, 45.0, 75.0], 'RecoAbove75':[75.0, 100.0, float('Inf')]}
    namesReco = {'RecoBelow20':[15], 'Reco20to75':[32, 60], 'RecoAbove75':[82, 200]}

    ptRange = {}
    ptRange[15.0] = "pt:[10.00,20.00]"
    ptRange[32.0] = "pt:[20.00,45.00]"
    ptRange[60.0] = "pt:[45.00,75.00]"
    ptRange[82.0] = "pt:[75.00,100.00]"
    ptRange[200.0] = "pt:[100.00,Inf]"

    new = {}
    for name, values in namesReco.items():
        typeSF = {}
        eta_pt = {}
        for eta in etaRange.keys():
            etaDict = {}
            for pt in values:
                pt = float(pt)
                ptDict = {}
                valsf = myevaluator.evaluate("2022Re-recoE+PromptFG","sf",name, eta, pt)
                val_up = myevaluator.evaluate("2022Re-recoE+PromptFG","sfup",name, eta, pt)
                val_down = myevaluator.evaluate("2022Re-recoE+PromptFG","sfdown",name, eta, pt)

                print("Eta {}, Pt {}, SF {} +{}-{}".format(eta, pt, valsf, val_up, val_down))
                ptDict["value"] = valsf
                ptDict["error_up"] = val_up
                ptDict['error_down'] = val_down
                etaDict[ptRange[pt]] = ptDict
            eta_pt[etaRange[eta]] = etaDict
        typeSF["eta_pt"] = eta_pt
        new[name] = typeSF

    fout = 'RunFG_SF_RECO.json'
    with open(fout, 'w+') as f:
        json.dump(new, f)

    files = ['RunFG_SF_RECO.json']

    for file in files:
        output = TFile(file.strip('.json') + '.root', 'RECREATE')

        era = file.split('_')[0][3:]
        sfType = file.split('_')[-1][:-5]
        if sfType == 'ISO':
            sfType = sfType.title() # 'Iso'

        print(file, era, sfType)

        f = open(file, 'r')
        results = json.load(f)

        for r in results.keys():
            xbinNames = [] # pt
            ybinNames = [] # eta
            values = {}
            errors_up = {}
            errors_down = {}

            etaLabel = 'eta_pt' if 'eta_pt' in results[r] else 'eta_pair_newTuneP_probe_pt'
            yLabelPosition = int(4) if 'eta_pt' in results[r] else int(24)

            for xbin in results[r][etaLabel]:
                xbinNames.append(xbin)
                for ybin in results[r][etaLabel][xbin]:
                    ybinNames.append(ybin)
                    values[ybin, xbin] = results[r][etaLabel][xbin][ybin]['value']
                    errors_up[ybin, xbin] = results[r][etaLabel][xbin][ybin]['error_up']
                    errors_down[ybin, xbin] = results[r][etaLabel][xbin][ybin]['error_down']


            # bins are stored as strings like 'pt:[20.00,25.00]', so figure out real values here

            xbinEdges = []
            for x in xbinNames:
                range_ = x.split(':')[-1].strip('[]').split(',')
                edge1 = float(range_[0])
                edge2 = float(range_[1])
                if edge1 == float('-Inf'):
                    edge1 = float(-2.5)
                if edge2 == float('Inf'):
                    edge2 = float(2.5)
                if not edge1 in xbinEdges:
                    xbinEdges.append(edge1)
                if not edge2 in xbinEdges:
                    xbinEdges.append(edge2)
            xbinEdges.sort()

            ybinEdges = []
            for y in ybinNames:
                range_ = y.split(':')[-1].strip('[]').split(',')
                edge1 = float(range_[0])
                edge2 = float(range_[1])
                if edge2 == float('Inf'):
                    edge2 = float(500.)
                if not edge1 in ybinEdges:
                    ybinEdges.append(edge1)
                if not edge2 in ybinEdges:
                    ybinEdges.append(edge2)
            ybinEdges.sort()

            print("r", r)

            h = TH2D(r, r, len(xbinEdges)-1, array('d', xbinEdges), len(ybinEdges)-1, array('d', ybinEdges))

            values = {}
            errors = {}

            for xbin in results[r][etaLabel]:
                #print('ybin', ybin)
                range_ = xbin.split(':')[-1].strip('[]').split(',')
                xval = float(range_[0])
                if xval == float("-Inf"):
                    xval = -2.5
                for ybin in results[r][etaLabel][xbin]:
                    #print("xval {}, yval {}".format(xval, yval))
                    yval = float(ybin.split(',')[0][yLabelPosition:])

                    #print("xbin", xbin)
                    #print("xval {}, yval {}, bin {}".format(xval+0.01, yval+1, h.FindBin(xval+0.01, yval+1)))
                    ibin = h.FindBin(xval + 0.01, yval + 1)
                    h.SetBinContent(ibin, results[r][etaLabel][xbin][ybin]['value'])
                    h.SetBinError(ibin, results[r][etaLabel][xbin][ybin]['error_up'])
                    h.SetBinError(ibin, -results[r][etaLabel][xbin][ybin]['error_down'])
                    print('x {}, y {}, bin {}, val {}, +{}-{}'.format(xval, yval, ibin, results[r][etaLabel][xbin][ybin]['value'], results[r][etaLabel][xbin][ybin]['error_up'], -results[r][etaLabel][xbin][ybin]['error_down']))


            #h.GetZaxis().SetRangeUser(0.7, 1.1)
            h.GetYaxis().SetTitle('muon pt [GeV]')
            h.GetXaxis().SetTitle('|#eta|')

            output.cd()
            h.Write()

        output.Close()

#####################
# combine reco files
#####################

#function to combine the reco plots
#plots from EGamma POG are separated by Pt ranges
def combineRecoPlots():

    f_low = TFile.Open('egammaEffi.txt_EGM2D_reco_lowPT.root', 'READ')
    f_med = TFile.Open('egammaEffi.txt_EGM2D_reco_medPT.root', 'READ')
    f_high = TFile.Open('egammaEffi.txt_EGM2D_reco_highPT.root', 'READ')

    fout = TFile.Open('egammaEffi.txt_EGM2D_RECO.root', 'RECREATE')

    h_sf_low = f_low.Get('EGamma_SF2D')
    h_sf_med = f_med.Get('EGamma_SF2D')
    h_sf_high = f_high.Get('EGamma_SF2D')

    xedges = []
    yedges = []
    content = []

    xaxis = h_sf_low.GetXaxis()
    yaxis_low = h_sf_low.GetYaxis()
    yaxis_med = h_sf_med.GetYaxis()
    yaxis_high = h_sf_high.GetYaxis()

    for binx in range(xaxis.GetNbins()+1):
        x_ = xaxis.GetBinUpEdge(binx)
        xedges.append(x_)
        for biny in range(yaxis_low.GetNbins()+1):
            y_ = yaxis_low.GetBinUpEdge(biny)
            if binx == 0: 
                yedges.append(y_)
            if biny!=yaxis_low.GetNbins() and binx!=xaxis.GetNbins():
                value = h_sf_low.GetBinContent(h_sf_low.FindBin(x_+0.01, y_+0.01))
                content.append(value)
                print("bin x {}, bin y {}, value {} entry {}".format(x_+0.01, y_+0.01, value, len(content)))


        for biny in range(yaxis_med.GetNbins()+1):
            y_ = yaxis_med.GetBinUpEdge(biny)
            if binx == 0 and biny!=0:
                yedges.append(y_)
            if biny!=yaxis_med.GetNbins() and binx!=xaxis.GetNbins():
                value = h_sf_med.GetBinContent(h_sf_med.FindBin(x_+0.01, y_+0.01))
                content.append(value)
                print("bin x {}, bin y {}, value {}, entry {}".format(x_+0.01, y_+0.01, value, len(content)))


        for biny in range(yaxis_high.GetNbins()+1):
            y_ = yaxis_high.GetBinUpEdge(biny)
            if binx == 0 and biny!=0:
                yedges.append(y_)
            if biny!=yaxis_high.GetNbins() and binx!=xaxis.GetNbins():
                value = h_sf_high.GetBinContent(h_sf_high.FindBin(x_+0.01, y_+0.01))
                content.append(value)
                print("bin x {}, bin y {}, value {}, entry {}".format(x_+0.01, y_+0.01, value, len(content)))


    xbins = array('d', xedges)
    ybins = array('d', yedges)
    values = array('d', content)

    h_reco = TH2D('h_reco', 'e /#gamma scale factors', len(xbins)-1, xbins, len(ybins)-1, ybins)
    h_reco.GetXaxis().SetTitle("Super Cluter #eta")
    h_reco.GetYaxis().SetTitle("p_{T} [GeV]")

    for x in range(len(xedges)-1):
        for y in range(len(yedges)-1):
            #print('x {}, y {}, value {}, entry {}'.format(xedges[x]+0.01, yedges[y]+0.01, content[(len(yedges)-1)*x + y], (len(yedges)-1)*x + y))
            h_reco.Fill(xedges[x]+0.01, yedges[y]+0.01, content[(len(yedges)-1)*x + y])

    fout.cd()
    h_reco.Write('EGamma_SF2D')
    fout.Write()

    fout.Close()
    f_low.Close()
    f_med.Close()
    f_high.Close()

if __name__ == "__main__":

    #electronID_sf()
    #electronReco_sf()
    combineRecoPlots()