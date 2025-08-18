#! /usr/bin/env python
# Example of how to read the JME-JERC JSON files
# For more information, see the README in
# https://gitlab.cern.ch/cms-nanoAOD/jsonpog-integration/-/tree/master/POG/JME
# For a comparison to the CMSSW-syntax refer to
# https://github.com/cms-jet/JECDatabase/blob/master/scripts/JERC2JSON/minimalDemo.py

import os
import correctionlib._core as core
import array
import numpy as np

from ROOT import TFile, gROOT, gStyle, TCanvas, TLegend, TPad, TLine, TH2D, TH3D

gROOT.SetBatch()
gROOT.ProcessLine( "gErrorIgnoreLevel = 6001;")
gROOT.ProcessLine( "gPrintViaErrorHandler = kTRUE;")
gStyle.SetOptStat(0)

# path to directory of this script
__this_dir__ = os.path.dirname(__file__)

#
# helper functions
#

def get_corr_inputs(input_dict, corr_obj):
    """
    Helper function for getting values of input variables
    given a dictionary and a correction object.
    """
    input_values = [input_dict[inp.name] for inp in corr_obj.inputs]
    return input_values


#
# values of input variables
#

example_value_dict = {
    # jet transverse momentum
    "JetPt": 100.0,
    # jet pseudorapidity
    "JetEta": 0.0,
    # jet azimuthal angle
    "JetPhi": 0.2,
    # jet area
    "JetA": 0.5,
    # median energy density (pileup)
    "Rho": 15.0,
    # systematic variation (only for JER SF)
    "systematic": "nom",
    # pT of matched gen-level jet (only for JER smearing)
    "GenPt": 80.0,  # or -1 if no match
    # unique event ID used for deterministic
    # pseudorandom number generation (only for JER smearing)
    "EventID": 12345,
    # Run number for Summer23
    "run" : 367765.0,
}


#
# JEC-related examples
#

filePeriods = ["Summer22","Summer22EE","Summer23","Summer23BPix"]
# filePeriods = ["Summer22"] # For testing

for filePeriod in filePeriods:

    # Run name
    runs = []
    if filePeriod == "Summer22": runs = ["_RunCD"]
    if filePeriod == "Summer22EE": runs = ["_RunE","_RunF","_RunG"]
    if filePeriod == "Summer23": runs = [""]
    if filePeriod == "Summer23BPix": runs = [""]

    # initial name
    initName = ""
    if filePeriod == "Summer22": initName = "Summer22_22Sep2023"
    if filePeriod == "Summer22EE": initName = "Summer22EE_22Sep2023"
    if filePeriod == "Summer23": initName = "Summer23Prompt23"
    if filePeriod == "Summer23BPix": initName = "Summer23BPixPrompt23"

    # Run numbers for Summer23
    if filePeriod == "Summer23": example_value_dict["run"] = 367765.0
    if filePeriod == "Summer23BPix": example_value_dict["run"] = 369803.0

    # Threshold for getting pt bin edges
    threshold = 0.005

    # Output file name
    h2dFile = TFile.Open(filePeriod + "_AK4PFPuppi.root","RECREATE")

    # jet algorithms
    algo = "AK4PFPuppi"

    # jet energy correction level
    lvl = "L2Relative"

    # data or MC
    datatype = "DATA"

    # jet energy uncertainty
    unc = "Total"

    # Version of jerc script
    version = "V3"
    if filePeriod != "Summer23BPix": version = "V2"
    print(version)

    #
    # load JSON files using correctionlib
    #

    # AK4
    fname = os.path.join(__this_dir__, filePeriod + "_jet_jerc.json.gz")
    print("\nLoading JSON file: {}".format(fname))
    cset = core.CorrectionSet.from_file(os.path.join(fname))

    # tool for JER smearing
    fname_jersmear = os.path.join(__this_dir__, filePeriod + "_jet_jerc.json.gz")
    print("\nLoading JSON file: {}".format(fname_jersmear))
    cset_jersmear = core.CorrectionSet.from_file(os.path.join(fname_jersmear))



    print("\n\nGetting MC_L2Relative_AK4PFPuppi correction\n===================")

    # Define variable bin edges for the x-axis
    # x_edges = range(1,7001,1)
    x_edges = [1, 9, 10, 11, 12, 13, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 79, 82, 85, 88, 91, 94, 97, 100, 103, 107, 111, 115, 119, 123, 127, 132, 137, 142, 147, 153, 159, 165, 171, 178, 185, 192, 200, 208, 217, 226, 236, 246, 257, 268, 280, 293, 306, 320, 335, 351, 368, 386, 405, 425, 447, 470, 495, 522, 550, 580, 7000]
    x_bins = array.array('d', x_edges)
    num_x_bins = len(x_bins) - 1

    # Define variable bin edges for the y-axis
    y_edges = []

    y_edges = [-5.191, -4.889, -4.716, -4.538, -4.363, -4.191, -4.013, -3.839, -3.664, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.83, -1.74, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087, 0.0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191, 4.363, 4.538, 4.716, 4.889, 5.191]
    y_bins = array.array('d', y_edges)
    num_y_bins = len(y_bins) - 1

    # JEC base tag
    jec = initName + "_" + version

    datatype = "MC"

    key = "{}_{}_{}_{}".format(jec, datatype, lvl, algo)
    print("JSON access to keys: '{}'".format(key))
    sf = cset[key]

    sf_input_names = [inp.name for inp in sf.inputs]
    print("Inputs: " + ", ".join(sf_input_names))

    h2d = TH2D("my_2d_histogram", "JEC_" + initName + "_" + version + "_MC_L2Relative_AK4PFPuppi;p_{T} [GeV];#eta", num_x_bins, x_bins, num_y_bins, y_bins)

    saveLen = 0
    saveEta = 0.0
    saveFinalEdgeList = []

    for i in range(len(y_edges) - 1):
        saveScaleArray = []
        saveScale = 0.0
        saveEdgeList = []
        saveEdge = 0.0
        for j in range(len(x_edges) - 1):
            example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
            example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
            inputs = get_corr_inputs(example_value_dict, sf)
            if saveEdge != x_edges[j] and abs(saveScale - sf.evaluate(*inputs)) > threshold:
                saveEdge = x_edges[j]
                saveEdgeList.append(saveEdge)
                saveScale = sf.evaluate(*inputs)
                saveScaleArray.append(sf.evaluate(*inputs))
            h2d.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0, sf.evaluate(*inputs))
        if len(saveEdgeList) > len(saveFinalEdgeList): saveFinalEdgeList = saveEdgeList
        if len(saveEdgeList) > saveLen:
            saveLen = len(saveEdgeList)
            saveEta = y_edges[i]

    saveFinalEdgeList.append(7000) # Assing last egde
    print("Final pt edges list: ",saveFinalEdgeList)
    print("Final pt edges list length: ",saveLen)
    print("Final eta for the selected pt edges list: ",saveEta)


    h2d.Draw("COLZ")  # "COLZ" draws a color palette indicating bin content
    h2d.SetName("JECL2MC_" + filePeriod + "_AK4PFPuppi")
    h2d.Write()



    print("\n\nGetting MC_L3Absolute_AK4PFPuppi correction\n===================")

    # Define variable bin edges for the x-axis
    # x_edges = range(1,7001,1)
    x_edges = [1, 7000]
    x_bins = array.array('d', x_edges)
    num_x_bins = len(x_bins) - 1

    # Define variable bin edges for the y-axis
    y_edges = []

    y_edges = [-5.191, 5.191]
    y_bins = array.array('d', y_edges)
    num_y_bins = len(y_bins) - 1

    # JEC base tag
    jec = initName + "_" + version

    # print input information
    print("\n\nJEC parameters")
    print("##############\n")

    print("jec = {}".format(jec))
    print("algo = {}".format(algo))
    for v in ("JetPt", "JetEta", "JetA", "JetPhi", "JetA", "Rho", "run"):
        print("{} = {}".format(v, example_value_dict[v]))

    datatype = "MC"
    lvl = "L3Absolute"

    key = "{}_{}_{}_{}".format(jec, datatype, lvl, algo)
    print("JSON access to keys: '{}'".format(key))
    sf = cset[key]

    sf_input_names = [inp.name for inp in sf.inputs]
    print("Inputs: " + ", ".join(sf_input_names))

    h2d = TH2D("my_2d_histogram", "JEC_" + initName + "_" + version + "_MC_L3Absolute_AK4PFPuppi;p_{T} [GeV];#eta", num_x_bins, x_bins, num_y_bins, y_bins)

    saveLen = 0
    saveEta = 0.0
    saveFinalEdgeList = []

    for i in range(len(y_edges) - 1):
        saveScaleArray = []
        saveScale = 0.0
        saveEdgeList = []
        saveEdge = 0.0
        for j in range(len(x_edges) - 1):
            example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
            example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
            inputs = get_corr_inputs(example_value_dict, sf)
            if saveEdge != x_edges[j] and abs(saveScale - sf.evaluate(*inputs)) > threshold:
                saveEdge = x_edges[j]
                saveEdgeList.append(saveEdge)
                saveScale = sf.evaluate(*inputs)
                saveScaleArray.append(sf.evaluate(*inputs))
            h2d.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0, sf.evaluate(*inputs))
        if len(saveEdgeList) > len(saveFinalEdgeList): saveFinalEdgeList = saveEdgeList
        if len(saveEdgeList) > saveLen:
            saveLen = len(saveEdgeList)
            saveEta = y_edges[i]

    saveFinalEdgeList.append(7000) # Assing last egde
    print("Final pt edges list: ",saveFinalEdgeList)
    print("Final pt edges list length: ",saveLen)
    print("Final eta for the selected pt edges list: ",saveEta)


    h2d.Draw("COLZ")  # "COLZ" draws a color palette indicating bin content
    h2d.SetName("JECL3MC_" + filePeriod + "_AK4PFPuppi")
    h2d.Write()




    print("\n\nGetting DATA_L2Relative_AK4PFPuppi correction\n===================")

    # Define variable bin edges for the x-axis
    # x_edges = range(1,7001,1)
    x_edges = [1, 9, 10, 11, 12, 13, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 79, 82, 85, 88, 91, 94, 97, 100, 103, 107, 111, 115, 119, 123, 127, 132, 137, 142, 147, 153, 159, 165, 171, 178, 185, 192, 200, 208, 217, 226, 236, 246, 257, 268, 280, 293, 306, 320, 335, 351, 368, 386, 405, 425, 447, 470, 495, 522, 550, 580, 7000]
    x_bins = array.array('d', x_edges)
    num_x_bins = len(x_bins) - 1

    # Define variable bin edges for the y-axis
    y_edges = []

    y_edges = [-5.191, -4.889, -4.716, -4.538, -4.363, -4.191, -4.013, -3.839, -3.664, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.83, -1.74, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087, 0.0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191, 4.363, 4.538, 4.716, 4.889, 5.191]
    y_bins = array.array('d', y_edges)
    num_y_bins = len(y_bins) - 1

    for run in runs:

        # JEC base tag
        jec = initName + run + "_" + version

        # print input information
        print("\n\nJEC parameters")
        print("##############\n")

        print("jec = {}".format(jec))
        print("algo = {}".format(algo))
        for v in ("JetPt", "JetEta", "JetA", "JetPhi", "JetA", "Rho", "run"):
            print("{} = {}".format(v, example_value_dict[v]))

        datatype = "DATA"
        lvl = "L2Relative"

        key = "{}_{}_{}_{}".format(jec, datatype, lvl, algo)
        print("JSON access to keys: '{}'".format(key))
        sf = cset[key]

        sf_input_names = [inp.name for inp in sf.inputs]
        print("Inputs: " + ", ".join(sf_input_names))

        h2d = TH2D("my_2d_histogram", "JEC_" + initName + run + "_" + version + "_DATA_L2Relative_AK4PFPuppi;p_{T} [GeV];#eta", num_x_bins, x_bins, num_y_bins, y_bins)

        saveLen = 0
        saveEta = 0.0
        saveFinalEdgeList = []

        for i in range(len(y_edges) - 1):
            saveScaleArray = []
            saveScale = 0.0
            saveEdgeList = []
            saveEdge = 0.0
            for j in range(len(x_edges) - 1):
                example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
                example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
                inputs = get_corr_inputs(example_value_dict, sf)
                if saveEdge != x_edges[j] and abs(saveScale - sf.evaluate(*inputs)) > threshold:
                    saveEdge = x_edges[j]
                    saveEdgeList.append(saveEdge)
                    saveScale = sf.evaluate(*inputs)
                    saveScaleArray.append(sf.evaluate(*inputs))
                h2d.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0, sf.evaluate(*inputs))
            if len(saveEdgeList) > len(saveFinalEdgeList): saveFinalEdgeList = saveEdgeList
            if len(saveEdgeList) > saveLen:
                saveLen = len(saveEdgeList)
                saveEta = y_edges[i]

        saveFinalEdgeList.append(7000) # Assing last egde
        print("Final pt edges list: ",saveFinalEdgeList)
        print("Final pt edges list length: ",saveLen)
        print("Final eta for the selected pt edges list: ",saveEta)


        h2d.Draw("COLZ")  # "COLZ" draws a color palette indicating bin content
        h2d.SetName("JECL2DATA_" + filePeriod + run + "_AK4PFPuppi")
        h2d.Write()



    print("\n\nGetting DATA_L3Absolute_AK4PFPuppi correction\n===================")

    # Define variable bin edges for the x-axis
    # x_edges = range(1,7001,1)
    x_edges = [1, 7000]
    x_bins = array.array('d', x_edges)
    num_x_bins = len(x_bins) - 1

    # Define variable bin edges for the y-axis
    y_edges = []

    y_edges = [-5.191, 5.191]
    y_bins = array.array('d', y_edges)
    num_y_bins = len(y_bins) - 1

    for run in runs:

        # JEC base tag
        jec = initName + run + "_" + version

        datatype = "DATA"
        lvl = "L3Absolute"

        key = "{}_{}_{}_{}".format(jec, datatype, lvl, algo)
        print("JSON access to keys: '{}'".format(key))
        sf = cset[key]

        sf_input_names = [inp.name for inp in sf.inputs]
        print("Inputs: " + ", ".join(sf_input_names))

        h2d = TH2D("my_2d_histogram", "JEC_" + initName + run + "_" + version + "_DATA_L3Absolute_AK4PFPuppi;p_{T} [GeV];#eta", num_x_bins, x_bins, num_y_bins, y_bins)

        saveLen = 0
        saveEta = 0.0
        saveFinalEdgeList = []

        for i in range(len(y_edges) - 1):
            saveScaleArray = []
            saveScale = 0.0
            saveEdgeList = []
            saveEdge = 0.0
            for j in range(len(x_edges) - 1):
                example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
                example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
                inputs = get_corr_inputs(example_value_dict, sf)
                if saveEdge != x_edges[j] and abs(saveScale - sf.evaluate(*inputs)) > threshold:
                    saveEdge = x_edges[j]
                    saveEdgeList.append(saveEdge)
                    saveScale = sf.evaluate(*inputs)
                    saveScaleArray.append(sf.evaluate(*inputs))
                h2d.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0, sf.evaluate(*inputs))
            if len(saveEdgeList) > len(saveFinalEdgeList): saveFinalEdgeList = saveEdgeList
            if len(saveEdgeList) > saveLen:
                saveLen = len(saveEdgeList)
                saveEta = y_edges[i]

        saveFinalEdgeList.append(7000) # Assing last egde
        print("Final pt edges list: ",saveFinalEdgeList)
        print("Final pt edges list length: ",saveLen)
        print("Final eta for the selected pt edges list: ",saveEta)


        h2d.Draw("COLZ")  # "COLZ" draws a color palette indicating bin content
        h2d.SetName("JECL3DATA_" + filePeriod + run + "_AK4PFPuppi")
        h2d.Write()



    datatype = "DATA"
    lvl = "L2L3Residual"

    print("\n\nGetting DATA_L2L3Residual_AK4PFPuppi correction\n===================")

    # Define variable bin edges for the x-axis
    # x_edges = range(1,7001,1)
    x_edges = [1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 72, 78, 86, 96, 111, 138, 647, 847, 1002, 1139, 1266, 1386, 1501, 1612, 1720, 1826, 1929, 2031, 2131, 2229, 2326, 2422, 2517, 2611, 2704, 2796, 2887, 2977, 3066, 3154, 3242, 3329, 3415, 3501, 3586, 3670, 3754, 3837, 3920, 4002, 4083, 4164, 4244, 4324, 4403, 4482, 4560, 4638, 4715, 4792, 4869, 4945, 5021, 5096, 5171, 5245, 5319, 5392, 5465, 5538, 5610, 5682, 5753, 5824, 5895, 5965, 6035, 6105, 6174, 6243, 6311, 6379, 6447, 7000]
    x_bins = array.array('d', x_edges)
    num_x_bins = len(x_bins) - 1

    # Define variable bin edges for the y-axis
    y_edges = []

    if filePeriod == "Summer22": y_edges = [-5.191, -4.583, -4.013, -3.839, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -1.93, -1.653, -1.479, -1.305, -1.044, -0.783, -0.522, -0.261, 0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191] # Summer22
    if filePeriod == "Summer22EE": y_edges = [-5.191, -4.583, -4.013, -3.839, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -1.93, -1.653, -1.479, -1.305, -1.044, -0.783, -0.522, -0.261, 0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191] # Summer22EE
    if filePeriod == "Summer23": y_edges = [-5.191, -4.583, -4.013, -3.839, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -1.93, -1.653, -1.479, -1.305, -1.044, -0.783, -0.522, -0.261, -0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191] # Summer23
    if filePeriod == "Summer23BPix": y_edges = [-5.191, -4.583, -4.013, -3.839, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -1.93, -1.653, -1.479, -1.305, -1.044, -0.783, -0.522, -0.261, -0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191] # Summer23BPix
    y_bins = array.array('d', y_edges)
    num_y_bins = len(y_bins) - 1

    for run in runs:

        # JEC base tag
        jec = initName + run + "_" + version

        key = "{}_{}_{}_{}".format(jec, datatype, lvl, algo)
        print("JSON access to keys: '{}'".format(key))
        sf = cset[key]

        sf_input_names = [inp.name for inp in sf.inputs]
        print("Inputs: " + ", ".join(sf_input_names))

        h2d = TH2D("my_2d_histogram" + run, "JEC_" + initName + run + "_" + version + "_DATA_L2L3Residual_AK4PFPuppi;p_{T} [GeV];#eta", num_x_bins, x_bins, num_y_bins, y_bins)

        saveLen = 0
        saveEta = 0.0
        saveFinalEdgeList = []

        for i in range(len(y_edges) - 1):
            saveScaleArray = []
            saveScale = 0.0
            saveEdgeList = []
            saveEdge = 0.0
            for j in range(len(x_edges) - 1):
                example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
                example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
                inputs = get_corr_inputs(example_value_dict, sf)
                if saveEdge != x_edges[j] and abs(saveScale - sf.evaluate(*inputs)) > threshold:
                    saveEdge = x_edges[j]
                    saveEdgeList.append(saveEdge)
                    saveScale = sf.evaluate(*inputs)
                    saveScaleArray.append(sf.evaluate(*inputs))

                h2d.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0,sf.evaluate(*inputs))
            if len(saveEdgeList) > len(saveFinalEdgeList): saveFinalEdgeList = saveEdgeList
            if len(saveEdgeList) > saveLen:
                saveLen = len(saveEdgeList)
                saveEta = y_edges[i]

        saveFinalEdgeList.append(7000) # Assing last egde
        print("Final pt edges list: ",saveFinalEdgeList)
        print("Final pt edges list length: ",saveLen)
        print("Final eta for the selected pt edges list: ",saveEta)

        h2d.Draw("COLZ")  # "COLZ" draws a color palette indicating bin content
        h2d.SetName("JECL2L3DATA_" + filePeriod + run + "_AK4PFPuppi")
        h2d.Write()



    print("\n\nGetting MC_Total_AK4PFPuppi correction\n===================")

    jec = initName + "_" + version
    datatype = "MC"

    # Define variable bin edges for the x-axis
    x_edges = [9.0, 11.0, 13.5, 16.5, 19.5, 22.5, 26.0, 30.0, 34.5, 40.0, 46.0, 52.5, 60.0, 69.0, 79.0, 90.5, 105.5, 123.5, 143.0, 163.5, 185.0, 208.0, 232.5, 258.5, 286.0, 331.0, 396.0, 468.5, 549.5, 639.0, 738.0, 847.5, 968.5, 1102.0, 1249.5, 1412.0, 1590.5, 1787.0, 2003.0, 2241.0, 2503.0, 2790.5, 3107.0, 3455.0, 3837.0, 4257.0, 4719.0, 5226.5, 5784.0, 6538.0]
    x_bins = array.array('d', x_edges)
    num_x_bins = len(x_bins) - 1

    # Define variable bin edges for the y-axis
    y_edges = [-5.4, -5.0, -4.4, -4.0, -3.5, -3.0, -2.8, -2.6, -2.4, -2.2, -2.0, -1.8, -1.6, -1.4, -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.5, 4.0, 4.4, 5.0, 5.4]
    y_bins = array.array('d', y_edges)
    num_y_bins = len(y_bins) - 1

    h2d2 = TH2D("my_2d_histogram", "JECUnc_" + initName + "_" + version + "_MC_Total_AK4PFPuppi;p_{T} [GeV];#eta", num_x_bins, x_bins, num_y_bins, y_bins)

    key = "{}_{}_{}_{}".format(jec, datatype, unc, algo)
    print("JSON access to key: '{}'".format(key))
    sf = cset[key]

    sf_input_names = [inp.name for inp in sf.inputs]
    print("Inputs: " + ", ".join(sf_input_names))

    inputs = get_corr_inputs(example_value_dict, sf)
    print("JSON result: {}".format(sf.evaluate(*inputs)))

    for j in range(len(x_edges) - 1):
        for i in range(len(y_edges) - 1):
            example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
            example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
            inputs = get_corr_inputs(example_value_dict, sf)
            h2d2.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0,sf.evaluate(*inputs))


    h2d2.Draw("COLZ")  # "COLZ" draws a color palette indicating bin content
    h2d2.SetName("JECUnc_" + filePeriod + "_AK4PFPuppi")
    h2d2.Write()



    # Complement for 2023
    complement = ""
    if filePeriod == "Summer23": complement = "_RunCv1234"
    if filePeriod == "Summer23BPix": complement = "_RunD"

    # # JER base tag
    jer = initName + complement + "_JRV1_MC"

    # # algorithms
    algo = "AK4PFPuppi"


    print("\n\nGetting JRV1_MC_ScaleFactor_AK4PFPuppi correction\n===================")

    # Define variable bin edges for the x-axis
    x_edges = []

    if filePeriod == "Summer22": x_edges = [10.0, 121.0, 140.0, 150.0, 191.0, 220.0, 249.0, 272.0, 352.0, 4000.0]
    if filePeriod == "Summer22EE": x_edges = [10.0, 121.0, 140.0, 150.0, 191.0, 219.0, 249.0, 272.0, 352.0, 4000.0]
    if filePeriod == "Summer23": x_edges = [10.0, 15.0, 21.0, 28.0, 37.0, 49.0, 59.0, 86.0, 110.0, 132.0, 170.0, 204.0, 236.0, 279.0, 302.0, 373.0, 460.0, 575.0, 638.0, 737.0, 846.0, 967.0, 1101.0, 1248.0, 1410.0, 1588.0, 1784.0, 2000.0, 2238.0, 2500.0, 2787.0, 3103.0, 3450.0, 4037.0, 5220.0, 5373.0]
    if filePeriod == "Summer23BPix": x_edges = [10.0, 15.0, 21.0, 28.0, 37.0, 49.0, 59.0, 86.0, 110.0, 132.0, 170.0, 204.0, 236.0, 279.0, 302.0, 373.0, 460.0, 575.0, 638.0, 737.0, 846.0, 967.0, 1101.0, 1248.0, 1410.0, 1588.0, 1784.0, 2000.0, 2238.0, 2500.0, 2787.0, 3103.0, 3450.0, 4037.0, 5220.0, 5373.0]
    x_bins = array.array('d', x_edges)
    num_x_bins = len(x_bins) - 1

    # Define variable bin edges for the y-axis
    y_edges = []

    if filePeriod == "Summer22": y_edges = [-5.191, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.74, -1.566, -1.305, -1.044, -0.783, -0.522, -0.261, -0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.566, 1.74, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 5.191]
    if filePeriod == "Summer22EE": y_edges = [-5.191, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.74, -1.566, -1.305, -1.044, -0.783, -0.522, -0.261, -0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.566, 1.74, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 5.191]
    if filePeriod == "Summer23": y_edges = [-5.191, -4.583, -4.013, -3.839, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -1.93, -1.74, -1.566, -1.305, -1.044, -0.783, -0.522, -0.261, -0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.566, 1.74, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191]
    if filePeriod == "Summer23BPix": y_edges = [-5.191, -4.583, -4.013, -3.839, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -1.93, -1.74, -1.566, -1.305, -1.044, -0.783, -0.522, -0.261, -0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.566, 1.74, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191]
    y_bins = array.array('d', y_edges)
    num_y_bins = len(y_bins) - 1

    h2d3 = TH2D("my_2d_histogram", "JERSFNom_" + initName + complement + "_JRV1_MC_ScaleFactor_AK4PFPuppi;p_{T} [GeV];#eta", num_x_bins, x_bins, num_y_bins, y_bins)
    h2d4 = TH2D("my_2d_histogram", "JERSFUp_" + initName + complement + "_JRV1_MC_ScaleFactor_AK4PFPuppi;p_{T} [GeV];#eta", num_x_bins, x_bins, num_y_bins, y_bins)
    h2d5 = TH2D("my_2d_histogram", "JERSFDown_" + initName + complement + "_JRV1_MC_ScaleFactor_AK4PFPuppi;p_{T} [GeV];#eta", num_x_bins, x_bins, num_y_bins, y_bins)

    key = "{}_{}_{}".format(jer, "ScaleFactor", algo)
    print("JSON access to key: '{}'".format(key))
    sf = cset[key]

    sf_input_names = [inp.name for inp in sf.inputs]
    print("Inputs: " + ", ".join(sf_input_names))

    inputs = get_corr_inputs(example_value_dict, sf)
    jersf_value = sf.evaluate(*inputs)
    print("JSON result: {}".format(jersf_value))

    example_value_dict["systematic"] = "nom"

    for j in range(len(x_edges) - 1):
        for i in range(len(y_edges) - 1):
            example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
            example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
            inputs = get_corr_inputs(example_value_dict, sf)
            h2d3.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0,sf.evaluate(*inputs))

    example_value_dict["systematic"] = "up"

    for j in range(len(x_edges) - 1):
        for i in range(len(y_edges) - 1):
            example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
            example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
            inputs = get_corr_inputs(example_value_dict, sf)
            h2d4.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0,sf.evaluate(*inputs))

    example_value_dict["systematic"] = "down"

    for j in range(len(x_edges) - 1):
        for i in range(len(y_edges) - 1):
            example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
            example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
            inputs = get_corr_inputs(example_value_dict, sf)
            h2d5.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0,sf.evaluate(*inputs))


    h2d3.Draw("COLZ")  # "COLZ" draws a color palette indicating bin content
    h2d3.SetName("JERSFNom_" + filePeriod + "_AK4PFPuppi")
    h2d4.Draw("COLZ")  # "COLZ" draws a color palette indicating bin content
    h2d4.SetName("JERSFUp_" + filePeriod + "_AK4PFPuppi")
    h2d5.Draw("COLZ")  # "COLZ" draws a color palette indicating bin content
    h2d5.SetName("JERSFDown_" + filePeriod + "_AK4PFPuppi")
    h2d3.Write()
    h2d4.Write()
    h2d5.Write()



    print("\n\nGetting JRV1_MC_PtResolution_AK4PFPuppi correction\n===================")

    # Define variable bin edges for the x-axis
    # x_edges = range(1,7001,1)
    x_edges = [1, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 65, 68, 71, 74, 77, 81, 85, 89, 94, 99, 105, 111, 118, 126, 135, 145, 157, 170, 185, 203, 224, 249, 279, 315, 359, 414, 483, 570, 683, 831, 1029, 1300, 1681, 2233, 7000]
    x_bins = array.array('d', x_edges)
    num_x_bins = len(x_bins) - 1

    # Define variable bin edges for the y-axis
    y_edges = [-5.191, -3.839, -3.489, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.74, -1.566, -1.305, -1.044, -0.783, -0.522, -0.261, 0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.566, 1.74, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.489, 3.839, 5.191]
    y_bins = array.array('d', y_edges)
    num_y_bins = len(y_bins) - 1

    # Define variable bin edges for the y-axis
    z_edges = []
    if filePeriod == "Summer22": z_edges = [0.0, 8.06, 14.6, 21.13, 27.67, 34.21, 40.74, 47.28, 53.82, 60.35]
    if filePeriod == "Summer22EE": z_edges = [0.0, 8.2, 14.76, 21.33, 27.89, 34.46, 41.02, 47.58, 54.15, 60.71]
    if filePeriod == "Summer23": z_edges = [0.0, 8.24, 14.52, 20.8, 27.08, 33.36, 39.64, 45.92, 52.2]
    if filePeriod == "Summer23BPix": z_edges = [0.0, 8.23, 14.49, 20.75, 27.01, 33.27, 39.53, 45.79, 52.05]
    z_bins = array.array('d', z_edges)
    num_z_bins = len(z_bins) - 1

    h3d = TH3D("my_3d_histogram", "JERPtRes_" + initName + complement + "_JRV1_MC_PtResolution_AK4PFPuppi;p_{T} [GeV];#eta;#rho [GeV]", num_x_bins, x_bins, num_y_bins, y_bins, num_z_bins, z_bins)

    key = "{}_{}_{}".format(jer, "PtResolution", algo)
    print("JSON access to key: '{}'".format(key))
    sf = cset[key]

    sf_input_names = [inp.name for inp in sf.inputs]
    print("Inputs: " + ", ".join(sf_input_names))

    inputs = get_corr_inputs(example_value_dict, sf)
    jer_value = sf.evaluate(*inputs)
    print("JSON result: {}".format(jer_value))

    saveLen = 0
    saveRho = 0.0
    saveEta = 0.0
    saveFinalEdgeList = []

    for k in range(len(z_edges) - 1):
        for i in range(len(y_edges) - 1):
            saveScaleArray = []
            saveScale = 0.0
            saveEdgeList = []
            saveEdge = 0.0
            for j in range(len(x_edges) - 1):

                example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
                example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
                example_value_dict["Rho"] = (z_edges[k] + z_edges[k+1]) / 2.0
                inputs = get_corr_inputs(example_value_dict, sf)
                if saveEdge != x_edges[j] and abs(saveScale - sf.evaluate(*inputs)) > threshold:
                    saveEdge = x_edges[j]
                    saveEdgeList.append(saveEdge)
                    saveScale = sf.evaluate(*inputs)
                    saveScaleArray.append(sf.evaluate(*inputs))
                h3d.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0,(z_edges[k] + z_edges[k+1]) / 2.0,sf.evaluate(*inputs))
            if len(saveEdgeList) > len(saveFinalEdgeList): saveFinalEdgeList = saveEdgeList
            if len(saveEdgeList) > saveLen:
                saveLen = len(saveEdgeList)
                saveEta = y_edges[i]

    saveFinalEdgeList.append(7000) # Assing last egde
    print("Final pt edges list: ",saveFinalEdgeList)
    print("Final pt edges list length: ",saveLen)
    print("Final eta for the selected pt edges list: ",saveEta)
    print("Final rho for the selected pt edges list: ",saveRho)

    h3d.SetName("JERPtRes_" + filePeriod + "_AK4PFPuppi")
    h3d.Write()
    h2dFile.Close()