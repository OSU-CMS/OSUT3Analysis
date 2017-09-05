#ifndef OSU_SECONDARYTRACK
#define OSU_SECONDARYTRACK

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"
#include "OSUT3Analysis/Collections/interface/Track.h"

#ifndef MAX_DR
#define MAX_DR (99.0)
#endif

#if IS_VALID(secondaryTracks)

namespace osu
{
  class SecondaryTrack : public GenMatchable<TYPE(secondaryTracks), 0>
    {
      public:
        SecondaryTrack ();
        SecondaryTrack (const TYPE(secondaryTracks) &);
        SecondaryTrack (const TYPE(secondaryTracks) &, const edm::Handle<vector<osu::Mcparticle> > &);
        SecondaryTrack (const TYPE(secondaryTracks) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        SecondaryTrack (const TYPE(secondaryTracks) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &, const edm::Handle<vector<reco::GsfTrack> > &, const EtaPhiList &, const EtaPhiList &);
        ~SecondaryTrack ();

        const double dRMinJet() const;
        void   set_dRMinJet(const double dRMinJet);

        const bool isFiducialElectronTrack () const;
        const bool isFiducialMuonTrack () const;

        const edm::Ref<vector<reco::GsfTrack> > matchedGsfTrack () const;
        const double dRToMatchedGsfTrack () const;

        const int gsfTrackMissingInnerHits () const;
        const int gsfTrackMissingMiddleHits () const;
        const int gsfTrackMissingOuterHits () const;

        const double innerP () const;
        const double outerP () const;
        const double fbrem () const;
        const double bremEnergy () const;

    private:
        double dRMinJet_;
        double minDeltaRForFiducialTrack_;

        bool isFiducialElectronTrack_;
        bool isFiducialMuonTrack_;

        edm::Ref<vector<reco::GsfTrack> > matchedGsfTrack_;
        double dRToMatchedGsfTrack_;

        double maxDeltaR_;

        const bool isFiducialTrack (const EtaPhiList &, const double) const;
        const edm::Ref<vector<reco::GsfTrack> > &findMatchedGsfTrack (const edm::Handle<vector<reco::GsfTrack> > &, edm::Ref<vector<reco::GsfTrack> > &, double &) const;
    };
}

#else

namespace osu
{
  typedef TYPE(secondaryTracks) SecondaryTrack;
}

#endif

#endif
