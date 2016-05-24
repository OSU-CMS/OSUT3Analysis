#ifndef OSU_MET
#define OSU_MET

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

#if IS_VALID(mets)

namespace osu
{
  class Met : public TYPE(mets)
    {
      public:
        Met ();
        Met (const TYPE(mets) &);
        Met (const TYPE(mets) &, const edm::Handle<vector<pat::PackedCandidate> > &);
        ~Met ();

        const double noMuPt () const;
        const double noMuPx () const;
        const double noMuPy () const;
        const double noMuPhi () const;

      private:
        double noMuPt_;
        double noMuPx_;
        double noMuPy_;
        double noMuPhi_;
    };
}

#else

namespace osu
{
  typedef TYPE(mets) Met;
}

#endif

#endif
