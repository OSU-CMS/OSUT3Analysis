#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/AnaTools/plugins/ObjectScalingFactorProducer.h"
#include <typeinfo>

ObjectScalingFactorProducer::ObjectScalingFactorProducer(const edm::ParameterSet &cfg) :
  EventVariableProducer(cfg)
{

  if (cfg.exists ("electronFile"))
    electronFile_ = cfg.getParameter<string>("electronFile");

  if (cfg.exists ("muonFile"))
    muonFile_ = cfg.getParameter<string>("muonFile");

  if (cfg.exists ("trackFile"))
    trackFile_ = cfg.getParameter<string>("trackFile");

  if (!cfg.exists ("scaleFactors")){
    clog << "ERROR [ObjectScalingFactorProducer]: No scale factors included\n";
    exit(1);
  }

  // parse definitions of scale factors
  std::vector<edm::ParameterSet> scaleFactorDefs_ = cfg.getParameter<vector<edm::ParameterSet> >("scaleFactors");
  for (auto &sfDef : scaleFactorDefs_){
    ScaleFactor sf;
    sf.inputCollection = sfDef.getParameter<string> ("inputCollection");
    sf.sfType = sfDef.getParameter<string> ("sfType");
    sf.version = sfDef.getParameter<string> ("version");
    sf.wp = sfDef.exists("wp") ? sf.wp = sfDef.getParameter<string> ("wp") : "";
    sf.additionalSystematic = sfDef.exists("additionalSystematic") ? sfDef.getParameter<double> ("additionalSystematic") : -1;
    sf.additionalSystematic = sfDef.exists("additionalSystematicBelow20GeV") ? sfDef.getParameter<double> ("additionalSystematicBelow20GeV") : -1;
    sf.additionalSystematic = sfDef.exists("additionalSystematicAbove80GeV") ? sfDef.getParameter<double> ("additionalSystematicAbove80GeV") : -1;
    sf.outputVariable = string(anatools::singular(sf.inputCollection)) + string(sf.sfType) + string(sf.version) + string(sf.wp);

    if (!sfDef.exists ("eras")){
      sf.inputPlots.push_back(sf.outputVariable);
      sf.inputLumis.push_back(1.0);
    }
    else{
      for (auto &era : sfDef.getParameter<vector<string> > ("eras"))
        sf.inputPlots.push_back(sf.outputVariable+era);
      for (auto &lumi : sfDef.getParameter<vector<double> >("lumis"))
	      sf.inputLumis.push_back(lumi);
    }

    objectsToGet_.insert(sf.inputCollection);
    scaleFactors_.push_back(sf);

  }

  anatools::getAllTokens (collections_, consumesCollector (), tokens_);
}

ObjectScalingFactorProducer::~ObjectScalingFactorProducer() {}

void
ObjectScalingFactorProducer::AddVariables (const edm::Event &event) {
#if DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == MINI_AOD

  if (event.isRealData ()) {
    for (auto &sf : scaleFactors_) {
      cout << "HAY -- inserting eventvariables: " << sf.outputVariable << endl;
      (*eventvariables)[sf.outputVariable] = 1.0;
      (*eventvariables)[sf.outputVariable + "Up"] = 1.0;
      (*eventvariables)[sf.outputVariable + "Down"] = 1.0;
    }
    return;
  }

  anatools::getRequiredCollections (objectsToGet_, handles_, event, tokens_);

  TFile *electronInputFile = 0;
  bool doElectrons = false;
  if(find(objectsToGet_.begin(), objectsToGet_.end(), "electrons") != objectsToGet_.end())
    doElectrons = true;

  if (doElectrons) {
    electronInputFile = TFile::Open (electronFile_.c_str ());
    if (!electronInputFile || electronInputFile->IsZombie()) {
      clog << "ERROR [ObjectScalingFactorProducer]: Could not find file: " << electronFile_
	         << "; will cause a seg fault." << endl;
      exit(1);
    }
  }

  TFile *muonInputFile = 0;
  bool doMuons = false;
  if(find(objectsToGet_.begin(), objectsToGet_.end(), "muons") != objectsToGet_.end())
    doMuons = true;

  if (doMuons) {
    muonInputFile = TFile::Open (muonFile_.c_str ());
    if (!muonInputFile || muonInputFile->IsZombie()) {
      clog << "ERROR [ObjectScalingFactorProducer]: Could not find file: " << muonFile_
	         << "; will cause a seg fault." << endl;
      exit(1);
    }
  }

  // loop over desired scale factors, treating each case independently
  for (auto &sf : scaleFactors_){
    double sfCentral = 1;
    double sfDown = 1;
    double sfUp = 1;

    // loop over different types of electron SFs
    // these aren't split into separate eras, so don't bother with looping over eras
    if (sf.inputCollection == "electrons") {
      TH2F * plot = (TH2F*)electronInputFile->Get(sf.inputPlots[0].c_str());
      if(!plot){
	       clog << "ERROR [ObjectScalingFactorProducer]: Could not find histogram: " << sf.inputPlots[0]
	            << "; will cause a seg fault." << endl;
         exit(1);
      }

      float xMin = plot->GetXaxis()->GetBinCenter(1);
      float xMax = plot->GetXaxis()->GetBinCenter(plot->GetNbinsX());
      float yMin = plot->GetYaxis()->GetBinCenter(1);
      float yMax = plot->GetYaxis()->GetBinCenter(plot->GetNbinsY());
      for (const auto &electron1 : *handles_.electrons) {
         float eta = electron1.eta();
         // the 2015 ID plots are in |eta| yet the rest are in eta, so check xMin
         if(xMin >= 0) eta = abs(eta);
         // check for eta outside of plot binning
         if(eta > xMax) eta = xMax;
         if(eta < xMin) eta = xMin;

         float pt = electron1.pt();
         if(pt < yMin) pt = yMin;
         if(pt > yMax) pt = yMax;

	       float sfValue = plot->GetBinContent(plot->FindBin(eta, pt));
	       float sfError = plot->GetBinError(plot->FindBin(eta, pt));

         // for 80X Moriond series (https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2#Electron_efficiencies_and_scale)
         // special systematic recommendation for pt<20 and pt>80
         if(sf.additionalSystematicBelow20GeV > 0 && pt < 20.0) sfError += sf.additionalSystematicBelow20GeV * sfValue;
         if(sf.additionalSystematicAbove80GeV > 0 && pt > 80.0) sfError += sf.additionalSystematicAbove80GeV * sfValue;

	       sfCentral *= sfValue;
	       sfUp *= sfValue + sfError;
         sfDown *= sfValue - sfError;
      } // end loop over electrons
      delete plot;
    }

    // muons are split up into eras, so loop over any provided
    // also can be either TH2F's or TGraphAsymmErrors, so test for each case
    else if (sf.inputCollection == "muons") {

      TObject * tempObj = muonInputFile->Get(sf.inputPlots[0].c_str());
      if(!tempObj) {
        clog << "ERROR [ObjectScalingFactorProducer]: Could not find object: " << sf.inputPlots[0]
             << "; will cause a seg fault." << endl;
        exit(1);
      }

      int numPlots = sf.inputPlots.size();
      vector<float> valuesByEra, valuesByEraUp, valuesByEraDown;

      if (tempObj->InheritsFrom("TGraphAsymmErrors")) {

        // find values and errors for each era, and store them in vectors
        for (int iGraph = 0; iGraph < numPlots; iGraph++) {
          TGraphAsymmErrors * plot = (TGraphAsymmErrors*)muonInputFile->Get(sf.inputPlots[iGraph].c_str());
          if(!plot) {
             clog << "ERROR [ObjectScalingFactorProducer]: Could not find TGraphAsymmErrors: " << sf.inputPlots[iGraph]
                  << "; will cause a seg fault." << endl;
             exit(1);
          }

          // For this era/graph, find the SF as the product of all muons' SFs
          float thisEraSF = 1.0;
          float thisEraSFUp = 1.0;
          float thisEraSFDown = 1.0;

          for (const auto &muon1 : *handles_.muons) {
             // find the point in the TGraph for this muon's |eta|
             double eta = abs(muon1.eta());
             int iPoint = 0;
             while(iPoint < plot->GetN()) {
               if(eta < plot->GetX()[iPoint] + plot->GetErrorXhigh(iPoint) &&
                  eta > plot->GetX()[iPoint] - plot->GetErrorXlow(iPoint))
                  break;
                else iPoint++;
             }
             // if the |eta| is past the highest point, just use the highest \eta| point with a value; |eta| can't be < 0 so no need to check
             if(iPoint == plot->GetN()) iPoint = plot->GetN() - 1;

             // Now include this muon's scale factor
             float thisMuonSF = plot->GetY()[iPoint];
             float thisMuonSFError = plot->GetErrorYhigh(iPoint);

             thisEraSF *= thisMuonSF;
             thisEraSFUp *= thisMuonSF + thisMuonSFError;
             thisEraSFDown *= thisMuonSF - thisMuonSFError;
          } // end loop over muons
          valuesByEra.push_back(thisEraSF);
          valuesByEraUp.push_back(thisEraSFUp);
          valuesByEraDown.push_back(thisEraSFDown);

          delete plot;
        } // end loop over eras

      } // end TGraphAsymmErrors case -- now have vectors of values and errors by era

      else if(tempObj->InheritsFrom("TH2")) {

        // find values and errors for each era, and store them in vectors
        for (int iPlot = 0; iPlot < numPlots; iPlot++) {
          TH2F * plot = (TH2F*)muonInputFile->Get(sf.inputPlots[iPlot].c_str());
          if(!plot) {
             clog << "ERROR [ObjectScalingFactorProducer]: Could not find histogram: " << sf.inputPlots[iPlot]
                  << "; will cause a seg fault." << endl;
             exit(1);
          }

          float thisEraSF = 1.0;
          float thisEraSFUp = 1.0;
          float thisEraSFDown = 1.0;

          float xMin = plot->GetXaxis()->GetBinCenter(1);
          float xMax = plot->GetXaxis()->GetBinCenter(plot->GetNbinsX());
          float yMax = plot->GetYaxis()->GetBinCenter(plot->GetNbinsY());

          for (const auto &muon1 : *handles_.muons) {
            float pt = muon1.pt();
            if(pt > xMax) pt = xMax;
            if(pt < xMin) pt = xMin;
            float eta = (abs(muon1.eta()) > yMax) ? yMax : abs(muon1.eta());
            int bin = plot->FindBin(pt, eta);

            float thisMuonSF = plot->GetBinContent(bin);
            float thisMuonSFError = plot->GetBinError(bin);

            thisEraSF *= thisMuonSF;
            thisEraSFUp *= thisMuonSF + thisMuonSFError;
            thisEraSFDown *= thisMuonSF - thisMuonSFError;

          } // end loop over muons
          valuesByEra.push_back(thisEraSF);
          valuesByEraUp.push_back(thisEraSFUp);
          valuesByEraDown.push_back(thisEraSFDown);

          delete plot;
        } // end loop over eras

      } // end TH2 case -- now have vectors of values and errors by era

      delete tempObj;

      // now we find the lumi-weighted averages amongst the eras

      double totalLumi = 0;
      for (auto lumi : sf.inputLumis) totalLumi += lumi;

      sfCentral = 0.0;
      sfUp = 0.0;
      sfDown = 0.0;
      for (unsigned int iEra = 0; iEra < valuesByEra.size(); iEra++) {
        sfCentral += valuesByEra[iEra] * sf.inputLumis[iEra] / totalLumi;
        sfUp += valuesByEraUp[iEra] * sf.inputLumis[iEra] / totalLumi;
        sfDown += valuesByEraDown[iEra] * sf.inputLumis[iEra] / totalLumi;
      }

      valuesByEra.clear();
      valuesByEraUp.clear();
      valuesByEraDown.clear();
    } // end if muons

    // in case a POG recommends an extra overall systematic for this SF
    if(sf.additionalSystematic > 0) {
      sfUp += sf.additionalSystematic * sfCentral;
      sfDown -= sf.additionalSystematic * sfCentral;
    }

    (*eventvariables)[sf.outputVariable] = sfCentral;
    (*eventvariables)[sf.outputVariable + "Up"] = sfUp;
    (*eventvariables)[sf.outputVariable + "Down"] = sfDown;

  }

  if (doElectrons)
    electronInputFile->Close();
  if (doMuons)
    muonInputFile->Close();

  return;

  if (doTrackSF_)
    {
      double sf = 1.0;
#if IS_VALID(tracks)
      TFile *trackSF = TFile::Open (trackFile_.c_str ());
      if (!trackSF || trackSF->IsZombie()) {
        clog << "ERROR [ObjectScalingFactorProducer]: Could not find file: " << trackFile_
         << "; will cause a seg fault." << endl;
        exit(1);
      }

      TH1D *data = (TH1D *) trackSF->Get ("missingOuterHits_data");
      data->SetDirectory (0);
      TH1D *mc = (TH1D *) trackSF->Get ("missingOuterHits_mc");
      mc->SetDirectory (0);
      trackSF->Close ();
      delete trackSF;

      data->Scale (1.0 / data->Integral ());
      mc->Scale (1.0 / mc->Integral ());
      data->Divide (mc);

      for (const auto &track : *handles_.tracks)
        {
          double missingOuterHits = track.hitPattern ().trackerLayersWithoutMeasurement (reco::HitPattern::MISSING_OUTER_HITS);
          sf *= data->GetBinContent (data->FindBin (missingOuterHits));
        }
      delete data;
      delete mc;
#endif
      (*eventvariables)["trackScalingFactor"] = sf;
    }
#else
  (*eventvariables)["trackScalingFactor"] = 1;
# endif
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ObjectScalingFactorProducer);
