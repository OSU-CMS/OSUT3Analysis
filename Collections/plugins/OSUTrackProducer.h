#ifndef TRACK_PRODUCER
#define TRACK_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "OSUT3Analysis/Collections/interface/Track.h"

class OSUTrackProducer : public edm::EDProducer
{
  public:
    OSUTrackProducer (const edm::ParameterSet &);
    ~OSUTrackProducer ();

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
    edm::EDGetTokenT<vector<TYPE(tracks)> >      token_;
    edm::EDGetTokenT<vector<osu::Mcparticle> >   mcparticleToken_;
    edm::EDGetTokenT<vector<TYPE(jets)> >        jetsToken_;
    edm::EDGetTokenT<EBRecHitCollection>         EBRecHitsToken_;
    edm::EDGetTokenT<EERecHitCollection>         EERecHitsToken_;
    edm::EDGetTokenT<HBHERecHitCollection>       HBHERecHitsToken_;
    edm::EDGetTokenT<vector<reco::GsfTrack> >    gsfTracksToken_;
    edm::EDGetTokenT<vector<reco::Track> >       tracksToken_;
    edm::ParameterSet  cfg_;
    ////////////////////////////////////////////////////////////////////////////

    EtaPhiList electronVetoList_;
    EtaPhiList muonVetoList_;

    // Payload for this EDFilter.
    unique_ptr<vector<osu::Track> > pl_;

    void extractFiducialMap (const edm::ParameterSet &, EtaPhiList &, stringstream &) const;
    void envSet (const edm::EventSetup &);
    int getChannelStatusMaps ();

    edm::ESHandle<CaloGeometry> caloGeometry_;
    edm::ESHandle<EcalChannelStatus> ecalStatus_;
    bool insideCone(TYPE(tracks)& track, const DetId& id, const double dR);
    GlobalPoint getPosition( const DetId& id);

    int maskedEcalChannelStatusThreshold_;
    bool outputBadEcalChannels_;

    map<DetId, vector<double> > EcalAllDeadChannelsValMap_;
    map<DetId, vector<int> >    EcalAllDeadChannelsBitMap_;

    const double getTrackIsolation (const reco::Track &, const vector<reco::Track> &, const bool, const bool, const double, const double = 1.0e-12) const;
    const double getOldTrackIsolation (const reco::Track &, const vector<reco::Track> &, const bool, const double, const double = 1.0e-12) const;
};

#endif
