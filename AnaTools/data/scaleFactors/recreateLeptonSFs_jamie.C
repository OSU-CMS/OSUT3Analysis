// Author: Brian Francis
// Usage: root -b -q -l recreateLeptonSFs.C
// Purpose: This macro combines the relevant lepton scale factors in the various ROOT files here
//          and takes special care to keep the older "TightID" and "RecoTightID" unchanged and unnamed.
//          This is to add updated scale factors (see twiki links herein) and not disturb analyzers using the older versions.
//          Running this macro should just reproduce ../*SF.root as ./*SF_new.root


void createElectronFile() {

  // Get/declare files

  TFile * fID_veto = new TFile("electronIDVeto.root");
  TFile * fID_loose = new TFile("electronIDLoose.root");
  TFile * fID_medium = new TFile("electronIDMedium.root");
  TFile * fID_tight = new TFile("electronIDTight.root");
  TFile * fReco = new TFile("electronRECO.root");

  // Get inputs

  TH2D * id_veto = (TH2D*)fID_veto->Get("EGamma_SF2D");
  TH2D * id_loose = (TH2D*)fID_loose->Get("EGamma_SF2D");
  TH2D * id_medium = (TH2D*)fID_medium->Get("EGamma_SF2D");
  TH2D * id_tight = (TH2D*)fID_tight->Get("EGamma_SF2D");
  TH2D * reco = (TH2D*)fReco->Get("EGamma_SF2D");

  // Write output
  TFile * fOutput = new TFile("electronSFs.root", "RECREATE");

  id_veto->Write("electronID2016Veto");
  id_loose->Write("electronID2016Loose");
  id_medium->Write("electronID2016Medium");
  id_tight->Write("electronID2016Tight");
  reco->Write("electronReco2016");

  fOutput->Close();

  fID_veto->Close();
  fID_loose->Close();
  fID_medium->Close();
  fID_tight->Close();
  fReco->Close();

}


void createMuonSFFile() {

  // Get/declare files

  TFile * fIDBCDEF = new TFile("MuonID_BCDEF.root");
  TFile * fIDGH = new TFile("MuonID_GH.root");
  TFile * fIsoBCDEF = new TFile("MuonIso_BCDEF.root");
  TFile * fIsoGH = new TFile("MuonIso_GH.root");
  TFile * fTracking = new TFile("Tracking_2016.root");


  // Get inputs

  TH2D * muIDLooseBCDEF = (TH2D*)fIDBCDEF->Get("MC_NUM_LooseID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  TH2D * muIDLooseGH = (TH2D*)fIDGH->Get("MC_NUM_LooseID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  TH2D * muIDMediumBCDEF = (TH2D*)fIDBCDEF->Get("MC_NUM_MediumID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  TH2D * muIDMediumGH = (TH2D*)fIDGH->Get("MC_NUM_MediumID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  TH2D * muIDTightBCDEF = (TH2D*)fIDBCDEF->Get("MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  TH2D * muIDTightGH = (TH2D*)fIDGH->Get("MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  //  TH2D * muIDHighPtBCDEF = (TH2D*)fIDBCDEF->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  //  TH2D * muIDHighPtGH = (TH2D*)fIDGH->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");

  TH2D * muIsoBCDEF = (TH2D*)fIsoBCDEF->Get("TightISO_TightID_pt_eta/pt_abseta_ratio");
  TH2D * muIsoGH = (TH2D*)fIsoGH->Get("TightISO_TightID_pt_eta/pt_abseta_ratio");

  TH2D * muTracking = (TH2D*)fTracking->Get("ratio_eff_aeta_dr030e030_corr");

  // Write output

  TFile * fOutput = new TFile("muonSFs.root", "RECREATE");

  muIDLooseBCDEF->Write("muonID2016LooseBCDEF");
  muIDLooseGH->Write("muonID2016LooseGH");
  muIDMediumBCDEF->Write("muonID2016MediumBCDEF");
  muIDMediumGH->Write("muonID2016MediumGH");
  muIDTightBCDEF->Write("muonID2016TightBCDEF");
  muIDTightGH->Write("muonID2016TightGH");
  //  muIDHighPtBCDEF->Write("muonID2016HighPtBCDEF");
  //  muIDHighPtGH->Write("muonID2016HighPtGH");

  muIsoBCDEF->Write("muonIso2016TightBCDEF");
  muIsoGH->Write("muonIso2016TightGH");

  muTracking->Write("muonReco2016");

  fOutput->Close();

  fIDBCDEF->Close();
  fIDGH->Close();
  fIsoBCDEF->Close();
  fIsoGH->Close();
  fTracking->Close();

}

void recreateLeptonSFs_jamie() {
  createElectronFile();
  createMuonSFFile();
}
