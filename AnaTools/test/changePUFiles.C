// Description:
// Change histograms from one root file to another.  
//
// Usage:
//     root -l -b -q 'changePUFiles.C+' |& tee changePUFiles.log
//
//     Or:
//     root [0] .x changePUFiles.C  
//
//     Or:
//     root [0] .L changePUFiles.C+  
//     root [1] changePUFiles
// 
// Modified from copyPUFiles.C written by Wells Wulsin.
//
// Copyright Information:
//      Copyright (C) 2013         OSU
//---------------------------------------------------------------
#include <iostream>  
using std::cout;
using std::endl;

#include "TCanvas.h" 
#include "TFile.h" 
#include "TH1.h" 
#include "TString.h" 

#ifndef __CINT__
#endif

void changePUFiles();
void changeOneFile(TFile* fin, TFile* fout, TString originalhistname, TString newhistname);
//-----------------
// Functions
//-----------------
void changePUFiles() {
  cout << "ChangePUFiles world." << endl;
  //Give the CMSSW working directory.
  TString dirCmssw = "/home/bing/CMSSW_6_1_2/src/";  
  //Give the path of the input file which includes the histograms you want to use.
  TFile *fin  = new TFile(dirCmssw+"DisplacedSUSY/BackgroundStudies/test/condor/condor_PU/pu.root", "READ"); 
  //Give the path of the output file which includes the histogram you want to change.
  TFile *fout = new TFile(dirCmssw+"OSUT3Analysis/Configuration/data/pu.root", "UPDATE"); 
  // changeOneFile(inputfile, outputfile,"histogram name in input file","histofram name in putput file") 
  changeOneFile(fin, fout, "W0jets", "Wjets");  


  fin ->Close();
  fout->Close();

  return;

} 


void changeOneFile(TFile* fin, TFile* fout, TString originalhistname, TString newhistname) {

  cout << "Changing histogram " << originalhistname << endl;  

  TH1* h;
  fin->GetObject(newhistname, h);
  if (!h) { 
    cout << "Could not find hist named " << newhistname << "in the input file"<< endl;
    return;
  }
  fout->cd();
  fout->Delete("W0jets*;*");
  fout->Delete(originalhistname + ";*");
  if (newhistname != originalhistname) {
    h->SetName(originalhistname);  
    cout << "Resetting name to be = " << h->GetName()
	 << endl;  
  }
  h->Write();

}







