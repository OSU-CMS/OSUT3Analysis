#ifndef OSU_TRACK
#define OSU_TRACK

#include <random>
#include <chrono>

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#define MAX_DR (99.0)

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
        Track (const TYPE(tracks) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &, const edm::Handle<vector<reco::GsfTrack> > &, const EtaPhiList &, const EtaPhiList &, const map<DetId, vector<double> > * const, const map<DetId, vector<int> > * const, const bool);
        ~Track ();

        const double dRMinJet() const;
        void   set_dRMinJet(const double dRMinJet);

        const bool isFiducialElectronTrack () const;
        const bool isFiducialMuonTrack () const;
        const bool isFiducialECALTrack () const;

        const edm::Ref<vector<reco::GsfTrack> > matchedGsfTrack () const;
        const double dRToMatchedGsfTrack () const;

        const int gsfTrackNumberOfValidHits () const;
        const int gsfTrackMissingInnerHits () const;
        const int gsfTrackMissingMiddleHits () const;
        const int gsfTrackMissingOuterHits () const;

        const int bestTrackNumberOfValidHits () const;
        const int bestTrackMissingInnerHits () const;
        const int bestTrackMissingMiddleHits () const;
        const int bestTrackMissingOuterHits () const;

        const int hitAndTOBDrop_missingMiddleHits () const;
        const int hitAndTOBDrop_gsfTrackMissingMiddleHits () const;
        const int hitAndTOBDrop_bestTrackMissingMiddleHits () const;

        const int hitAndTOBDrop_missingOuterHits () const;
        const int hitAndTOBDrop_gsfTrackMissingOuterHits () const;
        const int hitAndTOBDrop_bestTrackMissingOuterHits () const;

        const double innerP () const;
        const double outerP () const;
        const double fbrem () const;
        const double bremEnergy () const;

        const bool inTOBCrack () const;

    private:
        double dRMinJet_;
        double minDeltaRForFiducialTrack_;

        bool isFiducialElectronTrack_;
        bool isFiducialMuonTrack_;

        edm::Ref<vector<reco::GsfTrack> > matchedGsfTrack_;
        double dRToMatchedGsfTrack_;

        double maxDeltaR_;

        const map<DetId, vector<double> > * EcalAllDeadChannelsValMap_;
        const map<DetId, vector<int> >    * EcalAllDeadChannelsBitMap_;

        bool isFiducialECALTrack_;

        double dropTOBProbability_;
        double preTOBDropHitProbability_;
        double postTOBDropHitProbability_;

        bool dropTOBDecision_;
        vector<bool> dropHitDecisions_;

        const bool isFiducialTrack (const EtaPhiList &, const double) const;
        const edm::Ref<vector<reco::GsfTrack> > &findMatchedGsfTrack (const edm::Handle<vector<reco::GsfTrack> > &, edm::Ref<vector<reco::GsfTrack> > &, double &) const;
        const bool isBadGsfTrack (const reco::GsfTrack &) const;
        int isCloseToBadEcalChannel (const double &);
        template<class T> const int extraMissingMiddleHits (const T &) const;
        template<class T> const int extraMissingOuterHits (const T &) const;
    };
}

#else

namespace osu
{
  typedef TYPE(tracks) Track;
}

#endif

#endif
