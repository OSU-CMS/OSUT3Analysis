#include "OSUT3Analysis/AnaTools/interface/CutFlow.h"

CutFlow::CutFlow (const edm::Service<TFileService> &fs, const string &prefix)
{
  sw_.Start ();
  string cutFlowName = "cutFlow",
         selectionName = "selection",
         minusOneName = "minusOne";

  prefix_ = prefix;
  if (prefix_ != "")
    {
      cutFlowName = prefix_ + "CutFlow";
      selectionName = prefix_ + "Selection";
      minusOneName = prefix_ + "MinusOne";
    }

  TH1::SetDefaultSumw2 ();
  cutFlow_ = fs->make<TH1D> (cutFlowName.c_str (), "", 1, 0.0, 1.0);
  selection_ = fs->make<TH1D> (selectionName.c_str (), "", 1, 0.0, 1.0);
  minusOne_ = fs->make<TH1D> (minusOneName.c_str (), "", 1, 0.0, 1.0);

  cutFlow_->GetXaxis ()->SetBinLabel (1, "Total");
  selection_->GetXaxis ()->SetBinLabel (1, "Total");
  minusOne_->GetXaxis ()->SetBinLabel (1, "Total");
}

CutFlow::~CutFlow ()
{
  sw_.Stop ();
  outputTime ();
  outputCutFlow ();
}

bool &
CutFlow::operator[] (const string &cutName)
{
  if (!cuts_.count (cutName))
    {
      cutNames_.push_back (cutName);
      cuts_[cutName] = false;

      cutFlow_->SetBins (cutNames_.size () + 1, 0.0, cutNames_.size () + 1.0);
      selection_->SetBins (cutNames_.size () + 1, 0.0, cutNames_.size () + 1.0);
      minusOne_->SetBins (cutNames_.size () + 1, 0.0, cutNames_.size () + 1.0);

      cutFlow_->GetXaxis ()->SetBinLabel (cutNames_.size () + 1, cutName.c_str ());
      selection_->GetXaxis ()->SetBinLabel (cutNames_.size () + 1, cutName.c_str ());
      minusOne_->GetXaxis ()->SetBinLabel (cutNames_.size () + 1, cutName.c_str ());
    }

  return cuts_[cutName];
}

bool
CutFlow::pass () const
{
  for (map<string, bool>::const_iterator cut = cuts_.begin (); cut != cuts_.end (); cut++)
    {
      if (!cut->second)
        return false;
    }
  return true;
}

void
CutFlow::fillCutFlow (double w)
{
  bool fillCumulative = true, fillComplement = true;
  double complement = -1.0;

  cutFlow_->Fill (0.5, w);
  selection_->Fill (0.5, w);
  minusOne_->Fill (0.5, w);
  for (vector<string>::const_iterator cut = cutNames_.begin (); cut != cutNames_.end (); cut++)
    {
      if (cuts_[*cut])
        {
          double binCenter = selection_->GetBinCenter (cut - cutNames_.begin () + 2);

          selection_->Fill (binCenter, w);
          if (fillCumulative)
            cutFlow_->Fill (binCenter, w);
        }
      else
        {
          fillCumulative = false;
          if (complement < 0.0)
            complement = minusOne_->GetBinCenter (cut - cutNames_.begin () + 2);
          else
            fillComplement = false;
        }
    }
  if (fillCumulative)
    {
      for (vector<string>::const_iterator cut = cutNames_.begin (); cut != cutNames_.end (); cut++)
        {
          double binCenter = minusOne_->GetBinCenter (cut - cutNames_.begin () + 2);

          minusOne_->Fill (binCenter, w);
        }
    }
  if (!fillCumulative && fillComplement)
    minusOne_->Fill (complement, w);
}

void
CutFlow::outputTime ()
{
  double cpu, real;
  int days, hours, minutes;

  cpu = sw_.CpuTime ();
  real = sw_.RealTime ();

  days = (int) (cpu / (60.0 * 60.0 * 24.0));
  cpu -= days * (60.0 * 60.0 * 24.0);
  hours = (int) (cpu / (60.0 * 60.0));
  cpu -= hours * (60.0 * 60.0);
  minutes = (int) (cpu / 60.0);
  cpu -= minutes * 60.0;

  clog << endl;
  clog << "=============================================" << endl;
  clog << endl;

  clog << "CPU Time:  ";
  if (days)
    clog << days << " days, ";
  if (days || hours)
    clog << hours << " hours, ";
  if (days || hours || minutes)
    clog << minutes << " minutes, ";
  if (days || hours || minutes || cpu)
    clog << cpu << " seconds" << endl;

  days = (int) (real / (60.0 * 60.0 * 24.0));
  real -= days * (60.0 * 60.0 * 24.0);
  hours = (int) (real / (60.0 * 60.0));
  real -= hours * (60.0 * 60.0);
  minutes = (int) (real / 60.0);
  real -= minutes * 60.0;

  clog << "Real Time: ";
  if (days)
    clog << days << " days, ";
  if (days || hours)
    clog << hours << " hours, ";
  if (days || hours || minutes)
    clog << minutes << " minutes, ";
  if (days || hours || minutes || cpu)
    clog << real << " seconds" << endl;
}

void
CutFlow::outputCutFlow ()
{
  int totalEvents;
  string title = prefix_;

  clog << endl;
  clog << "=============================================" << endl;
  clog << endl;

  clog.setf(std::ios::fixed);

  if (prefix_ != "")
    {
      title[0] = toupper (title[0]);
      title += " Cuts";
      clog << title << endl;
    }

  uint longestCutName = 0;
  for (vector<string>::const_iterator cut = cutNames_.begin (); cut != cutNames_.end (); cut++){
    if((*cut).size() > longestCutName) longestCutName = (*cut).size();
  }
  longestCutName += 1;

  clog << setw (58+longestCutName) << setfill ('-') << '-' << setfill (' ') << endl;
  clog << setw (longestCutName) << left << "Cut Name" << right << setw (10) << setprecision(1) << "Events" << setw (16) << "Cumul. Eff." << setw (16) << "Indiv. Eff." << setw (16) << "Minus One" << endl;
  clog << setw (58+longestCutName) << setfill ('-') << '-' << setfill (' ') << endl;
  totalEvents = cutFlow_->GetBinContent (1);
  clog << setw (longestCutName) << left << "Total:" << right << setw (10) << setprecision(1) << (double) totalEvents << setw (16) << "100%    " << setw (16) << "100%    " << setw (16) << "0%    " << endl;
  for (vector<string>::const_iterator cut = cutNames_.begin (); cut != cutNames_.end (); cut++)
    {
      double cutFlow = cutFlow_->GetBinContent (cut - cutNames_.begin () + 2),
             selection = selection_->GetBinContent (cut - cutNames_.begin () + 2),
             minusOne = minusOne_->GetBinContent (cut - cutNames_.begin () + 2);

      clog << setw (longestCutName) << left << (*cut + ":") << right << setw (10) << setprecision(1) << cutFlow << setw (15) << setprecision(3) << 100.0 * (cutFlow / (double) totalEvents) << "%"
	   << setw (15) << setprecision(3) << 100.0 * (selection / (double) totalEvents) << "%"
	   << setw (15) << setprecision(3) << 100.0 * (minusOne / (double) totalEvents) << "%" << endl;
    }
  clog << setw (58+longestCutName) << setfill ('-') << '-' << setfill (' ') << endl;
}
