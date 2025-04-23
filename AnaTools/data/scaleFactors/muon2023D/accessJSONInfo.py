## example how to read the muon format v2
## (Adapted from JMAR and EGM examples)
from correctionlib import _core

##########################
## Example B: 2022preEE ##
##########################

def makeJSON(evaluator,names,ptRange,etaRange,outputJSON):

    # Single mu triggers Mu50_or_CascadeMu100_or_HighPtTkMu100 have distinct ptRange; change accordingly
    ptRangeTrig50 = {}
    ptRangeTrig50[54.0] = "pt:[52.00,56.00]"
    ptRangeTrig50[58.0] = "pt:[56.00,60.00]"
    ptRangeTrig50[90.0] = "pt:[60.00,120.00]"
    ptRangeTrig50[1200.0] = "pt:[120.00,1200.00]"

    # Create new output dictionary
    new = {}

    for name in names:
        if name == "NUM_Mu50_or_CascadeMu100_or_HighPtTkMu100_DEN_CutBasedIdTrkHighPt_and_TkIsoLoose":
            ptRange = ptRangeTrig50
        typeSF = {}
        abseta_pt = {}
        for eta in etaRange.keys():
            abseta = {}
            for pt in ptRange.keys():
                ptDict = {}
                # Use evaluator to extract needed information
                valsf_nom = evaluator[name].evaluate(eta, pt, "nominal")
                valsf_error = evaluator[name].evaluate(eta, pt, "systup") - valsf_nom
                ptDict["value"] = valsf_nom
                ptDict["error"] = valsf_error
                abseta[ptRange[pt]] = ptDict
            abseta_pt[etaRange[eta]] = abseta
        typeSF["abseta_pt"] = abseta_pt
        new[name] = typeSF

    import json
    json_object = json.dumps(new, indent = 4)

    # Write new dictionary to JSON file
    with open(outputJSON, "w") as outfile:
        outfile.write(json_object)

# Get json from compressed file and create evaluator
fname = "muon_Z.json.gz"
if fname.endswith(".json.gz"):
    import gzip
    with gzip.open(fname,'rt') as file:
        data = file.read().strip()
        evaluator = _core.CorrectionSet.from_string(data)
else:
    evaluator = _core.CorrectionSet.from_file(fname)

# Define pt and eta ranges for different SF types
ptRangeIDIso = {}
ptRangeIDIso[17.0] = "pt:[15.00,20.00]"
ptRangeIDIso[22.0] = "pt:[20.00,25.00]"
ptRangeIDIso[27.0] = "pt:[25.00,30.00]"
ptRangeIDIso[35.0] = "pt:[30.00,40.00]"
ptRangeIDIso[45.0] = "pt:[40.00,50.00]"
ptRangeIDIso[55.0] = "pt:[50.00,60.00]"
ptRangeIDIso[90.0] = "pt:[60.00,120.00]"
ptRangeIDIso[150.0] = "pt:[120.00,150.00]"

ptRangeTrig = {}
ptRangeTrig[28.0] = "pt:[26.00,30.00]"
ptRangeTrig[35.0] = "pt:[30.00,40.00]"
ptRangeTrig[45.0] = "pt:[40.00,50.00]"
ptRangeTrig[55.0] = "pt:[50.00,60.00]"
ptRangeTrig[90.0] = "pt:[60.00,120.00]"
ptRangeTrig[1200.0] = "pt:[120.00,1200.00]"

etaRange = {}
etaRange[0.4] = "abseta:[0.00,0.90]"
etaRange[1.1] = "abseta:[0.90,1.20]"
etaRange[1.5] = "abseta:[1.20,2.10]"
etaRange[2.2] = "abseta:[2.10,2.40]"

# Define names of SFs
idNames = ["NUM_LooseID_DEN_TrackerMuons","NUM_MediumID_DEN_TrackerMuons","NUM_MediumPromptID_DEN_TrackerMuons","NUM_TightID_DEN_TrackerMuons","NUM_SoftID_DEN_TrackerMuons","NUM_HighPtID_DEN_TrackerMuons","NUM_TrkHighPtID_DEN_TrackerMuons"]
isoNames = ["NUM_LoosePFIso_DEN_LooseID","NUM_LoosePFIso_DEN_MediumID","NUM_LoosePFIso_DEN_MediumPromptID","NUM_LoosePFIso_DEN_TightID","NUM_LooseRelTkIso_DEN_HighPtID","NUM_LooseRelTkIso_DEN_TrkHighPtID","NUM_TightPFIso_DEN_MediumID","NUM_TightPFIso_DEN_MediumPromptID","NUM_TightPFIso_DEN_TightID","NUM_TightRelTkIso_DEN_HighPtID","NUM_TightRelTkIso_DEN_TrkHighPtID","NUM_LooseMiniIso_DEN_LooseID","NUM_LooseMiniIso_DEN_MediumID","NUM_MediumMiniIso_DEN_MediumID","NUM_TightMiniIso_DEN_MediumID"]
trigNames = ["NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight","NUM_Mu50_or_CascadeMu100_or_HighPtTkMu100_DEN_CutBasedIdTrkHighPt_and_TkIsoLoose"]

# Create separate JSON files for each type
makeJSON(evaluator,idNames,ptRangeIDIso,etaRange,"RunD_SF_ID.json") # Creating ID JSON
makeJSON(evaluator,isoNames,ptRangeIDIso,etaRange,"RunD_SF_ISO.json") # Creating ISO JSON
makeJSON(evaluator,trigNames,ptRangeTrig,etaRange,"RunD_SF_Trig.json") # Creating trigger JSON