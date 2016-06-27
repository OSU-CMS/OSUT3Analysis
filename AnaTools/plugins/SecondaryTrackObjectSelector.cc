#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(secondaryTracks)
  typedef ObjectSelector<osu::SecondaryTrack, TYPE(secondaryTracks)> SecondaryTrackObjectSelector;
  DEFINE_FWK_MODULE(SecondaryTrackObjectSelector);
#endif
