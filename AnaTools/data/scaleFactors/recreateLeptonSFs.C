// Author: Brian Francis
// Usage: root -b -q -l recreateLeptonSFs.C

// https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2#Electron_efficiencies_and_scale
void createElectronFile_2015() {

  // Get/declare files

  TFile * fID_tight = new TFile("electron2015/CutBasedID_TightWP_76X_18Feb.txt_SF2D.root");
  TFile * fID_medium = new TFile("electron2015/CutBasedID_MediumWP_76X_18Feb.txt_SF2D.root");
  TFile * fID_loose = new TFile("electron2015/CutBasedID_LooseWP_76X_18Feb.txt_SF2D.root");
  TFile * fID_veto = new TFile("electron2015/CutBasedID_VetoWP_76X_18Feb.txt_SF2D.root");
  TFile * fReco = new TFile("electron2015/eleRECO.txt.egamma_SF2D.root");

  TFile * fOutput = new TFile("electronSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID_tight->Get("EGamma_SF2D");
  TH2D * id_medium = (TH2D*)fID_medium->Get("EGamma_SF2D");
  TH2D * id_loose = (TH2D*)fID_loose->Get("EGamma_SF2D");
  TH2D * id_veto = (TH2D*)fID_veto->Get("EGamma_SF2D");
  TH2D * reco = (TH2D*)fReco->Get("EGamma_SF2D");

  // Write output

  id_tight->Write("electronID2015Tight");
  id_medium->Write("electronID2015Medium");
  id_loose->Write("electronID2015Loose");
  id_veto->Write("electronID2015Veto");
  reco->Write("electronReco2015");

  fOutput->Close();

  fID_tight->Close();
  fID_medium->Close();
  fID_loose->Close();
  fID_veto->Close();
  fReco->Close();
}

void createElectronFile_2016() {

  // Get/declare files

  TFile * fID_veto = new TFile("electron2016/electronIDVeto.root");
  TFile * fID_loose = new TFile("electron2016/electronIDLoose.root");
  TFile * fID_medium = new TFile("electron2016/electronIDMedium.root");
  TFile * fID_tight = new TFile("electron2016/electronIDTight.root");
  TFile * fReco = new TFile("electron2016/electronRECO.root");

  TFile * fOutput = new TFile("electronSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID_tight->Get("EGamma_SF2D");
  TH2D * id_medium = (TH2D*)fID_medium->Get("EGamma_SF2D");
  TH2D * id_loose = (TH2D*)fID_loose->Get("EGamma_SF2D");
  TH2D * id_veto = (TH2D*)fID_veto->Get("EGamma_SF2D");
  TH2D * reco = (TH2D*)fReco->Get("EGamma_SF2D");

  // Write output

  id_tight->Write("electronID2016Tight");
  id_medium->Write("electronID2016Medium");
  id_loose->Write("electronID2016Loose");
  id_veto->Write("electronID2016Veto");
  reco->Write("electronReco2016");

  fOutput->Close();

  fID_tight->Close();
  fID_medium->Close();
  fID_loose->Close();
  fID_veto->Close();
  fReco->Close();
}

void createElectronFile_2017() {

  // Get/declare files
  // No Reco SF for 94X 2017

  TFile * fID_tight = new TFile("electron2017/ElectronCutBasedID_TightWP_94X_run2017BCDEF.root");
  TFile * fID_medium = new TFile("electron2017/ElectronCutBasedID_MediumWP_94X_run2017BCDEF.root");
  TFile * fID_loose = new TFile("electron2017/ElectronCutBasedID_LooseWP_94X_run2017BCDEF.root");
  TFile * fID_veto = new TFile("electron2017/ElectronCutBasedID_VetoWP_94X_run2017BCDEF.root");

  TFile * fOutput = new TFile("electronSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID_tight->Get("EGamma_SF2D");
  TH2D * id_medium = (TH2D*)fID_medium->Get("EGamma_SF2D");
  TH2D * id_loose = (TH2D*)fID_loose->Get("EGamma_SF2D");
  TH2D * id_veto = (TH2D*)fID_veto->Get("EGamma_SF2D");

  // Write output

  id_tight->Write("electronID2017Tight");
  id_medium->Write("electronID2017Medium");
  id_loose->Write("electronID2017Loose");
  id_veto->Write("electronID2017Veto");

  fOutput->Close();

  fID_tight->Close();
  fID_medium->Close();
  fID_loose->Close();
  fID_veto->Close();

}

// https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2#102X_series_Fall17V2_IDs_Scale_f
void createElectronFile_2017_v2() {

  // Get/declare files
  // No veto ID SF available for V2

  TFile * fID_tight = new TFile("electron2017/v2/2017_ElectronTight.root");
  TFile * fID_medium = new TFile("electron2017/v2/2017_ElectronMedium.root");
  TFile * fID_loose = new TFile("electron2017/v2/2017_ElectronLoose.root");
  TFile * fReco = new TFile("electron2017/v2/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root");

  TFile * fOutput = new TFile("electronSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID_tight->Get("EGamma_SF2D");
  TH2D * id_medium = (TH2D*)fID_medium->Get("EGamma_SF2D");
  TH2D * id_loose = (TH2D*)fID_loose->Get("EGamma_SF2D");
  TH2D * reco = (TH2D*)fReco->Get("EGamma_SF2D");

  // Write output

  id_tight->Write("electronID2017TightV2");
  id_medium->Write("electronID2017MediumV2");
  id_loose->Write("electronID2017LooseV2");
  reco->Write("electronReco2017V2");

  fOutput->Close();

  fID_tight->Close();
  fID_medium->Close();
  fID_loose->Close();
  fReco->Close();

}

// https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2#102X_series_Dataset_2018_Autumn
void createElectronFile_2018() {

  // Get/declare files
  // No veto ID SF available for 2018

  TFile * fID_tight = new TFile("electron2018/2018_ElectronTight.root");
  TFile * fID_medium = new TFile("electron2018/2018_ElectronMedium.root");
  TFile * fID_loose = new TFile("electron2018/2018_ElectronLoose.root");
  TFile * fReco = new TFile("electron2018/egammaEffi.txt_EGM2D_updatedAll.root");

  TFile * fOutput = new TFile("electronSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID_tight->Get("EGamma_SF2D");
  TH2D * id_medium = (TH2D*)fID_medium->Get("EGamma_SF2D");
  TH2D * id_loose = (TH2D*)fID_loose->Get("EGamma_SF2D");
  TH2D * reco = (TH2D*)fReco->Get("EGamma_SF2D");

  // Write output

  id_tight->Write("electronID2018Tight");
  id_medium->Write("electronID2018Medium");
  id_loose->Write("electronID2018Loose");
  reco->Write("electronReco2018");

  fOutput->Close();

  fID_tight->Close();
  fID_medium->Close();
  fID_loose->Close();
  fReco->Close();

}

// https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun3 (In progress)
void createElectronFile_2022CD() {

  // Get/declare files

  TFile * fID_tight = new TFile("electron2022CD/egammaEffi_LooseID.txt_EGM2D.root");
  TFile * fID_medium = new TFile("electron2022CD/egammaEffi_MediumID.txt_EGM2D.root");
  TFile * fID_loose = new TFile("electron2022CD/egammaEffi_TightID.txt_EGM2D.root");
  TFile * fReco = new TFile("electron2022CD/egammaEffi.txt_EGM2D_RECO.root");

  TFile * fOutput = new TFile("electronSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID_tight->Get("EGamma_SF2D");
  TH2D * id_medium = (TH2D*)fID_medium->Get("EGamma_SF2D");
  TH2D * id_loose = (TH2D*)fID_loose->Get("EGamma_SF2D");
  TH2D * reco = (TH2D*)fReco->Get("EGamma_SF2D");

  // Write output

  id_tight->Write("electronID2022CDTight");
  id_medium->Write("electronID2022CDMedium");
  id_loose->Write("electronID2022CDLoose");
  reco->Write("electronReco2022CD");

  fOutput->Close();

  fID_tight->Close();
  fID_medium->Close();
  fID_loose->Close();
  fReco->Close();

}

// https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun3 (In progress)
void createElectronFile_2022EFG() {

  // Get/declare files

  TFile * fID_tight = new TFile("electron2022EFG/egammaEffi.txt_EGM2D_ID_loose.root");
  TFile * fID_medium = new TFile("electron2022EFG/egammaEffi.txt_EGM2D_ID_medium.root");
  TFile * fID_loose = new TFile("electron2022EFG/egammaEffi.txt_EGM2D_ID_tight.root");
  TFile * fReco = new TFile("electron2022EFG/egammaEffi.txt_EGM2D_RECO.root");

  TFile * fOutput = new TFile("electronSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID_tight->Get("EGamma_SF2D");
  TH2D * id_medium = (TH2D*)fID_medium->Get("EGamma_SF2D");
  TH2D * id_loose = (TH2D*)fID_loose->Get("EGamma_SF2D");
  TH2D * reco = (TH2D*)fReco->Get("EGamma_SF2D");

  // Write output

  id_tight->Write("electronID2022EFGTight");
  id_medium->Write("electronID2022EFGMedium");
  id_loose->Write("electronID2022EFGLoose");
  reco->Write("electronReco2022EFG");

  fOutput->Close();

  fID_tight->Close();
  fID_medium->Close();
  fID_loose->Close();
  fReco->Close();

}


// https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun3 (In progress)
void createElectronFile_2023C() {

  // Get/declare files

  TFile * fID_tight = new TFile("electron2023C/egammaEffi_LooseID.txt_EGM2D.root");
  TFile * fID_medium = new TFile("electron2023C/egammaEffi_MediumID.txt_EGM2D.root");
  TFile * fID_loose = new TFile("electron2023C/egammaEffi_TightID.txt_EGM2D.root");
  TFile * fReco = new TFile("electron2023C/egammaEffi.txt_EGM2D_RECO.root");

  TFile * fOutput = new TFile("electronSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID_tight->Get("EGamma_SF2D");
  TH2D * id_medium = (TH2D*)fID_medium->Get("EGamma_SF2D");
  TH2D * id_loose = (TH2D*)fID_loose->Get("EGamma_SF2D");
  TH2D * reco = (TH2D*)fReco->Get("EGamma_SF2D");

  // Write output

  id_tight->Write("electronID2023CTight");
  id_medium->Write("electronID2023CMedium");
  id_loose->Write("electronID2023CLoose");
  reco->Write("electronReco2023C");

  fOutput->Close();

  fID_tight->Close();
  fID_medium->Close();
  fID_loose->Close();
  fReco->Close();

}

// https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun3 (In progress)
void createElectronFile_2023D() {

  // Get/declare files

  TFile * fID_tight_noHole = new TFile("electron2023D/NoHole_egammaEffi_LooseID.txt_EGM2D.root");
  TFile * fID_medium_noHole = new TFile("electron2023D/NoHole_egammaEffi_MediumID.txt_EGM2D.root");
  TFile * fID_loose_noHole = new TFile("electron2023D/NoHole_egammaEffi_TightID.txt_EGM2D.root");
  TFile * fID_tight_Hole = new TFile("electron2023D/Hole_egammaEffi_LooseID.txt_EGM2D.root");
  TFile * fID_medium_Hole = new TFile("electron2023D/Hole_egammaEffi_MediumID.txt_EGM2D.root");
  TFile * fID_loose_Hole = new TFile("electron2023D/Hole_egammaEffi_TightID.txt_EGM2D.root");
  TFile * fReco = new TFile("electron2023D/egammaEffi.txt_EGM2D_RECO.root");

  TFile * fOutput = new TFile("electronSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight_noHole = (TH2D*)fID_tight_noHole->Get("EGamma_SF2D");
  TH2D * id_medium_noHole = (TH2D*)fID_medium_noHole->Get("EGamma_SF2D");
  TH2D * id_loose_noHole = (TH2D*)fID_loose_noHole->Get("EGamma_SF2D");
  TH2D * id_tight_Hole = (TH2D*)fID_tight_Hole->Get("EGamma_SF2D");
  TH2D * id_medium_Hole = (TH2D*)fID_medium_Hole->Get("EGamma_SF2D");
  TH2D * id_loose_Hole = (TH2D*)fID_loose_Hole->Get("EGamma_SF2D");
  TH2D * reco = (TH2D*)fReco->Get("EGamma_SF2D");

  // Write output

  id_tight_noHole->Write("NoHoleElectronID2023DTight");
  id_medium_noHole->Write("NoHoleElectronID2023DMedium");
  id_loose_noHole->Write("NoHoleElectronID2023DLoose");
  id_tight_Hole->Write("HoleElectronID2023DTight");
  id_medium_Hole->Write("HoleElectronID2023DMedium");
  id_loose_Hole->Write("HoleElectronID2023DLoose");
  reco->Write("electronReco2023D");

  fOutput->Close();

  fID_tight_noHole->Close();
  fID_medium_noHole->Close();
  fID_loose_noHole->Close();
  fID_tight_Hole->Close();
  fID_medium_Hole->Close();
  fID_loose_Hole->Close();
  fReco->Close();

}


// https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffsRun2#Results_for_CMSSW_7_6_X_dataset
void createMuonSFFile_2015() {

  // Get/declare files

  TFile * fID = new TFile("muon2015/MuonID_Z_RunCD_Reco76X_Feb15.root");
  TFile * fIso = new TFile("muon2015/MuonIso_Z_RunCD_Reco76X_Feb15.root");
  TFile * fTrig = new TFile("muon2015/SingleMuonTrigger_Z_RunCD_Reco76X_Feb15.root");

  TFile * fOutput = new TFile("muonSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID->Get("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/pt_abseta_ratio");
  TH2D * id_medium = (TH2D*)fID->Get("MC_NUM_MediumID_DEN_genTracks_PAR_pt_spliteta_bin1/pt_abseta_ratio");
  TH2D * id_loose = (TH2D*)fID->Get("MC_NUM_LooseID_DEN_genTracks_PAR_pt_spliteta_bin1/pt_abseta_ratio");

  TH2D * iso_looseRel_looseID = (TH2D*)fIso->Get("MC_NUM_LooseRelIso_DEN_LooseID_PAR_pt_spliteta_bin1/pt_abseta_ratio");
  TH2D * iso_looseRel_mediumID = (TH2D*)fIso->Get("MC_NUM_LooseRelIso_DEN_MediumID_PAR_pt_spliteta_bin1/pt_abseta_ratio");
  TH2D * iso_looseRel_tightID = (TH2D*)fIso->Get("MC_NUM_LooseRelIso_DEN_TightID_PAR_pt_spliteta_bin1/pt_abseta_ratio");
  TH2D * iso_tightRel_mediumID = (TH2D*)fIso->Get("MC_NUM_TightRelIso_DEN_MediumID_PAR_pt_spliteta_bin1/pt_abseta_ratio");
  TH2D * iso_tightRel_tightID = (TH2D*)fIso->Get("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/pt_abseta_ratio");

  TH2D * trig_isoMu = (TH2D*)fTrig->Get("runD_IsoMu20_OR_IsoTkMu20_HLTv4p3_PtEtaBins/pt_abseta_ratio");

  // Write output

  id_tight->Write("muonID2015Tight");
  id_medium->Write("muonID2015Medium");
  id_loose->Write("muonID2015Loose");

  iso_looseRel_looseID->Write("muonIso2015LooseRelLooseID");
  iso_looseRel_mediumID->Write("muonIso2015LooseRelMediumID");
  iso_looseRel_tightID->Write("muonIso2015LooseRelTightID");
  iso_tightRel_mediumID->Write("muonIso2015TightRelMediumID");
  iso_tightRel_tightID->Write("muonIso2015TightRelTightID");

  trig_isoMu->Write("muonTrigger2015IsoMu20_OR_IsoTkMu20");

  fOutput->Close();

  fID->Close();
  fIso->Close();
  fTrig->Close();

}

// https://twiki.cern.ch/twiki/bin/view/CMS/MuonWorkInProgressAndPagResults#Results_on_7_6_fb
void createMuonSFFile_2016() {

  // Get/declare files

  TFile * fIDBCDEF = new TFile("muon2016/MuonID_BCDEF.root");
  TFile * fIDGH = new TFile("muon2016/MuonID_GH.root");
  TFile * fIsoBCDEF = new TFile("muon2016/MuonIso_BCDEF.root");
  TFile * fIsoGH = new TFile("muon2016/MuonIso_GH.root");
  TFile * fTracking = new TFile("muon2016/Tracking_2016.root");

  // Get inputs

  TH2D * muIDLooseBCDEF = (TH2D*)fIDBCDEF->Get("MC_NUM_LooseID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  TH2D * muIDLooseGH = (TH2D*)fIDGH->Get("MC_NUM_LooseID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  TH2D * muIDMediumBCDEF = (TH2D*)fIDBCDEF->Get("MC_NUM_MediumID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  TH2D * muIDMediumGH = (TH2D*)fIDGH->Get("MC_NUM_MediumID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  TH2D * muIDTightBCDEF = (TH2D*)fIDBCDEF->Get("MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  TH2D * muIDTightGH = (TH2D*)fIDGH->Get("MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  //  TH2D * muIDHighPtBCDEF = (TH2D*)fIDBCDEF->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  //  TH2D * muIDHighPtGH = (TH2D*)fIDGH->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");

  TH2D * iso_loose_looseID_BCDEF = (TH2D*)fIsoBCDEF->Get("LooseISO_LooseID_pt_eta/pt_abseta_ratio");
  TH2D * iso_loose_mediumID_BCDEF = (TH2D*)fIsoBCDEF->Get("LooseISO_MediumID_pt_eta/pt_abseta_ratio");
  TH2D * iso_loose_tightID_BCDEF = (TH2D*)fIsoBCDEF->Get("LooseISO_TightID_pt_eta/pt_abseta_ratio");
  TH2D * iso_tight_mediumID_BCDEF = (TH2D*)fIsoBCDEF->Get("TightISO_MediumID_pt_eta/pt_abseta_ratio");
  TH2D * iso_tight_tightID_BCDEF = (TH2D*)fIsoBCDEF->Get("TightISO_TightID_pt_eta/pt_abseta_ratio");

  TH2D * iso_loose_looseID_GH = (TH2D*)fIsoGH->Get("LooseISO_LooseID_pt_eta/pt_abseta_ratio");
  TH2D * iso_loose_mediumID_GH = (TH2D*)fIsoGH->Get("LooseISO_MediumID_pt_eta/pt_abseta_ratio");
  TH2D * iso_loose_tightID_GH = (TH2D*)fIsoGH->Get("LooseISO_TightID_pt_eta/pt_abseta_ratio");
  TH2D * iso_tight_mediumID_GH = (TH2D*)fIsoGH->Get("TightISO_MediumID_pt_eta/pt_abseta_ratio");
  TH2D * iso_tight_tightID_GH = (TH2D*)fIsoGH->Get("TightISO_TightID_pt_eta/pt_abseta_ratio");

  TGraphAsymmErrors * muTracking = (TGraphAsymmErrors*)fTracking->Get("ratio_eff_aeta_dr030e030_corr");

  TFile * fCurrent = new TFile("../muonSFs.root");

  TGraphAsymmErrors * muTrackingBCDEF = (TGraphAsymmErrors*)fCurrent->Get("muonTracking2016BCDEF");
  TGraphAsymmErrors * muTrackingGH = (TGraphAsymmErrors*)fCurrent->Get("muonTracking2016GH");

  // Write output

  TFile * fOutput = new TFile("muonSF_new.root", "UPDATE");

  muIDLooseBCDEF->Write("muonID2016LooseBCDEF");
  muIDMediumBCDEF->Write("muonID2016MediumBCDEF");
  muIDTightBCDEF->Write("muonID2016TightBCDEF");
  //  muIDHighPtBCDEF->Write("muonID2016HighPtBCDEF");

  muIDLooseGH->Write("muonID2016LooseGH");
  muIDMediumGH->Write("muonID2016MediumGH");
  muIDTightGH->Write("muonID2016TightGH");
  //  muIDHighPtGH->Write("muonID2016HighPtGH");

  iso_loose_looseID_BCDEF->Write("muonIso2016LooseLooseIDBCDEF");
  iso_loose_mediumID_BCDEF->Write("muonIso2016LooseMediumIDBCDEF");
  iso_loose_tightID_BCDEF->Write("muonIso2016LooseTightIDBCDEF");
  iso_tight_mediumID_BCDEF->Write("muonIso2016TightMediumIDBCDEF");
  iso_tight_tightID_BCDEF->Write("muonIso2016TightTightIDBCDEF");

  iso_loose_looseID_GH->Write("muonIso2016LooseLooseIDGH");
  iso_loose_mediumID_GH->Write("muonIso2016LooseMediumIDGH");
  iso_loose_tightID_GH->Write("muonIso2016LooseTightIDGH");
  iso_tight_mediumID_GH->Write("muonIso2016TightMediumIDGH");
  iso_tight_tightID_GH->Write("muonIso2016TightTightIDGH");

  muTracking->Write("muonTracking2016");
  muTrackingBCDEF->Write("muonTracking2016BCDEF");
  muTrackingGH->Write("muonTracking2016GH");

  fOutput->Close();

  fIDBCDEF->Close();
  fIDGH->Close();
  fIsoBCDEF->Close();
  fIsoGH->Close();
  fTracking->Close();
}

// https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffs2017#Muon_reconstruction_identificati
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/TWikiEXO-MUODocumentationRun2
void createMuonSFFile_2017() {

  // Get/declare files

  TFile * fID = new TFile("muon2017/RunBCDEF_SF_ID.json.root");
  TFile * fIso = new TFile("muon2017/RunBCDEF_SF_ISO.json.root");
  TFile * fTrig = new TFile("muon2017/SingleMuonTrigger_94X_run2017BCDEF.root");

  TFile * fOutput = new TFile("muonSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID->Get("NUM_TightID_DEN_genTracks");
  TH2D * id_medium = (TH2D*)fID->Get("NUM_MediumID_DEN_genTracks");
  TH2D * id_loose = (TH2D*)fID->Get("NUM_LooseID_DEN_genTracks");

  TH2D * iso_looseRel_looseID = (TH2D*)fIso->Get("NUM_LooseRelIso_DEN_LooseID");
  TH2D * iso_looseRel_mediumID = (TH2D*)fIso->Get("NUM_LooseRelIso_DEN_MediumID");
  TH2D * iso_looseRel_tightID = (TH2D*)fIso->Get("NUM_LooseRelIso_DEN_TightIDandIPCut");
  TH2D * iso_tightRel_mediumID = (TH2D*)fIso->Get("NUM_TightRelIso_DEN_MediumID");
  TH2D * iso_tightRel_tightID = (TH2D*)fIso->Get("NUM_TightRelIso_DEN_TightIDandIPCut");

  TH2D * muIsolatedTrigger = (TH2D*)fTrig->Get("IsoMu27_PtEtaBins/pt_abseta_ratio");
  TH2D * muNonisolatedTrigger = (TH2D*)fTrig->Get("Mu50_PtEtaBins/pt_abseta_ratio");

  // Write output

  id_tight->Write("muonID2017Tight");
  id_medium->Write("muonID2017Medium");
  id_loose->Write("muonID2017Loose");

  iso_looseRel_looseID->Write("muonIso2017LooseLooseID");
  iso_looseRel_mediumID->Write("muonIso2017LooseMediumID");
  iso_looseRel_tightID->Write("muonIso2017LooseTightID");
  iso_tightRel_mediumID->Write("muonIso2017TightMediumID");
  iso_tightRel_tightID->Write("muonIso2017TightTightID");

  muIsolatedTrigger->Write("muonTrigger2017IsoMu27");
  muNonisolatedTrigger->Write("muonTrigger2017Mu50");

  fOutput->Close();

  fID->Close();
  fIso->Close();
  fTrig->Close();
}

// https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffs2018
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/TWikiEXO-MUODocumentationRun2
void createMuonSFFile_2018() {

  // Get/declare files

  TFile * fID = new TFile("muon2018/RunABCD_SF_ID.json.root");
  TFile * fIso = new TFile("muon2018/RunABCD_SF_ISO.json.root");
  TFile * fTrigBefore = new TFile("muon2018/EfficienciesAndSF_2018Data_BeforeMuonHLTUpdate.root");
  TFile * fTrigAfter = new TFile("muon2018/EfficienciesAndSF_2018Data_AfterMuonHLTUpdate.root");

  TFile * fOutput = new TFile("muonSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID->Get("NUM_TightID_DEN_genTracks");
  TH2D * id_medium = (TH2D*)fID->Get("NUM_MediumID_DEN_genTracks");
  TH2D * id_loose = (TH2D*)fID->Get("NUM_LooseID_DEN_genTracks");

  TH2D * iso_looseRel_looseID = (TH2D*)fIso->Get("NUM_LooseRelIso_DEN_LooseID");
  TH2D * iso_looseRel_mediumID = (TH2D*)fIso->Get("NUM_LooseRelIso_DEN_MediumID");
  TH2D * iso_looseRel_tightID = (TH2D*)fIso->Get("NUM_LooseRelIso_DEN_TightIDandIPCut");
  TH2D * iso_tightRel_mediumID = (TH2D*)fIso->Get("NUM_TightRelIso_DEN_MediumID");
  TH2D * iso_tightRel_tightID = (TH2D*)fIso->Get("NUM_TightRelIso_DEN_TightIDandIPCut");

  TH2D * muIsolatedTriggerBefore = (TH2D*)fTrigBefore->Get("IsoMu24_PtEtaBins/pt_abseta_ratio");
  TH2D * muNonisolatedTriggerBefore = (TH2D*)fTrigBefore->Get("Mu50_OR_OldMu100_OR_TkMu100_PtEtaBins/pt_abseta_ratio");

  TH2D * muIsolatedTriggerAfter = (TH2D*)fTrigAfter->Get("IsoMu24_PtEtaBins/pt_abseta_ratio");
  TH2D * muNonisolatedTriggerAfter = (TH2D*)fTrigAfter->Get("Mu50_OR_OldMu100_OR_TkMu100_PtEtaBins/pt_abseta_ratio");

  // Lumi-weighted average trigger SFs
  double totalIntLumi = 59.613; //total 2018 golden json integrated lumi [/fb]
  double beforeIntLumi = 8.936; //2018 golden json integrated lumi before muon HLT update [/fb]
  double IntLumiA = 13.955; //2018A golden json integrated lumi [/fb]

  const double fracBeforeIntLumiABCD = 1.0*beforeIntLumi/totalIntLumi;
  const double fracAfterIntLumiABCD = 1.0*(totalIntLumi-beforeIntLumi)/totalIntLumi;
  const double fracBeforeIntLumiA = 1.0*beforeIntLumi/IntLumiA;
  const double fracAfterIntLumiA = 1.0*(IntLumiA-beforeIntLumi)/IntLumiA;

  TH2D * muIsolatedTriggerBeforeScaledABCD = (TH2D*)muIsolatedTriggerBefore->Clone();
  muIsolatedTriggerBeforeScaledABCD->Scale(fracBeforeIntLumiABCD);
  TH2D * muIsolatedTriggerAfterScaledABCD = (TH2D*)muIsolatedTriggerAfter->Clone();
  muIsolatedTriggerAfterScaledABCD->Scale(fracAfterIntLumiABCD);
  TH2D muIsolatedTriggerLumiWeightedAveABCD = (*muIsolatedTriggerBeforeScaledABCD) + (*muIsolatedTriggerAfterScaledABCD);

  TH2D * muNonisolatedTriggerBeforeScaledABCD = (TH2D*)muNonisolatedTriggerBefore->Clone();
  muNonisolatedTriggerBeforeScaledABCD->Scale(fracBeforeIntLumiABCD);
  TH2D * muNonisolatedTriggerAfterScaledABCD = (TH2D*)muNonisolatedTriggerAfter->Clone();
  muNonisolatedTriggerAfterScaledABCD->Scale(fracAfterIntLumiABCD);
  TH2D muNonisolatedTriggerLumiWeightedAveABCD = (*muNonisolatedTriggerBeforeScaledABCD) + (*muNonisolatedTriggerAfterScaledABCD);

  TH2D * muIsolatedTriggerBeforeScaledA = (TH2D*)muIsolatedTriggerBefore->Clone();
  muIsolatedTriggerBeforeScaledA->Scale(fracBeforeIntLumiA);
  TH2D * muIsolatedTriggerAfterScaledA = (TH2D*)muIsolatedTriggerAfter->Clone();
  muIsolatedTriggerAfterScaledA->Scale(fracAfterIntLumiA);
  TH2D muIsolatedTriggerLumiWeightedAveA = (*muIsolatedTriggerBeforeScaledA) + (*muIsolatedTriggerAfterScaledA);

  TH2D * muNonisolatedTriggerBeforeScaledA = (TH2D*)muNonisolatedTriggerBefore->Clone();
  muNonisolatedTriggerBeforeScaledA->Scale(fracBeforeIntLumiA);
  TH2D * muNonisolatedTriggerAfterScaledA = (TH2D*)muNonisolatedTriggerAfter->Clone();
  muNonisolatedTriggerAfterScaledA->Scale(fracAfterIntLumiA);
  TH2D muNonisolatedTriggerLumiWeightedAveA = (*muNonisolatedTriggerBeforeScaledA) + (*muNonisolatedTriggerAfterScaledA);


  // Write output

  id_tight->Write("muonID2018Tight");
  id_medium->Write("muonID2018Medium");
  id_loose->Write("muonID2018Loose");

  iso_looseRel_looseID->Write("muonIso2018LooseLooseID");
  iso_looseRel_mediumID->Write("muonIso2018LooseMediumID");
  iso_looseRel_tightID->Write("muonIso2018LooseTightID");
  iso_tightRel_mediumID->Write("muonIso2018TightMediumID");
  iso_tightRel_tightID->Write("muonIso2018TightTightID");

  muIsolatedTriggerBefore->Write("muonTrigger2018IsoMu24BeforeMuonHLTUpdate");
  muNonisolatedTriggerBefore->Write("muonTrigger2018Mu50OROldMu100ORTkMu100BeforeMuonHLTUpdate");
  muIsolatedTriggerAfter->Write("muonTrigger2018IsoMu24AfterMuonHLTUpdate");
  muNonisolatedTriggerAfter->Write("muonTrigger2018Mu50OROldMu100ORTkMu100AfterMuonHLTUpdate");

  muIsolatedTriggerLumiWeightedAveABCD.Write("muonTrigger2018IsoMu24LumiWeightedAveABCD");
  muNonisolatedTriggerLumiWeightedAveABCD.Write("muonTrigger2018Mu50OROldMu100ORTkMu100LumiWeightedAveABCD");
  muIsolatedTriggerLumiWeightedAveA.Write("muonTrigger2018IsoMu24LumiWeightedAveA");
  muNonisolatedTriggerLumiWeightedAveA.Write("muonTrigger2018Mu50OROldMu100ORTkMu100LumiWeightedAveA");

  fOutput->Close();

  fID->Close();
  fIso->Close();
  fTrigBefore->Close();
  fTrigAfter->Close();
}

// https://twiki.cern.ch/twiki/bin/view/CMS/MuonRun32022
void createMuonSFFile_2022CD() {

  // Get/declare files

  TFile * fID = new TFile("muon2022CD/RunCD_SF_ID.json.root");
  TFile * fIso = new TFile("muon2022CD/RunCD_SF_ISO.json.root");
  TFile * fTrig = new TFile("muon2022CD/RunCD_SF_Trig.json.root");

  TFile * fOutput = new TFile("muonSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID->Get("NUM_TightID_DEN_TrackerMuons");
  TH2D * id_medium = (TH2D*)fID->Get("NUM_MediumID_DEN_TrackerMuons");
  TH2D * id_loose = (TH2D*)fID->Get("NUM_LooseID_DEN_TrackerMuons");

  TH2D * iso_looseRel_looseID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_LooseID");
  TH2D * iso_looseRel_mediumID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_MediumID");
  TH2D * iso_looseRel_tightID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_TightID");
  TH2D * iso_tightRel_mediumID = (TH2D*)fIso->Get("NUM_TightPFIso_DEN_MediumID");
  TH2D * iso_tightRel_tightID = (TH2D*)fIso->Get("NUM_TightPFIso_DEN_TightID");

  TH2D * muIsolatedTrigger = (TH2D*)fTrig->Get("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight");
  TH2D * muNonisolatedTrigger = (TH2D*)fTrig->Get("NUM_Mu50_or_CascadeMu100_or_HighPtTkMu100_DEN_CutBasedIdTrkHighPt_and_TkIsoLoose");

  // Write output

  id_tight->Write("muonID2022CDTight");
  id_medium->Write("muonID2022CDMedium");
  id_loose->Write("muonID2022CDLoose");

  iso_looseRel_looseID->Write("muonIso2022CDLooseLooseID");
  iso_looseRel_mediumID->Write("muonIso2022CDLooseMediumID");
  iso_looseRel_tightID->Write("muonIso2022CDLooseTightID");
  iso_tightRel_mediumID->Write("muonIso2022CDTightMediumID");
  iso_tightRel_tightID->Write("muonIso2022CDTightTightID");

  muIsolatedTrigger->Write("muonTrigger2022CDIsoMu24");
  muNonisolatedTrigger->Write("muonTrigger2022CDMu50");

  fOutput->Close();

  fID->Close();
  fIso->Close();
  fTrig->Close();
}

// https://twiki.cern.ch/twiki/bin/view/CMS/MuonRun32022
void createMuonSFFile_2022EFG() {

  // Get/declare files

  TFile * fID = new TFile("muon2022EFG/RunEFG_SF_ID.json.root");
  TFile * fIso = new TFile("muon2022EFG/RunEFG_SF_ISO.json.root");
  TFile * fTrig = new TFile("muon2022EFG/RunEFG_SF_Trig.json.root");

  TFile * fOutput = new TFile("muonSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID->Get("NUM_TightID_DEN_TrackerMuons");
  TH2D * id_medium = (TH2D*)fID->Get("NUM_MediumID_DEN_TrackerMuons");
  TH2D * id_loose = (TH2D*)fID->Get("NUM_LooseID_DEN_TrackerMuons");

  TH2D * iso_looseRel_looseID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_LooseID");
  TH2D * iso_looseRel_mediumID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_MediumID");
  TH2D * iso_looseRel_tightID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_TightID");
  TH2D * iso_tightRel_mediumID = (TH2D*)fIso->Get("NUM_TightPFIso_DEN_MediumID");
  TH2D * iso_tightRel_tightID = (TH2D*)fIso->Get("NUM_TightPFIso_DEN_TightID");

  TH2D * muIsolatedTrigger = (TH2D*)fTrig->Get("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight");
  TH2D * muNonisolatedTrigger = (TH2D*)fTrig->Get("NUM_Mu50_or_CascadeMu100_or_HighPtTkMu100_DEN_CutBasedIdTrkHighPt_and_TkIsoLoose");

  // Write output

  id_tight->Write("muonID2022EFGTight");
  id_medium->Write("muonID2022EFGMedium");
  id_loose->Write("muonID2022EFGLoose");

  iso_looseRel_looseID->Write("muonIso2022EFGLooseLooseID");
  iso_looseRel_mediumID->Write("muonIso2022EFGLooseMediumID");
  iso_looseRel_tightID->Write("muonIso2022EFGLooseTightID");
  iso_tightRel_mediumID->Write("muonIso2022EFGTightMediumID");
  iso_tightRel_tightID->Write("muonIso2022EFGTightTightID");

  muIsolatedTrigger->Write("muonTrigger2022EFGIsoMu24");
  muNonisolatedTrigger->Write("muonTrigger2022EFGMu50");

  fOutput->Close();

  fID->Close();
  fIso->Close();
  fTrig->Close();
}

// https://twiki.cern.ch/twiki/bin/view/CMS/MuonRun3_2023
void createMuonSFFile_2023C() {

  // Get/declare files

  TFile * fID = new TFile("muon2023C/RunC_SF_ID.json.root");
  TFile * fIso = new TFile("muon2023C/RunC_SF_ISO.json.root");
  TFile * fTrig = new TFile("muon2023C/RunC_SF_Trig.json.root");

  TFile * fOutput = new TFile("muonSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID->Get("NUM_TightID_DEN_TrackerMuons");
  TH2D * id_medium = (TH2D*)fID->Get("NUM_MediumID_DEN_TrackerMuons");
  TH2D * id_loose = (TH2D*)fID->Get("NUM_LooseID_DEN_TrackerMuons");

  TH2D * iso_looseRel_looseID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_LooseID");
  TH2D * iso_looseRel_mediumID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_MediumID");
  TH2D * iso_looseRel_tightID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_TightID");
  TH2D * iso_tightRel_mediumID = (TH2D*)fIso->Get("NUM_TightPFIso_DEN_MediumID");
  TH2D * iso_tightRel_tightID = (TH2D*)fIso->Get("NUM_TightPFIso_DEN_TightID");

  TH2D * muIsolatedTrigger = (TH2D*)fTrig->Get("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight");
  TH2D * muNonisolatedTrigger = (TH2D*)fTrig->Get("NUM_Mu50_or_CascadeMu100_or_HighPtTkMu100_DEN_CutBasedIdTrkHighPt_and_TkIsoLoose");

  // Write output

  id_tight->Write("muonID2023CTight");
  id_medium->Write("muonID2023CMedium");
  id_loose->Write("muonID2023CLoose");

  iso_looseRel_looseID->Write("muonIso2023CLooseLooseID");
  iso_looseRel_mediumID->Write("muonIso2023CLooseMediumID");
  iso_looseRel_tightID->Write("muonIso2023CLooseTightID");
  iso_tightRel_mediumID->Write("muonIso2023CTightMediumID");
  iso_tightRel_tightID->Write("muonIso2023CTightTightID");

  muIsolatedTrigger->Write("muonTrigger2023CIsoMu24");
  muNonisolatedTrigger->Write("muonTrigger2023CMu50");

  fOutput->Close();

  fID->Close();
  fIso->Close();
  fTrig->Close();
}

// https://twiki.cern.ch/twiki/bin/view/CMS/MuonRun3_2023
void createMuonSFFile_2023D() {

  // Get/declare files

  TFile * fID = new TFile("muon2023D/RunD_SF_ID.json.root");
  TFile * fIso = new TFile("muon2023D/RunD_SF_ISO.json.root");
  TFile * fTrig = new TFile("muon2023D/RunD_SF_Trig.json.root");

  TFile * fOutput = new TFile("muonSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID->Get("NUM_TightID_DEN_TrackerMuons");
  TH2D * id_medium = (TH2D*)fID->Get("NUM_MediumID_DEN_TrackerMuons");
  TH2D * id_loose = (TH2D*)fID->Get("NUM_LooseID_DEN_TrackerMuons");

  TH2D * iso_looseRel_looseID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_LooseID");
  TH2D * iso_looseRel_mediumID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_MediumID");
  TH2D * iso_looseRel_tightID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_TightID");
  TH2D * iso_tightRel_mediumID = (TH2D*)fIso->Get("NUM_TightPFIso_DEN_MediumID");
  TH2D * iso_tightRel_tightID = (TH2D*)fIso->Get("NUM_TightPFIso_DEN_TightID");

  TH2D * muIsolatedTrigger = (TH2D*)fTrig->Get("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight");
  TH2D * muNonisolatedTrigger = (TH2D*)fTrig->Get("NUM_Mu50_or_CascadeMu100_or_HighPtTkMu100_DEN_CutBasedIdTrkHighPt_and_TkIsoLoose");

  // Write output

  id_tight->Write("muonID2023DTight");
  id_medium->Write("muonID2023DMedium");
  id_loose->Write("muonID2023DLoose");

  iso_looseRel_looseID->Write("muonIso2023DLooseLooseID");
  iso_looseRel_mediumID->Write("muonIso2023DLooseMediumID");
  iso_looseRel_tightID->Write("muonIso2023DLooseTightID");
  iso_tightRel_mediumID->Write("muonIso2023DTightMediumID");
  iso_tightRel_tightID->Write("muonIso2023DTightTightID");

  muIsolatedTrigger->Write("muonTrigger2023DIsoMu24");
  muNonisolatedTrigger->Write("muonTrigger2023DMu50");

  fOutput->Close();

  fID->Close();
  fIso->Close();
  fTrig->Close();
}

void createElectronFile_2024(){
  // Get/declare files

  TFile * fID_loose = new TFile("electron2024/merged_EGamma_SF2D_Loose.root");
  TFile * fID_medium = new TFile("electron2024/merged_EGamma_SF2D_Medium.root");
  TFile * fID_tight = new TFile("electron2024/merged_EGamma_SF2D_Tight.root");
  TFile * fID_veto = new TFile("electron2024/merged_EGamma_SF2D_Veto.root");

  TFile * fReco = new TFile("electron2024/egammaEffi.txt_EGM2D_RECO.root");

  TFile * fOutput = new TFile("electronSF_new.root", "UPDATE");

    // Get inputs

  TH2D * id_tight = (TH2D*)fID_tight->Get("EGamma_SF2D");
  TH2D * id_medium = (TH2D*)fID_medium->Get("EGamma_SF2D");
  TH2D * id_loose = (TH2D*)fID_loose->Get("EGamma_SF2D");
  TH2D * reco = (TH2D*)fReco->Get("EGamma_SF2D");
  TH2D * id_veto = (TH2D*)fID_veto->Get("EGamma_SF2D");

  fOutput->Close();

  fID_tight->Close();
  fID_medium->Close();
  fID_loose->Close();
  fID_veto->Close();
  fReco->Close();


}
void createMuonSFFile_2024(){
  // Get/declare files

  TFile * fID = new TFile("muon2024/RunD_SF_ID.json.root");
  TFile * fIso = new TFile("muon2024/RunD_SF_ISO.json.root");
  TFile * fTrig = new TFile("muon2024/RunD_SF_Trig.json.root");

  TFile * fOutput = new TFile("muonSF_new.root", "UPDATE");

  // Get inputs

  TH2D * id_tight = (TH2D*)fID->Get("NUM_TightID_DEN_TrackerMuons");
  TH2D * id_medium = (TH2D*)fID->Get("NUM_MediumID_DEN_TrackerMuons");
  TH2D * id_loose = (TH2D*)fID->Get("NUM_LooseID_DEN_TrackerMuons");

  TH2D * iso_looseRel_looseID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_LooseID");
  TH2D * iso_looseRel_mediumID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_MediumID");
  TH2D * iso_looseRel_tightID = (TH2D*)fIso->Get("NUM_LoosePFIso_DEN_TightID");
  TH2D * iso_tightRel_mediumID = (TH2D*)fIso->Get("NUM_TightPFIso_DEN_MediumID");
  TH2D * iso_tightRel_tightID = (TH2D*)fIso->Get("NUM_TightPFIso_DEN_TightID");

  TH2D * muIsolatedTrigger = (TH2D*)fTrig->Get("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight");
  TH2D * muNonisolatedTrigger = (TH2D*)fTrig->Get("NUM_Mu50_or_CascadeMu100_or_HighPtTkMu100_DEN_CutBasedIdTrkHighPt_and_TkIsoLoose");

  // Write output

  id_tight->Write("muonID2023DTight");
  id_medium->Write("muonID2023DMedium");
  id_loose->Write("muonID2023DLoose");

  iso_looseRel_looseID->Write("muonIso2023DLooseLooseID");
  iso_looseRel_mediumID->Write("muonIso2023DLooseMediumID");
  iso_looseRel_tightID->Write("muonIso2023DLooseTightID");
  iso_tightRel_mediumID->Write("muonIso2023DTightMediumID");
  iso_tightRel_tightID->Write("muonIso2023DTightTightID");

  muIsolatedTrigger->Write("muonTrigger2023DIsoMu24");
  muNonisolatedTrigger->Write("muonTrigger2023DMu50");

  fOutput->Close();

  fID->Close();
  fIso->Close();
  fTrig->Close();
}


void recreateLeptonSFs() {
  createElectronFile_2015();
  createElectronFile_2016();
  createElectronFile_2017();
  createElectronFile_2017_v2();
  createElectronFile_2018();
  createElectronFile_2022CD();
  createElectronFile_2022EFG();
  createElectronFile_2023C();
  createElectronFile_2023D();
  createElectronFile_2024();

  // createMuonSFFile_2015();
  // createMuonSFFile_2016();
  // createMuonSFFile_2017();
  // createMuonSFFile_2018();
  // createMuonSFFile_2022CD();
  // createMuonSFFile_2022EFG();
  // createMuonSFFile_2023C();
  // createMuonSFFile_2023D();
}
