#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(cschits)
  typedef ObjectSelector<osu::Cschit> CschitObjectSelector;
  DEFINE_FWK_MODULE(CschitObjectSelector);
#endif
