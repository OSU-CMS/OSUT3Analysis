#ifndef TRACK_PRODUCER
#define TRACK_PRODUCER

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/ESGetToken.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/DeDxData.h"

#include "OSUT3Analysis/Collections/interface/DisappearingTrack.h" // includes TrackBase.h

// FIXME:  Once OSUT3Analysis works with ROOT6, i.e., releases > CMSSW_7_4_5_ROOT5,
// then uncomment the following line:
// #include "OSUT3Analysis/AnaTools/interface/DataFormat.h"
// and remove these two lines:
#define INVALID_VALUE (numeric_limits<int>::min ())
#define IS_INVALID(x) (x <= INVALID_VALUE + 1)

//define CaloEnergy struct
struct CaloEnergy
{
  double eEM;
  double eHad;
};

template<class T>
  class OSUGenericTrackProducer : public edm::stream::EDProducer<>
{
  public:
    OSUGenericTrackProducer (const edm::ParameterSet &);
    ~OSUGenericTrackProducer ();

    void beginRun (const edm::Run &, const edm::EventSetup &);
    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::InputTag      collection_;
    edm::InputTag      EBRecHitsTag_;
    edm::InputTag      EERecHitsTag_;
    edm::InputTag      HBHERecHitsTag_;
    edm::InputTag gt2dedxPixelTag_;
    edm::InputTag gt2dedxStripTag_;
    edm::EDGetTokenT<vector<TYPE(tracks)> >      token_;
    edm::EDGetTokenT<vector<osu::Mcparticle> >   mcparticleToken_;
    edm::EDGetTokenT<vector<TYPE(jets)> >        jetsToken_;
    edm::EDGetTokenT<EBRecHitCollection>         EBRecHitsToken_;
    edm::EDGetTokenT<EERecHitCollection>         EERecHitsToken_;
    edm::EDGetTokenT<HBHERecHitCollection>       HBHERecHitsToken_;
    edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > gt2dedxStripToken_;
    edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > gt2dedxPixelToken_;
    edm::EDGetTokenT<vector<reco::GsfTrack> >    gsfTracksToken_;

#ifdef DISAPP_TRKS
    edm::EDGetTokenT<edm::View<TYPE(electrons)> > electronsToken_;
    edm::EDGetTokenT<vector<TYPE(muons)> >        muonsToken_;
    edm::EDGetTokenT<vector<TYPE(taus)> >         tausToken_;

    edm::EDGetTokenT<edm::ValueMap<bool> > eleVIDVetoIdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > eleVIDLooseIdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > eleVIDMediumIdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > eleVIDTightIdMapToken_;

    edm::EDGetTokenT<vector<TYPE(primaryvertexs)> > primaryvertexToken_;

    edm::EDGetTokenT<vector<pat::PackedCandidate> > lostTracksToken_;
#if DATA_FORMAT_FROM_MINIAOD && ( DATA_FORMAT_IS_2017 || DATA_FORMAT_IS_2022 )
    edm::EDGetTokenT<vector<pat::IsolatedTrack> > isolatedTracksToken_;
#endif

    edm::EDGetTokenT<vector<CandidateTrack> >    candidateTracksToken_;
#endif

    edm::EDGetTokenT<vector<reco::Track> >       tracksToken_;
    edm::EDGetTokenT<vector<pat::PackedCandidate> > pfCandidatesToken_;
    edm::ParameterSet  cfg_;
    ////////////////////////////////////////////////////////////////////////////

    bool useEraByEraFiducialMaps_;

    EtaPhiList electronVetoList_;
    EtaPhiList muonVetoList_;

    // Payload for this EDFilter.
    unique_ptr<vector<T> > pl_;

    void extractFiducialMap (const edm::ParameterSet &, EtaPhiList &, stringstream &) const;
    void envSet (const edm::EventSetup &);
    int getChannelStatusMaps ();

    edm::ESHandle<CaloGeometry> caloGeometry_;
    edm::ESHandle<EcalChannelStatus> ecalStatus_;
    //const CaloGeometry& caloGeometry_;
    //const EcalChannelStatus& ecalStatus_;

    edm::ESGetToken<CaloGeometry, CaloGeometryRecord> caloGeometryToken_;
    edm::ESGetToken<EcalChannelStatus, EcalChannelStatusRcd> ecalStatusToken_;

    const bool insideCone(TYPE(tracks)& track, const DetId& id, const double dR) const;
    const GlobalPoint getPosition( const DetId& id) const;

    int maskedEcalChannelStatusThreshold_;
    bool outputBadEcalChannels_;

    map<DetId, vector<double> > EcalAllDeadChannelsValMap_;
    map<DetId, vector<int> >    EcalAllDeadChannelsBitMap_;

    const double getTrackIsolation (TYPE(tracks)&, const vector<TYPE(tracks)> &, const bool, const bool, const double, const double = 1.0e-12) const;
    const double getOldTrackIsolation (TYPE(tracks)&, const vector<reco::Track> &, const bool, const double, const double = 1.0e-12) const;
    const CaloEnergy calculateCaloE (TYPE(tracks)&, const EBRecHitCollection &, const EERecHitCollection &, const HBHERecHitCollection &, const double dR = 0.5) const;

};

#endif
