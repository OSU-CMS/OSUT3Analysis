#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(cscsegs)
  typedef ObjectSelector<osu::Cscseg> CscsegObjectSelector;
  DEFINE_FWK_MODULE(CscsegObjectSelector);
#endif
