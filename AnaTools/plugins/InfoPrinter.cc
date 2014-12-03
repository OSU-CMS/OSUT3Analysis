#include <iostream>

#include "OSUT3Analysis/AnaTools/plugins/InfoPrinter.h"

#define EXIT_CODE 4

InfoPrinter::InfoPrinter (const edm::ParameterSet &cfg) :
  cutDecisions_                (cfg.getParameter<edm::InputTag>         ("cutDecisions")),
  eventsToPrint_               (cfg.getParameter<vector<edm::EventID>>  ("eventsToPrint")),
  printAllEvents_              (cfg.getParameter<bool>                  ("printAllEvents")),
  printCumulativeObjectFlags_  (cfg.getParameter<bool>                  ("printCumulativeObjectFlags")),
  printCutDecision_            (cfg.getParameter<bool>                  ("printCutDecision")),
  printEventDecision_          (cfg.getParameter<bool>                  ("printEventDecision")),
  printEventFlags_             (cfg.getParameter<bool>                  ("printEventFlags")),
  printObjectFlags_            (cfg.getParameter<bool>                  ("printObjectFlags")),
  printTriggerDecision_        (cfg.getParameter<bool>                  ("printTriggerDecision")),
  printTriggerFlags_           (cfg.getParameter<bool>                  ("printTriggerFlags")),
  printVetoTriggerFlags_       (cfg.getParameter<bool>                  ("printVetoTriggerFlags")),
  firstEvent_ (true),
  sw_ (new TStopwatch)
{
  sw_->Start ();
}

InfoPrinter::~InfoPrinter ()
{
  sw_->Stop ();
  outputTime ();
  clog << ss_.str ();
}

void
InfoPrinter::analyze (const edm::Event &event, const edm::EventSetup &setup)
{
  event.getByLabel (cutDecisions_, cutDecisions);
  if (firstEvent_ && !cutDecisions.isValid ())
    clog << "WARNING: failed to retrieve cut decisions from the event." << endl;
  maxCutWidth_ = maxTriggerWidth_ = maxVetoTriggerWidth_ = 0;

  for (auto eventToPrint = eventsToPrint_.begin (); printAllEvents_ || eventToPrint != eventsToPrint_.end (); eventToPrint++)
    {
      if (printAllEvents_ || ((*eventToPrint) == event.id ()))
        {
          ss_ << endl << "================================================================================" << endl;
          ss_ << "\033[1;36minfo for " << event.id () << "\033[0m" << endl;
          printObjectFlags_            &&  printObjectFlags            ();
          printCumulativeObjectFlags_  &&  printCumulativeObjectFlags  ();
          printTriggerFlags_           &&  printTriggerFlags           ();
          printVetoTriggerFlags_       &&  printVetoTriggerFlags       ();
          printEventFlags_             &&  printEventFlags             ();
          printTriggerDecision_        &&  printTriggerDecision        ();
          printCutDecision_            &&  printCutDecision            ();
          printEventDecision_          &&  printEventDecision          ();
          ss_ << "================================================================================" << endl;
        }
      if (printAllEvents_)
        break;
    }

  firstEvent_ = false;
}

bool
InfoPrinter::printEventDecision ()
{
  if (!cutDecisions.isValid ())
    return false;

  ss_ << endl << "\033[1;34mevent decision\033[0m: ";
  if (cutDecisions->eventDecision)
    ss_ << "\033[1;32mtrue\033[0m" << endl;
  else
    ss_ << "\033[1;31mfalse\033[0m" << endl;
  return true;
}

bool
InfoPrinter::printCutDecision ()
{
  if (!cutDecisions.isValid ())
    return false;

  ss_ << endl << "\033[1;34mcut decision\033[0m: ";
  if (cutDecisions->cutDecision)
    ss_ << "\033[1;32mtrue\033[0m" << endl;
  else
    ss_ << "\033[1;31mfalse\033[0m" << endl;
  return true;
}

bool
InfoPrinter::printTriggerDecision ()
{
  if (!cutDecisions.isValid ())
    return false;

  ss_ << endl << "\033[1;34mtrigger decision\033[0m: ";
  if (cutDecisions->triggerDecision)
    ss_ << "\033[1;32mtrue\033[0m" << endl;
  else
    ss_ << "\033[1;31mfalse\033[0m" << endl;
  return true;
}

bool
InfoPrinter::printEventFlags ()
{
  if (!cutDecisions.isValid ())
    return false;

  ss_ << endl;
  !maxCutWidth_ && (maxCutWidth_ = getMaxWidth (cutDecisions->cuts));
  for (auto flag = cutDecisions->eventFlags.begin (); flag != cutDecisions->eventFlags.end (); flag++)
    {
      ss_ << "\033[1;34m" << setw (maxCutWidth_) << left << cutDecisions->cuts.at (flag - cutDecisions->eventFlags.begin ()).name << "\033[0m";
      if (*flag)
        ss_ << "\033[1;32mtrue\033[0m" << endl;
      else
        ss_ << "\033[1;31mfalse\033[0m" << endl;
    }
  return true;
}

bool
InfoPrinter::printCumulativeObjectFlags ()
{
  if (!cutDecisions.isValid ())
    return false;

  ss_ << endl;
  if (!cutDecisions->cumulativeObjectFlags.size ())
    return true;
  vector<string> collections;
  for (const auto &collection : cutDecisions->cumulativeObjectFlags.at (0))
    collections.push_back (collection.first);
  sort (collections.begin (), collections.end ());
  !maxCutWidth_ && (maxCutWidth_ = getMaxWidth (cutDecisions->cuts));
  for (auto collection = collections.begin (); collection != collections.end (); collection++)
    {
      if (collection != collections.begin ())
        ss_ << endl;
      ss_ << "--------------------------------------------------------------------------------" << endl;
      ss_ << "\033[1;35mcumulative object flags for " << *collection << "\033[0m" << endl;
      ss_ << "--------------------------------------------------------------------------------" << endl;
      for (auto cut = cutDecisions->cumulativeObjectFlags.begin (); cut != cutDecisions->cumulativeObjectFlags.end (); cut++)
        {
          ss_ << "\033[1;34m" << setw (maxCutWidth_) << left << cutDecisions->cuts.at (cut - cutDecisions->cumulativeObjectFlags.begin ()).name << "\033[0m";
          for (auto flag = cut->at (*collection).begin (); flag != cut->at (*collection).end (); flag++)
            {
              if (flag != cut->at (*collection).begin ())
                ss_ << ", ";
              if (flag->second)
                {
                  if (flag->first)
                    ss_ << "\033[1;32m1\033[0m";
                  else
                    ss_ << "\033[1;31m0\033[0m";
                }
              else
                ss_ << "\033[1;33m-\033[0m";
            }
          ss_ << endl;
        }
      ss_ << "--------------------------------------------------------------------------------" << endl;
    }

  return true;
}

bool
InfoPrinter::printObjectFlags ()
{
  if (!cutDecisions.isValid ())
    return false;

  ss_ << endl;
  if (!cutDecisions->objectFlags.size ())
    return true;
  vector<string> collections;
  for (const auto &collection : cutDecisions->objectFlags.at (0))
    collections.push_back (collection.first);
  sort (collections.begin (), collections.end ());
  !maxCutWidth_ && (maxCutWidth_ = getMaxWidth (cutDecisions->cuts));
  for (auto collection = collections.begin (); collection != collections.end (); collection++)
    {
      if (collection != collections.begin ())
        ss_ << endl;
      ss_ << "--------------------------------------------------------------------------------" << endl;
      ss_ << "\033[1;35mobject flags for " << *collection << "\033[0m" << endl;
      ss_ << "--------------------------------------------------------------------------------" << endl;
      for (auto cut = cutDecisions->objectFlags.begin (); cut != cutDecisions->objectFlags.end (); cut++)
        {
          ss_ << "\033[1;34m" << setw (maxCutWidth_) << left << cutDecisions->cuts.at (cut - cutDecisions->objectFlags.begin ()).name << "\033[0m";
          for (auto flag = cut->at (*collection).begin (); flag != cut->at (*collection).end (); flag++)
            {
              if (flag != cut->at (*collection).begin ())
                ss_ << ", ";
              if (flag->second)
                {
                  if (flag->first)
                    ss_ << "\033[1;32m1\033[0m";
                  else
                    ss_ << "\033[1;31m0\033[0m";
                }
              else
                ss_ << "\033[1;33m-\033[0m";
            }
          ss_ << endl;
        }
      ss_ << "--------------------------------------------------------------------------------" << endl;
    }

  return true;
}

bool
InfoPrinter::printTriggerFlags ()
{
  if (!cutDecisions.isValid ())
    return false;

  ss_ << endl;
  !maxTriggerWidth_ && (maxTriggerWidth_ = getMaxWidth (cutDecisions->triggers));
  for (auto flag = cutDecisions->triggerFlags.begin (); flag != cutDecisions->triggerFlags.end (); flag++)
    {
      ss_ << "\033[1;34m" << setw (maxTriggerWidth_) << left << cutDecisions->triggers.at (flag - cutDecisions->triggerFlags.begin ()) << "\033[0m";
      if (*flag)
        ss_ << "\033[1;32mtrue\033[0m" << endl;
      else
        ss_ << "\033[1;31mfalse\033[0m" << endl;
    }
  return true;
}

bool
InfoPrinter::printVetoTriggerFlags ()
{
  if (!cutDecisions.isValid ())
    return false;

  ss_ << endl;
  !maxVetoTriggerWidth_ && (maxVetoTriggerWidth_ = getMaxWidth (cutDecisions->triggersToVeto));
  for (auto flag = cutDecisions->vetoTriggerFlags.begin (); flag != cutDecisions->vetoTriggerFlags.end (); flag++)
    {
      ss_ << "\033[1;34m" << setw (maxVetoTriggerWidth_) << left << cutDecisions->triggersToVeto.at (flag - cutDecisions->vetoTriggerFlags.begin ()) << "\033[0m";
      if (*flag)
        ss_ << "\033[1;32mtrue\033[0m" << endl;
      else
        ss_ << "\033[1;31mfalse\033[0m" << endl;
    }
  return true;
}

unsigned
InfoPrinter::getMaxWidth (const vector<string> &list) const
{
  unsigned w = 0;
  for (const auto &s : list)
    {
      if (s.length () > w)
        w = s.length ();
    }
  w += 2;

  return w;
}

unsigned
InfoPrinter::getMaxWidth (const vector<Cut> &list) const
{
  unsigned w = 0;
  for (const auto &cut : list)
    {
      if (cut.name.length () > w)
        w = cut.name.length ();
    }
  w += 2;

  return w;
}

void
InfoPrinter::outputTime ()
{
  ss_ << endl << "================================================================================" << endl;

  double cpu, real;
  int days, hours, minutes;

  cpu = sw_->CpuTime ();
  real = sw_->RealTime ();

  days = (int) (cpu / (60.0 * 60.0 * 24.0));
  cpu -= days * (60.0 * 60.0 * 24.0);
  hours = (int) (cpu / (60.0 * 60.0));
  cpu -= hours * (60.0 * 60.0);
  minutes = (int) (cpu / 60.0);
  cpu -= minutes * 60.0;

  ss_ << "\033[1;34mCPU time\033[0m:  ";
  days && ss_ << days << " days, ";
  (days || hours) && ss_ << hours << " hours, ";
  (days || hours || minutes) && ss_ << minutes << " minutes, ";
  (days || hours || minutes || cpu) && ss_ << cpu << " seconds" << endl;

  days = (int) (real / (60.0 * 60.0 * 24.0));
  real -= days * (60.0 * 60.0 * 24.0);
  hours = (int) (real / (60.0 * 60.0));
  real -= hours * (60.0 * 60.0);
  minutes = (int) (real / 60.0);
  real -= minutes * 60.0;

  ss_ << "\033[1;34mreal time\033[0m: ";
  days && ss_ << days << " days, ";
  (days || hours) && ss_ << hours << " hours, ";
  (days || hours || minutes) && ss_ << minutes << " minutes, ";
  (days || hours || minutes || cpu) && ss_ << real << " seconds" << endl;

  ss_ << "================================================================================" << endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(InfoPrinter);
