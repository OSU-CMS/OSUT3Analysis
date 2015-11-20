#include <TFile.h>
#include <TROOT.h>
#include <TKey.h>
#include <TH1.h>
#include <TH2.h>
#include <TDirectory.h>
#include <TList.h>
#include <TMath.h>
#include <boost/tokenizer.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <cstdlib>

using namespace boost::program_options;
using namespace boost;
using namespace std;

void make(TDirectory & out, TObject * o);
void fill(TDirectory & out, TObject * o, double);
double normCDF (const double);
void generateUpperLimitCutFlow (TDirectoryFile &, TH1D * const, const double);
void upperLimitCutFlow (TDirectoryFile &, const double);

static const char * const kHelpOpt = "help";
static const char * const kHelpCommandOpt = "help,h";
static const char * const kOutputFileOpt = "output-file";
static const char * const kOutputFileCommandOpt = "output-file,o";
static const char * const kInputFilesOpt = "input-files";
static const char * const kInputFilesCommandOpt = "input-files,i";
static const char * const kWeightsOpt = "weights";
static const char * const kWeightsCommandOpt = "weights,w";

vector<double> weights;

int main(int argc, char * argv[]) {
  string programName(argv[0]);
  string descString(programName);
  descString += " [options] ";
  descString += "data_file \nAllowed options";
  options_description desc(descString);

  desc.add_options()
    (kHelpCommandOpt, "produce help message")
    (kOutputFileCommandOpt, value<string>()->default_value("out.root"), "output root file")
    (kWeightsCommandOpt, value<string>(), "list of weights (comma separates).\ndefault: weights are assumed to be 1")
    (kInputFilesCommandOpt, value<vector<string> >()->multitoken(), "input root files");

  positional_options_description p;

  variables_map vm;
  try {
    store(command_line_parser(argc,argv).options(desc).positional(p).run(), vm);
    notify(vm);
  } catch(const error&) {
    cerr << "invalid arguments. usage:" << endl;
    cerr << desc <<std::endl;
    return -1;
  }

  if(vm.count(kHelpOpt)) {
    cout << desc <<std::endl;
    return 0;
  }

  vector<string> fileNames;
  if(vm.count(kInputFilesOpt)) {
    fileNames = vm[kInputFilesOpt].as<vector<string> >();
  } else {
    cerr << "option -i must be specifyed" << endl;
    return -1;
  }

  if(fileNames.size()==0) {
    cerr << "at least one file name must be specified with option -i" <<endl;
    return -1;
  }

  string outputFile;
  if(vm.count(kOutputFileOpt)) {
    outputFile = vm[kOutputFileOpt].as<string>();
  } else {
    cerr << "option -o must be specifyed" << endl;
    return -1;
  }

  if(vm.count(kWeightsOpt)) {
    string w = vm[kWeightsOpt].as<string>();
    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tokens(w, sep);
    for(tokenizer<char_separator<char> >::iterator t = tokens.begin(); t != tokens.end();++t) {
      const char * begin = t->c_str();
      char * end;
      double w = strtod(begin, &end);
      size_t s = end - begin;
      if(s < t->size()) {
        cerr << "invalid weight: " << begin << endl;
        exit(1);
      }
      weights.push_back(w);
    }
  } else {
    weights = vector<double>(fileNames.size(), 1.0);
  }
  if(weights.size() != fileNames.size()) {
    cerr << "the number of weights and the number of files must be the same" << endl;
    exit(-1);
  }

  gROOT->SetBatch();

  TFile out(outputFile.c_str(), "RECREATE");
  if(!out.IsOpen()) {
    cerr << "can't open output file: " << outputFile <<endl;
    return -1;
  }

  for(size_t i = 0; i < fileNames.size(); ++i) {
    string fileName = fileNames[i];
    TFile file(fileName.c_str(), "read");
    if(!file.IsOpen()) {
      cerr << "can't open input file: " << fileName <<endl;
      return -1;
    }

    TIter next(file.GetListOfKeys());
    TKey *key;
    while ((key = dynamic_cast<TKey*>(next()))) {
      string className(key->GetClassName());
      string name(key->GetName());
      TObject * obj = file.Get(name.c_str());
      if(obj == 0) {
        cerr <<"error: key " << name << " not found in file " << fileName << endl;
        return -1;
      }
      make(out, obj);
    }
    file.Close();
  }
  for(size_t i = 0; i < fileNames.size(); ++i) {
    string fileName = fileNames[i];
    TFile file(fileName.c_str(), "read");
    if(!file.IsOpen()) {
      cerr << "can't open input file: " << fileName <<endl;
      return -1;
    }

    TIter next(file.GetListOfKeys());
    TKey *key;
    while ((key = dynamic_cast<TKey*>(next()))) {
      string className(key->GetClassName());
      string name(key->GetName());
      TObject * obj = file.Get(name.c_str());
      if(obj == 0) {
        cerr <<"error: key " << name << " not found in file " << fileName << endl;
        return -1;
      }
      fill(out, obj, weights[i]);
    }
    file.Close();
  }

  out.Write();
  out.Close();

  TFile fout (outputFile.c_str(), "UPDATE");
  upperLimitCutFlow (fout, weights[0]);
  fout.Write();
  fout.Close();

  return 0;
}

void make(TDirectory & out, TObject * o) {
  TDirectory * dir = dynamic_cast<TDirectory*>(o);
  bool exists = out.Get (o->GetName ());
  TH1F * th1f;
  TH1D * th1d;
  TH2F * th2f;
  TH2D * th2d;
  out.cd();
  if (!dir && exists)
    return;
  if(dir != 0) {
    TDirectory * outDir;
    if (!exists)
      outDir = out.mkdir(dir->GetName(), dir->GetTitle());
    else
      outDir = (TDirectory *) out.Get(dir->GetName());
    TIter next(dir->GetListOfKeys());
    TKey *key;
    while( (key = dynamic_cast<TKey*>(next())) ) {
      string className(key->GetClassName());
      string name(key->GetName());
      TObject * obj = dir->Get(name.c_str());
      if(obj == 0) {
        cerr <<"error: key " << name << " not found in directory " << dir->GetName() << endl;
        exit(-1);
      }
      make(*outDir, obj);
    }
  } else if((th1f = dynamic_cast<TH1F*>(o)) != 0) {
    TH1F *h = (TH1F*) th1f->Clone();
    h->Reset();
    h->Sumw2();
    h->SetDirectory(&out);
  } else if((th1d = dynamic_cast<TH1D*>(o)) != 0) {
    TH1D *h = (TH1D*) th1d->Clone();
    h->Reset();
    h->Sumw2();
    h->SetDirectory(&out);
  } else if((th2f = dynamic_cast<TH2F*>(o)) != 0) {
    TH2F *h = (TH2F*) th2f->Clone();
    h->Reset();
    h->Sumw2();
    h->SetDirectory(&out);
  } else if((th2d = dynamic_cast<TH2D*>(o)) != 0) {
    TH2D *h = (TH2D*) th2d->Clone();
    h->Reset();
    h->Sumw2();
    h->SetDirectory(&out);
  }
}

void fill(TDirectory & out, TObject * o, double w) {
  TDirectory * dir;
  TH1F * th1f;
  TH1D * th1d;
  TH2F * th2f;
  TH2D * th2d;
  if((dir  = dynamic_cast<TDirectory*>(o)) != 0) {
    const char * name = dir->GetName();
    TDirectory * outDir = dynamic_cast<TDirectory*>(out.Get(name));
    if(outDir == 0) {
      cerr << "can't find directory " << name << " in output file" << endl;
      exit(-1);
    }
    TIter next(dir->GetListOfKeys());
    TKey *key;
    while( (key = dynamic_cast<TKey*>(next())) ) {
      string className(key->GetClassName());
      string name(key->GetName());
      TObject * obj = dir->Get(name.c_str());
      if(obj == 0) {
        cerr <<"error: key " << name << " not found in directory " << dir->GetName() << endl;
        exit(-1);
      }
      fill(*outDir, obj, w);
    }
  } else if((th1f = dynamic_cast<TH1F*>(o)) != 0) {
    const char * name = th1f->GetName();
    TH1F * outTh1f = dynamic_cast<TH1F*>(out.Get(name));
    if(outTh1f == 0) {
      cerr <<"error: histogram TH1F" << name << " not found in directory " << out.GetName() << endl;
      exit(-1);
    }
    th1f->Scale(w);

    TList *list = new TList();
    list->Add(th1f);
    outTh1f->Merge(list);
  } else if((th1d = dynamic_cast<TH1D*>(o)) != 0) {
    const char * name = th1d->GetName();
    TH1D * outTh1d = dynamic_cast<TH1D*>(out.Get(name));
    if(outTh1d == 0) {
      cerr <<"error: histogram TH1D" << name << " not found in directory " << out.GetName() << endl;
      exit(-1);
    }
    th1d->Scale(w);

    TList *list = new TList();
    list->Add(th1d);
    outTh1d->Merge(list);
  } else if((th2f = dynamic_cast<TH2F*>(o)) != 0) {
    const char * name = th2f->GetName();
    TH2F * outTh2f = dynamic_cast<TH2F*>(out.Get(name));
    if(outTh2f == 0) {
      cerr <<"error: histogram TH2F" << name << " not found in directory " << out.GetName() << endl;
      exit(-1);
    }
    th2f->Scale(w);

    TList *list = new TList();
    list->Add(th2f);
    outTh2f->Merge(list);
  } else if((th2d = dynamic_cast<TH2D*>(o)) != 0) {
    const char * name = th2d->GetName();
    TH2D * outTh2d = dynamic_cast<TH2D*>(out.Get(name));
    if(outTh2d == 0) {
      cerr <<"error: histogram TH2D" << name << " not found in directory " << out.GetName() << endl;
      exit(-1);
    }
    th2d->Scale(w);

    TList *list = new TList();
    list->Add(th2d);
    outTh2d->Merge(list);
  }
}

double
normCDF (const double x)
{
  return 0.5 * (1.0 + TMath::Erf (x / TMath::Sqrt2 ()));
}

void
generateUpperLimitCutFlow (TDirectoryFile &dirFile, TH1D * const cutFlow, const double w)
{
  vector<double> sigmas = {1.0, 2.0, 3.0};
  cutFlow->SetDirectory (0);
  for (const auto &sigma : sigmas)
    {
      double cl = 1.0 - 2.0 * normCDF (-sigma);
      stringstream ss;
      ss << ((int) (cl * 100.0));

      TH1D * const upperLimitCutFlowHist = (TH1D *) cutFlow->Clone (("cutFlow_" + ss.str () + "CL").c_str ());
      upperLimitCutFlowHist->SetDirectory (&dirFile);
      for (int i = 0; i <= cutFlow->GetXaxis ()->GetNbins () + 1; i++)
        {
          // The calculation of upper and lower limits is taken from the PDG Statistics chapter.  
          // "Poisson or binomal data", section 37.4.2.3 in 2013 version; Eqns. 37.71a, 37.71b.  
          // Here cl is the confidence level, so cl = 0.68 corresponds to a 68% C.L.
          // For example, the 68% CL upper limit on an observation of 0 events is:
          // 0.5 * TMath::ChisquareQuantile (0.68, 2) = 1.139  
          double content = cutFlow->GetBinContent (i) / w, upperLimit;
          upperLimit = 0.5 * TMath::ChisquareQuantile (cl, 2 * (content + 1));
          upperLimitCutFlowHist->SetBinContent (i, upperLimit * w);
        }
    }
}

void
upperLimitCutFlow (TDirectoryFile &dirFile, const double w)
{
  TIter next (dirFile.GetListOfKeys ());
  TKey *key;
  while ((key = (TKey *) next ()))
    {
      if (string (key->GetClassName ()) == "TDirectoryFile")
        upperLimitCutFlow (*((TDirectoryFile *) dirFile.Get (key->GetName ())), w);
      if (string (key->GetClassName ()) == "TH1D" && string (key->GetName ()) == "cutFlow")
        generateUpperLimitCutFlow (dirFile, (TH1D *) dirFile.Get (key->GetName ()), w);
    }
}
