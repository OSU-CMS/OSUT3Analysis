import json
from array import array
from ROOT import TFile, TH2D

# https://twiki.cern.ch/twiki/bin/view/CMS/MuonRun32022

files = ['Run2024_SF_Trig.json']

# each contains keys like NUM_<id>_DEN_genTracks, NUM_<iso>_DEN_<id>

for file in files:
	output = TFile(file + '.root', 'RECREATE')

	era = file.split('_')[0][3:]
	sfType = file.split('_')[-1][:-5]
	if sfType == 'ISO':
		sfType = sfType.title() # 'Iso'

	f = open(file, 'r')
	results = json.load(f)

	for r in results.keys():
		xbinNames = [] # pt
		ybinNames = [] # abseta
		values = {}
		errors = {}

		etaLabel = 'abseta_pt' if 'abseta_pt' in results[r] else 'abseta_pair_newTuneP_probe_pt'
		xLabelPosition = int(4) if 'abseta_pt' in results[r] else int(24)

		for ybin in results[r][etaLabel]:
			ybinNames.append(ybin)
			for xbin in results[r][etaLabel][ybin]:
				xbinNames.append(xbin)
				values[xbin, ybin] = results[r][etaLabel][ybin][xbin]['value']
				errors[xbin, ybin] = results[r][etaLabel][ybin][xbin]['error']

		# bins are stored as strings like 'pt:[20.00,25.00]', so figure out real values here

		xbinEdges = []
		for x in xbinNames:
			edge1 = float(x.split(',')[0][xLabelPosition:])
			edge2 = float(x.split(',')[1][:-1])
			if not edge1 in xbinEdges:
				xbinEdges.append(edge1)
			if not edge2 in xbinEdges:
				xbinEdges.append(edge2)
		xbinEdges.sort()

		ybinEdges = []
		for y in ybinNames:
			edge1 = float(y.split(',')[0][8:])
			edge2 = float(y.split(',')[1][:-1])
			if not edge1 in ybinEdges:
				ybinEdges.append(edge1)
			if not edge2 in ybinEdges:
				ybinEdges.append(edge2)
		ybinEdges.sort()

		h = TH2D(r, r, len(xbinEdges)-1, array('f', xbinEdges), len(ybinEdges)-1, array('f', ybinEdges))

		values = {}
		errors = {}

		for ybin in results[r][etaLabel]:
			yval = float(ybin.split(',')[0][8:])
			for xbin in results[r][etaLabel][ybin]:
				xval = float(xbin.split(',')[0][xLabelPosition:])

				ibin = h.FindBin(xval + 1.0, yval + 0.01)

				h.SetBinContent(ibin, results[r][etaLabel][ybin][xbin]['value'])
				h.SetBinError(ibin, results[r][etaLabel][ybin][xbin]['error'])

		h.GetZaxis().SetRangeUser(0.7, 1.1)
		h.GetXaxis().SetTitle('muon pt [GeV]')
		h.GetYaxis().SetTitle('|#eta|')

		output.cd()
		h.Write()

	output.Close()
