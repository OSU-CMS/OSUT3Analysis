#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "TFile.h"
#include "TDirectoryFile.h"
#include "TTree.h"
#include "TKey.h"
#include "TObject.h"

using namespace std;

void weightTrees (TDirectoryFile *, const double, vector<TTree *> &);
void printHelp (const string &);
void parseOptions (int, char *[], map<string, string> &, vector<string> &);

int
main (int argc, char *argv[])
{
  map<string, string> opt;
  vector<string> argVector;
  parseOptions (argc, argv, opt, argVector);
  if (argVector.size () != 2 || opt.count ("help"))
    {
      printHelp (argv[0]);
      return 0;
    }

  TFile *fin = TFile::Open (argVector.at (0).c_str (), "update");
  vector<TTree *> trees;
  weightTrees (fin, atof (argVector.at (1).c_str ()), trees);
  fin->Write (0, TObject::kOverwrite);
  fin->Close ();
}

void
weightTrees (TDirectoryFile *fin, const double w, vector<TTree *> &trees)
{
  TIter next (fin->GetListOfKeys ());
  TKey *key;
  while ((key = (TKey *) next ()))
    {
      if (string (key->GetClassName ()) == "TDirectoryFile")
        {
          TDirectoryFile *dir = (TDirectoryFile *) fin->Get (key->GetName ());
          weightTrees (dir, w, trees);
        }
      else if (string (key->GetClassName ()) == "TTree")
        {
          TTree *tree = (TTree *) fin->Get (key->GetName ());
          tree->SetWeight (w);
          trees.push_back (tree);
        }
    }
}

void
printHelp (const string &exeName)
{
  printf ("Usage: %s FILE WEIGHT\n", exeName.c_str ());
  printf ("Weights each TTree in FILE with WEIGHT.\n");
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
      opt[key] = value;
    }
}
