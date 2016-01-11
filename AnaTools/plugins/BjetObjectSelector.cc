#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(bjets)
typedef ObjectSelector<osu::Bjet, TYPE(bjets)> BjetObjectSelector;
  DEFINE_FWK_MODULE(BjetObjectSelector);
#endif
