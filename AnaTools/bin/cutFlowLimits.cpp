#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cstdlib>

#include "TFile.h"
#include "TDirectoryFile.h"
#include "TKey.h"
#include "TH1D.h"
#include "TAxis.h"
#include "TMath.h"

#define ALPHA 0.68

using namespace std;

void printHelp (const string &exeName);
void getLimits (TH1D *, TDirectoryFile *);
void parseOptions (int, char *[], map<string, string> &, vector<string> &);

int
main (int argc, char *argv[])
{
  map<string, string> opt;
  vector<string> argVector;
  parseOptions (argc, argv, opt, argVector);
  if (argVector.size () != 1 || opt.count ("help"))
    {
      printHelp (argv[0]);
      return 0;
    }
  TFile *fin;
  if (!(fin = TFile::Open (argVector.at (0).c_str (), "update")))
    {
      cout << "Failed to open " << argVector.at (0) << "!" << endl;
      return 0;
    }
  TIter next0 (fin->GetListOfKeys ());
  TObject *obj0;
  TH1D *cutFlow = 0;
  TDirectoryFile *dir = 0;
  while ((obj0 = next0 ()))
    {
      string obj0Class = ((TKey *) obj0)->GetClassName (),
             obj0Name = obj0->GetName ();

      if (obj0Class == "TDirectoryFile")
        {
          dir = (TDirectoryFile *) fin->Get (obj0Name.c_str ());
          TIter next1 (dir->GetListOfKeys ());
          TObject *obj1;
          while ((obj1 = next1 ()))
            {
              string obj1Class = ((TKey *) obj1)->GetClassName (),
                     obj1Name = obj1->GetName ();

              if (obj1Class == "TH1D" && obj1Name.length () >= 7 && ((obj1Name.substr (obj1Name.length () - 7, 7) == "CutFlow" || obj1Name.substr (obj1Name.length () - 7, 7) == "cutFlow")
               || (obj1Name.length () >= 9 && (obj1Name.substr (obj1Name.length () - 9, 9) == "Selection" || obj1Name.substr (obj1Name.length () - 9, 9) == "selection"))
               || (obj1Name.length () >= 8 && (obj1Name.substr (obj1Name.length () - 8, 8) == "MinusOne" || obj1Name.substr (obj1Name.length () - 8, 8) == "minusOne")))
               && !dir->Get ((obj1Name + "LowerLimit").c_str ()) && !dir->Get ((obj1Name + "UpperLimit").c_str ()))
                {
                  cutFlow = (TH1D *) dir->Get (obj1Name.c_str ());
                  cutFlow->SetDirectory (0);
                  getLimits (cutFlow, dir);
                }
            }
        }
    }
  fin->Close ();

  return 0;
}

void
printHelp (const string &exeName)
{
  printf ("Usage: %s FILE\n", exeName.c_str ());
  printf ("Calculate the lower and upper limits for each bin of each cut flow, selection,\n");
  printf ("and minus one histogram in the given FILE.\n");
}

void
getLimits (TH1D *cutFlow, TDirectoryFile *dir)
{
  string histName = cutFlow->GetName ();
  TH1D *lowerLimit = (TH1D *) cutFlow->Clone ((histName + "LowerLimit").c_str ()),
       *upperLimit = (TH1D *) cutFlow->Clone ((histName + "UpperLimit").c_str ());

  TAxis *x = cutFlow->GetXaxis ();
  for (int i = 1; i <= x->GetNbins (); i++)
    {
      double events = cutFlow->GetBinContent (i), eventsLowerLimit, eventsUpperLimit;

      // The calculation of upper and lower limits is taken from the PDG Statistics chapter.
      // "Poisson or binomal data", section 37.4.2.3 in 2013 version; Eqns. 37.71a, 37.71b.
      // Here ALPHA is the confidence level, so ALPHA = 0.68 corresponds to a 68% C.L.
      // The PDG uses a different convention:  the C.L is 1 - \alpha.
      // For example, the 68% CL upper limit on an observation of 0 events is:
      // 0.5 * TMath::ChisquareQuantile (0.68, 2) = 1.139
      eventsLowerLimit = 0.5 * TMath::ChisquareQuantile (1 - ALPHA, 2 * events);
      eventsUpperLimit = 0.5 * TMath::ChisquareQuantile (ALPHA, 2 * (events + 1));
      lowerLimit->SetBinContent (i, eventsLowerLimit);
      upperLimit->SetBinContent (i, eventsUpperLimit);
    }
  dir->cd ();
  lowerLimit->Write ((histName + "LowerLimit").c_str ());
  upperLimit->Write ((histName + "UpperLimit").c_str ());
}

void
parseOptions (int argc, char *argv[], map<string, string> &opt, vector<string> &argVector)
{
  for (int i = 1; i < argc; i++)
    {
      if (argv[i][0] != '-')
        {
          argVector.push_back (argv[i]);
          continue;
        }
      int offset = 1;
      if (argv[i][1] == '-')
        offset++;
      string key = argv[i] + offset,
             value = "";
      if (key == "h")
        key = "help";
      if (key == "s")
        key = "signalToBackground";
      if (key == "m")
        key = "marginal";
      if (key == "l")
        key = "luminosity";
      if (key == "luminosity")
        value = argv[i++ + 1];
      opt[key] = value;
    }
}
