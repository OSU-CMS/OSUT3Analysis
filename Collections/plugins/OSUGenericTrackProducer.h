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
#include "DataFormats/TrackReco/interface/DeDxHit.h"
#include "DataFormats/TrackReco/interface/DeDxHitInfo.h"
#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "RecoTracker/DeDx/interface/DeDxTools.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DisappTrksML/TreeMaker/interface/Infos.h"
#include "OSUT3Analysis/Collections/interface/Met.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "OSUT3Analysis/Collections/interface/DisappearingTrack.h" // includes TrackBase.h

#include "DataFormats/TrackReco/interface/DeDxHit.h"
#include "DataFormats/TrackReco/interface/DeDxHitInfo.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "RecoTracker/DeDx/interface/DeDxTools.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"


#include "DisappTrksML/TreeMaker/interface/Infos.h"
#include "OSUT3Analysis/Collections/interface/Met.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"

#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

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

enum DetType { None, EB, EE, ES, HCAL, CSC, DT, RPC };

struct CacheData {
  CacheData() : graphDef(nullptr), graphDefDS(nullptr) {}
  std::atomic<tensorflow::GraphDef*> graphDef;
  std::atomic<tensorflow::GraphDef*> graphDefDS;
};

template<class T>
  class OSUGenericTrackProducer : public edm::stream::EDProducer<edm::GlobalCache<CacheData> >
{
  public:
    OSUGenericTrackProducer (const edm::ParameterSet &, const CacheData*);
    ~OSUGenericTrackProducer ();

    static std::unique_ptr<CacheData> initializeGlobalCache(const edm::ParameterSet&);
    static void globalEndJob(const CacheData*);


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
    edm::EDGetTokenT<double>  rhoToken_;
    edm::EDGetTokenT<double>  rhoCaloToken_;
    edm::EDGetTokenT<double>  rhoCentralCaloToken_;
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
    edm::EDGetTokenT<vector<osu::Met> > metToken_;

    edm::EDGetTokenT<edm::TriggerResults> triggersToken_;
    edm::EDGetTokenT<vector<pat::TriggerObjectStandAlone> > trigObjsToken_;
    edm::EDGetTokenT<reco::DeDxHitInfoAss> isoTrk2dedxHitInfoToken_;
    edm::EDGetTokenT<edm::View<PileupSummaryInfo> > pileupInfoToken_;
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
    edm::ESHandle<TrackerTopology> trackerTopology_;

    edm::ESGetToken<CaloGeometry, CaloGeometryRecord> caloGeometryToken_;
    edm::ESGetToken<EcalChannelStatus, EcalChannelStatusRcd> ecalStatusToken_;
    edm::ESGetToken<TrackerTopology, TrackerTopologyRcd> trackerTopologyToken_;

    TrackInfo trackInfo;
    vector<pat::Electron> tagElectrons;
    vector<pat::Muon> tagMuons;

    tensorflow::Tensor inputDS;
    tensorflow::Tensor inputTrackDS;
    std::vector<tensorflow::Tensor> outputsDS;
    std::vector<tensorflow::Tensor> outputs;

    float scoreDS = 0.0;
    float score = 0.0;
    std::vector<std::vector<double>> hitMap;
    std::pair<double, double> maxHits;
    unsigned long encodedLayers;
    std::pair<std::array<double, 3>, std::array<double, 3>> closest_vtx;

    const bool insideCone(TYPE(tracks)& track, const DetId& id, const double dR) const;
    const GlobalPoint getPosition( const DetId& id) const;

    int maskedEcalChannelStatusThreshold_;
    bool outputBadEcalChannels_;

    map<DetId, vector<double> > EcalAllDeadChannelsValMap_;
    map<DetId, vector<int> >    EcalAllDeadChannelsBitMap_;

    const double getTrackIsolation (TYPE(tracks)&, const vector<TYPE(tracks)> &, const bool, const bool, const double, const double = 1.0e-12) const;
    const double getOldTrackIsolation (TYPE(tracks)&, const vector<reco::Track> &, const bool, const double, const double = 1.0e-12) const;
    const CaloEnergy calculateCaloE (TYPE(tracks)&, const EBRecHitCollection &, const EERecHitCollection &, const HBHERecHitCollection &, const double dR = 0.5) const;


    //functions and constants for machine learning

    int countGoodPrimaryVertices(const vector<reco::Vertex> &) const;
    int countGoodJets(const vector<pat::Jet> &) const;
    double getMaxDijetDeltaPhi(const vector<pat::Jet> &) const;
    double getLeadingJetMetPhi(const vector<pat::Jet> &, const pat::MET &) const;
    void getTracks(const reco::Vertex &, 
                  const vector<pat::Jet> &,
                  const edm::View<pat::Electron> &,
                  const vector<pat::Muon> &,
                  const vector<pat::Tau> &,
                  const vector<pat::Electron> &tagElectrons,
                  const vector<pat::Muon> &tagMuons,
                  const pat::MET &, 
                  const edm::Handle<vector<pat::IsolatedTrack> >, 
                  const edm::Handle<reco::DeDxHitInfoAss>,
                  const edm::Handle<edm::ValueMap<reco::DeDxData> > dEdxStrip,
                  const edm::Handle<edm::ValueMap<reco::DeDxData> > dEdxPixel);

    void getRecHits(const edm::Event &);
    void getGenParticles(const reco::CandidateView &);

    vector<pat::Electron> getTagElectrons(const edm::Event &,
                                        const edm::TriggerResults &,
                                        const vector<pat::TriggerObjectStandAlone> &,
                                        const reco::Vertex &,
                                        const edm::View<pat::Electron> &,
                                        const edm::Handle<edm::ValueMap<bool>> &);

    vector<pat::Muon> getTagMuons(const edm::Event &,
                                const edm::TriggerResults &,
                                const vector<pat::TriggerObjectStandAlone> &,
                                const reco::Vertex &,
                                const vector<pat::Muon> &);

    TrackInfo getTrackInfo(const T &track,
                                const reco::Vertex &pv, 
                                const vector<pat::Jet> &jets,
                                const edm::View<pat::Electron> &electrons,
                                const vector<pat::Muon> &muons,
                                const vector<pat::Tau> &taus,
                                const vector<pat::Electron> &tagElectrons,
                                const vector<pat::Muon> &tagMuons,
                                const pat::MET &met,
                                const edm::Handle<vector<pat::IsolatedTrack> > tracks, 
                                const edm::Handle<reco::DeDxHitInfoAss> isoTrk2dedxHitInfo,
                                const edm::Handle<edm::ValueMap<reco::DeDxData> > dEdxStrip,
                                const edm::Handle<edm::ValueMap<reco::DeDxData> > dEdxPixel);

    const bool isProbeTrack(const TrackInfo) const;

    // bit values reflect tag+probe status and charge products:
    //     0b<in same-sign pair><in opposite-sign pair>
    //     So 0 = 0b00 : not in any TP pair
    //        1 = 0b01 : in OS TP pair
    //        2 = 0b10 : in SS TP pair
    //        3 = 0b11 : in both an OS and SS pair
    const unsigned int isTagProbeElePair(const pat::IsolatedTrack &, const pat::Electron &) const;
    const unsigned int isTagProbeMuonPair(const pat::IsolatedTrack &, const pat::Muon &) const;
    const unsigned int isTagProbeTauToElePair(const pat::IsolatedTrack &, const pat::Electron &, const pat::MET &) const;
    const unsigned int isTagProbeTauToMuonPair(const pat::IsolatedTrack &, const pat::Muon &, const pat::MET &) const;

    const double minDRBadEcalChannel(const pat::IsolatedTrack &) const;

    std::vector<std::vector<double>> getHitMap(const vector<TrackDeDxInfo>) const;
    std::pair<double, double> getMaxHits(const vector<TrackDeDxInfo>) const;
    unsigned long encodeLayers(const std::vector<std::vector<double>>) const;
    std::pair<std::array<double, 3>, std::array<double, 3>> getClosestVertices(const std::vector<VertexInfo>, float track_vz, float track_vx, float track_vy) const;
    int getDetectorIndex(const int) const;

    std::string inputTensorName_;
    std::string outputTensorName_;
    std::string inputTensorNameDS_;
    std::string outputTensorNameDS_;
    std::string inputTrackTensorNameDS_;

    tensorflow::GraphDef* graphDef_;
    tensorflow::GraphDef* graphDefDS_;
    tensorflow::Session* session_;
    tensorflow::Session* sessionDS_;

    tensorflow::Tensor input_;

    vector<TrackInfo> trackInfos_;
    vector<RecHitInfo> recHitInfos_;
    vector<GenParticleInfo> genParticleInfos_;
    vector<VertexInfo> vertexInfos_;

    double minGenParticlePt_;
    double minTrackPt_;
    double maxRelTrackIso_;

    const string dataTakingPeriod_;

    // event information  
    int nPV_;
    unsigned long long eventNumber_;
    unsigned int lumiBlockNumber_;
    unsigned int runNumber_;
    vector<float> pileupZPosition_;
    unsigned int numTruePV_;

    // event-wide cut results
    bool firesGrandOrTrigger_;
    bool passMETFilters_;
    int numGoodPVs_;
    double metNoMu_;
    double numGoodJets_;
    double dijetDeltaPhiMax_;
    double leadingJetMetPhi_;

    bool is2017_;

    vector<string> signalTriggerNames, metFilterNames;

    // recHits range
    double EtaRange_ = 0.25;
    double PhiRange_ = 0.25;
    int maxHits_ = 100;

    std::string muonTriggerFilter_;

};

#endif
