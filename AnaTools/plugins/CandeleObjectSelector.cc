#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(candeles)
  typedef ObjectSelector<TYPE(candeles)> CandeleObjectSelector;
  DEFINE_FWK_MODULE(CandeleObjectSelector);
#endif
