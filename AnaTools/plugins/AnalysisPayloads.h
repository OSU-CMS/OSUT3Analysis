#ifndef ANALYSIS_PAYLOADS
#define ANALYSIS_PAYLOADS

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

struct CutCalculatorPayload
{
  bool            isValid;
  bool            cutDecision;
  bool            eventDecision;
  bool            triggerDecision;
  flagMap         cumulativeObjectFlags;
  flagMap         objectFlags;
  vector<bool>    eventFlags;
  vector<bool>    triggerFlags;
  vector<bool>    vetoTriggerFlags;
  vector<cut>     cuts;
  vector<string>  triggers;
  vector<string>  triggersToVeto;
};

#endif
