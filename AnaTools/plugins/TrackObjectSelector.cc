#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(tracks)
  typedef ObjectSelector<osu::Track, TYPE(tracks)> TrackObjectSelector;
  DEFINE_FWK_MODULE(TrackObjectSelector);

#if IS_VALID(secondaryTracks)
  typedef ObjectSelector<osu::SecondaryTrack, TYPE(secondaryTracks)> SecondaryTrackObjectSelector;
  DEFINE_FWK_MODULE(SecondaryTrackObjectSelector);
#endif

#endif
