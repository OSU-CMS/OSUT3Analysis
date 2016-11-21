#include "OSUT3Analysis/Collections/interface/Track.h"

#if IS_VALID(tracks)

osu::Track::Track () :
  dRMinJet_ (INVALID_VALUE),
  isFiducialElectronTrack_ (true),
  isFiducialMuonTrack_ (true),
  matchedGsfTrack_ (),
  dRToMatchedGsfTrack_ (INVALID_VALUE),
  maxDeltaR_ (-1.0),
  EcalAllDeadChannelsValMap_ (NULL),
  EcalAllDeadChannelsBitMap_ (NULL),
  isFiducialECALTrack_ (true),
  dropTOBDecision_ (-1.0),
  dropHitDecisions_ ({}),
  dropMiddleHitDecisions_ ({})
{
}

osu::Track::Track (const TYPE(tracks) &track) :
  GenMatchable (track),
  dRMinJet_ (INVALID_VALUE),
  isFiducialElectronTrack_ (true),
  isFiducialMuonTrack_ (true),
  matchedGsfTrack_ (),
  dRToMatchedGsfTrack_ (INVALID_VALUE),
  maxDeltaR_ (-1.0),
  EcalAllDeadChannelsValMap_ (NULL),
  EcalAllDeadChannelsBitMap_ (NULL),
  isFiducialECALTrack_ (true),
  dropTOBDecision_ (-1.0),
  dropHitDecisions_ ({}),
  dropMiddleHitDecisions_ ({})
{
}

osu::Track::Track (const TYPE(tracks) &track, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable (track, particles),
  dRMinJet_ (INVALID_VALUE),
  isFiducialElectronTrack_ (true),
  isFiducialMuonTrack_ (true),
  matchedGsfTrack_ (),
  dRToMatchedGsfTrack_ (INVALID_VALUE),
  maxDeltaR_ (-1.0),
  EcalAllDeadChannelsValMap_ (NULL),
  EcalAllDeadChannelsBitMap_ (NULL),
  isFiducialECALTrack_ (true),
  dropTOBDecision_ (-1.0),
  dropHitDecisions_ ({}),
  dropMiddleHitDecisions_ ({})
{
}

osu::Track::Track (const TYPE(tracks) &track, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable (track, particles, cfg),
  dRMinJet_ (INVALID_VALUE),
  isFiducialElectronTrack_ (true),
  isFiducialMuonTrack_ (true),
  matchedGsfTrack_ (),
  dRToMatchedGsfTrack_ (INVALID_VALUE),
  maxDeltaR_ (-1.0),
  EcalAllDeadChannelsValMap_ (NULL),
  EcalAllDeadChannelsBitMap_ (NULL),
  isFiducialECALTrack_ (true),
  dropTOBDecision_ (-1.0),
  dropHitDecisions_ ({}),
  dropMiddleHitDecisions_ ({})
{
}

osu::Track::Track (const TYPE(tracks) &track, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg, const edm::Handle<vector<reco::GsfTrack> > &gsfTracks, const EtaPhiList &electronVetoList, const EtaPhiList &muonVetoList, const map<DetId, vector<double> > * const EcalAllDeadChannelsValMap, const map<DetId, vector<int> > * const EcalAllDeadChannelsBitMap, const bool dropHits) :
  GenMatchable (track, particles, cfg),
  dRMinJet_ (INVALID_VALUE),
  minDeltaRForFiducialTrack_ (cfg.getParameter<double> ("minDeltaRForFiducialTrack")),
  isFiducialElectronTrack_ (isFiducialTrack (electronVetoList, minDeltaRForFiducialTrack_)),
  isFiducialMuonTrack_ (isFiducialTrack (muonVetoList, minDeltaRForFiducialTrack_)),
  EcalAllDeadChannelsValMap_ (EcalAllDeadChannelsValMap),
  EcalAllDeadChannelsBitMap_ (EcalAllDeadChannelsBitMap),
  isFiducialECALTrack_ (!isCloseToBadEcalChannel (minDeltaRForFiducialTrack_)),
  dropTOBDecision_ (-1.0),
  dropHitDecisions_ ({}),
  dropMiddleHitDecisions_ ({})
{
  maxDeltaR_ = cfg.getParameter<double> ("maxDeltaRForGsfTrackMatching");
  if (gsfTracks.isValid ())
    findMatchedGsfTrack (gsfTracks, matchedGsfTrack_, dRToMatchedGsfTrack_);
  EcalAllDeadChannelsValMap_ = NULL;
  EcalAllDeadChannelsBitMap_ = NULL;

  dropTOBProbability_ = cfg.getParameter<double> ("dropTOBProbability");
  preTOBDropHitProbability_ = cfg.getParameter<double> ("preTOBDropHitInefficiency");
  postTOBDropHitProbability_ = cfg.getParameter<double> ("postTOBDropHitInefficiency");
  hitProbability_ = cfg.getParameter<double> ("hitInefficiency");

  edm::LogInfo ("osu_Track")  <<  "dropTOBProbability:         "  <<  (dropTOBProbability_         *  100.0)  <<  "%"   <<  endl
                              <<  "preTOBDropHitProbability:   "  <<  (preTOBDropHitProbability_   *  100.0)  <<  "%"   <<  endl
                              <<  "postTOBDropHitProbability:  "  <<  (postTOBDropHitProbability_  *  100.0)  <<  "%"   <<  endl
                              <<  "hitProbability:             "  <<  (hitProbability_             *  100.0)  <<  "%";

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator (seed);
  uniform_real_distribution<double> distribution (0.0, 1.0);
  dropTOBDecision_ = (dropHits ? distribution (generator) : 1.0e6) < dropTOBProbability_;
  for (int i = 0; i < 50; i++)
    dropHitDecisions_.push_back ((dropHits ? distribution (generator) : 1.0e6) < (dropTOBDecision_ ? postTOBDropHitProbability_ : preTOBDropHitProbability_));
  for (int i = 0; i < 50; i++)
    dropMiddleHitDecisions_.push_back ((dropHits ? distribution (generator) : 1.0e6) < hitProbability_);
}

osu::Track::~Track ()
{
}

const bool
osu::Track::isFiducialElectronTrack () const
{
  return isFiducialElectronTrack_;
}

const bool
osu::Track::isFiducialMuonTrack () const
{
  return isFiducialMuonTrack_;
}

const bool
osu::Track::isFiducialECALTrack () const
{
  return isFiducialECALTrack_;
}

const edm::Ref<vector<reco::GsfTrack> >
osu::Track::matchedGsfTrack () const
{
  return matchedGsfTrack_;
}

const double
osu::Track::dRToMatchedGsfTrack () const
{
  if (IS_INVALID(dRToMatchedGsfTrack_))
    return MAX_DR;
  return dRToMatchedGsfTrack_;
}

const double
osu::Track::dRMinJet () const
{
  if (IS_INVALID(dRMinJet_))
    return MAX_DR;
  return dRMinJet_;
}

void
osu::Track::set_dRMinJet(const double dRMinJet)
{
  dRMinJet_ = dRMinJet;
}

const bool
osu::Track::isFiducialTrack (const EtaPhiList &vetoList, const double minDeltaR) const
{
  const double minDR = max (minDeltaR, vetoList.minDeltaR); // use the given parameter unless the bin size from which the veto list is calculated is larger
  for (const auto &etaPhi : vetoList)
    if (deltaR (this->eta (), this->phi (), etaPhi.eta, etaPhi.phi) < minDR)
      return false;
  return true;
}

const edm::Ref<vector<reco::GsfTrack> > &
osu::Track::findMatchedGsfTrack (const edm::Handle<vector<reco::GsfTrack> > &gsfTracks, edm::Ref<vector<reco::GsfTrack> > &matchedGsfTrack, double &dRToMatchedGsfTrack) const
{
  dRToMatchedGsfTrack = INVALID_VALUE;
  for (vector<reco::GsfTrack>::const_iterator gsfTrack = gsfTracks->begin (); gsfTrack != gsfTracks->end (); gsfTrack++)
    {
      double dR = deltaR (*gsfTrack, *this);
      if (maxDeltaR_ >= 0.0 && dR > maxDeltaR_)
        continue;
      if (dR < dRToMatchedGsfTrack || dRToMatchedGsfTrack < 0.0)
        {
          dRToMatchedGsfTrack = dR;
          matchedGsfTrack = edm::Ref<vector<reco::GsfTrack> > (gsfTracks, gsfTrack - gsfTracks->begin ());
        }
    }

  return matchedGsfTrack;
}

const int
osu::Track::gsfTrackNumberOfValidHits () const
{
  if (this->matchedGsfTrack_.isNonnull ())
    return this->matchedGsfTrack_->numberOfValidHits ();

  return INVALID_VALUE;
}

const int
osu::Track::gsfTrackMissingInnerHits () const
{
  if (this->matchedGsfTrack_.isNonnull ())
    return this->matchedGsfTrack_->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_INNER_HITS);

  return INVALID_VALUE;
}

const int
osu::Track::gsfTrackMissingMiddleHits () const
{
  if (this->matchedGsfTrack_.isNonnull ())
    return this->matchedGsfTrack_->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::TRACK_HITS);

  return INVALID_VALUE;
}

const int
osu::Track::gsfTrackMissingOuterHits () const
{
  if (this->matchedGsfTrack_.isNonnull ())
    return this->matchedGsfTrack_->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_OUTER_HITS);

  return INVALID_VALUE;
}

const int
osu::Track::bestTrackNumberOfValidHits () const
{
  int nHits = gsfTrackNumberOfValidHits ();
  if (IS_INVALID(nHits) || isBadGsfTrack (*this->matchedGsfTrack_))
    nHits = this->numberOfValidHits ();

  return nHits;
}

const int
osu::Track::bestTrackMissingInnerHits () const
{
  int nHits = gsfTrackMissingInnerHits ();
  if (IS_INVALID(nHits) || isBadGsfTrack (*this->matchedGsfTrack_))
    nHits = this->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_INNER_HITS);

  return nHits;
}

const int
osu::Track::bestTrackMissingMiddleHits () const
{
  int nHits = gsfTrackMissingMiddleHits ();
  if (IS_INVALID(nHits) || isBadGsfTrack (*this->matchedGsfTrack_))
    nHits = this->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::TRACK_HITS);

  return nHits;
}

const int
osu::Track::bestTrackMissingOuterHits () const
{
  int nHits = gsfTrackMissingOuterHits ();
  if (IS_INVALID(nHits) || isBadGsfTrack (*this->matchedGsfTrack_))
    nHits = this->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_OUTER_HITS);

  return nHits;
}

/*******************************************************************************
 * Methods for testing effect of dropping random hits and all the TOB hits.
*******************************************************************************/

/* Missing middle hits */

template<class T> const int
osu::Track::extraMissingMiddleHits (const T &track) const
{
  int nHits = 0;
  bool countMissingMiddleHits = false;
  for (int i = 0; i < track.hitPattern ().stripLayersWithMeasurement () - (dropTOBDecision_ ? this->hitPattern ().stripTOBLayersWithMeasurement () : 0); i++)
    {
      bool hit = !dropMiddleHitDecisions_.at (i);
      if (!hit && countMissingMiddleHits)
        nHits++;
      if (hit)
        countMissingMiddleHits = true;
    }

  return nHits;
}

const int
osu::Track::hitDrop_missingMiddleHits () const
{
  int nDropHits = extraMissingMiddleHits (*this);
  return this->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::TRACK_HITS) + nDropHits;
}

const int
osu::Track::hitDrop_gsfTrackMissingMiddleHits () const
{
  if (this->matchedGsfTrack_.isNonnull ())
    {
      int nDropHits = extraMissingMiddleHits (*this->matchedGsfTrack_);
      return this->matchedGsfTrack_->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::TRACK_HITS) + nDropHits;
    }

  return INVALID_VALUE;
}

const int
osu::Track::hitDrop_bestTrackMissingMiddleHits () const
{
  int nHits = hitDrop_gsfTrackMissingMiddleHits ();
  if (IS_INVALID(nHits) || isBadGsfTrack (*this->matchedGsfTrack_))
    nHits = hitDrop_missingMiddleHits ();

  return nHits;
}

/* Missing outer hits */

template<class T> const int
osu::Track::extraMissingOuterHits (const T &track) const
{
  int nHits = 0;
  for (int i = 0; i < track.hitPattern ().stripLayersWithMeasurement () - (dropTOBDecision_ ? this->hitPattern ().stripTOBLayersWithMeasurement () : 0); i++)
    {
      bool hit = !dropHitDecisions_.at (i);
      if (!hit)
        nHits++;
      else
        break;
    }

  return nHits;
}

const int
osu::Track::hitAndTOBDrop_missingOuterHits () const
{
  int nDropTOBHits = (dropTOBDecision_ ? this->hitPattern ().stripTOBLayersWithMeasurement () : 0);
  int nDropHits = extraMissingOuterHits (*this);
  return this->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_OUTER_HITS) + nDropTOBHits + nDropHits;
}

const int
osu::Track::hitAndTOBDrop_gsfTrackMissingOuterHits () const
{
  if (this->matchedGsfTrack_.isNonnull ())
    {
      int nDropTOBHits = (dropTOBDecision_ ? this->matchedGsfTrack_->hitPattern ().stripTOBLayersWithMeasurement () : 0);
      int nDropHits = extraMissingOuterHits (*this->matchedGsfTrack_);
      return this->matchedGsfTrack_->hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_OUTER_HITS) + nDropTOBHits + nDropHits;
    }

  return INVALID_VALUE;
}

const int
osu::Track::hitAndTOBDrop_bestTrackMissingOuterHits () const
{
  int nHits = hitAndTOBDrop_gsfTrackMissingOuterHits ();
  if (IS_INVALID(nHits) || isBadGsfTrack (*this->matchedGsfTrack_))
    nHits = hitAndTOBDrop_missingOuterHits ();

  return nHits;
}

/******************************************************************************/

const double
osu::Track::innerP () const
{
  double pInner = innerMomentum ().r ();
  if (this->matchedGsfTrack_.isNonnull ())
    pInner = this->matchedGsfTrack_->innerMomentum ().r ();

  return pInner;
}

const double
osu::Track::outerP () const
{
  double pInner = outerMomentum ().r ();
  if (this->matchedGsfTrack_.isNonnull ())
    pInner = this->matchedGsfTrack_->outerMomentum ().r ();

  return pInner;
}

const double
osu::Track::fbrem () const
{
  double pInner = this->innerP (),
         pOuter = this->outerP ();

  return (pInner ? max (0.0, pOuter - pInner) / pInner : 0.0);
}

const double
osu::Track::bremEnergy () const
{
  double pInner = this->innerP (),
         pOuter = this->outerP ();

  return max (0.0, pOuter - pInner);
}

const bool
osu::Track::isBadGsfTrack (const reco::GsfTrack &track) const
{
  bool passes = (track.pt () > 20.0)
             && (track.numberOfValidHits () >= 7)
             && (track.hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_INNER_HITS) == 0)
             && (track.hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::TRACK_HITS) <= 1)
             && (track.normalizedChi2 () < this->normalizedChi2 () + 1.0);

  return !passes;
}

int
osu::Track::isCloseToBadEcalChannel (const double &deltaRCut)
{
   double trackEta = this->eta(), trackPhi = this->phi();

   double min_dist = 999;
   DetId min_detId;

   std::map<DetId, std::vector<int> >::const_iterator bitItor;
   for(bitItor = EcalAllDeadChannelsBitMap_->begin(); bitItor != EcalAllDeadChannelsBitMap_->end(); bitItor++){

      DetId maskedDetId = bitItor->first;

      std::map<DetId, std::vector<double> >::const_iterator valItor = EcalAllDeadChannelsValMap_->find(maskedDetId);
      if( valItor == EcalAllDeadChannelsValMap_->end() ){ std::cout<<"Error cannot find maskedDetId in EcalAllDeadChannelsValMap_ ?!"<<std::endl; continue; }

      double eta = (valItor->second)[0], phi = (valItor->second)[1];

      double dist = reco::deltaR(eta, phi, trackEta, trackPhi);

      if( min_dist > dist ){ min_dist = dist; min_detId = maskedDetId; }
   }

   if( min_dist > deltaRCut && deltaRCut >0 ) return 0;

   return 1;
}

const bool
osu::Track::inTOBCrack () const
{
  return (fabs (this->dz ()) < 0.5 && fabs (this->lambda ()) < 1.0e-3);
}

#endif
