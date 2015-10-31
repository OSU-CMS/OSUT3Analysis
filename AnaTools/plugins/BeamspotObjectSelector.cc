#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(beamspots)
  typedef ObjectSelector<osu::Beamspot> BeamspotObjectSelector;
  DEFINE_FWK_MODULE(BeamspotObjectSelector);
#endif
