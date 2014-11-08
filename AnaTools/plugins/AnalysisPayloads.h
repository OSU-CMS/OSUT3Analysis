#ifndef ANALYSIS_PAYLOADS
#define ANALYSIS_PAYLOADS

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

struct CutCalculatorPayload
{
  FlagMap         cumulativeObjectFlags;
  FlagMap         objectFlags;
  bool            cutDecision;
  bool            eventDecision;
  bool            isValid;
  bool            triggerDecision;
  vector<Cut>     cuts;
  vector<bool>    eventFlags;
  vector<bool>    triggerFlags;
  vector<bool>    vetoTriggerFlags;
  vector<string>  triggers;
  vector<string>  triggersToVeto;
};

#endif
