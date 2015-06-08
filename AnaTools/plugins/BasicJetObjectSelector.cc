#include "OSUT3Analysis/AnaTools/interface/ObjectSelector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#if IS_VALID(basicjets)
  typedef ObjectSelector<TYPE(basicjets)> BasicJetObjectSelector;
  DEFINE_FWK_MODULE(BasicJetObjectSelector);
#endif
