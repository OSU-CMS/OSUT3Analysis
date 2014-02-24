#include "OSUT3Analysis/AnaTools/plugins/OSUAnalysis.h"
#include "LHAPDF/LHAPDF.h"    
using namespace std;

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
  triggerMetSFFile_ (cfg.getParameter<string> ("triggerMetSFFile")),
  trackNMissOutSFFile_ (cfg.getParameter<string> ("trackNMissOutSFFile")),
  dataPU_ (cfg.getParameter<string> ("dataPU")),
  electronSFID_ (cfg.getParameter<string> ("electronSFID")),
  electronSF_ (cfg.getParameter<string> ("electronSF")),
  muonSF_ (cfg.getParameter<string> ("muonSF")),
  triggerMetSF_ (cfg.getParameter<string> ("triggerMetSF")),
  trackNMissOutSF_ (cfg.getParameter<string> ("trackNMissOutSF")),
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
  trackSFShift_ (cfg.getParameter<string> ("trackSFShift")),
  printEventInfo_      (cfg.getParameter<bool> ("printEventInfo")),
  printAllTriggers_    (cfg.getParameter<bool> ("printAllTriggers")),
  useTrackCaloRhoCorr_ (cfg.getParameter<bool> ("useTrackCaloRhoCorr")),
  stopCTau_ (cfg.getParameter<vector<double> > ("stopCTau")),
  GetPlotsAfterEachCut_ (cfg.getParameter<bool> ("GetPlotsAfterEachCut")),
  verbose_ (cfg.getParameter<int> ("verbose"))
 {

  if (verbose_) printEventInfo_ = true;
  if (verbose_) clog << "Beginning OSUAnalysis::OSUAnalysis constructor." << endl;

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
  }
  if (datasetType_ == "signalMC" && regex_match (dataset_, regex ("stop.*to.*_.*mm.*")))
    stopCTauWeight_ = new StopCTauWeight (stopCTau_.at(0), stopCTau_.at(1), stops_);


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
    if(tempInputCollection == "mcparticle-track pairs")   tempInputCollection = "track-mcparticle pairs";
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
      if(tempInputCollection == "mcparticle-track pairs")   tempInputCollection = "track-mcparticle pairs";
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
        else if(currentObject == "track-mcparticle pairs")    currentObject = "trackMCparticlePairs";


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
  }

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
  if (!applyTriggerSF_ || datasetType_ != "data") triggerScaleFactor_ = 1.0; //reset the variable to 1 if we're not applying it, so it will take that value in its histogram
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
  if (datasetType_ == "signalMC" && regex_match (dataset_, regex ("stop.*to.*_.*mm.*")))
    stopCTauScaleFactor_ = stopCTauWeight_->at (event);
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
        else if(currentObject == "jet-jet pairs")           setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,jets.product(),jets.product(), "jet-jet pairs");
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
        else if(currentObject == "track-jet pairs")         setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,tracks.product(),jets.product(), "track-jet pairs");
        else if(currentObject == "muon-photon pairs")       setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),photons.product(), "muon-photon pairs");
        else if(currentObject == "track-event pairs")       setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,tracks.product(),events.product(), "track-event pairs");
        else if(currentObject == "electron-track pairs")    setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),tracks.product(),"electron-track pairs");
        else if(currentObject == "muon-track pairs")        setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),tracks.product(),"muon-track pairs");
        else if(currentObject == "secondary muon-track pairs") setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,secMuons.product(),tracks.product(),"secondary muon-track pairs");
        else if(currentObject == "muon-tau pairs")          setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),taus.product(),"muon-tau pairs");
        else if(currentObject == "tau-tau pairs")           setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,taus.product(),taus.product(),"tau-tau pairs");
        else if(currentObject == "tau-track pairs")         setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,taus .product(),tracks.product(),"tau-track pairs");
        else if(currentObject == "electron-trigobj pairs")  setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),trigobjs.product(),"electron-trigobj pairs");
        else if(currentObject == "muon-trigobj pairs")      setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,muons.product(),trigobjs.product(),"muon-trigobj pairs");
        else if(currentObject == "electron-mcparticle pairs") setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,electrons.product(),mcparticles.product(),"electron-mcparticle pairs");
        else if(currentObject == "track-mcparticle pairs")  setObjectFlags(currentCut,currentCutIndex,individualFlags,cumulativeFlags,tracks.product(),mcparticles.product(),"track-mcparticle pairs");

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
	  //	  cout << " after applying trackSFWeight" << muonTrackScaleFactor_ << "and d0 of muon =" << muons->at(muonIndex).correctedD0 << endl;
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
        
    channelScaleFactor_ *= muonScaleFactor_;
    channelScaleFactor_ *= electronScaleFactor_;
    channelScaleFactor_ *= muonTrackScaleFactor_;
    channelScaleFactor_ *= electronTrackScaleFactor_;
    channelScaleFactor_ *= bTagScaleFactor_;
    channelScaleFactor_ *= triggerMetScaleFactor_;
    channelScaleFactor_ *= trackNMissOutScaleFactor_;


    //calculate the total scale factor for the event and fill the cutflow for each channel
    eventScaleFactor_ = globalScaleFactor_ * channelScaleFactor_;

    cutFlows_.at(currentChannelIndex)->fillCutFlow(globalScaleFactor_, channelScaleFactor_);

    if (verbose_>1) clog << " Scale factors applied:  "
                         << " muonScaleFactor_ = " << muonScaleFactor_
                         << ", electronScaleFactor_ = " << electronScaleFactor_
                         << ", bTagScaleFactor_ = " << bTagScaleFactor_
                         << ", triggerMetScaleFactor_ = " << triggerMetScaleFactor_
                         << ", trackNMissOutScaleFactor_ = " << trackNMissOutScaleFactor_
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

          if (cumulativeFlags.count(currentHistogram.inputCollection) == 0 && currentHistogram.name.find ("status3") == string::npos)
            clog << "Error: no flags found for collection:  " << currentHistogram.inputCollection << ", will cause a seg fault" << endl;

          if (verbose_>1) clog << " Filling histogram " << currentHistogram.name << " for collection " << currentHistogram.inputCollection << endl;

          if(currentHistogram.inputVariables.size() == 1){
            TH1D* histo;
            histo = oneDHists_.at(currentChannelIndex).at(currentCut).at(currentHistogram.name);
            if     (currentHistogram.name.find ("status3") != string::npos) fill1DStatus3Histogram(histo,mcparticles.product(),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "jets")            fill1DHistogram(histo,currentHistogram,jets.product(),cumulativeFlags.at("jets").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary jets")  fill1DHistogram(histo,currentHistogram,jets.product(),cumulativeFlags.at("secondary jets").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary photons")  fill1DHistogram(histo,currentHistogram,photons.product(),cumulativeFlags.at("secondary photons").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muons")           fill1DHistogram(histo,currentHistogram,muons.product(),cumulativeFlags.at("muons").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary muons") fill1DHistogram(histo,currentHistogram,secMuons.product(),cumulativeFlags.at("secondary muons").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary electrons") fill1DHistogram(histo,currentHistogram,electrons.product(),cumulativeFlags.at("secondary electrons").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-muon pairs") fill1DHistogram(histo,currentHistogram,muons.product(),muons.product(),
                                                                                           cumulativeFlags.at("muon-muon pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-secondary muon pairs") fill1DHistogram(histo,currentHistogram,muons.product(),secMuons.product(),
                                                                                                     cumulativeFlags.at("muon-secondary muon pairs").at(currentDir),eventScaleFactor_);
             else if(currentHistogram.inputCollection == "muon-secondary photon pairs") fill1DHistogram(histo,currentHistogram,muons.product(),photons.product(),
                                                                                                     cumulativeFlags.at("muon-secondary photon pairs").at(currentDir),eventScaleFactor_);
             else if(currentHistogram.inputCollection == "muon-secondary jet pairs") fill1DHistogram(histo,currentHistogram,muons.product(),jets.product(),
                                                                                                     cumulativeFlags.at("muon-secondary jet pairs").at(currentDir),eventScaleFactor_);
             else if(currentHistogram.inputCollection == "photon-secondary jet pairs") fill1DHistogram(histo,currentHistogram,photons.product(),jets.product(),
                                                                                                     cumulativeFlags.at("photon-secondary jet pairs").at(currentDir),eventScaleFactor_);
             else if(currentHistogram.inputCollection == "electron-secondary jet pairs") fill1DHistogram(histo,currentHistogram,electrons.product(),jets.product(),
                                                                                                     cumulativeFlags.at("electron-secondary jet pairs").at(currentDir),eventScaleFactor_);

            else if(currentHistogram.inputCollection == "electrons") fill1DHistogram(histo,currentHistogram,electrons.product(),cumulativeFlags.at("electrons").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-electron pairs") fill1DHistogram(histo,currentHistogram,electrons.product(),electrons.product(),
                                                                                                   cumulativeFlags.at("electron-electron pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "jet-jet pairs") fill1DHistogram(histo,currentHistogram,jets.product(),jets.product(),
                                                                                         cumulativeFlags.at("jet-jet pairs").at(currentDir),eventScaleFactor_);
             else if(currentHistogram.inputCollection == "jet-secondary jet pairs") fill1DHistogram(histo,currentHistogram,jets.product(),jets.product(),
                                                                                                     cumulativeFlags.at("jet-secondary jet pairs").at(currentDir),eventScaleFactor_);

            else if(currentHistogram.inputCollection == "electron-secondary electron pairs") fill1DHistogram(histo,currentHistogram,electrons.product(),electrons.product(),
                                                                                                             cumulativeFlags.at("electron-secondary electron pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-muon pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),muons.product(),
                                                                                               cumulativeFlags.at("electron-muon pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-jet pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),jets.product(),
                                                                                              cumulativeFlags.at("electron-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary electron-jet pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),jets.product(),
                                                                                              cumulativeFlags.at("secondary electron-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary muon-jet pairs") fill1DHistogram(histo,currentHistogram, muons.product(),jets.product(),
                                                                                              cumulativeFlags.at("secondary muon-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "photon-jet pairs") fill1DHistogram(histo,currentHistogram, photons.product(),jets.product(),
                                                                                              cumulativeFlags.at("photon-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-jet pairs") fill1DHistogram(histo,currentHistogram, muons.product(),jets.product(),
                                                                                          cumulativeFlags.at("muon-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-event pairs") fill1DHistogram(histo,currentHistogram, muons.product(),events.product(),
                                                                                          cumulativeFlags.at("muon-event pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-event pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),events.product(),
                                                                                          cumulativeFlags.at("electron-event pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "met-jet pairs")  fill1DHistogram(histo,currentHistogram, mets.product(),jets.product(),
                                                                                          cumulativeFlags.at("met-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "track-jet pairs")  fill1DHistogram(histo,currentHistogram,tracks.product(),jets.product(),
                                                                                          cumulativeFlags.at("track-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-photon pairs") fill1DHistogram(histo,currentHistogram, muons.product(),photons.product(),
                                                                                          cumulativeFlags.at("muon-photon pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-photon pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),photons.product(),
                                                                                          cumulativeFlags.at("electron-photon pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-track pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),tracks.product(),
                                                                                                cumulativeFlags.at("electron-track pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-track pairs") fill1DHistogram(histo,currentHistogram, muons.product(),tracks.product(),
                                                                                            cumulativeFlags.at("muon-track pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary muon-track pairs") fill1DHistogram(histo,currentHistogram, secMuons.product(),tracks.product(),
                                                                                            cumulativeFlags.at("secondary muon-track pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-tau pairs") fill1DHistogram(histo,currentHistogram, muons.product(),taus.product(),
                                                                                          cumulativeFlags.at("muon-tau pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "tau-tau pairs") fill1DHistogram(histo,currentHistogram, taus.product(),taus.product(),
                                                                                         cumulativeFlags.at("tau-tau pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "tau-track pairs") fill1DHistogram(histo,currentHistogram, taus.product(),tracks.product(),
                                                                                           cumulativeFlags.at("tau-track pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-trigobj pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),trigobjs.product(),
                                                                                                  cumulativeFlags.at("electron-trigobj pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-trigobj pairs") fill1DHistogram(histo,currentHistogram, muons.product(),trigobjs.product(),
                                                                                              cumulativeFlags.at("muon-trigobj pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-mcparticle pairs") fill1DHistogram(histo,currentHistogram, electrons.product(),mcparticles.product(),
                                                                                              cumulativeFlags.at("electron-mcparticle pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "track-mcparticle pairs") fill1DHistogram(histo,currentHistogram, tracks.product(),mcparticles.product(),
                                                                                              cumulativeFlags.at("track-mcparticle pairs").at(currentDir),eventScaleFactor_);
            // fill the histograms of weighting factors with 1, to see the shape of a SF without any weight applied
            else if(currentHistogram.inputCollection == "events" && currentHistogram.name.find("ScaleFactor")!=string::npos) fill1DHistogram(histo,currentHistogram,events.product(),cumulativeFlags.at("events").at(currentDir),1.0);
            else if(currentHistogram.inputCollection == "events") fill1DHistogram(histo,currentHistogram,events.product(),cumulativeFlags.at("events").at(currentDir),eventScaleFactor_);

            else if(currentHistogram.inputCollection == "taus") fill1DHistogram(histo,currentHistogram,taus.product(),cumulativeFlags.at("taus").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "mets") fill1DHistogram(histo,currentHistogram,mets.product(),cumulativeFlags.at("mets").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "tracks") fill1DHistogram(histo,currentHistogram,tracks.product(),cumulativeFlags.at("tracks").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "genjets") fill1DHistogram(histo,currentHistogram,genjets.product(),cumulativeFlags.at("genjets").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "mcparticles") fill1DHistogram(histo,currentHistogram,mcparticles.product(),cumulativeFlags.at("mcparticles").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary mcparticles") fill1DHistogram(histo,currentHistogram,mcparticles.product(),cumulativeFlags.at("secondary mcparticles").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "primaryvertexs") fill1DHistogram(histo,currentHistogram,primaryvertexs.product(),cumulativeFlags.at("primaryvertexs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "bxlumis") fill1DHistogram(histo,currentHistogram,bxlumis.product(),cumulativeFlags.at("bxlumis").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "photons") fill1DHistogram(histo,currentHistogram,photons.product(),cumulativeFlags.at("photons").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "superclusters") fill1DHistogram(histo,currentHistogram,superclusters.product(),cumulativeFlags.at("superclusters").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "trigobjs") fill1DHistogram(histo,currentHistogram,trigobjs.product(),cumulativeFlags.at("trigobjs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "stops" && datasetType_ == "signalMC") fill1DHistogram(histo,currentHistogram,stops.product(),cumulativeFlags.at("stops").at(currentDir),eventScaleFactor_);
          }
          else if(currentHistogram.inputVariables.size() == 2){
            TH2D* histo;
            histo = twoDHists_.at(currentChannelIndex).at(currentCut).at(currentHistogram.name);
            if(currentHistogram.inputCollection == "jets")            fill2DHistogram(histo,currentHistogram,jets.product(),cumulativeFlags.at("jets").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary jets")  fill2DHistogram(histo,currentHistogram,jets.product(),cumulativeFlags.at("secondary jets").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary photons")  fill2DHistogram(histo,currentHistogram,photons.product(),cumulativeFlags.at("secondary photons").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muons")           fill2DHistogram(histo,currentHistogram,muons.product(),cumulativeFlags.at("muons").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary muons") fill2DHistogram(histo,currentHistogram,secMuons.product(),cumulativeFlags.at("secondary muons").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-muon pairs") fill2DHistogram(histo,currentHistogram,muons.product(),muons.product(),
                                                                                           cumulativeFlags.at("muon-muon pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-secondary muon pairs") fill2DHistogram(histo,currentHistogram,muons.product(),secMuons.product(),
                                                                                                     cumulativeFlags.at("muon-secondary muon pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-secondary photon pairs") fill2DHistogram(histo,currentHistogram,muons.product(),photons.product(),
                                                                                                     cumulativeFlags.at("muon-secondary photon pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-secondary jet pairs") fill2DHistogram(histo,currentHistogram,muons.product(),jets.product(),
                                                                                                     cumulativeFlags.at("muon-secondary jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-secondary jet pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),jets.product(),
                                                                                                     cumulativeFlags.at("electron-secondary jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "photon-secondary jet pairs") fill2DHistogram(histo,currentHistogram,photons.product(),jets.product(),
                                                                                                     cumulativeFlags.at("photon-secondary jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electrons") fill2DHistogram(histo,currentHistogram,electrons.product(),cumulativeFlags.at("electrons").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary electrons") fill2DHistogram(histo,currentHistogram,electrons.product(),cumulativeFlags.at("secondary electrons").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-electron pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),electrons.product(),
                                                                                                   cumulativeFlags.at("electron-electron pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "jet-jet pairs") fill2DHistogram(histo,currentHistogram,jets.product(),jets.product(),
                                                                                         cumulativeFlags.at("jet-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-secondary electron pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),electrons.product(),
                                                                                                             cumulativeFlags.at("electron-secondary electron pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-muon pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),muons.product(),
                                                                                               cumulativeFlags.at("electron-muon pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-jet pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),jets.product(),
                                                                                              cumulativeFlags.at("electron-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary electron-jet pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),jets.product(),
                                                                                              cumulativeFlags.at("secondary electron-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary muon-jet pairs") fill2DHistogram(histo,currentHistogram,muons.product(),jets.product(),
                                                                                              cumulativeFlags.at("secondary muon-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-photon pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),photons.product(),
                                                                                              cumulativeFlags.at("electron-photon pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-jet pairs") fill2DHistogram(histo,currentHistogram,muons.product(),jets.product(),
                                                                                          cumulativeFlags.at("muon-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-event pairs") fill2DHistogram(histo,currentHistogram,muons.product(),events.product(),
                                                                                          cumulativeFlags.at("muon-event pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-event pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),events.product(),
                                                                                          cumulativeFlags.at("electron-event pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "met-jet pairs") fill2DHistogram(histo,currentHistogram,mets.product(),jets.product(),
                                                                                         cumulativeFlags.at("met-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "track-jet pairs") fill2DHistogram(histo,currentHistogram,tracks.product(),jets.product(),
                                                                                         cumulativeFlags.at("track-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "photon-jet pairs") fill2DHistogram(histo,currentHistogram,photons.product(),jets.product(),
                                                                                          cumulativeFlags.at("photon-jet pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-photon pairs") fill2DHistogram(histo,currentHistogram,muons.product(),photons.product(),
                                                                                          cumulativeFlags.at("muon-photon pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-track pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),tracks.product(),
                                                                                                cumulativeFlags.at("electron-track pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-track pairs") fill2DHistogram(histo,currentHistogram,muons.product(),tracks.product(),
                                                                                            cumulativeFlags.at("muon-track pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary muon-track pairs") fill2DHistogram(histo,currentHistogram,secMuons.product(),tracks.product(),
                                                                                            cumulativeFlags.at("muon-track pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-tau pairs") fill2DHistogram(histo,currentHistogram,muons.product(),taus.product(),
                                                                                          cumulativeFlags.at("muon-tau pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "tau-tau pairs") fill2DHistogram(histo,currentHistogram,taus.product(),taus.product(),
                                                                                         cumulativeFlags.at("tau-tau pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "tau-track pairs") fill2DHistogram(histo,currentHistogram,taus.product(),tracks.product(),
                                                                                           cumulativeFlags.at("tau-track pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-trigobj pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),trigobjs.product(),
                                                                                                  cumulativeFlags.at("electron-trigobj pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "muon-trigobj pairs") fill2DHistogram(histo,currentHistogram,muons.product(),trigobjs.product(),
                                                                                              cumulativeFlags.at("muon-trigobj pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "electron-mcparticle pairs") fill2DHistogram(histo,currentHistogram,electrons.product(),mcparticles.product(),
                                                                                              cumulativeFlags.at("electron-mcparticle pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "track-mcparticle pairs") fill2DHistogram(histo,currentHistogram,tracks.product(),mcparticles.product(),
                                                                                              cumulativeFlags.at("track-mcparticle pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "events") fill2DHistogram(histo,currentHistogram,events.product(),cumulativeFlags.at("events").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "taus") fill2DHistogram(histo,currentHistogram,taus.product(),cumulativeFlags.at("taus").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "mets") fill2DHistogram(histo,currentHistogram,mets.product(),cumulativeFlags.at("mets").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "tracks") fill2DHistogram(histo,currentHistogram,tracks.product(),cumulativeFlags.at("tracks").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "track-event pairs") fill2DHistogram(histo,currentHistogram,tracks.product(),events.product(),
                                                                                             cumulativeFlags.at("track-event pairs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "genjets") fill2DHistogram(histo,currentHistogram,genjets.product(),cumulativeFlags.at("genjets").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "mcparticles") fill2DHistogram(histo,currentHistogram,mcparticles.product(),cumulativeFlags.at("mcparticles").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "secondary mcparticles") fill2DHistogram(histo,currentHistogram,mcparticles.product(),cumulativeFlags.at("secondary mcparticles").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "primaryvertexs") fill2DHistogram(histo,currentHistogram,primaryvertexs.product(),cumulativeFlags.at("primaryvertexs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "bxlumis") fill2DHistogram(histo,currentHistogram,bxlumis.product(),cumulativeFlags.at("bxlumis").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "photons") fill2DHistogram(histo,currentHistogram,photons.product(),cumulativeFlags.at("photons").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "superclusters") fill2DHistogram(histo,currentHistogram,superclusters.product(),cumulativeFlags.at("superclusters").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "trigobjs") fill2DHistogram(histo,currentHistogram,trigobjs.product(),cumulativeFlags.at("trigobjs").at(currentDir),eventScaleFactor_);
            else if(currentHistogram.inputCollection == "stops" && datasetType_ == "signalMC") fill2DHistogram(histo,currentHistogram,stops.product(),cumulativeFlags.at("stops").at(currentDir),eventScaleFactor_);
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
          else if(currentObject == "track-jet pairs")                    objectToPlot = "trackJetPairs";
          else if(currentObject == "jet-jet pairs")                      objectToPlot = "dijetPairs";
          else if(currentObject == "jet-secondary jet pairs")            objectToPlot = "jetSecondaryJetPairs";
          else if(currentObject == "secondary jets")                     objectToPlot = "secondaryJets";
          else if(currentObject == "secondary photons")                  objectToPlot = "secondaryPhotons";
          else if(currentObject == "electron-track pairs")               objectToPlot = "electronTrackPairs";
          else if(currentObject == "muon-track pairs")                   objectToPlot = "muonTrackPairs";
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
          else if(currentObject == "track-mcparticle pairs")             objectToPlot = "trackMCparticlePairs";
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
        else if(coll == "sedconary mcparticles")assignTreeBranch(brSpecs,mcparticles.product(),   cumulativeFlags.at(coll).back());
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

  masterCutFlow_->fillCutFlow();

  event.put (channelDecisions, "channelDecisions");

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


//!jet valueLookup
double
OSUAnalysis::valueLookup (const BNjet* object, string variable, string function, string &stringValue){

  double value = 0.0;
  if(variable == "energy") value = object->energy;
  else if(variable == "et") value = object->et;
  else if(variable == "pt") value = object->pt;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "phi") value = object->phi;
  else if(variable == "eta") value = object->eta;
  else if(variable == "theta") value = object->theta;
  else if(variable == "Upt") value = object->Upt;
  else if(variable == "Uenergy") value = object->Uenergy;
  else if(variable == "L2pt") value = object->L2pt;
  else if(variable == "L2L3pt") value = object->L2L3pt;
  else if(variable == "L2L3respt") value = object->L2L3respt;
  else if(variable == "respt") value = object->respt;
  else if(variable == "EMfrac") value = object->EMfrac;
  else if(variable == "Hadfrac") value = object->Hadfrac;
  else if(variable == "charge") value = object->charge;
  else if(variable == "mass") value = object->mass;
  else if(variable == "area") value = object->area;
  else if(variable == "fHPD") value = object->fHPD;
  else if(variable == "approximatefHPD") value = object->approximatefHPD;
  else if(variable == "genPartonET") value = object->genPartonET;
  else if(variable == "genPartonPT") value = object->genPartonPT;
  else if(variable == "genPartonEta") value = object->genPartonEta;
  else if(variable == "genPartonPhi") value = object->genPartonPhi;
  else if(variable == "genJetET") value = object->genJetET;
  else if(variable == "genJetPT") value = object->genJetPT;
  else if(variable == "genJetEta") value = object->genJetEta;
  else if(variable == "genJetPhi") value = object->genJetPhi;
  else if(variable == "btagTChighPur") value = object->btagTChighPur;
  else if(variable == "btagTChighEff") value = object->btagTChighEff;
  else if(variable == "btagJetProb") value = object->btagJetProb;
  else if(variable == "btagJetBProb") value = object->btagJetBProb;
  else if(variable == "btagSoftEle") value = object->btagSoftEle;
  else if(variable == "btagSoftMuon") value = object->btagSoftMuon;
  else if(variable == "btagSoftMuonNoIP") value = object->btagSoftMuonNoIP;
  else if(variable == "btagSecVertex") value = object->btagSecVertex;
  else if(variable == "btagSecVertexHighEff") value = object->btagSecVertexHighEff;
  else if(variable == "btagSecVertexHighPur") value = object->btagSecVertexHighPur;
  else if(variable == "btagCombinedSecVertex") value = object->btagCombinedSecVertex;
  else if(variable == "btagCombinedSecVertexMVA") value = object->btagCombinedSecVertexMVA;
  else if(variable == "btagSoftMuonByPt") value = object->btagSoftMuonByPt;
  else if(variable == "btagSoftMuonByIP3") value = object->btagSoftMuonByIP3;
  else if(variable == "btagSoftElectronByPt") value = object->btagSoftElectronByPt;
  else if(variable == "btagSoftElectronByIP3") value = object->btagSoftElectronByIP3;
  else if(variable == "n90Hits") value = object->n90Hits;
  else if(variable == "hitsInN90") value = object->hitsInN90;
  else if(variable == "chargedHadronEnergyFraction") value = object->chargedHadronEnergyFraction;
  else if(variable == "neutralHadronEnergyFraction") value = object->neutralHadronEnergyFraction;
  else if(variable == "chargedEmEnergyFraction") value = object->chargedEmEnergyFraction;
  else if(variable == "neutralEmEnergyFraction") value = object->neutralEmEnergyFraction;
  else if(variable == "chargedHadronEnergy") value = object->chargedHadronEnergyFraction*object->pt;
  else if(variable == "neutralHadronEnergy") value = object->neutralHadronEnergyFraction*object->pt;
  else if(variable == "chargedEmEnergy") value = object->chargedEmEnergyFraction*object->pt;
  else if(variable == "neutralEmEnergy") value = object->neutralEmEnergyFraction*object->pt;
  else if(variable == "fLong") value = object->fLong;
  else if(variable == "fShort") value = object->fShort;
  else if(variable == "etaetaMoment") value = object->etaetaMoment;
  else if(variable == "phiphiMoment") value = object->phiphiMoment;
  else if(variable == "JESunc") value = object->JESunc;
  else if(variable == "JECuncUp") value = object->JECuncUp;
  else if(variable == "JECuncDown") value = object->JECuncDown;
  else if(variable == "puJetMVA_full") value = object->puJetMVA_full;
  else if(variable == "puJetMVA_simple") value = object->puJetMVA_simple;
  else if(variable == "puJetMVA_cutbased") value = object->puJetMVA_cutbased;
  else if(variable == "dZ") value = object->dZ;
  else if(variable == "dR2Mean") value = object->dR2Mean;
  else if(variable == "dRMean") value = object->dRMean;
  else if(variable == "frac01") value = object->frac01;
  else if(variable == "frac02") value = object->frac02;
  else if(variable == "frac03") value = object->frac03;
  else if(variable == "frac04") value = object->frac04;
  else if(variable == "frac05") value = object->frac05;
  else if(variable == "frac06") value = object->frac06;
  else if(variable == "frac07") value = object->frac07;
  else if(variable == "beta") value = object->beta;
  else if(variable == "betaStar") value = object->betaStar;
  else if(variable == "betaClassic") value = object->betaClassic;
  else if(variable == "betaStarClassic") value = object->betaStarClassic;
  else if(variable == "ptD") value = object->ptD;
  else if(variable == "nvtx") value = object->nvtx;
  else if(variable == "d0") value = object->d0;
  else if(variable == "leadCandPt") value = object->leadCandPt;
  else if(variable == "leadCandVx") value = object->leadCandVx;
  else if(variable == "leadCandVy") value = object->leadCandVy;
  else if(variable == "leadCandVz") value = object->leadCandVz;
  else if(variable == "leadCandDistFromPV") value = object->leadCandDistFromPV;
  else if(variable == "flavour") value = object->flavour;
  else if(variable == "Nconst") value = object->Nconst;
  else if(variable == "jetIDMinimal") value = object->jetIDMinimal;
  else if(variable == "jetIDLooseAOD") value = object->jetIDLooseAOD;
  else if(variable == "jetIDLoose") value = object->jetIDLoose;
  else if(variable == "jetIDTight") value = object->jetIDTight;
  else if(variable == "genPartonId") value = object->genPartonId;
  else if(variable == "genPartonMotherId") value = object->genPartonMotherId;
  else if(variable == "genPartonMother0Id") value = object->genPartonMother0Id;
  else if(variable == "genPartonMother1Id") value = object->genPartonMother1Id;
  else if(variable == "genPartonGrandMotherId") value = object->genPartonGrandMotherId;
  else if(variable == "genPartonGrandMother00Id") value = object->genPartonGrandMother00Id;
  else if(variable == "genPartonGrandMother01Id") value = object->genPartonGrandMother01Id;
  else if(variable == "genPartonGrandMother10Id") value = object->genPartonGrandMother10Id;
  else if(variable == "genPartonGrandMother11Id") value = object->genPartonGrandMother11Id;
  else if(variable == "chargedMultiplicity") value = object->chargedMultiplicity;
  else if(variable == "neutralMultiplicity") value = object->neutralMultiplicity;
  else if(variable == "nconstituents") value = object->nconstituents;
  else if(variable == "nHit") value = object->nHit;
  else if(variable == "puJetId_full") value = object->puJetId_full;
  else if(variable == "puJetId_simple") value = object->puJetId_simple;
  else if(variable == "puJetId_cutbased") value = object->puJetId_cutbased;
  else if(variable == "puJetId_tight_full") value = object->puJetId_tight_full;
  else if(variable == "puJetId_tight_simple") value = object->puJetId_tight_simple;
  else if(variable == "puJetId_tight_cutbased") value = object->puJetId_tight_cutbased;
  else if(variable == "puJetId_medium_full") value = object->puJetId_medium_full;
  else if(variable == "puJetId_medium_simple") value = object->puJetId_medium_simple;
  else if(variable == "puJetId_medium_cutbased") value = object->puJetId_medium_cutbased;
  else if(variable == "puJetId_loose_full") value = object->puJetId_loose_full;
  else if(variable == "puJetId_loose_simple") value = object->puJetId_loose_simple;
  else if(variable == "puJetId_loose_cutbased") value = object->puJetId_loose_cutbased;

  //user defined variable
  else if(variable == "disappTrkLeadingJetID") {
    value = object->pt > 110
      && fabs(object->eta) < 2.4
      && object->chargedHadronEnergyFraction > 0.2
      && object->neutralHadronEnergyFraction < 0.7
      && object->chargedEmEnergyFraction < 0.5
      && object->neutralEmEnergyFraction < 0.7;
  }

  else if(variable == "disappTrkSubLeadingJetID") {
    value = object->pt > 30
      && fabs(object->eta) < 4.5
      && object->neutralHadronEnergyFraction < 0.7
      && object->chargedEmEnergyFraction < 0.5;
  }

  else if(variable == "dPhiMet") {
    if (const BNmet *met = chosenMET ()) {
      value = deltaPhi (object->phi, met->phi);
    } else value = -999;
  }

  else if(variable == "metPt") {  // allow making 2D plots of jet quantities vs. Met  
    if (const BNmet *met = chosenMET ()) {
      value = met->pt;  
    } else value = -999;
  }

  else if(variable == "isLeadingPtJet") {
    double ptMax = -99;  
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      string empty = "";  
      double jetPt = valueLookup(&jets->at(ijet), "pt", "", empty); 
      if (jetPt > ptMax) ptMax = jetPt;  
    }
    if (object->pt < ptMax) value = 0; 
    else                    value = 1;  
  }  

  else{clog << "WARNING: invalid jet variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end jet valueLookup


//!muon valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object, string variable, string function, string &stringValue){

  double value = 0.0;
  if(variable == "energy") value = object->energy;
  else if(variable == "et") value = object->et;
  else if(variable == "pt") value = object->pt;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "phi") value = object->phi;
  else if(variable == "eta") value = object->eta;
  else if(variable == "theta") value = object->theta;
  else if(variable == "trackIso") value = object->trackIso;
  else if(variable == "ecalIso") value = object->ecalIso;
  else if(variable == "hcalIso") value = object->hcalIso;
  else if(variable == "caloIso") value = object->caloIso;
  else if(variable == "trackIsDR03") value = object->trackIsoDR03;
  else if(variable == "ecalIsoDR03") value = object->ecalIsoDR03;
  else if(variable == "hcalIsoDR03") value = object->hcalIsoDR03;
  else if(variable == "caloIsoDR03") value = object->caloIsoDR03;
  else if(variable == "trackVetoIsoDR03") value = object->trackVetoIsoDR03;
  else if(variable == "ecalVetoIsoDR03") value = object->ecalVetoIsoDR03;
  else if(variable == "hcalVetoIsoDR03") value = object->hcalVetoIsoDR03;
  else if(variable == "caloVetoIsoDR03") value = object->caloVetoIsoDR03;
  else if(variable == "trackIsoDR05") value = object->trackIsoDR05;
  else if(variable == "ecalIsoDR05") value = object->ecalIsoDR05;
  else if(variable == "hcalIsoDR05") value = object->hcalIsoDR05;
  else if(variable == "caloIsoDR05") value = object->caloIsoDR05;
  else if(variable == "trackVetoIsoDR05") value = object->trackVetoIsoDR05;
  else if(variable == "ecalVetoIsoDR05") value = object->ecalVetoIsoDR05;
  else if(variable == "hcalVetoIsoDR05") value = object->hcalVetoIsoDR05;
  else if(variable == "caloVetoIsoDR05") value = object->caloVetoIsoDR05;
  else if(variable == "hcalE") value = object->hcalE;
  else if(variable == "ecalE") value = object->ecalE;
  else if(variable == "genET") value = object->genET;
  else if(variable == "genPT") value = object->genPT;
  else if(variable == "genPhi") value = object->genPhi;
  else if(variable == "genEta") value = object->genEta;
  else if(variable == "genMotherET") value = object->genMotherET;
  else if(variable == "genMotherPT") value = object->genMotherPT;
  else if(variable == "genMotherPhi") value = object->genMotherPhi;
  else if(variable == "genMotherEta") value = object->genMotherEta;
  else if(variable == "vx") value = object->vx;
  else if(variable == "vy") value = object->vy;
  else if(variable == "vz") value = object->vz;
  else if(variable == "tkNormChi2") value = object->tkNormChi2;
  else if(variable == "tkPT") value = object->tkPT;
  else if(variable == "tkEta") value = object->tkEta;
  else if(variable == "tkPhi") value = object->tkPhi;
  else if(variable == "tkDZ") value = object->tkDZ;
  else if(variable == "tkD0") value = object->tkD0;
  else if(variable == "tkD0bs") value = object->tkD0bs;
  else if(variable == "tkD0err") value = object->tkD0err;
  else if(variable == "samNormChi2") value = object->samNormChi2;
  else if(variable == "samPT") value = object->samPT;
  else if(variable == "samEta") value = object->samEta;
  else if(variable == "samPhi") value = object->samPhi;
  else if(variable == "samDZ") value = object->samDZ;
  else if(variable == "samD0") value = object->samD0;
  else if(variable == "samD0bs") value = object->samD0bs;
  else if(variable == "samD0err") value = object->samD0err;
  else if(variable == "comNormChi2") value = object->comNormChi2;
  else if(variable == "comPT") value = object->comPT;
  else if(variable == "comEta") value = object->comEta;
  else if(variable == "comPhi") value = object->comPhi;
  else if(variable == "comDZ") value = object->comDZ;
  else if(variable == "comD0") value = object->comD0;
  else if(variable == "comD0bs") value = object->comD0bs;
  else if(variable == "comD0err") value = object->comD0err;
  else if(variable == "isolationR03emVetoEt") value = object->isolationR03emVetoEt;
  else if(variable == "isolationR03hadVetoEt") value = object->isolationR03hadVetoEt;
  else if(variable == "normalizedChi2") value = object->normalizedChi2;
  else if(variable == "dVzPVz") value = object->dVzPVz;
  else if(variable == "dB") value = object->dB;
  else if(variable == "ptErr") value = object->ptErr;
  else if(variable == "innerTrackNormChi2") value = object->innerTrackNormChi2;
  else if(variable == "correctedD0") value = object->correctedD0;
  else if(variable == "correctedD0Vertex") value = object->correctedD0Vertex;
  else if(variable == "correctedDZ") value = object->correctedDZ;
  else if(variable == "particleIso") value = object->particleIso;
  else if(variable == "chargedHadronIso") value = object->chargedHadronIso;
  else if(variable == "neutralHadronIso") value = object->neutralHadronIso;
  else if(variable == "photonIso") value = object->photonIso;
  else if(variable == "puChargedHadronIso") value = object->puChargedHadronIso;
  else if(variable == "chargedHadronIsoDR03") value = object->chargedHadronIsoDR03;
  else if(variable == "neutralHadronIsoDR03") value = object->neutralHadronIsoDR03;
  else if(variable == "photonIsoDR03") value = object->photonIsoDR03;
  else if(variable == "puChargedHadronIsoDR03") value = object->puChargedHadronIsoDR03;
  else if(variable == "chargedHadronIsoDR04") value = object->chargedHadronIsoDR04;
  else if(variable == "neutralHadronIsoDR04") value = object->neutralHadronIsoDR04;
  else if(variable == "photonIsoDR04") value = object->photonIsoDR04;
  else if(variable == "puChargedHadronIsoDR04") value = object->puChargedHadronIsoDR04;
  else if(variable == "rhoPrime") value = object->rhoPrime;
  else if(variable == "AEffDr03") value = object->AEffDr03;
  else if(variable == "AEffDr04") value = object->AEffDr04;
  else if(variable == "pfIsoR03SumChargedHadronPt") value = object->pfIsoR03SumChargedHadronPt;
  else if(variable == "pfIsoR03SumNeutralHadronEt") value = object->pfIsoR03SumNeutralHadronEt;
  else if(variable == "pfIsoR03SumPhotonEt") value = object->pfIsoR03SumPhotonEt;
  else if(variable == "pfIsoR03SumPUPt") value = object->pfIsoR03SumPUPt;
  else if(variable == "relpfIsoR04SumExceptChargedHad") value = (object->pfIsoR04SumNeutralHadronEt + object->pfIsoR04SumPhotonEt - 0.5*object->pfIsoR04SumPUPt)/ object->pt;
  else if(variable == "relpfIsoR04SumChargedHadronPt") value = object->pfIsoR04SumChargedHadronPt/object->pt;
  else if(variable == "relpfIsoR04SumNeutralHadronEt") value = object->pfIsoR04SumNeutralHadronEt/object->pt;
  else if(variable == "relpfIsoR04SumPhotonEt") value = object->pfIsoR04SumPhotonEt/object->pt;
  else if(variable == "relpfIsoR04SumPUPt") value = object->pfIsoR04SumPUPt/object->pt;
  else if(variable == "pfIsoR04SumChargedHadronPt") value = object->pfIsoR04SumChargedHadronPt;
  else if(variable == "pfIsoR04SumNeutralHadronEt") value = object->pfIsoR04SumNeutralHadronEt;
  else if(variable == "pfIsoR04SumPhotonEt") value = object->pfIsoR04SumPhotonEt;
  else if(variable == "pfIsoR04SumPUPt") value = object->pfIsoR04SumPUPt;
  else if(variable == "IP") value = object->IP;
  else if(variable == "IPError") value = object->IPError;
  else if(variable == "timeAtIpInOut") value = object->timeAtIpInOut;
  else if(variable == "timeAtIpInOutErr") value = object->timeAtIpInOutErr;
  else if(variable == "timeAtIpOutIn") value = object->timeAtIpOutIn;
  else if(variable == "timeAtIpOutInErr") value = object->timeAtIpOutInErr;
  else if(variable == "ecal_time") value = object->ecal_time;
  else if(variable == "hcal_time") value = object->hcal_time;
  else if(variable == "ecal_timeError") value = object->ecal_timeError;
  else if(variable == "hcal_timeError") value = object->hcal_timeError;
  else if(variable == "energy_ecal") value = object->energy_ecal;
  else if(variable == "energy_hcal") value = object->energy_hcal;
  else if(variable == "e3x3_ecal") value = object->e3x3_ecal;
  else if(variable == "e3x3_hcal") value = object->e3x3_hcal;
  else if(variable == "energyMax_ecal") value = object->energyMax_ecal;
  else if(variable == "energyMax_hcal") value = object->energyMax_hcal;
  else if(variable == "charge") value = object->charge;
  else if(variable == "IDGMPTight") value = object->IDGMPTight;
  else if(variable == "tkNumValidHits") value = object->tkNumValidHits;
  else if(variable == "tkCharge") value = object->tkCharge;
  else if(variable == "samNumValidHits") value = object->samNumValidHits;
  else if(variable == "samCharge") value = object->samCharge;
  else if(variable == "comNumValidHits") value = object->comNumValidHits;
  else if(variable == "comCharge") value = object->comCharge;
  else if(variable == "genId") value = object->genId;
  else if(variable == "genCharge") value = object->genCharge;
  else if(variable == "genNumberOfMothers") value = object->genNumberOfMothers;
  else if(variable == "genMotherId") value = object->genMotherId;
  else if(variable == "genMotherCharge") value = object->genMotherCharge;
  else if(variable == "genMother0Id") value = object->genMother0Id;
  else if(variable == "genMother1Id") value = object->genMother1Id;
  else if(variable == "genGrandMother00Id") value = object->genGrandMother00Id;
  else if(variable == "genGrandMother01Id") value = object->genGrandMother01Id;
  else if(variable == "genGrandMother10Id") value = object->genGrandMother10Id;
  else if(variable == "genGrandMother11Id") value = object->genGrandMother11Id;
  else if(variable == "isPFMuon") value = object->isPFMuon;
  else if(variable == "isGoodMuon_1StationTight") value = object->isGoodMuon_1StationTight;
  else if(variable == "isGlobalMuon") value = object->isGlobalMuon;
  else if(variable == "isTrackerMuon") value = object->isTrackerMuon;
  else if(variable == "isStandAloneMuon") value = object->isStandAloneMuon;
  else if(variable == "isGlobalMuonPromptTight") value = object->isGlobalMuonPromptTight;
  else if(variable == "numberOfValidMuonHits") value = object->numberOfValidMuonHits;
  else if(variable == "numberOfValidTrackerHits") value = object->numberOfValidTrackerHits;
  else if(variable == "numberOfLayersWithMeasurement") value = object->numberOfLayersWithMeasurement;
  else if(variable == "pixelLayersWithMeasurement") value = object->pixelLayersWithMeasurement;
  else if(variable == "numberOfMatches") value = object->numberOfMatches;
  else if(variable == "numberOfValidTrackerHitsInnerTrack") value = object->numberOfValidTrackerHitsInnerTrack;
  else if(variable == "numberOfValidPixelHits") value = object->numberOfValidPixelHits;
  else if(variable == "numberOfMatchedStations") value = object->numberOfMatchedStations;
  else if(variable == "time_ndof") value = object->time_ndof;

  //user-defined variables
  else if(variable == "looseID") {
    value = object->pt > 10 &&
      (object->isGlobalMuon  > 0 ||
       object->isTrackerMuon > 0);
  }
  else if(variable == "looseIDGlobalMuon") {
    value = object->pt > 10 &&
      object->isGlobalMuon  > 0;
  }
  else if(variable == "vertexDistZ") value = object->vz - chosenVertex()->z;
  else if(variable == "correctedD0VertexErr") value =  hypot (object->tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
  else if(variable == "correctedD0VertexSig") value =  object->correctedD0Vertex / hypot (object->tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
  else if(variable == "correctedD0Err") value =  hypot (object->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "correctedD0Sig") value =  object->correctedD0 / hypot (object->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "detIso") value = (object->trackIsoDR03) / object->pt;
  else if(variable == "relPFdBetaIso") value = (object->pfIsoR04SumChargedHadronPt + max(0.0, object->pfIsoR04SumNeutralHadronEt + object->pfIsoR04SumPhotonEt - 0.5*object->pfIsoR04SumPUPt)) / object->pt;
  else if(variable == "PUInrelPFdBetaIso") value = (object->pfIsoR04SumPUPt)/ object->pt;
  else if(variable == "ChargedHadInrelPFdBetaIso") value = (object->pfIsoR04SumChargedHadronPt)/ object->pt;
  else if(variable == "NeutralHadInrelPFdBetaIso") value = (object->pfIsoR04SumNeutralHadronEt)/ object->pt;
  else if(variable == "PhotonInrelPFdBetaIso") value = (object->pfIsoR04SumPhotonEt)/ object->pt;
  else if(variable == "relPFdBetaIsoNoPUSubtracted") value = (object->pfIsoR04SumChargedHadronPt + max(0.0, object->pfIsoR04SumNeutralHadronEt + object->pfIsoR04SumPhotonEt)) / object->pt;
  else if(variable == "relPFdBetaIsoNew") value = (object->chargedHadronIsoDR04 + max(0.0, object->neutralHadronIsoDR04 + object->photonIsoDR04 - 0.5*object->puChargedHadronIsoDR04)) / object->pt;
  else if(variable == "relPFdBetaIsoPseudo") value = (object->pfIsoR04SumChargedHadronPt + object->pfIsoR04SumNeutralHadronEt + object->pfIsoR04SumPhotonEt - 0.5*object->pfIsoR04SumPUPt) / object->pt;
  else if(variable == "relPFrhoIso") value = ( object->chargedHadronIso + max(0.0, object->neutralHadronIso + object->photonIso - object->AEffDr03*object->rhoPrime) ) / object->pt;
  else if(variable == "metMT") {
    if (const BNmet *met = chosenMET ())
      {
        double dPhi = deltaPhi (object->phi, met->phi);
        value = sqrt (2 * object->pt * met->pt * (1 - cos (dPhi)));
      }
    else
      value = -999;
  }
  else if(variable == "metDeltaPhi") {
    if (const BNmet *met = chosenMET ())
      value = deltaPhi (object->phi, met->phi);
    else
      value = -999;
  }
  else if(variable == "ptPlusMet") {
    // Calculate the magnitude of the vector sum of the muon pT and the Met.
    if (const BNmet *met = chosenMET ())
      {
	TVector2 p2Met;
	TVector2 p2Muon;
	p2Met. SetMagPhi(   met->pt,    met->phi);
	p2Muon.SetMagPhi(object->pt, object->phi);  
	TVector2 p2MetMuon = p2Met + p2Muon;
        value = p2MetMuon.Mod();  
      }
    else
      value = -999;
  }

  else if(variable == "correctedD0VertexInEBPlus"){
    if(fabs(object->eta) < 0.8 && object->eta > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBPlus"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->eta > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEPlus"){
    if(fabs(object->eta) > 1.479 && object->eta > 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBPlus"){
    if(fabs(object->eta) < 0.8 && object->eta > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBPlus"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->eta > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEEPlus"){
    if(fabs(object->eta) > 1.479 && object->eta > 0) value = object->correctedD0;
    else value = -999;
  }

  else if(variable == "correctedD0VertexInEBMinus"){
    if(fabs(object->eta) < 0.8 && object->eta < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBMinus"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->eta < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEMinus"){
    if(fabs(object->eta) > 1.479 && object->eta < 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBMinus"){
    if(fabs(object->eta) < 0.8 && object->eta < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBMinus"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->eta < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEEMinus"){
    if(fabs(object->eta) > 1.479 && object->eta < 0) value = object->correctedD0;
    else value = -999;
  }


  else if(variable == "correctedD0VertexInEBPositiveCharge"){
    if(fabs(object->eta) < 0.8 && object->charge > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBPositiveCharge"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->charge > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEPositiveCharge"){
    if(fabs(object->eta) > 1.479 && object->charge > 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBPositiveCharge"){
    if(fabs(object->eta) < 0.8 && object->charge > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBPositiveCharge"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->charge > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEEPositiveCharge"){
    if(fabs(object->eta) > 1.479 && object->charge > 0) value = object->correctedD0;
    else value = -999;
  }

  else if(variable == "correctedD0VertexInEBNegativeCharge"){
    if(fabs(object->eta) < 0.8 && object->charge < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBNegativeCharge"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->charge < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEENegativeCharge"){
    if(fabs(object->eta) > 1.479 && object->charge < 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBNegativeCharge"){
    if(fabs(object->eta) < 0.8 && object->charge < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBNegativeCharge"){
    if(fabs(object->eta) < 1.479 && fabs(object->eta) > 0.8 && object->charge < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEENegativeCharge"){
    if(fabs(object->eta) > 1.479 && object->charge < 0) value = object->correctedD0;
    else value = -999;
  }


  else if(variable == "tightID") {
    value = object->isGlobalMuon > 0                \
      && object->isPFMuon > 0                        \
      && object->normalizedChi2 < 10                \
      && object->numberOfValidMuonHits > 0        \
      && object->numberOfMatchedStations > 1        \
      && fabs(object->correctedD0Vertex) < 0.2        \
      && fabs(object->correctedDZ) < 0.5        \
      && object->numberOfValidPixelHits > 0                \
      && object->numberOfLayersWithMeasurement > 5;
  }
  else if(variable == "tightIDdisplaced"){
    value = object->isGlobalMuon > 0                \
      && object->isPFMuon > 0                        \
      && object->normalizedChi2 < 10                \
      && object->numberOfValidMuonHits > 0        \
      && object->numberOfMatchedStations > 1        \
      && object->numberOfValidPixelHits > 0        \
      && object->numberOfLayersWithMeasurement > 5;
  }

  else if(variable == "genDeltaRLowest") value = getGenDeltaRLowest(object);

  else if(variable == "genMatchedPdgId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = mcparticles->at(index).id;
  }

  else if(variable == "genMatchedId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = getPdgIdBinValue(mcparticles->at(index).id);
  }
  else if(variable == "genMatchedMotherId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genMatchedMotherIdReverse"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 24;
    else value = 24 - getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genRecoChargeProduct"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 2;
    else value = object->charge * mcparticles->at(index).charge;
  }
  else if(variable == "genMatchedGrandmotherId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else if(fabs(mcparticles->at(index).motherId) == 15){
      int motherIndex = findTauMotherIndex(&mcparticles->at(index));
      if(motherIndex == -1) value = 0;
      else value = getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = getPdgIdBinValue(mcparticles->at(index).grandMotherId);
  }
  else if(variable == "genMatchedGrandmotherIdReverse"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 24;
    else if(fabs(mcparticles->at(index).motherId) == 15){
      int motherIndex = findTauMotherIndex(&mcparticles->at(index));
      if(motherIndex == -1) value = 24;
      else value = 24 - getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = 24 - getPdgIdBinValue(mcparticles->at(index).grandMotherId);
  }
  else if(variable == "pfMuonsFromVertex"){
    double d0Error, dzError;

    d0Error = hypot (object->tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
    dzError = hypot (object->tkDZerr, chosenVertex ()->zError);
    value = fabs (object->correctedD0Vertex) > 0.2 || fabs (object->correctedDZ) > 0.5
      || fabs (object->correctedD0Vertex / d0Error) > 99.0
      || fabs (object->correctedDZ / dzError) > 99.0;
    value = (object->isStandAloneMuon && !object->isTrackerMuon && !object->isGlobalMuon) || !value;
  }



  else{clog << "WARNING: invalid muon variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end muon valueLookup


//!electron valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object, string variable, string function, string &stringValue){

  double value = 0.0;
  if(variable == "energy") value = object->energy;
  else if(variable == "et") value = object->et;
  else if(variable == "gsfEt") value = object->gsfEt;
  else if(variable == "pt") value = object->pt;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "phi") value = object->phi;
  else if(variable == "eta") value = object->eta;
  else if(variable == "theta") value = object->theta;
  else if(variable == "pIn") value = object->pIn;
  else if(variable == "pOut") value = object->pOut;
  else if(variable == "EscOverPin") value = object->EscOverPin;
  else if(variable == "EseedOverPout") value = object->EseedOverPout;
  else if(variable == "hadOverEm") value = object->hadOverEm;
  else if(variable == "trackIso") value = object->trackIso;
  else if(variable == "ecalIso") value = object->ecalIso;
  else if(variable == "hcalIso") value = object->hcalIso;
  else if(variable == "caloIso") value = object->caloIso;
  else if(variable == "trackIsoDR03") value = object->trackIsoDR03;
  else if(variable == "ecalIsoDR03") value = object->ecalIsoDR03;
  else if(variable == "hcalIsoDR03") value = object->hcalIsoDR03;
  else if(variable == "hcalIsoDR03depth1") value = object->hcalIsoDR03depth1;
  else if(variable == "hcalIsoDR03depth2") value = object->hcalIsoDR03depth2;
  else if(variable == "caloIsoDR03") value = object->caloIsoDR03;
  else if(variable == "trackIsoDR04") value = object->trackIsoDR04;
  else if(variable == "ecalIsoDR04") value = object->ecalIsoDR04;
  else if(variable == "hcalIsoDR04") value = object->hcalIsoDR04;
  else if(variable == "hcalIsoDR04depth1") value = object->hcalIsoDR04depth1;
  else if(variable == "hcalIsoDR04depth2") value = object->hcalIsoDR04depth2;
  else if(variable == "caloIsoDR04") value = object->caloIsoDR04;
  else if(variable == "fbrem") value = object->fbrem;
  else if(variable == "absInvEMinusInvPin") value = object->absInvEMinusInvPin;
  else if(variable == "delPhiIn") value = object->delPhiIn;
  else if(variable == "delEtaIn") value = object->delEtaIn;
  else if(variable == "genET") value = object->genET;
  else if(variable == "genPT") value = object->genPT;
  else if(variable == "genPhi") value = object->genPhi;
  else if(variable == "genEta") value = object->genEta;
  else if(variable == "genMotherET") value = object->genMotherET;
  else if(variable == "genMotherPT") value = object->genMotherPT;
  else if(variable == "genMotherPhi") value = object->genMotherPhi;
  else if(variable == "genMotherEta") value = object->genMotherEta;
  else if(variable == "vx") value = object->vx;
  else if(variable == "vy") value = object->vy;
  else if(variable == "vz") value = object->vz;
  else if(variable == "scEnergy") value = object->scEnergy;
  else if(variable == "scRawEnergy") value = object->scRawEnergy;
  else if(variable == "scSigmaEtaEta") value = object->scSigmaEtaEta;
  else if(variable == "scSigmaIEtaIEta") value = object->scSigmaIEtaIEta;
  else if(variable == "scE1x5") value = object->scE1x5;
  else if(variable == "scE2x5Max") value = object->scE2x5Max;
  else if(variable == "scE5x5") value = object->scE5x5;
  else if(variable == "scEt") value = object->scEt;
  else if(variable == "scEta") value = object->scEta;
  else if(variable == "scPhi") value = object->scPhi;
  else if(variable == "scZ") value = object->scZ;
  else if(variable == "tkNormChi2") value = object->tkNormChi2;
  else if(variable == "tkPT") value = object->tkPT;
  else if(variable == "tkEta") value = object->tkEta;
  else if(variable == "tkPhi") value = object->tkPhi;
  else if(variable == "tkDZ") value = object->tkDZ;
  else if(variable == "tkD0") value = object->tkD0;
  else if(variable == "tkD0bs") value = object->tkD0bs;
  else if(variable == "tkD0err") value = object->tkD0err;
  else if(variable == "mva") value = object->mva;
  else if(variable == "mvaTrigV0") value = object->mvaTrigV0;
  else if(variable == "mvaTrigNoIPV0") value = object->mvaTrigNoIPV0;
  else if(variable == "mvaNonTrigV0") value = object->mvaNonTrigV0;
  else if(variable == "dist") value = object->dist;
  else if(variable == "dcot") value = object->dcot;
  else if(variable == "convradius") value = object->convradius;
  else if(variable == "convPointX") value = object->convPointX;
  else if(variable == "convPointY") value = object->convPointY;
  else if(variable == "convPointZ") value = object->convPointZ;
  else if(variable == "eMax") value = object->eMax;
  else if(variable == "eLeft") value = object->eLeft;
  else if(variable == "eRight") value = object->eRight;
  else if(variable == "eTop") value = object->eTop;
  else if(variable == "eBottom") value = object->eBottom;
  else if(variable == "e3x3") value = object->e3x3;
  else if(variable == "swissCross") value = object->swissCross;
  else if(variable == "seedEnergy") value = object->seedEnergy;
  else if(variable == "seedTime") value = object->seedTime;
  else if(variable == "swissCrossNoI85") value = object->swissCrossNoI85;
  else if(variable == "swissCrossI85") value = object->swissCrossI85;
  else if(variable == "E2overE9NoI85") value = object->E2overE9NoI85;
  else if(variable == "E2overE9I85") value = object->E2overE9I85;
  else if(variable == "correctedD0") value = object->correctedD0;
  else if(variable == "correctedD0Vertex") value = object->correctedD0Vertex;
  else if(variable == "correctedDZ") value = object->correctedDZ;
  else if(variable == "particleIso") value = object->particleIso;
  else if(variable == "chargedHadronIso") value = object->chargedHadronIso;
  else if(variable == "neutralHadronIso") value = object->neutralHadronIso;
  else if(variable == "photonIso") value = object->photonIso;
  else if(variable == "puChargedHadronIso") value = object->puChargedHadronIso;
  else if(variable == "chargedHadronIsoDR03") value = object->chargedHadronIsoDR03;
  else if(variable == "neutralHadronIsoDR03") value = object->neutralHadronIsoDR03;
  else if(variable == "photonIsoDR03") value = object->photonIsoDR03;
  else if(variable == "puChargedHadronIsoDR03") value = object->puChargedHadronIsoDR03;
  else if(variable == "chargedHadronIsoDR04") value = object->chargedHadronIsoDR04;
  else if(variable == "neutralHadronIsoDR04") value = object->neutralHadronIsoDR04;
  else if(variable == "photonIsoDR04") value = object->photonIsoDR04;
  else if(variable == "puChargedHadronIsoDR04") value = object->puChargedHadronIsoDR04;
  else if(variable == "rhoPrime") value = object->rhoPrime;
  else if(variable == "AEffDr03") value = object->AEffDr03;
  else if(variable == "AEffDr04") value = object->AEffDr04;
  else if(variable == "IP") value = object->IP;
  else if(variable == "IPError") value = object->IPError;
  else if(variable == "charge") value = object->charge;
  else if(variable == "classification") value = object->classification;
  else if(variable == "genId") value = object->genId;
  else if(variable == "genCharge") value = object->genCharge;
  else if(variable == "genNumberOfMothers") value = object->genNumberOfMothers;
  else if(variable == "genMotherId") value = object->genMotherId;
  else if(variable == "genMotherCharge") value = object->genMotherCharge;
  else if(variable == "genMother0Id") value = object->genMother0Id;
  else if(variable == "genMother1Id") value = object->genMother1Id;
  else if(variable == "genGrandMother00Id") value = object->genGrandMother00Id;
  else if(variable == "genGrandMother01Id") value = object->genGrandMother01Id;
  else if(variable == "genGrandMother10Id") value = object->genGrandMother10Id;
  else if(variable == "genGrandMother11Id") value = object->genGrandMother11Id;
  else if(variable == "numClusters") value = object->numClusters;
  else if(variable == "tkNumValidHits") value = object->tkNumValidHits;
  else if(variable == "tkCharge") value = object->tkCharge;
  else if(variable == "gsfCharge") value = object->gsfCharge;
  else if(variable == "isEB") value = object->isEB;
  else if(variable == "isEE") value = object->isEE;
  else if(variable == "isGap") value = object->isGap;
  else if(variable == "isEBEEGap") value = object->isEBEEGap;
  else if(variable == "isEBGap") value = object->isEBGap;
  else if(variable == "isEEGap") value = object->isEEGap;
  else if(variable == "isEcalDriven") value = object->isEcalDriven;
  else if(variable == "isTrackerDriven") value = object->isTrackerDriven;
  else if(variable == "numberOfLostHits") value = object->numberOfLostHits;
  else if(variable == "numberOfExpectedInnerHits") value = object->numberOfExpectedInnerHits;
  else if(variable == "numberOfValidPixelHits") value = object->numberOfValidPixelHits;
  else if(variable == "numberOfValidPixelBarrelHits") value = object->numberOfValidPixelBarrelHits;
  else if(variable == "numberOfValidPixelEndcapHits") value = object->numberOfValidPixelEndcapHits;
  else if(variable == "isHEEP") value = object->isHEEP;
  else if(variable == "isHEEPnoEt") value = object->isHEEPnoEt;
  else if(variable == "seedRecoFlag") value = object->seedRecoFlag;
  else if(variable == "eidRobustHighEnergy") value = object->eidRobustHighEnergy;
  else if(variable == "eidRobustLoose") value = object->eidRobustLoose;
  else if(variable == "eidRobustTight") value = object->eidRobustTight;
  else if(variable == "eidLoose") value = object->eidLoose;
  else if(variable == "eidTight") value = object->eidTight;
  else if(variable == "eidVeryLooseMC") value = object->eidVeryLooseMC;
  else if(variable == "eidLooseMC") value = object->eidLooseMC;
  else if(variable == "eidMediumMC") value = object->eidMediumMC;
  else if(variable == "eidTightMC") value = object->eidTightMC;
  else if(variable == "eidSuperTightMC") value = object->eidSuperTightMC;
  else if(variable == "eidHyperTight1MC") value = object->eidHyperTight1MC;
  else if(variable == "eidHyperTight2MC") value = object->eidHyperTight2MC;
  else if(variable == "eidHyperTight3MC") value = object->eidHyperTight3MC;
  else if(variable == "eidHyperTight4MC") value = object->eidHyperTight4MC;
  else if(variable == "passConvVeto") value = object->passConvVeto;

  //user-defined variables
  else if(variable == "vertexDistZ") value = object->vz - chosenVertex()->z;
  else if(variable == "correctedD0VertexErr") value =  hypot (object->tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
  else if(variable == "correctedD0VertexSig") value =  object->correctedD0Vertex / hypot (object->tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
  else if(variable == "correctedD0Err") value =  hypot (object->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "correctedD0Sig") value =  object->correctedD0 / hypot (object->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "detIso") value = (object->trackIso) / object->pt;
  else if(variable == "relPFrhoIso") value = ( object->chargedHadronIsoDR03 + max(0.0, object->neutralHadronIsoDR03 + object->photonIsoDR03 - object->AEffDr03*object->rhoPrime) ) / object->pt;
  else if(variable == "relPFrhoIsoNoPUSubtracted") value = ( object->chargedHadronIsoDR03 + max(0.0, object->neutralHadronIsoDR03 + object->photonIsoDR03) ) / object->pt;
  else if(variable == "PUInrelPFrhoIso") value = (object->AEffDr03*object->rhoPrime)/ object->pt;
  else if(variable == "ChargedHadInrelPFrhoIso") value = (object->chargedHadronIsoDR03)/ object->pt;
  else if(variable == "NeutralHadInrelPFrhoIso") value = (object->neutralHadronIsoDR03)/ object->pt;
  else if(variable == "PhotonInrelPFrhoIso") value = (object->photonIsoDR03)/ object->pt;
  else if(variable == "relPFrhoIsoEB") value = object->isEB ? ( object->chargedHadronIsoDR03 + max(0.0, object->neutralHadronIsoDR03 + object->photonIsoDR03 - object->AEffDr03*object->rhoPrime) ) / object->pt : -999;
  else if(variable == "relPFrhoIsoEE") value = object->isEE ? ( object->chargedHadronIsoDR03 + max(0.0, object->neutralHadronIsoDR03 + object->photonIsoDR03 - object->AEffDr03*object->rhoPrime) ) / object->pt : -999;
  else if(variable == "metMT") {
    if (const BNmet *met = chosenMET ())
      {
        double dPhi = deltaPhi (object->phi, met->phi);
        value = sqrt (2 * object->pt * met->pt * (1 - cos (dPhi)));
      }
    else
      value = -999;
  }
  else if(variable == "metDeltaPhi") {
    if (const BNmet *met = chosenMET ())
      value = deltaPhi (object->phi, met->phi);
    else
      value = -999;
  }
  else if(variable == "ptPlusMet") {
    // Calculate the magnitude of the vector sum of the electron pT and the Met.
    if (const BNmet *met = chosenMET ())
      {
        TVector2 p2Met;
        TVector2 p2Elec;
        p2Met. SetMagPhi(   met->pt,    met->phi);
        p2Elec.SetMagPhi(object->pt, object->phi);
        TVector2 p2MetElec = p2Met + p2Elec;
        value = p2MetElec.Mod();
      }
    else
      value = -999;
  }

  else if(variable == "correctedD0VertexEEPositiveChargeLowPt"){
    if(fabs(object->eta) > 1.479 && object->charge > 0 && object->pt > 45) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEPositiveChargeHighPt"){
    if(fabs(object->eta) > 1.479 && object->charge > 0 && object->pt < 45) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0VertexInEBPlus"){
    if(fabs(object->eta) < 0.8 && object->eta > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBPlus"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->eta > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEPlus"){
    if(object->isEE && object->eta > 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBPlus"){
    if(fabs(object->eta) < 0.8 && object->eta > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBPlus"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->eta > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEEPlus"){
    if(object->isEE && object->eta > 0) value = object->correctedD0;
    else value = -999;
  }

  else if(variable == "correctedD0VertexInEBMinus"){
    if(fabs(object->eta) < 0.8 && object->eta < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBMinus"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->eta < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEMinus"){
    if(object->isEE && object->eta < 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBMinus"){
    if(fabs(object->eta) < 0.8 && object->eta < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBMinus"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->eta < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEEMinus"){
    if(object->isEE && object->eta < 0) value = object->correctedD0;
    else value = -999;
  }

  else if(variable == "looseID"){
    if (object->isEB)
      {
        value = fabs(object->delEtaIn) < 0.007 \
          && fabs (object->delPhiIn) < 0.15 \
          && object->scSigmaIEtaIEta < 0.01 \
          && object->hadOverEm < 0.12 \
          && fabs (object->correctedD0Vertex) < 0.02 \
          && fabs (object->correctedDZ) < 0.2 \
          && object->absInvEMinusInvPin < 0.05 \
          && object->passConvVeto;
      }
    else
      {
        value = fabs(object->delEtaIn) < 0.009 \
          && fabs (object->delPhiIn) < 0.10 \
          && object->scSigmaIEtaIEta < 0.03 \
          && object->hadOverEm < 0.10 \
          && fabs (object->correctedD0Vertex) < 0.02 \
          && fabs (object->correctedDZ) < 0.2 \
          && object->absInvEMinusInvPin < 0.05 \
          && object->passConvVeto;
      }
  }

  else if(variable == "tightID"){
    if (object->isEB)
      {
        value = fabs(object->delEtaIn) < 0.004 \
          && fabs (object->delPhiIn) < 0.03 \
          && object->scSigmaIEtaIEta < 0.01 \
          && object->hadOverEm < 0.12 \
          && fabs (object->correctedD0Vertex) < 0.02 \
          && fabs (object->correctedDZ) < 0.1 \
          && object->absInvEMinusInvPin < 0.05 \
          && object->passConvVeto;
      }
    else
      {
        value = fabs(object->delEtaIn) < 0.005 \
          && fabs (object->delPhiIn) < 0.02 \
          && object->scSigmaIEtaIEta < 0.03 \
          && object->hadOverEm < 0.10 \
          && fabs (object->correctedD0Vertex) < 0.02 \
          && fabs (object->correctedDZ) < 0.1 \
          && object->absInvEMinusInvPin < 0.05 \
          && object->passConvVeto;
      }
  }

  else if(variable == "mvaNonTrig_HtoZZto4l"){ // https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentification#Non_triggering_MVA
    value = false;
    if (object->pt > 7.0 && object->pt < 10.0)
      {
        if (abs (object->scEta) < 0.8 && object->mvaNonTrigV0 > 0.47)
          value = true;
        else if (abs (object->scEta) < 1.479 && object->mvaNonTrigV0 > 0.004)
          value = true;
        else if (abs (object->scEta) < 2.5 && object->mvaNonTrigV0 > 0.295)
          value = true;
      }
    else if (object->pt >= 10.0)
      {
        if (abs (object->scEta) < 0.8 && object->mvaNonTrigV0 > -0.34)
          value = true;
        else if (abs (object->scEta) < 1.479 && object->mvaNonTrigV0 > -0.65)
          value = true;
        else if (abs (object->scEta) < 2.5 && object->mvaNonTrigV0 > 0.60)
          value = true;
      }
  }
  else if(variable == "mvaTrig_HtoWWto2l2nu"){ // https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentification#Triggering_MVA
    value = false;
    if (object->pt > 10.0 && object->pt < 20.0)
      {
        if (abs (object->scEta) < 0.8 && object->mvaTrigV0 > 0.00)
          value = true;
        else if (abs (object->scEta) < 1.479 && object->mvaTrigV0 > 0.10)
          value = true;
        else if (abs (object->scEta) < 2.5 && object->mvaTrigV0 > 0.62)
          value = true;
      }
    else if (object->pt >= 20.0)
      {
        if (abs (object->scEta) < 0.8 && object->mvaTrigV0 > 0.94)
          value = true;
        else if (abs (object->scEta) < 1.479 && object->mvaTrigV0 > 0.85)
          value = true;
        else if (abs (object->scEta) < 2.5 && object->mvaTrigV0 > 0.92)
          value = true;
      }
  }

  else if(variable == "looseID_MVA"){
    value = object->pt > 10
      && object->mvaNonTrigV0 > 0;
      }
  else if(variable == "correctedD0VertexInEBPositiveCharge"){
    if(fabs(object->eta) < 0.8 && object->charge > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBPositiveCharge"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->charge > 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEEPositiveCharge"){
    if(object->isEE && object->charge > 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBPositiveCharge"){
    if(fabs(object->eta) < 0.8 && object->charge > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBPositiveCharge"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->charge > 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEEPositiveCharge"){
    if(object->isEE && object->charge > 0) value = object->correctedD0;
    else value = -999;
  }

  else if(variable == "correctedD0VertexInEBNegativeCharge"){
    if(fabs(object->eta) < 0.8 && object->charge < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexOutEBNegativeCharge"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->charge < 0) value = object->correctedD0Vertex;
    else value = -999;
  }
  else if(variable == "correctedD0VertexEENegativeCharge"){
    if(object->isEE && object->charge < 0) value = object->correctedD0Vertex;
    else value = -999;
  }

  else if(variable == "correctedD0BeamspotInEBNegativeCharge"){
    if(fabs(object->eta) < 0.8 && object->charge < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotOutEBNegativeCharge"){
    if(object->isEB && fabs(object->eta) > 0.8 && object->charge < 0) value = object->correctedD0;
    else value = -999;
  }
  else if(variable == "correctedD0BeamspotEENegativeCharge"){
    if(object->isEE && object->charge < 0) value = object->correctedD0;
    else value = -999;
  }


  else if(variable == "tightIDdisplaced"){
    if (object->isEB)
      {
        value = fabs(object->delEtaIn) < 0.004 \
          && fabs (object->delPhiIn) < 0.03 \
          && object->scSigmaIEtaIEta < 0.01 \
          && object->hadOverEm < 0.12 \
          && object->absInvEMinusInvPin < 0.05;
      }
    else
      {
        value = fabs (object->delEtaIn) < 0.005 \
          && fabs (object->delPhiIn) < 0.02 \
          && object->scSigmaIEtaIEta < 0.03 \
          && object->hadOverEm < 0.10 \
          && object->absInvEMinusInvPin < 0.05;
      }
  }


  else if(variable == "genDeltaRLowest") value = getGenDeltaRLowest(object);

  else if(variable == "genMatchedPdgId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = mcparticles->at(index).id;
  }

  else if(variable == "genRecoChargeProduct"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 2;
    else value = object->charge * mcparticles->at(index).charge;
  }

  else if(variable == "genMatchedId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = getPdgIdBinValue(mcparticles->at(index).id);
  }
  else if(variable == "genMatchedMotherId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genMatchedMotherIdReverse"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 24;
    else value = 24 -getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genMatchedGrandmotherId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else if(fabs(mcparticles->at(index).motherId) == 15){
      int motherIndex = findTauMotherIndex(&mcparticles->at(index));
      if(motherIndex == -1) value = 0;
      else value = getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = getPdgIdBinValue(mcparticles->at(index).grandMotherId);
  }
  else if(variable == "genMatchedGrandmotherIdReverse"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 24;
    else if(fabs(mcparticles->at(index).motherId) == 15){
      int motherIndex = findTauMotherIndex(&mcparticles->at(index));
      if(motherIndex == -1) value = 24;
      else value = 24 - getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = 24 - getPdgIdBinValue(mcparticles->at(index).grandMotherId);
  }
  else if(variable == "pfElectronsFromVertex"){
    double d0Error, dzError;

    d0Error = hypot (object->tkD0err, hypot (chosenVertex ()->xError, chosenVertex ()->yError));
    dzError = hypot (object->tkDZerr, chosenVertex ()->zError);
    value = fabs (object->correctedD0Vertex) > 0.2 || fabs (object->correctedDZ) > 0.5
      || fabs (object->correctedD0Vertex / d0Error) > 99.0
      || fabs (object->correctedDZ / dzError) > 99.0;
    value = !value;
  }



  else{clog << "WARNING: invalid electron variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end electron valueLookup


//!event valueLookup
double
OSUAnalysis::valueLookup (const BNevent* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "weight") value = object->weight;
  else if(variable == "pthat") value = object->pthat;
  else if(variable == "qScale") value = object->qScale;
  else if(variable == "alphaQCD") value = object->alphaQCD;
  else if(variable == "alphaQED") value = object->alphaQED;
  else if(variable == "scalePDF") value = object->scalePDF;
  else if(variable == "x1") value = object->x1;
  else if(variable == "nExtraPartons") value = GetNumExtraPartons(mcparticles.product());
  else if(variable == "x2") value = object->x2;
  else if(variable == "xPDF1") value = object->xPDF1;
  else if(variable == "xPDF2") value = object->xPDF2;
  else if(variable == "BSx") value = object->BSx;
  else if(variable == "BSy") value = object->BSy;
  else if(variable == "BSz") value = object->BSz;
  else if(variable == "PVx") value = object->PVx;
  else if(variable == "PVy") value = object->PVy;
  else if(variable == "PVz") value = object->PVz;
  else if(variable == "bField") value = object->bField;
  else if(variable == "instLumi") value = object->instLumi;
  else if(variable == "bxLumi") value = object->bxLumi;
  else if(variable == "FilterOutScrapingFraction") value = object->FilterOutScrapingFraction;
  else if(variable == "sumNVtx") value = object->sumNVtx;
  else if(variable == "sumTrueNVtx") value = object->sumTrueNVtx;
  else if(variable == "nm1_true") value = object->nm1_true;
  else if(variable == "n0_true") value = object->n0_true;
  else if(variable == "np1_true") value = object->np1_true;
  else if(variable == "numTruePV") value = object->numTruePV;
  else if(variable == "Q2ScaleUpWgt") value = object->Q2ScaleUpWgt;
  else if(variable == "Q2ScaleDownWgt") value = object->Q2ScaleDownWgt;
  else if(variable == "rho_kt6PFJets") value = object->rho_kt6PFJets;
  else if(variable == "rho_kt6PFJetsCentralChargedPileUp") value = object->rho_kt6PFJetsCentralChargedPileUp;
  else if(variable == "rho_kt6PFJetsCentralNeutral") value = object->rho_kt6PFJetsCentralNeutral;
  else if(variable == "rho_kt6PFJetsCentralNeutralTight") value = object->rho_kt6PFJetsCentralNeutralTight;
  else if(variable == "run") value = object->run;
  else if(variable == "lumi") value = object->lumi;
  else if(variable == "sample") value = object->sample;
  else if(variable == "numPV") value = object->numPV;
  else if(variable == "W0decay") value = object->W0decay;
  else if(variable == "W1decay") value = object->W1decay;
  else if(variable == "Z0decay") value = object->Z0decay;
  else if(variable == "Z1decay") value = object->Z1decay;
  else if(variable == "H0decay") value = object->H0decay;
  else if(variable == "H1decay") value = object->H1decay;
  else if(variable == "hcalnoiseLoose") value = object->hcalnoiseLoose;
  else if(variable == "hcalnoiseTight") value = object->hcalnoiseTight;
  else if(variable == "GoodVertex") value = object->GoodVertex;
  else if(variable == "FilterOutScraping") value = object->FilterOutScraping;
  else if(variable == "HBHENoiseFilter") value = object->HBHENoiseFilter;
  else if(variable == "CSCLooseHaloId") value = object->CSCLooseHaloId;
  else if(variable == "CSCTightHaloId") value = object->CSCTightHaloId;
  else if(variable == "EcalLooseHaloId") value = object->EcalLooseHaloId;
  else if(variable == "EcalTightHaloId") value = object->EcalTightHaloId;
  else if(variable == "HcalLooseHaloId") value = object->HcalLooseHaloId;
  else if(variable == "HcalTightHaloId") value = object->HcalTightHaloId;
  else if(variable == "GlobalLooseHaloId") value = object->GlobalLooseHaloId;
  else if(variable == "GlobalTightHaloId") value = object->GlobalTightHaloId;
  else if(variable == "LooseId") value = object->LooseId;
  else if(variable == "TightId") value = object->TightId;
  else if(variable == "numGenPV") value = object->numGenPV;
  else if(variable == "nm1") value = object->nm1;
  else if(variable == "n0") value = object->n0;
  else if(variable == "np1") value = object->np1;
  else if(variable == "id1") value = object->id1;
  else if(variable == "id2") value = object->id2;
  else if(variable == "evt") value = object->evt;
  else if(variable == "puScaleFactor"){
    if(doPileupReweighting_ && datasetType_ != "data")
      value = puWeight_->at (events->at (0).numTruePV);
    else
      value = 1.0;
  }
  else if(variable == "muonScaleFactor") value = muonScaleFactor_;
  else if(variable == "electronScaleFactor") value = electronScaleFactor_;
  else if(variable == "muonTrackScaleFactor") value = muonTrackScaleFactor_;
  else if(variable == "electronTrackScaleFactor") value = electronTrackScaleFactor_;
  else if(variable == "stopCTauScaleFactor") value = stopCTauScaleFactor_;
  else if(variable == "bTagScaleFactor") value = bTagScaleFactor_;
  else if(variable == "topPtScaleFactor") value = topPtScaleFactor_;
  else if(variable == "triggerScaleFactor") value = triggerScaleFactor_;
  else if(variable == "triggerMetScaleFactor") value = triggerMetScaleFactor_;
  else if(variable == "trackNMissOutScaleFactor") value = trackNMissOutScaleFactor_;
  else if(variable == "globalScaleFactor") value = globalScaleFactor_;
  else if(variable == "channelScaleFactor") value = channelScaleFactor_;
  else if(variable == "eventScaleFactor") value = eventScaleFactor_;
  else if(variable == "unfilteredHt") value = getHt(jets.product());
  else if(variable == "ht") value = chosenHT ();
  else if(variable == "unfilteredSt") value = getSt(electrons.product(),muons.product(),jets.product());
  else if(variable == "st") value = chosenST ();

  else if(variable == "leadMuPairInvMass"){
    pair<const BNmuon *, const BNmuon *> muPair = leadMuonPair ();
    TLorentzVector p0 (muPair.first->px, muPair.first->py, muPair.first->pz, muPair.first->energy),
                   p1 (muPair.second->px, muPair.second->py, muPair.second->pz, muPair.second->energy);
    value = (p0 + p1).M ();
  }
  else if(variable == "leadMuPairPt"){
    pair<const BNmuon *, const BNmuon *> muPair = leadMuonPair ();
    TVector2 pt0 (muPair.first->px, muPair.first->py),
             pt1 (muPair.second->px, muPair.second->py);
    pt0 += pt1;
    value = pt0.Mod ();
  }
  else if(variable == "leadElPairInvMass"){
    pair<const BNelectron *, const BNelectron *> muPair = leadElectronPair ();
    TLorentzVector p0 (muPair.first->px, muPair.first->py, muPair.first->pz, muPair.first->energy),
                   p1 (muPair.second->px, muPair.second->py, muPair.second->pz, muPair.second->energy);
    value = (p0 + p1).M ();
  }
  else if(variable == "leadElPairPt"){
    pair<const BNelectron *, const BNelectron *> muPair = leadElectronPair ();
    TVector2 pt0 (muPair.first->px, muPair.first->py),
             pt1 (muPair.second->px, muPair.second->py);
    pt0 += pt1;
    value = pt0.Mod ();
  }
  else{clog << "WARNING: invalid event variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end event valueLookup


//!tau valueLookup
double
OSUAnalysis::valueLookup (const BNtau* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "energy") value = object->energy;
  else if(variable == "et") value = object->et;
  else if(variable == "pt") value = object->pt;
  else if(variable == "eta") value = object->eta;
  else if(variable == "phi") value = object->phi;
  else if(variable == "emFraction") value = object->emFraction;
  else if(variable == "leadingTrackPt") value = object->leadingTrackPt;
  else if(variable == "leadingTrackIpVtdxy") value = object->leadingTrackIpVtdxy;
  else if(variable == "leadingTrackIpVtdz") value = object->leadingTrackIpVtdz;
  else if(variable == "leadingTrackIpVtdxyError") value = object->leadingTrackIpVtdxyError;
  else if(variable == "leadingTrackIpVtdzError") value = object->leadingTrackIpVtdzError;
  else if(variable == "leadingTrackVx") value = object->leadingTrackVx;
  else if(variable == "leadingTrackVy") value = object->leadingTrackVy;
  else if(variable == "leadingTrackVz") value = object->leadingTrackVz;
  else if(variable == "leadingTrackValidHits") value = object->leadingTrackValidHits;
  else if(variable == "leadingTrackNormChiSqrd") value = object->leadingTrackNormChiSqrd;
  else if(variable == "numProngs") value = object->numProngs;
  else if(variable == "numSignalGammas") value = object->numSignalGammas;
  else if(variable == "numSignalNeutrals") value = object->numSignalNeutrals;
  else if(variable == "numSignalPiZeros") value = object->numSignalPiZeros;
  else if(variable == "decayMode") value = object->decayMode;
  else if(variable == "charge") value = object->charge;
  else if(variable == "inTheCracks") value = object->inTheCracks;
  else if(variable == "HPSagainstElectronLoose") value = object->HPSagainstElectronLoose;
  else if(variable == "HPSagainstElectronMVA") value = object->HPSagainstElectronMVA;
  else if(variable == "HPSagainstElectronMedium") value = object->HPSagainstElectronMedium;
  else if(variable == "HPSagainstElectronTight") value = object->HPSagainstElectronTight;
  else if(variable == "HPSagainstMuonLoose") value = object->HPSagainstMuonLoose;
  else if(variable == "HPSagainstMuonMedium") value = object->HPSagainstMuonMedium;
  else if(variable == "HPSagainstMuonTight") value = object->HPSagainstMuonTight;
  else if(variable == "HPSbyLooseCombinedIsolationDeltaBetaCorr") value = object->HPSbyLooseCombinedIsolationDeltaBetaCorr;
  else if(variable == "HPSbyMediumCombinedIsolationDeltaBetaCorr") value = object->HPSbyMediumCombinedIsolationDeltaBetaCorr;
  else if(variable == "HPSbyTightCombinedIsolationDeltaBetaCorr") value = object->HPSbyTightCombinedIsolationDeltaBetaCorr;
  else if(variable == "HPSbyVLooseCombinedIsolationDeltaBetaCorr") value = object->HPSbyVLooseCombinedIsolationDeltaBetaCorr;
  else if(variable == "HPSdecayModeFinding") value = object->HPSdecayModeFinding;
  else if(variable == "leadingTrackValid") value = object->leadingTrackValid;

  else if (variable == "looseHadronicID") {
    //criteria taken from http://cms.cern.ch/iCMS/jsp/db_notes/showNoteDetails.jsp?noteID=CMS%20AN-2011/019
    value = object->pt > 30
      && fabs(object->eta) < 2.3
      && object->HPSbyLooseCombinedIsolationDeltaBetaCorr > 0
      && object->HPSdecayModeFinding > 0
      && object->HPSagainstElectronLoose > 0
      && object->HPSagainstMuonTight > 0;
  }

  else if(variable == "genDeltaRLowest") value = getGenDeltaRLowest(object);

  else if(variable == "genMatchedPdgId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = mcparticles->at(index).id;
  }

  else if(variable == "genMatchedId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = getPdgIdBinValue(mcparticles->at(index).id);
  }
  else if(variable == "genMatchedMotherId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genMatchedMotherIdReverse"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 24;
    else value = 24 -getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genMatchedGrandmotherId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else if(fabs(mcparticles->at(index).motherId) == 15){
      int motherIndex = findTauMotherIndex(&mcparticles->at(index));
      if(motherIndex == -1) value = 0;
      else value = getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = getPdgIdBinValue(mcparticles->at(index).grandMotherId);
  }
  else if(variable == "genMatchedGrandmotherIdReverse"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 24;
    else if(fabs(mcparticles->at(index).motherId) == 15){
      int motherIndex = findTauMotherIndex(&mcparticles->at(index));
      if(motherIndex == -1) value = 24;
      else value = 24 - getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = 24 - getPdgIdBinValue(mcparticles->at(index).grandMotherId);
  }


  else{clog << "WARNING: invalid tau variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end tau valueLookup


//!met valueLookup
double
OSUAnalysis::valueLookup (const BNmet* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "et") value = object->et;
  else if(variable == "pt") value = object->pt;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "phi") value = object->phi;
  else if(variable == "Upt") value = object->Upt;
  else if(variable == "Uphi") value = object->Uphi;
  else if(variable == "NeutralEMFraction") value = object->NeutralEMFraction;
  else if(variable == "NeutralHadEtFraction") value = object->NeutralHadEtFraction;
  else if(variable == "ChargedEMEtFraction") value = object->ChargedEMEtFraction;
  else if(variable == "ChargedHadEtFraction") value = object->ChargedHadEtFraction;
  else if(variable == "MuonEtFraction") value = object->MuonEtFraction;
  else if(variable == "Type6EtFraction") value = object->Type6EtFraction;
  else if(variable == "Type7EtFraction") value = object->Type7EtFraction;
  else if(variable == "genPT") value = object->genPT;
  else if(variable == "genPhi") value = object->genPhi;
  else if(variable == "muonCorEx") value = object->muonCorEx;
  else if(variable == "muonCorEy") value = object->muonCorEy;
  else if(variable == "jet20CorEx") value = object->jet20CorEx;
  else if(variable == "jet20CorEy") value = object->jet20CorEy;
  else if(variable == "jet1CorEx") value = object->jet1CorEx;
  else if(variable == "jet1CorEy") value = object->jet1CorEy;
  else if(variable == "sumET") value = object->sumET;
  else if(variable == "corSumET") value = object->corSumET;
  else if(variable == "mEtSig") value = object->mEtSig;
  else if(variable == "metSignificance") value = object->metSignificance;
  else if(variable == "significance") value = object->significance;
  else if(variable == "sigmaX2") value = object->sigmaX2;
  else if(variable == "sigmaY2") value = object->sigmaY2;
  else if(variable == "sigmaXY") value = object->sigmaXY;
  else if(variable == "sigmaYX") value = object->sigmaYX;
  else if(variable == "maxEtInEmTowers") value = object->maxEtInEmTowers;
  else if(variable == "emEtFraction") value = object->emEtFraction;
  else if(variable == "emEtInEB") value = object->emEtInEB;
  else if(variable == "emEtInEE") value = object->emEtInEE;
  else if(variable == "emEtInHF") value = object->emEtInHF;
  else if(variable == "maxEtInHadTowers") value = object->maxEtInHadTowers;
  else if(variable == "hadEtFraction") value = object->hadEtFraction;
  else if(variable == "hadEtInHB") value = object->hadEtInHB;
  else if(variable == "hadEtInHE") value = object->hadEtInHE;
  else if(variable == "hadEtInHF") value = object->hadEtInHF;
  else if(variable == "hadEtInHO") value = object->hadEtInHO;
  else if(variable == "UDeltaPx") value = object->UDeltaPx;
  else if(variable == "UDeltaPy") value = object->UDeltaPy;
  else if(variable == "UDeltaP") value = object->UDeltaP;
  else if(variable == "Uscale") value = object->Uscale;
  else if(variable == "type2corPx") value = object->type2corPx;
  else if(variable == "type2corPy") value = object->type2corPy;
  else if(variable == "T2pt") value = object->T2pt;
  else if(variable == "T2px") value = object->T2px;
  else if(variable == "T2py") value = object->T2py;
  else if(variable == "T2phi") value = object->T2phi;
  else if(variable == "T2sumET") value = object->T2sumET;
  else if(variable == "pfT1jet1pt") value = object->pfT1jet1pt;
  else if(variable == "pfT1jet1phi") value = object->pfT1jet1phi;
  else if(variable == "pfT1jet6pt") value = object->pfT1jet6pt;
  else if(variable == "pfT1jet6phi") value = object->pfT1jet6phi;
  else if(variable == "pfT1jet10pt") value = object->pfT1jet10pt;
  else if(variable == "pfT1jet10phi") value = object->pfT1jet10phi;

  else if(variable == "metNoMu") { 
    // Calculate the MET, without including muons in the sum of E_T  
    TVector2 p2Met;
    TVector2 p2Muon;
    p2Met. SetMagPhi(object->pt, object->phi);
    p2Muon.SetMagPhi(0, 0);  
    if (!muons.product()) clog << "ERROR: cannot find metNoMu because muons collection is not initialized." << endl;
    for (uint imuon = 0; imuon<muons->size(); imuon++) {  
      string empty = "";
      double muonPt  = valueLookup(&muons->at(imuon), "pt",  "", empty);
      double muonPhi = valueLookup(&muons->at(imuon), "phi", "", empty);
      TVector2 p2MuonTmp;
      p2MuonTmp.SetMagPhi(muonPt, muonPhi);  
      p2Muon += p2MuonTmp;  
    }
    TVector2 p2MetNoMu = p2Met + p2Muon;
    value = p2MetNoMu.Mod();
  }
  else if(variable == "metNoElec") { 
    // Calculate the MET, without including electrons in the sum of E_T  
    TVector2 p2Met;
    TVector2 p2Elec;
    p2Met. SetMagPhi(object->pt, object->phi);
    p2Elec.SetMagPhi(0, 0);  
    if (!electrons.product()) clog << "ERROR: cannot find metNoElec because electron collection is not initialized." << endl;
    for (uint ielec = 0; ielec<electrons->size(); ielec++) {  
      string empty = "";  
      double elecPt  = valueLookup(&electrons->at(ielec), "pt",  "", empty);
      double elecPhi = valueLookup(&electrons->at(ielec), "phi", "", empty);
      TVector2 p2ElecTmp;
      p2ElecTmp.SetMagPhi(elecPt, elecPhi);  
      p2Elec += p2ElecTmp;  
    }
    TVector2 p2MetNoElec = p2Met + p2Elec;
    value =  p2MetNoElec.Mod();
  }
  else if(variable == "deltaPhiMin2Jets") {
    // minimum |deltaPhi| between met vector and either of the two leading jets 
    // only consider jets with pt > 45 GeV and |eta|<2.8  
    if (!jets.product()) clog << "ERROR:  cannot find deltaPhiMin2Jets because jets collection is not initialized." << endl;
    double ptJet1 = -99; // leading jet
    double ptJet2 = -99; // 2nd jet 
    double phiJet1 = -99;
    double phiJet2 = -99;  
    if (verbose_>4) clog << "Debug:  calculating deltaPhiMin2Jets" << endl;  
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      // find indices of candidate jets 
      string empty = "";
      double jetPt  = valueLookup(&jets->at(ijet), "pt", "", empty); 
      double jetEta = valueLookup(&jets->at(ijet), "eta", "", empty); 
      double jetPhi = valueLookup(&jets->at(ijet), "phi", "", empty); 
      if (jetPt < 45 ||
	  fabs(jetEta) > 2.8) continue;  
      if (verbose_>4) clog << "  Found jet with pt=" << jetPt << ", eta=" << jetEta << ", phi=" << jetPhi << endl;  
      if        (jetPt > ptJet1) { 
	ptJet2 = ptJet1; 
	ptJet1 = jetPt;  
	phiJet2 = phiJet1;  
	phiJet1 = jetPhi;  
      } else if (jetPt > ptJet2) {
	ptJet2 = jetPt;  
	phiJet2 = jetPhi;  
      }
    }

    // find minimum deltaPhi
    double deltaPhiMin2Jets = 99.;  
    if (ptJet1 >=0) {
      double dPhi = fabs(deltaPhi (phiJet1, object->phi));  
      if (dPhi<deltaPhiMin2Jets) deltaPhiMin2Jets = dPhi;  
    }
    if (ptJet2 >=0) {
      double dPhi = fabs(deltaPhi (phiJet2, object->phi));  
      if (dPhi<deltaPhiMin2Jets) deltaPhiMin2Jets = dPhi;  
    }
    value = deltaPhiMin2Jets;  
    if (verbose_>4) clog  << "  Met phi==" << object->phi << endl;  
    if (verbose_>4) clog << "  Calculated value:  deltaPhiMin2Jets=" << deltaPhiMin2Jets << endl;  
  }

  else if(variable == "deltaPhiJet1") {
    // |deltaPhi| between met vector and leading jet 
    // only consider jets with pt > 45 GeV and |eta|<2.8  
    if (!jets.product()) clog << "ERROR:  cannot find deltaPhiJet1 because jets collection is not initialized." << endl;
    double ptJet1 = -99; // leading jet
    double phiJet1 = -99;
    if (verbose_>4) clog << "Debug:  calculating deltaPhiJet1" << endl;  
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      // find indices of candidate jets 
      string empty = "";
      double jetPt  = valueLookup(&jets->at(ijet), "pt", "", empty); 
      double jetEta = valueLookup(&jets->at(ijet), "eta", "", empty); 
      double jetPhi = valueLookup(&jets->at(ijet), "phi", "", empty); 
      if (jetPt < 45 ||
	  fabs(jetEta) > 2.8) continue;  
      if (verbose_>4) clog << "  Found jet with pt=" << jetPt << ", eta=" << jetEta << ", phi=" << jetPhi << endl;  
      if (jetPt > ptJet1) { 
	ptJet1 = jetPt;  
	phiJet1 = jetPhi;  
      }
    }

    double deltaPhiJet1 = 99.; 
    if (ptJet1 >=0) {
      double dPhi = fabs(deltaPhi (phiJet1, object->phi));  
      deltaPhiJet1 = dPhi;  
    }
    value = deltaPhiJet1;  
  }

  else if(variable == "deltaPhiJet2") {
    // |deltaPhi| between met vector and subleading jet 
    // only consider jets with pt > 45 GeV and |eta|<2.8  
    if (!jets.product()) clog << "ERROR:  cannot find deltaPhiJet2 because jets collection is not initialized." << endl;
    double ptJet1 = -99; // leading jet
    double ptJet2 = -99; // 2nd jet 
    double phiJet1 = -99;
    double phiJet2 = -99;  
    if (verbose_>4) clog << "Debug:  calculating deltaPhiMin2Jets" << endl;  
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      // find indices of candidate jets 
      string empty = "";
      double jetPt  = valueLookup(&jets->at(ijet), "pt", "", empty); 
      double jetEta = valueLookup(&jets->at(ijet), "eta", "", empty); 
      double jetPhi = valueLookup(&jets->at(ijet), "phi", "", empty); 
      if (jetPt < 45 ||
	  fabs(jetEta) > 2.8) continue;  
      if (verbose_>4) clog << "  Found jet with pt=" << jetPt << ", eta=" << jetEta << ", phi=" << jetPhi << endl;  
      if        (jetPt > ptJet1) { 
	ptJet2 = ptJet1; 
	ptJet1 = jetPt;  
	phiJet2 = phiJet1;  
	phiJet1 = jetPhi;  
      } else if (jetPt > ptJet2) {
	ptJet2 = jetPt;  
	phiJet2 = jetPhi;  
      }
    }

    // find minimum deltaPhi
    double deltaPhiJet2 = 99.;  
    if (ptJet2 >=0) {
      double dPhi = fabs(deltaPhi (phiJet2, object->phi));  
      deltaPhiJet2 = dPhi;  
    }
    value = deltaPhiJet2;  
  }

  else{clog << "WARNING: invalid met variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end met valueLookup


//!track valueLookup
double
OSUAnalysis::valueLookup (const BNtrack* object, string variable, string function, string &stringValue){

  double value = 0.0;
  double pMag = sqrt(object->pt * object->pt +
                     object->pz * object->pz);

  if(variable == "pt") value = object->pt;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "phi") value = object->phi;
  else if(variable == "eta") value = object->eta;
  else if(variable == "theta") value = object->theta;
  else if(variable == "normChi2") value = object->normChi2;
  else if(variable == "dZ") value = object->dZ;
  else if(variable == "d0") value = object->d0;
  else if(variable == "d0err") value = object->d0err;
  else if(variable == "vx") value = object->vx;
  else if(variable == "vy") value = object->vy;
  else if(variable == "vz") value = object->vz;
  else if(variable == "charge") value = object->charge;
  else if(variable == "numValidHits") value = object->numValidHits;
  else if(variable == "isHighPurity") value = object->isHighPurity;

  //additional BNs info for disappTrks
  else if(variable == "caloEMDeltaRp3")     value = object->caloEMDeltaRp3;
  else if(variable == "caloHadDeltaRp3")    value = object->caloHadDeltaRp3;
  else if(variable == "caloEMDeltaRp4")     value = object->caloEMDeltaRp4;
  else if(variable == "caloHadDeltaRp4")    value = object->caloHadDeltaRp4;
  else if(variable == "caloEMDeltaRp5")     value = object->caloEMDeltaRp5;
  else if(variable == "caloHadDeltaRp5")    value = object->caloHadDeltaRp5;
  else if(variable == "nHitsMissingOuter")  value = object->nHitsMissingOuter;
  else if(variable == "nHitsMissingInner")  value = object->nHitsMissingInner;
  else if(variable == "nHitsMissingMiddle") value = object->nHitsMissingMiddle;
  else if(variable == "depTrkRp3")          value = object->depTrkRp3;
  else if(variable == "trkRelIsoRp3")     { value = (object->depTrkRp3 - object->pt) / object->pt; if (value<0) value = 0; }
  else if(variable == "trkRelIsoRp5")     { value = (object->depTrkRp5 - object->pt) / object->pt; if (value<0) value = 0; }
  else if(variable == "depEcalRp3")         value = object->depEcalRp3;
  else if(variable == "depHcalRp3")         value = object->depHcalRp3;
  else if(variable == "depHoRp3")           value = object->depHoRp3;
  else if(variable == "nTracksRp3")         value = object->nTracksRp3;
  else if(variable == "trackerVetoPtRp3")   value = object->trackerVetoPtRp3;
  else if(variable == "emVetoEtRp3")        value = object->emVetoEtRp3;
  else if(variable == "hadVetoEtRp3")       value = object->hadVetoEtRp3;
  else if(variable == "hoVetoEtRp3")        value = object->hoVetoEtRp3;
  else if(variable == "depTrkRp5")          value = object->depTrkRp5;
  else if(variable == "depEcalRp5")         value = object->depEcalRp5;
  else if(variable == "depHcalRp5")         value = object->depHcalRp5;
  else if(variable == "depHoRp5")           value = object->depHoRp5;
  else if(variable == "nTracksRp5")         value = object->nTracksRp5;
  else if(variable == "trackerVetoPtRp5")   value = object->trackerVetoPtRp5;
  else if(variable == "emVetoEtRp5")        value = object->emVetoEtRp5;
  else if(variable == "hadVetoEtRp5")       value = object->hadVetoEtRp5;
  else if(variable == "hoVetoEtRp5")        value = object->hoVetoEtRp5;

  //user defined variables
  else if(variable == "d0wrtBS") value = (object->vx-events->at(0).BSx)*object->py/object->pt - (object->vy-events->at(0).BSy)*object->px/object->pt;
  else if(variable == "dZwrtBS") value = object->dZ - events->at(0).BSz;
  else if(variable == "depTrkRp5MinusPt"){
    if ( (object->depTrkRp5 - object->pt) < 0 ) {
//       clog << "Warning:  found track with depTrkRp5 < pt:  depTrkRp5=" << object->depTrkRp5
//            << "; pt=" << object->pt
//            << "; object->depTrkRp5 - object->pt = " << object->depTrkRp5 - object->pt
//            << endl;
           value = 0;
         }
         else value =  (object->depTrkRp5 - object->pt);
  }
  else if(variable == "depTrkRp3MinusPt"){
    if ( (object->depTrkRp3 - object->pt) < 0 ) {
      value = 0;
    }
    else value =  (object->depTrkRp3 - object->pt);
  }

  else if(variable == "dPhiMet") {
    if (const BNmet *met = chosenMET ()) {
      value = deltaPhi (object->phi, met->phi);
    } else value = -999;
  }

  else if (variable == "rhoCorrRp5")                 value = getRhoCorr(object);
  else if(variable == "caloTotDeltaRp5")            value =  (object->caloHadDeltaRp5 + object->caloEMDeltaRp5);
  else if(variable == "caloTotDeltaRp5ByP")         value = ((object->caloHadDeltaRp5 + object->caloEMDeltaRp5)/pMag);
  else if(variable == "caloTotDeltaRp5RhoCorr")     value = getTrkCaloTotRhoCorr(object);
  else if(variable == "caloTotDeltaRp5ByPRhoCorr")  value = getTrkCaloTotRhoCorr(object) / pMag;
  else if(variable == "depTrkRp5RhoCorr")           value = getTrkDepTrkRp5RhoCorr(object);
  else if(variable == "depTrkRp3RhoCorr")           value = getTrkDepTrkRp3RhoCorr(object);

  else if(variable == "depTrkRp5MinusPtRhoCorr")    {
    if ( (getTrkDepTrkRp5RhoCorr(object) - object->pt) < 0 ) value = 0;
    else {value = (getTrkDepTrkRp5RhoCorr(object) - object->pt );}
  }

  else if(variable == "depTrkRp3MinusPtRhoCorr")
    {
      if ( (getTrkDepTrkRp3RhoCorr(object) - object->pt) < 0 ) value = 0;
      else {value = (getTrkDepTrkRp3RhoCorr(object) - object->pt );}
    }

  else if(variable == "isIso")                      value = getTrkIsIso(object, tracks.product());
  else if(variable == "isMatchedDeadEcal")          value = getTrkIsMatchedDeadEcal(object);
    else if(variable == "trkDeadEcalDeltaR")          value = getTrkDeadEcalDeltaR(object);
  else if(variable == "isMatchedBadCSC")            value = getTrkIsMatchedBadCSC  (object);
  else if(variable == "ptErrorByPt")                value = (object->ptError/object->pt);
  else if(variable == "ptError")                    value = object->ptError;
  else if(variable == "ptRes")                      value = getTrkPtRes(object);
  else if(variable == "ptTrue")                     value = getTrkPtTrue(object, mcparticles.product());
  else if (variable == "d0wrtPV"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
      px = object->px,
      py = object->py,
      pt = object->pt;
    value = (-vx * py + vy * px) / pt;
  }
  else if (variable == "dZwrtPV"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
      vz = object->vz - chosenVertex ()->z,
      px = object->px,
      py = object->py,
      pz = object->pz,
      pt = object->pt;
    value = vz - (vx * px + vy * py)/pt * (pz/pt);
  }
  else if (variable == "dZSinTheta"){
    string empty = ""; 
    double dZwrtPV = valueLookup(object, "dZwrtPV", "", empty);
    value = dZwrtPV * (object->pt / pMag);  // sin(theta) = pt / p
  }

  else if(variable == "deltaRMinSubLeadJet") {
    // calculate minimum deltaR between track and any other subleading jet
    double trkJetDeltaRMin = 99.;
    if (!jets.product()) clog << "ERROR:  cannot find deltaRMinSubLeadJet because jets collection is not initialized." << endl;
    for (uint ijet = 0; ijet<jets->size(); ijet++) {
      string empty = "";
      double isSubLeadingJet = valueLookup(&jets->at(ijet), "disappTrkSubLeadingJetID", "", empty);
      if (!isSubLeadingJet) continue;  // only consider jets that pass the subleading jet ID criteria
      double jetEta = valueLookup(&jets->at(ijet), "eta", "", empty);
      double jetPhi = valueLookup(&jets->at(ijet), "phi", "", empty);
      double trkJetDeltaR = deltaR(object->eta, object->phi, jetEta, jetPhi);
      if (trkJetDeltaR < trkJetDeltaRMin) trkJetDeltaRMin = trkJetDeltaR;
    }
    value = trkJetDeltaRMin;
  }

  else if(variable == "deltaRMinElecLooseMvaId") {
    // calculate minimum deltaR between track and any other electron passing loose mva ID, i.e., mvaNonTrigV0 > 0
    double trkElecDeltaRMin = 99.;
    if (!electrons.product()) clog << "ERROR:  cannot find deltaRMinElecLooseMvaId because electrons collection is not initialized." << endl;
    for (uint ielec = 0; ielec<electrons->size(); ielec++) {
      string empty = "";
      double mvaNonTrigV0 = valueLookup(&electrons->at(ielec), "mvaNonTrigV0", "", empty);
      if (mvaNonTrigV0 < 0) continue;  // only consider electrons with mvaNonTrigV0 > 0
      double elecEta = valueLookup(&electrons->at(ielec), "eta", "", empty);
      double elecPhi = valueLookup(&electrons->at(ielec), "phi", "", empty);
      double trkElecDeltaR = deltaR(object->eta, object->phi, elecEta, elecPhi);
      if (trkElecDeltaR < trkElecDeltaRMin) trkElecDeltaRMin = trkElecDeltaR;
    }
    value = trkElecDeltaRMin;
  }

  else if(variable == "isPassMuonLooseID") {
    // boolean for whether track is loosely identified with a muon, 
    // i.e., true if it is DeltaR-matched to a member of either of the muon or secondary muon collections
    string empty = "";
    double trkMuonDeltaRMin    = valueLookup(object, "deltaRMinMuonLooseId",    "", empty);
    double trkSecMuonDeltaRMin = valueLookup(object, "deltaRMinSecMuonLooseId", "", empty); 
    value = 0;                                  // initialize to be false 
    if (trkMuonDeltaRMin    < 0.15) value = 1;  // true if matched to muon 
    if (trkSecMuonDeltaRMin < 0.15) value = 1;  // true if matched to secondary muon 
  }

  else if(variable == "deltaRMinMuonLooseId") {
    // calculate minimum deltaR between track and any muon
    double trkMuonDeltaRMin = 99.;
    if (!muons.product()) clog << "ERROR:  cannot find deltaRMinMuonLooseId because muons collection is not initialized." << endl;
    for (uint imuon = 0; imuon<muons->size(); imuon++) {
      string empty = "";
      double muonEta = valueLookup(&muons->at(imuon), "eta", "", empty);
      double muonPhi = valueLookup(&muons->at(imuon), "phi", "", empty);
      double trkMuonDeltaR = deltaR(object->eta, object->phi, muonEta, muonPhi);
      if (trkMuonDeltaR < trkMuonDeltaRMin) trkMuonDeltaRMin = trkMuonDeltaR;
    }
    value = trkMuonDeltaRMin;
  }
  else if(variable == "deltaRMinSecMuonLooseId") {
    // calculate minimum deltaR between track and any secondary muon                                       
    double trkMuonDeltaRMin = 99.;
    if (!secMuons.product()) clog << "ERROR:  cannot find deltaRMinSecMuonLooseId because secMuons collection is not initialized." << endl;
    for (uint imuon = 0; imuon<secMuons->size(); imuon++) {
      string empty = "";
      double muonEta = valueLookup(&secMuons->at(imuon), "eta", "", empty);
      double muonPhi = valueLookup(&secMuons->at(imuon), "phi", "", empty);
      double trkMuonDeltaR = deltaR(object->eta, object->phi, muonEta, muonPhi);
      if (trkMuonDeltaR < trkMuonDeltaRMin) trkMuonDeltaRMin = trkMuonDeltaR;
    }
    value = trkMuonDeltaRMin;
  }

  else if(variable == "deltaRMinSecMuonLooseIdGlobal") {
    // calculate minimum deltaR between track and any other loose-Id muon
    double trkMuonDeltaRMin = 99.;
    if (!secMuons.product()) clog << "ERROR:  cannot find deltaRMinSecMuonLooseIdGlobal because secMuons collection is not initialized." << endl;
    for (uint imuon = 0; imuon<secMuons->size(); imuon++) {
      string empty = "";
      double isLooseIdMuon = valueLookup(&secMuons->at(imuon), "looseIDGlobalMuon", "", empty);
      if (!isLooseIdMuon) continue;  // only consider muons that pass the looseIDGlobal criteria
      double muonEta = valueLookup(&secMuons->at(imuon), "eta", "", empty);
      double muonPhi = valueLookup(&secMuons->at(imuon), "phi", "", empty);
      double trkMuonDeltaR = deltaR(object->eta, object->phi, muonEta, muonPhi);
      if (trkMuonDeltaR < trkMuonDeltaRMin) trkMuonDeltaRMin = trkMuonDeltaR;
    }
    value = trkMuonDeltaRMin;
  }

  else if(variable == "deltaRMinTauLooseHadronicId") {
    // calculate minimum deltaR between track and any other tau passing loose hadronic ID
    double trkTauDeltaRMin = 99.;
    if (!taus.product()) clog << "ERROR:  cannot find deltaRMinTauLooseHadronicId because taus collection is not initialized." << endl;
    for (uint itau = 0; itau<taus->size(); itau++) {
      string empty = "";
      double isIdTau = valueLookup(&taus->at(itau), "looseHadronicID", "", empty);
      if (!isIdTau) continue;  // only consider taus that pass the loose hadronic ID criteria
      double tauEta = valueLookup(&taus->at(itau), "eta", "", empty);
      double tauPhi = valueLookup(&taus->at(itau), "phi", "", empty);
      double trkTauDeltaR = deltaR(object->eta, object->phi, tauEta, tauPhi);
      if (trkTauDeltaR < trkTauDeltaRMin) trkTauDeltaRMin = trkTauDeltaR;
    }
    value = trkTauDeltaRMin;
  }

  else if(variable == "deltaPhiMuMuPair") {
    // delta phi between track and dimuon pair   
    // Set to dummy value if there are not 2 muons with pT>25, opposite sign, and 80<m(mu-mu)<100 GeV.  
    string empty = "";  

    TLorentzVector p4muon0;  // first muon                                                                                                                                                             
    TLorentzVector p4muon1;  // second muon                                                                                                                                                            
    TLorentzVector p4mumu;   // dimuon pair                                                                                                                                                            
    double mMuon = 0.105658;  // mass from PDG
    int imuon0 = -1;
    int imuon1 = -1;  
    double phiMuMu = 0;  
    for (uint imuon = 0; imuon<muons->size(); imuon++) {
      double pt0   = valueLookup(&muons->at(imuon), "pt",     "", empty); 
      double eta0  = valueLookup(&muons->at(imuon), "eta",    "", empty); 
      double phi0  = valueLookup(&muons->at(imuon), "phi",    "", empty); 
      double chg0  = valueLookup(&muons->at(imuon), "charge", "", empty); 
      if (pt0<25) continue; 
      for (uint jmuon = imuon+1; jmuon<muons->size(); jmuon++) {
	double pt1   = valueLookup(&muons->at(jmuon), "pt",      "", empty); 
	double eta1  = valueLookup(&muons->at(jmuon), "eta",     "", empty); 
	double phi1  = valueLookup(&muons->at(jmuon), "phi",     "", empty); 
	double chg1  = valueLookup(&muons->at(jmuon), "charge",  "", empty); 
	if (pt1<25)          continue; 
	if (chg0*chg1 != -1) continue;
	p4muon0.SetPtEtaPhiM(pt0, eta0, phi0, mMuon); 
	p4muon1.SetPtEtaPhiM(pt1, eta1, phi1, mMuon); 
	p4mumu = p4muon0 + p4muon1;
	if (p4mumu.M()<80 || 100<p4mumu.M()) continue;  
	// Now two muons have passed the required criteria.  
	if (imuon0>=0 || imuon1>=0) {
	  clog << "Warning [OSUAnalysis::valueLookup()]: More than one dimuon pair passes criteria in deltaPhiMuMuPair calculation." << endl;    
	}
	imuon0 = imuon;
	imuon1 = jmuon;  
	phiMuMu = p4mumu.Phi();  
      }
    }

    value = -99;                            // initialize to dummy value  
    if (imuon0>=0 && imuon1>=0) value = deltaPhi(phiMuMu, object->phi);  // only set the value if two muons pass the given criteria  

  }

  else if(variable == "isNotSigOrMatchedToSusy") {
    // Return true if the input dataset is not signal MC or if the track is matched to a SUSY particle.  
    if (datasetType_ != "signalMC") { value = 1; } else {
      int index = getGenMatchedParticleIndex(object);
      if (index == -1) value = 0;  // not matched 
      else { 
	int pdgId = abs(mcparticles->at(index).id);  
	if (pdgId>1000001 && pdgId<3160113) value = 1;  // matched to a SUSY particle  
	else                                value = 0;  
      }        
    }  
  }  

  else if(variable == "genDeltaRLowest") value = getGenDeltaRLowest(object);

  else if(variable == "genMatchedPdgId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = mcparticles->at(index).id;
  }


  else if(variable == "genMatchedId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = getPdgIdBinValue(mcparticles->at(index).id);
  }
  else if(variable == "genMatchedMotherId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genMatchedMotherIdReverse"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 24;
    else value = 24 -getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genMatchedGrandmotherId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else if(fabs(mcparticles->at(index).motherId) == 15){
      int motherIndex = findTauMotherIndex(&mcparticles->at(index));
      if(motherIndex == -1) value = 0;
      else value = getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = getPdgIdBinValue(mcparticles->at(index).grandMotherId);
  }
  else if(variable == "genMatchedGrandmotherIdReverse"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 24;
    else if(fabs(mcparticles->at(index).motherId) == 15){
      int motherIndex = findTauMotherIndex(&mcparticles->at(index));
      if(motherIndex == -1) value = 24;
      else value = 24 - getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = 24 - getPdgIdBinValue(mcparticles->at(index).grandMotherId);
  }

  else { 
    //  else{clog << "WARNING: invalid track variable '" << variable << "'\n"; value = -999;}
    // Allow looking up event-level quantities.  
    // FIXME:  Check for whether "variable" is valid for valueLookup(&events->at(0)...).  
    value = valueLookup(&events->at(0), variable, function, stringValue); 
  }

  value = applyFunction(function, value);

  return value;
} // end track valueLookup


//!genjet valueLookup
double
OSUAnalysis::valueLookup (const BNgenjet* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "pt") value = object->pt;
  else if(variable == "eta") value = object->eta;
  else if(variable == "phi") value = object->phi;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "et") value = object->et;
  else if(variable == "energy") value = object->energy;
  else if(variable == "mass") value = object->mass;
  else if(variable == "emEnergy") value = object->emEnergy;
  else if(variable == "hadEnergy") value = object->hadEnergy;
  else if(variable == "invisibleEnergy") value = object->invisibleEnergy;
  else if(variable == "auxiliaryEnergy") value = object->auxiliaryEnergy;
  else if(variable == "charge") value = object->charge;


  else{clog << "WARNING: invalid genjet variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}

//!mcparticle valueLookup
double
OSUAnalysis::valueLookup (const BNmcparticle* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "energy") value = object->energy;
  else if(variable == "et") value = object->et;
  else if(variable == "pt") value = object->pt;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "phi") value = object->phi;
  else if(variable == "eta") value = object->eta;
  else if(variable == "theta") value = object->theta;
  else if(variable == "mass") value = object->mass;
  else if(variable == "vx") value = object->vx;
  else if(variable == "vy") value = object->vy;
  else if(variable == "vz") value = object->vz;
  else if(variable == "motherET") value = object->motherET;
  else if(variable == "motherPT") value = object->motherPT;
  else if(variable == "motherPhi") value = object->motherPhi;
  else if(variable == "motherEta") value = object->motherEta;
  else if(variable == "mother0ET") value = object->mother0ET;
  else if(variable == "mother0PT") value = object->mother0PT;
  else if(variable == "mother0Phi") value = object->mother0Phi;
  else if(variable == "mother0Eta") value = object->mother0Eta;
  else if(variable == "mother1ET") value = object->mother1ET;
  else if(variable == "mother1PT") value = object->mother1PT;
  else if(variable == "mother1Phi") value = object->mother1Phi;
  else if(variable == "mother1Eta") value = object->mother1Eta;
  else if(variable == "daughter0ET") value = object->daughter0ET;
  else if(variable == "daughter0PT") value = object->daughter0PT;
  else if(variable == "daughter0Phi") value = object->daughter0Phi;
  else if(variable == "daughter0Eta") value = object->daughter0Eta;
  else if(variable == "daughter1ET") value = object->daughter1ET;
  else if(variable == "daughter1PT") value = object->daughter1PT;
  else if(variable == "daughter1Phi") value = object->daughter1Phi;
  else if(variable == "daughter1Eta") value = object->daughter1Eta;
  else if(variable == "grandMotherET") value = object->grandMotherET;
  else if(variable == "grandMotherPT") value = object->grandMotherPT;
  else if(variable == "grandMotherPhi") value = object->grandMotherPhi;
  else if(variable == "grandMotherEta") value = object->grandMotherEta;
  else if(variable == "grandMother00ET") value = object->grandMother00ET;
  else if(variable == "grandMother00PT") value = object->grandMother00PT;
  else if(variable == "grandMother00Phi") value = object->grandMother00Phi;
  else if(variable == "grandMother00Eta") value = object->grandMother00Eta;
  else if(variable == "grandMother01ET") value = object->grandMother01ET;
  else if(variable == "grandMother01PT") value = object->grandMother01PT;
  else if(variable == "grandMother01Phi") value = object->grandMother01Phi;
  else if(variable == "grandMother01Eta") value = object->grandMother01Eta;
  else if(variable == "grandMother10ET") value = object->grandMother10ET;
  else if(variable == "grandMother10PT") value = object->grandMother10PT;
  else if(variable == "grandMother10Phi") value = object->grandMother10Phi;
  else if(variable == "grandMother10Eta") value = object->grandMother10Eta;
  else if(variable == "grandMother11ET") value = object->grandMother11ET;
  else if(variable == "grandMother11PT") value = object->grandMother11PT;
  else if(variable == "grandMother11Phi") value = object->grandMother11Phi;
  else if(variable == "grandMother11Eta") value = object->grandMother11Eta;
  else if(variable == "charge") value = object->charge;
  else if(variable == "id") value = object->id;
  else if(variable == "status") value = object->status;
  else if(variable == "motherId") value = object->motherId;
  else if(variable == "motherCharge") value = object->motherCharge;
  else if(variable == "mother0Id") value = object->mother0Id;
  else if(variable == "mother0Status") value = object->mother0Status;
  else if(variable == "mother0Charge") value = object->mother0Charge;
  else if(variable == "mother1Id") value = object->mother1Id;
  else if(variable == "mother1Status") value = object->mother1Status;
  else if(variable == "mother1Charge") value = object->mother1Charge;
  else if(variable == "daughter0Id") value = object->daughter0Id;
  else if(variable == "daughter0Status") value = object->daughter0Status;
  else if(variable == "daughter0Charge") value = object->daughter0Charge;
  else if(variable == "daughter1Id") value = object->daughter1Id;
  else if(variable == "daughter1Status") value = object->daughter1Status;
  else if(variable == "daughter1Charge") value = object->daughter1Charge;
  else if(variable == "grandMotherId") value = object->grandMotherId;
  else if(variable == "grandMotherCharge") value = object->grandMotherCharge;
  else if(variable == "grandMother00Id") value = object->grandMother00Id;
  else if(variable == "grandMother00Status") value = object->grandMother00Status;
  else if(variable == "grandMother00Charge") value = object->grandMother00Charge;
  else if(variable == "grandMother01Id") value = object->grandMother01Id;
  else if(variable == "grandMother01Status") value = object->grandMother01Status;
  else if(variable == "grandMother01Charge") value = object->grandMother01Charge;
  else if(variable == "grandMother10Id") value = object->grandMother10Id;
  else if(variable == "grandMother10Status") value = object->grandMother10Status;
  else if(variable == "grandMother10Charge") value = object->grandMother10Charge;
  else if(variable == "grandMother11Id") value = object->grandMother11Id;
  else if(variable == "grandMother11Status") value = object->grandMother11Status;
  else if(variable == "grandMother11Charge") value = object->grandMother11Charge;


  //user-defined variables
  else if(variable == "genMatchedPdgGrandmotherId"){
    if(fabs(object->motherId) == 15){
      int motherIndex = findTauMotherIndex(object);
      if(motherIndex == -1) value = 0;
      else value = mcparticles->at(motherIndex).motherId;
    }
    else value = object->grandMotherId;
  }





  else if(variable == "genMatchedId") value = getPdgIdBinValue(object->id);
  else if(variable == "genMatchedMotherId") value = getPdgIdBinValue(object->motherId);
  else if(variable == "genMatchedMotherIdReverse") value = 24 - getPdgIdBinValue(object->motherId);
  else if(variable == "genMatchedGrandmotherId"){
    if(fabs(object->motherId) == 15){
      int motherIndex = findTauMotherIndex(object);
      if(motherIndex == -1) value = 0;
      else value = getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = getPdgIdBinValue(object->grandMotherId);
  }
  else if(variable == "genMatchedGrandmotherIdReverse"){
    if(fabs(object->motherId) == 15){
      int motherIndex = findTauMotherIndex(object);
      if(motherIndex == -1) value = 0;
      else value = 24 - getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = 24 - getPdgIdBinValue(object->grandMotherId);
  }



  else if (variable == "d0Vertex"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
      px = object->px,
      py = object->py,
      pt = object->pt;
    value = (-vx * py + vy * px) / pt;
  }
  else if (variable == "d0Beamspot"){
    double vx = object->vx - events->at(0).BSx,
      vy = object->vy - events->at(0).BSy,
      px = object->px,
      py = object->py,
      pt = object->pt;
    value = (-vx * py + vy * px) / pt;
  }
  else if (variable == "d0Origin"){
    double vx = object->vx,
      vy = object->vy,
      px = object->px,
      py = object->py,
      pt = object->pt;
    value = (-vx * py + vy * px) / pt;
  }

  else if (variable == "dz"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
      vz = object->vz - chosenVertex ()->z,
      px = object->px,
      py = object->py,
      pz = object->pz,
      pt = object->pt;
    value = vz - (vx * px + vy * py)/pt * (pz/pt);
  }
  else if(variable == "v0"){
    value = sqrt(object->vx*object->vx + object->vy*object->vy);
  }
  else if(variable == "deltaV0"){
    value = sqrt((object->vx-chosenVertex ()->x)*(object->vx-chosenVertex ()->x) + (object->vy-chosenVertex ()->y)*(object->vy-chosenVertex ()->y));
  }
  else if (variable == "deltaVx"){
    value = object->vx - chosenVertex ()->x;
  }
  else if (variable == "deltaVy"){
    value = object->vy - chosenVertex ()->y;
  }
  else if (variable == "deltaVz"){
    value = object->vz - chosenVertex ()->z;
  }

  else{clog << "WARNING: invalid mcparticle variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end mcparticle valueLookup

//!primaryvertex valueLookup
double
OSUAnalysis::valueLookup (const BNprimaryvertex* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "x") value = object->x;
  else if(variable == "xError") value = object->xError;
  else if(variable == "y") value = object->y;
  else if(variable == "yError") value = object->yError;
  else if(variable == "z") value = object->z;
  else if(variable == "zError") value = object->zError;
  else if(variable == "rho") value = object->rho;
  else if(variable == "normalizedChi2") value = object->normalizedChi2;
  else if(variable == "ndof") value = object->ndof;
  else if(variable == "isFake") value = object->isFake;
  else if(variable == "isValid") value = object->isValid;
  else if(variable == "tracksSize") value = object->tracksSize;
  else if(variable == "isGood") value = object->isGood;


  else{clog << "WARNING: invalid primaryvertex variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}

//!bxlumi valueLookup
double
OSUAnalysis::valueLookup (const BNbxlumi* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "bx_B1_now") value = object->bx_B1_now;
  else if(variable == "bx_B2_now") value = object->bx_B2_now;
  else if(variable == "bx_LUMI_now") value = object->bx_LUMI_now;


  else{clog << "WARNING: invalid bxlumi variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}

//!photon valueLookup
double
OSUAnalysis::valueLookup (const BNphoton* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "energy") value = object->energy;
  else if(variable == "et") value = object->et;
  else if(variable == "pt") value = object->pt;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "phi") value = object->phi;
  else if(variable == "eta") value = object->eta;
  else if(variable == "theta") value = object->theta;
  else if(variable == "trackIso") value = object->trackIso;
  else if(variable == "ecalIso") value = object->ecalIso;
  else if(variable == "hcalIso") value = object->hcalIso;
  else if(variable == "caloIso") value = object->caloIso;
  else if(variable == "trackIsoHollowConeDR03") value = object->trackIsoHollowConeDR03;
  else if(variable == "trackIsoSolidConeDR03") value = object->trackIsoSolidConeDR03;
  else if(variable == "ecalIsoDR03") value = object->ecalIsoDR03;
  else if(variable == "hcalIsoDR03") value = object->hcalIsoDR03;
  else if(variable == "caloIsoDR03") value = object->caloIsoDR03;
  else if(variable == "trackIsoHollowConeDR04") value = object->trackIsoHollowConeDR04;
  else if(variable == "trackIsoSolidConeDR04") value = object->trackIsoSolidConeDR04;
  else if(variable == "ecalIsoDR04") value = object->ecalIsoDR04;
  else if(variable == "hcalIsoDR04") value = object->hcalIsoDR04;
  else if(variable == "caloIsoDR04") value = object->caloIsoDR04;
  else if(variable == "hadOverEm") value = object->hadOverEm;
  else if(variable == "sigmaEtaEta") value = object->sigmaEtaEta;
  else if(variable == "sigmaIetaIeta") value = object->sigmaIetaIeta;
  else if(variable == "r9") value = object->r9;
  else if(variable == "scEnergy") value = object->scEnergy;
  else if(variable == "scRawEnergy") value = object->scRawEnergy;
  else if(variable == "scSeedEnergy") value = object->scSeedEnergy;
  else if(variable == "scEta") value = object->scEta;
  else if(variable == "scPhi") value = object->scPhi;
  else if(variable == "scZ") value = object->scZ;
  else if(variable == "genET") value = object->genET;
  else if(variable == "genPT") value = object->genPT;
  else if(variable == "genPhi") value = object->genPhi;
  else if(variable == "genEta") value = object->genEta;
  else if(variable == "genMotherET") value = object->genMotherET;
  else if(variable == "genMotherPT") value = object->genMotherPT;
  else if(variable == "genMotherPhi") value = object->genMotherPhi;
  else if(variable == "genMotherEta") value = object->genMotherEta;
  else if(variable == "eMax") value = object->eMax;
  else if(variable == "eLeft") value = object->eLeft;
  else if(variable == "eRight") value = object->eRight;
  else if(variable == "eTop") value = object->eTop;
  else if(variable == "eBottom") value = object->eBottom;
  else if(variable == "e3x3") value = object->e3x3;
  else if(variable == "swissCross") value = object->swissCross;
  else if(variable == "seedEnergy") value = object->seedEnergy;
  else if(variable == "seedTime") value = object->seedTime;
  else if(variable == "swissCrossNoI85") value = object->swissCrossNoI85;
  else if(variable == "swissCrossI85") value = object->swissCrossI85;
  else if(variable == "E2overE9NoI85") value = object->E2overE9NoI85;
  else if(variable == "E2overE9I85") value = object->E2overE9I85;
  else if(variable == "IDTight") value = object->IDTight;
  else if(variable == "IDLoose") value = object->IDLoose;
  else if(variable == "IDLooseEM") value = object->IDLooseEM;
  else if(variable == "genId") value = object->genId;
  else if(variable == "genCharge") value = object->genCharge;
  else if(variable == "genMotherId") value = object->genMotherId;
  else if(variable == "genMotherCharge") value = object->genMotherCharge;
  else if(variable == "isEB") value = object->isEB;
  else if(variable == "isEE") value = object->isEE;
  else if(variable == "isGap") value = object->isGap;
  else if(variable == "isEBEEGap") value = object->isEBEEGap;
  else if(variable == "isEBGap") value = object->isEBGap;
  else if(variable == "isEEGap") value = object->isEEGap;
  else if(variable == "hasPixelSeed") value = object->hasPixelSeed;
  else if(variable == "seedRecoFlag") value = object->seedRecoFlag;




  else if(variable == "genDeltaRLowest") value = getGenDeltaRLowest(object);

  else if(variable == "genMatchedPdgId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = mcparticles->at(index).id;
  }



  else if(variable == "genMatchedId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = getPdgIdBinValue(mcparticles->at(index).id);
  }
  else if(variable == "genMatchedMotherId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else value = getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genMatchedMotherIdReverse"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 24;
    else value = 24 -getPdgIdBinValue(mcparticles->at(index).motherId);
  }
  else if(variable == "genMatchedGrandmotherId"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 0;
    else if(fabs(mcparticles->at(index).motherId) == 15){
      int motherIndex = findTauMotherIndex(&mcparticles->at(index));
      if(motherIndex == -1) value = 0;
      else value = getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = getPdgIdBinValue(mcparticles->at(index).grandMotherId);
  }
  else if(variable == "genMatchedGrandmotherIdReverse"){
    int index = getGenMatchedParticleIndex(object);
    if(index == -1) value = 24;
    else if(fabs(mcparticles->at(index).motherId) == 15){
      int motherIndex = findTauMotherIndex(&mcparticles->at(index));
      if(motherIndex == -1) value = 24;
      else value = 24 - getPdgIdBinValue(mcparticles->at(motherIndex).motherId);
    }
    else value = 24 - getPdgIdBinValue(mcparticles->at(index).grandMotherId);
  }


  else{clog << "WARNING: invalid photon variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end photon valueLookup


//!supercluster valueLookup
double
OSUAnalysis::valueLookup (const BNsupercluster* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "energy") value = object->energy;
  else if(variable == "et") value = object->et;
  else if(variable == "ex") value = object->ex;
  else if(variable == "ey") value = object->ey;
  else if(variable == "ez") value = object->ez;
  else if(variable == "phi") value = object->phi;
  else if(variable == "eta") value = object->eta;
  else if(variable == "theta") value = object->theta;


  else{clog << "WARNING: invalid supercluster variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}

//!trigobj valueLookup
double
OSUAnalysis::valueLookup (const BNtrigobj* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "pt") value = object->pt;
  else if(variable == "eta") value = object->eta;
  else if(variable == "phi") value = object->phi;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "et") value = object->et;
  else if(variable == "energy") value = object->energy;
  else if(variable == "etTotal") value = object->etTotal;
  else if(variable == "id") value = object->id;
  else if(variable == "charge") value = object->charge;
  else if(variable == "isIsolated") value = object->isIsolated;
  else if(variable == "isMip") value = object->isMip;
  else if(variable == "isForward") value = object->isForward;
  else if(variable == "isRPC") value = object->isRPC;
  else if(variable == "bx") value = object->bx;
  else if(variable == "filter") {
    if ((stringValue = object->filter) == "")
      stringValue = "none";  // stringValue should only be empty if value is filled
  }

  else{clog << "WARNING: invalid trigobj variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}

//!muon-muon pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNmuon* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "alpha")
    {
      static const double pi = 3.1415926535897932384626433832795028841971693993751058;
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (pi-threeVector1.Angle(threeVector2));
    }
  else if(variable == "deltaCorrectedD0Vertex") value = object1->correctedD0Vertex - object2->correctedD0Vertex;
  else if(variable == "deltaAbsCorrectedD0Vertex") value = fabs(object1->correctedD0Vertex) - fabs(object2->correctedD0Vertex);
  else if(variable == "d0Sign"){
    double d0Sign = (object1->correctedD0Vertex*object2->correctedD0Vertex)/fabs(object1->correctedD0Vertex*object2->correctedD0Vertex);
    if(d0Sign < 0) value = -0.5;
    else if (d0Sign > 0) value = 0.5;
    else value = -999;
  }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else if(variable == "muon1CorrectedD0Vertex"){
    value = object1->correctedD0Vertex;
  }
  else if(variable == "muon2CorrectedD0Vertex"){
    value = object2->correctedD0Vertex;
  }
  else if(variable == "muon1timeAtIpInOut"){
    value = object1->timeAtIpInOut;
  }
  else if(variable == "muon2timeAtIpInOut"){
    value = object2->timeAtIpInOut;
  }
  else if(variable == "muon1CorrectedD0")
    {
      value = object1->correctedD0;
    }
  else if(variable == "muon2CorrectedD0")
    {
      value = object2->correctedD0;
    }

  else{clog << "WARNING: invalid muon-muon pair variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end muon-muon pair valueLookup


//!muon-photon pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNphoton* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "photonEta") value = object2->eta;
  else if(variable == "photonPt") value = object2->pt;
  else if(variable == "muonEta") value = object1->eta;
  else if(variable == "photonPhi") value = object2->phi;
  else if(variable == "muonPhi") value = object1->phi;
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "photonGenMotherId") value = object2->genMotherId;
  else if(variable == "muonRelPFdBetaIso") value = (object1->pfIsoR04SumChargedHadronPt + max(0.0, object1->pfIsoR04SumNeutralHadronEt + object1->pfIsoR04SumPhotonEt - 0.5*object1->pfIsoR04SumPUPt)) / object1->pt;
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "alpha")
    {
      static const double pi = 3.1415926535897932384626433832795028841971693993751058;
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (pi-threeVector1.Angle(threeVector2));
    }
  else{clog << "WARNING: invalid muon-photon pair variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end muon-photon pair valueLookup


//!electron-photon pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNphoton* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "photonEta") value = object2->eta;
  else if(variable == "photonPt") value = object2->pt;
  else if(variable == "electronEta") value = object1->eta;
  else if(variable == "photonPhi") value = object2->phi;
  else if(variable == "electronPhi") value = object1->phi;
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "photonGenMotherId") value = object2->genMotherId;
  else if(variable == "electronRelPFrhoIso") value = ( object1->chargedHadronIsoDR03 + max(0.0, object1->neutralHadronIsoDR03 + object1->photonIsoDR03 - object1->AEffDr03*object1->rhoPrime) ) / object1->pt;
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "alpha")
    {
      static const double pi = 3.1415926535897932384626433832795028841971693993751058;
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (pi-threeVector1.Angle(threeVector2));
    }
  else{clog << "WARNING: invalid electron-photon pair variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end electron-photon pair valueLookup


//!electron-electron pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNelectron* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "deltaCorrectedD0Vertex") value = object1->correctedD0Vertex - object2->correctedD0Vertex;
  else if(variable == "deltaAbsCorrectedD0Vertex") value = fabs(object1->correctedD0Vertex) - fabs(object2->correctedD0Vertex);
  else if(variable == "d0Sign"){
    double d0Sign = (object1->correctedD0Vertex*object2->correctedD0Vertex)/fabs(object1->correctedD0Vertex*object2->correctedD0Vertex);
    if(d0Sign < 0) value = -0.5;
    else if (d0Sign > 0) value = 0.5;
    else value = -999;
  }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else if(variable == "electron1CorrectedD0Vertex"){
    value = object1->correctedD0Vertex;
  }
  else if(variable == "electron2CorrectedD0Vertex"){
    value = object2->correctedD0Vertex;
  }
  else if(variable == "electron1CorrectedD0"){
    value = object1->correctedD0;
  }
  else if(variable == "electron2CorrectedD0"){
    value = object2->correctedD0;
  }

  else{clog << "WARNING: invalid electron-electron pair variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
} // end electron-electron pair valueLookup


//!electron-muon pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNmuon* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "deltaCorrectedD0Vertex") value = object1->correctedD0Vertex - object2->correctedD0Vertex;
  else if(variable == "deltaAbsCorrectedD0Vertex") value = fabs(object1->correctedD0Vertex) - fabs(object2->correctedD0Vertex);
  else if(variable == "d0Sign"){
    double d0Sign = (object1->correctedD0Vertex*object2->correctedD0Vertex)/fabs(object1->correctedD0Vertex*object2->correctedD0Vertex);
    if(d0Sign < 0) value = -0.5;
    else if (d0Sign > 0) value = 0.5;
    else value = -999;
  }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else if(variable == "electronCorrectedD0Vertex"){
    value = object1->correctedD0Vertex;
  }
  else if(variable == "muonCorrectedD0Vertex"){
    value = object2->correctedD0Vertex;
  }
  else if(variable == "electronCorrectedD0"){
    value = object1->correctedD0;
  }
  else if(variable == "muonCorrectedD0"){
    value = object2->correctedD0;
  }
  else if(variable == "electronCorrectedD0Err") value =  hypot (object1->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "electronCorrectedD0Sig") value =  object1->correctedD0 / hypot (object1->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "muonCorrectedD0Err") value =  hypot (object2->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "muonCorrectedD0Sig") value =  object2->correctedD0 / hypot (object2->tkD0err, hypot (events->at (0).BSxError, events->at (0).BSyError));
  else if(variable == "electronDetIso"){
    value = (object1->trackIso) / object1->pt;
  }
  else if(variable == "muonDetIso"){
    value = (object2->trackIsoDR03) / object2->pt;
  }
  else if(variable == "electronRelPFrhoIso"){
    value = ( object1->chargedHadronIsoDR03 + max(0.0, object1->neutralHadronIsoDR03 + object1->photonIsoDR03 - object1->AEffDr03*object1->rhoPrime) ) / object1->pt;
  }
  else if(variable == "muonRelPFdBetaIso"){
    value = (object2->pfIsoR04SumChargedHadronPt + max(0.0, object2->pfIsoR04SumNeutralHadronEt + object2->pfIsoR04SumPhotonEt - 0.5*object2->pfIsoR04SumPUPt)) / object2->pt;
  }
  else if(variable == "electronPt"){
    value = object1->pt;
  }
  else if(variable == "muonPt"){
    value = object2->pt;
  }
  else if(variable == "electronEta"){
    value = object1->eta;
  }
  else if(variable == "muonEta"){
    value = object2->eta;
  }
  else if(variable == "electronMetMT"){
    string dummy = "";
    value = valueLookup(object1,string("metMT"),string(""),dummy);
  }
  else if(variable == "muonMetMT"){
    string dummy = "";
    value = valueLookup(object2,string("metMT"),string(""),dummy);
  }
  else if(variable == "dz"){
    // based on definition of "dz" in DataFormats/TrackReco/interface/TrackBase.h
    value = object1->vz - object2->vz;
    value -= (object1->vx * object1->px + object1->vy * object1->py) / object1->pt * (object1->pz / object1->pt);
    value += (object2->vx * object2->px + object2->vy * object2->py) / object2->pt * (object2->pz / object2->pt);
  }
  else if(variable == "dzSig"){
    // based on definition of "dz" in DataFormats/TrackReco/interface/TrackBase.h
    value = object1->vz - object2->vz;
    value -= (object1->vx * object1->px + object1->vy * object1->py) / object1->pt * (object1->pz / object1->pt);
    value += (object2->vx * object2->px + object2->vy * object2->py) / object2->pt * (object2->pz / object2->pt);
    value /= hypot (object1->tkDZerr, object2->tkDZerr);
  }




  else{clog << "WARNING: invalid electron-muon pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
} // end electron-muon pair valueLookup


//!electron-jet pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNjet* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "distance")
   {
      Line::PositionType pos(GlobalPoint(object1->vx, object1->vy, object1->vz));
      Line::DirectionType dir(GlobalVector(object1->px, object1->py, object1->pz).unit());
      Line electron(pos, dir);
      Line::PositionType pos2(GlobalPoint(chosenVertex()->x, chosenVertex()->y, chosenVertex()->z));
      Line::DirectionType dir2(GlobalVector(object2->px, object2->py, object2->pz).unit());
      Line jet(pos2, dir2);
      value = (jet.distance(electron)).mag();
   }
  else if(variable == "jetEta") value = object2->eta;
  else if(variable == "jetPhi") value = object2->phi;
  else if(variable == "jetPt") value = object2->pt;
  else if(variable == "electronEta") value = object1->eta;
  else if(variable == "electronPhi") value = object1->phi;
  else if(variable == "electronPt") value = object1->pt;
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "deltaZ") value = object1->vz - object2->leadCandVz;
  else if(variable == "relPFrhoIso") value = ( object1->chargedHadronIsoDR03 + max(0.0, object1->neutralHadronIsoDR03 + object1->photonIsoDR03 - object1->AEffDr03*object1->rhoPrime) ) / object1->pt;
  else if(variable == "relPFrhoIsoNoPUSbtractedJet") value = (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt)) / object1->pt;
  else if(variable == "relPFrhoIsoJet") value = (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt - object1->AEffDr03*object1->rhoPrime)) / object1->pt;
  else if(variable == "PUInrelPFrhoIso") value = (object1->AEffDr03*object1->rhoPrime) / object1->pt;
  else if(variable == "ChargedHadInrelPFrhoIsoDiff") value = (object2->chargedHadronEnergyFraction*object2->pt - object1->chargedHadronIsoDR03) / object1->pt;
  else if(variable == "NeutralHadInrelPFrhoIsoDiff") value = (object2->neutralHadronEnergyFraction*object2->pt - object1->neutralHadronIsoDR03) / object1->pt;
  else if(variable == "PhotonInrelPFrhoIsoDiff") value = (object2->neutralEmEnergyFraction*object2->pt - object1->photonIsoDR03) / object1->pt;
  else if(variable == "ChargedHadInrelPFrhoIsoJet") value = (object2->chargedHadronEnergyFraction*object2->pt) / object1->pt;
  else if(variable == "NeutralHadInrelPFrhoIsoJet") value = (object2->neutralHadronEnergyFraction*object2->pt) / object1->pt;
  else if(variable == "PhotonInrelPFrhoIsoJet") value = (object2->neutralEmEnergyFraction*object2->pt) / object1->pt;
  else if(variable == "relPFrhoIsoDiff") value =  ( object1->chargedHadronIsoDR03 + max(0.0, object1->neutralHadronIsoDR03 + object1->photonIsoDR03 - object1->AEffDr03*object1->rhoPrime) ) / object1->pt - (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt - object1->AEffDr03*object1->rhoPrime)) / object1->pt;
  else if(variable == "relPFrhoIsoNoPUSubtractedDiff") value = ( object1->chargedHadronIsoDR03 + max(0.0, object1->neutralHadronIsoDR03 + object1->photonIsoDR03) ) / object1->pt - (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt)) / object1->pt;
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else if(variable == "ptOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNelectron, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else value = closestJet->pt;
  }
  else if(variable == "massOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNelectron, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else value = closestJet->mass;
  }
  else if(variable == "areaOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNelectron, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else value = closestJet->area;
  }
  else if(variable == "pTransverseToClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNelectron, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else{
      TVector3 jetP (closestJet->px, closestJet->py, closestJet->pz),
               leptonP (object1->px, object1->py, object1->pz),
               projection, rejection;
      projection = jetP.Unit ();
      projection *= (leptonP * projection);
      rejection = leptonP - projection;
      value = rejection.Mag ();
    }
  }

  else{clog << "WARNING: invalid electron-jet pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}

//!electron-mcparticle pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNmcparticle* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid electron-mcparticle variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}

//!track-mcparticle pair valueLookup
double
OSUAnalysis::valueLookup (const BNtrack* object1, const BNmcparticle* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if     (variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR")   value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid electron-mcparticle variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}


//!photon-jet pair valueLookup
double
OSUAnalysis::valueLookup (const BNphoton* object1, const BNjet* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "jetEta") value = object2->eta;
  else if(variable == "jetPhi") value = object2->phi;
  else if(variable == "photonEta") value = object1->eta;
  else if(variable == "photonPhi") value = object1->phi;
  else if(variable == "photonGenMotherId") value = object1->genMotherId;
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else{clog << "WARNING: invalid photon-jet pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}

// track-jet pair valueLookup
double
OSUAnalysis::valueLookup (const BNtrack* object1, const BNjet* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);

  else{clog << "WARNING: invalid track-jet pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;

}



// met-jet pair valueLookup
double
OSUAnalysis::valueLookup (const BNmet* object1, const BNjet* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));

  else{clog << "WARNING: invalid met-jet pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;

}



//!muon-jet pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNjet* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "jetEta") value = object2->eta;
  else if(variable == "distance")
   {
      Line::PositionType pos(GlobalPoint(object1->vx, object1->vy, object1->vz));
      Line::DirectionType dir(GlobalVector(object1->px, object1->py, object1->pz).unit());
      Line muon(pos, dir);
      Line::PositionType pos2(GlobalPoint(chosenVertex()->x, chosenVertex()->y, chosenVertex()->z));
      Line::DirectionType dir2(GlobalVector(object2->px, object2->py, object2->pz).unit());
      Line jet(pos2, dir2);
      value = (jet.distance(muon)).mag();
   }
  else if(variable == "relPFdBetaIso") value = (object1->pfIsoR04SumChargedHadronPt + max(0.0, object1->pfIsoR04SumNeutralHadronEt + object1->pfIsoR04SumPhotonEt - 0.5*object1->pfIsoR04SumPUPt)) / object1->pt;
  else if(variable == "relPFdBetaIsoNoPUSbtractedJet") value = (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt)) / object1->pt;
  else if(variable == "relPFdBetaIsoJet") value = (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt - 0.5*object1->pfIsoR04SumPUPt)) / object1->pt;
  else if(variable == "PUInrelPFdBetaIso") value = (0.5*object1->pfIsoR04SumPUPt) / object1->pt;
  else if(variable == "ChargedHadInrelPFdBetaIsoDiff") value = (object2->chargedHadronEnergyFraction*object2->pt - object1->pfIsoR04SumChargedHadronPt) / object1->pt;
  else if(variable == "NeutralHadInrelPFdBetaIsoDiff") value = (object2->neutralHadronEnergyFraction*object2->pt - object1->pfIsoR04SumNeutralHadronEt) / object1->pt;
  else if(variable == "PhotonInrelPFdBetaIsoDiff") value = (object2->neutralEmEnergyFraction*object2->pt - object1->pfIsoR04SumPhotonEt) / object1->pt;
  else if(variable == "ChargedHadInrelPFdBetaIsoJet") value = (object2->chargedHadronEnergyFraction*object2->pt) / object1->pt;
  else if(variable == "NeutralHadInrelPFdBetaIsoJet") value = (object2->neutralHadronEnergyFraction*object2->pt) / object1->pt;
  else if(variable == "PhotonInrelPFdBetaIsoJet") value = (object2->neutralEmEnergyFraction*object2->pt) / object1->pt;
  else if(variable == "relPFdBetaIsoDiff") value = (object1->pfIsoR04SumChargedHadronPt + max(0.0, object1->pfIsoR04SumNeutralHadronEt + object1->pfIsoR04SumPhotonEt - 0.5*object1->pfIsoR04SumPUPt))/ object1->pt - (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt- 0.5*object1->pfIsoR04SumPUPt)) / object1 -> pt;
  else if(variable == "relPFdBetaIsoNoPUSubtractedDiff") value =(object1->pfIsoR04SumChargedHadronPt + max(0.0, object1->pfIsoR04SumNeutralHadronEt + object1->pfIsoR04SumPhotonEt))/ object1->pt - (object2->chargedHadronEnergyFraction*object2->pt + max(0.0, object2->neutralHadronEnergyFraction*object2->pt + object2->neutralEmEnergyFraction*object2->pt)) / object1->pt;
  else if(variable == "jetPt") value = object2->pt;
  else if(variable == "jetPhi") value = object2->phi;
  else if(variable == "deltaPt") value = object1->pt - object2->pt;
  else if(variable == "muonEta") value = object1->eta;
  else if(variable == "muonPt") value = object1->pt;
  else if(variable == "muonPhi") value = object1->phi;
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "deltaZ") value = object1->vz - object2->leadCandVz;
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else if(variable == "ptOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNmuon, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else value = closestJet->pt;
  }
  else if(variable == "massOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNmuon, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else value = closestJet->mass;
  }
  else if(variable == "areaOfClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNmuon, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else value = closestJet->area;
  }
  else if(variable == "pTransverseToClosestJetByDeltaR"){
    const BNjet *closestJet = closest<BNmuon, BNjet> (object1, jets, "jets");
    if (!closestJet) value = -999;
    else{
      TVector3 jetP (closestJet->px, closestJet->py, closestJet->pz),
               leptonP (object1->px, object1->py, object1->pz),
               projection, rejection;
      projection = jetP.Unit ();
      projection *= (leptonP * projection);
      rejection = leptonP - projection;
      value = rejection.Mag ();
    }
  }

  else{clog << "WARNING: invalid muon-jet pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
} // end muon-jet pair valueLookup


//!muon-event valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNevent* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "muonEta") value = object1->eta;
  else if(variable == "muonPt") value = object1->pt;
  else if(variable == "muonPhi") value = object1->phi;
  else if(variable == "Ht") value = getHt(jets.product());
  else if(variable == "pthat")   value = object2->pthat;
  else if(variable == "numPV")   value = object2->numPV;
  else if(variable == "correctedD0")   value = object1->correctedD0;
  else if(variable == "relPFdBetaIso") value = (object1->pfIsoR04SumChargedHadronPt + max(0.0, object1->pfIsoR04SumNeutralHadronEt + object1->pfIsoR04SumPhotonEt - 0.5*object1->pfIsoR04SumPUPt)) / object1->pt;
  else{clog << "WARNING: invalid muon-event pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}
//!electron-event valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNevent* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if     (variable == "numPV")       value = object2->numPV;
  else if(variable == "correctedD0") value = object1->correctedD0;
  else{clog << "WARNING: invalid electron-event pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}
//!jet-jet pair valueLookup
double
OSUAnalysis::valueLookup (const BNjet* object1, const BNjet* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaPt") value = object1->pt - object2->pt;
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "pt"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).Pt();
  }
  else if(variable == "threeDAngle")
    {
      TVector3 threeVector1(object1->px, object1->py, object1->pz);
      TVector3 threeVector2(object2->px, object2->py, object2->pz);
      value = (threeVector1.Angle(threeVector2));
    }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid jet-jet pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);

  return value;
}

//!electron-track pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNtrack* object2, string variable, string function, string &stringValue){
  double electronMass = 0.000511;
  double value = 0.0;
  TLorentzVector fourVector1(0, 0, 0, 0);
  TLorentzVector fourVector2(0, 0, 0, 0);
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "deltaRLooseMvaId") {
    // only consider electrons that pass "loose MVA id", i.e., mvaNonTrigV0 > 0;
    // otherwise return very large value (99)
    if (object1->mvaNonTrigV0>0) value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
    else                         value = 99.;
  }
  else if(variable == "invMass"){
    fourVector1.SetPtEtaPhiM(object1->pt, object1->eta, object1->phi, electronMass);
    fourVector2.SetPtEtaPhiM(object2->pt, object2->eta, object2->phi, electronMass );

    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }
  else{clog << "WARNING: invalid electron-track pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);
  return value;

}


//!muon-track pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNtrack* object2, string variable, string function, string &stringValue){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaPt")  value = fabs(object1->pt - object2->pt);  
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "deltaRLooseID") {
    // only consider muons that pass looseId criteria; otherwise return very large value (99.)
    string empty = "";
    double isLooseId = valueLookup(object1, "looseID", "", empty);
    if (isLooseId) value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
    else           value = 99.;
  }
  else if(variable == "deltaRGlobalMuon") {
    // only consider muons that pass looseId criteria; otherwise return very large value (99.)
    if (object1->isGlobalMuon) value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
    else                       value = 99.;
  }
  else if(variable == "invMass"){
    double pionMass = 0.140;
    double muonMass = 0.106;
    TLorentzVector fourVector1(0, 0, 0, 0);
    TLorentzVector fourVector2(0, 0, 0, 0);
    fourVector1.SetPtEtaPhiM(object1->pt, object1->eta, object1->phi, muonMass);
    fourVector2.SetPtEtaPhiM(object2->pt, object2->eta, object2->phi, pionMass );
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid muon-track pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);
  return value;
}

//!tau-tau pair valueLookup
double
OSUAnalysis::valueLookup (const BNtau* object1, const BNtau* object2, string variable, string function, string &stringValue){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }

  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid tau-tau pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);
  return value;
}

//!muon-tau pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNtau* object2, string variable, string function, string &stringValue){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "invMass"){
    TLorentzVector fourVector1(object1->px, object1->py, object1->pz, object1->energy);
    TLorentzVector fourVector2(object2->px, object2->py, object2->pz, object2->energy);
    value = (fourVector1 + fourVector2).M();
  }

  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid muon-tau pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);
  return value;
}

//!tau-track pair valueLookup
double
OSUAnalysis::valueLookup (const BNtau* object1, const BNtrack* object2, string variable, string function, string &stringValue){
  double value = 0.0;
  if(variable == "deltaPhi") value = fabs(deltaPhi(object1->phi,object2->phi));
  else if(variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if(variable == "deltaRLooseHadronicID") {
    // only consider tau's that pass the loose hadronic ID criteria; otherwise return very large value (99)
    string empty = "";
    double isLooseHadronicID = valueLookup(object1, "looseHadronicID", "", empty);

    if (isLooseHadronicID) value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
    else                   value = 99.;
  }
  else if(variable == "deltaEta") value = fabs(object1->eta - object2->eta);
  else if(variable == "invMass"){
    double tauMass = 1.777;  // PDG 2010
    double pionMass = 0.140; // PDG 2010
    TLorentzVector fourVector1(0, 0, 0, 0);
    TLorentzVector fourVector2(0, 0, 0, 0);
    fourVector1.SetPtEtaPhiM(object1->pt, object1->eta, object1->phi, tauMass);
    fourVector2.SetPtEtaPhiM(object2->pt, object2->eta, object2->phi, pionMass );
    value = (fourVector1 + fourVector2).M();
  }
  else if(variable == "chargeProduct"){
    value = object1->charge*object2->charge;
  }

  else{clog << "WARNING: invalid tau-track pair variable '" << variable << "'\n"; value = -999;}
  value = applyFunction(function, value);
  return value;
}


//!track-event pair valueLookup
double
OSUAnalysis::valueLookup (const BNtrack* object1, const BNevent* object2, string variable, string function, string &stringValue){

  double value = 0.0;
  double pMag = sqrt(object1->pt * object1->pt +
                     object1->pz * object1->pz);

  if      (variable == "numPV")                      value = object2->numPV;
  else if (variable == "rhoCorrRp5")                 value = getRhoCorr(object1);
  else if (variable == "caloTotDeltaRp5")            value =  (object1->caloHadDeltaRp5 + object1->caloEMDeltaRp5);
  else if (variable == "caloTotDeltaRp5ByP")         value = ((object1->caloHadDeltaRp5 + object1->caloEMDeltaRp5)/pMag);
  else if (variable == "caloTotDeltaRp5_RhoCorr")    value = getTrkCaloTotRhoCorr(object1);
  else if (variable == "caloTotDeltaRp5ByP_RhoCorr") value = getTrkCaloTotRhoCorr(object1) / pMag;

  else { clog << "WARNING: invalid track-event pair variable '" << variable << "'\n"; value = -999; }

  value = applyFunction(function, value);

  return value;

}

//!electron-trigobj pair valueLookup
double
OSUAnalysis::valueLookup (const BNelectron* object1, const BNtrigobj* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if (variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);
  else if (variable == "match"){
    if (deltaR(object1->eta,object1->phi,object2->eta,object2->phi) < 0.2 && abs(object2->id) == 11)
      stringValue = object2->filter;
    else
      stringValue = "none";
  }

  else { clog << "WARNING: invalid electron-trigobj variable '" << variable << "'\n"; value = -999; }

  value = applyFunction(function, value);

  return value;

}

//!muon-trigobj pair valueLookup
double
OSUAnalysis::valueLookup (const BNmuon* object1, const BNtrigobj* object2, string variable, string function, string &stringValue){

  double value = 0.0;

  if (variable == "deltaR") value = deltaR(object1->eta,object1->phi,object2->eta,object2->phi);

  else { clog << "WARNING: invalid muon-trigobj variable '" << variable << "'\n"; value = -999; }

  value = applyFunction(function, value);

  return value;

}

//!stop valueLookup
double
OSUAnalysis::valueLookup (const BNstop* object, string variable, string function, string &stringValue){


  double value = 0.0;

  if(variable == "ctau") value = object->ctau;

  else if (variable == "d0"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
      px = object->px,
      py = object->py,
      pt = object->pt;
    value = (-vx * py + vy * px) / pt;
  }

  else if (variable == "dz"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
      vz = object->vz - chosenVertex ()->z,
      px = object->px,
      py = object->py,
      pz = object->pz,
      pt = object->pt;
    value = vz - (vx * px + vy * py)/pt * (pz/pt);
  }

  else if (variable == "minD0"){
    double minD0=999;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      double vx = object->vx - vertex->x,
        vy = object->vy - vertex->y,
        px = object->px,
        py = object->py,
        pt = object->pt;
      value = (-vx * py + vy * px) / pt;
      if(abs(value) < abs(minD0)) minD0 = value;
    }
    value = minD0;
  }
  else if (variable == "minDz"){
    double minDz=999;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      double vx = object->vx - vertex->x,
        vy = object->vy - vertex->y,
        vz = object->vz - vertex->z,
        px = object->px,
        py = object->py,
        pz = object->pz,
        pt = object->pt;
      value = vz - (vx * px + vy * py)/pt * (pz/pt);
      if(abs(value) < abs(minDz)) minDz = value;
    }
    value = minDz;
  }
  else if(variable == "distToVertex"){
    value = sqrt((object->vx-chosenVertex()->x)*(object->vx-chosenVertex()->x) + \
                 (object->vy-chosenVertex()->y)*(object->vy-chosenVertex()->y) + \
                 (object->vz-chosenVertex()->z)*(object->vz-chosenVertex()->z));
  }
  else if (variable == "minDistToVertex"){
    double minDistToVertex=999;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      value = sqrt((object->vx-vertex->x)*(object->vx-vertex->x) + \
                   (object->vy-vertex->y)*(object->vy-vertex->y) + \
                   (object->vz-vertex->z)*(object->vz-vertex->z));

      if(abs(value) < abs(minDistToVertex)) minDistToVertex = value;
    }
    value = minDistToVertex;
  }
  else if (variable == "distToVertexDifference"){
    double minDistToVertex=999;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      value = sqrt((object->vx-vertex->x)*(object->vx-vertex->x) + \
                   (object->vy-vertex->y)*(object->vy-vertex->y) + \
                   (object->vz-vertex->z)*(object->vz-vertex->z));

      if(abs(value) < abs(minDistToVertex)) minDistToVertex = value;
    }
    double distToChosenVertex = sqrt((object->vx-chosenVertex()->x)*(object->vx-chosenVertex()->x) + \
                                     (object->vy-chosenVertex()->y)*(object->vy-chosenVertex()->y) + \
                                     (object->vz-chosenVertex()->z)*(object->vz-chosenVertex()->z));

    value = distToChosenVertex - minDistToVertex;
  }

  else if (variable == "closestVertexRank"){
    double minDistToVertex=999;
    int vertex_rank = 0;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      vertex_rank++;
      int dist = sqrt((object->vx-vertex->x)*(object->vx-vertex->x) + \
                      (object->vy-vertex->y)*(object->vy-vertex->y) + \
                      (object->vz-vertex->z)*(object->vz-vertex->z));

      if(abs(dist) < abs(minDistToVertex)){
        value = vertex_rank;
        minDistToVertex = dist;
      }
    }
  }

  else if (variable == "decaysToTau"){
    value = abs (object->daughter0Id) == 15 || abs (object->daughter1Id) == 15;
  }




  else { clog << "WARNING: invalid stop variable '" << variable << "'\n"; value = -999; }

  value = applyFunction(function, value);

  return value;

} // end stop valueLookup





// Calculate the number of tracks in cone of DeltaR<0.5 around track1.
// Return true iff no other tracks are found in this cone.
int
OSUAnalysis::getTrkIsIso (const BNtrack* track1, const BNtrackCollection* trackColl){
  for(BNtrackCollection::const_iterator track2 = trackColl->begin(); track2 !=trackColl->end(); track2++){
    if(track1->eta == track2->eta && track1->phi == track2->phi) continue; // Do not compare the track to itself.
    double deltaRtrk = deltaR(track1->eta, track1->phi, track2->eta, track2->phi);
    if(deltaRtrk < 0.5) return 0;
  }
  return 1;

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
//calculate the scalar sum of Jet Pt in the event.
double
OSUAnalysis::getHt (const BNjetCollection* jetColl){
  double Ht = 0;
  for(BNjetCollection::const_iterator jet = jetColl->begin(); jet !=jetColl->end(); jet++){
    Ht += abs(jet->pt);
  }
  return Ht;
}


unsigned int OSUAnalysis::GetNumExtraPartons(const BNmcparticleCollection* genPartColl){

  int nList = 0;
  unsigned int nPartons = 0;
  for( BNmcparticleCollection::const_iterator mcparticle = genPartColl->begin(); mcparticle != genPartColl->end(); mcparticle++)
   {
     nList ++;
    int id = mcparticle->id;
    int motherID = mcparticle->motherId;
    int status = mcparticle->status;
    int aid = abs(id);

    if(status == 3){//only status 3 particles (which are listed first)
      if(nList>6){//dont look at first 6 (incomming event)
        if( (aid>0 && aid<6) || aid ==21 || aid ==9){//udscb gluon
          if(abs(motherID) !=23 && abs(motherID) !=24 && abs(motherID) != 25 && abs(motherID)!=6 ){ //not from WZHt
            nPartons++;
          }
        }
      }
    }else{
      break; //break when you run out of stat==3
    }
  }

  return nPartons;

}

//calculate the scalar sum of Electron, Muon, and Jet Pt in the event.
double
OSUAnalysis::getSt (const BNelectronCollection* electronColl, const BNmuonCollection* muonColl, const BNjetCollection* jetColl){
  double St = 0;
  if (!electronColl || !muonColl || !jetColl) return -999.0;
  for(BNelectronCollection::const_iterator electron = electronColl->begin(); electron !=electronColl->end(); electron++){
    St += abs(electron->pt);
  }
  for(BNmuonCollection::const_iterator muon = muonColl->begin(); muon !=muonColl->end(); muon++){
    St += abs(muon->pt);
  }
  for(BNjetCollection::const_iterator jet = jetColl->begin(); jet !=jetColl->end(); jet++){
    St += abs(jet->pt);
  }

  return St;
}


double
OSUAnalysis::getTrkPtRes (const BNtrack* track1){

  double ptTrue = getTrkPtTrue(track1, mcparticles.product());
  double PtRes = (track1->pt - ptTrue) / ptTrue;

  return PtRes;

}


double
OSUAnalysis::getTrkPtTrue (const BNtrack* track1, const BNmcparticleCollection* genPartColl){
  double value = -99;
  double genDeltaRLowest = 999;

  for (BNmcparticleCollection::const_iterator genPart = genPartColl->begin(); genPart !=genPartColl->end(); genPart++){
    double genDeltaRtemp = deltaR(genPart->eta, genPart->phi,track1->eta, track1->phi);
    if (genDeltaRtemp < genDeltaRLowest) {
      genDeltaRLowest = genDeltaRtemp;
      if (genDeltaRLowest < 0.15) {   // Only consider it truth-matched if DeltaR<0.15.
        double ptTrue = genPart->pt;
        value = ptTrue;
      }
    }
  }

  return value;

}

double
OSUAnalysis::getTrkCaloTotRhoCorr(const BNtrack* track) {
  // Return the pile-up (rho) corrected isolation energy, i.e., the total calorimeter energy around the candidate track.
  if (!useTrackCaloRhoCorr_) return -99;
  // if (!rhokt6CaloJetsHandle_) {
  //   clog << "ERROR [getTrkCaloTotRhoCorr]:  The collection rhokt6CaloJetsHandle is not available!" << endl;
  //   return -99;
  // }
  double radDeltaRCone = 0.5;
  double rhoCorr_kt6CaloJets = *rhokt6CaloJetsHandle_ * TMath::Pi() * pow(radDeltaRCone, 2);  // Define effective area as pi*r^2, where r is radius of DeltaR cone.
  double rawCaloTot = track->caloHadDeltaRp5 + track->caloEMDeltaRp5;
  double caloTotRhoCorrCalo = TMath::Max(0., rawCaloTot - rhoCorr_kt6CaloJets);
  return caloTotRhoCorrCalo;

}

double
OSUAnalysis::getRhoCorr(const BNtrack* track) {
  // Return the pile-up (rho) corrected isolation energy, i.e., the total calorimeter energy around the candidate track.                                                                                  
  if (!useTrackCaloRhoCorr_) return -99;
  // if (!rhokt6CaloJetsHandle_) {                                                                                                                                                                        
  //   clog << "ERROR [getTrkCaloTotRhoCorr]:  The collection rhokt6CaloJetsHandle is not available!" << endl;                                                                                            
  //   return -99;                                                                                                                                                                                        
  // }                                                                                                                                                                                                    
  double radDeltaRCone = 0.5;
  double rhoCorr_kt6CaloJets = *rhokt6CaloJetsHandle_ * TMath::Pi() * pow(radDeltaRCone, 2);  // Define effective area as pi*r^2, where r is radius of DeltaR cone.                                       
  return rhoCorr_kt6CaloJets;

}


double
OSUAnalysis::getTrkDepTrkRp5RhoCorr(const BNtrack* track) {
  // Return the pile-up (rho) corrected isolation energy, i.e., the total calorimeter energy around the candidate track.
  if (!useTrackCaloRhoCorr_) return -99;
  // if (!rhokt6CaloJetsHandle_) {
  //   clog << "ERROR [getTrkCaloTotRhoCorr]:  The collection rhokt6CaloJetsHandle is not available!" << endl;
  //   return -99;
  // }
  double radDeltaRCone = 0.5;
  double rhoCorr_kt6CaloJets = *rhokt6CaloJetsHandle_ * TMath::Pi() * pow(radDeltaRCone, 2);  // Define effective area as pi*r^2, where r is radius of DeltaR cone.
  double rawDepTrkRp5 = track->depTrkRp5;
 double depTrkRp5RhoCorr = TMath::Max(0., rawDepTrkRp5 - rhoCorr_kt6CaloJets);
 return depTrkRp5RhoCorr;

}

double
OSUAnalysis::getTrkDepTrkRp3RhoCorr(const BNtrack* track) {
  // Return the pile-up (rho) corrected isolation energy, i.e., the total calorimeter energy around the candidate track.
  if (!useTrackCaloRhoCorr_) return -99;
  // if (!rhokt6CaloJetsHandle_) {
  //   clog << "ERROR [getTrkCaloTotRhoCorr]:  The collection rhokt6CaloJetsHandle is not available!" << endl;
  //   return -99;
  // }
  double radDeltaRCone = 0.3;
// Define effective area as pi*r^2, where r is radius of DeltaR cone
  double rhoCorr_kt6CaloJets = *rhokt6CaloJetsHandle_ * TMath::Pi() * pow(radDeltaRCone, 2);
  double rawDepTrkRp3 = track->depTrkRp3;
  double depTrkRp3RhoCorr = TMath::Max(0., rawDepTrkRp3 - rhoCorr_kt6CaloJets);
  return depTrkRp3RhoCorr;

}




//creates a map of the dead Ecal channels in the barrel and endcap
//to see how the map of dead Ecal channels is created look at function getChannelStatusMaps() here:
//http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/jbrinson/DisappTrk/OSUT3Analysis/AnaTools/src/OSUAnalysis.cc?revision=1.88&view=markup
void
OSUAnalysis::WriteDeadEcal (){
  double etaEcal, phiEcal;
  ifstream DeadEcalFile(deadEcalFile_);
  if(!DeadEcalFile) {
    clog << "Error: DeadEcalFile has not been found." << endl;
    return;
  }
  if(DeadEcalVec.size()!= 0){
    clog << "Error: DeadEcalVec has a nonzero size" << endl;
    return;
  }
  while(!DeadEcalFile.eof())
    {
      DeadEcalFile >> etaEcal >> phiEcal;
      DeadEcal newChan;
      newChan.etaEcal = etaEcal;
      newChan.phiEcal = phiEcal;
      DeadEcalVec.push_back(newChan);
    }
  if(DeadEcalVec.size() == 0) clog << "Warning: No dead Ecal channels have been found." << endl;
}



// Creates a map of the bad CSC chambers.  
// The list of bad CSC chambers is taken from:  
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/CSCDPGConditions#CSC_bad_chambers_in_2012_30_05_2
void
OSUAnalysis::WriteBadCSC() {
  double etaCSC, phiCSC;
  ifstream BadCSCFile(badCSCFile_);
  if (!BadCSCFile) {
    clog << "Error: BadCSCFile has not been found." << endl;
    return;
  }
  if (BadCSCVec.size()!= 0) {
    clog << "Error: BadCSCVec has a nonzero size" << endl;
    return;
  }
  while (!BadCSCFile.eof()) {
    BadCSCFile >> etaCSC >> phiCSC;
    BadCSC newChan;
    newChan.etaCSC = etaCSC;
    newChan.phiCSC = phiCSC;
    BadCSCVec.push_back(newChan);
    //    clog << "Debug:  Adding bad CSC with eta=" << etaCSC << ", phi=" << phiCSC << endl;  
  }
  if (BadCSCVec.size() == 0) clog << "Warning: No bad CSC chambers have been found." << endl;
}  


//if a track is found within dR<0.05 of a dead Ecal channel value = 1, otherwise value = 0
int
OSUAnalysis::getTrkIsMatchedDeadEcal (const BNtrack* track1){
  int value = 0;
  if (getTrkDeadEcalDeltaR(track1)<0.05) {value = 1;}
  else {value = 0;}
  return value;
}

double
OSUAnalysis::getTrkDeadEcalDeltaR (const BNtrack* track1){
  double deltaRLowest = 999;
  if (DeadEcalVec.size() == 0) WriteDeadEcal();
  for(vector<DeadEcal>::const_iterator ecal = DeadEcalVec.begin(); ecal != DeadEcalVec.end(); ++ecal){
    double eta = ecal->etaEcal;
    double phi = ecal->phiEcal;
    double deltaRtemp = deltaR(eta, phi, track1->eta, track1->phi);
    if(deltaRtemp < deltaRLowest) deltaRLowest = deltaRtemp;
  }
  if (verbose_) clog << deltaRLowest << endl;
  return deltaRLowest;
}



// If a track is found within dR<0.25 of a bad CSC chamber, value = 1, otherwise value = 0.  
// FIXME:  Instead of a deltaR cut, it would be better to use the boundaries of the chamber 
int
OSUAnalysis::getTrkIsMatchedBadCSC (const BNtrack* track1){
  double deltaRLowest = 999;
  int value = 0;
  if (BadCSCVec.size() == 0) WriteBadCSC();
  for(vector<BadCSC>::const_iterator csc = BadCSCVec.begin(); csc != BadCSCVec.end(); ++csc) {
    double eta = csc->etaCSC;
    double phi = csc->phiCSC;
    double deltaRtemp = deltaR(eta, phi, track1->eta, track1->phi);
    if(deltaRtemp < deltaRLowest) deltaRLowest = deltaRtemp;
  }
  if (deltaRLowest<0.25) { value = 1; }
  else                  { value = 0; } 
  return value;
}


// Returns the smallest DeltaR between the object and any generated true particle in the event.
template <class InputObject>
double OSUAnalysis::getGenDeltaRLowest(InputObject object){
  double genDeltaRLowest = 999.;
  for(BNmcparticleCollection::const_iterator mcparticle = mcparticles->begin (); mcparticle != mcparticles->end (); mcparticle++){
    double deltaRtemp = deltaR(mcparticle->eta, mcparticle->phi, object->eta, object->phi);
    if (deltaRtemp < genDeltaRLowest) genDeltaRLowest = deltaRtemp;
  }
  return genDeltaRLowest;
}

double
OSUAnalysis::applyFunction(string function, double value){

  if(function == "abs") value = fabs(value);
  else if(function == "fabs") value = fabs(value);
  else if(function == "log10") value = log10(value);
  else if(function == "log") value = log10(value);

  else if(function == "") value = value;
  else{clog << "WARNING: invalid function '" << function << "'\n";}

  return value;

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

  bool sameObjects = false;
  if(typeid(InputCollection1).name() == typeid(InputCollection2).name()) sameObjects = true;  // FIXME:  is sameObjects just the not of isTwoTypesOfObject?  If so, it's redundant.

  // Get the strings for the two objects that make up the pair.
  string obj1Type, obj2Type;
  getTwoObjs(inputType, obj1Type, obj2Type);
  bool isTwoTypesOfObject = true;
  if (obj1Type==obj2Type) isTwoTypesOfObject = false;

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

      if(sameObjects && object1 >= object2) continue;//account for duplicate pairs if both collections are the same


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
void OSUAnalysis::fill1DHistogram(TH1* histo, histogram parameters, InputCollection inputCollection, flagPair flags, double scaleFactor){

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
                                  flagPair pairFlags, double scaleFactor){

  bool sameObjects = false;
  if(typeid(InputCollection1).name() == typeid(InputCollection2).name()) sameObjects = true;

  int pairCounter = -1;
  for (uint object1 = 0; object1 != inputCollection1->size(); object1++){
    for (uint object2 = 0; object2 != inputCollection2->size(); object2++){

      if(sameObjects && object1 >= object2) continue;//account for duplicate pairs if both collections are the same

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
void OSUAnalysis::fill2DHistogram(TH2* histo, histogram parameters, InputCollection inputCollection, flagPair flags, double scaleFactor){

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
                                  flagPair pairFlags, double scaleFactor){

  bool sameObjects = false;
  if(typeid(InputCollection1).name() == typeid(InputCollection2).name()) sameObjects = true;

  int pairCounter = -1;
  for (uint object1 = 0; object1 != inputCollection1->size(); object1++){
    for (uint object2 = 0; object2 != inputCollection2->size(); object2++){

      if(sameObjects && object1 >= object2) continue;//account for duplicate pairs if both collections are the same

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

template <class InputObject>
int OSUAnalysis::getGenMatchedParticleIndex(InputObject object){

  int bestMatchIndex = -1;
  double bestMatchDeltaR = 999;

  for(BNmcparticleCollection::const_iterator mcparticle = mcparticles->begin (); mcparticle != mcparticles->end (); mcparticle++){

    double currentDeltaR = deltaR(object->eta,object->phi,mcparticle->eta,mcparticle->phi);
    if(currentDeltaR > 0.05) continue;
    //     clog << setprecision(3) << setw(20)
    //          << "\tcurrentParticle:  eta = " << mcparticles->at(mcparticle - mcparticles->begin()).eta
    //          << setw(20)
    //          << "\tphi = " << mcparticles->at(mcparticle - mcparticles->begin()).phi
    //          << setw(20)
    //          << "\tdeltaR = " << currentDeltaR
    //          << setprecision(1)
    //          << setw(20)
    //          << "\tid = " << mcparticles->at(mcparticle - mcparticles->begin()).id
    //          << setw(20)
    //          << "\tmotherId = " << mcparticles->at(mcparticle - mcparticles->begin()).motherId
    //          << setw(20)
    //          << "\tstatus = " << mcparticles->at(mcparticle - mcparticles->begin()).status<< endl;
    if(currentDeltaR < bestMatchDeltaR && mcparticles->at(mcparticle - mcparticles->begin()).id != mcparticles->at(mcparticle - mcparticles->begin()).motherId){
      bestMatchIndex = mcparticle - mcparticles->begin();
      bestMatchDeltaR = currentDeltaR;
    }

  }
  //   if(bestMatchDeltaR != 999)  clog << "bestMatch:  deltaR = " << bestMatchDeltaR << "   id = " << mcparticles->at(bestMatchIndex).id << "   motherId = " << mcparticles->at(bestMatchIndex).motherId << endl;
  //   else clog << "no match found..." << endl;
  return bestMatchIndex;

}


int OSUAnalysis::findTauMotherIndex(const BNmcparticle* tau){

  int bestMatchIndex = -1;
  double bestMatchDeltaR = 999;

  for(BNmcparticleCollection::const_iterator mcparticle = mcparticles->begin (); mcparticle != mcparticles->end (); mcparticle++){

    if(fabs(mcparticle->id) != 15 || mcparticle->status !=3) continue;

    double currentDeltaR = deltaR(tau->eta,tau->phi,mcparticle->eta,mcparticle->phi);
    if(currentDeltaR > 0.05) continue;

    if(currentDeltaR < bestMatchDeltaR && mcparticles->at(mcparticle - mcparticles->begin()).id != mcparticles->at(mcparticle - mcparticles->begin()).motherId){
      bestMatchIndex = mcparticle - mcparticles->begin();
      bestMatchDeltaR = currentDeltaR;
    }

  }

  return bestMatchIndex;
}


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

int OSUAnalysis::getPdgIdBinValue(int pdgId){

  int binValue = -999;
  int absPdgId = fabs(pdgId);
  if(pdgId == -1) binValue = 0;
  else if(absPdgId <= 6 ) binValue = absPdgId;
  else if(absPdgId == 11 ) binValue = 7;
  else if(absPdgId == 13 ) binValue = 8;
  else if(absPdgId == 15 ) binValue = 9;
  else if(absPdgId == 12 || absPdgId == 14 || absPdgId == 16 ) binValue = 10;
  else if(absPdgId == 21 ) binValue = 11;
  else if(absPdgId == 22 ) binValue = 12;
  else if(absPdgId == 23 ) binValue = 13;
  else if(absPdgId == 24 ) binValue = 14;
  else if(absPdgId > 100 && absPdgId < 300 && absPdgId != 130  ) binValue = 15;
  else if( absPdgId == 130 || (absPdgId > 300 && absPdgId < 400)  ) binValue = 16;
  else if(absPdgId > 400 && absPdgId < 500  ) binValue = 17;
  else if(absPdgId > 500 && absPdgId < 600  ) binValue = 18;
  else if(absPdgId > 1000 && absPdgId < 3000  ) binValue = 19;
  else if(absPdgId > 3000 && absPdgId < 4000  ) binValue = 20;
  else if(absPdgId > 4000 && absPdgId < 5000  ) binValue = 21;
  else if(absPdgId > 5000 && absPdgId < 6000  ) binValue = 22;
  else if(absPdgId == 92  ) binValue = 23;

  else binValue = 24;

  return binValue;

}

const BNprimaryvertex *
OSUAnalysis::chosenVertex ()
{
  const BNprimaryvertex *chosenVertex = 0;
  if(cumulativeFlags.find ("primaryvertexs") != cumulativeFlags.end ()){
    flagPair vertexFlags;
    for (int i = cumulativeFlags.at("primaryvertexs").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("primaryvertexs").at(i).size()){
        vertexFlags = cumulativeFlags.at("primaryvertexs").at(i);
        break;
      }
    }
    for (uint vertexIndex = 0; vertexIndex != vertexFlags.size(); vertexIndex++){
      if(!vertexFlags.at(vertexIndex).first) continue;
      chosenVertex = & primaryvertexs->at(vertexIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "primaryvertexs") != objectsToGet.end ())
    chosenVertex = & primaryvertexs->at (0);

  return chosenVertex;
}

const BNmet *
OSUAnalysis::chosenMET ()
{
  const BNmet *chosenMET = 0;
  if(cumulativeFlags.find ("mets") != cumulativeFlags.end ()){
    flagPair metFlags;
    for (int i = cumulativeFlags.at("mets").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("mets").at(i).size()){
        metFlags = cumulativeFlags.at("mets").at(i);
        break;
      }
    }
    for (uint metIndex = 0; metIndex != metFlags.size(); metIndex++){
      if(!metFlags.at(metIndex).first) continue;
      chosenMET = & mets->at(metIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "mets") != objectsToGet.end ())
    chosenMET = & mets->at (0);

  return chosenMET;
}

const BNelectron *
OSUAnalysis::chosenElectron ()
{
  const BNelectron *chosenElectron = 0;
  if(cumulativeFlags.find ("electrons") != cumulativeFlags.end ()){
    flagPair electronFlags;
    for (int i = cumulativeFlags.at("electrons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("electrons").at(i).size()){
        electronFlags = cumulativeFlags.at("electrons").at(i);
        break;
      }
    }
    for (uint electronIndex = 0; electronIndex != electronFlags.size(); electronIndex++){
      if(!electronFlags.at(electronIndex).first) continue;
      chosenElectron = & electrons->at(electronIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "electrons") != objectsToGet.end ())
    chosenElectron = & electrons->at (0);

  return chosenElectron;
}


const BNmuon *
OSUAnalysis::chosenMuon ()
{
  const BNmuon *chosenMuon = 0;
  if(cumulativeFlags.find ("muons") != cumulativeFlags.end ()){
    flagPair muonFlags;
    for (int i = cumulativeFlags.at("muons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("muons").at(i).size()){
        muonFlags = cumulativeFlags.at("muons").at(i);
        break;
      }
    }
    for (uint muonIndex = 0; muonIndex != muonFlags.size(); muonIndex++){
      if(!muonFlags.at(muonIndex).first) continue;
      chosenMuon = & muons->at(muonIndex);
      break;
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), "muons") != objectsToGet.end ())
    chosenMuon = & muons->at (0);

  return chosenMuon;
}

template<typename T0, typename T1> const T1 *
OSUAnalysis::closest (const T0 *object0, const edm::Handle<vector<T1> > &object1Collection, const string &name)
{
  const T1 *closestObject1 = 0;
  double smallestDeltaR;
  if(cumulativeFlags.find (name) != cumulativeFlags.end ()){
    flagPair flags;
    for (int i = cumulativeFlags.at(name).size() - 1; i >= 0; i--){
      if (cumulativeFlags.at(name).at(i).size()){
        flags = cumulativeFlags.at(name).at(i);
        break;
      }
    }
    for (uint index = 0; index != flags.size(); index++){
      if(!flags.at(index).first) continue;
      const T1 *object1 = &object1Collection->at(index);
      double dR;
      if(!closestObject1 || (dR = reco::deltaR (object0->eta, object0->phi, object1->eta, object1->phi)) < smallestDeltaR){
        closestObject1 = object1;
        smallestDeltaR = dR;
      }
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), name) != objectsToGet.end ())
    closestObject1 = & object1Collection->at(0);

  return closestObject1;
}

double
OSUAnalysis::chosenHT ()
{
  double chosenHT = 0.0;
  if(cumulativeFlags.find ("jets") != cumulativeFlags.end ()){
    flagPair jetFlags;
    for (int i = cumulativeFlags.at("jets").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("jets").at(i).size()){
        jetFlags = cumulativeFlags.at("jets").at(i);
        break;
      }
    }
    for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++){
      if(!jetFlags.at(jetIndex).first) continue;
      chosenHT += jets->at(jetIndex).pt;
    }
  }

  return chosenHT;
}

double
OSUAnalysis::chosenST ()
{
  double chosenST = 0.0;
  if(cumulativeFlags.find ("electrons") != cumulativeFlags.end ()){
    flagPair electronFlags;
    for (int i = cumulativeFlags.at("electrons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("electrons").at(i).size()){
        electronFlags = cumulativeFlags.at("electrons").at(i);
        break;
      }
    }
    for (uint electronIndex = 0; electronIndex != electronFlags.size(); electronIndex++){
      if(!electronFlags.at(electronIndex).first) continue;
      chosenST += electrons->at(electronIndex).pt;
    }
  }

  if(cumulativeFlags.find ("muons") != cumulativeFlags.end ()){
    flagPair muonFlags;
    for (int i = cumulativeFlags.at("muons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("muons").at(i).size()){
        muonFlags = cumulativeFlags.at("muons").at(i);
        break;
      }
    }
    for (uint muonIndex = 0; muonIndex != muonFlags.size(); muonIndex++){
      if(!muonFlags.at(muonIndex).first) continue;
      chosenST += muons->at(muonIndex).pt;
    }
  }

  if(cumulativeFlags.find ("jets") != cumulativeFlags.end ()){
    flagPair jetFlags;
    for (int i = cumulativeFlags.at("jets").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("jets").at(i).size()){
        jetFlags = cumulativeFlags.at("jets").at(i);
        break;
      }
    }
    for (uint jetIndex = 0; jetIndex != jetFlags.size(); jetIndex++){
      if(!jetFlags.at(jetIndex).first) continue;
      chosenST += jets->at(jetIndex).pt;
    }
  }

  return chosenST;
}


pair<const BNmuon *, const BNmuon *>
OSUAnalysis::leadMuonPair ()
{
  pair<const BNmuon *, const BNmuon *> leadMuonPair;
  leadMuonPair.first = leadMuonPair.second = 0;

  if(cumulativeFlags.find ("muons") != cumulativeFlags.end ()){
    flagPair muonFlags;
    for (int i = cumulativeFlags.at("muons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("muons").at(i).size()){
        muonFlags = cumulativeFlags.at("muons").at(i);
        break;
      }
    }
    for (uint muonIndex0 = 0; muonIndex0 != muonFlags.size(); muonIndex0++){
      if(!muonFlags.at(muonIndex0).first) continue;
      for (uint muonIndex1 = muonIndex0 + 1; muonIndex1 < muonFlags.size(); muonIndex1++){
        if(!muonFlags.at(muonIndex1).first) continue;
        const BNmuon *mu0 = & muons->at(muonIndex0);
        const BNmuon *mu1 = & muons->at(muonIndex1);
        if(leadMuonPair.first == 0 || leadMuonPair.second == 0){
          leadMuonPair.first = mu0;
          leadMuonPair.second = mu1;
        }
        else{
          TVector2 newPt0 (mu0->px, mu0->py),
                   newPt1 (mu1->px, mu1->py),
                   oldPt0 (leadMuonPair.first->px, leadMuonPair.first->py),
                   oldPt1 (leadMuonPair.second->px, leadMuonPair.second->py);
          if(newPt0.Mod () + newPt1.Mod () > oldPt0.Mod() + oldPt1.Mod ())
            {
              leadMuonPair.first = mu0;
              leadMuonPair.second = mu1;
            }
        }
      }
    }
  }

  return leadMuonPair;
}

pair<const BNelectron *, const BNelectron *>
OSUAnalysis::leadElectronPair ()
{
  pair<const BNelectron *, const BNelectron *> leadElectronPair;
  leadElectronPair.first = leadElectronPair.second = 0;
  if(cumulativeFlags.find ("electrons") != cumulativeFlags.end ()){
    flagPair electronFlags;
    for (int i = cumulativeFlags.at("electrons").size() - 1; i >= 0; i--){
      if (cumulativeFlags.at("electrons").at(i).size()){
        electronFlags = cumulativeFlags.at("electrons").at(i);
        break;
      }
    }
    for (uint electronIndex0 = 0; electronIndex0 != electronFlags.size(); electronIndex0++){
      if(!electronFlags.at(electronIndex0).first) continue;
      for (uint electronIndex1 = electronIndex0 + 1; electronIndex1 < electronFlags.size(); electronIndex1++){
        if(!electronFlags.at(electronIndex1).first) continue;
        const BNelectron *el0 = & electrons->at(electronIndex0);
        const BNelectron *el1 = & electrons->at(electronIndex1);
        if(leadElectronPair.first == 0 || leadElectronPair.second == 0){
          leadElectronPair.first = el0;
          leadElectronPair.second = el1;
        }
        else{
          TVector2 newPt0 (el0->px, el0->py),
                   newPt1 (el1->px, el1->py),
                   oldPt0 (leadElectronPair.first->px, leadElectronPair.first->py),
                   oldPt1 (leadElectronPair.second->px, leadElectronPair.second->py);
          if(newPt0.Mod () + newPt1.Mod () > oldPt0.Mod() + oldPt1.Mod ())
            {
              leadElectronPair.first = el0;
              leadElectronPair.second = el1;
            }
        }
      }
    }
  }

  return leadElectronPair;
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
