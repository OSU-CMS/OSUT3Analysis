#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(photons)
typedef ObjectSelector<osu::Photon, TYPE(photons)> PhotonObjectSelector;
  DEFINE_FWK_MODULE(PhotonObjectSelector);
#endif
