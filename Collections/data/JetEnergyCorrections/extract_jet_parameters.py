#! /usr/bin/env python
# Example of how to read the JME-JERC JSON files
# For more information, see the README in
# https://gitlab.cern.ch/cms-nanoAOD/jsonpog-integration/-/tree/master/POG/JME
# For a comparison to the CMSSW-syntax refer to
# https://github.com/cms-jet/JECDatabase/blob/master/scripts/JERC2JSON/minimalDemo.py

import os
import correctionlib._core as core
import array

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

    # Output file name
    h2dFile = TFile.Open(filePeriod + "_AK4PFPuppi.root","RECREATE")

    # jet algorithms
    algo = "AK4PFPuppi"

    # jet energy correction level
    lvl = "L2L3Residual"

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


    #
    # example 0: getting a single JEC level
    #
    # Preparing for MC truth correction

    print("\n\nExample 0: single JEC level\n===================")

    # Define variable bin edges for the x-axis
    x_edges = range(10,7001,1)
    x_bins = array.array('d', x_edges)
    num_x_bins = len(x_bins) - 1

    # Define variable bin edges for the y-axis
    y_edges = []

    if filePeriod == "Summer22": y_edges = [-5.191, -4.889, -4.716, -4.538, -4.363, -4.191, -4.013, -3.839, -3.664, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.83, -1.74, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087, 0.0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191, 4.363, 4.538, 4.716, 4.889, 5.191] # Summer22
    if filePeriod == "Summer22EE": y_edges = [-5.191, -4.583, -4.013, -3.839, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -1.93, -1.653, -1.479, -1.305, -1.044, -0.783, -0.522, -0.261, 0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191] # Summer22EE
    if filePeriod == "Summer23": y_edges = [-5.191, -4.583, -4.013, -3.839, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -1.93, -1.653, -1.479, -1.305, -1.044, -0.783, -0.522, -0.261, -0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191] # Summer23
    if filePeriod == "Summer23BPix": y_edges = [-5.191, -4.583, -4.013, -3.839, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -1.93, -1.653, -1.479, -1.305, -1.044, -0.783, -0.522, -0.261, -0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191] # Summer23BPix
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

        key = "{}_{}_{}_{}".format(jec, datatype, lvl, algo)
        print("JSON access to keys: '{}'".format(key))
        sf = cset[key]

        sf_input_names = [inp.name for inp in sf.inputs]
        print("Inputs: " + ", ".join(sf_input_names))

        h2d = TH2D("my_2d_histogram" + run, "JEC_" + initName + run + "_" + version + "_MC_Truth_AK4PFPuppi;p_{T} [GeV];#eta", num_x_bins, x_bins, num_y_bins, y_bins)

        for j in range(len(x_edges) - 1):
            for i in range(len(y_edges) - 1):
                example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
                example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
                inputs = get_corr_inputs(example_value_dict, sf)
                h2d.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0, 1.0)


        h2d.Draw("COLZ")  # "COLZ" draws a color palette indicating bin content
        h2d.SetName("JECMCTruth_" + filePeriod + run + "_AK4PFPuppi")
        h2d.Write()


    #
    # example 1: getting a compound JEC level
    #

    print("\n\nExample 1: compound JEC level\n===================")

    # Define variable bin edges for the x-axis
    x_edges = range(10,7001,1)
    x_bins = array.array('d', x_edges)
    num_x_bins = len(x_bins) - 1

    # Define variable bin edges for the y-axis
    y_edges = []

    if filePeriod == "Summer22": y_edges = [-5.191, -4.889, -4.716, -4.538, -4.363, -4.191, -4.013, -3.839, -3.664, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.83, -1.74, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087, 0.0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191, 4.363, 4.538, 4.716, 4.889, 5.191] # Summer22
    if filePeriod == "Summer22EE": y_edges = [-5.191, -4.583, -4.013, -3.839, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -1.93, -1.653, -1.479, -1.305, -1.044, -0.783, -0.522, -0.261, 0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191] # Summer22EE
    if filePeriod == "Summer23": y_edges = [-5.191, -4.583, -4.013, -3.839, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -1.93, -1.653, -1.479, -1.305, -1.044, -0.783, -0.522, -0.261, -0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191] # Summer23
    if filePeriod == "Summer23BPix": y_edges = [-5.191, -4.583, -4.013, -3.839, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -1.93, -1.653, -1.479, -1.305, -1.044, -0.783, -0.522, -0.261, -0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191] # Summer23BPix
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

        key = "{}_{}_{}_{}".format(jec, datatype, lvl, algo)
        print("JSON access to keys: '{}'".format(key))
        sf = cset[key]

        sf_input_names = [inp.name for inp in sf.inputs]
        print("Inputs: " + ", ".join(sf_input_names))

        h2d = TH2D("my_2d_histogram" + run, "JEC_" + initName + run + "_" + version + "_DATA_L2L3Residual_AK4PFPuppi;p_{T} [GeV];#eta", num_x_bins, x_bins, num_y_bins, y_bins)

        for j in range(len(x_edges) - 1):
            for i in range(len(y_edges) - 1):
                example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
                example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
                inputs = get_corr_inputs(example_value_dict, sf)
                h2d.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0,sf.evaluate(*inputs))


        h2d.Draw("COLZ")  # "COLZ" draws a color palette indicating bin content
        h2d.SetName("JECL2L3_" + filePeriod + run + "_AK4PFPuppi")
        h2d.Write()

    #
    # example 3: accessing the JEC uncertainty sources
    #

    print("\n\nExample 3: JEC uncertainty source\n===================")

    # additional note: Regrouped/reduced set of uncertainty sorces as detailed in
    # https://twiki.cern.ch/twiki/bin/viewauth/CMS/JECUncertaintySources#Run_2_reduced_set_of_uncertainty  # noqa
    # are included in relevant JSON files (currently UL) with a "Regrouped_"-prefix,
    # e.g. for 2016 one could access "Absolute_2016" via:
    # sf = cset["Summer19UL16_V7_MC_Regrouped_Absolute_2016_AK4PFchs"]

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



    # ########################
    # # JER-related examples #
    # ########################

    # Complement for 2023
    complement = ""
    if filePeriod == "Summer23": complement = "_RunCv1234"
    if filePeriod == "Summer23BPix": complement = "_RunD"

    # # JER base tag
    jer = initName + complement + "_JRV1_MC"

    # # algorithms
    algo = "AK4PFPuppi"

    # print input information
    print("\n\nJER parameters")
    print("##############\n")

    print("jer = {}".format(jer))
    print("algo = {}".format(algo))
    for v in ("JetPt", "JetEta", "Rho"):
        print("{} = {}".format(v, example_value_dict[v]))


    #
    # example 4: accessing the JER scale factor
    #

    print("\n\nExample 4: JER scale factor\n===================")

    # Define variable bin edges for the x-axis
    x_edges = [10.0, 121.0, 140.0, 150.0, 191.0, 220.0, 249.0, 272.0, 352.0, 4000.0]
    x_bins = array.array('d', x_edges)
    num_x_bins = len(x_bins) - 1

    # Define variable bin edges for the y-axis
    y_edges = [-5.191, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.74, -1.566, -1.305, -1.044, -0.783, -0.522, -0.261, -0.0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.566, 1.74, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 5.191]
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

    #
    # example 5: accessing the JER
    #

    print("\n\nExample 5: JER (pT resolution)\n===================")

    # Define variable bin edges for the x-axis
    x_edges = range(10,7001,1)
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

    for j in range(len(x_edges) - 1):
        for i in range(len(y_edges) - 1):
            for k in range(len(z_edges) - 1):
                example_value_dict["JetEta"] = (y_edges[i] + y_edges[i+1]) / 2.0
                example_value_dict["JetPt"] = (x_edges[j] + x_edges[j+1]) / 2.0
                example_value_dict["Rho"] = (z_edges[k] + z_edges[k+1]) / 2.0
                inputs = get_corr_inputs(example_value_dict, sf)
                h3d.Fill((x_edges[j] + x_edges[j+1]) / 2.0,(y_edges[i] + y_edges[i+1]) / 2.0,(z_edges[k] + z_edges[k+1]) / 2.0,sf.evaluate(*inputs))


    h3d.SetName("JERPtRes_" + filePeriod + "_AK4PFPuppi")
    h3d.Write()
    h2dFile.Close()