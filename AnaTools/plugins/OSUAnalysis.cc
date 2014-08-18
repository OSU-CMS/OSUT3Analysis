#include "OSUT3Analysis/AnaTools/plugins/OSUAnalysis.h"
#include "LHAPDF/LHAPDF.h"    

OSUAnalysis::OSUAnalysis (const edm::ParameterSet &cfg) :
  /// Retrieve parameters from the configuration file.
  jets_ (cfg.getParameter<edm::InputTag> ("jets")),
  muons_ (cfg.getParameter<edm::InputTag> ("muons")),
  secMuons_ (cfg.getParameter<edm::InputTag> ("secMuons")),
  electrons_ (cfg.getParameter<edm::InputTag> ("electrons")),
  events_ (cfg.getParameter<edm::InputTag> ("events")),
  taus_ (cfg.getParameter<edm::InputTag> ("taus")),
  mets_ (cfg.getParameter<edm::InputTag> ("mets")),
  tracks_ (cfg.getParameter<edm::InputTag> ("tracks")),
  genjets_ (cfg.getParameter<edm::InputTag> ("genjets")),
  mcparticles_ (cfg.getParameter<edm::InputTag> ("mcparticles")),
  stops_ (cfg.getParameter<edm::InputTag> ("stops")),
  primaryvertexs_ (cfg.getParameter<edm::InputTag> ("primaryvertexs")),
  bxlumis_ (cfg.getParameter<edm::InputTag> ("bxlumis")),
  photons_ (cfg.getParameter<edm::InputTag> ("photons")),
  superclusters_ (cfg.getParameter<edm::InputTag> ("superclusters")),
  triggers_ (cfg.getParameter<edm::InputTag> ("triggers")),
  trigobjs_ (cfg.getParameter<edm::InputTag> ("trigobjs")),
  puFile_ (cfg.getParameter<string> ("puFile")),
  deadEcalFile_ (cfg.getParameter<string> ("deadEcalFile")),
  badCSCFile_ (cfg.getParameter<string> ("badCSCFile")),
  electronSFFile_ (cfg.getParameter<string> ("electronSFFile")),
  muonSFFile_ (cfg.getParameter<string> ("muonSFFile")),
  jESJERCorr_(cfg.getParameter<string> ("jESJERCorr")),
  targetTriggers_(cfg.getParameter<vector <string> > ("targetTriggers")),
  flagJESJERCorr_(cfg.getParameter<bool> ("flagJESJERCorr")),
  triggerMetSFFile_ (cfg.getParameter<string> ("triggerMetSFFile")),
  trackNMissOutSFFile_ (cfg.getParameter<string> ("trackNMissOutSFFile")),
  EcaloVarySFFile_ (cfg.getParameter<string> ("EcaloVarySFFile")),
  muonCutFile_ (cfg.getParameter<string> ("muonCutFile")),
  muonCut_ (cfg.getParameter<string> ("muonCut")),
  electronCutFile_ (cfg.getParameter<string> ("electronCutFile")),
  electronCut_ (cfg.getParameter<string> ("electronCut")),
  recoMuonFile_ (cfg.getParameter<string> ("recoMuonFile")),
  recoMuon_ (cfg.getParameter<string> ("recoMuon")),
  recoElectronFile_ (cfg.getParameter<string> ("recoElectronFile")),
  recoElectron_ (cfg.getParameter<string> ("recoElectron")),
  isrVarySFFile_ (cfg.getParameter<string> ("isrVarySFFile")),
  dataPU_ (cfg.getParameter<string> ("dataPU")),
  electronSFID_ (cfg.getParameter<string> ("electronSFID")),
  electronSF_ (cfg.getParameter<string> ("electronSF")),
  muonSF_ (cfg.getParameter<string> ("muonSF")),
  triggerMetSF_ (cfg.getParameter<string> ("triggerMetSF")),
  trackNMissOutSF_ (cfg.getParameter<string> ("trackNMissOutSF")),
  EcaloVarySF_ (cfg.getParameter<string> ("EcaloVarySF")),
  isrVarySF_ (cfg.getParameter<string> ("isrVarySF")),
  dataset_ (cfg.getParameter<string> ("dataset")),
  datasetType_ (cfg.getParameter<string> ("datasetType")),
  channels_  (cfg.getParameter<vector<edm::ParameterSet> >("channels")),
  histogramSets_ (cfg.getParameter<vector<edm::ParameterSet> >("histogramSets")),
  useEDMFormat_   (cfg.getParameter<bool>("useEDMFormat")),
  treeBranchSets_   (cfg.getParameter<vector<edm::ParameterSet> >("treeBranchSets")),
  plotAllObjectsInPassingEvents_ (cfg.getParameter<bool> ("plotAllObjectsInPassingEvents")),
  doPileupReweighting_ (cfg.getParameter<bool> ("doPileupReweighting")),
  doTopPtReweighting_  (cfg.getParameter<bool> ("doTopPtReweighting")),
  applyTriggerSF_ (cfg.getParameter<bool> ("applyTriggerSF")),
  triggerScaleFactor_ (cfg.getParameter<double> ("triggerScaleFactor")),
  applyLeptonSF_ (cfg.getParameter<bool> ("applyLeptonSF")),
  applyGentoRecoEfficiency_ (cfg.getParameter<bool> ("applyGentoRecoEfficiency")),
  applyTrackingSF_ (cfg.getParameter<bool> ("applyTrackingSF")),
  applyBtagSF_ (cfg.getParameter<bool> ("applyBtagSF")),
  minBtag_ (cfg.getParameter<int> ("minBtag")),
  calcPdfWeights_ (cfg.getParameter<bool> ("calcPdfWeights")),  
  pdfSet_ (cfg.getParameter<string> ("pdfSet")),  
  pdfSetFlag_ (cfg.getParameter<int> ("pdfSetFlag")),  
  electronSFShift_ (cfg.getParameter<string> ("electronSFShift")),
  muonSFShift_ (cfg.getParameter<string> ("muonSFShift")),
  triggerMetSFShift_ (cfg.getParameter<string> ("triggerMetSFShift")),
  trackNMissOutSFShift_ (cfg.getParameter<string> ("trackNMissOutSFShift")),
  NmissoutShiftUp_(cfg.getParameter<bool> ("NmissoutShiftUp")),
  NmissoutShiftDown_(cfg.getParameter<bool> ("NmissoutShiftDown")),
  EcaloVarySFShift_ (cfg.getParameter<string> ("EcaloVarySFShift")),
  isrVarySFShift_ (cfg.getParameter<string> ("isrVarySFShift")),
  trackSFShift_ (cfg.getParameter<string> ("trackSFShift")),
  printEventInfo_      (cfg.getParameter<bool> ("printEventInfo")),
  printAllTriggers_    (cfg.getParameter<bool> ("printAllTriggers")),
  useTrackCaloRhoCorr_ (cfg.getParameter<bool> ("useTrackCaloRhoCorr")),
  stopCTau_ (cfg.getParameter<vector<double> > ("stopCTau")),
  GetPlotsAfterEachCut_ (cfg.getParameter<bool> ("GetPlotsAfterEachCut")),
  verbose_ (cfg.getParameter<int> ("verbose"))
{
  
  //  if (verbose_) printEventInfo_ = true;
  if (verbose_) clog << "Beginning OSUAnalysis::OSUAnalysis constructor." << endl;
  
  if (verbose_) clog << "Using the following parameters:  " << endl
		     << "  jets_                            = " <<  jets_                            << endl   
		     << "  muons_                           = " <<  muons_                           << endl   
		     << "  secMuons_                        = " <<  secMuons_                        << endl   
		     << "  electrons_                       = " <<  electrons_                       << endl   
		     << "  events_                          = " <<  events_                          << endl   
		     << "  taus_                            = " <<  taus_                            << endl   
		     << "  mets_                            = " <<  mets_                            << endl   
		     << "  tracks_                          = " <<  tracks_                          << endl   
		     << "  genjets_                         = " <<  genjets_                         << endl   
		     << "  mcparticles_                     = " <<  mcparticles_                     << endl   
		     << "  stops_                           = " <<  stops_                           << endl   
		     << "  primaryvertexs_                  = " <<  primaryvertexs_                  << endl   
		     << "  bxlumis_                         = " <<  bxlumis_                         << endl   
		     << "  photons_                         = " <<  photons_                         << endl   
		     << "  superclusters_                   = " <<  superclusters_                   << endl   
		     << "  triggers_                        = " <<  triggers_                        << endl   
		     << "  trigobjs_                        = " <<  trigobjs_                        << endl   
		     << "  puFile_                          = " <<  puFile_                          << endl   
		     << "  deadEcalFile_                    = " <<  deadEcalFile_                    << endl   
		     << "  badCSCFile_                      = " <<  badCSCFile_                      << endl   
		     << "  electronSFFile_                  = " <<  electronSFFile_                  << endl   
		     << "  muonSFFile_                      = " <<  muonSFFile_                      << endl   
		     << "  jESJERCorr_                      = " <<  jESJERCorr_                      << endl   
		     << "  flagJESJERCorr_                  = " <<  flagJESJERCorr_                  << endl   
		     << "  triggerMetSFFile_                = " <<  triggerMetSFFile_                << endl   
		     << "  trackNMissOutSFFile_             = " <<  trackNMissOutSFFile_             << endl   
		     << "  muonCutWeight_             = " <<  muonCutWeight_             << endl   
    		     << "  electronCutWeight_             = " <<  electronCutWeight_             << endl  
		     << "  recoMuonWeight_             = " <<  recoMuonWeight_             << endl   
		     << "  recoElectronWeight_             = " <<  recoElectronWeight_             << endl   
		     << "  isrVarySFFile_                   = " <<  isrVarySFFile_                   << endl   
		     << "  dataPU_                          = " <<  dataPU_                          << endl   
		     << "  electronSFID_                    = " <<  electronSFID_                    << endl   
		     << "  electronSF_                      = " <<  electronSF_                      << endl   
		     << "  muonSF_                          = " <<  muonSF_                          << endl   
		     << "  triggerMetSF_                    = " <<  triggerMetSF_                    << endl   
		     << "  trackNMissOutSF_                 = " <<  trackNMissOutSF_                 << endl   
		     << "  muonCut_                 = " <<  muonCut_                 << endl   
		     << "  electronCut_                 = " <<  electronCut_                 << endl   
		     << "  recoMuon_                 = " <<  recoMuon_                 << endl   
		     << "  recoElectron_                 = " <<  recoElectron_                 << endl   
		     << "  isrVarySF_                       = " <<  isrVarySF_                       << endl   
		     << "  dataset_                         = " <<  dataset_                         << endl   
		     << "  datasetType_                     = " <<  datasetType_                     << endl   
    //		     << "  channels_                        = " <<  channels_                        << endl   
    //		     << "  histogramSets_                   = " <<  histogramSets_                   << endl   
		     << "  useEDMFormat_                    = " <<  useEDMFormat_                    << endl   
    //		     << "  treeBranchSets_                  = " <<  treeBranchSets_                  << endl   
		     << "  plotAllObjectsInPassingEvents_   = " <<  plotAllObjectsInPassingEvents_   << endl   
		     << "  doPileupReweighting_             = " <<  doPileupReweighting_             << endl   
		     << "  doTopPtReweighting_              = " <<  doTopPtReweighting_              << endl   
		     << "  applyTriggerSF_                  = " <<  applyTriggerSF_                  << endl   
		     << "  applyGentoRecoEfficiency_        = " <<  applyGentoRecoEfficiency_        << endl   
		     << "  triggerScaleFactor_              = " <<  triggerScaleFactor_              << endl   
		     << "  applyLeptonSF_                   = " <<  applyLeptonSF_                   << endl   
		     << "  applyTrackingSF_                 = " <<  applyTrackingSF_                 << endl   
		     << "  applyBtagSF_                     = " <<  applyBtagSF_                     << endl   
		     << "  minBtag_                         = " <<  minBtag_                         << endl   
		     << "  calcPdfWeights_                  = " <<  calcPdfWeights_                  << endl   
		     << "  pdfSet_                          = " <<  pdfSet_                          << endl   
		     << "  pdfSetFlag_                      = " <<  pdfSetFlag_                      << endl   
		     << "  electronSFShift_                 = " <<  electronSFShift_                 << endl   
		     << "  muonSFShift_                     = " <<  muonSFShift_                     << endl   
		     << "  triggerMetSFShift_               = " <<  triggerMetSFShift_               << endl   
		     << "  trackNMissOutSFShift_            = " <<  trackNMissOutSFShift_            << endl   
		     << "  isrVarySFShift_                  = " <<  isrVarySFShift_                  << endl   
		     << "  trackSFShift_                    = " <<  trackSFShift_                    << endl   
		     << "  printEventInfo_                  = " <<  printEventInfo_                  << endl   
		     << "  printAllTriggers_                = " <<  printAllTriggers_                << endl   
		     << "  useTrackCaloRhoCorr_             = " <<  useTrackCaloRhoCorr_             << endl   
    //		     << "  stopCTau_                        = " <<  stopCTau_                        << endl   
		     << "  GetPlotsAfterEachCut_            = " <<  GetPlotsAfterEachCut_            << endl   
		     << "  verbose_                         = " <<  verbose_                         << endl   
		     << endl;  

  TH1::SetDefaultSumw2();

  //create reweighting objects, if necessary 

  if (calcPdfWeights_) {
    LHAPDF::initPDFSet(pdfSetFlag_, pdfSet_.c_str()); 
    
    // PDFsets tried:  
    //LHAPDF::initPDFSet(1, "cteq66.LHgrid"); 
    //LHAPDF::initPDFSet(1, "CT10.LHgrid"); 
    //LHAPDF::initPDFSet(1, "cteq6ll.LHpdf"); 
    //LHAPDF::initPDFSet(2, "cteq66alphas.LHgrid");
    //LHAPDF::initPDFSet(1, "MSTW2008nlo68cl.LHgrid");
    //LHAPDF::initPDFSet(2, "MSTW2008nlo68cl_asmz+68cl.LHgrid");
    //LHAPDF::initPDFSet(3, "MSTW2008nlo68cl_asmz-68cl.LHgrid");
    //LHAPDF::initPDFSet(1,"NNPDF20_100.LHgrid");  //This is the central with as=0.119 
    //LHAPDF::initPDFSet(6,"NNPDF20_as_0116_100.LHgrid"); //Need to fix max allowed sets 
    //LHAPDF::initPDFSet(4,"NNPDF20_as_0117_100.LHgrid");
    //LHAPDF::initPDFSet(2,"NNPDF20_as_0118_100.LHgrid");
    //LHAPDF::initPDFSet(3,"NNPDF20_as_0120_100.LHgrid");
    //LHAPDF::initPDFSet(5,"NNPDF20_as_0121_100.LHgrid");
    //LHAPDF::initPDFSet(7,"NNPDF20_as_0122_100.LHgrid");
    
    ///Initialize PDF study counters
    passedSF=0.0;
    allSF=0.0;
    allNo=0;
    passedNo=0;
    
    passedSF2=0.0;
    allSF2=0.0;
    
    passedCentralW2=0.0;
    allCentralW2=0.0;
    
    
    //    clog << "Debug:  LHAPDF::numberPDF() = " << LHAPDF::numberPDF() << endl;  
    for(int i=0;i<=LHAPDF::numberPDF();i++){
      allSums.push_back(0.0);
      passedSums.push_back(0.0);
    }
  }  


  if(datasetType_ != "data") {
    if(doPileupReweighting_) puWeight_ = new PUWeight (puFile_, dataPU_, dataset_);
    if (applyLeptonSF_){
      muonSFWeight_ = new MuonSFWeight (muonSFFile_, muonSF_);
      electronSFWeight_ = new ElectronSFWeight ("53X", electronSFID_, electronSFFile_, electronSF_);
    }
    if(applyTrackingSF_) {
      trackSFWeight_ = new TrackSFWeight ();
    }
    if (applyBtagSF_){
      bTagSFWeight_ = new BtagSFWeight;
    }
    if (triggerMetSFFile_ != "" ){
      triggerMetSFWeight_ = new TriggerMetSFWeight(triggerMetSFFile_, triggerMetSF_);  
    }
    if (trackNMissOutSFFile_ != "" ){
      trackNMissOutSFWeight_ = new TrackNMissOutSFWeight(trackNMissOutSFFile_, trackNMissOutSF_);  
    }
    if (EcaloVarySFFile_ != "" ){
      EcaloVarySFWeight_ = new EcaloVarySFWeight(EcaloVarySFFile_, EcaloVarySF_);  
    }
    if(applyGentoRecoEfficiency_){
      muonCutWeight_ = new MuonCutWeight(muonCutFile_, muonCut_);  
      electronCutWeight_ = new ElectronCutWeight(electronCutFile_, electronCut_);
      recoElectronWeight_ = new RecoElectronWeight(recoElectronFile_, recoElectron_);
      recoMuonWeight_ = new RecoMuonWeight(recoMuonFile_, recoMuon_);
    }
    if (isrVarySFFile_ != "" ){
      if (isrVarySFFile_.find("XXX")!=string::npos) { 
	// Assumes that isrVarySFFile_ has the form "compareIsr*XXXK*Ratio.root and that 
	// dataset_ has form "AMSB_mGravXXXK_*  
	size_t pos = dataset_.find("mGrav");  
	if (pos==string::npos) clog << "Error:  could not find mass for ISR file in dataset name:  " << dataset_ << endl;  
	string strMass = dataset_.substr(pos+5);  
	pos = strMass.find("K");  
	strMass = strMass.substr(0, pos);
	isrVarySFFile_.replace(isrVarySFFile_.find("XXX"), string("XXX").length(), strMass);  
	clog << "Modified isrVarySFFile_ to be:  " << isrVarySFFile_ << endl;  
      }
      isrVarySFWeight_ = new IsrVarySFWeight(isrVarySFFile_, isrVarySF_);  
    }
  }

  if (datasetType_ == "signalMC" && 
      (regex_match (dataset_, regex ("stop.*to.*_.*mm.*")) || 
       //       regex_match (dataset_, regex ("AMSB.*RewtCtau.*cm")))) {
       regex_match (dataset_, regex ("AMSB.*cm")))) {
    //    if (verbose_) 
    clog << "Setting stopctau with:  stopCTau_.at(0)=" << stopCTau_.at(0) << "; stopCTau_.at(1)=" << stopCTau_.at(1) << endl;  
    stopCTauWeight_ = new StopCTauWeight (stopCTau_.at(0), stopCTau_.at(1), stops_);
  }  

  // Construct Cutflow Objects. These store the results of cut decisions and
  // handle filling cut flow histograms.
  masterCutFlow_ = new CutFlow (fs_);

  //always get vertex collection so we can assign the primary vertex in the event
  objectsToGet.push_back("primaryvertexs");
  objectsToPlot.push_back("primaryvertexs");
  objectsToFlag.push_back("primaryvertexs");


  //always get the MC particles to do GEN-matching
  objectsToGet.push_back("mcparticles");

  //always get the event collection to do pile-up reweighting
  objectsToGet.push_back("events");


  // Parse the tree variable definitions.
  for (uint iBranchSet = 0; !useEDMFormat_ && iBranchSet<treeBranchSets_.size(); iBranchSet++) {
    string tempInputCollection = treeBranchSets_.at(iBranchSet).getParameter<string> ("inputCollection");
    if(tempInputCollection.find("pairs")!=string::npos) { clog << "Warning:  tree filling is not configured for pairs of objects, so will not work for collection: " << tempInputCollection << endl; }
    objectsToGet.push_back(tempInputCollection);
    objectsToFlag.push_back(tempInputCollection);

    vector<string> branchList(treeBranchSets_.at(iBranchSet).getParameter<vector<string> >("branches"));

    for (uint iBranch = 0; iBranch<branchList.size(); iBranch++) {
      BranchSpecs br;
      br.inputCollection = tempInputCollection;
      br.inputVariable = branchList.at(iBranch);
      TString newName = TString(br.inputCollection) + "_" + TString(br.inputVariable);
      br.name = string(newName.Data());
      treeBranches_.push_back(br);
      if (verbose_>3) clog << "   Adding branch to BNTree: " << br.name << endl;
    }

  } // end   for (uint iBranchSet = 0; iBranchSet<treeBranchSets_.size(); iBranchSet++)


  //parse the histogram definitions
  for(uint currentHistogramSet = 0; currentHistogramSet != histogramSets_.size(); currentHistogramSet++){

    string tempInputCollection = histogramSets_.at(currentHistogramSet).getParameter<string> ("inputCollection");
    if(tempInputCollection == "muon-electron pairs") tempInputCollection = "electron-muon pairs";
    if(tempInputCollection == "photon-muon pairs") tempInputCollection = "muon-photon pairs";
    if(tempInputCollection == "photon-electron pairs") tempInputCollection = "electron-photon pairs";
    if(tempInputCollection == "jet-electron pairs") tempInputCollection = "electron-jet pairs";
    if(tempInputCollection == "jet-photon pairs") tempInputCollection = "photon-jet pairs";
    if(tempInputCollection == "jet-muon pairs") tempInputCollection = "muon-jet pairs";
    if(tempInputCollection == "event-muon pairs") tempInputCollection = "muon-event pairs";
    if(tempInputCollection == "event-electron pairs") tempInputCollection = "electron-event pairs";
    if(tempInputCollection == "jet-met pairs")  tempInputCollection = "met-jet pairs";
    if(tempInputCollection == "mcparticle-met pairs")  tempInputCollection = "met-mcparticle pairs";
    if(tempInputCollection == "secondary mcparticle-jet pairs")  tempInputCollection = "jet-secondary mcparticle pairs";
    if(tempInputCollection == "mcparticle-jet pairs")  tempInputCollection = "jet-mcparticle pairs";
    if(tempInputCollection == "secondary mcparticle-mcparticle pairs")  tempInputCollection = "mcparticle-secondary mcparticle pairs";
    if(tempInputCollection == "track-jet pairs")  tempInputCollection = "track-jet pairs";
    if(tempInputCollection == "event-track pairs")   tempInputCollection = "track-event pairs";
    if(tempInputCollection == "secondary muon-muon pairs")   tempInputCollection = "muon-secondary muon pairs";
    if(tempInputCollection == "secondary jet-muon pairs")   tempInputCollection = "muon-secondary jet pairs";
    if(tempInputCollection == "jet-secondary muon pairs")   tempInputCollection = "secondary muon-jet pairs";
    if(tempInputCollection == "secondary photon-muon pairs")   tempInputCollection = "muon-secondary photon pairs";
    if(tempInputCollection == "secondary jet-electron pairs")   tempInputCollection = "electron-secondary jet pairs";
    if(tempInputCollection == "jet-secondary electron pairs")   tempInputCollection = "secondary electron-jet pairs";
    if(tempInputCollection == "secondary jet-photon pairs")   tempInputCollection = "photon-secondary jet pairs";
    if(tempInputCollection == "secondary jet-jet pairs")   tempInputCollection = "jet-secondary jet pairs";
    if(tempInputCollection == "secondary electron-electron pairs")   tempInputCollection = "electron-secondary electron pairs";
    if(tempInputCollection == "trigobj-electron pairs")   tempInputCollection = "electron-trigobj pairs";
    if(tempInputCollection == "trigobj-muon pairs")   tempInputCollection = "muon-trigobj pairs";
    if(tempInputCollection == "mcparticle-electron pairs")   tempInputCollection = "electron-mcparticle pairs";
    if(tempInputCollection == "mcparticle-muon pairs")   tempInputCollection = "muon-mcparticle pairs";
    if(tempInputCollection == "secondary mcparticle-muon pairs")   tempInputCollection = "muon-secondary mcparticle pairs";
    if(tempInputCollection == "mcparticle-track pairs")   tempInputCollection = "track-mcparticle pairs";
    if(tempInputCollection == "mcparticle-stop pairs")    tempInputCollection = "stop-mcparticle pairs";    
    if(tempInputCollection.find("pairs")==string::npos){ //just a single object
      if(tempInputCollection.find("secondary")!=string::npos){//secondary object
        if(tempInputCollection.find("secondary muons")!=string::npos){//treat secondary muons differently; allow for a different input collection
          objectsToGet.push_back("secondary muons");
        } else {
          int spaceIndex = tempInputCollection.find(" ");
          int secondWordLength = tempInputCollection.size() - spaceIndex;
          objectsToGet.push_back(tempInputCollection.substr(spaceIndex+1,secondWordLength));
        }
      } else{
        objectsToGet.push_back(tempInputCollection);
      }
      objectsToPlot.push_back(tempInputCollection);
      objectsToFlag.push_back(tempInputCollection);
    } else { //pair of objects, need to add the pair and the individual objects to the lists of things to Get/Plot/Cut
      string obj1;
      string obj2;
      getTwoObjs(tempInputCollection, obj1, obj2);
      string obj2ToGet = getObjToGet(obj2);
      objectsToFlag.push_back(tempInputCollection);
      objectsToFlag.push_back(obj1);
      objectsToFlag.push_back(obj2);
      objectsToPlot.push_back(tempInputCollection);
      objectsToPlot.push_back(obj1);
      objectsToPlot.push_back(obj2);
      objectsToGet.push_back(tempInputCollection);
      objectsToGet.push_back(obj1);
      objectsToGet.push_back(obj2ToGet);
    } // end else
    if (find(objectsToPlot.begin(), objectsToPlot.end(), "events") != objectsToPlot.end())
      objectsToGet.push_back("jets");


    vector<edm::ParameterSet> histogramList_  (histogramSets_.at(currentHistogramSet).getParameter<vector<edm::ParameterSet> >("histograms"));

    for(uint currentHistogram = 0; currentHistogram != histogramList_.size(); currentHistogram++){

      vector<double> defaultValue;
      defaultValue.push_back (-1.0);

      histogram tempHistogram;
      tempHistogram.inputCollection = tempInputCollection;
      tempHistogram.name = histogramList_.at(currentHistogram).getParameter<string>("name");
      tempHistogram.title = histogramList_.at(currentHistogram).getParameter<string>("title");
      tempHistogram.bins = histogramList_.at(currentHistogram).getUntrackedParameter<vector<double> >("bins", defaultValue);
      tempHistogram.variableBinsX = histogramList_.at(currentHistogram).getUntrackedParameter<vector<double> >("variableBinsX", defaultValue);
      tempHistogram.variableBinsY = histogramList_.at(currentHistogram).getUntrackedParameter<vector<double> >("variableBinsY", defaultValue);
      tempHistogram.inputVariables = histogramList_.at(currentHistogram).getParameter<vector<string> >("inputVariables");

      bool histExists = false;
      for (uint i=0; i<histograms.size(); i++) {
        if (histograms.at(i).name == tempHistogram.name) { histExists = true; break; }
      }
      if (histExists) {
        clog << "Warning:  histogram " << tempHistogram.name << " already exists; will not book an additional instance." << endl;
      } else {
        histograms.push_back(tempHistogram);
      }

    }
  } //   for(uint currentHistogramSet = 0; currentHistogramSet != histogramSets_.size(); currentHistogramSet++)

  //make unique vector of objects we need to plot (so we can book a histogram with the number of each object)
  sort( objectsToPlot.begin(), objectsToPlot.end() );
  objectsToPlot.erase( unique( objectsToPlot.begin(), objectsToPlot.end() ), objectsToPlot.end() );



  //add histograms with the gen-matched id, mother id, and grandmother id
  histogram tempStatus3Histo, tempStatus3Histo2;
  tempStatus3Histo.inputCollection = "mcparticles";
  tempStatus3Histo.name = "status3OutgoingID";
  tempStatus3Histo.title = "Status 3 Outgoing Particle IDs;Outgoing Particle ID";

  int maxNum = 25;
  vector<double> binVector;
  binVector.push_back(maxNum);
  binVector.push_back(0);
  binVector.push_back(maxNum);

  tempStatus3Histo.bins = binVector;
  tempStatus3Histo.inputVariables.push_back("status3OutgoingID");
  histograms.push_back(tempStatus3Histo);

  for(uint currentObjectIndex = 0; currentObjectIndex != objectsToPlot.size(); currentObjectIndex++){

    string currentObject = objectsToPlot.at(currentObjectIndex);
    if(currentObject != "muons" &&
       currentObject != "secondary muons" &&
       currentObject != "secondary electrons" &&
       currentObject != "electrons" &&
       currentObject != "taus" &&
       currentObject != "tracks" &&
       currentObject != "photons" &&
       currentObject != "secondary photons" &&
       currentObject != "superclusters" &&
       currentObject != "mcparticles" &&
       currentObject != "secondary mcparticles")
      continue;

    histogram tempIdHisto;
    histogram tempMomIdHisto;
    histogram tempGmaIdHisto;
    histogram tempIdVsMomIdHisto;
    histogram tempIdVsGmaIdHisto;

    tempIdHisto.inputCollection = currentObject;
    tempMomIdHisto.inputCollection = currentObject;
    tempGmaIdHisto.inputCollection = currentObject;
    tempIdVsMomIdHisto.inputCollection = currentObject;
    tempIdVsGmaIdHisto.inputCollection = currentObject;

    if(currentObject == "secondary muons") currentObject = "secondaryMuons";
    if(currentObject == "secondary photons") currentObject = "secondaryPhotons";
    if(currentObject == "secondary electrons") currentObject = "secondaryElectrons";
    if(currentObject == "secondary mcparticles") currentObject = "secondaryMCParticles";

    currentObject = currentObject.substr(0, currentObject.size()-1);
    tempIdHisto.name = currentObject+"GenMatchId";
    tempMomIdHisto.name = currentObject+"GenMatchMotherId";
    tempGmaIdHisto.name = currentObject+"GenMatchGrandmotherId";
    tempIdVsMomIdHisto.name = currentObject+"GenMatchIdVsMotherId";
    tempIdVsGmaIdHisto.name = currentObject+"GenMatchIdVsGrandmotherId";

    currentObject.at(0) = toupper(currentObject.at(0));
    tempIdHisto.title = currentObject+" Gen-matched Particle";
    tempMomIdHisto.title = currentObject+" Gen-matched Particle's Mother";
    tempGmaIdHisto.title = currentObject+" Gen-matched Particle's Grandmother";
    tempIdVsMomIdHisto.title = currentObject+" Gen-matched Particle's Mother vs. Particle;Particle;Mother";
    tempIdVsGmaIdHisto.title = currentObject+" Gen-matched Particle's Grandmother vs. Particle;Particle;Grandmother";


    int maxNum = 25;
    vector<double> binVector;
    binVector.push_back(maxNum);
    binVector.push_back(0);
    binVector.push_back(maxNum);

    tempIdHisto.bins = binVector;
    tempIdHisto.inputVariables.push_back("genMatchedId");
    tempMomIdHisto.bins = binVector;
    tempMomIdHisto.inputVariables.push_back("genMatchedMotherId");
    tempGmaIdHisto.bins = binVector;
    tempGmaIdHisto.inputVariables.push_back("genMatchedGrandmotherId");
    binVector.push_back(maxNum);
    binVector.push_back(0);
    binVector.push_back(maxNum);
    tempIdVsMomIdHisto.bins = binVector;
    tempIdVsMomIdHisto.inputVariables.push_back("genMatchedId");
    tempIdVsMomIdHisto.inputVariables.push_back("genMatchedMotherIdReverse");
    tempIdVsGmaIdHisto.bins = binVector;
    tempIdVsGmaIdHisto.inputVariables.push_back("genMatchedId");
    tempIdVsGmaIdHisto.inputVariables.push_back("genMatchedGrandmotherIdReverse");

    histograms.push_back(tempIdHisto);
    histograms.push_back(tempMomIdHisto);
    histograms.push_back(tempGmaIdHisto);
    histograms.push_back(tempIdVsMomIdHisto);
    histograms.push_back(tempIdVsGmaIdHisto);
  }


  channel tempChannel;
  //loop over all channels (event selections)
  for(uint currentChannel = 0; currentChannel != channels_.size(); currentChannel++){

    //get name of channel
    string channelName  (channels_.at(currentChannel).getParameter<string>("name"));
    tempChannel.name = channelName;
    TString channelLabel = channelName;
    if (verbose_) clog << "Processing channel:  " << channelName << endl;

    tempChannel.inStatus3Outgoing.clear ();
    tempChannel.notInStatus3Outgoing.clear ();
    tempChannel.absInStatus3Outgoing.clear ();
    tempChannel.absNotInStatus3Outgoing.clear ();
    if(channels_.at(currentChannel).exists("inStatus3Outgoing")){
      tempChannel.inStatus3Outgoing = channels_.at(currentChannel).getParameter<vector<int> >("inStatus3Outgoing");
      tempChannel.inStatus3OutgoingCutName = "status 3 outgoing inclusion";
    }
    if(channels_.at(currentChannel).exists("notInStatus3Outgoing")){
      tempChannel.notInStatus3Outgoing = channels_.at(currentChannel).getParameter<vector<int> >("notInStatus3Outgoing");
      tempChannel.notInStatus3OutgoingCutName = "status 3 outgoing exclusion";
    }
    if(channels_.at(currentChannel).exists("absInStatus3Outgoing")){
      tempChannel.absInStatus3Outgoing = channels_.at(currentChannel).getParameter<vector<int> >("absInStatus3Outgoing");
      tempChannel.inStatus3OutgoingCutName = "status 3 outgoing inclusion";
    }
    if(channels_.at(currentChannel).exists("absNotInStatus3Outgoing")){
      tempChannel.absNotInStatus3Outgoing = channels_.at(currentChannel).getParameter<vector<int> >("absNotInStatus3Outgoing");
      tempChannel.notInStatus3OutgoingCutName = "status 3 outgoing exclusion";
    }

    //set triggers for this channel
    tempChannel.triggers.clear();
    tempChannel.triggersToVeto.clear();
    if(channels_.at(currentChannel).exists("triggers")){
      tempChannel.triggers = channels_.at(currentChannel).getParameter<vector<string> >("triggers");
      objectsToGet.push_back("triggers");
    }
    if(channels_.at(currentChannel).exists("triggersToVeto")){
      tempChannel.triggersToVeto = channels_.at(currentChannel).getParameter<vector<string> >("triggersToVeto");
      objectsToGet.push_back("triggers");
    }



    //create cutFlow for this channel
    cutFlows_.push_back (new CutFlow (fs_, channelName));
    vector<TFileDirectory> directories; //vector of directories in the output file.
    vector<TFileDirectory> treeDirectories; //vector of directories for trees in the output file.
    vector<string> subSubDirNames;//subdirectories in each channel.
    //get list of cuts for this channel
    vector<edm::ParameterSet> cuts_  (channels_.at(currentChannel).getParameter<vector<edm::ParameterSet> >("cuts"));


    //loop over and parse all cuts
    for(uint currentCut = 0; currentCut != cuts_.size(); currentCut++){
      cut tempCut;
      //store input collection for cut
      string tempInputCollection = cuts_.at(currentCut).getParameter<string> ("inputCollection");
      if(tempInputCollection == "muon-electron pairs") tempInputCollection = "electron-muon pairs";
      if(tempInputCollection == "photon-electron pairs") tempInputCollection = "electron-photon pairs";
      if(tempInputCollection == "photon-muon pairs") tempInputCollection = "muon-photon pairs";
      if(tempInputCollection == "jet-electron pairs") tempInputCollection = "electron-jet pairs";
      if(tempInputCollection == "jet-muon pairs") tempInputCollection = "muon-jet pairs";
      if(tempInputCollection == "event-muon pairs") tempInputCollection = "muon-event pairs";
      if(tempInputCollection == "event-electron pairs") tempInputCollection = "electron-event pairs";
      if(tempInputCollection == "jet-met pairs")  tempInputCollection = "met-jet pairs";
      if(tempInputCollection == "mcparticle-met pairs")  tempInputCollection = "met-mcparticle pairs";
      if(tempInputCollection == "secondary mcparticle-jet pairs")  tempInputCollection = "jet-secondary mcparticle pairs";
      if(tempInputCollection == "mcparticle-jet pairs")  tempInputCollection = "jet-mcparticle pairs";
      if(tempInputCollection == "secondary mcparticle-mcparticle pairs")  tempInputCollection = "mcparticle-secondary mcparticle pairs";
      if(tempInputCollection == "track-jet pairs")  tempInputCollection = "track-jet pairs";
      if(tempInputCollection == "jet-photon pairs") tempInputCollection = "photon-jet pairs";
      if(tempInputCollection == "event-track pairs")   tempInputCollection = "track-event pairs";
      if(tempInputCollection == "secondary muon-muon pairs")   tempInputCollection = "muon-secondary muon pairs";
      if(tempInputCollection == "secondary jet-jet pairs")   tempInputCollection = "jet-secondary jet pairs";
      if(tempInputCollection == "secondary jet-muon pairs")   tempInputCollection = "muon-secondary jet pairs";
      if(tempInputCollection == "secondary photon-muon pairs")   tempInputCollection = "muon-secondary photon pairs";
      if(tempInputCollection == "secondary jet-photon pairs")   tempInputCollection = "photon-secondary jet pairs";
      if(tempInputCollection == "secondary jet-electron pairs")   tempInputCollection = "electron-secondary jet pairs";
      if(tempInputCollection == "jet-secondary muon pairs")   tempInputCollection = "secondary muon-jet pairs";
      if(tempInputCollection == "jet-secondary electron pairs")   tempInputCollection = "secondary electron-jet pairs";
      if(tempInputCollection == "secondary electron-electron pairs")   tempInputCollection = "electron-secondary electron pairs";
      if(tempInputCollection == "trigobj-electron pairs")   tempInputCollection = "electron-trigobj pairs";
      if(tempInputCollection == "trigobj-muon pairs")   tempInputCollection = "muon-trigobj pairs";
      if(tempInputCollection == "mcparticle-electron pairs")   tempInputCollection = "electron-mcparticle pairs";
      if(tempInputCollection == "mcparticle-muon pairs")   tempInputCollection = "muon-mcparticle pairs";
      if(tempInputCollection == "secondary mcparticle-muon pairs")   tempInputCollection = "muon-secondary mcparticle pairs";
      if(tempInputCollection == "mcparticle-track pairs")   tempInputCollection = "track-mcparticle pairs";
      if(tempInputCollection == "mcparticle-stop pairs")    tempInputCollection = "stop-mcparticle pairs";    
      tempCut.inputCollection = tempInputCollection;
      if(tempInputCollection.find("pairs")==string::npos){ //just a single object
        if(tempInputCollection.find("secondary")!=string::npos){//secondary object
          if(tempInputCollection.find("secondary muons")!=string::npos){//treat secondary muons differently; allow for a different input collection
            objectsToGet.push_back("secondary muons");
          } else {
            int spaceIndex = tempInputCollection.find(" ");
            int secondWordLength = tempInputCollection.size() - spaceIndex;
            objectsToGet.push_back(tempInputCollection.substr(spaceIndex+1,secondWordLength));
          }
        }
        else{
          objectsToGet.push_back(tempInputCollection);
        }
        objectsToCut.push_back(tempInputCollection);
        objectsToFlag.push_back(tempInputCollection);
      }
      else{//pair of objects, need to add them both to objectsToGet
        string obj1;
        string obj2;
        getTwoObjs(tempInputCollection, obj1, obj2);
        string obj2ToGet = getObjToGet(obj2);
        objectsToCut.push_back(tempInputCollection);
        objectsToCut.push_back(obj1);
        objectsToCut.push_back(obj2);
        objectsToFlag.push_back(tempInputCollection);
        objectsToFlag.push_back(obj1);
        objectsToFlag.push_back(obj2);
        objectsToGet.push_back(tempInputCollection);
        objectsToGet.push_back(obj1);
        objectsToGet.push_back(obj2ToGet);

      }



      //split cut string into parts and store them
      string cutString = cuts_.at(currentCut).getParameter<string> ("cutString");
      vector<string> cutStringVector = splitString(cutString);
      if(cutStringVector.size()!=3 && cutStringVector.size() % 4 !=3){
        clog << "Error: Didn't find the expected number elements in the following cut string: '" << cutString << "'\n";
        exit(0);
      }
      tempCut.numSubcuts = (cutStringVector.size()+1)/4;
      for(int subcutIndex = 0; subcutIndex != tempCut.numSubcuts; subcutIndex++){//loop over all the pieces of the cut combined using &,|
        int indexOffset = 4 * subcutIndex;
        string currentVariableString = cutStringVector.at(indexOffset);
        if(currentVariableString.find("(")==string::npos){
          tempCut.functions.push_back("");//no function was specified
          tempCut.variables.push_back(currentVariableString);// variable to cut on
        }
        else{
          tempCut.functions.push_back(currentVariableString.substr(0,currentVariableString.find("(")));//function comes before the "("
          string tempVariable = currentVariableString.substr(currentVariableString.find("(")+1);//get rest of string
          tempCut.variables.push_back(tempVariable.substr(0,tempVariable.size()-1));//remove trailing ")"
        }
        tempCut.comparativeOperators.push_back(cutStringVector.at(indexOffset+1));// comparison to make
        tempCut.cutValues.push_back(atof(cutStringVector.at(indexOffset+2).c_str()));// threshold value to pass cut
        tempCut.cutStringValues.push_back(cutStringVector.at(indexOffset+2));// string value to pass cut
        if(subcutIndex != 0) tempCut.logicalOperators.push_back(cutStringVector.at(indexOffset-1)); // logical comparison (and, or)
      }

      //get number of objects required to pass cut for event to pass
      string numberRequiredString = cuts_.at(currentCut).getParameter<string> ("numberRequired");
      vector<string> numberRequiredVector = splitString(numberRequiredString);
      if(numberRequiredVector.size()!=2){
        clog << "Error: Didn't find two elements in the following number requirement string: '" << numberRequiredString << "'\n";
        exit(0);
      }

      int numberRequiredInt = atoi(numberRequiredVector.at(1).c_str());
      tempCut.numberRequired = numberRequiredInt;// number of objects required to pass the cut
      tempCut.eventComparativeOperator = numberRequiredVector.at(0);// comparison to make

      //Set up vectors to store the directories and subDirectories for each channel.
      string subSubDirName;
      string tempCutName;
      if(cuts_.at(currentCut).exists("alias")){
        tempCutName = cuts_.at(currentCut).getParameter<string> ("alias");
        subSubDirName = "After " + tempCutName + " Cut Applied";
      }
      else{
        //construct string for cutflow table
        bool plural = numberRequiredInt != 1;
        string collectionString = plural ? tempInputCollection : tempInputCollection.substr(0, tempInputCollection.size()-1);
        string cutName =  numberRequiredString + " " + collectionString + " with " + cutString;
        tempCutName = cutName;
        subSubDirName = "After " + numberRequiredString + " " + collectionString + " with " + cutString + " Cut Applied";
      }

      tempCut.isVeto = false;
      if(cuts_.at(currentCut).exists("isVeto"))
        tempCut.isVeto = cuts_.at(currentCut).getParameter<bool> ("isVeto");
      subSubDirNames.push_back(subSubDirName);
      tempCut.name = tempCutName;
      if (verbose_) clog << "Setting up cut, index: " << currentCut << ", input collection: " << tempInputCollection<< ", name: " << tempCut.name << endl;

      tempChannel.cuts.push_back(tempCut);


    }//end loop over cuts

    vector<map<string, TH1D*>> oneDHistsTmp;
    vector<map<string, TH2D*>> twoDHistsTmp;
    //book a directory in the output file with the name of the channel
    TFileDirectory subDir = fs_->mkdir( channelName );
    //loop over the cuts to set up subdirectory for each cut if GetPlotsAfterEachCut_ is true.
    if(GetPlotsAfterEachCut_){
      for( uint currentDir=0; currentDir != subSubDirNames.size(); currentDir++){
        TFileDirectory subSubDir = subDir.mkdir( subSubDirNames[currentDir] );
        directories.push_back(subSubDir);
        map<string, TH1D*> oneDhistoMap;
        oneDHistsTmp.push_back(oneDhistoMap);
        map<string, TH2D*> twoDhistoMap;
        twoDHistsTmp.push_back(twoDhistoMap);
      }
      treeDirectories.push_back(subDir);
      oneDHists_.push_back(oneDHistsTmp);
      twoDHists_.push_back(twoDHistsTmp);
    }
    //only set up directories with names of the channels if GetPlotsAfterEachCut_ is false.
    else{
      map<string, TH1D*> oneDhistoMap;
      oneDHistsTmp.push_back(oneDhistoMap);
      map<string, TH2D*> twoDhistoMap;
      twoDHistsTmp.push_back(twoDhistoMap);
      oneDHists_.push_back(oneDHistsTmp);
      twoDHists_.push_back(twoDHistsTmp);
      directories.push_back(subDir);
      treeDirectories.push_back(subDir);

    }

    for(uint currentDir = 0; !useEDMFormat_ && currentDir != treeDirectories.size(); currentDir++){
      TTree* newTree = treeDirectories.at(currentDir).make<TTree> (TString("BNTree_"+channelLabel), TString("BNTree_"+channelLabel));
      BNTrees_.push_back(newTree);
      BNTrees_.back()->Branch("event_runInt",  &BNTreeBranchVals_runInt_,  "event_runInt/I");
      BNTrees_.back()->Branch("event_lumiInt", &BNTreeBranchVals_lumiInt_, "event_lumiInt/I");
      BNTrees_.back()->Branch("event_evtLong", &BNTreeBranchVals_evtLong_, "event_evtLong/L");
      for (uint iBranch = 0; iBranch < treeBranches_.size(); iBranch++){
        BranchSpecs currentVar = treeBranches_.at(iBranch);
        vector<float> newVec;
        BNTreeBranchVals_[currentVar.name] = newVec;
        BNTrees_.back()->Branch(TString(currentVar.name), &BNTreeBranchVals_.at(currentVar.name));
      } // end for (uint iBranch = 0; iBranch < treeBranches_.size(); iBranch++)
    }

    //book all histograms included in the configuration
    for(uint currentDir = 0; currentDir != directories.size(); currentDir++){//loop over all the directories.

      for(uint currentHistogramIndex = 0; currentHistogramIndex != histograms.size(); currentHistogramIndex++){

        histogram currentHistogram = histograms.at(currentHistogramIndex);
        int numBinsElements = currentHistogram.bins.size();
        int numInputVariables = currentHistogram.inputVariables.size();
        int numBinEdgesX = currentHistogram.variableBinsX.size();
        int numBinEdgesY = currentHistogram.variableBinsY.size();

        if(numBinsElements == 1){
          if(numBinEdgesX > 1){
            if(numBinEdgesY > 1)
              numBinsElements = 6;
            else
              numBinsElements = 3;
          }
        }
        if(numBinsElements != 3 && numBinsElements !=6){
          clog << "Error: Didn't find correct number of bin specifications for histogram named '" << currentHistogram.name << "'\n";
          exit(0);
        }
        else if((numBinsElements == 3 && numInputVariables !=1) || (numBinsElements == 6 && numInputVariables!=2)){
          clog << "Error: Didn't find correct number of input variables for histogram named '" << currentHistogram.name << "'\n";
          exit(0);
        }
        else if(numBinsElements == 3){
          if (currentHistogram.bins.size () == 3)
            oneDHists_.at(currentChannel).at(currentDir)[currentHistogram.name] = directories.at(currentDir).make<TH1D> (TString(currentHistogram.name),channelLabel+" channel: "+currentHistogram.title, currentHistogram.bins.at(0), currentHistogram.bins.at(1), currentHistogram.bins.at(2));
          else
            {
              oneDHists_.at(currentChannel).at(currentDir)[currentHistogram.name] = directories.at(currentDir).make<TH1D> (TString(currentHistogram.name),channelLabel+" channel: "+currentHistogram.title, numBinEdgesX - 1, currentHistogram.variableBinsX.data ());
            }
        }
        else if(numBinsElements == 6){
          if (currentHistogram.bins.size () == 6)
            twoDHists_.at(currentChannel).at(currentDir)[currentHistogram.name] = directories.at(currentDir).make<TH2D> (TString(currentHistogram.name),channelLabel+" channel: "+currentHistogram.title, currentHistogram.bins.at(0), currentHistogram.bins.at(1), currentHistogram.bins.at(2),currentHistogram.bins.at(3),currentHistogram.bins.at(4),currentHistogram.bins.at(5));
          else
            twoDHists_.at(currentChannel).at(currentDir)[currentHistogram.name] = directories.at(currentDir).make<TH2D> (TString(currentHistogram.name),channelLabel+" channel: "+currentHistogram.title, numBinEdgesX - 1, currentHistogram.variableBinsX.data (), numBinEdgesY - 1, currentHistogram.variableBinsY.data ());
        }


        if(currentHistogram.name.find("GenMatch")==string::npos && currentHistogram.name.find("status3")==string::npos) continue;

        // bin      particle type
        // ---      -------------
        //  0        unmatched
        //  1        u
        //  2        d
        //  3        s
        //  4        c
        //  5        b
        //  6        t
        //  7        e
        //  8        mu
        //  9        tau
        // 10        nu
        // 11        g
        // 12        gamma
        // 13        Z
        // 14        W
        // 15        light meson
        // 16        K meson
        // 17        D meson
        // 18        B meson
        // 19        light baryon
        // 20        strange baryon
        // 21        charm baryon
        // 22        bottom baryon
        // 23        QCD string
        // 24        other

        vector<TString> labelArray;
        labelArray.push_back("unmatched");
        labelArray.push_back("u");
        labelArray.push_back("d");
        labelArray.push_back("s");
        labelArray.push_back("c");
        labelArray.push_back("b");
        labelArray.push_back("t");
        labelArray.push_back("e");
        labelArray.push_back("#mu");
        labelArray.push_back("#tau");
        labelArray.push_back("#nu");
        labelArray.push_back("g");
        labelArray.push_back("#gamma");
        labelArray.push_back("Z");
        labelArray.push_back("W");
        labelArray.push_back("light meson");
        labelArray.push_back("K meson");
        labelArray.push_back("D meson");
        labelArray.push_back("B meson");
        labelArray.push_back("light baryon");
        labelArray.push_back("strange baryon");
        labelArray.push_back("charm baryon");
        labelArray.push_back("bottom baryon");
        labelArray.push_back("QCD string");
        labelArray.push_back("other");

        bool is2D = currentHistogram.name.find("GenMatchIdVsMotherId")!=string::npos
                 || currentHistogram.name.find("GenMatchIdVsGrandmotherId")!=string::npos;
        for(int bin = 0; bin !=currentHistogram.bins.at(0); bin++){
          if(!is2D)
            oneDHists_.at(currentChannel).at(currentDir)[currentHistogram.name]->GetXaxis()->SetBinLabel(bin+1,labelArray.at(bin));
          else {
            twoDHists_.at(currentChannel).at(currentDir)[currentHistogram.name]->GetYaxis()->SetBinLabel(bin+1,labelArray.at(currentHistogram.bins.at(0)-bin-1));
            twoDHists_.at(currentChannel).at(currentDir)[currentHistogram.name]->GetXaxis()->SetBinLabel(bin+1,labelArray.at(bin));
          }
        }
        if(is2D) {
          twoDHists_.at(currentChannel).at(currentDir)[currentHistogram.name]->GetXaxis()->CenterTitle();
          twoDHists_.at(currentChannel).at(currentDir)[currentHistogram.name]->GetYaxis()->CenterTitle();
        }

      }  // end      for(uint currentHistogramIndex = 0; currentHistogramIndex != histograms.size(); currentHistogramIndex++)


      // Book a histogram for the number of each object type to be plotted.
      // Name of objectToPlot here must match the name specified in OSUAnalysis::analyze().
      for (uint currentObjectIndex = 0; currentObjectIndex != objectsToPlot.size(); currentObjectIndex++){
        string currentObject = objectsToPlot.at(currentObjectIndex);
        int maxNum = 10;
        if(currentObject == "mcparticles" || currentObject == "secondary mcparticles") maxNum = 50;
        else if(currentObject == "primaryvertexs") maxNum = 50;
        else if(currentObject == "jets") maxNum = 25;

        if(currentObject == "muon-muon pairs")                currentObject = "dimuonPairs";
        else if(currentObject == "electron-electron pairs")   currentObject = "dielectronPairs";
        else if(currentObject == "electron-muon pairs")       currentObject = "electronMuonPairs";
        else if(currentObject == "electron-photon pairs")     currentObject = "electronPhotonPairs";
        else if(currentObject == "muon-photon pairs")         currentObject = "muonPhotonPairs";
        else if(currentObject == "secondary jets")            currentObject = "secondaryJets";
        else if(currentObject == "secondary photons")            currentObject = "secondaryPhotons";
        else if(currentObject == "jet-jet pairs")             currentObject = "dijetPairs";
        else if(currentObject == "jet-secondary jet pairs")   currentObject = "jetSecondaryJetPairs";
        else if(currentObject == "electron-jet pairs")        currentObject = "electronJetPairs";
        else if(currentObject == "muon-jet pairs")            currentObject = "muonJetPairs";
        else if(currentObject == "muon-event pairs")            currentObject = "muonEventPairs";
        else if(currentObject == "electron-event pairs")            currentObject = "electronEventPairs";
        else if(currentObject == "photon-jet pairs")            currentObject = "photonJetPairs";
        else if(currentObject == "met-jet pairs")             currentObject = "metJetPairs";
        else if(currentObject == "met-mcparticle pairs")      currentObject = "metMcParticlePairs";
        else if(currentObject == "jet-mcparticle pairs")      currentObject = "jetMcParticlePairs";
        else if(currentObject == "jet-secondary mcparticle pairs")      currentObject = "jetSecondaryMcParticlePairs";
        else if(currentObject == "mcparticle-secondary mcparticle pairs")      currentObject = "mcParticleSecondaryMcParticlePairs";
        else if(currentObject == "track-jet pairs")           currentObject = "trackJetPairs";
        else if(currentObject == "muon-secondary jet pairs")  currentObject = "muonSecondaryJetPairs";
        else if(currentObject == "muon-secondary photon pairs")  currentObject = "muonSecondaryPhotonPairs";
        else if(currentObject == "photon-secondary jet pairs")  currentObject = "photonSecondaryJetPairs";
        else if(currentObject == "electron-secondary jet pairs")  currentObject = "electronSecondaryJetPairs";
        else if(currentObject == "secondary electron-jet pairs")  currentObject = "secondaryElectronJetPairs";
        else if(currentObject == "secondary muon-jet pairs")  currentObject = "secondaryMuonJetPairs";
        else if(currentObject == "track-event pairs")         currentObject = "trackEventPairs";
        else if(currentObject == "electron-track pairs")      currentObject = "electronTrackPairs";
        else if(currentObject == "muon-track pairs")          currentObject = "muonTrackPairs";
        else if(currentObject == "jet-track pairs")          currentObject = "jetTrackPairs";
        else if(currentObject == "secondary muon-track pairs") currentObject = "secondaryMuonTrackPairs";
        else if(currentObject == "muon-tau pairs")            currentObject = "muonTauPairs";
        else if(currentObject == "tau-tau pairs")             currentObject = "ditauPairs";
        else if(currentObject == "tau-track pairs")           currentObject = "tauTrackPairs";
        else if(currentObject == "muon-secondary muon pairs") currentObject = "muonSecondaryMuonPairs";
        else if(currentObject == "secondary muons")           currentObject = "secondaryMuons";
        else if(currentObject == "electron-secondary electron pairs") currentObject = "electronSecondaryElectronPairs";
        else if(currentObject == "secondary electrons")       currentObject = "secondaryElectrons";
        else if(currentObject == "electron-trigobj pairs")    currentObject = "electronTrigobjPairs";
        else if(currentObject == "muon-trigobj pairs")        currentObject = "muonTrigobjPairs";
        else if(currentObject == "electron-mcparticle pairs") currentObject = "electronMCparticlePairs";
        else if(currentObject == "muon-secondary mcparticle pairs") currentObject = "muonSecondayMCparticlePairs";
        else if(currentObject == "muon-mcparticle pairs") currentObject = "muonMCparticlePairs";
        else if(currentObject == "track-mcparticle pairs")    currentObject = "trackMCparticlePairs";
	else if(currentObject == "stop-mcparticle pairs")     currentObject = "stopMCparticlePairs";     

        currentObject.at(0) = toupper(currentObject.at(0));
        string histoName = "num" + currentObject;

        if(histoName == "numPrimaryvertexs"){
          string newHistoName = histoName + "BeforePileupCorrection";
          oneDHists_.at(currentChannel).at(currentDir)[newHistoName] = directories.at(currentDir).make<TH1D> (TString(newHistoName),channelLabel+" channel: Number of Selected "+currentObject+" Before Pileup Correction; # "+currentObject, maxNum, 0, maxNum);
          newHistoName = histoName + "AfterPileupCorrection";
          oneDHists_.at(currentChannel).at(currentDir)[newHistoName] = directories.at(currentDir).make<TH1D> (TString(newHistoName),channelLabel+" channel: Number of Selected "+currentObject+ " After Pileup Correction; # "+currentObject, maxNum, 0, maxNum);
        }
        else
          oneDHists_.at(currentChannel).at(currentDir)[histoName] = directories.at(currentDir).make<TH1D> (TString(histoName),channelLabel+" channel: Number of Selected "+currentObject+"; # "+currentObject, maxNum, 0, maxNum);
      }

      if (verbose_) {
        clog << "List of 1D histograms booked for channel " << channelName << " and directory " << currentDir << ":" << endl;
        for(map<string, TH1D*>::iterator
              iter = oneDHists_.at(currentChannel).at(currentDir).begin();
            iter  != oneDHists_.at(currentChannel).at(currentDir).end();
            iter++) {
          clog << " " << iter->first
               << ":  name=" << iter->second->GetName()
               << ", title=" << iter->second->GetTitle()
               << ", bins=(" << iter->second->GetNbinsX()
               << "," << iter->second->GetXaxis()->GetXmin()
               << "," << iter->second->GetXaxis()->GetXmax()
               << ")" << endl;
        }
        clog << "List of 2D histograms booked for channel " << channelName << " and directory " << currentDir << ":" << endl;
        for(map<string, TH2D*>::iterator
              iter = twoDHists_.at(currentChannel).at(currentDir).begin();
            iter  != twoDHists_.at(currentChannel).at(currentDir).end();
            iter++) {
          clog << " " << iter->first
               << ":  name=" << iter->second->GetName()
               << ", title=" << iter->second->GetTitle()
               << ", binsX=(" << iter->second->GetNbinsX()
               << "," << iter->second->GetXaxis()->GetXmin()
               << "," << iter->second->GetXaxis()->GetXmax()
               << ")"
               << ", binsY=(" << iter->second->GetNbinsY()
               << "," << iter->second->GetYaxis()->GetXmin()
               << "," << iter->second->GetYaxis()->GetXmax()
               << ")"
               << endl;
        }
      }

    }//end of loop over directories
    channels.push_back(tempChannel);
    tempChannel.cuts.clear();
  }//end loop over channels


  // Create the cutflow histogram and fill with 0 weight, in case no events are found in the input file.
  for(uint currentChannelIndex = 0; currentChannelIndex != channels.size(); currentChannelIndex++){
    channel currentChannel = channels.at(currentChannelIndex);
    if(currentChannel.inStatus3Outgoing.size () || currentChannel.absInStatus3Outgoing.size ()){
      cutFlows_.at(currentChannelIndex)->at(currentChannel.inStatus3OutgoingCutName) = true;
    }
    if(currentChannel.notInStatus3Outgoing.size () || currentChannel.absNotInStatus3Outgoing.size ()){
      cutFlows_.at(currentChannelIndex)->at(currentChannel.notInStatus3OutgoingCutName) = true;
    }
    if(currentChannel.triggers.size() != 0 || currentChannel.triggersToVeto.size() != 0){  //triggers specified
      cutFlows_.at(currentChannelIndex)->at("trigger") = true;
    }
    for(uint currentCutIndex = 0; currentCutIndex != currentChannel.cuts.size(); currentCutIndex++){
      cut currentCut = currentChannel.cuts.at(currentCutIndex);
      cutFlows_.at(currentChannelIndex)->at (currentCut.name) = true;
    }
    cutFlows_.at(currentChannelIndex)->fillCutFlow(0);  // fill cutflow with 0 weight, just to create the cutflow histograms
    masterCutFlow_->at(currentChannel.name) = true; 
  }
  masterCutFlow_->fillCutFlow(0); 

  //make unique vector of objects we need to get from the event
  sort( objectsToGet.begin(), objectsToGet.end() );
  objectsToGet.erase( unique( objectsToGet.begin(), objectsToGet.end() ), objectsToGet.end() );
  //make unique vector of objects we need to cut on
  sort( objectsToCut.begin(), objectsToCut.end() );
  objectsToCut.erase( unique( objectsToCut.begin(), objectsToCut.end() ), objectsToCut.end() );
  //make unique vector of objects we need to set flags for
  sort( objectsToFlag.begin(), objectsToFlag.end() );
  objectsToFlag.erase( unique( objectsToFlag.begin(), objectsToFlag.end() ), objectsToFlag.end() );

  // Move all paired objects to the end of the list, so that the flags for single objects are always set before those of paired objects.
  for (uint i=0; i<objectsToFlag.size(); i++) {
    if (objectsToFlag.at(i).find("pairs")!=string::npos) { // if it contains "pairs"
      objectsToFlag.push_back(objectsToFlag.at(i));
      objectsToFlag.erase(objectsToFlag.begin()+i);
    }
  }

  if (verbose_) {
    clog << "List of channels:" << endl;
    for (uint i=0; i<channels.size(); i++) {
      clog << " " << channels.at(i).name << endl;
    }
    clog << "List of objects to get:" << endl;
    for (uint i=0; i<objectsToGet.size(); i++) {
      clog << " " << objectsToGet.at(i) << endl;
    }
    clog << "List of objects to plot:" << endl;
    for (uint i=0; i<objectsToPlot.size(); i++) {
      clog << " " << objectsToPlot.at(i) << endl;
    }
    clog << "List of objects to cut:" << endl;
    for (uint i=0; i<objectsToCut.size(); i++) {
      clog << " " << objectsToCut.at(i) << endl;
    }
    clog << "List of objects to flag:" << endl;
    for (uint i=0; i<objectsToFlag.size(); i++) {
      clog << " " << objectsToFlag.at(i) << endl;
    }
  }

  produces<map<string, bool> > ("channelDecisions");

  if (printEventInfo_) {
    findEventsLog = new ofstream();
    findEventsLog->open("findEvents.txt");
    clog << "Listing run:lumi:event in file findEvents.txt for events that pass full selection (of any channel)." << endl;
  } else {
    findEventsLog = 0;
  }

  isFirstEvent_ = true;

  if (verbose_) clog << "Finished OSUAnalysis::OSUAnalysis constructor." << endl;


  } // end constructor OSUAnalysis::OSUAnalysis()


OSUAnalysis::~OSUAnalysis ()
{

  if (verbose_) clog << "Beginning OSUAnalysis::OSUAnalysis destructor." << endl;

  // Destroying the CutFlow objects causes the cut flow numbers and time
  // information to be printed to standard output.
  for(uint currentChannel = 0; currentChannel != channels_.size(); currentChannel++){
    delete cutFlows_.at(currentChannel);
  }

  if (printEventInfo_ && findEventsLog) findEventsLog->close();


  if (calcPdfWeights_) {
    ///PRINT HERE PDF RESULTS 
    //Put "all" and "passed" numbers in pairs for easy manipulation 
    cout << "XXX "<<allNo<<" "<<passedNo<<" "<<allSF<<" "<<passedSF<<" "<<allSF2<<" "<<passedSF2<<" "<<allCentralW2<<" "<<passedCentralW2;
    // allSums and passedSums size should be the same so use either 
    //    cout << endl << "Debug: passedSums.size() = " << passedSums.size() << endl;  
    for(uint i=0;i<passedSums.size();i++){
      cout <<" "<<allSums[i]<<" "<<passedSums[i];
    }
    cout << endl;
  }

  clog << "=============================================" << endl;
  clog << "Successfully completed OSUAnalysis." << endl;
  clog << "=============================================" << endl;

  if (verbose_) clog << "Finished OSUAnalysis::OSUAnalysis destructor." << endl;

}

void
OSUAnalysis::produce (edm::Event &event, const edm::EventSetup &setup)
{
  // Retrieve necessary collections from the event.
  if (verbose_) clog << "Beginning OSUAnalysis::produce." << endl;

  if (find(objectsToGet.begin(), objectsToGet.end(), "triggers") != objectsToGet.end()) {
    event.getByLabel (triggers_, triggers);
    if (!triggers.product()) clog << "ERROR:  could not get triggers input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "trigobjs") != objectsToGet.end()) {
    event.getByLabel (trigobjs_, trigobjs);
    if (!trigobjs.product()) clog << "ERROR:  could not get trigobjs input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "jets") != objectsToGet.end()) {
    event.getByLabel (jets_, jets);
    if (!jets.product()) clog << "ERROR:  could not get jets input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "muons") != objectsToGet.end()) {
    event.getByLabel (muons_, muons);
    if (!muons.product()) clog << "ERROR:  could not get muons input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "secondary muons") != objectsToGet.end()) {
    event.getByLabel (secMuons_, secMuons);
    if (!secMuons.product()) clog << "ERROR:  could not get secMuons input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "electrons") != objectsToGet.end()) {
    event.getByLabel (electrons_, electrons);
    if (!electrons.product()) clog << "ERROR:  could not get electrons input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "events") != objectsToGet.end()) {
    event.getByLabel (events_, events);
    if (!events.product()) clog << "ERROR:  could not get events input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "taus") != objectsToGet.end()) {
    event.getByLabel (taus_, taus);
    if (!taus.product()) clog << "ERROR:  could not get taus input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "mets") != objectsToGet.end()) {
    event.getByLabel (mets_, mets);
    if (!mets.product()) clog << "ERROR:  could not get mets input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "tracks") != objectsToGet.end()) {
    event.getByLabel (tracks_, tracks);
    if (!tracks.product()) clog << "ERROR:  could not get tracks input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "genjets") != objectsToGet.end()) {
    event.getByLabel (genjets_, genjets);
    if (!genjets.product()) clog << "ERROR:  could not get genjets input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "mcparticles") != objectsToGet.end()) {
    event.getByLabel (mcparticles_, mcparticles);
    if (!mcparticles.product()) clog << "ERROR:  could not get mcparticles input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "primaryvertexs") != objectsToGet.end()) {
    event.getByLabel (primaryvertexs_, primaryvertexs);
    if (!primaryvertexs.product()) clog << "ERROR:  could not get primaryvertexs input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "bxlumis") != objectsToGet.end()) {
    event.getByLabel (bxlumis_, bxlumis);
    if (!bxlumis.product()) clog << "ERROR:  could not get bxlumis input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "photons") != objectsToGet.end()) {
    event.getByLabel (photons_, photons);
    if (!photons.product()) clog << "ERROR:  could not get photons input collection" << endl;
  }

  if (find(objectsToGet.begin(), objectsToGet.end(), "superclusters") != objectsToGet.end()) {
    event.getByLabel (superclusters_, superclusters);
    if (!superclusters.product()) clog << "ERROR:  could not get superclusters input collection" << endl;
  }

  if (datasetType_ == "signalMC"){
    if (find(objectsToGet.begin(), objectsToGet.end(), "stops") != objectsToGet.end()) {
      event.getByLabel (stops_, stops);
      if (!stops.product()) clog << "ERROR:  could not get stops input collection" << endl;
    }
  }

  if (useTrackCaloRhoCorr_) {
    // Used only for pile-up correction of by-hand calculation of isolation energy.
    // This rho collection is not available in all BEANs.
    // For description of rho values for different jet reconstruction algorithms, see
    // https://twiki.cern.ch/twiki/bin/view/CMS/JetAlgorithms#Algorithms
    event.getByLabel ("kt6CaloJets","rho", rhokt6CaloJetsHandle_);
    if (!rhokt6CaloJetsHandle_.product()) clog << "ERROR:  could not get kt6CaloJets input collection" << endl;
  }



  // ///////////////////////////////////////////////////////////
  // start application of various scale factors
  // some are applied equally to all channels,
  // while others can vary from channel to channel
  // ///////////////////////////////////////////////////////////

  globalScaleFactor_ = 1.0; //this variable holds the product of all SFs applied equally to all channels

  //apply trigger efficiency
  if (!applyTriggerSF_ || datasetType_ == "data") triggerScaleFactor_ = 1.0; //reset the variable to 1 if we're not applying it, so it will take that value in its histogram
  if (applyTriggerSF_ && datasetType_ != "data") globalScaleFactor_ *= triggerScaleFactor_;



  //get pile-up event weight
  if (doPileupReweighting_ && datasetType_ != "data") {
    //for "data" datasets, the numTruePV is always set to -1
    if (events->at(0).numTruePV < 0 && isFirstEvent_) {
      clog << "WARNING[OSUAnalysis::analyze]: Event has numTruePV<0.  Turning off pile-up reweighting." << endl;
      doPileupReweighting_ = false;
    }
    else globalScaleFactor_ *= puWeight_->at (events->at (0).numTruePV);
  }

  stopCTauScaleFactor_ = 1.0;
  if (datasetType_ == "signalMC" && 
      (regex_match (dataset_, regex ("stop.*to.*_.*mm.*")) || 
       //       regex_match (dataset_, regex ("AMSB.*RewtCtau.*cm")))
       regex_match (dataset_, regex ("AMSB.*cm")))
      ) stopCTauScaleFactor_ = stopCTauWeight_->at (event);
  globalScaleFactor_ *= stopCTauScaleFactor_;

  topPtScaleFactor_ = 1.0;
  if (doTopPtReweighting_ && dataset_.find("TTbar_")!=string::npos)
    topPtScaleFactor_ = getTopPtWeight();
  globalScaleFactor_ *= topPtScaleFactor_;


  //loop over all channels
  auto_ptr<map<string, bool> > channelDecisions (new map<string, bool>);

  for(uint currentChannelIndex = 0; currentChannelIndex != channels.size(); currentChannelIndex++){

    channel currentChannel = channels.at(currentChannelIndex);
    if (verbose_>1) clog << " Processing channel " << currentChannel.name << endl;

    flagMap individualFlags;
    counterMap passingCounter;
    cumulativeFlags.clear ();

    for (map<string, vector<float>>::iterator iter = BNTreeBranchVals_.begin();
         iter != BNTreeBranchVals_.end(); iter++) {
      iter->second.clear();  // clear array
    }

    bool triggerDecision = true;
    if(currentChannel.triggers.size() != 0 || currentChannel.triggersToVeto.size() != 0){  //triggers specified
      triggerDecision = evaluateTriggers(currentChannel.triggers, currentChannel.triggersToVeto, triggers.product());
      cutFlows_.at(currentChannelIndex)->at ("trigger") = triggerDecision;
    }

    bool notInStatus3Decision = true;
    vector<int> inStatus3Outgoing (currentChannel.inStatus3Outgoing),
                absInStatus3Outgoing (currentChannel.absInStatus3Outgoing);
    unsigned count = 0;
    for (BNmcparticleCollection::const_iterator mcparticle = mcparticles->begin () + 6; mcparticle < mcparticles->end (); mcparticle++, count++)
      {
        if (mcparticle->status != 3)
          break;
        bool erased = false;
        for (vector<int>::iterator id = inStatus3Outgoing.begin (); id != inStatus3Outgoing.end (); id++)
          {
            if (mcparticle->id == *id)
              {
                inStatus3Outgoing.erase (id);
                erased = true;
                break;
              }
          }
        for (vector<int>::iterator id = absInStatus3Outgoing.begin (); id != absInStatus3Outgoing.end () && !erased; id++)
          {
            if (abs (mcparticle->id) == abs (*id))
              {
                absInStatus3Outgoing.erase (id);
                break;
              }
          }
        for (vector<int>::const_iterator id = currentChannel.notInStatus3Outgoing.begin (); id != currentChannel.notInStatus3Outgoing.end () && notInStatus3Decision; id++)
          {
            if (mcparticle->id == *id)
              notInStatus3Decision = false;
          }
        for (vector<int>::const_iterator id = currentChannel.absNotInStatus3Outgoing.begin (); id != currentChannel.absNotInStatus3Outgoing.end () && notInStatus3Decision; id++)
          {
            if (abs (mcparticle->id) == abs (*id))
              notInStatus3Decision = false;
          }
      }
    if (!count && mcparticles->size ()
     && (currentChannel.inStatus3Outgoing.size () || currentChannel.notInStatus3Outgoing.size ()
     || currentChannel.absInStatus3Outgoing.size () || currentChannel.absNotInStatus3Outgoing.size ()))
      {
        clog << "WARNING[OSUAnalysis::analyze]: mcparticle collection has no status 3 particles." << endl;
        clog << "WARNING[OSUAnalysis::analyze]: turning off status 3 inclusion/exclusion cuts." << endl;
        channels.at (currentChannelIndex).inStatus3Outgoing.clear ();
        channels.at (currentChannelIndex).notInStatus3Outgoing.clear ();
        channels.at (currentChannelIndex).absInStatus3Outgoing.clear ();
        channels.at (currentChannelIndex).absNotInStatus3Outgoing.clear ();
        notInStatus3Decision = true;
        inStatus3Outgoing.clear ();
        absInStatus3Outgoing.clear ();
      }
    if (currentChannel.inStatus3Outgoing.size () || currentChannel.absInStatus3Outgoing.size ())
      cutFlows_.at(currentChannelIndex)->at (currentChannel.inStatus3OutgoingCutName) = !inStatus3Outgoing.size () && !absInStatus3Outgoing.size ();
    if (currentChannel.notInStatus3Outgoing.size () || currentChannel.absNotInStatus3Outgoing.size ())
      cutFlows_.at(currentChannelIndex)->at (currentChannel.notInStatus3OutgoingCutName) = notInStatus3Decision;

    //loop over all cuts
    for(uint currentCutIndex = 0; currentCutIndex != currentChannel.cuts.size(); currentCutIndex++){
      cut currentCut = currentChannel.cuts.at(currentCutIndex);
      if (verbose_>2) clog << "  Processing cut, index: " << currentCutIndex << ", input collection: " << currentCut.inputCollection << ", name: " << currentCut.name << endl;

      for(uint currentObjectIndex = 0; currentObjectIndex != objectsToFlag.size(); currentObjectIndex++){
        string currentObject = objectsToFlag.at(currentObjectIndex);

        //initialize maps to get ready to set cuts
        individualFlags[currentObject].push_back (vector<pair<bool,bool> > ());
        cumulativeFlags[currentObject].push_back (vector<pair<bool,bool> > ());

      }

      //set flags for all relevant objects
      for(int currentObjectIndex = -1; currentObjectIndex != int(objectsToFlag.size()); currentObjectIndex++){
        string currentObject;
        if (currentObjectIndex < 0) currentObject = currentCut.inputCollection;  // In the first loop, set the flags for the collection that the cut is acting on.
                                                                                 // That way other paired collections can access the correct flag for the current cut.
        else currentObject = objectsToFlag.at(currentObjectIndex);
        if (currentObjectIndex >= 0 && currentObject == currentCut.inputCollection) continue;  // Flags have already been set for the inputCollection object, so should not be set again.

        // single object collections
        if     (currentObject == "jets")                setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,jets.product(),"jets");
        else if(currentObject == "secondary jets")      setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,jets.product(),"secondary jets");
        else if(currentObject == "secondary photons")   setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,photons.product(),"secondary photons");
        else if(currentObject == "muons")               setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),"muons");
        else if(currentObject == "secondary muons")     setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,secMuons.product(),"secondary muons");
        else if(currentObject == "secondary electrons") setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),"secondary electrons");
        else if(currentObject == "electrons")           setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),"electrons");
        else if(currentObject == "events")              setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,events.product(),"events");
        else if(currentObject == "taus")                setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,taus.product(),"taus");
        else if(currentObject == "mets")                setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,mets.product(),"mets");
        else if(currentObject == "tracks")              setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,tracks.product(),"tracks");
        else if(currentObject == "genjets")             setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,genjets.product(),"genjets");
        else if(currentObject == "mcparticles")         setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,mcparticles.product(),"mcparticles");
        else if(currentObject == "secondary mcparticles")setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,mcparticles.product(),"secondary mcparticles");
        else if(currentObject == "primaryvertexs")      setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,primaryvertexs.product(),"primaryvertexs");
        else if(currentObject == "bxlumis")             setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,bxlumis.product(),"bxlumis");
        else if(currentObject == "photons")             setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,photons.product(),"photons");
        else if(currentObject == "superclusters")       setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,superclusters.product(),"superclusters");
        else if(currentObject == "trigobjs")            setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,trigobjs.product(),"trigobjs");


        // paired object collections
        else if(currentObject == "muon-muon pairs")                   setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),muons.product(), "muon-muon pairs");
        else if(currentObject == "muon-secondary muon pairs")         setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),secMuons.product(), "muon-secondary muon pairs");

        else if(currentObject == "muon-secondary photon pairs")       setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),photons.product(), "muon-secondary photon pairs");
        else if(currentObject == "muon-secondary jet pairs")          setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),jets.product(), "muon-secondary jet pairs");
        else if(currentObject == "photon-secondary jet pairs")        setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,photons.product(),jets.product(), "photon-secondary jet pairs");
        else if(currentObject == "electron-secondary jet pairs")      setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),jets.product(), "electron-secondary jet pairs");
        else if(currentObject == "electron-secondary electron pairs") setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),electrons.product(), "electron-secondary electron pairs");

        else if(currentObject == "electron-electron pairs")           setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),electrons.product(), "electron-electron pairs");
        else if(currentObject == "electron-muon pairs")     setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),muons.product(), "electron-muon pairs");
        else if(currentObject == "jet-secondary jet pairs") setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,jets.product(),jets.product(), "jet-secondary jet pairs");
        else if(currentObject == "electron-jet pairs")      setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),jets.product(), "electron-jet pairs");
        else if(currentObject == "secondary electron-jet pairs")      setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),jets.product(), "secondary electron-jet pairs");
        else if(currentObject == "secondary muon-jet pairs")      setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),jets.product(), "secondary muon-jet pairs");
        else if(currentObject == "electron-photon pairs")   setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),photons.product(), "electron-photon pairs");
        else if(currentObject == "photon-jet pairs")        setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,photons.product(),jets.product(), "photon-jet pairs");
        else if(currentObject == "muon-jet pairs")          setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),jets.product(), "muon-jet pairs");
        else if(currentObject == "muon-event pairs")        setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),events.product(), "muon-event pairs");
        else if(currentObject == "electron-event pairs")        setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),events.product(), "electron-event pairs");
        else if(currentObject == "met-jet pairs")           setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,mets.product(),jets.product(), "met-jet pairs");
        else if(currentObject == "met-mcparticle pairs")    setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,mets.product(),mcparticles.product(), "met-mcparticle pairs");
        else if(currentObject == "jet-mcparticle pairs")    setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,jets.product(),mcparticles.product(), "jet-mcparticle pairs");
        else if(currentObject == "jet-secondary mcparticle pairs")    setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,jets.product(),mcparticles.product(), "jet-secondary mcparticle pairs");
        else if(currentObject == "mcparticle-secondary mcparticle pairs")    setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,mcparticles.product(),mcparticles.product(), "mcparticle-secondary mcparticle pairs");
        else if(currentObject == "track-jet pairs")         setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,tracks.product(),jets.product(), "track-jet pairs");
        else if(currentObject == "muon-photon pairs")       setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),photons.product(), "muon-photon pairs");
        else if(currentObject == "track-event pairs")       setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,tracks.product(),events.product(), "track-event pairs");
        else if(currentObject == "electron-track pairs")    setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),tracks.product(),"electron-track pairs");
        else if(currentObject == "muon-track pairs")        setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),tracks.product(),"muon-track pairs");
        else if(currentObject == "jet-jet pairs")        setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,jets.product(),jets.product(),"jet-jet pairs");
        else if(currentObject == "jet-track pairs")        setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,jets.product(),tracks.product(),"jet-track pairs");
        else if(currentObject == "secondary muon-track pairs") setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,secMuons.product(),tracks.product(),"secondary muon-track pairs");
        else if(currentObject == "muon-tau pairs")          setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),taus.product(),"muon-tau pairs");
        else if(currentObject == "tau-tau pairs")           setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,taus.product(),taus.product(),"tau-tau pairs");
        else if(currentObject == "tau-track pairs")         setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,taus .product(),tracks.product(),"tau-track pairs");
        else if(currentObject == "electron-trigobj pairs")  setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),trigobjs.product(),"electron-trigobj pairs");
        else if(currentObject == "muon-trigobj pairs")      setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),trigobjs.product(),"muon-trigobj pairs");
        else if(currentObject == "electron-mcparticle pairs") setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),mcparticles.product(),"electron-mcparticle pairs");
        else if(currentObject == "muon-secondary mcparticle pairs") setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),mcparticles.product(),"muon-secondary mcparticle pairs");
        else if(currentObject == "muon-mcparticle pairs") setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),mcparticles.product(),"muon-mcparticle pairs");
        else if(currentObject == "track-mcparticle pairs")  setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,tracks.product(),mcparticles.product(),"track-mcparticle pairs");
	else if(currentObject == "stop-mcparticle pairs")   setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,stops.product(), mcparticles.product(),"stop-mcparticle pairs");    
        if(currentObject == "stops" && datasetType_ == "signalMC") setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,stops.product(),"stops");
      }

    }//end loop over all cuts



    //use cumulative flags to apply cuts at event level
    vector<bool> eventPassedPreviousCuts;    //a vector to store cumulative cut decisions after each cut.
    bool eventPassedAllCuts = true;
    eventPassedAllCuts = eventPassedAllCuts && !inStatus3Outgoing.size () && !absInStatus3Outgoing.size () && notInStatus3Decision && triggerDecision;    //apply trigger (true if none were specified)

    for(uint currentCutIndex = 0; currentCutIndex != currentChannel.cuts.size(); currentCutIndex++){

      //loop over all objects and count how many passed the cumulative selection up to this point
      cut currentCut = currentChannel.cuts.at(currentCutIndex);
      int numberPassing = 0;
      int numberPassingPrev = 0;  // number of objects that pass cuts up to the previous one

      for (uint object = 0; object != cumulativeFlags.at(currentCut.inputCollection).at(currentCutIndex).size() ; object++){
        if(cumulativeFlags.at(currentCut.inputCollection).at(currentCutIndex).at(object).first) numberPassing++;
      }
      bool cutDecision;
      if (!currentCut.isVeto) {
        cutDecision = evaluateComparison(numberPassing,currentCut.eventComparativeOperator,currentCut.numberRequired);
      } else {
        int prevCutIndex = currentCutIndex - 1;
        if (prevCutIndex<0) {
          numberPassingPrev = cumulativeFlags.at(currentCut.inputCollection).at(currentCutIndex).size();  // count all objects in collection if cut is the first
        } else {
          for (uint object = 0; object != cumulativeFlags.at(currentCut.inputCollection).at(prevCutIndex).size() ; object++){
            if (cumulativeFlags.at(currentCut.inputCollection).at(prevCutIndex).at(object).first) {
              numberPassingPrev++;
              if (verbose_>1) clog << " object passed previous cut" << endl;
            }
          }
        }
        int numberFailCut = numberPassingPrev - numberPassing;
        cutDecision = evaluateComparison(numberFailCut,currentCut.eventComparativeOperator,currentCut.numberRequired);
      }

      cutFlows_.at(currentChannelIndex)->at (currentCut.name) = cutDecision;
      eventPassedAllCuts = eventPassedAllCuts && cutDecision;
      eventPassedPreviousCuts.push_back(eventPassedAllCuts);
      if (verbose_>1) clog << " Event passed cuts up to cut index " << currentCutIndex << endl;

    }

    //applying all appropriate scale factors for individual objects

    channelScaleFactor_ = 1.0; //this variable holds the product of all SFs calculated separately for each channel

    muonScaleFactor_ = electronScaleFactor_ =  muonTrackScaleFactor_ =  electronTrackScaleFactor_ = bTagScaleFactor_ = 1.0;
    triggerMetScaleFactor_    = 1.0;  
    trackNMissOutScaleFactor_ = 1.0;  
    EcaloVaryScaleFactor_ = 1.0;  
    isrVaryScaleFactor_       = 1.0;  
    muonCutEfficiency_ = electronCutEfficiency_ =  recoMuonEfficiency_ =  recoElectronEfficiency_ = 1.0;



    if(applyLeptonSF_ && datasetType_ != "data"){
      //only apply SFs if we've cut on this object
      if(find(objectsToCut.begin(),objectsToCut.end(),"muons") != objectsToCut.end ()){
        flagPair muonFlags;
        //get the last valid flags in the flag map
        for (int i = cumulativeFlags.at("muons").size() - 1; i >= 0; i--){
          if (cumulativeFlags.at("muons").at(i).size()){
            muonFlags = cumulativeFlags.at("muons").at(i);
            break;
          }
        }
        //apply the weight for each of those objects
        for (uint muonIndex = 0; muonIndex != muonFlags.size(); muonIndex++){
          if(!muonFlags.at(muonIndex).second) continue;
          int shiftUpDown = 0;
          if (muonSFShift_ == "up") shiftUpDown = 1;
          if (muonSFShift_ == "down") shiftUpDown = -1;
          muonScaleFactor_ *= muonSFWeight_->at (muons->at(muonIndex).eta, muons->at(muonIndex).pt,shiftUpDown);
        }
      }

      //only apply SFs if we've cut on this object
      if(find(objectsToCut.begin(),objectsToCut.end(),"electrons") != objectsToCut.end ()){
        flagPair electronFlags;
        //get the last valid flags in the flag map
        for (int i = cumulativeFlags.at("electrons").size() - 1; i >= 0; i--){
          if (cumulativeFlags.at("electrons").at(i).size()){
            electronFlags = cumulativeFlags.at("electrons").at(i);
            break;
          }
        }
        //apply the weight for each of those objects
        for (uint electronIndex = 0; electronIndex != electronFlags.size(); electronIndex++){
          if(!electronFlags.at(electronIndex).second) continue;
          int shiftUpDown = 0;
          if (electronSFShift_ == "up") shiftUpDown = 1;
          if (electronSFShift_ == "down") shiftUpDown = -1;
          electronScaleFactor_ *= electronSFWeight_->at (electrons->at(electronIndex).scEta, electrons->at(electronIndex).pt, shiftUpDown);
        }
      }
    }
      

    // Reweighting for generated event to emulate CMS reco efficiency and analysis cut efficiency

    if(applyGentoRecoEfficiency_ && datasetType_ != "data"){
      string dummy = "";


      //RecoElectronWeight function of d0Beamspot
      if(find(objectsToCut.begin(),objectsToCut.end(),"mcparticles") != objectsToCut.end ()){
        flagPair mcFlags;
        //get the last valid flags in the flag map
        for (int i = cumulativeFlags.at("mcparticles").size() - 1; i >= 0; i--){
          if (cumulativeFlags.at("mcparticles").at(i).size()){
            mcFlags = cumulativeFlags.at("mcparticles").at(i);
            break;
          }
        }
        //apply the weight for each of those objects
        for (uint mcparticleIndex = 0; mcparticleIndex != mcFlags.size(); mcparticleIndex++){
          if(!mcFlags.at(mcparticleIndex).second) continue;
	  recoElectronEfficiency_ *= recoElectronWeight_->at (abs(valueLookup(&mcparticles->at(mcparticleIndex), "d0Beamspot", "", dummy)));
	  recoElectronEfficiency_ *= electronCutWeight_->at (mcparticles->at(mcparticleIndex).pt);
        }
      }

      // RecoMuonWeight function of d0Beamspot
      if(find(objectsToCut.begin(),objectsToCut.end(),"secondary mcparticles") != objectsToCut.end ()){
        flagPair mcFlags;
        //get the last valid flags in the flag map
        for (int i = cumulativeFlags.at("secondary mcparticles").size() - 1; i >= 0; i--){
          if (cumulativeFlags.at("secondary mcparticles").at(i).size()){
	    mcFlags = cumulativeFlags.at("secondary mcparticles").at(i);
            break;
          }
        }
        //apply the weight for each of those objects
        for (uint secondaryMcParticleIndex = 0; secondaryMcParticleIndex != mcFlags.size(); secondaryMcParticleIndex++){
          if(!mcFlags.at(secondaryMcParticleIndex).second) continue;
	  recoMuonEfficiency_ *= recoMuonWeight_->at (abs(valueLookup(&mcparticles->at(secondaryMcParticleIndex), "d0Beamspot", "", dummy)));
        }
      }


      // ElectronCutWeight function of pt
      if(find(objectsToCut.begin(),objectsToCut.end(),"mcparticles") != objectsToCut.end ()){
        flagPair mcFlags;
        //get the last valid flags in the flag map
        for (int i = cumulativeFlags.at("mcparticles").size() - 1; i >= 0; i--){
          if (cumulativeFlags.at("mcparticles").at(i).size()){
            mcFlags = cumulativeFlags.at("mcparticles").at(i);
            break;
          }
        }
        //apply the weight for each of those objects
        for (uint mcparticleIndex = 0; mcparticleIndex != mcFlags.size(); mcparticleIndex++){
          if(!mcFlags.at(mcparticleIndex).second) continue;
	  electronCutEfficiency_ *= electronCutWeight_->at (mcparticles->at(mcparticleIndex).pt);
        }
      }

      // MuonCutWeight function of pt 
      if(find(objectsToCut.begin(),objectsToCut.end(),"secondary mcparticles") != objectsToCut.end ()){
        flagPair mcFlags;
        //get the last valid flags in the flag map
        for (int i = cumulativeFlags.at("secondary mcparticles").size() - 1; i >= 0; i--){
          if (cumulativeFlags.at("secondary mcparticles").at(i).size()){
	    mcFlags = cumulativeFlags.at("secondary mcparticles").at(i);
            break;
          }
        }
        //apply the weight for each of those objects
        for (uint secondaryMcParticleIndex = 0; secondaryMcParticleIndex != mcFlags.size(); secondaryMcParticleIndex++){
          if(!mcFlags.at(secondaryMcParticleIndex).second) continue;
	  muonCutEfficiency_ *= muonCutWeight_->at (mcparticles->at(secondaryMcParticleIndex).pt);
        }
      }
    }
     

    // Track weighting for muons

    if(applyTrackingSF_ && datasetType_ != "data"){
      //only apply SFs if we've cut on this object
      if(find(objectsToCut.begin(),objectsToCut.end(),"muons") != objectsToCut.end ()){
        flagPair muonFlags;
        //get the last valid flags in the flag map
        for (int i = cumulativeFlags.at("muons").size() - 1; i >= 0; i--){
          if (cumulativeFlags.at("muons").at(i).size()){
            muonFlags = cumulativeFlags.at("muons").at(i);
            break;
          }
        }
        //apply the weight for each of those objects
        for (uint muonIndex = 0; muonIndex != muonFlags.size(); muonIndex++){
          if(!muonFlags.at(muonIndex).second) continue;
          int shiftUpDown = 0;
          if (trackSFShift_ == "up") shiftUpDown = 1;
          if (trackSFShift_ == "down") shiftUpDown = -1;
	  //	  cout << "before applying trackSFWeight muons SF=" << muonTrackScaleFactor_ << "and d0 of muon =" << muons->at(muonIndex).correctedD0 << endl;
	  muonTrackScaleFactor_ *= trackSFWeight_->at (muons->at(muonIndex).correctedD0,shiftUpDown);
        }
      }
      
      // Track weighting for electrons

      //only apply SFs if we've cut on this object
      if(find(objectsToCut.begin(),objectsToCut.end(),"electrons") != objectsToCut.end ()){
        flagPair electronFlags;
        //get the last valid flags in the flag map
        for (int i = cumulativeFlags.at("electrons").size() - 1; i >= 0; i--){
          if (cumulativeFlags.at("electrons").at(i).size()){
            electronFlags = cumulativeFlags.at("electrons").at(i);
            break;
          }
        }
        //apply the weight for each of those objects
        for (uint electronIndex = 0; electronIndex != electronFlags.size(); electronIndex++){
          if(!electronFlags.at(electronIndex).second) continue;
          int shiftUpDown = 0;
          if (trackSFShift_ == "up") shiftUpDown = 1;
          if (trackSFShift_ == "down") shiftUpDown = -1;
	  electronTrackScaleFactor_ *= trackSFWeight_->at (electrons->at(electronIndex).correctedD0,shiftUpDown);
	}
      }
    }


    if(applyBtagSF_ && datasetType_ != "data"){
      //only apply SFs if we've cut on this object
      if(find(objectsToCut.begin(),objectsToCut.end(),"jets") != objectsToCut.end ()){
        flagPair jetFlags;
        vector<double> jetSFs;
        //get the last valid flags in the flag map
        for (int i = cumulativeFlags.at("jets").size() - 1; i >= 0; i--){
          if (cumulativeFlags.at("jets").at(i).size()){
            jetFlags = cumulativeFlags.at("jets").at(i);
            break;
          }
        }
        //apply the weight for each of those objects
        for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++){
          if(!jetFlags.at(jetIndex).second) continue;
          double jetSFTmp = bTagSFWeight_->sflookup(jets->at(jetIndex).btagCombinedSecVertex, jets->at(jetIndex).pt, jets->at(jetIndex).flavour, jets->at(jetIndex).eta);
          jetSFs.push_back(jetSFTmp);
        }
        bTagScaleFactor_ *= bTagSFWeight_->weight( jetSFs, minBtag_);
      }
    }

    if(triggerMetSFFile_ != "" && datasetType_ != "data"){
      int shiftUpDown = 0;
      if (triggerMetSFShift_ == "up")   shiftUpDown =  1; 
      if (triggerMetSFShift_ == "down") shiftUpDown = -1; 
      if (const BNmet *met = chosenMET ()) { 
	triggerMetScaleFactor_ *= triggerMetSFWeight_->at (met->pt, shiftUpDown);  
      }

    }

    if(trackNMissOutSFFile_ != "" && datasetType_ != "data"){
      if(find(objectsToCut.begin(),objectsToCut.end(),"tracks") != objectsToCut.end ()){
        flagPair trkFlags;
        //get the last valid flags in the flag map
        for (int i = cumulativeFlags.at("tracks").size() - 1; i >= 0; i--){
          if (cumulativeFlags.at("tracks").at(i).size()){
            trkFlags = cumulativeFlags.at("tracks").at(i);
            break;
          }
        }
        //apply the weight for each of those objects
        for (uint trkIndex = 0; trkIndex != trkFlags.size(); trkIndex++){
          if(!trkFlags.at(trkIndex).second) continue;
          int shiftUpDown = 0;
          if (trackNMissOutSFShift_ == "up")   shiftUpDown =  1; 
          if (trackNMissOutSFShift_ == "down") shiftUpDown = -1; 
	  trackNMissOutScaleFactor_ *= trackNMissOutSFWeight_->at (tracks->at(trkIndex).nHitsMissingOuter, shiftUpDown);
	}
      }
    }

    if(EcaloVarySFFile_ != "" && datasetType_ != "data"){
      if(find(objectsToCut.begin(),objectsToCut.end(),"tracks") != objectsToCut.end ()){
        flagPair trkFlags;
        //get the last valid flags in the flag map
        for (int i = cumulativeFlags.at("tracks").size() - 1; i >= 0; i--){
          if (cumulativeFlags.at("tracks").at(i).size()){
            trkFlags = cumulativeFlags.at("tracks").at(i);
            break;
          }
        }
        //apply the weight for each of those objects
        for (uint trkIndex = 0; trkIndex != trkFlags.size(); trkIndex++){
          if(!trkFlags.at(trkIndex).second) continue;
          int shiftUpDown = 0;
          if (EcaloVarySFShift_ == "up")   shiftUpDown =  1; 
          if (EcaloVarySFShift_ == "down") shiftUpDown = -1; 
	  string dummy = "";
	  double caloTot = valueLookup(&tracks->at(trkIndex), "caloTotDeltaRp5RhoCorr", "", dummy);
	  EcaloVaryScaleFactor_ *= EcaloVarySFWeight_->at (caloTot, shiftUpDown);
	}
      }
    }



    if (isrVarySFFile_ != "" && datasetType_ != "data"){
      int shiftUpDown = 0;
      if (isrVarySFShift_ == "up")   shiftUpDown =  1; 
      if (isrVarySFShift_ == "down") shiftUpDown = -1; 
      string dummy = ""; 
      double ptSusy = valueLookup(&events->at(0), "totalMcparticleStatus3SusyIdPt", "", dummy);  
      isrVaryScaleFactor_ *= isrVarySFWeight_->at(ptSusy, shiftUpDown);  
    }  
        
    channelScaleFactor_ *= muonScaleFactor_;
    channelScaleFactor_ *= electronScaleFactor_;
    channelScaleFactor_ *= muonTrackScaleFactor_;
    channelScaleFactor_ *= electronTrackScaleFactor_;
    channelScaleFactor_ *= bTagScaleFactor_;
    channelScaleFactor_ *= triggerMetScaleFactor_;
    channelScaleFactor_ *= trackNMissOutScaleFactor_;
    channelScaleFactor_ *= EcaloVaryScaleFactor_;
    channelScaleFactor_ *= isrVaryScaleFactor_;  
    channelScaleFactor_ *= recoElectronEfficiency_;
    channelScaleFactor_ *= recoMuonEfficiency_;
    channelScaleFactor_ *= electronCutEfficiency_;
    channelScaleFactor_ *= muonCutEfficiency_;


    


    //calculate the total scale factor for the event and fill the cutflow for each channel
    eventScaleFactor_ = globalScaleFactor_ * channelScaleFactor_;

    cutFlows_.at(currentChannelIndex)->fillCutFlow(globalScaleFactor_, channelScaleFactor_);

    if (verbose_>1) clog << " Scale factors applied:  "
                         << " muonScaleFactor_ = " << muonScaleFactor_
                         << ", electronScaleFactor_ = " << electronScaleFactor_
                         << ", bTagScaleFactor_ = " << bTagScaleFactor_
                         << ", triggerMetScaleFactor_ = " << triggerMetScaleFactor_
                         << ", trackNMissOutScaleFactor_ = " << trackNMissOutScaleFactor_
                         << ", isrVaryScaleFactor_ = " << isrVaryScaleFactor_ 
                         << ", total scale factor = " << eventScaleFactor_
                         << endl;

    if (printEventInfo_ && eventPassedAllCuts) {
      // Write information about event to screen, for testing purposes.
      clog << "Event passed all cuts in channel " <<  currentChannel.name
           << ":  run:lumi:evt=  "  << events->at(0).run
           << ":" << events->at(0).lumi
           << ":" << events->at(0).evt
           << endl;
      if (findEventsLog) {
        (*findEventsLog) << events->at(0).run
                         << ":" << events->at(0).lumi
                         << ":" << events->at(0).evt << endl;
      }

    }

    if(calcPdfWeights_){
      //Calculate weights every time 
      std::vector<double> weights = getPdfWeights();
      allNo++;
      allSF += eventScaleFactor_;
      allSF2 +=eventScaleFactor_*eventScaleFactor_;
      //add for all events                                                                                                                                                                                
      for(uint i=0; i<weights.size();i++){
        allSums[i]+=weights[i]*eventScaleFactor_;
      }


      allCentralW2+=weights[0]*eventScaleFactor_*weights[0]*eventScaleFactor_;

      if(eventPassedAllCuts) {
        //add weights for events that pass                                                                                                                                                                
        passedNo++;
        passedSF += eventScaleFactor_;
        passedSF2 += eventScaleFactor_*eventScaleFactor_;
        passedCentralW2+=weights[0]*eventScaleFactor_*weights[0]*eventScaleFactor_;

        for(uint i=0; i< weights.size();i++){
          passedSums[i]+=weights[i]*eventScaleFactor_;

        }
      }//Event passed all cuts                                                                                                                                                                            
    }//Calc pdf weights                                                                                                                                                                                   

    //filling histograms
    for(uint currentCut = 0; currentCut != oneDHists_.at(currentChannelIndex).size(); currentCut++){//loop over all the cuts in each channel; if GetPlotsAfterEachCut_ is false, only the last cut will be used.

      if (verbose_>2) clog << "  Filling histograms for currentcut = " << currentCut << endl;

      uint currentDir;
      if (!GetPlotsAfterEachCut_) { currentDir =  currentChannel.cuts.size() - oneDHists_.at(currentChannelIndex).size(); } //if GetPlotsAfterEachCut_ is false, set currentDir point to the last cut.
      else{
        currentDir = currentCut;
      }


      if(eventPassedPreviousCuts.at(currentDir)){

        for (uint histogramIndex = 0; histogramIndex != histograms.size(); histogramIndex++){
          histogram currentHistogram = histograms.at(histogramIndex);
	  string inputCollection = currentHistogram.inputCollection;

          if (cumulativeFlags.count(currentHistogram.inputCollection) == 0 && currentHistogram.name.find ("status3") == string::npos)
            clog << "Error: no flags found for collection:  " << currentHistogram.inputCollection << ", will cause a seg fault" << endl;

          if (verbose_>1) clog << " Filling histogram " << currentHistogram.name << " for collection " << currentHistogram.inputCollection << endl;

          if(currentHistogram.inputVariables.size() == 1){
            TH1D* histo;
            histo = oneDHists_.at(currentChannelIndex).at(currentCut).at(currentHistogram.name);
            if     (currentHistogram.name.find ("status3") != string::npos) fill1DStatus3Histogram(histo,mcparticles.product(),eventScaleFactor_);
            else if(inputCollection == "jets")            fill1DHistogram(histo,currentHistogram,jets.product(),cumulativeFlags.at("jets").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary jets")  fill1DHistogram(histo,currentHistogram,jets.product(),cumulativeFlags.at("secondary jets").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary photons")  fill1DHistogram(histo,currentHistogram,photons.product(),cumulativeFlags.at("secondary photons").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muons")           fill1DHistogram(histo,currentHistogram,muons.product(),cumulativeFlags.at("muons").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary muons") fill1DHistogram(histo,currentHistogram,secMuons.product(),cumulativeFlags.at("secondary muons").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary electrons") fill1DHistogram(histo,currentHistogram,electrons.product(),cumulativeFlags.at("secondary electrons").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-muon pairs") fill1DHistogram(histo,currentHistogram,muons.product(),muons.product(),
                                                                                           cumulativeFlags.at("muon-muon pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-secondary muon pairs") fill1DHistogram(histo,currentHistogram,muons.product(),secMuons.product(),
                                                                                                     cumulativeFlags.at("muon-secondary muon pairs").at(currentDir),inputCollection,eventScaleFactor_);
             else if(inputCollection == "muon-secondary photon pairs") fill1DHistogram(histo,currentHistogram,muons.product(),photons.product(),
                                                                                                     cumulativeFlags.at("muon-secondary photon pairs").at(currentDir),inputCollection,eventScaleFactor_);
             else if(inputCollection == "muon-secondary jet pairs") fill1DHistogram(histo,currentHistogram,muons.product(),jets.product(),
                                                                                                     cumulativeFlags.at("muon-secondary jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
             else if(inputCollection == "photon-secondary jet pairs") fill1DHistogram(histo,currentHistogram,photons.product(),jets.product(),
                                                                                                     cumulativeFlags.at("photon-secondary jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
             else if(inputCollection == "electron-secondary jet pairs") fill1DHistogram(histo,currentHistogram,electrons.product(),jets.product(),
                                                                                                     cumulativeFlags.at("electron-secondary jet pairs").at(currentDir),inputCollection,eventScaleFactor_);

            else if(inputCollection == "electrons") fill1DHistogram(histo,currentHistogram,electrons.product(),cumulativeFlags.at("electrons").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-electron pairs") fill1DHistogram(histo,currentHistogram,electrons.product(),electrons.product(),
                                                                                                   cumulativeFlags.at("electron-electron pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "jet-jet pairs") fill1DHistogram(histo,currentHistogram,jets.product(),jets.product(),
                                                                                         cumulativeFlags.at("jet-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
             else if(inputCollection == "jet-secondary jet pairs") fill1DHistogram(histo,currentHistogram,jets.product(),jets.product(),
                                                                                                     cumulativeFlags.at("jet-secondary jet pairs").at(currentDir),inputCollection,eventScaleFactor_);

            else if(inputCollection == "electron-secondary electron pairs") fill1DHistogram(histo,currentHistogram,electrons.product(),electrons.product(),
                                                                                                             cumulativeFlags.at("electron-secondary electron pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-muon pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),muons.product(),
                                                                                               cumulativeFlags.at("electron-muon pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-jet pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),jets.product(),
                                                                                              cumulativeFlags.at("electron-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary electron-jet pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),jets.product(),
                                                                                              cumulativeFlags.at("secondary electron-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary muon-jet pairs") fill1DHistogram(histo,currentHistogram, muons.product(),jets.product(),
                                                                                              cumulativeFlags.at("secondary muon-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "photon-jet pairs") fill1DHistogram(histo,currentHistogram, photons.product(),jets.product(),
                                                                                              cumulativeFlags.at("photon-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-jet pairs") fill1DHistogram(histo,currentHistogram, muons.product(),jets.product(),
                                                                                          cumulativeFlags.at("muon-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-event pairs") fill1DHistogram(histo,currentHistogram, muons.product(),events.product(),
                                                                                          cumulativeFlags.at("muon-event pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-event pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),events.product(),
                                                                                          cumulativeFlags.at("electron-event pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "met-jet pairs")  fill1DHistogram(histo,currentHistogram, mets.product(),jets.product(),
                                                                                          cumulativeFlags.at("met-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "met-mcparticle pairs")  fill1DHistogram(histo,currentHistogram, mets.product(),mcparticles.product(),
                                                                                          cumulativeFlags.at("met-mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "jet-mcparticle pairs")  fill1DHistogram(histo,currentHistogram, jets.product(),mcparticles.product(),
                                                                                          cumulativeFlags.at("jet-mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "jet-secondary mcparticle pairs")  fill1DHistogram(histo,currentHistogram, jets.product(),mcparticles.product(),
                                                                                          cumulativeFlags.at("jet-secondary mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "mcparticle-secondary mcparticle pairs")  fill1DHistogram(histo,currentHistogram, jets.product(),mcparticles.product(),
                                                                                          cumulativeFlags.at("mcparticle-secondary mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "track-jet pairs")  fill1DHistogram(histo,currentHistogram,tracks.product(),jets.product(),
                                                                                          cumulativeFlags.at("track-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-photon pairs") fill1DHistogram(histo,currentHistogram, muons.product(),photons.product(),
                                                                                          cumulativeFlags.at("muon-photon pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-photon pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),photons.product(),
                                                                                          cumulativeFlags.at("electron-photon pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-track pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),tracks.product(),
                                                                                                cumulativeFlags.at("electron-track pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-track pairs") fill1DHistogram(histo,currentHistogram, muons.product(),tracks.product(),
                                                                                            cumulativeFlags.at("muon-track pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "jet-track pairs") fill1DHistogram(histo,currentHistogram, jets.product(),tracks.product(),
                                                                                            cumulativeFlags.at("jet-track pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary muon-track pairs") fill1DHistogram(histo,currentHistogram, secMuons.product(),tracks.product(),
                                                                                            cumulativeFlags.at("secondary muon-track pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-tau pairs") fill1DHistogram(histo,currentHistogram, muons.product(),taus.product(),
                                                                                          cumulativeFlags.at("muon-tau pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "tau-tau pairs") fill1DHistogram(histo,currentHistogram, taus.product(),taus.product(),
                                                                                         cumulativeFlags.at("tau-tau pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "tau-track pairs") fill1DHistogram(histo,currentHistogram, taus.product(),tracks.product(),
                                                                                           cumulativeFlags.at("tau-track pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-trigobj pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),trigobjs.product(),
                                                                                                  cumulativeFlags.at("electron-trigobj pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-trigobj pairs") fill1DHistogram(histo,currentHistogram, muons.product(),trigobjs.product(),
                                                                                              cumulativeFlags.at("muon-trigobj pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-mcparticle pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),mcparticles.product(), cumulativeFlags.at("electron-mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-mcparticle pairs") fill1DHistogram(histo,currentHistogram, muons.product(),mcparticles.product(),
                                                                                              cumulativeFlags.at("muon-mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-secondary mcparticle pairs") fill1DHistogram(histo,currentHistogram, muons.product(),mcparticles.product(),
                                                                                              cumulativeFlags.at("muon-secondary mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "track-mcparticle pairs") fill1DHistogram(histo,currentHistogram, tracks.product(),mcparticles.product(),
                                                                                              cumulativeFlags.at("track-mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "stop-mcparticle pairs") fill1DHistogram(histo,currentHistogram, stops.product(),mcparticles.product(),         
                                                                                             cumulativeFlags.at("stop-mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_); 
            // fill the histograms of weighting factors with 1, to see the shape of a SF without any weight applied
            else if(inputCollection == "events" && currentHistogram.name.find("ScaleFactor")!=string::npos) fill1DHistogram(histo,currentHistogram,events.product(),cumulativeFlags.at("events").at(currentDir),inputCollection,1.0);
            else if(inputCollection == "events") fill1DHistogram(histo,currentHistogram,events.product(),cumulativeFlags.at("events").at(currentDir),inputCollection,eventScaleFactor_);

            else if(inputCollection == "taus") fill1DHistogram(histo,currentHistogram,taus.product(),cumulativeFlags.at("taus").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "mets") fill1DHistogram(histo,currentHistogram,mets.product(),cumulativeFlags.at("mets").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "tracks") fill1DHistogram(histo,currentHistogram,tracks.product(),cumulativeFlags.at("tracks").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "genjets") fill1DHistogram(histo,currentHistogram,genjets.product(),cumulativeFlags.at("genjets").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "mcparticles") fill1DHistogram(histo,currentHistogram,mcparticles.product(),cumulativeFlags.at("mcparticles").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary mcparticles") fill1DHistogram(histo,currentHistogram,mcparticles.product(),cumulativeFlags.at("secondary mcparticles").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "primaryvertexs") fill1DHistogram(histo,currentHistogram,primaryvertexs.product(),cumulativeFlags.at("primaryvertexs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "bxlumis") fill1DHistogram(histo,currentHistogram,bxlumis.product(),cumulativeFlags.at("bxlumis").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "photons") fill1DHistogram(histo,currentHistogram,photons.product(),cumulativeFlags.at("photons").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "superclusters") fill1DHistogram(histo,currentHistogram,superclusters.product(),cumulativeFlags.at("superclusters").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "trigobjs") fill1DHistogram(histo,currentHistogram,trigobjs.product(),cumulativeFlags.at("trigobjs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "stops" && datasetType_ == "signalMC") fill1DHistogram(histo,currentHistogram,stops.product(),cumulativeFlags.at("stops").at(currentDir),inputCollection,eventScaleFactor_);
          }
          else if(currentHistogram.inputVariables.size() == 2){
            TH2D* histo;
            histo = twoDHists_.at(currentChannelIndex).at(currentCut).at(currentHistogram.name);
            if(inputCollection == "jets")            fill2DHistogram(histo,currentHistogram,jets.product(),cumulativeFlags.at("jets").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary jets")  fill2DHistogram(histo,currentHistogram,jets.product(),cumulativeFlags.at("secondary jets").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary photons")  fill2DHistogram(histo,currentHistogram,photons.product(),cumulativeFlags.at("secondary photons").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muons")           fill2DHistogram(histo,currentHistogram,muons.product(),cumulativeFlags.at("muons").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary muons") fill2DHistogram(histo,currentHistogram,secMuons.product(),cumulativeFlags.at("secondary muons").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-muon pairs") fill2DHistogram(histo,currentHistogram,muons.product(),muons.product(),
                                                                                           cumulativeFlags.at("muon-muon pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-secondary muon pairs") fill2DHistogram(histo,currentHistogram,muons.product(),secMuons.product(),
                                                                                                     cumulativeFlags.at("muon-secondary muon pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-secondary photon pairs") fill2DHistogram(histo,currentHistogram,muons.product(),photons.product(),
                                                                                                     cumulativeFlags.at("muon-secondary photon pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-secondary jet pairs") fill2DHistogram(histo,currentHistogram,muons.product(),jets.product(),
                                                                                                     cumulativeFlags.at("muon-secondary jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-secondary jet pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),jets.product(),
                                                                                                     cumulativeFlags.at("electron-secondary jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "photon-secondary jet pairs") fill2DHistogram(histo,currentHistogram,photons.product(),jets.product(),
                                                                                                     cumulativeFlags.at("photon-secondary jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electrons") fill2DHistogram(histo,currentHistogram,electrons.product(),cumulativeFlags.at("electrons").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary electrons") fill2DHistogram(histo,currentHistogram,electrons.product(),cumulativeFlags.at("secondary electrons").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-electron pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),electrons.product(),
                                                                                                   cumulativeFlags.at("electron-electron pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "jet-jet pairs") fill2DHistogram(histo,currentHistogram,jets.product(),jets.product(),
                                                                                         cumulativeFlags.at("jet-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-secondary electron pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),electrons.product(),
                                                                                                             cumulativeFlags.at("electron-secondary electron pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-muon pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),muons.product(),
                                                                                               cumulativeFlags.at("electron-muon pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-jet pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),jets.product(),
                                                                                              cumulativeFlags.at("electron-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary electron-jet pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),jets.product(),
                                                                                              cumulativeFlags.at("secondary electron-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary muon-jet pairs") fill2DHistogram(histo,currentHistogram,muons.product(),jets.product(),
                                                                                              cumulativeFlags.at("secondary muon-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-photon pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),photons.product(),
                                                                                              cumulativeFlags.at("electron-photon pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-jet pairs") fill2DHistogram(histo,currentHistogram,muons.product(),jets.product(),
                                                                                          cumulativeFlags.at("muon-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-event pairs") fill2DHistogram(histo,currentHistogram,muons.product(),events.product(),
                                                                                          cumulativeFlags.at("muon-event pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-event pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),events.product(),
                                                                                          cumulativeFlags.at("electron-event pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "met-jet pairs") fill2DHistogram(histo,currentHistogram,mets.product(),jets.product(),
                                                                                         cumulativeFlags.at("met-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "met-mcparticle pairs") fill2DHistogram(histo,currentHistogram,mets.product(),mcparticles.product(),
                                                                                         cumulativeFlags.at("met-mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "jet-mcparticle pairs") fill2DHistogram(histo,currentHistogram,jets.product(),mcparticles.product(),
                                                                                         cumulativeFlags.at("jet-mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "jet-secondary mcparticle pairs") fill2DHistogram(histo,currentHistogram,jets.product(),mcparticles.product(),
                                                                                         cumulativeFlags.at("jet-secondary mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "mcparticle-secondary mcparticle pairs") fill2DHistogram(histo,currentHistogram,jets.product(),mcparticles.product(),
                                                                                         cumulativeFlags.at("mcparticle-secondary mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "track-jet pairs") fill2DHistogram(histo,currentHistogram,tracks.product(),jets.product(),
                                                                                         cumulativeFlags.at("track-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "photon-jet pairs") fill2DHistogram(histo,currentHistogram,photons.product(),jets.product(),
                                                                                          cumulativeFlags.at("photon-jet pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-photon pairs") fill2DHistogram(histo,currentHistogram,muons.product(),photons.product(),
                                                                                          cumulativeFlags.at("muon-photon pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-track pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),tracks.product(),
                                                                                                cumulativeFlags.at("electron-track pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-track pairs") fill2DHistogram(histo,currentHistogram,muons.product(),tracks.product(),
                                                                                            cumulativeFlags.at("muon-track pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "jet-track pairs") fill2DHistogram(histo,currentHistogram,jets.product(),tracks.product(),
                                                                                            cumulativeFlags.at("jet-track pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary muon-track pairs") fill2DHistogram(histo,currentHistogram,secMuons.product(),tracks.product(),
                                                                                            cumulativeFlags.at("muon-track pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-tau pairs") fill2DHistogram(histo,currentHistogram,muons.product(),taus.product(),
                                                                                          cumulativeFlags.at("muon-tau pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "tau-tau pairs") fill2DHistogram(histo,currentHistogram,taus.product(),taus.product(),
                                                                                         cumulativeFlags.at("tau-tau pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "tau-track pairs") fill2DHistogram(histo,currentHistogram,taus.product(),tracks.product(),
                                                                                           cumulativeFlags.at("tau-track pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-trigobj pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),trigobjs.product(),
                                                                                                  cumulativeFlags.at("electron-trigobj pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-trigobj pairs") fill2DHistogram(histo,currentHistogram,muons.product(),trigobjs.product(),
                                                                                              cumulativeFlags.at("muon-trigobj pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "electron-mcparticle pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),mcparticles.product(), cumulativeFlags.at("electron-mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-mcparticle pairs") fill2DHistogram(histo,currentHistogram,muons.product(),mcparticles.product(), cumulativeFlags.at("muon-mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "muon-secondary mcparticle pairs") fill2DHistogram(histo,currentHistogram,muons.product(),mcparticles.product(), cumulativeFlags.at("muon-secondary mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "track-mcparticle pairs") fill2DHistogram(histo,currentHistogram,tracks.product(),mcparticles.product(),
                                                                                              cumulativeFlags.at("track-mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "stop-mcparticle pairs") fill2DHistogram(histo,currentHistogram,stops.product(),mcparticles.product(),    
                                                                                             cumulativeFlags.at("stop-mcparticle pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "events") fill2DHistogram(histo,currentHistogram,events.product(),cumulativeFlags.at("events").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "taus") fill2DHistogram(histo,currentHistogram,taus.product(),cumulativeFlags.at("taus").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "mets") fill2DHistogram(histo,currentHistogram,mets.product(),cumulativeFlags.at("mets").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "tracks") fill2DHistogram(histo,currentHistogram,tracks.product(),cumulativeFlags.at("tracks").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "track-event pairs") fill2DHistogram(histo,currentHistogram,tracks.product(),events.product(),
                                                                                             cumulativeFlags.at("track-event pairs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "genjets") fill2DHistogram(histo,currentHistogram,genjets.product(),cumulativeFlags.at("genjets").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "mcparticles") fill2DHistogram(histo,currentHistogram,mcparticles.product(),cumulativeFlags.at("mcparticles").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "secondary mcparticles") fill2DHistogram(histo,currentHistogram,mcparticles.product(),cumulativeFlags.at("secondary mcparticles").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "primaryvertexs") fill2DHistogram(histo,currentHistogram,primaryvertexs.product(),cumulativeFlags.at("primaryvertexs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "bxlumis") fill2DHistogram(histo,currentHistogram,bxlumis.product(),cumulativeFlags.at("bxlumis").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "photons") fill2DHistogram(histo,currentHistogram,photons.product(),cumulativeFlags.at("photons").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "superclusters") fill2DHistogram(histo,currentHistogram,superclusters.product(),cumulativeFlags.at("superclusters").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "trigobjs") fill2DHistogram(histo,currentHistogram,trigobjs.product(),cumulativeFlags.at("trigobjs").at(currentDir),inputCollection,eventScaleFactor_);
            else if(inputCollection == "stops" && datasetType_ == "signalMC") fill2DHistogram(histo,currentHistogram,stops.product(),cumulativeFlags.at("stops").at(currentDir),inputCollection,eventScaleFactor_);
          }

        }


       //fills histograms with the sizes of collections
        for (uint currentObjectIndex = 0; currentObjectIndex != objectsToPlot.size(); currentObjectIndex++){

          string currentObject = objectsToPlot.at(currentObjectIndex);
          string objectToPlot = "";

          if (verbose_) clog << "Filling histogram of number of selected objects in collection: " << currentObject << endl;

          // Name of objectToPlot here must match the name specified in OSUAnalysis::OSUAnalysis().
          if(currentObject == "muon-muon pairs")                         objectToPlot = "dimuonPairs";
          else if(currentObject == "electron-electron pairs")            objectToPlot = "dielectronPairs";
          else if(currentObject == "electron-muon pairs")                objectToPlot = "electronMuonPairs";
          else if(currentObject == "electron-photon pairs")              objectToPlot = "electronPhotonPairs";
          else if(currentObject == "electron-jet pairs")                 objectToPlot = "electronJetPairs";
          else if(currentObject == "muon-jet pairs")                     objectToPlot = "muonJetPairs";
          else if(currentObject == "muon-event pairs")                   objectToPlot = "muonEventPairs";
          else if(currentObject == "electron-event pairs")               objectToPlot = "electronEventPairs";
          else if(currentObject == "muon-photon pairs")                  objectToPlot = "muonPhotonPairs";
          else if(currentObject == "photon-jet pairs")                   objectToPlot = "photonJetPairs";
          else if(currentObject == "met-jet pairs")                      objectToPlot = "metJetPairs";
          else if(currentObject == "met-mcparticle pairs")               objectToPlot = "metMcParticlePairs";
          else if(currentObject == "jet-mcparticle pairs")               objectToPlot = "jetMcParticlePairs";
          else if(currentObject == "jet-secondary mcparticle pairs")     objectToPlot = "jetSecondaryMcParticlePairs";
          else if(currentObject == "mcparticle-secondary mcparticle pairs")     objectToPlot = "mcParticleSecondaryMcParticlePairs";
          else if(currentObject == "track-jet pairs")                    objectToPlot = "trackJetPairs";
          else if(currentObject == "jet-jet pairs")                      objectToPlot = "dijetPairs";
          else if(currentObject == "jet-secondary jet pairs")            objectToPlot = "jetSecondaryJetPairs";
          else if(currentObject == "secondary jets")                     objectToPlot = "secondaryJets";
          else if(currentObject == "secondary photons")                  objectToPlot = "secondaryPhotons";
          else if(currentObject == "electron-track pairs")               objectToPlot = "electronTrackPairs";
          else if(currentObject == "muon-track pairs")                   objectToPlot = "muonTrackPairs";
          else if(currentObject == "jet-track pairs")                   objectToPlot = "jetTrackPairs";
          else if(currentObject == "secondary muon-track pairs")         objectToPlot = "secondaryMuonTrackPairs";
          else if(currentObject == "secondary muon-jet pairs")           objectToPlot = "secondaryMuonJetPairs";
          else if(currentObject == "secondary electron-jet pairs")       objectToPlot = "secondaryElectronJetPairs";
          else if(currentObject == "muon-tau pairs")                     objectToPlot = "muonTauPairs";
          else if(currentObject == "tau-tau pairs")                      objectToPlot = "ditauPairs";
          else if(currentObject == "tau-track pairs")                    objectToPlot = "tauTrackPairs";
          else if(currentObject == "track-event pairs")                  objectToPlot = "trackEventPairs";
          else if(currentObject == "muon-secondary muon pairs")          objectToPlot = "muonSecondaryMuonPairs";
          else if(currentObject == "secondary muons")                    objectToPlot = "secondaryMuons";
          else if(currentObject == "muon-secondary jet pairs")           objectToPlot = "muonSecondaryJetPairs";
          else if(currentObject == "muon-secondary photon pairs")        objectToPlot = "muonSecondaryPhotonPairs";
          else if(currentObject == "electron-secondary jet pairs")       objectToPlot = "electronSecondaryJetPairs";
          else if(currentObject == "photon-secondary jet pairs")         objectToPlot = "photonSecondaryJetPairs";
          else if(currentObject == "electron-secondary electron pairs")  objectToPlot = "electronSecondaryElectronPairs";
          else if(currentObject == "secondary electrons")                objectToPlot = "secondaryElectrons";
          else if(currentObject == "electron-trigobj pairs")             objectToPlot = "electronTrigobjPairs";
          else if(currentObject == "muon-trigobj pairs")                 objectToPlot = "muonTrigobjPairs";
          else if(currentObject == "electron-mcparticle pairs")          objectToPlot = "electronMCparticlePairs";
          else if(currentObject == "muon-mcparticle pairs")          objectToPlot = "muonMCparticlePairs";
          else if(currentObject == "muon-secondary mcparticle pairs")          objectToPlot = "muonSecondaryMCparticlePairs";
          else if(currentObject == "track-mcparticle pairs")             objectToPlot = "trackMCparticlePairs";
	  else if(currentObject == "stop-mcparticle pairs")              objectToPlot = "stopMCparticlePairs";       
          else objectToPlot = currentObject;

          string tempCurrentObject = objectToPlot;
          tempCurrentObject.at(0) = toupper(tempCurrentObject.at(0));
          string histoName = "num" + tempCurrentObject;


          if(find(objectsToPlot.begin(), objectsToPlot.end(), currentObject) != objectsToPlot.end()) {
            flagPair lastCutFlags = cumulativeFlags.at(currentObject).at(currentDir);
            int numToPlot = 0;
            for (uint iObj = 0; iObj != lastCutFlags.size(); iObj++){  // loop over all the objects
              if(lastCutFlags.at(iObj).second) {
                numToPlot++;
                if (verbose_>3) clog << "   Found object " << iObj << " in collection " << currentObject << " to plot." << endl;
              }
            }

            if(objectToPlot == "primaryvertexs"){
              oneDHists_.at(currentChannelIndex).at(currentCut).at(histoName+"BeforePileupCorrection")->Fill(primaryvertexs->size());
              oneDHists_.at(currentChannelIndex).at(currentCut).at(histoName+"AfterPileupCorrection")->Fill(primaryvertexs->size(),eventScaleFactor_);
            }
            else {
              if (printEventInfo_) clog << "Number of selected " << objectToPlot << " to plot:  " << numToPlot << endl;
              oneDHists_.at(currentChannelIndex).at(currentCut).at(histoName)->Fill(numToPlot,eventScaleFactor_);
            }
          }

        } // end for (uint currentObjectIndex = 0; currentObjectIndex != objectsToPlot.size(); currentObjectIndex++)


      } // end if(eventPassedPreviousCuts.at(currentDir))
    } // end loop over cuts

    if (!useEDMFormat_ && eventPassedAllCuts){
      // Assign BNTree variables
      for (uint iBranch = 0; iBranch != treeBranches_.size(); iBranch++) {
        BranchSpecs brSpecs = treeBranches_.at(iBranch);
        string coll = brSpecs.inputCollection;
        if (cumulativeFlags.count(coll) == 0) clog << "Error: no flags found for collection:  " << coll << ", will cause a seg fault" << endl;

        if     (coll == "jets")                assignTreeBranch(brSpecs,jets.product(),          cumulativeFlags.at(coll).back());
        else if(coll == "secondary jets")      assignTreeBranch(brSpecs,jets.product(),          cumulativeFlags.at(coll).back());
        else if(coll == "secondary photons")   assignTreeBranch(brSpecs,photons.product(),       cumulativeFlags.at(coll).back());
        else if(coll == "muons")               assignTreeBranch(brSpecs,muons.product(),         cumulativeFlags.at(coll).back());
        else if(coll == "secondary muons")     assignTreeBranch(brSpecs,secMuons.product(),      cumulativeFlags.at(coll).back());
        else if(coll == "electrons")           assignTreeBranch(brSpecs,electrons.product(),     cumulativeFlags.at(coll).back());
        else if(coll == "secondary electrons") assignTreeBranch(brSpecs,electrons.product(),     cumulativeFlags.at(coll).back());
        else if(coll == "events")              assignTreeBranch(brSpecs,events.product(),        cumulativeFlags.at(coll).back());
        else if(coll == "taus")                assignTreeBranch(brSpecs,taus.product(),          cumulativeFlags.at(coll).back());
        else if(coll == "mets")                assignTreeBranch(brSpecs,mets.product(),          cumulativeFlags.at(coll).back());
        else if(coll == "tracks")              assignTreeBranch(brSpecs,tracks.product(),        cumulativeFlags.at(coll).back());
        else if(coll == "genjets")             assignTreeBranch(brSpecs,genjets.product(),       cumulativeFlags.at(coll).back());
        else if(coll == "mcparticles")         assignTreeBranch(brSpecs,mcparticles.product(),   cumulativeFlags.at(coll).back());
        else if(coll == "secondary mcparticles")assignTreeBranch(brSpecs,mcparticles.product(),   cumulativeFlags.at(coll).back());
        else if(coll == "primaryvertexs")      assignTreeBranch(brSpecs,primaryvertexs.product(),cumulativeFlags.at(coll).back());
        else if(coll == "bxlumis")             assignTreeBranch(brSpecs,bxlumis.product(),       cumulativeFlags.at(coll).back());
        else if(coll == "photons")             assignTreeBranch(brSpecs,photons.product(),       cumulativeFlags.at(coll).back());
        else if(coll == "superclusters")       assignTreeBranch(brSpecs,superclusters.product(), cumulativeFlags.at(coll).back());
        else if(coll == "trigobjs")            assignTreeBranch(brSpecs,trigobjs.product(),      cumulativeFlags.at(coll).back());
        else if(coll == "stops"
                && datasetType_ == "signalMC") assignTreeBranch(brSpecs,stops.product(),         cumulativeFlags.at(coll).back());
      } // end loop over branches
      // set the evtLong, runInt, and lumiInt variables separately, since they are not type float
      BNTreeBranchVals_evtLong_ = events->at(0).evt;
      BNTreeBranchVals_runInt_  = events->at(0).run;
      BNTreeBranchVals_lumiInt_ = events->at(0).lumi;

      if (!BNTrees_.at(currentChannelIndex)) { clog << "ERROR:  Undefined BNTree.  Will likely seg fault." << endl; }
      BNTrees_.at(currentChannelIndex)->Fill();  // only fill if event has passed cuts
    }

    (*channelDecisions)[currentChannel.name] = eventPassedAllCuts;

  } // end loop over channel

  for(uint currentChannelIndex = 0; currentChannelIndex != channels.size(); currentChannelIndex++){
    channel currentChannel = channels.at(currentChannelIndex);
    masterCutFlow_->at(currentChannel.name) = channelDecisions->at (currentChannel.name);
  }
  masterCutFlow_->fillCutFlow();

  event.put (channelDecisions, "channelDecisions");
  channelDecisions.reset ();

  isFirstEvent_ = false;

  if (verbose_) clog << "Finished OSUAnalysis::produce." << endl;

} // end void OSUAnalysis::produce (const edm::Event &event, const edm::EventSetup &setup)



bool
OSUAnalysis::evaluateComparison (double testValue, string comparison, double cutValue){

  bool returnValue;

  if (verbose_>5) clog << "     deciding whether " << testValue << " " << comparison << " " << cutValue;

  if(comparison == ">")       returnValue = testValue >  cutValue;
  else if(comparison == ">=") returnValue = testValue >= cutValue;
  else if(comparison == "<")  returnValue = testValue <  cutValue;
  else if(comparison == "<=") returnValue = testValue <= cutValue;
  else if(comparison == "==") returnValue = testValue == cutValue;
  else if(comparison == "=") returnValue = testValue == cutValue;
  else if(comparison == "!=") returnValue = testValue != cutValue;
  else {clog << "WARNING: invalid comparison operator '" << comparison << "'\n"; returnValue = false;}

  if (verbose_>5) clog << "; outcome is: " << returnValue << endl;

  return returnValue;
}

bool
OSUAnalysis::evaluateComparison (string testValue, string comparison, string cutValue){

  bool returnValue;

  if (verbose_>5) clog << "     deciding whether " << testValue << " " << comparison << " " << cutValue;

  if(comparison == ">")       returnValue = testValue >  cutValue;
  else if(comparison == ">=") returnValue = testValue >= cutValue;
  else if(comparison == "<")  returnValue = testValue <  cutValue;
  else if(comparison == "<=") returnValue = testValue <= cutValue;
  else if(comparison == "==") returnValue = testValue == cutValue;
  else if(comparison == "=") returnValue = testValue == cutValue;
  else if(comparison == "!=") returnValue = testValue != cutValue;
  else {clog << "WARNING: invalid comparison operator '" << comparison << "'\n"; returnValue = false;}

  if (verbose_>5) clog << "; outcome is: " << returnValue << endl;

  return returnValue;

}

bool
OSUAnalysis::evaluateTriggers (vector<string> triggersToTest, vector<string> triggersToVeto, const BNtriggerCollection* triggerCollection){

  //initialize to false until a chosen trigger is passed
  bool triggerDecision = false;

  if (printAllTriggers_) clog << "Printing list of all available triggers (which this event may or may not pass):" << endl;
  //loop over all triggers defined in the event
  for (BNtriggerCollection::const_iterator trigger = triggerCollection->begin (); trigger != triggerCollection->end (); trigger++){

    for(uint triggerName = 0; triggerName != triggersToTest.size(); triggerName++){
    if (triggersToTest.at(triggerName) == "emulateHLT_MonoCentralPFJet80_PFMETnoMu105_NHEF0p95" && emulateHLT_MonoCentralPFJet80_PFMETnoMu105_NHEF0p95(targetTriggers_, trigobjs.product()) > 0) {
      triggerDecision = true;
	}
    }
    if (printAllTriggers_) clog << "   " << trigger->name << endl;

    //we're only interested in triggers that actually passed
    if(trigger->pass != 1) continue;

    //if the event passes one of the veto triggers, exit and return false
    for(uint triggerName = 0; triggerName != triggersToVeto.size(); triggerName++){
      if(trigger->name.find(triggersToVeto.at(triggerName))!=string::npos) return false;
    }
    //if the event passes one of the chosen triggers, set triggerDecision to true
    for(uint triggerName = 0; triggerName != triggersToTest.size(); triggerName++){
      if(trigger->name.find(triggersToTest.at(triggerName))!=string::npos) triggerDecision = true;
    }
  }

  printAllTriggers_ = false;  // only print triggers once, not every event

  //if none of the veto triggers fired:
  //return the OR of all the chosen triggers
  if (triggersToTest.size() != 0) return triggerDecision;
  //or if no triggers were defined return true
  else return true;
}


vector<string>
OSUAnalysis::splitString (string inputString){

  stringstream stringStream(inputString);
  istream_iterator<string> begin(stringStream);
  istream_iterator<string> end;
  vector<string> stringVector(begin, end);
  return stringVector;

}


void OSUAnalysis::getTwoObjs(string tempInputCollection, string& obj1, string& obj2) {
  // Set two object strings from the tempInputCollection string,
  // For example, if tempInputCollection is "electron-muon pairs",
  // then obj1 = "electrons" and obj2 = "muons".
  // Note that the objects have an "s" appended.

  int dashIndex = tempInputCollection.find("-");
  int spaceIndex = tempInputCollection.find_last_of(" ");
  int secondWordLength = spaceIndex - dashIndex;
  obj1 = tempInputCollection.substr(0,dashIndex) + "s";
  obj2 = tempInputCollection.substr(dashIndex+1,secondWordLength-1)+"s";

}


string OSUAnalysis::getObjToGet(string obj) {
  // Return the string corresponding to the object to get for the given obj string.
  // Right now this only handles the case in which obj contains "secondary",
  // e.g, "secondary muons".
  // Note that "s" is NOT appended.

  if (obj.find("secondary")==string::npos) return obj;  // "secondary" is not found
  int firstSpaceIndex = obj.find_first_of(" ");
  return obj.substr(firstSpaceIndex+1,obj.length()-1);

}


// Return the weighting factors for variable bins.
double 
OSUAnalysis::getVariableBinsWeights(vector<double> variableBins, double value){
  vector<double> variableBinWidths;
  for (unsigned i = 0; i != variableBins.size() - 1; i++){
        variableBinWidths.push_back(variableBins[i+1] - variableBins[i]);
 }
  sort(variableBinWidths.begin(), variableBinWidths.end());
  double scalingFactor = -999;
  double num = 1;
  double den = 1;
  for (unsigned i = 0; i != variableBins.size() - 1; i++){
	if (value >= variableBins.at(i) && value < variableBins.at(i+1)){
              den = (double)(variableBins.at(i+1) - variableBins.at(i));
              num = (double)variableBinWidths.at(0);
	      scalingFactor = (double) num/den;
       }
  }
  if (scalingFactor != -999){
       return scalingFactor;
     }
  else {return 1;}
}


double OSUAnalysis::emulateHLT_MonoCentralPFJet80_PFMETnoMu105_NHEF0p95(const vector<string> & targetTriggers, const BNtrigobjCollection * triggerObjects){

  double filterValue = 0;
  double filter1 = 0;
  double filter2 = 0;
  double filter3 = 0;
  double filter4 = 0;
  double filter5 = 0;

  for ( BNtrigobjCollection::const_iterator iObj = triggerObjects->begin();
        iObj != triggerObjects->end();
        iObj ++ ) {
    // look for a matching name again
    for (vector<string>::const_iterator iTarget = targetTriggers.begin();
         iTarget != targetTriggers.end();
         iTarget++) {

 if ( iObj->filter.find("hltL1sL1ETM40") != std::string::npos) {
   //   std::cout << "have found hltL1sL1ETM40 with pt = " << iObj->pt << std::endl;
   if (iObj->pt < 40) continue;
   filter1 = 1;
 }

 if ( iObj->filter.find("hltCentralJet65L1FastJet") != std::string::npos) {
   //   std::cout << "have found hltCentralJet65L1FastJet with pt = " << iObj->pt << std::endl;
   if (iObj->pt < 65) continue;
   filter2 = 1;
 }

 if ( iObj->filter.find("hltMET65") != std::string::npos) {
   //   std::cout << "have found hltMET65 with pt = " << iObj->pt << std::endl;
   if (iObj->pt < 65) continue;
   filter3 = 1;
 }

 //This filter does not have a straightforward pT cut; postpone for now
 /* if ( iObj->filter.find("hltPFNHEF95Filter") != std::string::npos) {
   std::cout << "have found hltPFNHEF95Filter with pt = " << iObj->pt << std::endl;
   }*/

 if ( iObj->filter.find("hltCentralPFJet80") != std::string::npos) {
   //   std::cout << "have found hltCentralPFJet80 with pt = " << iObj->pt << std::endl;
   if (iObj->pt < 80) continue;
   filter4 = 1;
 }

 if ( iObj->filter.find("hltPFMETWOM") != std::string::npos) {
   //  if ( iObj->filter.find("hltPFMETnoMu") != std::string::npos) {
   //   std::cout << "have found hltPFMETWOM with pt = " << iObj->pt << std::endl;
   if (iObj->pt < 105) continue;
   filter5 = 1;      
}
 if (filter1 == 1 && filter2 == 1 && filter3 == 1 && filter4 == 1 && filter5 ==1) filterValue = 1;
    }
  }
  return filterValue;


}


template <class InputCollection>
void OSUAnalysis::setObjectFlags(cut &currentCut, uint currentCutIndex, flagMap &individualFlags, flagMap &cumulativeFlags, InputCollection inputCollection, string inputType){

  if (verbose_>2) clog << "  Beginning setObjectFlags for cut " << currentCutIndex << ": " << currentCut.name
                       << ", inputType=" << inputType
                       << endl;
  if (currentCut.inputCollection.find("pair")!=string::npos)  {
    string obj1, obj2;
    getTwoObjs(currentCut.inputCollection, obj1, obj2);
    if (verbose_>2) clog << "  Two object types:  " << obj1 << ", " << obj2 << endl;
    if (inputType==obj1 ||
        inputType==obj2) {
      // Do not add a cut to individualFlags or cumulativeFlags, if the cut is on a paired collection,
      // and the inputType is a member of the pair.
      // The cut will instead be applied when the setObjectFlags() is called for the paired collection.
      // For example, if currentCut.inputCollection==electron-muon pairs,
      // then the flags should not be set here when inputType==muons or inputType==electrons.
      return;
    }
  }

  if (!inputCollection) clog << "ERROR:  invalid input collection for inputType=" << inputType << endl;

  if (verbose_>3) clog << "   Collection size:  " << inputCollection->size() << endl;

  for (uint object = 0; object != inputCollection->size(); object++){

    if (verbose_>4) clog << "    Setting flags for object " << object << endl;

    bool cutDecision = true;//object passes if this cut doesn't cut on that type of object
    bool plotDecision = true;

    if(currentCut.inputCollection == inputType){

      vector<bool> subcutDecisions;
      for( int subcutIndex = 0; subcutIndex != currentCut.numSubcuts; subcutIndex++){
        string stringValue = "";//!!!!
        double value = valueLookup(&inputCollection->at(object), currentCut.variables.at(subcutIndex), currentCut.functions.at(subcutIndex), stringValue);
        if (stringValue == "") subcutDecisions.push_back(evaluateComparison(value,currentCut.comparativeOperators.at(subcutIndex),currentCut.cutValues.at(subcutIndex)));
        else subcutDecisions.push_back(evaluateComparison(stringValue,currentCut.comparativeOperators.at(subcutIndex),currentCut.cutStringValues.at(subcutIndex)));

      }
      if(currentCut.numSubcuts == 1) cutDecision = subcutDecisions.at(0);
      else{
        bool tempDecision = subcutDecisions.at(0);
        for( int subcutIndex = 1; subcutIndex < currentCut.numSubcuts; subcutIndex++){
          if(currentCut.logicalOperators.at(subcutIndex-1) == "&" || currentCut.logicalOperators.at(subcutIndex-1) == "&&")
            tempDecision = tempDecision && subcutDecisions.at(subcutIndex);
          else if(currentCut.logicalOperators.at(subcutIndex-1) == "|"|| currentCut.logicalOperators.at(subcutIndex-1) == "||")
            tempDecision = tempDecision || subcutDecisions.at(subcutIndex);
        }
        cutDecision = tempDecision;
      }
      //invert the cut if this cut is a veto
      if(currentCut.isVeto) cutDecision = !cutDecision;
      plotDecision = cutDecision;
    }

    if (verbose_>4) clog << "     individual flag: " << cutDecision << endl;
    individualFlags.at(inputType).at(currentCutIndex).push_back(make_pair(cutDecision,plotDecision));

    //set flags for objects that pass this cut AND all the previous cuts
    bool previousCumulativeCutFlag = true;
    for(uint previousCutIndex = 0; previousCutIndex != currentCutIndex; previousCutIndex++){
      if(previousCumulativeCutFlag && individualFlags.at(inputType).at(previousCutIndex).at(object).first) previousCumulativeCutFlag = true;
      else{ previousCumulativeCutFlag = false; break;}
    }
    previousCumulativeCutFlag = previousCumulativeCutFlag && cutDecision;
    bool previousCumulativePlotFlag = true;
    for(uint previousCutIndex = 0; previousCutIndex != currentCutIndex; previousCutIndex++){
      if(previousCumulativePlotFlag && individualFlags.at(inputType).at(previousCutIndex).at(object).second) previousCumulativePlotFlag = true;
      else{ previousCumulativePlotFlag = false; break;}
    }
    previousCumulativePlotFlag = previousCumulativePlotFlag && plotDecision;

    if (verbose_>4) clog << "     cumulative flag: " << previousCumulativeCutFlag << endl;
    cumulativeFlags.at(inputType).at(currentCutIndex).push_back(make_pair(previousCumulativeCutFlag,previousCumulativePlotFlag));

  }  //   for (uint object = 0; object != inputCollection->size(); object++){

} // end void OSUAnalysis::setObjectFlags


template <class InputCollection1, class InputCollection2>
void OSUAnalysis::setObjectFlags(cut &currentCut, uint currentCutIndex, flagMap &individualFlags, flagMap &cumulativeFlags,
                                 InputCollection1 inputCollection1, InputCollection2 inputCollection2, string inputType){
  // This function sets the flags for the paired object collection.
  // If the cut is applying on the given paired object collection, then the flags for the single object collections are also set.
  // If not, then the flags for the paired object collection are taken as the AND of the flags for each single object collection.

  if (verbose_>2) clog << "  Beginning setObjectFlags for cut=" << currentCut.name
                       << ", inputType=" << inputType
                       << endl;

  // Get the strings for the two objects that make up the pair.
  string obj1Type, obj2Type;
  getTwoObjs(inputType, obj1Type, obj2Type);
  bool isTwoTypesOfObject = true;
  if (obj1Type==obj2Type) isTwoTypesOfObject = false;

  if (!inputCollection1 ||   
      !inputCollection2) clog << "ERROR:  invalid input collection for inputType=" << inputType << endl;    

  // Initialize the flags for individual objects to all be false, if the cut is on the pair.
  // Set them to true later, if any paired object passes (in which case both of its constituents should pass).
  if (currentCut.inputCollection == inputType) {
    for (uint object1 = 0; object1 != inputCollection1->size(); object1++) {
      individualFlags.at(obj1Type).at(currentCutIndex).push_back(make_pair(false,false));
      cumulativeFlags.at(obj1Type).at(currentCutIndex).push_back(make_pair(false,false));
    }
    if (isTwoTypesOfObject) { // Only initialize the second object if it is different from the first.
      for (uint object2 = 0; object2 != inputCollection2->size(); object2++)  {
        individualFlags.at(obj2Type).at(currentCutIndex).push_back(make_pair(false,false));
        cumulativeFlags.at(obj2Type).at(currentCutIndex).push_back(make_pair(false,false));
      }
    }
  }

  int counter = 0;

  for (uint object1 = 0; object1 != inputCollection1->size(); object1++){
    for (uint object2 = 0; object2 != inputCollection2->size(); object2++){

      if(!isTwoTypesOfObject && object1 >= object2) continue;//account for duplicate pairs if both collections are the same


      bool cutDecision  = true;//object passes if this cut doesn't cut on that type of object
      bool plotDecision = true;

      // Determine whether each pair passes the cut, only if inputCollection is the same as the inputType.
      if(currentCut.inputCollection == inputType){

        vector<bool> subcutDecisions;
        for( int subcutIndex = 0; subcutIndex != currentCut.numSubcuts; subcutIndex++){
          string stringValue = "";
          double value = valueLookup(&inputCollection1->at(object1), &inputCollection2->at(object2), currentCut.variables.at(subcutIndex), currentCut.functions.at(subcutIndex), stringValue);
          if (verbose_>1) clog << currentCut.variables.at(subcutIndex) << " = " << value
                               << endl;
          if (stringValue == "") subcutDecisions.push_back(evaluateComparison(value,currentCut.comparativeOperators.at(subcutIndex),currentCut.cutValues.at(subcutIndex)));
          else subcutDecisions.push_back(evaluateComparison(stringValue,currentCut.comparativeOperators.at(subcutIndex),currentCut.cutStringValues.at(subcutIndex)));
        }

        if(currentCut.numSubcuts == 1) cutDecision = subcutDecisions.at(0);
        else{
          bool tempDecision = subcutDecisions.at(0);
          for( int subcutIndex = 1; subcutIndex < currentCut.numSubcuts; subcutIndex++){
            if(currentCut.logicalOperators.at(subcutIndex-1) == "&" || currentCut.logicalOperators.at(subcutIndex-1) == "&&")
              tempDecision = tempDecision && subcutDecisions.at(subcutIndex);
            else if(currentCut.logicalOperators.at(subcutIndex-1) == "|"|| currentCut.logicalOperators.at(subcutIndex-1) == "||")
              tempDecision = tempDecision || subcutDecisions.at(subcutIndex);
          }
          cutDecision = tempDecision;
        }
        //invert the cut if this cut is a veto
        if (currentCut.isVeto) cutDecision = !cutDecision;
        plotDecision = cutDecision;

        if (verbose_>1)        clog << " cutDecision = " << cutDecision
                             << "; for currentCut.inputCollection = " << currentCut.inputCollection
                             << "; object1 (" << obj1Type << ") = " << object1
                             << "; object2 (" << obj2Type << ") = " << object2
                             << endl;

        if (cutDecision) {  // only set the flags for the individual objects if the pair object is being cut on
          individualFlags.at(obj1Type).at(currentCutIndex).at(object1).first = true;
          individualFlags.at(obj2Type).at(currentCutIndex).at(object2).first = true;
        }
        if (plotDecision) {  // only set the flags for the individual objects if the pair object is being cut on
          individualFlags.at(obj1Type).at(currentCutIndex).at(object1).second = true;
          individualFlags.at(obj2Type).at(currentCutIndex).at(object2).second = true;
        }


      } // if(currentCut.inputCollection == inputType){

      // The individualFlags will be true if the inputCollection is not the same as the inputType.
      // They are also independent of the previous flags on the single objects.
      individualFlags.at(inputType).at(currentCutIndex).push_back(make_pair(cutDecision,plotDecision));



      // ************************************
      // Determine cumulative flags
      // ************************************
      // determine whether this paired object passes this cut AND all previous cuts
      bool previousCumulativeCutFlag = true;
      for(uint previousCutIndex = 0; previousCutIndex != currentCutIndex; previousCutIndex++){
             if(previousCumulativeCutFlag && individualFlags.at(inputType).at(previousCutIndex).at(counter).first) previousCumulativeCutFlag = true;
             else{ previousCumulativeCutFlag = false; break;}
      }
      previousCumulativeCutFlag = previousCumulativeCutFlag && cutDecision;

      bool previousCumulativePlotFlag = true;
      for(uint previousCutIndex = 0; previousCutIndex != currentCutIndex; previousCutIndex++){
        if(previousCumulativePlotFlag && individualFlags.at(inputType).at(previousCutIndex).at(counter).second) previousCumulativePlotFlag = true;
        else{ previousCumulativePlotFlag = false; break;}
      }
      previousCumulativePlotFlag = previousCumulativePlotFlag && plotDecision;

      // Get the index for the flags of each of the single objects in the pair.  Usually this is the index of the previous cut, i.e., currentCutIndex-1.
      int cutIdxFlagsObj1 = max(int(currentCutIndex-1),0);
      int cutIdxFlagsObj2 = max(int(currentCutIndex-1),0);
      // If the inputCollection of the cut is not equal to the inputType but the inputCollection includes objects that are contained in inputType, then use the currentCutIndex for those collections.
      // For example, if the inputType is jet-jet pairs, and the inputCollection is track-jet pairs, then use currentCutIndex for cutIdxFlagsObj{1,2}, i.e., for both jets.
      // For example, if the inputType is jets, and the inputCollection is track-jet pairs, then use currentCutIndex for cutIdxFlagsObj2 (jets) and currentCutIndex-1 for cutIdxFlagsObj1 (tracks).
      if (currentCut.inputCollection != inputType) {
              if (currentCut.inputCollection.find(obj1Type)!=string::npos) cutIdxFlagsObj1 = currentCutIndex;
              if (currentCut.inputCollection.find(obj2Type)!=string::npos) cutIdxFlagsObj2 = currentCutIndex;
      }
      flagPair flags1 = cumulativeFlags.at(obj1Type).at(cutIdxFlagsObj1);  // flag for input collection 1
      flagPair flags2 = cumulativeFlags.at(obj2Type).at(cutIdxFlagsObj2);  // flag for input collection 2

      // The cumulative flag is only true if the paired object cumulative flag is true, and if the single object cumulative flags are true.
      bool currentCumulativeCutFlag = true;
      bool currentCumulativePlotFlag = true;
      if(flags1.size() == 0 && flags2.size() == 0) currentCumulativeCutFlag = previousCumulativeCutFlag;
      else if(flags1.size() == 0) currentCumulativeCutFlag = previousCumulativeCutFlag && flags2.at(object2).first;
      else if(flags2.size() == 0) currentCumulativeCutFlag = previousCumulativeCutFlag && flags1.at(object1).first;
      else currentCumulativeCutFlag = previousCumulativeCutFlag && flags1.at(object1).first && flags2.at(object2).first;

      if(flags1.size() == 0 && flags2.size() == 0) currentCumulativePlotFlag = previousCumulativePlotFlag;
      else if(flags1.size() == 0) currentCumulativePlotFlag = previousCumulativePlotFlag && flags2.at(object2).second;
      else if(flags2.size() == 0) currentCumulativePlotFlag = previousCumulativePlotFlag && flags1.at(object1).second;
      else currentCumulativePlotFlag = previousCumulativePlotFlag && flags1.at(object1).first && flags2.at(object2).second;

      cumulativeFlags.at(inputType).at(currentCutIndex).push_back(make_pair(currentCumulativeCutFlag,currentCumulativePlotFlag));  // Set the flag for the paired object


      if (currentCumulativeCutFlag && currentCut.inputCollection == inputType) {  // Set the flags for the individual objects if the paired object is being cut on.
        cumulativeFlags.at(obj1Type).at(currentCutIndex).at(object1).first  = true && getPreviousCumulativeFlags(currentCutIndex, individualFlags, obj1Type, object1, "cut");
        cumulativeFlags.at(obj2Type).at(currentCutIndex).at(object2).first  = true && getPreviousCumulativeFlags(currentCutIndex, individualFlags, obj2Type, object2, "cut");
        cumulativeFlags.at(obj1Type).at(currentCutIndex).at(object1).second = true && getPreviousCumulativeFlags(currentCutIndex, individualFlags, obj1Type, object1, "plot");
        cumulativeFlags.at(obj2Type).at(currentCutIndex).at(object2).second = true && getPreviousCumulativeFlags(currentCutIndex, individualFlags, obj2Type, object2, "plot");

        if (verbose_>1) clog << " previousCumulativeCutFlag for object1 = " << getPreviousCumulativeFlags(currentCutIndex, individualFlags, obj2Type, object1, "cut") << endl;
        if (verbose_>1) clog << " previousCumulativeCutFlag for object2 = " << getPreviousCumulativeFlags(currentCutIndex, individualFlags, obj2Type, object2, "cut") << endl;

      }

      counter++;

    } // end   for (uint object2 = 0; object2 != inputCollection2->size(); object2++)
  }  // end   for (uint object1 = 0; object1 != inputCollection1->size(); object1++)

} // end void OSUAnalysis::setObjectFlags


bool OSUAnalysis::getPreviousCumulativeFlags(uint currentCutIndex, flagMap &individualFlags, string obj1Type, uint object1, string flagType) {
  // Return true iff for the collection obj1Type, the element with index object1 has individal flags set to true for
  // all cuts up to currentCutIndex
  bool previousCumulativeFlag = true;
  for (uint previousCutIndex = 0; previousCutIndex < currentCutIndex; previousCutIndex++) {
    bool tempFlag = false;
    if(flagType == "cut") tempFlag = individualFlags.at(obj1Type).at(previousCutIndex).at(object1).first;
    else if(flagType == "plot") tempFlag = individualFlags.at(obj1Type).at(previousCutIndex).at(object1).second;

    if (previousCumulativeFlag && tempFlag) previousCumulativeFlag = true;
    else {
      previousCumulativeFlag = false; break;
    }
  }
  return previousCumulativeFlag;
}




template <class InputCollection>
void OSUAnalysis::assignTreeBranch(BranchSpecs parameters, InputCollection inputCollection, flagPair flags){
  // This function is similar to fill1DHistogram(), but instead of filling a histogram it assigns a value to a variable for the BNTree

  if (BNTreeBranchVals_.count(parameters.name)==0) clog << "Error[assignTreeBranch]:  trying to assign value to " << parameters.name << " that does not have a branch set up.  Will likely seg fault." << endl;
  for (uint object = 0; object != inputCollection->size(); object++) {

    if (!plotAllObjectsInPassingEvents_ && !flags.at(object).second) continue;

    string inputVariable = parameters.inputVariable;
    string function = "";
    string stringValue = "";
    double value = valueLookup(&inputCollection->at(object), inputVariable, function, stringValue);
    BNTreeBranchVals_.at(parameters.name).push_back(value);

  }
}


template <class InputCollection>
void OSUAnalysis::fill1DHistogram(TH1* histo, histogram parameters, InputCollection inputCollection, flagPair flags, string inputType, double scaleFactor){

  if (verbose_>2) clog << "  Filling histogram for " << parameters.name << endl;

  for (uint object = 0; object != inputCollection->size(); object++){

    if(!plotAllObjectsInPassingEvents_ && !flags.at(object).second) continue;

    string currentString = parameters.inputVariables.at(0);
    string inputVariable = "";
    string function = "";
    if(currentString.find("(")==string::npos){
      inputVariable = currentString;// variable to cut on
    }
    else{
      function = currentString.substr(0,currentString.find("("));//function comes before the "("
      inputVariable = currentString.substr(currentString.find("(")+1);//get rest of string
      inputVariable = inputVariable.substr(0,inputVariable.size()-1);//remove trailing ")"
    }

    string stringValue = "";
    double value = valueLookup(&inputCollection->at(object), inputVariable, function, stringValue);
    if( parameters.variableBinsX.size() != 0)
    { scaleFactor = scaleFactor*getVariableBinsWeights(parameters.variableBinsX,value);
    }
    histo->Fill(value,scaleFactor);
    
    if (printEventInfo_) {
      // Write information about event to screen, for testing purposes.
      clog << "  Info for event:  value for histogram " << histo->GetName() << ":  " << value << " (object number " << object << ")" << endl;
    }
    
  }
}

template <class InputCollection1, class InputCollection2>
void OSUAnalysis::fill1DHistogram(TH1* histo, histogram parameters, InputCollection1 inputCollection1, InputCollection2 inputCollection2,
                                  flagPair pairFlags, string inputType, double scaleFactor){

  // Get the strings for the two objects that make up the pair.
  string obj1Type, obj2Type;
  getTwoObjs(inputType, obj1Type, obj2Type);
  bool isTwoTypesOfObject = true;
  if (obj1Type==obj2Type) isTwoTypesOfObject = false;

  int pairCounter = -1;
  for (uint object1 = 0; object1 != inputCollection1->size(); object1++){
    for (uint object2 = 0; object2 != inputCollection2->size(); object2++){

      if(!isTwoTypesOfObject && object1 >= object2) continue;//account for duplicate pairs if both collections are the same

      pairCounter++;
      if(!plotAllObjectsInPassingEvents_ && !pairFlags.at(pairCounter).second) continue;

      string currentString = parameters.inputVariables.at(0);
      string inputVariable = "";
      string function = "";
      if(currentString.find("(")==string::npos){
        inputVariable = currentString;// variable to cut on
      }
      else{
        function = currentString.substr(0,currentString.find("("));//function comes before the "("
        inputVariable = currentString.substr(currentString.find("(")+1);//get rest of string
        inputVariable = inputVariable.substr(0,inputVariable.size()-1);//remove trailing ")"
      }

      string stringValue = "";
      double value = valueLookup(&inputCollection1->at(object1), &inputCollection2->at(object2), inputVariable, function, stringValue);
      histo->Fill(value,scaleFactor);

      if (printEventInfo_) {
        // Write information about event to screen, for testing purposes.
        clog << "  Info for event:  value for histogram " << histo->GetName() << ":  " << value
             << " (object1 number " << object1 << "), "
             << " (object2 number " << object2 << ")"
             << endl;
      }

    }
  }

}

void OSUAnalysis::fill1DStatus3Histogram(TH1* histo, const BNmcparticleCollection *inputCollection, double scaleFactor){

  if (verbose_>2) clog << "  Filling histogram for status 3 outgoing" << endl;

  for (unsigned i = 6; i < inputCollection->size (); i++)
    {
      if (inputCollection->at (i).status != 3)
        break;
      unsigned value = abs (inputCollection->at (i).id);
      value = getPdgIdBinValue (value);
      histo->Fill (value, scaleFactor);

      if (printEventInfo_) {
        // Write information about event to screen, for testing purposes.
        clog << "  Info for event:  value for histogram " << histo->GetName() << ":  " << value << " (object number " << i << ")" << endl;
      }
    }
}

template <class InputCollection>
void OSUAnalysis::fill2DHistogram(TH2* histo, histogram parameters, InputCollection inputCollection, flagPair flags, string inputType, double scaleFactor){

  for (uint object = 0; object != inputCollection->size(); object++){

    if(!plotAllObjectsInPassingEvents_ && !flags.at(object).second) continue;

    string stringValue = "";
    string currentString = parameters.inputVariables.at(0);
    string inputVariable = "";
    string function = "";
    if(currentString.find("(")==string::npos){
      inputVariable = currentString;// variable to cut on
    }
    else{
      function = currentString.substr(0,currentString.find("("));//function comes before the "("
      inputVariable = currentString.substr(currentString.find("(")+1);//get rest of string
      inputVariable = inputVariable.substr(0,inputVariable.size()-1);//remove trailing ")"
    }
    double valueX = valueLookup(&inputCollection->at(object), inputVariable, function, stringValue);

    currentString = parameters.inputVariables.at(1);
    inputVariable = "";
    function = "";
    if(currentString.find("(")==string::npos){
      inputVariable = currentString;// variable to cut on
    }
    else{
      function = currentString.substr(0,currentString.find("("));//function comes before the "("
      inputVariable = currentString.substr(currentString.find("(")+1);//get rest of string
      inputVariable = inputVariable.substr(0,inputVariable.size()-1);//remove trailing ")"
    }

    double valueY = valueLookup(&inputCollection->at(object), inputVariable, function, stringValue);

    histo->Fill(valueX,valueY,scaleFactor);

  }

}

template <class InputCollection1, class InputCollection2>
void OSUAnalysis::fill2DHistogram(TH2* histo, histogram parameters, InputCollection1 inputCollection1, InputCollection2 inputCollection2,
                                  flagPair pairFlags, string inputType, double scaleFactor){

  // Get the strings for the two objects that make up the pair.
  string obj1Type, obj2Type;
  getTwoObjs(inputType, obj1Type, obj2Type);
  bool isTwoTypesOfObject = true;
  if (obj1Type==obj2Type) isTwoTypesOfObject = false;

  int pairCounter = -1;
  for (uint object1 = 0; object1 != inputCollection1->size(); object1++){
    for (uint object2 = 0; object2 != inputCollection2->size(); object2++){

      if(!isTwoTypesOfObject && object1 >= object2) continue;//account for duplicate pairs if both collections are the same

      pairCounter++;

      if(!plotAllObjectsInPassingEvents_ && !pairFlags.at(pairCounter).second) continue;

      string stringValue = "";
      string currentString = parameters.inputVariables.at(0);
      string inputVariable = "";
      string function = "";
      if(currentString.find("(")==string::npos){
        inputVariable = currentString;// variable to cut on
      }
      else{
        function = currentString.substr(0,currentString.find("("));//function comes before the "("
        inputVariable = currentString.substr(currentString.find("(")+1);//get rest of string
        inputVariable = inputVariable.substr(0,inputVariable.size()-1);//remove trailing ")"
      }
      double valueX = valueLookup(&inputCollection1->at(object1), &inputCollection2->at(object2), inputVariable, function, stringValue);

      currentString = parameters.inputVariables.at(1);
      inputVariable = "";
      function = "";
      if(currentString.find("(")==string::npos){
        inputVariable = currentString;// variable to cut on
      }
      else{
        function = currentString.substr(0,currentString.find("("));//function comes before the "("
        inputVariable = currentString.substr(currentString.find("(")+1);//get rest of string
        inputVariable = inputVariable.substr(0,inputVariable.size()-1);//remove trailing ")"
      }
      double valueY = valueLookup(&inputCollection1->at(object1), &inputCollection2->at(object2), inputVariable, function, stringValue);


      histo->Fill(valueX,valueY,scaleFactor);

    }
  }

}


//lifted from BEANHelper.cc
double
OSUAnalysis::getTopPtWeight()
{
  double topPt = -1.0;

  for(BNmcparticleCollection::const_iterator mcparticle = mcparticles->begin (); mcparticle != mcparticles->end (); mcparticle++){
    if(mcparticle->status == 3 && mcparticle->id == 6){
      topPt = mcparticle->pt;
      break;
    }
  }

  if( topPt<0 ) return 1;

  double p0 = 1.18246e+00;
  double p1 = 4.63312e+02;
  double p2 = 2.10061e-06;

  if( topPt>p1 ) topPt = p1;

  double result = p0 + p2 * topPt * ( topPt - 2 * p1 );
  return result;

}



std::vector<double> OSUAnalysis::getPdfWeights()
{
  double x1 = events->at(0).x1;
  double x2 = events->at(0).x2;
  double q = events->at(0).qScale;
  double id1 = events->at(0).id1;
  double id2 = events->at(0).id2;

  std::vector<double> pdf_weights;

  //for CTEQ6L1 xPDF is ONLY PDF so we multiply with x1       
  //get weight from event //NOTE: EVNET WEIGHT DOES NOT HAVE X1 x2 contrary to what it says    
  double xpdf1 = (events->at(0).xPDF1)*x1;
  double xpdf2 = (events->at(0).xPDF2)*x2;
  double w0 = xpdf1 * xpdf2;

  const int NumberReplicas = LHAPDF::numberPDF();

  //  std::cout<<"DEBUG: event xPDF: "<<xpdf1<<" "<<xpdf2<< " x1: " << x1 <<  " x2: " << x2 <<  " id1: " <<id1<< " id2: " << id2 << " q: " << q <<" member PDFs: " << endl; 
  //Store nominal as well     
  for(int i=0; i <=NumberReplicas; ++i){
    LHAPDF::usePDFMember(1,i);
    double xpdf1_new = LHAPDF::xfx(1, x1, q, id1);
    double xpdf2_new = LHAPDF::xfx(1, x2, q, id2);  //central weight is 1.0    

    //    std::cout<<xpdf1_new <<" "<<xpdf2_new<<" ";   
    double weight = xpdf1_new * xpdf2_new / w0;
    pdf_weights.push_back(weight);
  }

  //  std::cout<<std::endl;  

  return pdf_weights;
}


DEFINE_FWK_MODULE(OSUAnalysis);
