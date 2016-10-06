#ifndef OSU_TAU
#define OSU_TAU

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"
#include "OSUT3Analysis/Collections/interface/Met.h"

#if IS_VALID(taus)

namespace osu
{
  class Tau : public GenMatchable<TYPE(taus), 15>
    {
      public:
        Tau ();
        Tau (const TYPE(taus) &);
        Tau (const TYPE(taus) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Tau (const TYPE(taus) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        Tau (const TYPE(taus) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &, const osu::Met &);
        ~Tau ();

        const bool passesDecayModeReconstruction () const;
        const bool passesLightFlavorRejection () const;
        const bool passesLooseCombinedIsolation () const;
        const bool passesMediumCombinedIsolation () const;
        const bool passesTightCombinedIsolation () const;

        const double metMinusOnePt () const;
        const double metMinusOnePx () const;
        const double metMinusOnePy () const;
        const double metMinusOnePhi () const;

        const double metNoMuMinusOnePt () const;
        const double metNoMuMinusOnePx () const;
        const double metNoMuMinusOnePy () const;
        const double metNoMuMinusOnePhi () const;

        const double metMinusOneUpPt () const;
        const double metMinusOneUpPx () const;
        const double metMinusOneUpPy () const;
        const double metMinusOneUpPhi () const;

        const double metNoMuMinusOneUpPt () const;
        const double metNoMuMinusOneUpPx () const;
        const double metNoMuMinusOneUpPy () const;
        const double metNoMuMinusOneUpPhi () const;

      private:

        double metMinusOnePt_;
        double metMinusOnePx_;
        double metMinusOnePy_;
        double metMinusOnePhi_;

        double metNoMuMinusOnePt_;
        double metNoMuMinusOnePx_;
        double metNoMuMinusOnePy_;
        double metNoMuMinusOnePhi_;

        double metMinusOneUpPt_;
        double metMinusOneUpPx_;
        double metMinusOneUpPy_;
        double metMinusOneUpPhi_;

        double metNoMuMinusOneUpPt_;
        double metNoMuMinusOneUpPx_;
        double metNoMuMinusOneUpPy_;
        double metNoMuMinusOneUpPhi_;
    };
}

#else

namespace osu
{
  typedef TYPE(taus) Tau;
}

#endif

#endif
