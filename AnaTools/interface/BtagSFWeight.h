#include <cmath>
#include <iostream>
#include <vector>
using namespace std;
class BtagSFWeight {
 public:
  bool filter(int t, int minTags);
  double weight(vector<double> jets, int useMinTags);
  double sflookup(double jetCSV, double pt, double flavor, double jetEta);
};
