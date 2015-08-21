#include <iomanip>
#include <iostream>

#include "OSUT3Analysis/AnaTools/plugins/CutFlowPlotter.h"

#include "TString.h"

#define EXIT_CODE 4

CutFlowPlotter::CutFlowPlotter (const edm::ParameterSet &cfg) :
  cutDecisions_ (cfg.getParameter<edm::InputTag> ("cutDecisions")),
  module_type_  (cfg.getParameter<std::string>("@module_type")),
  module_label_ (cfg.getParameter<std::string>("@module_label")),
  firstEvent_ (true)
{
  assert (strcmp (PROJECT_VERSION, SUPPORTED_VERSION) == 0);

  //////////////////////////////////////////////////////////////////////////////
  // Create a directory for this channel and book the cut flow histograms
  // within.
  //////////////////////////////////////////////////////////////////////////////
  TH1::SetDefaultSumw2 ();
  oneDHists_["eventCounter"]  =  fs_->make<TH1D>  ("eventCounter",  ";;events",          1,  0.0,  1.0);
  oneDHists_["cutFlow"]       =  fs_->make<TH1D>  ("cutFlow",       ";;passing events",  1,  0.0,  1.0);
  oneDHists_["selection"]     =  fs_->make<TH1D>  ("selection",     ";;passing events",  1,  0.0,  1.0);
  oneDHists_["minusOne"]      =  fs_->make<TH1D>  ("minusOne",      ";;passing events",  1,  0.0,  1.0);
  //////////////////////////////////////////////////////////////////////////////
}

CutFlowPlotter::~CutFlowPlotter ()
{

  TString channel = TString(module_label_).ReplaceAll(module_type_, "");
  // module_label_ = channel + module_type_  (module_type_ = "CutFlowPlotter")

  TH1D* cutFlow_   = oneDHists_["cutFlow"];
  TH1D* selection_ = oneDHists_["selection"];
  TH1D* minusOne_  = oneDHists_["minusOne"];

  // Print all the cutflow information stored in histograms when this class is destroyed.
  int totalEvents;
  clog << endl;
  clog.setf(std::ios::fixed);
  uint longestCutName = 30;
  //  uint textWidth = 58;  // including minusOne
  uint textWidth = 42;  // without minusOne

  for (int i=1; i<=cutFlow_->GetNbinsX(); i++) {
    string cutName = cutFlow_->GetXaxis()->GetBinLabel(i);
    if (cutName.size() > longestCutName) longestCutName = cutName.size();
  }
  longestCutName += 2;
  clog << channel << " channel:  " << endl;
  clog << setw (textWidth+longestCutName) << setfill ('-') << '-' << setfill (' ') << endl;
  clog << setw (longestCutName) << left << "Cut Name" << right
       << setw (10) << setprecision(1) << "Events"
       << setw (16) << "Cumul. Eff."
       << setw (16) << "Indiv. Eff."
    //       << setw (16) << "Minus One" // FIXME:  Minus one efficiencies are not correctly calculated.
       << endl;
  clog << setw (textWidth+longestCutName) << setfill ('-') << '-' << setfill (' ') << endl;
  totalEvents = cutFlow_->GetBinContent (1);
  for (int i = 1; i <= cutFlow_->GetNbinsX(); i++) {
    double cutFlow   =   cutFlow_->GetBinContent (i);
    double selection = selection_->GetBinContent (i);
    double minusOne  =  minusOne_->GetBinContent (i);
    minusOne *= 1.0; // Dummy statement to avoid compilation error for unused variable.
    TString name = cutFlow_->GetXaxis()->GetBinLabel(i);
    clog << setw (longestCutName) << left << name << right << setw (10) << setprecision(1) << cutFlow
         << setw (15) << setprecision(3) << 100.0 * (cutFlow   / (double) totalEvents) << "%"
         << setw (15) << setprecision(3) << 100.0 * (selection / (double) totalEvents) << "%"
      //         << setw (15) << setprecision(3) << 100.0 * (minusOne  / (double) totalEvents) << "%"
         << endl;
  }
  clog << setw (textWidth+longestCutName) << setfill ('-') << '-' << setfill (' ') << endl;

}

void
CutFlowPlotter::analyze (const edm::Event &event, const edm::EventSetup &setup)
{
  //////////////////////////////////////////////////////////////////////////////
  // Try to retrieve the cut decisions from the event and print a warning if
  // there is a problem.
  //////////////////////////////////////////////////////////////////////////////
  event.getByLabel (cutDecisions_, cutDecisions);
  if (firstEvent_ && !cutDecisions.isValid ())
    clog << "WARNING: failed to retrieve cut decisions from the event." << endl;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Fill the cut flow histograms, initializing their bins if it is the first
  // event.
  //////////////////////////////////////////////////////////////////////////////
  firstEvent_ && initializeCutFlow ();
  fillCutFlow ();
  firstEvent_ = false;
  //////////////////////////////////////////////////////////////////////////////
}

bool
CutFlowPlotter::initializeCutFlow ()
{
  //////////////////////////////////////////////////////////////////////////////
  // Set the bin label for the first bin, which counts the total number of
  // events. If the cut decisions could not be retrieved from the event, we can
  // do no more, so return false.
  //////////////////////////////////////////////////////////////////////////////
  unsigned bin = 1;
  oneDHists_.at ("cutFlow")->GetXaxis    ()->SetBinLabel  (bin,  "total");
  oneDHists_.at ("selection")->GetXaxis  ()->SetBinLabel  (bin,  "total");
  oneDHists_.at ("minusOne")->GetXaxis   ()->SetBinLabel  (bin,  "total");
  bin++;
  if (!cutDecisions.isValid ())
    return false;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Extend the x-axis of the cut flow histograms to accommodate all the cuts.
  // If triggers have been specified, add a special bin for the trigger
  // decision.
  //////////////////////////////////////////////////////////////////////////////
  unsigned nCuts = cutDecisions->cuts.size ();
  cutDecisions->triggers.size () && nCuts++;
  cutDecisions->triggerFilters.size () && nCuts++;
  oneDHists_.at ("cutFlow")->SetBins    (nCuts + 1,  0.0,  nCuts + 1);
  oneDHists_.at ("selection")->SetBins  (nCuts + 1,  0.0,  nCuts + 1);
  oneDHists_.at ("minusOne")->SetBins   (nCuts + 1,  0.0,  nCuts + 1);
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Set the bin labels for the rest of the bins according to the name of the
  // cut. The special bin for the trigger decision is simply labeled "trigger".
  //////////////////////////////////////////////////////////////////////////////
  if (cutDecisions->triggers.size ())
    {
      oneDHists_.at ("cutFlow")->GetXaxis    ()->SetBinLabel  (bin,  "trigger");
      oneDHists_.at ("selection")->GetXaxis  ()->SetBinLabel  (bin,  "trigger");
      oneDHists_.at ("minusOne")->GetXaxis   ()->SetBinLabel  (bin,  "trigger");
      bin++;
    }
  if (cutDecisions->triggerFilters.size ())
    {
      oneDHists_.at ("cutFlow")->GetXaxis    ()->SetBinLabel  (bin,  "trigger filter");
      oneDHists_.at ("selection")->GetXaxis  ()->SetBinLabel  (bin,  "trigger filter");
      oneDHists_.at ("minusOne")->GetXaxis   ()->SetBinLabel  (bin,  "trigger filter");
      bin++;
    }
  for (vector<Cut>::const_iterator cut = cutDecisions->cuts.begin (); cut != cutDecisions->cuts.end (); cut++, bin++)
    {
      oneDHists_.at ("cutFlow")->GetXaxis    ()->SetBinLabel  (bin,  cut->name.c_str  ());
      oneDHists_.at ("selection")->GetXaxis  ()->SetBinLabel  (bin,  cut->name.c_str  ());
      oneDHists_.at ("minusOne")->GetXaxis   ()->SetBinLabel  (bin,  cut->name.c_str  ());
    }
  //////////////////////////////////////////////////////////////////////////////

  // Return true if the initialization was successful.
  return true;
}

bool
CutFlowPlotter::fillCutFlow (double w)
{
  //////////////////////////////////////////////////////////////////////////////
  // Fill the first bin, which counts the total number of events. If the cut
  // decision could not be retrieved from the event, we can do no more so
  // return false.
  //////////////////////////////////////////////////////////////////////////////
  double bin = 0.5;
  bool passes = true;
  oneDHists_.at ("eventCounter")->Fill  (bin,  w);
  oneDHists_.at ("cutFlow")->Fill       (bin,  w);
  oneDHists_.at ("selection")->Fill     (bin,  w);
  bin++;
  if (!cutDecisions.isValid ())
    return false;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Fill the rest of the bins according to the flags in the cut decisions
  // object.
  //////////////////////////////////////////////////////////////////////////////
  if (cutDecisions->triggers.size ())
    {
      passes = passes && cutDecisions->triggerDecision;
      if (cutDecisions->triggerDecision)
        oneDHists_.at ("selection")->Fill  (bin,  w);
      if (passes)
        oneDHists_.at ("cutFlow")->Fill    (bin,  w);
      bin++;
    }
  if (cutDecisions->triggerFilters.size ())
    {
      passes = passes && cutDecisions->triggerFilterDecision;
      if (cutDecisions->triggerFilterDecision)
        oneDHists_.at ("selection")->Fill  (bin,  w);
      if (passes)
        oneDHists_.at ("cutFlow")->Fill    (bin,  w);
      bin++;
    }
  for (vector<bool>::const_iterator flag = cutDecisions->eventFlags.begin (); flag != cutDecisions->eventFlags.end (); flag++, bin++)
    {
      passes = passes && (*flag);
      if (*flag)
        oneDHists_.at ("selection")->Fill (bin, w);
      if (passes)
        oneDHists_.at ("cutFlow")->Fill (bin, w);
    }
  //////////////////////////////////////////////////////////////////////////////

  // Return true if the filling was successful.
  return true;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CutFlowPlotter);
