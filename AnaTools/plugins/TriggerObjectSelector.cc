#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(triggers)
  typedef ObjectSelector<TYPE(triggers)> TriggerObjectSelector;
  DEFINE_FWK_MODULE(TriggerObjectSelector);
#endif
