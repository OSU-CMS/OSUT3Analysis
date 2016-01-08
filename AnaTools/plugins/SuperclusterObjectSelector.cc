#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(superclusters)
  typedef ObjectSelector<osu::Supercluster, TYPE(superclusters)> SuperclusterObjectSelector;
  DEFINE_FWK_MODULE(SuperclusterObjectSelector);
#endif
