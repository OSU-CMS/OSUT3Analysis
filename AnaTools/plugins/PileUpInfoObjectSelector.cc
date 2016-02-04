#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(pileupinfos)
  typedef ObjectSelector<osu::PileUpInfo,TYPE(pileupinfos) > PileUpInfoObjectSelector;
  DEFINE_FWK_MODULE(PileUpInfoObjectSelector);
#endif
