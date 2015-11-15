#ifndef OSU_TRACK
#define OSU_TRACK

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(tracks)

namespace osu
{
  class Track : public GenMatchable<TYPE(tracks), 0>
    {
      public:
        Track ();
        Track (const TYPE(tracks) &);
        Track (const TYPE(tracks) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Track (const TYPE(tracks) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        ~Track ();
    };
}

#else

namespace osu
{
  typedef TYPE(tracks) Track;
}

#endif

#endif
