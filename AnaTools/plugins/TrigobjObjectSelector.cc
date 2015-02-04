#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(trigobjs)
  typedef ObjectSelector<TYPE(trigobjs)> TrigobjObjectSelector;
  DEFINE_FWK_MODULE(TrigobjObjectSelector);
#endif
