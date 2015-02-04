#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(muons)
  typedef ObjectSelector<TYPE(muons)> MuonObjectSelector;
  DEFINE_FWK_MODULE(MuonObjectSelector);
#endif
