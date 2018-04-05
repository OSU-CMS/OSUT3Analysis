#include "OSUT3Analysis/AnaTools/interface/DataFormatMiniAOD.h"

#undef   tracks_TYPE
#undef   secondaryTracks_TYPE

#undef   tracks_INVALID
#undef   secondaryTracks_INVALID

#define  tracks_TYPE                      pat::IsolatedTrack
#define  secondaryTracks_TYPE             pat::IsolatedTrack

#include "DataFormats/PatCandidates/interface/IsolatedTrack.h"