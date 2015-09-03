class MuonID:
  pass

run2 = MuonID ()

# based on https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonIdRun2#Tight_Muon
run2.tightID_displaced = "                                                              \
                              isGlobalMuon                                              \
                           && isPFMuon                                                  \
                           && (globalTrack.normalizedChi2 < 10)                         \
                           && (globalTrack.hitPattern_.numberOfValidMuonHits > 0)       \
                           && (numberOfMatchedStations > 1)                             \
                           && (innerTrack.hitPattern_.numberOfValidPixelHits > 0)       \
                           && (innerTrack.hitPattern_.trackerLayersWithMeasurement > 5) \
                         "
