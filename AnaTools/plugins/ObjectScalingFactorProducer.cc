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
    sf.wp = "";
    if (sfDef.exists("wp"))
      sf.wp = sfDef.getParameter<string> ("wp");

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

  if (event.isRealData ())
    return;

  anatools::getRequiredCollections (objectsToGet_, handles_, event, tokens_);

  TFile *electronInputFile = 0;
  bool doElectrons = false;
  if(find(objectsToGet_.begin(), objectsToGet_.end(), "electrons") != objectsToGet_.end())
    doElectrons = true;

  if (doElectrons){
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

  if (doMuons){
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

    double totalLumi = 0;
    for (auto lumi : sf.inputLumis)
      totalLumi += lumi;

    // loop over different types of electron SFs
    if (sf.inputCollection == "electrons"){
      TH2F *plot;
      electronInputFile->GetObject(sf.inputPlots[0].c_str(),plot);
      if(!plot){
	clog << "ERROR [ObjectScalingFactorProducer]: Could not find histogram: " << sf.inputPlots[0]
	     << "; will cause a seg fault." << endl;
	exit(1);
      }
      for (const auto &electron1 : *handles_.electrons) {
	float xMax = plot->GetXaxis()->GetBinCenter(plot->GetNbinsX());
	float yMax = plot->GetYaxis()->GetBinCenter(plot->GetNbinsY());
	float eta = abs(electron1.eta()) > xMax ? xMax : abs(electron1.eta());
	float pt = electron1.pt() > yMax ? yMax : electron1.pt();
	if (plot->GetXaxis()->FindBin(electron1.eta()))
	  eta = electron1.eta() > 0 ? eta : -eta;
	float sfValue = plot->GetBinContent(plot->FindBin(eta,pt));
	float sfError = plot->GetBinError(plot->FindBin(eta,pt));
	sfCentral *= sfValue;
	sfUp *= sfValue + sfError;
	sfDown *= sfValue - sfError;
      } // end loop over electrons
      delete plot;
    }


    else if (sf.inputCollection == "muons"){

      if (sf.sfType == "Reco"){
	// why in god's name would you use a TGraph for this, muon POG?
	// i'm just gonna hardcode the solution for this particular case...
	TGraphAsymmErrors *plot;
	muonInputFile->GetObject(sf.inputPlots[0].c_str(), plot);
	if(!plot){
	  clog << "ERROR [ObjectScalingFactorProducer]: Could not find histogram: " << sf.inputPlots[0]
	       << "; will cause a seg fault." << endl;
	  exit(1);
	}
	for (const auto &muon1 : *handles_.muons) {
	  double eta = abs(muon1.eta());
	  int bin = floor(eta / 0.2);
	  float sfValue = plot->GetY()[bin];
	  float sfError = plot->GetErrorYhigh(bin);
	  sfCentral *= sfValue;
	  sfUp *= sfValue + sfError;
	  sfDown *= sfValue - sfError;
	} // end loop over muons
	delete plot;
      }


      // these two came as era-dependent SFs
      // we'll use a lumi-weighted sum of the two eras
      else if (sf.sfType == "ID" || sf.sfType == "Iso"){
	int numPlots = sf.inputPlots.size();
	vector<TH2F*> plots;
	//prepare input plots by applying lumi weighting, store in vector
	for (int x = 0; x != numPlots; x++){
	  TH2F *plot;
	  muonInputFile->GetObject(sf.inputPlots[x].c_str(), plot);
	  if(!plot){
	    clog << "ERROR [ObjectScalingFactorProducer]: Could not find histogram: " << sf.inputPlots[x]
		 << "; will cause a seg fault." << endl;
	    exit(1);
	  }
	  float lumiWeight = sf.inputLumis[x]/totalLumi;
	  plot->Scale(lumiWeight);
	  plot->SetDirectory(0);
	  plots.push_back(plot);
	}


        for (const auto &muon1 : *handles_.muons) {
	  float sfValue = 0;
	  float sfVariance = 0;
	  
	  for (auto &plot : plots){
	    float xMax = plot->GetXaxis()->GetBinCenter(plot->GetNbinsX());
	    float yMax = plot->GetYaxis()->GetBinCenter(plot->GetNbinsY());
	    float pt = muon1.pt() > xMax ? xMax : muon1.pt();
	    float eta = abs(muon1.eta()) > yMax ? yMax : abs(muon1.eta());
	    int bin = plot->FindBin(pt,eta);
	    sfValue += plot->GetBinContent(bin);
	    sfVariance += plot->GetBinError(bin) * plot->GetBinError(bin);
	  } // end loop over input plots

	  float sfError = sqrt(sfVariance);
	  sfCentral *= sfValue;
	  sfUp *= sfValue + sfError;
	  sfDown *= sfValue - sfError;
	} // end loop over muons

      }

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
