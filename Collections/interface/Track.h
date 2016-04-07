#ifndef OSU_TRACK
#define OSU_TRACK

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

#if IS_VALID(tracks)

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

namespace osu
{
  class Track : public GenMatchable<TYPE(tracks), 0>
    {
      public:
        Track ();
        Track (const TYPE(tracks) &);
        Track (const TYPE(tracks) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Track (const TYPE(tracks) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        Track (const TYPE(tracks) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &, const EtaPhiList &, const EtaPhiList &);
        ~Track ();

        const double dRMinJet() const { return dRMinJet_; }
        void   set_dRMinJet(const double dRMinJet);

        const bool isFiducialElectronTrack () const;
        const bool isFiducialMuonTrack () const;

    private:
	double dRMinJet_;  
        double minDeltaRForFiducialTrack_;

        bool isFiducialElectronTrack_;
        bool isFiducialMuonTrack_;

        const bool isFiducialTrack (const EtaPhiList &, const double) const;
    };
}

#else

namespace osu
{
  typedef TYPE(tracks) Track;
}

#endif

#endif
