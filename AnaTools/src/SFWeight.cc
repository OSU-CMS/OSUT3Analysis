#include "OSUT3Analysis/AnaTools/interface/SFWeight.h"


double
TrackSFWeight::at(const double &correctedD0, const int &shiftUpDown)
{

  double scaleFactor = 0;
  double error =0;
  double crit_value= 0.02; // For displaced stuff all the different d_0 are almost  equivalent
  if (abs(correctedD0) < crit_value )
    {
      scaleFactor = 1;
      error = 0.0; // The prompt region is used by the whole collaboration and very well know and no correction from tracking SF are currently applied so we set the error to be 0
    }
  else 
    {
      scaleFactor = 0.960;
      error = 0.012;
    }
  return scaleFactor + shiftUpDown * error; 
}

TrackSFWeight::~TrackSFWeight ()
{
}



MuonSFWeight::MuonSFWeight (const string &sfFile, const string &dataOverMC)
{
  TFile *fin = TFile::Open (sfFile.c_str ());
  TH2F* SF_Combined_TOT = (TH2F *) fin->Get(dataOverMC.c_str ());
  muonSFWeight_ = (TH2F*)SF_Combined_TOT->Clone();
  muonSFWeight_->GetEntries();// to avoid the crashing warning
  delete SF_Combined_TOT;
  fin->Close ();
  delete fin;
 }

 
double
MuonSFWeight::at(const double &eta, const double &pt, const int &shiftUpDown) 
{
  double pt_hist= pt;
  double eta_hist= eta;
  // to give a non null SF for muons being out of eta and/or pt range of the input histo
  if (pt > 300 && abs(eta) < muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetLast()) )
    {
      pt_hist =( muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetYaxis()->GetNbins() - 1) + muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetYaxis()->GetNbins() - 2))/2;
      if (pt > 300 && abs(eta) < 0.9) 
	{
	  pt_hist =( muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetYaxis()->GetNbins()) + muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetYaxis()->GetNbins() - 1))/2;
	}
    }
  else if (pt < 300 && abs(eta) > muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetLast()))
    {
      eta_hist =(muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetLast()) + muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetNbins() - 1))/2;
    }
  else if (pt > 300 && abs(eta) > muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetLast()))
    {
      pt_hist =( muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetYaxis()->GetNbins() - 1) + muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetYaxis()->GetNbins() - 2))/2;
      eta_hist =(muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetLast()) + muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetNbins() - 1))/2;
    }
  
  return muonSFWeight_->GetBinContent(muonSFWeight_->FindBin(abs(eta_hist),pt_hist)) + shiftUpDown * muonSFWeight_->GetBinError(muonSFWeight_->FindBin(abs(eta_hist),pt_hist)); 
}

MuonSFWeight::~MuonSFWeight ()
{
  delete muonSFWeight_;
}



ElectronSFWeight::ElectronSFWeight (const string &cmsswRelease, const string &id, const string &sfFile, const string &dataOverMC) :
  cmsswRelease_ (cmsswRelease),
  id_ (id),
  electronSFWeight_ (NULL)
{
  ifstream finStream (sfFile);
  if (!finStream)
    return;
  finStream.close ();
  TFile *fin = TFile::Open (sfFile.c_str ());
  electronSFWeight_ = (TH2F *) fin->Get (dataOverMC.c_str ());
  electronSFWeight_->SetDirectory (0);
  fin->Close ();
  delete fin;
}

double
ElectronSFWeight::at (const double &eta, const double &pt, const int &shiftUpDown)
{
  double scaleFactor = 1.0, minus = 0.0, plus = 0.0;

  if (electronSFWeight_)
    {
      double x = eta, y = pt;
      if (strcasestr (electronSFWeight_->GetYaxis ()->GetTitle (), "eta"))
        {
          x = pt;
          y = eta;
        }
      int xBin = electronSFWeight_->GetXaxis ()->FindBin (x),
          yBin = electronSFWeight_->GetYaxis ()->FindBin (y);
      xBin = min (xBin, electronSFWeight_->GetXaxis ()->GetNbins ());
      xBin = max (xBin, 1);
      yBin = min (yBin, electronSFWeight_->GetYaxis ()->GetNbins ());
      yBin = max (yBin, 1);

      scaleFactor = electronSFWeight_->GetBinContent (xBin, yBin);
      minus = plus = electronSFWeight_->GetBinError (xBin, yBin);
    }
  else if (cmsswRelease_ == "53X")
    {
      if (id_ == "loose")
        {
          if (fabs (eta) < 0.8)
            {
              if (pt < 15.0 && pt >= 10.0)
                scaleFactor = 0.855;
              if (pt < 20.0 && pt >= 15.0)
                scaleFactor = 0.962;
              if (pt < 30.0 && pt >= 20.0)
                scaleFactor = 1.005;
              if (pt < 40.0 && pt >= 30.0)
                scaleFactor = 1.004;
              if (pt < 50.0 && pt >= 40.0)
                scaleFactor = 1.008;
              if (pt >= 50.0)
                scaleFactor = 1.008;
            }
          if (fabs (eta) < 1.442 && fabs (eta) >= 0.8)
            {
              if (pt < 15.0 && pt >= 10.0)
                scaleFactor = 0.858;
              if (pt < 20.0 && pt >= 15.0)
                scaleFactor = 0.962;
              if (pt < 30.0 && pt >= 20.0)
                scaleFactor = 0.981;
              if (pt < 40.0 && pt >= 30.0)
                scaleFactor = 0.991;
              if (pt < 50.0 && pt >= 40.0)
                scaleFactor = 0.994;
              if (pt >= 50.0)
                scaleFactor = 0.999;
            }
          if (fabs (eta) < 1.556 && fabs (eta) >= 1.442)
            {
              if (pt < 15.0 && pt >= 10.0)
                scaleFactor = 1.109;
              if (pt < 20.0 && pt >= 15.0)
                scaleFactor = 0.903;
              if (pt < 30.0 && pt >= 20.0)
                scaleFactor = 1.044;
              if (pt < 40.0 && pt >= 30.0)
                scaleFactor = 0.998;
              if (pt < 50.0 && pt >= 40.0)
                scaleFactor = 0.989;
              if (pt >= 50.0)
                scaleFactor = 0.994;
            }
          if (fabs (eta) < 2.0 && fabs (eta) >= 1.556)
            {
              if (pt < 15.0 && pt >= 10.0)
                scaleFactor = 0.838;
              if (pt < 20.0 && pt >= 15.0)
                scaleFactor = 0.939;
              if (pt < 30.0 && pt >= 20.0)
                scaleFactor = 0.980;
              if (pt < 40.0 && pt >= 30.0)
                scaleFactor = 0.992;
              if (pt < 50.0 && pt >= 40.0)
                scaleFactor = 1.004;
              if (pt >= 50.0)
                scaleFactor = 1.006;
            }
          if (fabs (eta) < 2.5 && fabs (eta) >= 2.0)
            {
              if (pt < 15.0 && pt >= 10.0)
                scaleFactor = 1.034;
              if (pt < 20.0 && pt >= 15.0)
                scaleFactor = 0.970;
              if (pt < 30.0 && pt >= 20.0)
                scaleFactor = 1.017;
              if (pt < 40.0 && pt >= 30.0)
                scaleFactor = 1.019;
              if (pt < 50.0 && pt >= 40.0)
                scaleFactor = 1.005;
              if (pt >= 50.0)
                scaleFactor = 1.009;
            }
        }
      if (id_ == "tight")
        {
          if (fabs (eta) < 0.8)
            {
              if (pt < 15.0 && pt >= 10.0)
                scaleFactor = 0.818;
              if (pt < 20.0 && pt >= 15.0)
                scaleFactor = 0.928;
              if (pt < 30.0 && pt >= 20.0)
                scaleFactor = 0.973;
              if (pt < 40.0 && pt >= 30.0)
                scaleFactor = 0.979;
              if (pt < 50.0 && pt >= 40.0)
                scaleFactor = 0.984;
              if (pt >= 50.0)
                scaleFactor = 0.983;
            }
          if (fabs (eta) < 1.442 && fabs (eta) >= 0.8)
            {
              if (pt < 15.0 && pt >= 10.0)
                scaleFactor = 0.840;
              if (pt < 20.0 && pt >= 15.0)
                scaleFactor = 0.914;
              if (pt < 30.0 && pt >= 20.0)
                scaleFactor = 0.948;
              if (pt < 40.0 && pt >= 30.0)
                scaleFactor = 0.961;
              if (pt < 50.0 && pt >= 40.0)
                scaleFactor = 0.972;
              if (pt >= 50.0)
                scaleFactor = 0.977;
            }
          if (fabs (eta) < 1.556 && fabs (eta) >= 1.442)
            {
              if (pt < 15.0 && pt >= 10.0)
                scaleFactor = 1.008;
              if (pt < 20.0 && pt >= 15.0)
                scaleFactor = 0.877;
              if (pt < 30.0 && pt >= 20.0)
                scaleFactor = 0.983;
              if (pt < 40.0 && pt >= 30.0)
                scaleFactor = 0.983;
              if (pt < 50.0 && pt >= 40.0)
                scaleFactor = 0.957;
              if (pt >= 50.0)
                scaleFactor = 0.978;
            }
          if (fabs (eta) < 2.0 && fabs (eta) >= 1.556)
            {
              if (pt < 15.0 && pt >= 10.0)
                scaleFactor = 0.906;
              if (pt < 20.0 && pt >= 15.0)
                scaleFactor = 0.907;
              if (pt < 30.0 && pt >= 20.0)
                scaleFactor = 0.957;
              if (pt < 40.0 && pt >= 30.0)
                scaleFactor = 0.962;
              if (pt < 50.0 && pt >= 40.0)
                scaleFactor = 0.985;
              if (pt >= 50.0)
                scaleFactor = 0.986;
            }
          if (fabs (eta) < 2.5 && fabs (eta) >= 2.0)
            {
              if (pt < 15.0 && pt >= 10.0)
                scaleFactor = 0.991;
              if (pt < 20.0 && pt >= 15.0)
                scaleFactor = 0.939;
              if (pt < 30.0 && pt >= 20.0)
                scaleFactor = 1.001;
              if (pt < 40.0 && pt >= 30.0)
                scaleFactor = 1.002;
              if (pt < 50.0 && pt >= 40.0)
                scaleFactor = 0.999;
              if (pt >= 50.0)
                scaleFactor = 0.995;
            }
        }
      if (id_ == "mvaTrig0p9") // https://twiki.cern.ch/twiki/bin/view/CMS/KoPFAElectronTagAndProbe
        {
          if (fabs (eta) < 0.8)
            {
              if (pt < 30.0 && pt >= 20.0)
                {
                  scaleFactor = 0.953;
                  plus = 0.185;
                  minus = 0.007;
                }
              if (pt < 40.0 && pt >= 30.0)
                {
                  scaleFactor = 0.945;
                  plus = 0.003;
                  minus = 0.003;
                }
              if (pt < 50.0 && pt >= 40.0)
                {
                  scaleFactor = 0.948;
                  plus = 0.001;
                  minus = 0.001;
                }
              if (pt >= 50.0)
                {
                  scaleFactor = 0.960;
                  plus = 0.001;
                  minus = 0.001;
                }
            }
          if (fabs (eta) < 1.478 && fabs (eta) >= 0.8)
            {
              if (pt < 30.0 && pt >= 20.0)
                {
                  scaleFactor = 0.904;
                  plus = 0.010;
                  minus = 0.010;
                }
              if (pt < 40.0 && pt >= 30.0)
                {
                  scaleFactor = 0.916;
                  plus = 0.002;
                  minus = 0.001;
                }
              if (pt < 50.0 && pt >= 40.0)
                {
                  scaleFactor = 0.959;
                  plus = 0.001;
                  minus = 0.001;
                }
              if (pt >= 50.0)
                {
                  scaleFactor = 0.957;
                  plus = 0.002;
                  minus = 0.002;
                }
            }
          if (fabs (eta) <= 2.5 && fabs (eta) >= 1.478)
            {
              if (pt < 30.0 && pt >= 20.0)
                {
                  scaleFactor = 0.863;
                  plus = 0.008;
                  minus = 0.008;
                }
              if (pt < 40.0 && pt >= 30.0)
                {
                  scaleFactor = 0.896;
                  plus = 0.004;
                  minus = 0.004;
                }
              if (pt < 50.0 && pt >= 40.0)
                {
                  scaleFactor = 0.944;
                  plus = 0.002;
                  minus = 0.005;
                }
              if (pt >= 50.0)
                {
                  scaleFactor = 0.953;
                  plus = 0.008;
                  minus = 0.007;
                }
            }
        }
    }

  double error = shiftUpDown > 0 ? plus : minus;
  return scaleFactor + shiftUpDown * error;
}

ElectronSFWeight::~ElectronSFWeight ()
{
  if (electronSFWeight_)
    delete electronSFWeight_;
}



TriggerMetSFWeight::TriggerMetSFWeight (const string &sfFile, const string &dataOverMC)
{
  TFile *fin = TFile::Open (sfFile.c_str ());
  TH1F* dataOverMCHist = (TH1F *) fin->Get(dataOverMC.c_str ());
  if (!dataOverMCHist) cout << "Fatal Error [TriggerMetSFWeight::TriggerMetSFWeight]:  could not find histogram " << dataOverMC << " in " << sfFile << endl;  
  triggerMetSFWeight_ = (TH1F*)  dataOverMCHist->Clone();
  triggerMetSFWeight_->GetEntries();// to avoid the crashing warning
  delete dataOverMCHist;
  fin->Close ();
  delete fin;
 }

 
double
TriggerMetSFWeight::at(const double &Met, const int &shiftUpDown) 
{  
  int bin = triggerMetSFWeight_->FindBin(Met);  
  return 1.0 + triggerMetSFWeight_->GetBinContent(bin) + shiftUpDown * triggerMetSFWeight_->GetBinError(bin);  // Add 1.0 because the histogram bin content is (data-MC)/MC 
}

TriggerMetSFWeight::~TriggerMetSFWeight ()
{
  delete triggerMetSFWeight_;
}



TrackNMissOutSFWeight::TrackNMissOutSFWeight (const string &sfFile, const string &dataOverMC)
{
  TFile *fin = TFile::Open (sfFile.c_str ());
  TH1F* dataOverMCHist = (TH1F *) fin->Get(dataOverMC.c_str ());
  if (!dataOverMCHist) cout << "Fatal Error [TrackNMissOutSFWeight::TrackNMissOutSFWeight]:  could not find histogram " << dataOverMC << " in " << sfFile << endl;  
  trackNMissOutSFWeight_ = (TH1F*)  dataOverMCHist->Clone();
  trackNMissOutSFWeight_->GetEntries();  // to avoid the crashing warning
  delete dataOverMCHist;
  fin->Close ();
  delete fin;
}

 
double
TrackNMissOutSFWeight::at(const double &NMissOut, const int &shiftUpDown) 
{  
  int bin = trackNMissOutSFWeight_->FindBin(NMissOut);  
  return 1.0 + trackNMissOutSFWeight_->GetBinContent(bin) + shiftUpDown * trackNMissOutSFWeight_->GetBinError(bin);  // Add 1.0 because the histogram bin content is (data-MC)/MC 
}

TrackNMissOutSFWeight::~TrackNMissOutSFWeight ()
{
  delete trackNMissOutSFWeight_;
}



IsrVarySFWeight::IsrVarySFWeight (const string &sfFile, const string &dataOverMC)
{
  TFile *fin = TFile::Open (sfFile.c_str ());
  TH1F* dataOverMCHist = (TH1F *) fin->Get(dataOverMC.c_str ());
  if (!dataOverMCHist) cout << "Fatal Error [IsrVarySFWeight::IsrVarySFWeight]:  could not find histogram " << dataOverMC << " in " << sfFile << endl;  
  isrVarySFWeight_ = (TH1F*)  dataOverMCHist->Clone();
  isrVarySFWeight_->GetEntries();// to avoid the crashing warning
  delete dataOverMCHist;
  clog << "Will use hist " << isrVarySFWeight_->GetName() << " from file " << sfFile << " to do ISR reweighting." << endl;  
  fin->Close ();
  delete fin;
 }
 
double
IsrVarySFWeight::at(const double &ptSusy, const int &shiftUpDown) 
{  
  int bin = isrVarySFWeight_->FindBin(ptSusy);  
  return 1.0 + isrVarySFWeight_->GetBinContent(bin) + shiftUpDown * isrVarySFWeight_->GetBinError(bin);  // Add 1.0 because the histogram bin content is (data-MC)/MC 
}

IsrVarySFWeight::~IsrVarySFWeight ()
{
  delete isrVarySFWeight_;
}


// Define four classes that will be used to reweight generated event to emulate the CMS reconstruction and the set of cut applied in the displaced susy analysis

// MuonCutWeight
MuonCutWeight::MuonCutWeight (const string &sfFile, const string &dataOverMC)
{
  TFile *fin = TFile::Open (sfFile.c_str ());
  TH1F* dataOverMCHist = (TH1F *) fin->Get(dataOverMC.c_str ());
  if (!dataOverMCHist) cout << "Fatal Error [MuonCutWeight::MuonCutWeight]:  could not find histogram " << dataOverMC << " in " << sfFile << endl;  
  muonCutWeight_ = (TH1F*)  dataOverMCHist->Clone();
  muonCutWeight_->GetEntries();  // to avoid the crashing warning
  delete dataOverMCHist;
  fin->Close ();
  delete fin;
}

 
double
MuonCutWeight::at(const double &pt) 
{  
  int bin = muonCutWeight_->FindBin(pt);  
  return  muonCutWeight_->GetBinContent(bin);
}

MuonCutWeight::~MuonCutWeight ()
{
  delete muonCutWeight_;
}


// ElectronCutWeight
ElectronCutWeight::ElectronCutWeight (const string &sfFile, const string &dataOverMC)
{
  TFile *fin = TFile::Open (sfFile.c_str ());
  TH1F* dataOverMCHist = (TH1F *) fin->Get(dataOverMC.c_str ());
  if (!dataOverMCHist) cout << "Fatal Error [ElectronCutWeight::ElectronCutWeight]:  could not find histogram " << dataOverMC << " in " << sfFile << endl;  
  electronCutWeight_ = (TH1F*)  dataOverMCHist->Clone();
  electronCutWeight_->GetEntries();  // to avoid the crashing warning
  delete dataOverMCHist;
  fin->Close ();
  delete fin;
}

 
double
ElectronCutWeight::at(const double &pt) 
{  
  int bin = electronCutWeight_->FindBin(pt);  
  return  electronCutWeight_->GetBinContent(bin); 
}

ElectronCutWeight::~ElectronCutWeight ()
{
  delete electronCutWeight_;
}

// RecoElectronWeight
RecoElectronWeight::RecoElectronWeight (const string &sfFile, const string &dataOverMC)
{
  TFile *fin = TFile::Open (sfFile.c_str ());
  TH1F* dataOverMCHist = (TH1F *) fin->Get(dataOverMC.c_str ());
  if (!dataOverMCHist) cout << "Fatal Error [RecoElectronWeight::RecoElectronWeight]:  could not find histogram " << dataOverMC << " in " << sfFile << endl;  
  recoElectronWeight_ = (TH1F*)  dataOverMCHist->Clone();
  recoElectronWeight_->GetEntries();  
  delete dataOverMCHist;
  fin->Close ();
  delete fin;
}

 
double
RecoElectronWeight::at(const double &d0) 
{  
  int bin = recoElectronWeight_->FindBin(d0);  
  return recoElectronWeight_->GetBinContent(bin); 
}

RecoElectronWeight::~RecoElectronWeight ()
{
  delete recoElectronWeight_;
}

// RecoMuonWeight
RecoMuonWeight::RecoMuonWeight (const string &sfFile, const string &dataOverMC)
{
  TFile *fin = TFile::Open (sfFile.c_str ());
  TH1F* dataOverMCHist = (TH1F *) fin->Get(dataOverMC.c_str ());
  if (!dataOverMCHist) cout << "Fatal Error [RecoMuonWeight::RecoMuonWeight]:  could not find histogram " << dataOverMC << " in " << sfFile << endl;  
  recoMuonWeight_ = (TH1F*)  dataOverMCHist->Clone();
  recoMuonWeight_->GetEntries();  // to avoid the crashing warning
  delete dataOverMCHist;
  fin->Close ();
  delete fin;
}

 
double
RecoMuonWeight::at(const double &d0) 
{  
  int bin = recoMuonWeight_->FindBin(d0);  
  return  recoMuonWeight_->GetBinContent(bin); 
}

RecoMuonWeight::~RecoMuonWeight ()
{
  delete recoMuonWeight_;
}





/*
--- Used previously

MuonSFWeight::MuonSFWeight (const string &sfFile, const string &dataOverMC)
{

  TFile *fin = TFile::Open (sfFile.c_str ());
  TGraphAsymmErrors *data = (TGraphAsymmErrors *) fin->Get (dataOverMC.c_str ());

  double bins[data->GetN () + 1];
  for (int i = 0; i < data->GetN (); i++)
    {
      double x, y, exl, exh;

      data->GetPoint (i, x, y);
      exl = data->GetErrorXlow (i);
      exh = data->GetErrorXhigh (i);
      if (i == 0)
        bins[i] = x - exl;
      bins[i + 1] = x + exh;
    }

  muonSFWeight_ = new TH1D ("muonSFWeight", "", data->GetN (), bins);
  muonSFWeight_->SetDirectory (0);
  for (int i = 0; i < data->GetN (); i++)
    {
      double dataX, dataY;

      data->GetPoint (i, dataX, dataY);
      muonSFWeight_->SetBinContent (i + 1, dataY);
    }

  fin->Close ();
  delete data;
}
--- Used previously 
*/
