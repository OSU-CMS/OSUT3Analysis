#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
#include <map>
#include <vector>
#include <cmath>
#include <fstream>

#include "TFile.h"
#include "TDirectoryFile.h"
#include "TKey.h"
#include "TH1D.h"
#include "TAxis.h"
#include "TString.h"

#define BIG_INT (1.0e6)

using namespace std;

string bigInt (double, string option = "");
string bigEff (double);
void printHelp (const string &exeName);
void parseOptions (int, char **, map<string, string> &, vector<string> &);
void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace);
int myprecision_ = 3;  // default value


int
main (int argc, char *argv[])
{
  map<string, string> opt;
  vector<string> argVector;
  parseOptions (argc, argv, opt, argVector);
  if (argVector.size () % 3 || opt.count ("help"))
    {
      printHelp (argv[0]);
      return 0;
    }
  bool sb = false;
  string sbLabel = "";
  if ((opt.count ("diff") || opt.count ("ratio") || opt.count ("signalToBackground") || opt.count ("totalBkgd") )) {
    if (argVector.size () <= 3) {
      cerr << "ERROR[cutFlowTable.cpp]:  When using options diff, ratio, signalToBackground, or totalBkgd, more than one dataset must be specified." << endl;
      return 0;
    }
    sb = true;
  }
  if (opt.count ("precision")) {
    myprecision_ = atoi(opt.at("precision").c_str());
    cerr << "Will use precision of " << myprecision_ << endl;
  }

  map<string, double> xsecs;
  if (opt.count ("xsecTheory")) {
    ifstream xsecFile(opt.at("xsecTheory").c_str());
    if (!xsecFile) { cerr << "Error: file " << opt.at("xsecTheory") << " could not be opened." << endl; return 0; }
    cerr << "Will use cross sections from " << opt.at("xsecTheory") << endl;
    string sample;
    double xsec;
    while (!xsecFile.eof()) {
      xsecFile >> sample >> xsec;
      xsecs[sample] = xsec;
    }
  }
  if (opt.count ("diff"))
    sbLabel = opt.at ("diff");
  if (opt.count ("ratio"))
    sbLabel = opt.at ("ratio");
  if (opt.count ("signalToBackground"))
    sbLabel = opt.at ("signalToBackground");
  if (opt.count ("totalBkgd"))
    sbLabel = opt.at ("totalBkgd");
  vector<vector<string> > table;
  vector<vector<string> > effTable;
  vector<vector<string> > marginalEffTable;
  vector<double> numerator, denominator, numeratorError2, denominatorError2;
  vector<string> xlabels, ylabels;
  for (unsigned i = 0; i < argVector.size () / 3; i++)
    {
      string fileName = argVector.at (i * 3),
             histName = argVector.at (1 + i * 3);
      xlabels.push_back (argVector.at (2 + i * 3));
      string fileToOpen = fileName;
      if (sb && (fileName[0] == '<' || fileName[0] == '>'))
        fileToOpen = fileName.substr (1, fileName.size () - 1);
      TFile *fin;
      if (!(fin = TFile::Open (fileToOpen.c_str ())))
        {
          cerr << "Failed to open " << fileToOpen << "!" << endl;
          return 0;
        }
      TIter next0 (fin->GetListOfKeys ());
      TObject *obj0;
      TH1D *cutFlow = 0; //, *upperLimit = 0;

      double yieldTheory = -99;
      double xsec = -99;
      if (opt.count ("xsecTheory")) {
        TString fileNameStr(fileName);
        for (map<string, double>::iterator it = xsecs.begin(); it != xsecs.end(); it++) {
          TString sample = TString(it->first);
          if (fileNameStr.Contains(sample.Data())) {
            xsec = it->second;
            yieldTheory = xsec * atof (opt["luminosity"].c_str ());
            break;
          }
        }
        cerr << "Found for fileName: " << fileName << ": xsec = " << xsec << ", yieldTheory = " << yieldTheory << endl;
      }

      while (!cutFlow && (obj0 = next0 ()))
        {
          string obj0Class = ((TKey *) obj0)->GetClassName (),
                 obj0Name = obj0->GetName ();

          if (obj0Class == "TDirectoryFile")
            {
              TDirectoryFile *dir = (TDirectoryFile *) fin->Get (obj0Name.c_str ());
              TIter next1 (dir->GetListOfKeys ());
              TObject *obj1;
              while (!cutFlow && (obj1 = next1 ()))
                {
                  string obj1Class = ((TKey *) obj1)->GetClassName (),
                         obj1Name = obj1->GetName ();

                  TString objFullName = obj0->GetName();
                  objFullName += TString("/") + TString(obj1->GetName());

                  TString histNameStr = histName;

                  if (obj1Class == "TH1D" && objFullName == histNameStr)
                    {
                      cutFlow = (TH1D *) dir->Get (obj1Name.c_str ());
                      if (!cutFlow) cerr << "Problem accessing cutFlow" << endl;
                      //                      upperLimit = (TH1D *) dir->Get ((obj1Name + "UpperLimit").c_str ());
                    }
                }
            }
        }
      if (!cutFlow)
        {
          cerr << "Did not find a histogram named " << histName << " in " << fileToOpen << "!" << endl;
          return 0;
        }
//       if (!upperLimit)
//         {
//           cerr << "Did not find a histogram named " << (histName + "UpperLimit") << " in " << fileToOpen << "!" << endl;
//           return 0;
//         }
      cutFlow->SetDirectory (0);
      //      upperLimit->SetDirectory (0);
      fin->Close ();

      TAxis *x = cutFlow->GetXaxis ();
      table.push_back (vector<string> ());
      effTable.push_back (vector<string> ());
      marginalEffTable.push_back (vector<string> ());
      for (unsigned j = 1; j <= (unsigned) x->GetNbins (); j++)
        {
          double binContent = cutFlow->GetBinContent(j);
          //          double binUpperLimit = upperLimit->GetBinContent (j);
          double binUpperLimit = cutFlow->GetBinContent(j) + cutFlow->GetBinError(j);
          double binError = cutFlow->GetBinError(j);
          if (j==1 && opt.count ("xsecTheory") && yieldTheory > 0.0) {
            binContent = yieldTheory;
          }

          string tableContent;
          if (binContent - binError > 0)
            {
              if (binContent >= BIG_INT)
                {
                  if (opt.count ("noErrors")) {
                    tableContent = bigInt (binContent, "big") + " ";
                  } else {
                    tableContent = "(" + bigInt (binContent, "big") + " \\pm ";
                    tableContent += bigInt (binError, "error") + ") ";
                  }
                  tableContent += bigInt (0.0, "exponent");
                }
              else
                if (opt.count ("noErrors")) {
                  tableContent = bigInt (binContent);
                } else {
                  tableContent = bigInt (binContent) + " \\pm " + bigInt (binError);
                }
            }
          else
            {
              if (binUpperLimit >= BIG_INT)
                {
                  tableContent = "< " + bigInt (binUpperLimit, "big");
                  tableContent += bigInt (0.0, "exponent");
                }
              else
                tableContent = "< " + bigInt (binUpperLimit);
            }
          table.back ().push_back (tableContent);

          effTable.back ().push_back (bigEff (100.0 * (cutFlow->GetBinContent (j) / (double) cutFlow->GetBinContent (1))));
          if (j == 1)
            marginalEffTable.back ().push_back (bigEff (100.0));
          else
            marginalEffTable.back ().push_back (bigEff (100.0 * (cutFlow->GetBinContent (j) / (double) cutFlow->GetBinContent (j - 1))));
          if (i == 0)
            {
              string newlabel = x->GetBinLabel (j);
              ReplaceStringInPlace(newlabel, "&", "\\&");  // treat this as a special case, so that other instances of "&" in the tex file are not affected
              ylabels.push_back (newlabel);
            }
          if (sb && fileName[0] == '<')
            {
              if (numerator.size () >= j)
                {
                  numerator.at (j - 1) += cutFlow->GetBinContent (j);
                  numeratorError2.at (j - 1) += cutFlow->GetBinError (j) * cutFlow->GetBinError (j);
                }
              else
                {
                  numerator.push_back (cutFlow->GetBinContent (j));
                  numeratorError2.push_back (cutFlow->GetBinError (j) * cutFlow->GetBinError (j));
                }
            }
          if (sb && fileName[0] == '>')
            {
              if (denominator.size () >= j)
                {
                  denominator.at (j - 1) += cutFlow->GetBinContent (j);
                  denominatorError2.at (j - 1) += cutFlow->GetBinError (j) * cutFlow->GetBinError (j);
                }
              else
                {
                  denominator.push_back (cutFlow->GetBinContent (j));
                  denominatorError2.push_back (cutFlow->GetBinError (j) * cutFlow->GetBinError (j));
                }
            }
        }
    }

  numerator.resize (ylabels.size (), 0.0);
  denominator.resize (ylabels.size (), 0.0);
  numeratorError2.resize (ylabels.size (), 0.0);
  denominatorError2.resize (ylabels.size (), 0.0);
  vector<string> extraColumn;
  for (unsigned i = 0; sb && i < numerator.size (); i++)
    {
      double x = numerator.at (i), y = denominator.at (i),
             dx = sqrt (numeratorError2.at (i)), dy = sqrt (denominatorError2.at (i)),
             binContent = 0.0, binError = 0.0;
      string columnContent;

      if (opt.count ("diff"))
        {
          binContent = x - y;
          binError = sqrt (dx * dx + dy * dy);
        }
      else if (opt.count ("ratio"))
        {
          binContent = (x - y) / y;
          binError = sqrt (x * x * dy * dy + y * y * dx * dx) / (y * y);
        }
      else if (opt.count ("totalBkgd"))
        {
          binContent = y;
          binError = dy;
        }
      else if (opt.count ("signalToBackground"))
        {
          binContent = x / sqrt (x + y);
          // Calculate error with standard error propagation:
          // F = x / sqrt (x + y)
          // errF = sqrt ( (dF/dx)^2 * dx^2 + (dF/dy)^2 * dy^2 )
          // A lot of algebra gives the expression below.
          binError = 4.0 * y * y * dx * dx + 4 * x * y * dx * dx + x * x * dy * dy + x * x * dx * dx;
          binError /= 4 * (x + y) * (x + y) * (x + y);
          binError = sqrt (binError);
        }

      if (::isnan (binContent ))
        columnContent = "\\mathrm{nan}";
      else if (::isinf (binContent))
        {
          if (binContent > 0)
            columnContent = "+\\infty";
          else
            columnContent = "-\\infty";
        }
      else if (fabs (binContent) >= BIG_INT)
        {
          if (opt.count ("noErrors")) {
            columnContent = bigInt (binContent, "big") + " ";
          } else {
            columnContent = "(" + bigInt (binContent, "big") + " \\pm ";
            columnContent += bigInt (binError, "error") + ") ";
          }
            columnContent += bigInt (0.0, "exponent");
        }
      else {
        if (opt.count ("noErrors")) {
          columnContent = bigInt (binContent);
        } else {
          columnContent = bigInt (binContent) + " \\pm " + bigInt (binError);
        }
      }
      extraColumn.push_back (columnContent);
    }

  cout << "\\begin{table}[htbp]  \\begin{center} \\begin{tabular}{";
  if (sb)
    {
      for (unsigned i = 0; i < xlabels.size () + 2; i++)
        cout << "l";
      cout << "}" << endl;
      if (opt.count ("luminosity"))
        cout << "$\\mathrm{L} = " << atof (opt["luminosity"].c_str ()) / 1000.0 << "\\,\\mathrm{fb}^{-1}$ & \\multicolumn{" << xlabels.size () + 1 << "}{r}{Event yield} \\\\" << endl;
      cout << "\\hline" << endl;
      for (unsigned i = 0; i < xlabels.size (); i++)
        cout << "& " << xlabels.at (i) << " ";
      cout << "& " << sbLabel << " ";
      cout << "\\\\" << endl;
      cout << "\\hline" << endl;
      for (unsigned i = 0; i < table.at (0).size (); i++)
        {
          cout << ylabels.at (i) << " &";
          for (unsigned j = 0; j < table.size (); j++)
            cout << " $" << table.at (j).at (i) << "$ &";
          cout << " $" << extraColumn.at (i) << "$ \\\\" << endl;
        }
      cout << "\\hline" << endl;
      cout << "\\end{tabular} \\end{center} \\end{table}" << endl << endl;
    }
  else
    {

      for (unsigned i = 0; i < xlabels.size () + 1; i++)
        cout << "l";
      cout << "}" << endl;
      if (opt.count ("luminosity"))
        cout << "$\\mathrm{L} = " << atof (opt["luminosity"].c_str ()) / 1000.0 << "\\,\\mathrm{fb}^{-1}$ & \\multicolumn{" << xlabels.size () << "}{r}{Event yield} \\\\" << endl;
      cout << "\\hline" << endl;
      cout << "&";
      for (unsigned i = 0; i < xlabels.size () - 1; i++)
        cout << " " << xlabels.at (i) << " &";
      cout << " " << xlabels.back () << " \\\\" << endl;
      cout << "\\hline" << endl;
      for (unsigned i = 0; i < table.at (0).size (); i++)
        {
          cout << ylabels.at (i) << " &";
          for (unsigned j = 0; j < table.size () - 1; j++)
            cout << " $" << table.at (j).at (i) << "$ &";
          cout << " $" << table.back ().at (i) << "$ \\\\" << endl;
        }
      cout << "\\hline" << endl;
      cout << "\\end{tabular} \\end{center} \\end{table}" << endl << endl;
    }
  cout << "\\begin{table}[htbp]  \\begin{center} \\begin{tabular}{";
  for (unsigned i = 0; i < xlabels.size () + 1; i++)
    cout << "l";
  cout << "}" << endl;
  if (opt.count ("luminosity"))
    cout << "$\\mathrm{L} = " << atof (opt["luminosity"].c_str ()) / 1000.0 << "\\,\\mathrm{fb}^{-1}$ & \\multicolumn{" << xlabels.size () << "}{r}{Efficiency} \\\\" << endl;
  cout << "\\hline" << endl;
  cout << "&";
  for (unsigned i = 0; i < xlabels.size () - 1; i++)
    cout << " " << xlabels.at (i) << " &";
  cout << " " << xlabels.back () << " \\\\" << endl;
  cout << "\\hline" << endl;
  for (unsigned i = 0; i < effTable.at (0).size (); i++)
    {
      cout << ylabels.at (i) << " &";
      for (unsigned j = 0; j < effTable.size () - 1; j++)
        cout << " $" << effTable.at (j).at (i) << "\\%$ &";
      cout << " $" << effTable.back ().at (i) << "\\%$ \\\\" << endl;
    }
  cout << "\\hline" << endl;
  cout << "\\end{tabular} \\end{center} \\end{table}" << endl << endl;

  if (opt.count ("marginal"))
    {
      cout << "\\begin{table}[htbp]  \\begin{center} \\begin{tabular}{";
      for (unsigned i = 0; i < xlabels.size () + 1; i++)
        cout << "l";
      cout << "}" << endl;
      if (opt.count ("luminosity"))
        cout << "$\\mathrm{L} = " << atof (opt["luminosity"].c_str ()) / 1000.0 << "\\,\\mathrm{fb}^{-1}$ & \\multicolumn{" << xlabels.size () << "}{r}{Marginal efficiency} \\\\" << endl;
      cout << "\\hline" << endl;
      cout << "&";
      for (unsigned i = 0; i < xlabels.size () - 1; i++)
        cout << " " << xlabels.at (i) << " &";
      cout << " " << xlabels.back () << " \\\\" << endl;
      cout << "\\hline" << endl;
      for (unsigned i = 0; i < marginalEffTable.at (0).size (); i++)
        {
          cout << ylabels.at (i) << " &";
          for (unsigned j = 0; j < marginalEffTable.size () - 1; j++)
            cout << " $" << marginalEffTable.at (j).at (i) << "\\%$ &";
          cout << " $" << marginalEffTable.back ().at (i) << "\\%$ \\\\" << endl;
        }
      cout << "\\hline" << endl;
      cout << "\\end{tabular} \\end{center} \\end{table}" << endl << endl;
    }

  return 0;
}

string
bigInt (double n, string option)
{
  stringstream ss;
  string num, numDecimals;
  unsigned len;
  static unsigned exponent = 0;
  bool decimals = false;

  if (option == "error")
    n /= pow (10.0, exponent);
  else if (option == "exponent")
    {
      ss << exponent;
      return "\\times 10^{" + ss.str () + "}";
    }
  else if (option == "big")
    {
      exponent = 0;
      for (; fabs (n) > 1.0; n /= 10.0, exponent++);
      n *= 10.0;
      exponent--;
    }
  else
    exponent = 0;

  if (fabs (n) < 100.0)
    decimals = true;

  if (decimals)
    {
      if (option == "error")
        ss << fixed << setprecision (myprecision_) << n;
      else
        ss << setprecision (myprecision_) << n;
    }
  else
    ss << (long) (n + 0.5 * (n / fabs (n)));
  num = ss.str ();
  numDecimals = ss.str ();
  if (num.find ('.') != string::npos)
    num = num.substr (0, num.find ('.'));
  string sign = "";
  if (n < 0)
    {
      sign = "-";
      num = num.substr (1);
    }
  len = num.length ();
  for (unsigned i = 1; i * 3 < len; i++)
    {
      string left, right;

      left = num.substr (0, len - i * 3);
      right = num.substr (len - i * 3);
      num = left + "\\;" + right;
    }
  num = sign + num;
  if (decimals && numDecimals.find ('.') != string::npos)
    {
      numDecimals = numDecimals.substr (numDecimals.find ('.') + 1, numDecimals.length () - numDecimals.find ('.') - 1);
      num += "." + numDecimals;
    }
  if (num.find ("e") != string::npos)
    {
      string before = num.substr (0, num.find ("e")),
             after = num.substr (num.find ("e") + 1);

      ss.str ("");
      ss << atoi (after.c_str ());
      num = before + " \\times 10^{" + ss.str () + "}";
    }

  return num;
}

string
bigEff (double n)
{
  stringstream ss;
  string num;

  if (::isnan (n) || ::isinf (n))
    n = 0.0;
  ss << setprecision (myprecision_) << n;
  num = ss.str ();
  if (num.find ("e") != string::npos)
    {
      string before = num.substr (0, num.find ("e")),
             after = num.substr (num.find ("e") + 1);

      ss.str ("");
      ss << atoi (after.c_str ());
      num = before + " \\times 10^{" + ss.str () + "}";
    }

  return num;
}

void
printHelp (const string &exeName)
{
  printf ("Usage: %s [OPTION]... FILE HIST LABEL [FILE HIST LABEL...]\n", exeName.c_str ());
  printf ("Prints a cutflow table in LaTeX format from the histogram named HIST in the\n");
  printf ("given ROOT file. If there are multiple triplets of FILE HIST LABEL, then\n");
  printf ("multiple columns are created, each labeled with LABEL.\n");
  printf ("\n");
  printf ("%-29s%s\n", "  -d, --diff LABEL", "add a column for X-Y");
  printf ("%-29s%s\n", "  -h, --help", "print this help message");
  printf ("%-29s%s\n", "  -l, --luminosity LUMI", "integrated luminosity in inverse picobarns");
  printf ("%-29s%s\n", "  -m, --marginal", "include a table of marginal efficiencies");
  printf ("%-29s%s\n", "  -r, --ratio LABEL", "add a column for (X-Y)/Y");
  printf ("%-29s%s\n", "  -p, --precision VALUE", "set the precision (integer) of table values");
  printf ("%-29s%s\n", "  -t, --total LABEL", "add a column for Y");
  printf ("%-29s%s\n", "  -s, --sToB LABEL", "add a column for X/sqrt(X+Y)");
  printf ("%-29s%s\n", "  -e, --noErrors", "do not print errors");
  printf ("\n");
  printf ("For the \"-d\", \"-r\", and \"-s\" options, X is defined as the sum of those columns\n");
  printf ("whose FILE is prefixed with \"<\". Likewise, Y is defined by prefixing the FILE\n");
  printf ("with \">\". The argument to each of these options is used as the title of the\n");
  printf ("column which is added to the table.\n");
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
        {
          key = "signalToBackground";
          opt.erase ("diff");
          opt.erase ("ratio");
          opt.erase ("totalBkgd");
        }
      if (key == "m")
        key = "marginal";
      if (key == "l")
        key = "luminosity";
      if (key == "e")
        key = "noErrors";
      if (key == "d")
        {
          key = "diff";
          opt.erase ("ratio");
          opt.erase ("signalToBackground");
          opt.erase ("totalBkgd");
        }
      if (key == "r")
        {
          key = "ratio";
          opt.erase ("diff");
          opt.erase ("signalToBackground");
          opt.erase ("totalBkgd");
        }
      if (key == "p")
        {
          key = "precision";
        }
      if (key == "x")
        {
          key = "xsecTheory";
        }
      if (key == "t")
        {
          key = "totalBkgd";
          opt.erase ("diff");
          opt.erase ("signalToBackground");
          opt.erase ("ratio");
        }
      if (key == "luminosity" || key == "signalToBackground" || key == "diff" || key == "ratio" || key == "totalBkgd")
        value = argv[i++ + 1];
      if (key == "precision" || key == "xsecTheory")
        value = argv[i++ + 1];
      opt[key] = value;
    }
}


// Copy from
// http://stackoverflow.com/questions/3418231/c-replace-part-of-a-string-with-another-string
void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}
