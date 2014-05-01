#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip>

#include "TFile.h"
#include "TDirectoryFile.h"
#include "TKey.h"
#include "TH1D.h"
#include "TAxis.h"
#include "TTree.h"

using namespace std;

void printHelp (const string &exeName);
unsigned isEDM (TFile *);

int
main (int argc, char *argv[])
{
  if (argc != 3)
    {
      printHelp (argv[0]);
      return 0;
    }
  string fileName = argv[1],
         histName = argv[2],
         HistName = argv[2];
  TFile *fin;
  HistName[0] = toupper (HistName[0]);
  if (!(fin = TFile::Open (fileName.c_str ())))
    {
      cout << "Failed to open " << fileName << "!" << endl;
      return 0;
    }
  unsigned events = isEDM (fin);
  if (events)
    {
      cout << "EDM events: " << events << endl;
      return 0;
    }
  if (!fin->GetNkeys ())
    {
      cout << fileName << " appears to be empty!" << endl;
      return 0;
    }
  TIter next0 (fin->GetListOfKeys ());
  TObject *obj0;
  vector<TH1D *> cutFlows;
  while ((obj0 = next0 ()))
    {
      string obj0Class = ((TKey *) obj0)->GetClassName (),
             obj0Name = obj0->GetName ();

      if (obj0Class == "TDirectoryFile")
        {
          TDirectoryFile *dir = (TDirectoryFile *) fin->Get (obj0Name.c_str ());
          if (!dir->GetNkeys ())
            {
              cout << fileName << " appears to be empty!" << endl;
              return 0;
            }
          TIter next1 (dir->GetListOfKeys ());
          TObject *obj1;
          while ((obj1 = next1 ()))
            {
              string obj1Class = ((TKey *) obj1)->GetClassName (),
                     obj1Name = obj1->GetName ();

              if (obj1Class == "TH1D" && (obj1Name == histName || obj1Name.substr (obj1Name.length () - HistName.length ()) == HistName))
                {
                  cutFlows.push_back ((TH1D *) dir->Get (obj1Name.c_str ()));
                  cutFlows.back ()->SetDirectory (0);
                }
            }
        }
    }
  if (!cutFlows.size ())
    {
      cout << "Did not find a histogram named " << histName << "!" << endl;
      return 0;
    }
  fin->Close ();

  for (vector<TH1D *>::const_iterator cutFlow = cutFlows.begin (); cutFlow != cutFlows.end (); cutFlow++)
    {
      TAxis *x = (*cutFlow)->GetXaxis ();
      double totalEvents = (*cutFlow)->GetBinContent (1),
             selectedEvents = (*cutFlow)->GetBinContent (x->GetNbins ());
      cout << (*cutFlow)->GetName () << ": " << setprecision (16) << selectedEvents << " / " << setprecision (16) << totalEvents << endl;
    }

  return 0;
}

void
printHelp (const string &exeName)
{
  printf ("Usage: %s FILE HIST\n", exeName.c_str ());
  printf ("Prints the total number of events based on the cutflow in HIST from the given\n");
  printf ("ROOT file.\n");
}

unsigned
isEDM (TFile *f)
{
  unsigned edm = 1;
  TTree *events = 0;

  edm = edm && f->Get ("MetaData");
  edm = edm && f->Get ("ParameterSets");
  edm = edm && f->Get ("Parentage");
  edm = edm && (events = (TTree *) f->Get ("Events"));
  edm = edm && f->Get ("LuminosityBlocks");
  edm = edm && f->Get ("Runs");

  if (edm)
    edm = events->GetEntries ();

  return edm;
}
