#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(dtsegs)
  typedef ObjectSelector<osu::Dtseg, TYPE(dtsegs)> DtsegObjectSelector;
  DEFINE_FWK_MODULE(DtsegObjectSelector);
#endif
