import FWCore.ParameterSet.Config as cms

infoPrinter = cms.EDAnalyzer ("InfoPrinter",
    eventsToPrint               =  cms.VEventID (
#        cms.EventID (1, 728, 7),
    ),
    printAllEvents              =  cms.bool  (False),  # print information for all events, overriding eventsToPrint
    printPassedEvents           =  cms.bool  (True),   # print information for events that pass all cuts 
    valuesToPrint               =  cms.VPSet (
#        cms.PSet (
#            inputCollection = cms.vstring ("jets"),
#            valueToPrint = cms.string ("pt")
#        ),
    ),
    printObjectFlags            =  cms.bool  (False),  # print the noncumulative flags for each object
    printCumulativeObjectFlags  =  cms.bool  (False),  # print the cumulative flags for each object
    printTriggerFlags           =  cms.bool  (False),  # print the decision for each trigger
    printVetoTriggerFlags       =  cms.bool  (False),  # print the decision for each trigger to veto
    printEventFlags             =  cms.bool  (False),  # print the decision for each cut
    printTriggerDecision        =  cms.bool  (False),  # print whether the event passes the triggers
    printCutDecision            =  cms.bool  (False),  # print whether the event passes all cuts, not including the triggers
    printEventDecision          =  cms.bool  (False),  # print whether the event passes the triggers and all cuts
    printAllTriggers            =  cms.bool  (False)   # print all triggers in the event
)
