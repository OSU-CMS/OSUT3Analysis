#include <iostream>
#include <iomanip>
#include <string>

#include "TFile.h"
#include "TList.h"
#include "TKey.h"
#include "TH1D.h"
#include "TH2D.h"

using namespace std;

TH1D * copy1DHistogram (TH1D * const);
TH2D * copy2DHistogram (TH2D * const);

int
main (int argc, char *argv[])
{
  TH1::SetDefaultSumw2 ();

  TFile *fin = TFile::Open (argv[1]);
  TFile *fout = TFile::Open (argv[2], "recreate");
  TIter next (fin->GetListOfKeys ());
  TKey *key;
  int n = fin->GetListOfKeys ()->GetEntries (), i = 0;
  while ((key = dynamic_cast<TKey *> (next ())))
    {
      string className (key->GetClassName ());
      string name (key->GetName ());
      TObject *obj = fin->Get (name.c_str ());
      cout << "(" << setw (3) << right << (++i) << "/" << setw (3) << right << n << ") recreating \"" << name << "\"..." << endl;
      if (className.find ("TH1") == 0)
        {
          const TH1D * const h = copy1DHistogram ((TH1D *) obj);
          fout->cd ();
          h->Write (name.c_str ());
          delete h;
        }
      else if (className.find ("TH2") == 0)
        {
          const TH2D * const h = copy2DHistogram ((TH2D *) obj);
          fout->cd ();
          h->Write (name.c_str ());
          delete h;
        }
      delete obj;
    }
  fin->Close ();
  fout->Close ();
  delete fin;
  delete fout;

  return 0;
}

TH1D *
copy1DHistogram (TH1D * const h)
{
  string name (h->GetName ());
  h->SetName ((name + "_old").c_str ());

  TH1D *h_new = NULL;
  vector<double> binsX;
  for (int i = 1; i <= h->GetNbinsX () + 1; i++)
    binsX.push_back (h->GetXaxis ()->GetBinLowEdge (i));

  if (h->GetXaxis ()->IsVariableBinSize ())
    h_new = new TH1D (name.c_str (), "",
                      binsX.size () - 1, binsX.data ());
  else
    h_new = new TH1D (name.c_str (), "",
                      h->GetNbinsX (), h->GetXaxis ()->GetBinLowEdge (1), h->GetXaxis ()->GetBinUpEdge (h->GetNbinsX ()));
  h_new->SetDirectory (0);

  h_new->SetTitle (h->GetTitle ());
  h_new->GetXaxis ()->SetTitle (h->GetXaxis ()->GetTitle ());
  h_new->GetYaxis ()->SetTitle (h->GetYaxis ()->GetTitle ());

  for (int i = 0; i <= h->GetNbinsX () + 1; i++)
    {
      h_new->SetBinContent (i, h->GetBinContent (i));
      h_new->SetBinError (i, h->GetBinError (i));
    }

  h_new->SetEntries (h->GetEntries ());

  return h_new;
}

TH2D *
copy2DHistogram (TH2D * const h)
{
  string name (h->GetName ());
  h->SetName ((name + "_old").c_str ());

  TH2D *h_new = NULL;
  vector<double> binsX, binsY;
  for (int i = 1; i <= h->GetNbinsX () + 1; i++)
    binsX.push_back (h->GetXaxis ()->GetBinLowEdge (i));
  for (int i = 1; i <= h->GetNbinsY () + 1; i++)
    binsY.push_back (h->GetYaxis ()->GetBinLowEdge (i));

  if (h->GetXaxis ()->IsVariableBinSize () && h->GetYaxis ()->IsVariableBinSize ())
    h_new = new TH2D (name.c_str (), "",
                      binsX.size () - 1, binsX.data (),
                      binsY.size () - 1, binsY.data ());
  else if (h->GetXaxis ()->IsVariableBinSize () && !h->GetYaxis ()->IsVariableBinSize ())
    h_new = new TH2D (name.c_str (), "",
                      binsX.size () - 1, binsX.data (),
                      h->GetNbinsY (), h->GetYaxis ()->GetBinLowEdge (1), h->GetYaxis ()->GetBinUpEdge (h->GetNbinsY ()));
  else if (!h->GetXaxis ()->IsVariableBinSize () && h->GetYaxis ()->IsVariableBinSize ())
    h_new = new TH2D (name.c_str (), "",
                      h->GetNbinsX (), h->GetXaxis ()->GetBinLowEdge (1), h->GetXaxis ()->GetBinUpEdge (h->GetNbinsX ()),
                      binsY.size () - 1, binsY.data ());
  else if (!h->GetXaxis ()->IsVariableBinSize () && !h->GetYaxis ()->IsVariableBinSize ())
    h_new = new TH2D (name.c_str (), "",
                      h->GetNbinsX (), h->GetXaxis ()->GetBinLowEdge (1), h->GetXaxis ()->GetBinUpEdge (h->GetNbinsX ()),
                      h->GetNbinsY (), h->GetYaxis ()->GetBinLowEdge (1), h->GetYaxis ()->GetBinUpEdge (h->GetNbinsY ()));
  h_new->SetDirectory (0);

  h_new->SetTitle (h->GetTitle ());
  h_new->GetXaxis ()->SetTitle (h->GetXaxis ()->GetTitle ());
  h_new->GetYaxis ()->SetTitle (h->GetYaxis ()->GetTitle ());

  for (int i = 0; i <= h->GetNbinsX () + 1; i++)
    for (int j = 0; j <= h->GetNbinsY () + 1; j++)
      {
        h_new->SetBinContent (i, j, h->GetBinContent (i, j));
        h_new->SetBinError (i, j, h->GetBinError (i, j));
      }

  h_new->SetEntries (h->GetEntries ());

  return h_new;
}
