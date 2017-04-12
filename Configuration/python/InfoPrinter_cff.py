import FWCore.ParameterSet.Config as cms

infoPrinter = cms.EDAnalyzer ("InfoPrinter",
################################################################################
# N.B.: one of the following three options must be set in order to print out
# extra information
    eventsToPrint               =  cms.VEventID (
#        cms.EventID (1, 16, 2686),
    ),
    printAllEvents              =  cms.bool  (False),  # print information for all events, overriding eventsToPrint
    printPassedEvents           =  cms.bool  (False),  # print information for events that pass all cuts
################################################################################

    valuesToPrint               =  cms.VPSet (
#        cms.PSet (
#            inputCollection = cms.vstring ("jets"),
#            valueToPrint = cms.string ("pt")
#        ),
    ),
    printIndividualObjectFlags  =  cms.bool  (False),  # print the noncumulative flags for each object
    printCumulativeObjectFlags  =  cms.bool  (False),  # print the cumulative flags for each object
    printTriggerFlags           =  cms.bool  (False),  # print the decision for each trigger
    printVetoTriggerFlags       =  cms.bool  (False),  # print the decision for each trigger to veto
    printTriggerFilterFlags     =  cms.bool  (False),  # print the decision for each trigger filter
    printTriggerInMenuFlags     =  cms.bool  (False),  # print the decision for each trigger required to be in the menu
    printMETFilterFlags         =  cms.bool  (False),  # print the decision for each MET filter
    printIndividualEventFlags   =  cms.bool  (False),  # print the decision for each cut, independent of any other cuts
    printCumulativeEventFlags   =  cms.bool  (False),  # print the decision for each cut, based on objects selected by previous cuts
    printTriggerDecision        =  cms.bool  (False),  # print whether the event passes the triggers
    printMETFilterDecision      =  cms.bool  (False),  # print whether the event passes the MET filters
    printCutDecision            =  cms.bool  (False),  # print whether the event passes all cuts, not including the triggers or MET filters
    printEventDecision          =  cms.bool  (False),  # print whether the event passes the triggers, MET filters, and all cuts
    printAllTriggers            =  cms.bool  (False),  # print all triggers in the event
    printAllTriggerFilters      =  cms.bool  (False),  # print all trigger filters in the event
    printAllMETFilters          =  cms.bool  (False),  # print all MET filters in the event
)
