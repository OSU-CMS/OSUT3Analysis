#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(events)
  typedef ObjectSelector<osu::Event> EventObjectSelector;
  DEFINE_FWK_MODULE(EventObjectSelector);
#endif
