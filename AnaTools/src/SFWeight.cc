#include "OSUT3Analysis/AnaTools/interface/SFWeight.h"

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

MuonSFWeight::MuonSFWeight (const string &sfFile, const string &dataEff, const string &mcEff)
{
  TFile *fin = TFile::Open (sfFile.c_str ());
  TGraphAsymmErrors *mc = (TGraphAsymmErrors *) fin->Get (mcEff.c_str ());
  TGraphAsymmErrors *data = (TGraphAsymmErrors *) fin->Get (dataEff.c_str ());

  double bins[mc->GetN () + 1];
  for (int i = 0; i < mc->GetN (); i++)
    {
      double x, y, exl, exh;

      mc->GetPoint (i, x, y);
      exl = mc->GetErrorXlow (i);
      exh = mc->GetErrorXhigh (i);
      if (i == 0)
        bins[i] = x - exl;
      bins[i + 1] = x + exh;
    }

  muonSFWeight_ = new TH1D ("muonSFWeight", "", mc->GetN (), bins);
  muonSFWeight_->SetDirectory (0);
  for (int i = 0; i < mc->GetN (); i++)
    {
      double mcX, mcY, dataX, dataY;

      mc->GetPoint (i, mcX, mcY);
      data->GetPoint (i, dataX, dataY);
      muonSFWeight_->SetBinContent (i + 1, dataY / mcY);
    }

  fin->Close ();
  delete mc;
  delete data;
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
ElectronSFWeight::at (const double &eta, const double &pt)
{
  //cout << eta << " " << pt << endl;
  //cout << "doing it\n";
  if (cmsswRelease_ == "53X")
    {
      if (id_ == "loose")
        {
          if (fabs (eta) < 0.8)
            {
              if (pt < 15.0 && pt >= 10.0)
                return 0.855;
              if (pt < 20.0 && pt >= 15.0)
                return 0.962;
              if (pt < 30.0 && pt >= 20.0)
                return 1.005;
              if (pt < 40.0 && pt >= 30.0)
                return 1.004;
              if (pt < 50.0 && pt >= 40.0)
                return 1.008;
              if (pt >= 50.0)
                return 1.008;
            }
          if (fabs (eta) < 1.442 && fabs (eta) >= 0.8)
            {
              if (pt < 15.0 && pt >= 10.0)
                return 0.858;
              if (pt < 20.0 && pt >= 15.0)
                return 0.962;
              if (pt < 30.0 && pt >= 20.0)
                return 0.981;
              if (pt < 40.0 && pt >= 30.0)
                return 0.991;
              if (pt < 50.0 && pt >= 40.0)
                return 0.994;
              if (pt >= 50.0)
                return 0.999;
            }
          if (fabs (eta) < 1.556 && fabs (eta) >= 1.442)
            {
              if (pt < 15.0 && pt >= 10.0)
                return 1.109;
              if (pt < 20.0 && pt >= 15.0)
                return 0.903;
              if (pt < 30.0 && pt >= 20.0)
                return 1.044;
              if (pt < 40.0 && pt >= 30.0)
                return 0.998;
              if (pt < 50.0 && pt >= 40.0)
                return 0.989;
              if (pt >= 50.0)
                return 0.994;
            }
          if (fabs (eta) < 2.0 && fabs (eta) >= 1.556)
            {
              if (pt < 15.0 && pt >= 10.0)
                return 0.838;
              if (pt < 20.0 && pt >= 15.0)
                return 0.939;
              if (pt < 30.0 && pt >= 20.0)
                return 0.980;
              if (pt < 40.0 && pt >= 30.0)
                return 0.992;
              if (pt < 50.0 && pt >= 40.0)
                return 1.004;
              if (pt >= 50.0)
                return 1.006;
            }
          if (fabs (eta) < 2.5 && fabs (eta) >= 2.0)
            {
              if (pt < 15.0 && pt >= 10.0)
                return 1.034;
              if (pt < 20.0 && pt >= 15.0)
                return 0.970;
              if (pt < 30.0 && pt >= 20.0)
                return 1.017;
              if (pt < 40.0 && pt >= 30.0)
                return 1.019;
              if (pt < 50.0 && pt >= 40.0)
                return 1.005;
              if (pt >= 50.0)
                return 1.009;
            }
        }
      if (id_ == "tight")
        {
          if (fabs (eta) < 0.8)
            {
              if (pt < 15.0 && pt >= 10.0)
                return 0.818;
              if (pt < 20.0 && pt >= 15.0)
                return 0.928;
              if (pt < 30.0 && pt >= 20.0)
                return 0.973;
              if (pt < 40.0 && pt >= 30.0)
                return 0.979;
              if (pt < 50.0 && pt >= 40.0)
                return 0.984;
              if (pt >= 50.0)
                return 0.983;
            }
          if (fabs (eta) < 1.442 && fabs (eta) >= 0.8)
            {
              if (pt < 15.0 && pt >= 10.0)
                return 0.840;
              if (pt < 20.0 && pt >= 15.0)
                return 0.914;
              if (pt < 30.0 && pt >= 20.0)
                return 0.948;
              if (pt < 40.0 && pt >= 30.0)
                return 0.961;
              if (pt < 50.0 && pt >= 40.0)
                return 0.972;
              if (pt >= 50.0)
                return 0.977;
            }
          if (fabs (eta) < 1.556 && fabs (eta) >= 1.442)
            {
              if (pt < 15.0 && pt >= 10.0)
                return 1.008;
              if (pt < 20.0 && pt >= 15.0)
                return 0.877;
              if (pt < 30.0 && pt >= 20.0)
                return 0.983;
              if (pt < 40.0 && pt >= 30.0)
                return 0.983;
              if (pt < 50.0 && pt >= 40.0)
                return 0.957;
              if (pt >= 50.0)
                return 0.978;
            }
          if (fabs (eta) < 2.0 && fabs (eta) >= 1.556)
            {
              if (pt < 15.0 && pt >= 10.0)
                return 0.906;
              if (pt < 20.0 && pt >= 15.0)
                return 0.907;
              if (pt < 30.0 && pt >= 20.0)
                return 0.957;
              if (pt < 40.0 && pt >= 30.0)
                return 0.962;
              if (pt < 50.0 && pt >= 40.0)
                return 0.985;
              if (pt >= 50.0)
                return 0.986;
            }
          if (fabs (eta) < 2.5 && fabs (eta) >= 2.0)
            {
              if (pt < 15.0 && pt >= 10.0)
                return 0.991;
              if (pt < 20.0 && pt >= 15.0)
                return 0.939;
              if (pt < 30.0 && pt >= 20.0)
                return 1.001;
              if (pt < 40.0 && pt >= 30.0)
                return 1.002;
              if (pt < 50.0 && pt >= 40.0)
                return 0.999;
              if (pt >= 50.0)
                return 0.995;
            }
        }
      if (id_ == "mvaNonTrig")
        {
	  //cout << "mvaNonTrig\n";
          if (fabs (eta) < 0.8)
            {
              if (pt < 20.0)
                return 1.0;
              if (pt < 30.0 && pt >= 20.0)
                return 0.994;
              if (pt < 40.0 && pt >= 30.0)
                return 0.974;
              if (pt < 50.0 && pt >= 40.0)
                return 0.988;
              if (pt >= 50.0)
                return 0.999;
            }
          if (fabs (eta) < 1.478 && fabs (eta) >= 0.8)
            {
              if (pt < 20.0)
                return 1.0;
              if (pt < 30.0 && pt >= 20.0)
                return 0.971;
              if (pt < 40.0 && pt >= 30.0)
                return 0.967;
              if (pt < 50.0 && pt >= 40.0)
                return 0.984;
              if (pt >= 50.0)
                return 0.998;
            }
          if (fabs (eta) <= 2.5 && fabs (eta) >= 1.478)
            {
              if (pt < 20.0)
                return 1.0;
              if (pt < 30.0 && pt >= 20.0)
                return 0.921;
              if (pt < 40.0 && pt >= 30.0)
                return 0.932;
              if (pt < 50.0 && pt >= 40.0)
                return 0.968;
              if (pt >= 50.0)
                return 0.983;
            }
        }
    }

  return 0.0;
}

ElectronSFWeight::~ElectronSFWeight ()
{
}
