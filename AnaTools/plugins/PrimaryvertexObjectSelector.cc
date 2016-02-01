#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(primaryvertexs)
typedef ObjectSelector<osu::Primaryvertex, TYPE(primaryvertexs)> PrimaryvertexObjectSelector;
  DEFINE_FWK_MODULE(PrimaryvertexObjectSelector);
#endif
