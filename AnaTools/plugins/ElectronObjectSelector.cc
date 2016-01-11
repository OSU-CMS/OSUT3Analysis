#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(electrons)
typedef ObjectSelector<osu::Electron, TYPE(electrons)> ElectronObjectSelector;
  DEFINE_FWK_MODULE(ElectronObjectSelector);
#endif
