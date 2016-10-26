import FWCore.ParameterSet.Config as cms

infoPrinter = cms.EDAnalyzer ("InfoPrinter",
    eventsToPrint               =  cms.VEventID (
#        cms.EventID (1, 16, 2686),
    ),
    printAllEvents              =  cms.bool  (False),  # print information for all events, overriding eventsToPrint
    printPassedEvents           =  cms.bool  (False),  # print information for events that pass all cuts
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
    printIndividualEventFlags   =  cms.bool  (False),  # print the decision for each cut, independent of any other cuts
    printCumulativeEventFlags   =  cms.bool  (False),  # print the decision for each cut, based on objects selected by previous cuts
    printTriggerDecision        =  cms.bool  (False),  # print whether the event passes the triggers
    printCutDecision            =  cms.bool  (False),  # print whether the event passes all cuts, not including the triggers
    printEventDecision          =  cms.bool  (False),  # print whether the event passes the triggers and all cuts
    printAllTriggers            =  cms.bool  (False),  # print all triggers in the event
    printAllTriggerFilters      =  cms.bool  (False),  # print all trigger filters in the event
)
