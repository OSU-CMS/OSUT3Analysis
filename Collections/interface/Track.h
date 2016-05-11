#ifndef OSU_TRACK
#define OSU_TRACK

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

struct EtaPhi
{
  double eta;
  double phi;

  EtaPhi (const double a, const double b) :
    eta (a),
    phi (b)
  {
  }
};

struct EtaPhiList : public vector<EtaPhi>
{
  double minDeltaR;

  EtaPhiList () :
    minDeltaR (0.0)
  {
  }
};

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
        Track (const TYPE(tracks) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &, const edm::Handle<vector<reco::GsfTrack> > &, const EtaPhiList &, const EtaPhiList &);
        ~Track ();

        const double dRMinJet() const { return dRMinJet_; }
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
  typedef TYPE(tracks) Track;
}

#endif

#endif
