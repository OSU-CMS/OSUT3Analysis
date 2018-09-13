#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(hardInteractionMcparticles)
typedef ObjectSelector<osu::HardInteractionMcparticle, TYPE(hardInteractionMcparticles)> HardInteractionMcparticleObjectSelector;
  DEFINE_FWK_MODULE(HardInteractionMcparticleObjectSelector);
#endif
