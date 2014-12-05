#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

vector<string>
getSingleObjects (string inputLabel)
{
  vector<string> singleObjects;
  size_t hyphen;
  while ((hyphen = inputLabel.find ('-')) != string::npos)
    {
      singleObjects.push_back (plural (inputLabel.substr (0, hyphen)));
      inputLabel = inputLabel.substr (hyphen + 1);
    }
  singleObjects.push_back (plural (inputLabel));
  sort (singleObjects.begin (), singleObjects.end ());

  return singleObjects;
}

string
concatenateInputCollection (const vector<string> &inputCollections)
{
  string catInputCollection = "";
  for (auto collection = inputCollections.begin (); collection != inputCollections.end (); collection++)
    {
      if (collection != inputCollections.begin ())
        catInputCollection += "-";
      catInputCollection += singular (*collection);
    }
  return plural (catInputCollection);
}

string
capitalize (string input)
{
  input.front () = toupper (input.front ());
  return input;
}

string
singular (string input)
{
  if (tolower (input.back ()) == 's')
    return input.substr (0, input.size () - 1);
  else
    return input;
}

string
plural (string input)
{
  if (tolower (input.back ()) == 's')
    return input;
  else
    return input + "s";
}

string &
ltrim (string &s)
{
  return s.erase (0, s.find_first_not_of (" \t\f\n\r"));
}

string &
rtrim (string &s)
{
  return s.erase (s.find_last_not_of (" \t\f\n\r") + 1);
}

string &
trim (string &s)
{
  return ltrim (rtrim (s));
}

bool
firstOfPairAscending (pair<size_t, string> a, pair<size_t, string> b)
{
  return (a.first < b.first);
}

bool
collectionIndexAscending (pair<string, tuple<unsigned, unsigned, void *> > a, pair<string, tuple<unsigned, unsigned, void *> > b)
{
  return (get<0> (a.second) < get<0> (b.second));
}
