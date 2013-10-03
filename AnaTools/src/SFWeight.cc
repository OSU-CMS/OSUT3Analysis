#include "OSUT3Analysis/AnaTools/interface/SFWeight.h"


MuonSFWeight::MuonSFWeight (const string &sfFile, const string &dataOverMC)
{
  TFile *fin = TFile::Open (sfFile.c_str ());
  TH2F* SF_Combined_TOT = (TH2F *) fin->Get(dataOverMC.c_str ());
  muonSFWeight_ = (TH2F*)SF_Combined_TOT->Clone();
  muonSFWeight_->GetEntries();// to avoid the crashing warning
  delete SF_Combined_TOT;
  fin->Close ();
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

ElectronSFWeight::ElectronSFWeight (const string &cmsswRelease, const string &id) :
  cmsswRelease_ (cmsswRelease),
  id_ (id)
{
}

double
ElectronSFWeight::at (const double &eta, const double &pt, const int &shiftUpDown)
{
  double scaleFactor = 1.0, minus = 0.0, plus = 0.0;

  if (cmsswRelease_ == "53X")
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
