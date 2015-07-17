#include <iomanip>
#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"
#include "OSUT3Analysis/AnaTools/plugins/InfoPrinter.h"

#define EXIT_CODE 4

InfoPrinter::InfoPrinter (const edm::ParameterSet &cfg) :
  collections_                 (cfg.getParameter<edm::ParameterSet>      ("collections")),
  cutDecisions_                (cfg.getParameter<edm::InputTag>          ("cutDecisions")),
  eventsToPrint_               (cfg.getParameter<vector<edm::EventID> >  ("eventsToPrint")),
  printAllEvents_              (cfg.getParameter<bool>                   ("printAllEvents")),
  printCumulativeObjectFlags_  (cfg.getParameter<bool>                   ("printCumulativeObjectFlags")),
  printCutDecision_            (cfg.getParameter<bool>                   ("printCutDecision")),
  printEventDecision_          (cfg.getParameter<bool>                   ("printEventDecision")),
  printEventFlags_             (cfg.getParameter<bool>                   ("printEventFlags")),
  printObjectFlags_            (cfg.getParameter<bool>                   ("printObjectFlags")),
  printTriggerDecision_        (cfg.getParameter<bool>                   ("printTriggerDecision")),
  printTriggerFlags_           (cfg.getParameter<bool>                   ("printTriggerFlags")),
  printVetoTriggerFlags_       (cfg.getParameter<bool>                   ("printVetoTriggerFlags")),
  printAllTriggers_            (cfg.getParameter<bool>                   ("printAllTriggers")),
  valuesToPrint_               (cfg.getParameter<edm::VParameterSet>     ("valuesToPrint")),
  firstEvent_ (true),
  counter_ (0),
  sw_ (new TStopwatch)
{
  // Start the timer.
  sw_->Start ();

  unpackValuesToPrint ();
}

InfoPrinter::~InfoPrinter ()
{
  //////////////////////////////////////////////////////////////////////////////
  // Stop the timer and output the time, as well as any additional information
  // requested by the user.
  //////////////////////////////////////////////////////////////////////////////
  sw_->Stop ();
  outputTime ();
  clog << ss_.str ();
  //////////////////////////////////////////////////////////////////////////////

  for (auto &value : valuesToPrint)
    delete value.valueLookupTree;
  delete sw_;
}

void
InfoPrinter::analyze (const edm::Event &event, const edm::EventSetup &setup)
{
  counter_++;
  anatools::getRequiredCollections (objectsToGet_, collections_, handles_, event);

  //////////////////////////////////////////////////////////////////////////////
  // Get the cut decisions out of the event.
  //////////////////////////////////////////////////////////////////////////////
  event.getByLabel (cutDecisions_, cutDecisions);
  if (firstEvent_ && !cutDecisions.isValid ())
    clog << "WARNING: failed to retrieve cut decisions from the event." << endl;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Set all the private variables in the ValueLookup object before using it,
  // and parse the cut strings in the unpacked cuts into ValueLookupTree
  // objects.
  //////////////////////////////////////////////////////////////////////////////
  if (!initializeValueLookupForest (valuesToPrint, &handles_))
    {
      clog << "ERROR: failed to parse all cut strings. Quitting..." << endl;
      exit (EXIT_CODE);
    }
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // For each type of information requested by the user, and for each event
  // requested, print that information to the stringstream which is printed in
  // the destructor.
  //////////////////////////////////////////////////////////////////////////////
  maxCutWidth_ = maxTriggerWidth_ = maxVetoTriggerWidth_ = maxValueWidth_ = maxAllTriggerWidth_ = 0;
  for (auto eventToPrint = eventsToPrint_.begin (); printAllEvents_ || eventToPrint != eventsToPrint_.end (); eventToPrint++)
    {
      if (printAllEvents_ || ((*eventToPrint) == event.id ()))
        {
          ss_ << endl << "================================================================================" << endl;
          ss_ << "\033[1;36minfo for " << event.id () << " (record " << counter_ << ")\033[0m" << endl;
          valuesToPrint.size ()        &&  printValuesToPrint          ();
          printObjectFlags_            &&  printObjectFlags            ();
          printCumulativeObjectFlags_  &&  printCumulativeObjectFlags  ();
          printTriggerFlags_           &&  printTriggerFlags           ();
          printVetoTriggerFlags_       &&  printVetoTriggerFlags       ();
          printEventFlags_             &&  printEventFlags             ();
          printTriggerDecision_        &&  printTriggerDecision        ();
          printCutDecision_            &&  printCutDecision            ();
          printEventDecision_          &&  printEventDecision          ();
          printAllTriggers_            &&  printAllTriggers            (event);
          ss_ << "================================================================================" << endl;
        }
      if (printAllEvents_)
        break;
    }
  //////////////////////////////////////////////////////////////////////////////

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

bool
InfoPrinter::printValuesToPrint ()
{
  ss_ << endl;
  ss_ << "--------------------------------------------------------------------------------" << endl;
  ss_ << "\033[1;35mvalues to print\033[0m" << endl;
  ss_ << "--------------------------------------------------------------------------------" << endl;
  !maxValueWidth_ && (maxValueWidth_ = getMaxWidth (valuesToPrint));
  for (const auto &valueToPrint : valuesToPrint)
    {
      ss_ << "\033[1;34m" << setw (maxValueWidth_) << left << (valueToPrint.inputLabel + ": " + valueToPrint.valueToPrint) << "\033[0m";
      for (auto value = valueToPrint.valueLookupTree->evaluate ().begin (); value != valueToPrint.valueLookupTree->evaluate ().end (); value++)
        {
          if (value != valueToPrint.valueLookupTree->evaluate ().begin ())
            ss_ << ", ";
          double v = boost::get<double> (*value);
          if (v > numeric_limits<int>::min () + 1)
            ss_ << v;
          else
            ss_ << "-";
        }
      ss_ << endl;
    }
  ss_ << "--------------------------------------------------------------------------------" << endl;

  return true;
}

bool
InfoPrinter::printAllTriggers (const edm::Event &event)
{
  ss_ << endl;
  ss_ << "--------------------------------------------------------------------------------" << endl;
  ss_ << "\033[1;35mavailable triggers\033[0m" << endl;
  ss_ << "--------------------------------------------------------------------------------" << endl;
  map<string, bool> triggers;
#if DATA_FORMAT == BEAN
  for (const auto &trigger : *handles_.triggers)
    {
      string name = trigger.name;
      bool pass = trigger.pass;
#elif DATA_FORMAT == MINI_AOD || DATA_FORMAT == AOD
  const edm::TriggerNames &triggerNames = event.triggerNames (*handles_.triggers);
  for (unsigned i = 0; i < triggerNames.size (); i++)
    {
      string name = triggerNames.triggerName (i);
      bool pass = handles_.triggers->accept (i);
#else
  #error "Data format is not valid."
#endif
      triggers[name] = pass;
    }
  !maxAllTriggerWidth_ && (maxAllTriggerWidth_ = getMaxWidth (triggers));
  for (const auto &trigger : triggers)
    {
      ss_ << "\033[1;34m" << setw (maxAllTriggerWidth_) << left << trigger.first << "\033[0m";
      if (trigger.second)
        ss_ << "\033[1;32maccept\033[0m" << endl;
      else
        ss_ << "\033[1;31mreject\033[0m" << endl;
    }

  return true;
}

unsigned
InfoPrinter::getMaxWidth (const vector<string> &list) const
{
  //////////////////////////////////////////////////////////////////////////////
  // Calculate the maximum length of the strings in a vector.
  //////////////////////////////////////////////////////////////////////////////
  unsigned w = 0;
  for (const auto &s : list)
    {
      if (s.length () > w)
        w = s.length ();
    }
  //////////////////////////////////////////////////////////////////////////////

  // Add two to the maximum length so that there are at least two spaces before
  // the next column in the table.
  w += 2;

  return w;
}

unsigned
InfoPrinter::getMaxWidth (const map<string, bool> &list) const
{
  //////////////////////////////////////////////////////////////////////////////
  // Calculate the maximum length of the strings in a vector.
  //////////////////////////////////////////////////////////////////////////////
  unsigned w = 0;
  for (const auto &s : list)
    {
      if (s.first.length () > w)
        w = s.first.length ();
    }
  //////////////////////////////////////////////////////////////////////////////

  // Add two to the maximum length so that there are at least two spaces before
  // the next column in the table.
  w += 2;

  return w;
}

unsigned
InfoPrinter::getMaxWidth (const Cuts &list) const
{
  //////////////////////////////////////////////////////////////////////////////
  // Calculate the maximum length of the names of cuts in a vector.
  //////////////////////////////////////////////////////////////////////////////
  unsigned w = 0;
  for (const auto &cut : list)
    {
      if (cut.name.length () > w)
        w = cut.name.length ();
    }
  //////////////////////////////////////////////////////////////////////////////

  // Add two to the maximum length so that there are at least two spaces before
  // the next column in the table.
  w += 2;

  return w;
}

unsigned
InfoPrinter::getMaxWidth (const ValuesToPrint &list) const
{
  unsigned w = 0;
  for (const auto &value : list)
    {
      if ((value.inputLabel + ": " + value.valueToPrint).length () > w)
        w = (value.inputLabel + ": " + value.valueToPrint).length ();
    }

  w += 2;

  return w;
}

void
InfoPrinter::outputTime ()
{
  ss_ << endl << "================================================================================" << endl;

  double cpu, real, cpuPerEvt, realPerEvt;
  int days, hours, minutes;

  real = sw_->RealTime ();
  cpu = sw_->CpuTime ();
  cpuPerEvt  =  cpu / counter_;
  realPerEvt = real / counter_;

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
  (days || hours || minutes || cpu) && ss_ << cpu << " seconds";
  ss_ << "  ("  <<  cpuPerEvt << " seconds per event)" << endl;


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
  (days || hours || minutes || cpu) && ss_ << real << " seconds";
  ss_ << "  ("  <<  realPerEvt << " seconds per event)" << endl;

  ss_ << "================================================================================" << endl;
}

void
InfoPrinter::unpackValuesToPrint ()
{
  for (const auto &value : valuesToPrint_)
    {
      valuesToPrint.push_back (ValueToPrint ());
      valuesToPrint.back ().inputCollections = value.getParameter<vector<string> > ("inputCollection");
      sort (valuesToPrint.back ().inputCollections.begin (), valuesToPrint.back ().inputCollections.end ());
      valuesToPrint.back ().inputLabel = anatools::concatenateInputCollection (valuesToPrint.back ().inputCollections);
      valuesToPrint.back ().valueToPrint = value.getParameter<string> ("valueToPrint");

      objectsToGet_.insert (valuesToPrint.back ().inputCollections.begin (), valuesToPrint.back ().inputCollections.end ());
    }
  if (printAllTriggers_)
    objectsToGet_.insert ("triggers");
}

bool
InfoPrinter::initializeValueLookupForest (ValuesToPrint &values, Collections * const handles)
{
  for (auto &value : values)
    {
      if (firstEvent_)
        {
          value.valueLookupTree = new ValueLookupTree (value);
          if (!value.valueLookupTree->isValid ())
            return false;
        }
      value.valueLookupTree->setCollections (handles);
    }
  return true;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(InfoPrinter);
