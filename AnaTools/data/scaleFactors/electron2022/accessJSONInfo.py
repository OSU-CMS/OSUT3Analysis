## example how to read the muon format v2
## (Adapted from JMAR and EGM examples)
from correctionlib import _core
import json
from array import array
from ROOT import TFile, TH2D, TCanvas

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

namesReco = ['RecoBelow20', 'Reco20to75', 'RecoAbove75']

##########
# Electron ID
###########

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

############
# Electron RECO
#############


if __name__ == "__main__":

    electronID_sf()