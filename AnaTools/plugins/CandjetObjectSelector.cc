#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(candjets)
  typedef ObjectSelector<TYPE(candjets)> CandjetObjectSelector;
  DEFINE_FWK_MODULE(CandjetObjectSelector);
#endif
