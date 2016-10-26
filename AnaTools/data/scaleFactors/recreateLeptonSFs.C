// Author: Brian Francis
// Usage: root -b -q -l recreateLeptonSFs.C
// Purpose: This macro combines the relevant lepton scale factors in the various ROOT files here
//          and takes special care to keep the older "TightID" and "RecoTightID" unchanged and unnamed.
//          This is to add updated scale factors (see twiki links herein) and not disturb analyzers using the older versions.
//          Running this macro should just reproduce ../*SF.root as ./*SF_new.root

// Returns id*reco
TH2D * ElectronProduct(TH2D * id, TH2D * reco, TString name) {

  TH2D * h = (TH2D*)reco->Clone(name);

  for(int i = 1; i <= reco->GetNbinsX(); i++) {
    for(int j = 1; j <= reco->GetNbinsY(); j++) {

      double x = reco->GetBinContent(i, j);
      double ex = reco->GetBinError(i, j);

      Int_t idBin_x = id->GetXaxis()->FindBin(fabs(reco->GetXaxis()->GetBinCenter(i)));
      Int_t idBin_y = id->GetYaxis()->FindBin(reco->GetYaxis()->GetBinCenter(j));

      Int_t idBin = id->FindBin(fabs(reco->GetXaxis()->GetBinCenter(i)),
				                        reco->GetYaxis()->GetBinCenter(j));

      double y = id->GetBinContent(idBin_x, idBin_y);
      double ey = id->GetBinError(idBin_x, idBin_y);

      h->SetBinContent(i, j, x * y);
      h->SetBinError(i, j, TMath::Sqrt(y*y*ex*ex + x*x*ey*ey));
    }
  }

  return h;
}

// In the 80X ICHEP series, the binning for GSF tracking efficiency (reco) is all messed up
//    finer eta binning than for the ID SF
//    only one PT bin (20 - 200)
// So just take binning from the ID histo and fetch values at that (pt, eta)...
//     always use PT>20 (ybin=1) from the GSF (reco) part
//        ...this could cause problems for PT<20 electrons if you allow them...
TH2D * ElectronProduct80X(TH2D * id, TH2D * reco, TString name) {

  TH2D * h = (TH2D*)id->Clone(name);

  for(int i = 1; i <= id->GetNbinsX(); i++) {
    for(int j = 1; j <= id->GetNbinsY(); j++) {

      double x = id->GetBinContent(i, j);
      double ex = id->GetBinError(i, j);

      // reco's x-axis is binner more finely than id's
      // so average all the bins that are contained in the range of id's binning...
      Int_t recoBin_lo = reco->GetXaxis()->FindBin(id->GetXaxis()->GetBinLowEdge(i));
      Int_t recoBin_hi = reco->GetXaxis()->FindBin(id->GetXaxis()->GetBinLowEdge(i+1));

      int ny = 0;
      double y = 0;
      double ey = 0;

      for(int k = recoBin_lo; k < recoBin_hi; k++) {
	       ny++;
	       y += reco->GetBinContent(k, 1);
	       ey += reco->GetBinError(k, 1) * reco->GetBinError(k, 1);
      }
      y /= ny;
      ey = TMath::Sqrt(ey) / ny;

      // now take id*reco finally
      h->SetBinContent(i, j, x * y);
      h->SetBinError(i, j, TMath::Sqrt(y*y*ex*ex + x*x*ey*ey));
    }
  }

  return h;
}

// returns a*b -- assumes binning is the same between them
TH2D * MuonProduct(TH2D * a, TH2D * b, TString name) {

  TH2D * h = (TH2D*)a->Clone(name);

  for(int i = 0; i <= a->GetNbinsX()+1; i++) {
    for(int j = 0; j <= a->GetNbinsY()+1; j++) {
      double x = a->GetBinContent(i, j);
      double ex = a->GetBinError(i, j);

      double y = b->GetBinContent(i, j);
      double ey = b->GetBinError(i, j);

      h->SetBinContent(i, j, x * y);
      h->SetBinError(i, j, TMath::Sqrt(y*y*ex*ex + x*x*ey*ey));
    }
  }

  return h;
}

// Add <syst> percent to the error as prescribed by twiki
void AddExtraSystematic(TH2D *& h, double syst) {

  for(int i = 0; i <= h->GetNbinsX(); i++) {
    for(int j = 0; j <= h->GetNbinsY(); j++) {

      double value = h->GetBinContent(i, j);
      double error = h->GetBinError(i, j);

      double extraError = syst * value;

      h->SetBinError(i, j, TMath::Sqrt(error*error + extraError*extraError));
    }
  }

}

// https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2#Electron_efficiencies_and_scale
void createElectronFile_76X() {

  // Get/declare files

  TFile * fID_tight = new TFile("CutBasedID_TightWP_76X_18Feb.txt_SF2D.root");
  TFile * fID_medium = new TFile("CutBasedID_MediumWP_76X_18Feb.txt_SF2D.root");
  TFile * fID_loose = new TFile("CutBasedID_LooseWP_76X_18Feb.txt_SF2D.root");
  TFile * fID_veto = new TFile("CutBasedID_VetoWP_76X_18Feb.txt_SF2D.root");

  TFile * fReco = new TFile("eleRECO.txt.egamma_SF2D.root");

  TFile * fCurrent = new TFile("../electronSF.root");

  TFile * fOutput = new TFile("electronSF_new.root", "RECREATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID_tight->Get("EGamma_SF2D");
  TH2D * id_medium = (TH2D*)fID_medium->Get("EGamma_SF2D");
  TH2D * id_loose = (TH2D*)fID_loose->Get("EGamma_SF2D");
  TH2D * id_veto = (TH2D*)fID_veto->Get("EGamma_SF2D");

  TH2D * reco = (TH2D*)fReco->Get("EGamma_SF2D");

  TH2D * currentTightID = (TH2D*)fCurrent->Get("TightID");
  TH2D * currentRecoTightID = (TH2D*)fCurrent->Get("RecoTightID");

  // Products

  TH2D * recoID_tight = ElectronProduct(id_tight, reco, "RecoTightID_76X");
  TH2D * recoID_medium = ElectronProduct(id_medium, reco, "RecoMediumID_76X");
  TH2D * recoID_loose = ElectronProduct(id_loose, reco, "RecoLooseID_76X");
  TH2D * recoID_veto = ElectronProduct(id_veto, reco, "RecoVetoID_76X");

  // Write output

  id_tight->Write("TightID_76X");
  id_medium->Write("MediumID_76X");
  id_loose->Write("LooseID_76X");
  id_veto->Write("VetoID_76X");

  recoID_tight->Write("RecoTightID_76X");
  recoID_medium->Write("RecoMediumID_76X");
  recoID_loose->Write("RecoLooseID_76X");
  recoID_veto->Write("RecoVetoID_76X");

  currentTightID->Write("TightID");
  currentRecoTightID->Write("RecoTightID");

  fOutput->Close();

  fID_tight->Close();
  fID_medium->Close();
  fID_loose->Close();
  fID_veto->Close();

}

void createElectronFile_80X() {

  // Get/declare files

  TFile * fID_tight = new TFile("CutBasedID_TightWP_80X_ICHEP.root");
  TFile * fID_medium = new TFile("CutBasedID_MediumWP_80X_ICHEP.root");
  TFile * fID_loose = new TFile("CutBasedID_LooseWP_80X_ICHEP.root");
  TFile * fID_veto = new TFile("CutBasedID_VetoWP_80X_ICHEP.root");

  TFile * fReco = new TFile("eleGSFTracking_80X_ICHEP.root");

  TFile * fOutput = new TFile("electronSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID_tight->Get("EGamma_SF2D");
  TH2D * id_medium = (TH2D*)fID_medium->Get("EGamma_SF2D");
  TH2D * id_loose = (TH2D*)fID_loose->Get("EGamma_SF2D");
  TH2D * id_veto = (TH2D*)fID_veto->Get("EGamma_SF2D");

  TH2D * reco = (TH2D*)fReco->Get("EGamma_SF2D");

  // Products

  TH2D * recoID_tight = ElectronProduct80X(id_tight, reco, "RecoTightID_80X");
  TH2D * recoID_medium = ElectronProduct80X(id_medium, reco, "RecoMediumID_80X");
  TH2D * recoID_loose = ElectronProduct80X(id_loose, reco, "RecoLooseID_80X");
  TH2D * recoID_veto = ElectronProduct80X(id_veto, reco, "RecoVetoID_80X");

  // Write output

  id_tight->Write("TightID_80X");
  id_medium->Write("MediumID_80X");
  id_loose->Write("LooseID_80X");
  id_veto->Write("VetoID_80X");

  recoID_tight->Write("RecoTightID_80X");
  recoID_medium->Write("RecoMediumID_80X");
  recoID_loose->Write("RecoLooseID_80X");
  recoID_veto->Write("RecoVetoID_80X");

  fOutput->Close();

  fID_tight->Close();
  fID_medium->Close();
  fID_loose->Close();
  fID_veto->Close();

}

// https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffsRun2#Results_for_CMSSW_7_6_X_dataset
void createMuonSFFile_76X() {

  // Get/declare files

  TFile * fID = new TFile("MuonID_Z_RunCD_Reco76X_Feb15.root");
  TFile * fIso = new TFile("MuonIso_Z_RunCD_Reco76X_Feb15.root");
  TFile * fTrig = new TFile("SingleMuonTrigger_Z_RunCD_Reco76X_Feb15.root");

  TFile * fCurrent = new TFile("../muonSF.root");

  TFile * fOutput = new TFile("muonSF_new.root", "RECREATE");

  // Get inputs

  TH2D * muID = (TH2D*)fID->Get("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/pt_abseta_ratio");
  TH2D * muIso = (TH2D*)fIso->Get("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/pt_abseta_ratio");
  TH2D * muTrigD = (TH2D*)fTrig->Get("runD_IsoMu20_OR_IsoTkMu20_HLTv4p3_PtEtaBins/pt_abseta_ratio");

  TH2D * currentTightID = (TH2D*)fCurrent->Get("TightID");
  TH2D * currentTightIDIso = (TH2D*)fCurrent->Get("TightIDIso");

  // Add extra systematics (see twiki)
  AddExtraSystematic(muID, 0.01);
  AddExtraSystematic(muIso, 0.005);
  AddExtraSystematic(muTrigD, 0.005);

  // Products

  TH2D * idIso = MuonProduct(muID, muIso, "TightIDIso_76X");
  TH2D * idIsoTrig = MuonProduct(idIso, muTrigD, "TightIDIsoTrigD_76X");

  // Write output

  muID->Write("TightID_76X");
  idIso->Write("TightIDIso_76X");
  idIsoTrig->Write("TightIDIsoTrig_76X");

  currentTightID->Write("TightID");
  currentTightIDIso->Write("TightIDIso");

  fOutput->Close();

  fID->Close();
  fIso->Close();
  fTrig->Close();

}

// https://twiki.cern.ch/twiki/bin/view/CMS/MuonWorkInProgressAndPagResults#Results_on_7_6_fb
// Trigger SFs are only half-done (only data) currently
void createMuonSFFile_80X() {

  // Get/declare files

  TFile * fID = new TFile("MuonID_Z_RunBCD_prompt80X_7p65.root");
  TFile * fIso = new TFile("MuonIso_Z_RunBCD_prompt80X_7p65.root");

  TFile * fOutput = new TFile("muonSF_new.root", "UPDATE");

  // Get inputs

  TH2D * muID = (TH2D*)fID->Get("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/pt_abseta_ratio");
  TH2D * muIso = (TH2D*)fIso->Get("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/pt_abseta_ratio");

  // Products

  TH2D * idIso = MuonProduct(muID, muIso, "TightIDIso_76X");

  // Write output

  muID->Write("TightID_80X");
  idIso->Write("TightIDIso_80X");

  fOutput->Close();

  fID->Close();
  fIso->Close();

}

void recreateLeptonSFs() {
  createElectronFile_76X();
  createElectronFile_80X();

  createMuonSFFile_76X();
  createMuonSFFile_80X();
}
