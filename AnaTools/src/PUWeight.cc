#include "OSUT3Analysis/AnaTools/interface/PUWeight.h"

PUWeight::PUWeight (const string &puFile, const string &dataPU, const string &mcPU)
{
  TFile *fin = TFile::Open (puFile.c_str ());
  if (!fin || fin->IsZombie()) {
    clog << "ERROR [PUWeight]: Could not find file: " << puFile
         << "; will cause a seg fault." << endl;
    exit(1);
  }

  TH1D *mc;
  fin->GetObject(mcPU.c_str(), mc);
  fin->GetObject(dataPU.c_str(), puWeight_);
  if (!mc) {
    clog << "ERROR [PUWeight]: Could not find histogram: " << mcPU
         << "; will cause a seg fault." << endl;
    exit(1);
  }
  if (!puWeight_) {
    clog << "ERROR [PUWeight]: Could not find histogram: " << dataPU
         << "; will cause a seg fault." << endl;
    exit(1);
  }

  mc->SetDirectory (0);
  puWeight_->SetDirectory (0);
  mc->Scale (puWeight_->Integral () / mc->Integral ());
  TH1D *trimmedMC = new TH1D ("bla", "bla", puWeight_->GetNbinsX(), 0, puWeight_->GetNbinsX());
  for (int bin = 1; bin <= puWeight_->GetNbinsX(); bin++)
    trimmedMC->SetBinContent (bin, mc->GetBinContent (bin));
  puWeight_->Divide (trimmedMC);
  fin->Close ();
  delete fin;
  delete mc;
  delete trimmedMC;
}

PUWeight::~PUWeight ()
{
  delete puWeight_;
}
