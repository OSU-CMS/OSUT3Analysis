#ifndef OSU_MUON
#define OSU_MUON

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(muons)

namespace osu
{
  class Muon : public GenMatchable<TYPE(muons), 13>
    {
      public:
        Muon ();
        Muon (const TYPE(muons) &);
        Muon (const TYPE(muons) &, const edm::Handle<vector<osu::Mcparticle> > &);
        ~Muon ();

	const bool isTightMuonWRTVtx() const { return isTightMuonWRTVtx_; }   
	void   set_isTightMuonWRTVtx(const bool isTightMuon);   

      private:
	bool isTightMuonWRTVtx_; 

    };
}

#else

namespace osu
{
  typedef TYPE(muons) Muon;
}

#endif

#endif
