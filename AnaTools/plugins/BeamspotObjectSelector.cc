#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

extern template class ObjectSelector<osu::Beamspot, TYPE(beamspots)>;

#if IS_VALID(beamspots)
typedef ObjectSelector<osu::Beamspot, TYPE(beamspots)> BeamspotObjectSelector;
  DEFINE_FWK_MODULE(BeamspotObjectSelector);
#endif
