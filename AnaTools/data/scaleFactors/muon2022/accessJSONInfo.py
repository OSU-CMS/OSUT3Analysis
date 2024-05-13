## example how to read the muon format v2
## (Adapted from JMAR and EGM examples)
from correctionlib import _core

##########################
## Example B: 2022preEE ##
##########################

fname = "muon_Z.json.gz"
if fname.endswith(".json.gz"):
    import gzip
    with gzip.open(fname,'rt') as file:
        data = file.read().strip()
        evaluator = _core.CorrectionSet.from_string(data)
else:
    evaluator = _core.CorrectionSet.from_file(fname)

ptRange = {}
ptRange[17.0] = "pt:[15.00,20.00]"
ptRange[22.0] = "pt:[20.00,25.00]"
ptRange[27.0] = "pt:[25.00,30.00]"
ptRange[35.0] = "pt:[30.00,40.00]"
ptRange[45.0] = "pt:[40.00,50.00]"
ptRange[55.0] = "pt:[50.00,60.00]"
ptRange[90.0] = "pt:[60.00,120.00]"
ptRange[150.0] = "pt:[120.00,Inf]"

etaRange = {}
etaRange[0.4] = "abseta:[0.00,0.90]"
etaRange[1.1] = "abseta:[0.90,1.20]"
etaRange[1.5] = "abseta:[1.20,2.10]"
etaRange[2.2] = "abseta:[2.10,2.40]"

idNames = ["NUM_LooseID_DEN_TrackerMuons","NUM_MediumID_DEN_TrackerMuons","NUM_MediumPromptID_DEN_TrackerMuons","NUM_TightID_DEN_TrackerMuons","NUM_SoftID_DEN_TrackerMuons","NUM_HighPtID_DEN_TrackerMuons","NUM_TrkHighPtID_DEN_TrackerMuons"]
isoNames = ["NUM_LoosePFIso_DEN_LooseID","NUM_LoosePFIso_DEN_MediumID","NUM_LoosePFIso_DEN_MediumPromptID","NUM_LoosePFIso_DEN_TightID","NUM_LooseRelTkIso_DEN_HighPtID","NUM_LooseRelTkIso_DEN_TrkHighPtID","NUM_TightPFIso_DEN_MediumID","NUM_TightPFIso_DEN_MediumPromptID","NUM_TightPFIso_DEN_TightID","NUM_TightRelTkIso_DEN_HighPtID","NUM_TightRelTkIso_DEN_TrkHighPtID","NUM_LooseMiniIso_DEN_LooseID","NUM_LooseMiniIso_DEN_MediumID","NUM_MediumMiniIso_DEN_MediumID","NUM_TightMiniIso_DEN_MediumID"]

new = {}

# for name in idNames:
for name in isoNames:
    typeSF = {}
    abseta_pt = {}
    for eta in etaRange.keys():
        abseta = {}
        for pt in ptRange.keys():
            ptDict = {}
            valsf_nom = evaluator[name].evaluate(eta, pt, "nominal")
            valsf_error = evaluator[name].evaluate(eta, pt, "systup") - valsf_nom
            ptDict["value"] = valsf_nom
            ptDict["error"] = valsf_error
            abseta[ptRange[pt]] = ptDict
        abseta_pt[etaRange[eta]] = abseta
    typeSF["abseta_pt"] = abseta_pt
    new[name] = typeSF

# Medium ID 2022 scale factor using eta as input
valsf_eta = evaluator[name].evaluate(-1.1, 45.0, "nominal")
valsf_eta_error = evaluator[name].evaluate(-1.1, 45.0, "systup")
print("sf for eta = -1.1: " + str(valsf_eta))

import json
json_object = json.dumps(new, indent = 4)

print(json_object)

with open("RunEFG_SF_ISO.json", "w") as outfile:
    outfile.write(json_object)

# Medium ID 2022 scale factor using eta as input ==> Note that this value should be the same
# as the previous one, since even though the input can be signed eta, the SFs for 2022 were
# computed for |eta|. This is valid for ALL the years and jsons
valsf_eta = evaluator[name].evaluate(1.1, 45.0, "nominal")
print("sf for eta = 1.1 " + str(valsf_eta))

# Trigger 2022 systematic uncertainty only
valsyst = evaluator["NUM_IsoMu24_DEN_CutBasedIdMedium_and_PFIsoMedium"].evaluate(-1.8, 54.0, "syst")
print("syst is: " + str(valsyst))
