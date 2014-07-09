#ifndef SF_WEIGHT

#define SF_WEIGHT

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include <cmath>


#include "TH1D.h"
#include "TH2D.h"
#include "TGraphAsymmErrors.h"
#include "TFile.h"

using namespace std;



class TrackSFWeight
{
 public:
  TrackSFWeight () {};
  ~TrackSFWeight ();
  double at (const double &, const int &shiftUpDown = 0);

 private:
  double *trackSFWeight_;
};


class MuonSFWeight
  {
    public:
      MuonSFWeight () {};
      MuonSFWeight (const string &, const string &);
      ~MuonSFWeight ();
      double at (const double &, const double &, const int &shiftUpDown = 0);

    private:
      TH2 *muonSFWeight_;
  };



class ElectronSFWeight
  {
    public:
      ElectronSFWeight () {};
      ElectronSFWeight (const string &, const string &, const string &sfFile = "", const string &dataOverMC = "");
      ~ElectronSFWeight ();
      double at (const double &, const double &, const int &shiftUpDown = 0);

    private:
      string cmsswRelease_;
      string id_;

      TH2F *electronSFWeight_;
  };


class TriggerMetSFWeight 
// Class for assigning a scale factor according to the trigger efficiency as a function of Met
  {
    public:
      TriggerMetSFWeight (const string &sfFile, const string &dataOverMC); 
      ~TriggerMetSFWeight ();
      double at (const double &Met, const int &shiftUpDown = 0);

    private:
      TH1F *triggerMetSFWeight_; 
  };

class TrackNMissOutSFWeight 
// Class for assigning a scale factor according to the number of missing out her hits for the selected track
  {
    public:
      TrackNMissOutSFWeight (const string &sfFile, const string &dataOverMC); 
      ~TrackNMissOutSFWeight ();
      double at (const double &NMissOut, const int &shiftUpDown = 0);

    private:
      TH1F *trackNMissOutSFWeight_; 
  };

class EcaloVarySFWeight
// Class for assigning a scale factor according to the number of missing out her hits for the selected track        
{
 public:
  EcaloVarySFWeight (const string &sfFile, const string &dataOverMC);
  ~EcaloVarySFWeight ();
  double at (const double &EcaloVary, const int &shiftUpDown = 0);

 private:
  TH1F *EcaloVarySFWeight_;
};



class IsrVarySFWeight 
// Class for assigning a scale factor according to the pT of ISR, which is taken to be equivalent to the sum(pT) of all status 3 Susy particles  
  {
    public:  
      IsrVarySFWeight (const string &sfFile, const string &dataOverMC);  
      ~IsrVarySFWeight ();  
      double at (const double &ptSusy, const int &shiftUpDown = 0);  

    private:
      TH1F *isrVarySFWeight_; 
  };

#endif
