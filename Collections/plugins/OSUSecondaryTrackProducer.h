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

#include "OSUT3Analysis/Collections/interface/SecondaryTrack.h"

class OSUSecondaryTrackProducer : public edm::EDProducer
{
  public:
    OSUSecondaryTrackProducer (const edm::ParameterSet &);
    ~OSUSecondaryTrackProducer ();

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
    edm::EDGetTokenT<vector<TYPE(secondaryTracks)> >      token_;
    edm::EDGetTokenT<vector<osu::Mcparticle> >   mcparticleToken_;
    edm::EDGetTokenT<vector<TYPE(jets)> >        jetsToken_;
    edm::EDGetTokenT<EBRecHitCollection>         EBRecHitsToken_;
    edm::EDGetTokenT<EERecHitCollection>         EERecHitsToken_;
    edm::EDGetTokenT<HBHERecHitCollection>       HBHERecHitsToken_;
    edm::EDGetTokenT<vector<reco::GsfTrack> >    gsfTracksToken_;
    edm::ParameterSet  cfg_;
    ////////////////////////////////////////////////////////////////////////////

    EtaPhiList electronVetoList_;
    EtaPhiList muonVetoList_;

    // Payload for this EDFilter.
    unique_ptr<vector<osu::SecondaryTrack> > pl_;

    void extractFiducialMap (const edm::ParameterSet &, EtaPhiList &, stringstream &) const;

    edm::ESHandle<CaloGeometry> caloGeometry_;
    bool insideCone(TYPE(secondaryTracks)& secondaryTrack, const DetId& id, const double dR);
    GlobalPoint getPosition( const DetId& id);

};

#endif
