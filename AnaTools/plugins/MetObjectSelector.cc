#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(mets)
  typedef ObjectSelector<TYPE(mets)> MetObjectSelector;
  DEFINE_FWK_MODULE(MetObjectSelector);
#endif
